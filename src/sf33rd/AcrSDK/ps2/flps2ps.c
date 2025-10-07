#include "sf33rd/AcrSDK/ps2/flps2ps.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2asm.h"
#include "sf33rd/AcrSDK/ps2/flps2dma.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

s32 flPS2psAddQueue(QWORD* dma_data) {
    u32 qwc;
    u32* dst;

    qwc = (dma_data->I32[0] & 0x7FFF) + 1;
    dst = (u32*)flPS2GetSystemTmpBuff(qwc * 0x10, 0x10);
    dma_data->I32[0] = dma_data->I32[0] | 0x80000000;
    dma_data->I32[2] = 0x13000000;
    dma_data->I32[3] = (qwc - 1) | 0x51000000;
    flPS2_Mem_move16_16A(dma_data, (void*)dst, qwc);
    flPS2DmaAddQueue2(0, ((uintptr_t)dst & 0x0FFFFFFF) | 0x40000000, (uintptr_t)dst, &flPs2VIF1Control);
    return 1;
}
