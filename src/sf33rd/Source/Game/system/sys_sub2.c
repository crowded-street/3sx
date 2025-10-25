/**
 * @file sys_sub2.c
 * Display Config and Screen Adjustment Utilities
 */

#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "common.h"
#include "sf33rd/Source/Game/system/work_sys.h"

u8 dspwhPack(u8 xdsp, u8 ydsp) {
    u8 rnum = 100 - ydsp;
    rnum |= (100 - xdsp) * 16;
    return rnum;
}

void dspwhUnpack(u8 src, u8* xdsp, u8* ydsp) {
    *xdsp = 100 - ((src >> 4) & 0xF);
    *ydsp = 100 - (src & 0xF);
}
