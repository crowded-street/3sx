#ifndef FLPS2ETC_H
#define FLPS2ETC_H

#include "structs.h"
#include "types.h"

s32 flFileRead(s8* filename, void* buf, s32 len);
s32 flFileWrite(s8* filename, void* buf, s32 len);
s32 flFileAppend(s8* filename, void* buf, ssize_t len);
s32 flFileLength(s8* filename);
void flMemset(void* dst, u32 pat, s32 size);
void flMemcpy(void* dst, void* src, s32 size);
void* flAllocMemory(s32 size);
s32 flGetFrame(FMS_FRAME* frame);
s32 flGetSpace();
void* flAllocMemoryS(s32 size);
u32 flPS2GetSystemMemoryHandle(s32 len, s32 type);
void flPS2ReleaseSystemMemory(u32 handle);
void* flPS2GetSystemBuffAdrs(u32 handle);
void flPS2SystemTmpBuffInit();
void flPS2SystemTmpBuffFlush();
uintptr_t flPS2GetSystemTmpBuff(s32 len, s32 align);
u32 flCreateTextureFromFile(s8* file, u32 flag);

#endif // FLPS2ETC_H
