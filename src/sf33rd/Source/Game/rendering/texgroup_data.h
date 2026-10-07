#ifndef TEXGROUP_DATA_H
#define TEXGROUP_DATA_H

#include "types.h"

typedef enum TexGroupMode {
    /// Load textures.
    TEXGROUP_MODE_NORMAL,

    /// Load textures and character data.
    /// Can re-use resources loaded by the other character.
    TEXGROUP_MODE_CHARACTER,

    /// Load textures.
    /// Can re-use resources loaded by the other character.
    TEXGROUP_MODE_SHARED,
} TexGroupMode;

#define TEXGROUP_COUNT 105

typedef struct TexGroupData {
    u16 num_of_1st;
    s32 apfn;
    TexGroupMode mode;
    u32 to_tex;
    u32 to_chd;
} TexGroupData;

extern const TexGroupData texgrpdat[TEXGROUP_COUNT];

#endif
