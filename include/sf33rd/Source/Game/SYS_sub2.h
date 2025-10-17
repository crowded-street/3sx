#ifndef SYSSUB2_H
#define SYSSUB2_H

#include "types.h"

u8 dspwhPack(u8 xdsp, u8 ydsp);
void dspwhUnpack(u8 src, u8* xdsp, u8* ydsp);
void Setup_Disp_Size(s16 /* unused */);

#endif
