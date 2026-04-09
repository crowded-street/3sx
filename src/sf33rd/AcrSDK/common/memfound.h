#ifndef MEMFOUND_H
#define MEMFOUND_H

#include "sf33rd/AcrSDK/common/memmgr.h"
#include "structs.h"
#include "types.h"

extern MEM_BLOCK sysmemblock[4096];

void mflInit(void* mem_ptr, s32 memsize, s32 memalign);
u32 mflGetSpace();
size_t mflGetFreeSpace();
u32 mflRegisterS(s32 len);
u32 mflRegister(s32 len);
void* mflTemporaryUse(s32 len);
void* mflRetrieve(u32 handle);
s32 mflRelease(u32 handle);
void* mflCompact();

#endif
