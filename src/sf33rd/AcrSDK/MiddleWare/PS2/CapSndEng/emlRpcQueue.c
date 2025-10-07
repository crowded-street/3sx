#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRpcQueue.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/eflSifRpc.h"

#include <eekernel.h>
#include <memory.h>

// bss
CSE_RPCQUEUE RpcQueue[RPCQUEUE_MAX] __attribute__((aligned(64))); // size: 0x900, address: 0x6EA2C0

// sbss
u32 ixTop;        // size: 0x4, address: 0x57B2B4
u32 ixEnd;        // size: 0x4, address: 0x57B2B0
u32 num;          // size: 0x4, address: 0x57B2AC
u32 numVTransReq; // size: 0x4, address: 0x57B2A8

s32 mlRpcQueueInit() {
    ixTop = 0;
    ixEnd = 0;
    num = 0;
    numVTransReq = 0;
    return 0;
}

s32 mlRpcQueueSetData(u32 type, void* data, u32 size) {
    if (num == RPCQUEUE_MAX) {
        scePrintf("[EE]");
        scePrintf("(ERR)");
        scePrintf("mlRpcQueueSetData : Queue is full!\n");
        return -1;
    }

    RpcQueue[ixTop].Type = type;
    RpcQueue[ixTop].Size = size;
    memset(RpcQueue[ixTop].Data, 0, sizeof(RpcQueue[ixTop].Data));
    memcpy(RpcQueue[ixTop].Data, data, size);
    ixTop = (ixTop + 1) % RPCQUEUE_MAX;
    num++;

    if (type == 3) {
        numVTransReq++;
    }

    return num;
}

u32 mlRpcQueueSend() {
    CSE_RPCQUEUE_RESULT* pResult;
    s32 result;
    u32 num_loop;

    for (num_loop = num; num_loop != 0; num_loop--) {
        pResult = (CSE_RPCQUEUE_RESULT*)flSifRpcSend(RpcQueue[ixEnd].Type, RpcQueue[ixEnd].Data, RpcQueue[ixEnd].Size);

        if (pResult->result >= 0) {
            num--;

            if (RpcQueue[ixEnd].Type == 3) {
                numVTransReq--;
            }

        } else {
            result = mlRpcQueueSetData(RpcQueue[ixEnd].Type, RpcQueue[ixEnd].Data, RpcQueue[ixEnd].Size);

            if (result >= 0) {
                num--;

                if (RpcQueue[ixEnd].Type == 3) {
                    numVTransReq--;
                }

            } else {
                ixTop = (ixTop + 1) % RPCQUEUE_MAX;
            }
        }
        ixEnd = (ixEnd + 1) % RPCQUEUE_MAX;
    }

    return num;
}

u32 mlRpcQueueGetNumVtransReq() {
    return numVTransReq;
}
