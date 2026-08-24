#ifndef TEXGROUP_DATA_H
#define TEXGROUP_DATA_H

#include "types.h"

typedef struct {
    u16 num_of_1st;
    s16 apfn;
    s16 conv;
    s16 ix1st;
    u32 to_tex;
    u32 to_chd;
} TexGroupData;

extern const TexGroupData texgrpdat[100];

#endif
