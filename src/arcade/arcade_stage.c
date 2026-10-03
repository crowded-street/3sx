#include "arcade/arcade_stage.h"
#include "port/io/cache.h"

/// Offset of SIMM5 in the graphics region
#define SIMM5_OFFSET 0x2000000

/// BG palette blocks in SIMM5. Each stage stores its normal block followed by a faded one of the same size.
typedef struct StagePaletteSource {
    Uint32 offset;
    size_t count;
    CacheFile file;
} StagePaletteSource;

static const StagePaletteSource palette_sources[] = {
    { .offset = SIMM5_OFFSET + 0xF4D280, .count = 17 * 64, .file = CACHE_FILE_SHIN_AKUMA_BG_PALETTE },
};

static bool shin_akuma_stage_available = false;

/// Converts a CPS3 xRGB555 color to the PS2 file format: xBGR555 with bit 15 set on everything
/// except the transparent color that starts each 64-color row.
static Uint16 convert_color(Uint16 color, size_t index) {
    color &= 0x7FFF;
    const Uint16 swapped = ((color & 0x1F) << 10) | (color & 0x3E0) | (color >> 10);

    if (index % 64 == 0 && swapped == 0) {
        return 0;
    }

    return swapped | 0x8000;
}

static void write_palette(const StagePaletteSource* source, const Uint8* graphics, size_t graphics_size) {
    if (graphics == NULL || source->offset + source->count * 2 > graphics_size) {
        SDL_Log("Couldn't build %s: ROM has no data for it", Cache_GetPath(source->file));
        return;
    }

    Uint16* palette = SDL_malloc(source->count * sizeof(Uint16));
    const Uint8* src = graphics + source->offset;

    for (size_t i = 0; i < source->count; i++) {
        palette[i] = SDL_Swap16LE(convert_color(src[i * 2] | (src[i * 2 + 1] << 8), i));
    }

    Cache_Write(source->file, palette, source->count * sizeof(Uint16));
    SDL_free(palette);
}

void ArcadeStage_Init(const Rom* rom) {
    if (rom != NULL) {
        size_t graphics_size;
        const Uint8* graphics = Rom_GetGraphics(rom, &graphics_size);

        for (int i = 0; i < SDL_arraysize(palette_sources); i++) {
            write_palette(&palette_sources[i], graphics, graphics_size);
        }
    }

    shin_akuma_stage_available = Cache_Exists(CACHE_FILE_SHIN_AKUMA_BG_PALETTE);
}

bool ArcadeStage_IsShinAkumaStageAvailable() {
    return shin_akuma_stage_available;
}
