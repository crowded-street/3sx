#ifndef PRILAY_H
#define PRILAY_H

#include "plcommon.h"
#include "structs.h"
#include "types.h"

s32 plReport(s8* format, ...);
void plMemset(void* dst, u32 pat, s32 size);
void plMemmove(void* dst, void* src, s32 size);
void* plCalcAddress(s32 x, s32 y, plContext* lpcontext);
s32 plDrawPixel(plContext* dst, Pixel* ptr);
s32 plDrawPixel_3(plContext* dst, s32 x, s32 y, u32 color);
u32 plGetColor(s32 x, s32 y, plContext* lpcontext);
s32 plConvertContext(plContext* dst, plContext* src);

#endif
