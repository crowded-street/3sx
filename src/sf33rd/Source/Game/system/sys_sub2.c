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

const s16 Adjust_XY_Data[4][2] = { { 18, 12 }, { 12, 8 }, { 6, 4 }, { 0, 0 } };

void Setup_Disp_Size(s16 /* unused */) {
    s16 dixH = 3;
    s16 dixV = 3;

    Correct_X[3] = Adjust_XY_Data[dixH][0];
    Correct_Y[3] = Adjust_XY_Data[dixV][1];
    Screen_Zoom_X = 1.0f;
    Screen_Zoom_Y = 1.0f;
    Correct_X[1] = Correct_X[2] = Correct_X[3];
    Correct_Y[1] = Correct_Y[2] = Correct_Y[3];
}
