#ifndef MEMMGR_H
#define MEMMGR_H

#include "types.h"

#define MEM_NULL_HANDLE 0xFFFF

typedef struct {
    u8* ptr;
    u32 len;
    u16 align;
    u16 id;
    u16 prev;
    u16 next;
} MEM_BLOCK;

typedef struct {
    s32 cnt;
    s32 memsize;
    MEM_BLOCK* block;
    s32 direction;
    u8* memnow;
    u8* memptr;
    s32 memalign;
    s32 used_size;
    s32 tmemsize;
    u32 blocklist;
} MEM_MGR;

void plmemInit(MEM_MGR* memmgr, MEM_BLOCK* block, s32 count, void* mem_ptr, s32 memsize, s32 memalign, s32 direction);
u32 plmemRegister(MEM_MGR* memmgr, s32 len);
u32 plmemRegisterAlign(MEM_MGR* memmgr, s32 len, s32 align);
u32 plmemRegisterS(MEM_MGR* memmgr, s32 len);
void* plmemTemporaryUse(MEM_MGR* memmgr, s32 len);
void* plmemRetrieve(MEM_MGR* memmgr, u32 handle);
s32 plmemRelease(MEM_MGR* memmgr, u32 handle);
void* plmemCompact(MEM_MGR* memmgr);
u32 plmemGetSpace(MEM_MGR* memmgr);
size_t plmemGetFreeSpace(MEM_MGR* memmgr);

#endif
