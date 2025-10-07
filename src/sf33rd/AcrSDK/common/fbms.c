#include "common.h"
#include "structs.h"

ptrdiff_t fmsCalcSpace(FL_FMS* lp) {
    return lp->frame[1] - lp->frame[0];
}

s32 fmsInitialize(FL_FMS* lp, void* memory_ptr, s32 memsize, s32 memalign) {
    memsize = ~(memalign - 1) & (memsize + memalign - 1);
    lp->memoryblock = (u8*)memory_ptr;
    lp->align = memalign;
    lp->baseandcap[0] = (u8*)((uintptr_t)lp->memoryblock + lp->align - 1 & ~(lp->align - 1));
    lp->baseandcap[1] = (u8*)(((uintptr_t)lp->memoryblock + memsize + lp->align - 1) & ~(lp->align - 1));
    lp->frame[0] = lp->baseandcap[0];
    lp->frame[1] = lp->baseandcap[1];
    return 1;
}

void* fmsAllocMemory(FL_FMS* lp, s32 bytes, s32 heapnum) {
    void* pMem;
    bytes = ~(lp->align - 1) & ((uintptr_t)bytes + lp->align - 1);

    if (lp->frame[0] + bytes > lp->frame[1]) {
        return NULL;
    }

    if (heapnum != 0) {
        lp->frame[1] -= bytes;
        pMem = lp->frame[1];
    } else {
        pMem = lp->frame[0];
        lp->frame[0] += bytes;
    }

    return pMem;
}

s32 fmsGetFrame(FL_FMS* lp, s32 heapnum, FMS_FRAME* frame) {
    frame->pFrame = lp->frame[heapnum];
    frame->heapnum = heapnum;
    return 1;
}
