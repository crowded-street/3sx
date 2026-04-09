#ifndef KNJSUB_H
#define KNJSUB_H

#include "structs.h"
#include "types.h"

void KnjInit(u32 type, uintptr_t adrs, u32 disp_max, u32 top_dbp);
void KnjFinish();
void KnjSetSize(s32 dispw, s32 disph);
void KnjLocate(s32 x, s32 y);
void KnjSetColor(u32 color);
void KnjSetAlpha(u32 alpha);
void KnjSetRgb(u32 color);
void KnjPuts(const s8* str);
void KnjPrintf(const s8* fmt, ...);
void KnjFlush();
s32 KnjStrLen(const s8* str);
s32 KnjCheckCode(const s8* str);

#endif
