#if ARCADE_ROM_TEXTURES

#include "arcade/arcade_texture.h"

#include "arcade/arcade_char_data.h"
#include "arcade/cps3_decrypt.h"
#include "port/resources.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/rendering/texgroup_data.h"
#include "structs.h"

#include <SDL3/SDL.h>
#include <minizip-ng/mz.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_strm_os.h>
#include <minizip-ng/mz_zip.h>

#define CHIP_BYTES 0x200000
#define PROGRAM_BYTES 0x800000
#define GRAPHICS_BYTES 0x4000000
#define CG_COUNT 0x10000
#define CG_TABLE_BYTES (CG_COUNT * 8)
#define SIMM2_BASE 0x06800000

typedef struct ByteBuffer {
    Uint8* data;
    size_t size;
    size_t capacity;
} ByteBuffer;

typedef struct CgDescriptor {
    const Uint8* data;
    Uint16 allocation;
    Uint16 spans;
    Uint16 parts;
    Uint32 dictionary;
} CgDescriptor;

static Uint8* program = NULL;
static Uint8* graphics = NULL;
static bool load_attempted = false;

static Uint16 be16(const Uint8* p) {
    return ((Uint16)p[0] << 8) | p[1];
}

static Uint32 be32(const Uint8* p) {
    return ((Uint32)p[0] << 24) | ((Uint32)p[1] << 16) | ((Uint32)p[2] << 8) | p[3];
}

static bool grow(ByteBuffer* buffer, size_t additional) {
    if (additional > SIZE_MAX - buffer->size) {
        return false;
    }

    const size_t required = buffer->size + additional;

    if (required <= buffer->capacity) {
        return true;
    }

    size_t capacity = buffer->capacity ? buffer->capacity : 4096;

    while (capacity < required) {
        if (capacity > SIZE_MAX / 2) {
            capacity = required;
            break;
        }

        capacity *= 2;
    }

    void* resized = SDL_realloc(buffer->data, capacity);

    if (resized == NULL) {
        return false;
    }

    buffer->data = resized;
    buffer->capacity = capacity;
    return true;
}

static bool append(ByteBuffer* buffer, const void* bytes, size_t size) {
    if (!grow(buffer, size)) {
        return false;
    }

    SDL_memcpy(buffer->data + buffer->size, bytes, size);
    buffer->size += size;
    return true;
}

static bool read_chip(void* zip, Uint8** destination) {
    mz_zip_file* info = NULL;

    if (mz_zip_entry_get_info(zip, &info) != MZ_OK || info == NULL || info->uncompressed_size != CHIP_BYTES) {
        return false;
    }

    Uint8* chip = SDL_malloc(CHIP_BYTES);

    if (chip == NULL || mz_zip_entry_read_open(zip, false, NULL) != MZ_OK) {
        SDL_free(chip);
        return false;
    }

    size_t read = 0;

    while (read < CHIP_BYTES) {
        int32_t count = mz_zip_entry_read(zip, chip + read, (int32_t)(CHIP_BYTES - read));

        if (count <= 0) {
            break;
        }

        read += (size_t)count;
    }

    mz_zip_entry_close(zip);

    if (read != CHIP_BYTES) {
        SDL_free(chip);
        return false;
    }

    *destination = chip;
    return true;
}

static bool load_rom() {
    Uint8* chips[7][8] = { 0 };
    const char* path = Resources_GetPath("sfiii3nr1.zip");
    void* stream = mz_stream_os_create();
    void* zip = mz_zip_create();
    bool success = false;

    if (path != NULL && stream != NULL && zip != NULL && mz_stream_open(stream, path, MZ_OPEN_MODE_READ) == MZ_OK &&
        mz_zip_open(zip, stream, MZ_OPEN_MODE_READ) == MZ_OK) {
        for (int32_t status = mz_zip_goto_first_entry(zip); status == MZ_OK; status = mz_zip_goto_next_entry(zip)) {
            mz_zip_file* info = NULL;

            if (mz_zip_entry_get_info(zip, &info) != MZ_OK || info == NULL) {
                break;
            }

            const char* name = SDL_strrchr(info->filename, '/');
            name = name != NULL ? name + 1 : info->filename;
            int bank = 0;
            int chip = 0;
            int consumed = 0;

            if (SDL_sscanf(name, "sfiii3-simm%d.%d%n", &bank, &chip, &consumed) == 2 && name[consumed] == '\0' &&
                bank >= 2 && bank <= 6 && chip >= 0 && chip < (bank == 2 ? 4 : 8) && chips[bank][chip] == NULL) {
                if (!read_chip(zip, &chips[bank][chip])) {
                    break;
                }
            }
        }

        success = true;

        for (int bank = 2; bank <= 6; bank++) {
            for (int chip = 0; chip < (bank == 2 ? 4 : 8); chip++) {
                if (chips[bank][chip] == NULL) {
                    success = false;
                }
            }
        }

        if (success) {
            program = SDL_malloc(PROGRAM_BYTES);
            graphics = SDL_malloc(GRAPHICS_BYTES);
            success = program != NULL && graphics != NULL;
        }

        if (success) {
            for (int i = 0; i < CHIP_BYTES; i++) {
                Uint32 word =
                    cps3_decrypt_at(chips[2][0][i], chips[2][1][i], chips[2][2][i], chips[2][3][i], SIMM2_BASE + i * 4);
                SDL_memcpy(program + i * 4, &word, 4);
            }

            // Graphics SIMMs use pairs of 16-bit lanes, unlike the four program byte lanes.
            for (int bank = 3; bank <= 6; bank++) {
                Uint8* bank_start = graphics + (bank - 3) * 0x1000000;

                for (int pair = 0; pair < 4; pair++) {
                    Uint8* dst = bank_start + pair * 0x400000;
                    const Uint8* even = chips[bank][pair * 2];
                    const Uint8* odd = chips[bank][pair * 2 + 1];

                    for (int i = 0; i < CHIP_BYTES / 2; i++) {
                        dst[i * 4] = even[i * 2];
                        dst[i * 4 + 1] = odd[i * 2];
                        dst[i * 4 + 2] = even[i * 2 + 1];
                        dst[i * 4 + 3] = odd[i * 2 + 1];
                    }
                }
            }
        }
    }

    for (int bank = 2; bank <= 6; bank++) {
        for (int chip = 0; chip < 8; chip++) {
            SDL_free(chips[bank][chip]);
        }
    }

    if (zip != NULL) {
        mz_zip_close(zip);
        mz_zip_delete(&zip);
    }

    if (stream != NULL) {
        mz_stream_os_delete(&stream);
    }

    SDL_free((void*)path);

    if (!success) {
        SDL_free(program);
        SDL_free(graphics);
        program = graphics = NULL;
    }

    return success;
}

static bool descriptor_for(Uint16 cg, CgDescriptor* result) {
    const Uint32 address = be32(program + (Uint32)cg * 8 + 4);

    if (address < SIMM2_BASE + CG_TABLE_BYTES || address >= SIMM2_BASE + PROGRAM_BYTES - 12) {
        return false;
    }

    const Uint32 offset = address - SIMM2_BASE;
    const Uint8* p = program + offset;
    const Uint16 spans = be16(p + 4);
    const Uint16 parts = be16(p + 6);

    if (!spans || spans > 0x400 || parts > 0x400 ||
        (Uint64)offset + 12 + (Uint64)spans * 8 + (Uint64)parts * 8 > PROGRAM_BYTES) {
        return false;
    }

    *result = (CgDescriptor) {
        .data = p, .allocation = be16(p + 2), .spans = spans, .parts = parts, .dictionary = be32(p + 8)
    };

    return true;
}

static bool graphics_offset(Uint32 dma_word, size_t* offset) {
    const Uint64 byte_address = (Uint64)dma_word * 2;

    if (byte_address < 0x400000 || byte_address - 0x400000 >= GRAPHICS_BYTES) {
        return false;
    }

    *offset = (size_t)(byte_address - 0x400000);
    return true;
}

static void emit_dma_byte(Uint8 value, Uint8* dst, size_t length, size_t* written, Uint8* previous) {
    if (value & 0x40) {
        size_t run = (value & 0x3f) + 1;

        if (run > length - *written) {
            run = length - *written;
        }

        SDL_memset(dst + *written, *previous, run);
        *written += run;
    } else if (*written < length) {
        dst[(*written)++] = value;
        *previous = value;
    }
}

static bool decode_dma(size_t source, size_t dictionary, Uint8* dst, size_t length) {
    size_t written = 0;
    Uint8 previous = 0;

    while (written < length) {
        if (source >= GRAPHICS_BYTES) {
            return false;
        }

        Uint8 control = graphics[source];
        source += 1;

        if (control & 0x80) {
            size_t entry = dictionary + (control & 0x7f) * 2;

            if (entry + 1 >= GRAPHICS_BYTES) {
                return false;
            }

            emit_dma_byte(graphics[entry], dst, length, &written, &previous);
            emit_dma_byte(graphics[entry + 1], dst, length, &written, &previous);
        } else {
            emit_dma_byte(control, dst, length, &written, &previous);
        }
    }

    return true;
}

static Uint8* decode_cram(const CgDescriptor* descriptor, size_t* allocation) {
    *allocation = ((descriptor->allocation >> 5) + 1) * 0x1000;
    Uint8* cram = SDL_calloc(1, *allocation);
    size_t dictionary = 0;

    if (cram == NULL || !graphics_offset(descriptor->dictionary, &dictionary)) {
        SDL_free(cram);
        return NULL;
    }

    for (int i = 0; i < descriptor->spans; i++) {
        const Uint8* span = descriptor->data + 12 + i * 8;
        const size_t destination = (size_t)be16(span + 6) * 16;
        const size_t length = ((size_t)be16(span + 4) + 1) * 16;
        size_t source = 0;

        if (destination > *allocation || length > *allocation - destination || !graphics_offset(be32(span), &source) ||
            !decode_dma(source, dictionary, cram + destination, length)) {
            SDL_free(cram);
            return NULL;
        }
    }

    // DMA's ^3 write and the PPU's reversed four-byte pixel groups cancel;
    // this buffer is the logical row-major 16x16 tile image.
    return cram;
}

static int signed10(Uint16 value) {
    value &= 0x3ff;
    return value & 0x200 ? (int)value - 0x400 : value;
}

static bool add_cg(Uint16 cg, ByteBuffer* placements, ByteBuffer* pixels, Uint32* texture_offsets, int* texture_count) {
    CgDescriptor descriptor;

    if (!descriptor_for(cg, &descriptor)) {
        Uint16 empty = 0;
        return append(placements, &empty, sizeof(empty));
    }

    size_t cram_size = 0;
    Uint8* cram = decode_cram(&descriptor, &cram_size);

    if (cram == NULL) {
        return false;
    }

    ByteBuffer entries = { 0 };
    const Sint16 origin_x = (Sint16)be16(program + (Uint32)cg * 8);
    const Sint16 origin_y = (Sint16)be16(program + (Uint32)cg * 8 + 2);
    const Uint8 size_table[4] = { 8, 1, 2, 4 };
    int previous_x = 0;
    int previous_y = 0;
    int count = 0;
    bool success = true;

    for (int part_index = 0; part_index < descriptor.parts && success; part_index++) {
        const Uint8* part = descriptor.data + 12 + descriptor.spans * 8 + part_index * 8;
        const int tile_base = be16(part) / 2;
        const Uint16 attributes = be16(part + 2);
        const int shape = be16(part + 6) >> 12;
        const int tiles_x = size_table[shape & 3];
        const int tiles_y = size_table[shape >> 2];
        const int left = origin_x + (Sint16)be16(part + 4) - tiles_x * 8;
        const int top = origin_y + signed10(be16(part + 6)) - tiles_y * 8;
        const bool flip_x = (attributes & 0x1000) != 0;
        const bool flip_y = (attributes & 0x0800) != 0;

        for (int block_y = 0; block_y < tiles_y && success; block_y += 2) {
            for (int block_x = 0; block_x < tiles_x && success; block_x += 2) {
                const int block_tiles_x = SDL_min(2, tiles_x - block_x);
                const int block_tiles_y = SDL_min(2, tiles_y - block_y);
                const int side = block_tiles_x == 2 || block_tiles_y == 2 ? 32 : 16;
                Uint8 tex[1 + 1024] = { 0 };
                tex[0] = (block_tiles_x == 2 ? 0x80 : 0x40) | (block_tiles_y == 2 ? 0x10 : 0x08) | (side == 32 ? 3 : 1);

                for (int y = 0; y < block_tiles_y * 16 && success; y++) {
                    for (int x = 0; x < block_tiles_x * 16; x++) {
                        const int part_x = block_x * 16 + x;
                        const int part_y = block_y * 16 + y;
                        const int source_x = flip_x ? tiles_x * 16 - 1 - part_x : part_x;
                        const int source_y = flip_y ? tiles_y * 16 - 1 - part_y : part_y;
                        const int tile = tile_base + (source_x / 16) * tiles_y + source_y / 16;
                        const size_t pixel = (size_t)tile * 256 + (source_y & 15) * 16 + (source_x & 15);

                        if (pixel >= cram_size) {
                            success = false;
                            break;
                        }

                        tex[1 + dctex_linear[x + y * 32]] = cram[pixel] & 0x3f;
                    }
                }

                if (!success || *texture_count >= UINT16_MAX || pixels->size > UINT32_MAX) {
                    success = false;
                    break;
                }

                texture_offsets[*texture_count] = (Uint32)pixels->size;
                TileMapEntry entry = { .x = (Sint16)(previous_x - (left + block_x * 16)),
                                       .y = (Sint16)(-top - block_y * 16 - previous_y),
                                       .attr = 0,
                                       .code = (Uint16)*texture_count };
                previous_x = left + block_x * 16;
                previous_y = -top - block_y * 16;
                success = append(pixels, tex, 1 + side * side) && append(&entries, &entry, sizeof(entry));
                *texture_count += 1;
                count += 1;
            }
        }
    }

    if (success && count <= UINT16_MAX) {
        const Uint16 count16 = (Uint16)count;
        success = append(placements, &count16, sizeof(count16)) && append(placements, entries.data, entries.size);
    }

    SDL_free(entries.data);
    SDL_free(cram);
    return success;
}

// Omit the unused/unmapped CGs instead of assigning speculative ROM sources.
static bool omit_cg(int group, int index) {
    switch (group) {
    case 4:
        // Unreferenced square placeholders.
        // Slots 1272–1282 were allocated separately for arcade nmca[53].
        return (index >= 1283 && index <= 1284) || (index >= 1292 && index <= 1424);
    case 10:
        // The sphere scripts establish slots 1305–1399; this next slot
        // still has no verified ROM source.
        return index == 1400;
    case 18:
        // Unused fire-tail artwork with no game CG references.
        return index >= 1424 && index <= 1425;
    default:
        return false;
    }
}

bool ArcadeTexture_BuildGroup(int group, Character character, ArcadeTextureGroup* result) {
    SDL_zero(*result);

    if (group < 1 || group > 20 || dctex_linear == NULL) {
        return false;
    }

    if (!load_attempted) {
        load_attempted = true;

        if (!load_rom()) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load CPS3 texture SIMMs");
        }
    }

    if (program == NULL || graphics == NULL) {
        return false;
    }

    const int first = texgrpdat[group].num_of_1st;
    const int total = texgrpdat[group + 1].num_of_1st - first;
    Sint32* sources = SDL_malloc(total * sizeof(*sources));
    Uint32* texture_offsets = SDL_malloc(CG_COUNT * sizeof(*texture_offsets));
    ByteBuffer placements = { 0 };
    ByteBuffer pixels = { 0 };
    int texture_count = 0;
    bool success = sources != NULL && texture_offsets != NULL && grow(&placements, total * 4);

    if (success) {
        SDL_memset(placements.data, 0, total * 4);
        placements.size = total * 4;

        for (int i = 0; i < total; i++) {
            sources[i] = -1;
        }

        for (int cg = 0; cg < CG_COUNT; cg++) {
            const int mapped = ArcadeCharData_RemapCgNumber((Uint16)cg, character);

            if (mapped >= first && mapped < first + total) {
                CgDescriptor descriptor;

                if (descriptor_for((Uint16)cg, &descriptor) && (sources[mapped - first] < 0 || cg >= 0x7000)) {
                    // The explicit 0x70xx select/bonus remaps supersede the
                    // ordinary CG occupying the same 3SX number.
                    sources[mapped - first] = (Sint32)cg;
                }
            }
        }

        for (int i = 0; i < total && success; i++) {
            if (placements.size > UINT32_MAX) {
                success = false;
                break;
            }

            ((Uint32*)placements.data)[i] = (Uint32)placements.size;

            if (sources[i] < 0 || omit_cg(group, i)) {
                const Uint16 empty = 0;
                success = append(&placements, &empty, sizeof(empty));
            } else {
                success = add_cg((Uint16)sources[i], &placements, &pixels, texture_offsets, &texture_count);
            }
        }
    }

    if (success && pixels.size <= UINT32_MAX - (size_t)texture_count * 4) {
        const size_t header_size = (size_t)texture_count * 4;
        Uint8* table = SDL_malloc(header_size + pixels.size);
        success = table != NULL;

        if (success) {
            for (int i = 0; i < texture_count; i++) {
                ((Uint32*)table)[i] = (Uint32)(header_size + texture_offsets[i]);
            }

            SDL_memcpy(table + header_size, pixels.data, pixels.size);
            result->trans_table = placements.data;
            result->trans_size = placements.size;
            result->texture_table = table;
            result->texture_size = header_size + pixels.size;
            placements.data = NULL;
            SDL_Log("CPS3 ROM textures: group %d, %d textures, %d CG slots", group, texture_count, total);
        }
    }

    SDL_free(sources);
    SDL_free(texture_offsets);
    SDL_free(placements.data);
    SDL_free(pixels.data);

    if (!success) {
        ArcadeTexture_FreeGroup(result);
    }

    return success;
}

void ArcadeTexture_FreeGroup(ArcadeTextureGroup* group) {
    SDL_free(group->trans_table);
    SDL_free(group->texture_table);
    SDL_zero(*group);
}

void ArcadeTexture_Finish() {
    SDL_free(program);
    SDL_free(graphics);
    program = NULL;
    graphics = NULL;
    load_attempted = false;
}

#endif
