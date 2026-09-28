#ifndef ARCADE_TEXTURE_H
#define ARCADE_TEXTURE_H

#if ARCADE_ROM_TEXTURES

#include "arcade/rom/rom.h"
#include "constants.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ArcadeTextureGroup {
    void* trans_table;
    size_t trans_size;
    void* texture_table;
    size_t texture_size;
} ArcadeTextureGroup;

/// Must be called before building groups. `rom` must stay alive until `ArcadeTexture_Finish`.
void ArcadeTexture_Init(const Rom* rom);

/// Build the two tables consumed by mtrans.c from CPS3 SIMMs.
/// Omitted CGs have empty placement records. The caller owns both buffers.
bool ArcadeTexture_BuildGroup(int group, Character character, ArcadeTextureGroup* result);

void ArcadeTexture_FreeGroup(ArcadeTextureGroup* group);
void ArcadeTexture_Finish();

#endif
#endif
