#ifndef ARCADE_TEXTURE_H
#define ARCADE_TEXTURE_H

#if ARCADE_ROM_TEXTURES

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

/// Build the two tables consumed by mtrans.c from CPS3 SIMMs.
/// Omitted CGs have empty placement records. The caller owns both buffers.
bool ArcadeTexture_BuildGroup(int group, Character character, ArcadeTextureGroup* result);

void ArcadeTexture_FreeGroup(ArcadeTextureGroup* group);
void ArcadeTexture_Finish();

#endif
#endif
