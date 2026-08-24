#ifndef MEMMAN_H
#define MEMMAN_H

#include "structs.h"
#include "types.h"

void mmSystemInitialize();
void mmHeapInitialize(_MEMMAN_OBJ* mmobj, u8* adrs, s32 size, s32 unit, s8* format);
uintptr_t mmRoundUp(s32 unit, uintptr_t num);
uintptr_t mmRoundOff(s32 unit, uintptr_t num);
ssize_t mmGetRemainder(_MEMMAN_OBJ* mmobj);
ssize_t mmGetRemainderMin(_MEMMAN_OBJ* mmobj);
void* mmAlloc(_MEMMAN_OBJ* mmobj, ssize_t size, s32 flag);
struct _MEMMAN_CELL* mmAllocSub(_MEMMAN_OBJ* mmobj, ssize_t size, s32 flag);
void mmFree(_MEMMAN_OBJ* mmobj, void* adrs);

#endif
