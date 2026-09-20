#ifndef DEMO00_H
#define DEMO00_H

#include "types.h"

s32 Warning();
void Warning_Init();
void Put_Warning(s16 type);
void Pal_Cursor_Put(s16 type);
s32 CAPCOM_Logo();
/* The seven values Put_char took, in its own parameter order. */
typedef struct {
    const f32* ptr;
    u32 indexG;
    u16 prio;
    s16 x;
    s16 y;
    f32 zx;
    f32 zy;
} PutCharArgs;

void Put_char(const PutCharArgs* a);

#endif
