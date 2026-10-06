#include "arcade/arcade_stage.h"
#include "arcade/arcade_cg.h"
#include "arcade/byte_buffer.h"
#include "arcade/rom/cps3_dma.h"
#include "port/io/cache.h"
#include "sf33rd/Source/Game/rendering/texgroup_data.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

#include <zlib.h>

/// Offset of SIMM5 in the graphics region
#define SIMM5_OFFSET 0x2000000

/// Palette rows copied from the graphics region
typedef struct PaletteSource {
    RomGame game;
    Uint32 offset;
    int rows;
    size_t row_stride; // Colors from the start of one row to the next

    /// Whether the first color of each row is always transparent. Sprite palettes need it.
    bool transparent_pen;

    CacheFile file;
} PaletteSource;

static const PaletteSource palette_sources[] = {
    // BG palette block in SIMM5. Each stage stores its normal block followed by a faded one of the same size.
    { .game = ROM_GAME_SFIII3,
      .offset = SIMM5_OFFSET + 0xF4D280,
      .rows = 17,
      .row_stride = 64,
      .file = CACHE_FILE_SHIN_AKUMA_BG_PALETTE },

    // Ibuki's tanuki (effect 94) for each of Ibuki's NG colors. NG loads it with each color's palette 5.
    { .game = ROM_GAME_SFIII,
      .offset = 0x23D5300,
      .rows = 6,
      .row_stride = 128,
      .transparent_pen = true,
      .file = CACHE_FILE_NG_EF94_PALETTE },
};

/// Program tables that the stage conversion reads. Most are indexed by bg_index.
typedef struct StageTables {
    Uint32 use_scr;          // s16: number of scroll layers
    Uint32 bg_cg_tbl;        // {u32 dictionary, u32 size, u32 source, u32 mode}: tile DMA, word addresses
    Uint32 bg_pal_tbl;       // s16: palette_load id
    Uint32 bg_pal_base_tbl;  // s16: palette code that map entries are relative to
    Uint32 bg_block_count;   // s16[4]: map blocks per layer
    Uint32 bg_map_base;      // u32: map blocks of 16x16 entries
    Uint32 bg_block_list;    // u32[4]: {u32 tilemap offset, u32 block index} lists, in RAM
    Uint32 palette_load_tbl; // {u32 source, u32 destination, u32 size}, in bytes, indexed by palette id
    Uint32 ram_image;        // Initialized RAM at 0x02000000 is copied from here at boot
    Uint32 cg_table;         // CG entries, indexed by CG number. They end with the descriptor address.
    Uint32 cg_entry_size;
    Uint32 cg_origin_offset; // Offset of the s16 origin x within a CG entry. Origin y follows it.
} StageTables;

static const StageTables stage_tables[ROM_GAME_COUNT] = {
    [ROM_GAME_SFIII] = {
        .use_scr = 0x0640B658,
        .bg_cg_tbl = 0x0640BEEC,
        .bg_pal_tbl = 0x0640D6E8,
        .bg_pal_base_tbl = 0x0640C14C,
        .bg_block_count = 0x0640C184,
        .bg_map_base = 0x0640C25C,
        .bg_block_list = 0x02005CC4,
        .palette_load_tbl = 0x0642D74C,
        .ram_image = 0x0643BC60,
        .cg_table = 0x06500000,
        .cg_entry_size = 12,
        .cg_origin_offset = 2,
    },
};

#define MAX_OBJECT_PALETTES 4
#define MAX_REWRITE_CHIPS 16
#define MAX_PALETTE_STEPS 2

/// A step of NG's effect 14, which animates a layer by cycling the palettes of a rectangle of tilemap cells
typedef struct PaletteStep {
    int variant;
    int step; // Index in the variant's palette sequence
} PaletteStep;

/// A chip of an alternate tilemap that the stage swaps in to animate a layer. It becomes a rewrite chip, which the PPG
/// stores after the layer chips.
typedef struct RewriteChip {
    Uint32 blocks; // {u32 tilemap offset, u32 block index} list of the alternate tilemap, or 0 for the layer's own
    int block_count;
    int layer; // Used when `blocks` is 0
    int chip;

    /// Effect 14 steps applied to the tilemap
    PaletteStep palette_steps[MAX_PALETTE_STEPS];
    int palette_step_count;
} RewriteChip;

/// An arcade stage, converted to a PS2-style palette and stage PPG
typedef struct StageSource {
    RomGame game;
    int bg_index;
    Area area;
    CacheFile palette_file;
    CacheFile ppg_file;

    /// Palettes that the stage's setup code loads for its objects. They follow the stage palette.
    int object_palettes[MAX_OBJECT_PALETTES];
    int object_palette_count;

    /// Must match `rewrite_scr` and the stage's `bgrw_data_tbl` entries
    RewriteChip rewrite_chips[MAX_REWRITE_CHIPS];
    int rewrite_chip_count;
} StageSource;

/// Alternate tilemaps of Gill's stage. NG's effect 12 cycles through them to animate the lava of layer 0.
#define NG_GILL_LAVA_MAP_0 0x0643275C
#define NG_GILL_LAVA_MAP_1 0x064327DC

/// Variants of NG's effect 14 (20 bytes each): {u8[3], u8 layer, u8[4], u32 palette sequence, u32 cell mask,
/// u32 size}. The sequence holds {u8, u8 frames, u16 palette row} steps, the mask a byte per cell (1 if the cell
/// changes) and the size {u8 width, u8 height} in cells.
#define NG_EFFECT14_VARIANTS 0x06433400

/// u16 tilemap offset of each variant's rectangle. The rectangles are in the lower half of the tilemap.
#define NG_EFFECT14_OFFSETS 0x0643305C
#define NG_EFFECT14_OFFSET_BASE 0x2000

static const StageSource stage_sources[] = {
    { .game = ROM_GAME_SFIII,
      .bg_index = 0,
      .area = AREA_NG_GILL,
      .palette_file = CACHE_FILE_NG_GILL_BG_PALETTE,
      .ppg_file = CACHE_FILE_NG_GILL_STAGE,
      // The lava chips 49-53 of layer 0, in the order rw231-rw235 use them. Effect 12 shows the upper half of map 0,
      // the lower half of map 0 (the regular tilemap), the upper half of map 1 and the lower half of map 1.
      .rewrite_chips = { { NG_GILL_LAVA_MAP_0, 16, 17 },
                         { NG_GILL_LAVA_MAP_1, 16, 17 },
                         { NG_GILL_LAVA_MAP_1, 16, 49 },
                         { NG_GILL_LAVA_MAP_0, 16, 18 },
                         { NG_GILL_LAVA_MAP_1, 16, 18 },
                         { NG_GILL_LAVA_MAP_1, 16, 50 },
                         { NG_GILL_LAVA_MAP_0, 16, 19 },
                         { NG_GILL_LAVA_MAP_1, 16, 19 },
                         { NG_GILL_LAVA_MAP_1, 16, 51 },
                         { NG_GILL_LAVA_MAP_0, 16, 20 },
                         { NG_GILL_LAVA_MAP_1, 16, 20 },
                         { NG_GILL_LAVA_MAP_1, 16, 52 },
                         { NG_GILL_LAVA_MAP_1, 16, 21 } },
      .rewrite_chip_count = 13 },
    { .game = ROM_GAME_SFIII,
      .bg_index = 1,
      .area = AREA_NG_ALEX,
      .palette_file = CACHE_FILE_NG_ALEX_BG_PALETTE,
      .ppg_file = CACHE_FILE_NG_ALEX_STAGE,
      .object_palettes = { 0x68 },
      .object_palette_count = 1 },
    { .game = ROM_GAME_SFIII,
      .bg_index = 3,
      .area = AREA_NG_RYU_A,
      .palette_file = CACHE_FILE_NG_RYU_A_BG_PALETTE,
      .ppg_file = CACHE_FILE_NG_RYU_A_STAGE,
      .object_palettes = { 0x66 },
      .object_palette_count = 1,
      // Steps 1 and 2 of effect 14's variants 1 and 2 (chips 59 and 60 of layer 0) and variant 3 (chip 52 of layer 2),
      // in the order rw261-rw263 use them. The regular tilemap already shows step 0.
      .rewrite_chips = { { .layer = 0, .chip = 59, .palette_steps = { { 1, 1 } }, .palette_step_count = 1 },
                         { .layer = 0, .chip = 59, .palette_steps = { { 1, 2 } }, .palette_step_count = 1 },
                         { .layer = 0, .chip = 60, .palette_steps = { { 1, 1 }, { 2, 1 } }, .palette_step_count = 2 },
                         { .layer = 0, .chip = 60, .palette_steps = { { 1, 2 }, { 2, 2 } }, .palette_step_count = 2 },
                         { .layer = 2, .chip = 52, .palette_steps = { { 3, 1 } }, .palette_step_count = 1 },
                         { .layer = 2, .chip = 52, .palette_steps = { { 3, 2 } }, .palette_step_count = 1 } },
      .rewrite_chip_count = 6 },
};

#define MAX_CG_RANGES 4

typedef struct CgRange {
    Uint16 first;
    int count;
} CgRange;

/// A texture group built from arcade CGs. The ranges fill the group's slots in order.
typedef struct CgGroupSource {
    RomGame game;
    int group;
    CgRange ranges[MAX_CG_RANGES];
    int range_count;
    ArcadeCgFlags flags;
    CacheFile file;
} CgGroupSource;

static const CgGroupSource cg_group_sources[] = {
    // Objects of NG Alex's stage
    { .game = ROM_GAME_SFIII,
      .group = 100,
      .ranges = { { 0x5620, 320 } },
      .range_count = 1,
      .flags = ARCADE_CG_PART_PALETTES | ARCADE_CG_COMPRESS,
      .file = CACHE_FILE_NG_ALEX_OBJECTS },

    // Ibuki's tanuki (effect 94). Its char table is _ng_eff94_char_table. The tanuki itself uses part palette 5, while
    // the notes and the figures use Ibuki's palette.
    { .game = ROM_GAME_SFIII,
      .group = 101,
      .ranges = { { 0x235B, 160 }, { 0x7245, 15 }, { 0x7263, 3 }, { 0x732C, 6 } },
      .range_count = 4,
      .flags = ARCADE_CG_PART_PALETTES | ARCADE_CG_COMPRESS,
      .file = CACHE_FILE_NG_EF94 },

    // Objects of NG Gill's stage. Its char table is _ng_fnl_char_table.
    { .game = ROM_GAME_SFIII,
      .group = 102,
      .ranges = { { 0x6000, 156 }, { 0x6390, 82 } },
      .range_count = 2,
      .flags = ARCADE_CG_PART_PALETTES | ARCADE_CG_COMPRESS,
      .file = CACHE_FILE_NG_GILL_OBJECTS },

    // Objects of the first area of NG Ryu's stage. Its char table is _ng_j10_a_char_table.
    { .game = ROM_GAME_SFIII,
      .group = 103,
      .ranges = { { 0x41C6, 432 }, { 0x7718, 32 } },
      .range_count = 2,
      .flags = ARCADE_CG_PART_PALETTES | ARCADE_CG_COMPRESS,
      .file = CACHE_FILE_NG_RYU_A_OBJECTS },
};

/// CPS3 graphics addresses start this far before the graphics region
#define GRAPHICS_ADDRESS_BASE 0x400000

#define RAM_BASE 0x02000000
#define RAM_SIZE 0x100000
#define TILEMAP_SIZE 64
#define TILE_BYTES 256
#define PPG_CHIP_SIZE 128
#define PPG_CHIP_BLOCKS 8
#define PPG_FIRST_CHIP 32
#define PPG_LAYER_CHIPS 32
#define PPG_LAYERS 3

/// The parts of a ROM set that the stage conversion reads
typedef struct StageRom {
    RomRegion program; // Starts at ROM_PROGRAM_BASE
    RomRegion graphics;
    const StageTables* tables;
} StageRom;

static bool rom_processed[ROM_GAME_COUNT] = { false };

/// Converts a CPS3 xRGB555 color to the PS2 file format: xBGR555 with bit 15 set on everything
/// except the transparent color that starts each 64-color row.
/// @param transparent_pen Whether the first color of each row is always transparent
static Uint16 convert_color(Uint16 color, size_t index, bool transparent_pen) {
    color &= 0x7FFF;
    const Uint16 swapped = ((color & 0x1F) << 10) | (color & 0x3E0) | (color >> 10);

    if (index % 64 == 0 && (swapped == 0 || transparent_pen)) {
        return 0;
    }

    return swapped | 0x8000;
}

static bool write_palette(const PaletteSource* source, RomRegion graphics) {
    const size_t count = source->rows * 64;
    const size_t end = source->offset + ((source->rows - 1) * source->row_stride + 64) * 2;

    if (graphics.data == NULL || end > graphics.size) {
        SDL_Log("Couldn't build %s: ROM has no data for it", Cache_GetPath(source->file));
        return false;
    }

    Uint16* palette = SDL_malloc(count * sizeof(Uint16));

    if (palette == NULL) {
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        const Uint8* src = graphics.data + source->offset + ((i / 64) * source->row_stride + i % 64) * 2;
        palette[i] = SDL_Swap16LE(convert_color(src[0] | (src[1] << 8), i, source->transparent_pen));
    }

    const bool success = Cache_Write(source->file, palette, count * sizeof(Uint16));
    SDL_free(palette);
    return success;
}

// MARK: - Stage conversion

static bool program_read(const StageRom* rom, Uint32 address, size_t size, const Uint8** result) {
    if (address >= RAM_BASE && address < RAM_BASE + RAM_SIZE) {
        address = address - RAM_BASE + rom->tables->ram_image;
    }

    if (address < ROM_PROGRAM_BASE || address - ROM_PROGRAM_BASE + size > rom->program.size) {
        return false;
    }

    *result = rom->program.data + (address - ROM_PROGRAM_BASE);
    return true;
}

static bool program_u16(const StageRom* rom, Uint32 address, Uint16* value) {
    const Uint8* p;

    if (!program_read(rom, address, 2, &p)) {
        return false;
    }

    *value = ((Uint16)p[0] << 8) | p[1];
    return true;
}

static bool program_u32(const StageRom* rom, Uint32 address, Uint32* value) {
    const Uint8* p;

    if (!program_read(rom, address, 4, &p)) {
        return false;
    }

    *value = ((Uint32)p[0] << 24) | ((Uint32)p[1] << 16) | ((Uint32)p[2] << 8) | p[3];
    return true;
}

static bool graphics_offset(const StageRom* rom, Uint32 address, size_t* offset) {
    if (address < GRAPHICS_ADDRESS_BASE || address - GRAPHICS_ADDRESS_BASE >= rom->graphics.size) {
        return false;
    }

    *offset = address - GRAPHICS_ADDRESS_BASE;
    return true;
}

/// Decompresses the stage's 16x16 tiles.
static Uint8* load_tiles(const StageRom* rom, int bg_index, size_t* tiles_size) {
    const Uint32 record = rom->tables->bg_cg_tbl + bg_index * 0x10;
    Uint32 dictionary_word;
    Uint32 size;
    Uint32 source_word;
    size_t dictionary;
    size_t source;

    if (!program_u32(rom, record, &dictionary_word) || !program_u32(rom, record + 4, &size) ||
        !program_u32(rom, record + 8, &source_word) || !graphics_offset(rom, dictionary_word * 2, &dictionary) ||
        !graphics_offset(rom, source_word * 2, &source)) {
        return NULL;
    }

    *tiles_size = ((size_t)size + 1) * 16;
    Uint8* tiles = SDL_malloc(*tiles_size);

    if (tiles == NULL || !Cps3_DecodeDma(rom->graphics, source, dictionary, tiles, *tiles_size)) {
        SDL_free(tiles);
        return NULL;
    }

    return tiles;
}

/// Converts a palette that `palette_load` loads.
/// @param palette Receives the colors after the `*count` already in it.
/// @param first_row Receives the CPS3 palette row that the palette is loaded at.
static bool load_palette(
    const StageRom* rom, Uint16 palette_id, Uint16* palette, size_t capacity, size_t* count, int* first_row
) {
    const Uint32 record = rom->tables->palette_load_tbl + (palette_id & 0xFF) * 12;
    Uint32 source_address;
    Uint32 destination;
    Uint32 size;
    size_t source;

    if (!program_u32(rom, record, &source_address) || !program_u32(rom, record + 4, &destination) ||
        !program_u32(rom, record + 8, &size) || !graphics_offset(rom, source_address, &source) ||
        source + size > rom->graphics.size || *count + size / 2 > capacity) {
        return false;
    }

    *first_row = destination / 2 / 64;

    for (size_t i = 0; i < size / 2; i++) {
        const Uint8* src = rom->graphics.data + source + i * 2;
        palette[*count + i] = SDL_Swap16LE(convert_color(src[0] | (src[1] << 8), i, true));
    }

    *count += size / 2;
    return true;
}

#define MAX_STAGE_PALETTE_COLORS (128 * 64)

/// Converts the stage palette that `bg_initialize` loads, followed by the stage's object palettes.
/// @param first_row Receives the CPS3 palette row that the stage palette is loaded at.
static Uint16* load_stage_palette(const StageRom* rom, const StageSource* source, size_t* count, int* first_row) {
    Uint16* palette = SDL_malloc(MAX_STAGE_PALETTE_COLORS * sizeof(Uint16));
    Uint16 palette_id;
    bool success = palette != NULL && program_u16(rom, rom->tables->bg_pal_tbl + source->bg_index * 2, &palette_id);
    *count = 0;
    success = success && load_palette(rom, palette_id, palette, MAX_STAGE_PALETTE_COLORS, count, first_row);

    for (int i = 0; i < source->object_palette_count && success; i++) {
        int row;
        const int expected_row = *first_row + (int)(*count / 64);

        // Object palettes must continue the stage palette, since the PS2 format loads one block
        success = load_palette(rom, source->object_palettes[i], palette, MAX_STAGE_PALETTE_COLORS, count, &row) &&
                  row == expected_row;
    }

    if (!success) {
        SDL_free(palette);
        return NULL;
    }

    return palette;
}

/// Builds a 64x64 tilemap from a {u32 tilemap offset, u32 block index} list. Each entry holds the tile number in the
/// upper half and the attributes in the lower half. Entries that no map block covers are 0xFFFFFFFF.
static bool build_tilemap_from_blocks(const StageRom* rom, int bg_index, Uint32 blocks, int count, Uint32* tilemap) {
    Uint32 map_base;

    SDL_memset(tilemap, 0xFF, TILEMAP_SIZE * TILEMAP_SIZE * sizeof(Uint32));

    if (!program_u32(rom, rom->tables->bg_map_base + bg_index * 4, &map_base)) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        Uint32 offset;
        Uint32 block;

        if (!program_u32(rom, blocks + i * 8, &offset) || !program_u32(rom, blocks + i * 8 + 4, &block)) {
            return false;
        }

        const int top = offset / 0x100;
        const int left = (offset % 0x100) / 4;

        if (top + 16 > TILEMAP_SIZE || left + 16 > TILEMAP_SIZE) {
            return false;
        }

        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                if (!program_u32(
                        rom, map_base + block * 0x400 + (y * 16 + x) * 4, &tilemap[(top + y) * TILEMAP_SIZE + left + x]
                    )) {
                    return false;
                }
            }
        }
    }

    return true;
}

/// Builds a layer's 64x64 tilemap from the stage's map block list.
static bool build_tilemap(const StageRom* rom, int bg_index, int layer, Uint32* tilemap) {
    Uint16 count;
    Uint32 blocks;

    return program_u16(rom, rom->tables->bg_block_count + bg_index * 8 + layer * 2, &count) &&
           program_u32(rom, rom->tables->bg_block_list + bg_index * 0x10 + layer * 4, &blocks) &&
           build_tilemap_from_blocks(rom, bg_index, blocks, count, tilemap);
}

/// Sets the palettes that a step of NG's effect 14 gives to its cells.
/// @param palette_base Palette code that map entries are relative to
static bool apply_palette_step(
    const StageRom* rom, const PaletteStep* step, int layer, int palette_base, Uint32* tilemap
) {
    const Uint32 variant = NG_EFFECT14_VARIANTS + step->variant * 20;
    const Uint8* header;
    const Uint8* size;
    const Uint8* mask;
    Uint32 sequence;
    Uint32 mask_address;
    Uint32 size_address;
    Uint16 offset;
    Uint16 palette;

    if (!program_read(rom, variant, 8, &header) || header[3] != layer || !program_u32(rom, variant + 8, &sequence) ||
        !program_u32(rom, variant + 12, &mask_address) || !program_u32(rom, variant + 16, &size_address) ||
        !program_read(rom, size_address, 2, &size) || !program_read(rom, mask_address, size[0] * size[1], &mask) ||
        !program_u16(rom, NG_EFFECT14_OFFSETS + step->variant * 2, &offset) ||
        !program_u16(rom, sequence + step->step * 4 + 2, &palette)) {
        return false;
    }

    const int top = (NG_EFFECT14_OFFSET_BASE + offset) / 0x100;
    const int left = (offset % 0x100) / 4;

    if (top + size[1] > TILEMAP_SIZE || left + size[0] > TILEMAP_SIZE) {
        return false;
    }

    for (int y = 0; y < size[1]; y++) {
        for (int x = 0; x < size[0]; x++) {
            Uint32* entry = &tilemap[(top + y) * TILEMAP_SIZE + left + x];

            if (mask[y * size[0] + x] == 1 && *entry != 0xFFFFFFFF) {
                // The effect writes absolute palette codes, while map entries are relative to palette_base
                *entry = (*entry & 0xFFFFFE00) | ((palette - palette_base) & 0x1FF);
            }
        }
    }

    return true;
}

/// Draws a 128x128 chip from the tilemap.
/// @param block_palettes Receives the palette row of each 16x16 block, or -1 if the block is empty.
static bool draw_chip(
    const Uint32* tilemap, const Uint8* tiles, size_t tiles_size, int chip, int palette_base, int palette_rows,
    Uint8* pixels, int* block_palettes
) {
    const int chip_x = (chip % PPG_CHIP_BLOCKS) * PPG_CHIP_BLOCKS;
    const int chip_y = (chip / PPG_CHIP_BLOCKS) * PPG_CHIP_BLOCKS;

    SDL_memset(pixels, 0, PPG_CHIP_SIZE * PPG_CHIP_SIZE);

    for (int by = 0; by < PPG_CHIP_BLOCKS; by++) {
        for (int bx = 0; bx < PPG_CHIP_BLOCKS; bx++) {
            const Uint32 entry = tilemap[(chip_y + by) * TILEMAP_SIZE + chip_x + bx];
            int* block_palette = &block_palettes[by * PPG_CHIP_BLOCKS + bx];
            *block_palette = -1;

            if (entry == 0xFFFFFFFF) {
                continue;
            }

            const size_t tile = (entry >> 16) >> 1;
            const Uint16 attributes = (Uint16)(entry + palette_base);
            const int palette = (attributes & 0x1FF) - (palette_base & 0x1FF);
            const bool flip_y = attributes & 0x800;
            const bool flip_x = attributes & 0x1000;
            bool empty = true;

            // Only 6bpp tiles fit in the PS2 format's 64-color palettes
            if (!(attributes & 0x200) || (tile + 1) * TILE_BYTES > tiles_size) {
                return false;
            }

            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16; x++) {
                    const int source_x = flip_x ? 15 - x : x;
                    const int source_y = flip_y ? 15 - y : y;
                    const Uint8 pixel = tiles[tile * TILE_BYTES + source_y * 16 + source_x] & 0x3F;
                    pixels[(by * 16 + y) * PPG_CHIP_SIZE + bx * 16 + x] = pixel;
                    empty = empty && pixel == 0;
                }
            }

            if (empty) {
                continue;
            }

            if (palette < 0 || palette >= palette_rows) {
                SDL_LogDebug(
                    SDL_LOG_CATEGORY_APPLICATION,
                    "Dropping tile %zu of chip %d: palette %d is outside the stage palette",
                    tile,
                    chip,
                    palette
                );

                for (int y = 0; y < 16; y++) {
                    SDL_memset(&pixels[(by * 16 + y) * PPG_CHIP_SIZE + bx * 16], 0, 16);
                }

                continue;
            }

            *block_palette = palette;
        }
    }

    return true;
}

/// Appends a pTEX chunk. Its trans entries are rectangles of 16x16 blocks that share a palette.
static bool append_chip(ByteBuffer* ppg, const Uint8* pixels, const int* block_palettes) {
    Uint8 trans[PPG_CHIP_BLOCKS * PPG_CHIP_BLOCKS * 3];
    bool covered[PPG_CHIP_BLOCKS * PPG_CHIP_BLOCKS] = { false };
    int trans_count = 0;

    for (int by = 0; by < PPG_CHIP_BLOCKS; by++) {
        for (int bx = 0; bx < PPG_CHIP_BLOCKS; bx++) {
            const int palette = block_palettes[by * PPG_CHIP_BLOCKS + bx];

            if (palette < 0 || covered[by * PPG_CHIP_BLOCKS + bx]) {
                continue;
            }

            int width = 1;
            int height = 1;

            while (bx + width < PPG_CHIP_BLOCKS && !covered[by * PPG_CHIP_BLOCKS + bx + width] &&
                   block_palettes[by * PPG_CHIP_BLOCKS + bx + width] == palette) {
                width++;
            }

            for (bool grow = true; grow && by + height < PPG_CHIP_BLOCKS;) {
                for (int x = bx; x < bx + width && grow; x++) {
                    grow = !covered[(by + height) * PPG_CHIP_BLOCKS + x] &&
                           block_palettes[(by + height) * PPG_CHIP_BLOCKS + x] == palette;
                }

                height += grow;
            }

            for (int y = by; y < by + height; y++) {
                for (int x = bx; x < bx + width; x++) {
                    covered[y * PPG_CHIP_BLOCKS + x] = true;
                }
            }

            trans[trans_count * 3] = palette;
            trans[trans_count * 3 + 1] = by * PPG_CHIP_BLOCKS + bx;
            trans[trans_count * 3 + 2] = ((width - 1) << 4) | (height - 1);
            trans_count++;
        }
    }

    uLongf compressed_size = compressBound(PPG_CHIP_SIZE * PPG_CHIP_SIZE);
    Uint8* compressed = SDL_malloc(compressed_size);

    if (compressed == NULL ||
        compress2(compressed, &compressed_size, pixels, PPG_CHIP_SIZE * PPG_CHIP_SIZE, Z_BEST_COMPRESSION) != Z_OK) {
        SDL_free(compressed);
        return false;
    }

    const Uint32 size = 16 + trans_count * 3 + (Uint32)compressed_size;
    const Uint8 header[16] = { 'p',
                               'T',
                               'E',
                               'X',
                               size >> 24,
                               size >> 16,
                               size >> 8,
                               size,
                               PPG_CHIP_SIZE / 16,
                               PPG_CHIP_SIZE / 16,
                               2,    // zlib
                               0x81, // 8-bit indices
                               0,
                               0,
                               trans_count >> 8,
                               trans_count };
    const Uint8 padding[3] = { 0 };

    const bool success =
        ByteBuffer_Append(ppg, header, sizeof(header)) && ByteBuffer_Append(ppg, trans, trans_count * 3) &&
        ByteBuffer_Append(ppg, compressed, compressed_size) && ByteBuffer_Append(ppg, padding, (4 - size % 4) % 4);
    SDL_free(compressed);
    return success;
}

/// Builds the stage PPG: one pTEX per chip that `bgtex_stage_gbix` selects, layer by layer, followed by the rewrite
/// chips.
static bool build_ppg(
    const StageRom* rom, const StageSource* source, const Uint8* tiles, size_t tiles_size, int palette_base,
    int palette_rows, ByteBuffer* ppg
) {
    Uint16 layer_count = 0;
    Uint32* tilemap = SDL_malloc(TILEMAP_SIZE * TILEMAP_SIZE * sizeof(Uint32));
    Uint8* pixels = SDL_malloc(PPG_CHIP_SIZE * PPG_CHIP_SIZE);
    int block_palettes[PPG_CHIP_BLOCKS * PPG_CHIP_BLOCKS];
    bool success = tilemap != NULL && pixels != NULL &&
                   program_u16(rom, rom->tables->use_scr + source->bg_index * 2, &layer_count) &&
                   layer_count <= PPG_LAYERS;

    for (int layer = 0; layer < layer_count && success; layer++) {
        const Uint32 mask = bgtex_stage_gbix[source->area][layer];
        success = build_tilemap(rom, source->bg_index, layer, tilemap);

        for (int i = 0; i < PPG_LAYER_CHIPS && success; i++) {
            if (!(mask & (0x80000000 >> i))) {
                continue;
            }

            success =
                draw_chip(
                    tilemap, tiles, tiles_size, PPG_FIRST_CHIP + i, palette_base, palette_rows, pixels, block_palettes
                ) &&
                append_chip(ppg, pixels, block_palettes);
        }
    }

    for (int i = 0; i < source->rewrite_chip_count && success; i++) {
        const RewriteChip* chip = &source->rewrite_chips[i];

        if (chip->blocks != 0) {
            success = build_tilemap_from_blocks(rom, source->bg_index, chip->blocks, chip->block_count, tilemap);
        } else {
            success = build_tilemap(rom, source->bg_index, chip->layer, tilemap);
        }

        for (int j = 0; j < chip->palette_step_count && success; j++) {
            success = apply_palette_step(rom, &chip->palette_steps[j], chip->layer, palette_base, tilemap);
        }

        success =
            success &&
            draw_chip(tilemap, tiles, tiles_size, chip->chip, palette_base, palette_rows, pixels, block_palettes) &&
            append_chip(ppg, pixels, block_palettes);
    }

    success = success && ByteBuffer_Append(ppg, "pEND", 4);
    SDL_free(tilemap);
    SDL_free(pixels);
    return success;
}

/// Builds a texture group from CGs. Its trans table is padded to the group's `to_tex`.
static bool write_cg_group(const CgGroupSource* source, const StageRom* rom) {
    const ArcadeCgSource cg_source = {
        .program = rom->program,
        .program_base = ROM_PROGRAM_BASE,
        .table = rom->tables->cg_table,
        .entry_size = rom->tables->cg_entry_size,
        .origin_offset = rom->tables->cg_origin_offset,
        .graphics = rom->graphics,
    };

    int slot_count = 0;

    for (int i = 0; i < source->range_count; i++) {
        slot_count += source->ranges[i].count;
    }

    const size_t to_tex = texgrpdat[source->group].to_tex;
    Sint32* cgs = SDL_malloc(slot_count * sizeof(Sint32));
    ArcadeCgGroup group = { 0 };
    Uint8* file = NULL;
    bool success = cgs != NULL;

    for (int i = 0, slot = 0; i < source->range_count && success; i++) {
        for (int j = 0; j < source->ranges[i].count; j++, slot++) {
            const Uint16 cg = source->ranges[i].first + j;
            cgs[slot] = ArcadeCg_Exists(&cg_source, cg) ? cg : -1;
        }
    }

    success = success && ArcadeCg_BuildGroup(&cg_source, cgs, slot_count, source->flags, &group);

    if (success && group.trans_size > to_tex) {
        SDL_Log(
            "Trans table of %s needs %zu bytes, but to_tex is %zu",
            Cache_GetPath(source->file),
            group.trans_size,
            to_tex
        );
        success = false;
    }

    if (success) {
        file = SDL_calloc(1, to_tex + group.texture_size);
        success = file != NULL;
    }

    if (success) {
        SDL_memcpy(file, group.trans_table, group.trans_size);
        SDL_memcpy(file + to_tex, group.texture_table, group.texture_size);
        success = Cache_Write(source->file, file, to_tex + group.texture_size);
    }

    if (!success) {
        SDL_Log("Couldn't build %s", Cache_GetPath(source->file));
    }

    SDL_free(cgs);
    SDL_free(file);
    ArcadeCg_FreeGroup(&group);
    return success;
}

static bool write_stage(const StageSource* source, const StageRom* rom) {
    size_t tiles_size = 0;
    size_t palette_count = 0;
    int first_row = 0;
    Uint16 palette_base;
    ByteBuffer ppg = { 0 };
    Uint8* tiles = load_tiles(rom, source->bg_index, &tiles_size);
    Uint16* palette = load_stage_palette(rom, source, &palette_count, &first_row);
    bool success = tiles != NULL && palette != NULL &&
                   program_u16(rom, rom->tables->bg_pal_base_tbl + source->bg_index * 2, &palette_base);

    if (success) {
        // Mirrors FUN_0607bc36. Its special case for the stage 0xD isn't needed yet.
        palette_base = palette_base == 0x200 ? 0x240 : palette_base + 0x10;
        success = (palette_base & 0x1FF) == first_row &&
                  build_ppg(rom, source, tiles, tiles_size, palette_base, (int)(palette_count / 64), &ppg) &&
                  Cache_Write(source->palette_file, palette, palette_count * sizeof(Uint16)) &&
                  Cache_Write(source->ppg_file, ppg.data, ppg.size);
    }

    if (!success) {
        SDL_Log("Couldn't build %s", Cache_GetPath(source->ppg_file));
    }

    SDL_free(tiles);
    SDL_free(palette);
    SDL_free(ppg.data);
    return success;
}

/// @return `false` if the ROM set isn't loaded.
static bool make_stage_rom(const Rom* const roms[ROM_GAME_COUNT], RomGame game, StageRom* result) {
    const Rom* rom = roms[game];

    if (rom == NULL) {
        return false;
    }

    *result = (StageRom) {
        .program = Rom_GetProgram(rom),
        .graphics = Rom_GetGraphics(rom),
        .tables = &stage_tables[game],
    };
    return true;
}

void ArcadeStage_Init(const Rom* const roms[ROM_GAME_COUNT]) {
    for (int game = 0; game < ROM_GAME_COUNT; game++) {
        rom_processed[game] = roms[game] != NULL;
    }

    for (int i = 0; i < SDL_arraysize(palette_sources); i++) {
        const PaletteSource* source = &palette_sources[i];
        StageRom stage_rom;

        if (make_stage_rom(roms, source->game, &stage_rom) && !write_palette(source, stage_rom.graphics)) {
            rom_processed[source->game] = false;
        }
    }

    for (int i = 0; i < SDL_arraysize(stage_sources); i++) {
        const StageSource* source = &stage_sources[i];
        StageRom stage_rom;

        if (make_stage_rom(roms, source->game, &stage_rom) && !write_stage(source, &stage_rom)) {
            rom_processed[source->game] = false;
        }
    }

    for (int i = 0; i < SDL_arraysize(cg_group_sources); i++) {
        const CgGroupSource* source = &cg_group_sources[i];
        StageRom stage_rom;

        if (make_stage_rom(roms, source->game, &stage_rom) && !write_cg_group(source, &stage_rom)) {
            rom_processed[source->game] = false;
        }
    }
}

bool ArcadeStage_IsRomProcessed(RomGame game) {
    return rom_processed[game];
}
