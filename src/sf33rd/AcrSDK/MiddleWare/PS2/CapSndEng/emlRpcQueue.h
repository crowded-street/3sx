#ifndef EFL_RPC_QUEUE_H
#define EFL_RPC_QUEUE_H

#include "structs.h"
#include "types.h"

#define RPCQUEUE_DATA_MAX 64
#define RPCQUEUE_MAX 32

typedef struct {
    u32 Type;
    u8 Data[RPCQUEUE_DATA_MAX];
    u32 Size;
} CSE_RPCQUEUE;

extern CSE_RPCQUEUE RpcQueue[RPCQUEUE_MAX];
extern u32 ixTop;
extern u32 ixEnd;
extern u32 num;
extern u32 numVTransReq;

s32 mlRpcQueueInit();
s32 mlRpcQueueSetData(u32 type, void* data, u32 size);
u32 mlRpcQueueSend();
u32 mlRpcQueueGetNumVtransReq();

#endif
