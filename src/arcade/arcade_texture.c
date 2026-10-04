#if ARCADE_ROM_TEXTURES

#include "arcade/arcade_texture.h"

#include "arcade/arcade_cg.h"
#include "arcade/arcade_char_data.h"
#include "sf33rd/Source/Game/rendering/texgroup_data.h"
#include "structs.h"

#include <SDL3/SDL.h>

#define CG_COUNT 0x10000
#define SIMM2_BASE 0x06800000

static ArcadeCgSource cg_source = { 0 };

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

    if (group < 1 || group > 20 || cg_source.program.data == NULL || cg_source.graphics.data == NULL) {
        return false;
    }

    const int first = texgrpdat[group].num_of_1st;
    const int total = texgrpdat[group + 1].num_of_1st - first;
    Sint32* sources = SDL_malloc(total * sizeof(*sources));

    if (sources == NULL) {
        return false;
    }

    for (int i = 0; i < total; i++) {
        sources[i] = -1;
    }

    for (int cg = 0; cg < CG_COUNT; cg++) {
        const int mapped = ArcadeCharData_RemapCgNumber((Uint16)cg, character);

        if (mapped >= first && mapped < first + total && ArcadeCg_Exists(&cg_source, (Uint16)cg) &&
            (sources[mapped - first] < 0 || cg >= 0x7000)) {
            // The explicit 0x70xx select/bonus remaps supersede the
            // ordinary CG occupying the same 3SX number.
            sources[mapped - first] = (Sint32)cg;
        }
    }

    for (int i = 0; i < total; i++) {
        if (omit_cg(group, i)) {
            sources[i] = -1;
        }
    }

    ArcadeCgGroup built;
    const bool success = ArcadeCg_BuildGroup(&cg_source, sources, total, 0, &built);
    SDL_free(sources);

    if (!success) {
        return false;
    }

    result->trans_table = built.trans_table;
    result->trans_size = built.trans_size;
    result->texture_table = built.texture_table;
    result->texture_size = built.texture_size;
    SDL_Log("CPS3 ROM textures: group %d, %d CG slots", group, total);
    return true;
}

void ArcadeTexture_FreeGroup(ArcadeTextureGroup* group) {
    SDL_free(group->trans_table);
    SDL_free(group->texture_table);
    SDL_zero(*group);
}

void ArcadeTexture_Init(const Rom* rom) {
    cg_source.program = Rom_GetSimm(rom, 2);
    cg_source.program_base = SIMM2_BASE;
    cg_source.table = SIMM2_BASE;
    cg_source.entry_size = 8;
    cg_source.origin_offset = 0;
    cg_source.graphics = Rom_GetGraphics(rom);
}

void ArcadeTexture_Finish() {
    SDL_zero(cg_source);
}

#endif
