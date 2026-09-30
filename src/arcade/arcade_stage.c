#include "arcade/arcade_stage.h"

/// Offset of SIMM5 in the graphics region
#define SIMM5_OFFSET 0x2000000

/// BG palette blocks in SIMM5. Each stage stores its normal block followed by a faded one of the same size.
typedef struct StagePaletteSource {
    Uint32 offset;
    size_t count;
} StagePaletteSource;

static const StagePaletteSource palette_sources[STAGE_COUNT] = {
    [STAGE_3S_SHIN_AKUMA] = { .offset = SIMM5_OFFSET + 0xF4D280, .count = 17 * 64 },
};

static Uint16* palettes[STAGE_COUNT] = { 0 };

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

void ArcadeStage_Init(const Rom* rom) {
    size_t graphics_size;
    const Uint8* graphics = Rom_GetGraphics(rom, &graphics_size);

    for (int stage = 0; stage < STAGE_COUNT; stage++) {
        const StagePaletteSource* source = &palette_sources[stage];

        if (source->count == 0) {
            continue;
        }

        if (graphics == NULL || source->offset + source->count * 2 > graphics_size) {
            SDL_Log("Couldn't build palette for stage %d: ROM has no data for it", stage);
            continue;
        }

        Uint16* palette = SDL_malloc(source->count * sizeof(Uint16));
        const Uint8* src = graphics + source->offset;

        // Graphics are stored in MAME byte order, where palette words read as little-endian
        for (size_t i = 0; i < source->count; i++) {
            palette[i] = convert_color(src[i * 2] | (src[i * 2 + 1] << 8), i);
        }

        palettes[stage] = palette;
    }
}

void ArcadeStage_Finish() {
    for (int stage = 0; stage < STAGE_COUNT; stage++) {
        SDL_free(palettes[stage]);
        palettes[stage] = NULL;
    }
}

const Uint16* ArcadeStage_GetPalette(Stage stage, size_t* count) {
    if (palettes[stage] == NULL) {
        return NULL;
    }

    *count = palette_sources[stage].count;
    return palettes[stage];
}
