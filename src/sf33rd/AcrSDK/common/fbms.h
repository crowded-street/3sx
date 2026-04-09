#ifndef FBMS_H
#define FBMS_H

#include "structs.h"
#include "types.h"

s32 fmsCalcSpace(FL_FMS* lp);
s32 fmsInitialize(FL_FMS* lp, void* memory_ptr, s32 memsize, s32 memalign);
void* fmsAllocMemory(FL_FMS* lp, s32 bytes, s32 heapnum);
s32 fmsGetFrame(FL_FMS* lp, s32 heapnum, FMS_FRAME* frame);

#endif
