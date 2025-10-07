#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/eflSifRpc.h"
#include "common.h"

#include <eekernel.h>
#include <memory.h>
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#if defined(TARGET_PS2)
void __assert(const s8* file, s32 line, const s8* expr);
#define assert(e) (__assert("eflSifRpc.c", 0, "0"))
#else
#include <assert.h>
#endif

// sbss
struct _sif_client_data ScdComm;  // size: 0x28, address: 0x57B220
struct _sif_client_data ScdStat;  // size: 0x28, address: 0x57B1F0
struct _sif_client_data ScdThMon; // size: 0x28, address: 0x57B1C0
struct _sif_client_data* pScd;    // size: 0x4, address: 0x57B1B0
void* pSendBuf;                   // size: 0x4, address: 0x57B1AC
void* pRecvBuf;                   // size: 0x4, address: 0x57B1A8
u32 SendBufSize;                  // size: 0x4, address: 0x57B1A4
u32 RecvBufSize;                  // size: 0x4, address: 0x57B1A0

// bss
CSE_RPCBUFF RpcBuff __attribute__((aligned(64))); // size: 0xC0, address: 0x6EA140
#if defined(TARGET_PS2)
u8 ThMonSendBuf[THMONSENDBUF_MAX] __attribute__((section(".bss"))); // size: 0x10, address: 0x6EA100
#else
u8 ThMonSendBuf[THMONSENDBUF_MAX]; // size: 0x10, address: 0x6EA100
#endif
u8 ThMonRecvBuf[THMONRECVBUF_MAX] __attribute__((aligned(256))); // size: 0x400, address: 0x6E9D00

s32 flSifRpcInit() {
#if !defined(TARGET_PS2)
    // No need to init SIF RPC on non-PS2 systems
    return 0;
#endif

    sceSifInitRpc(0);
    scePrintf("[EE]");
    scePrintf("(SYS)");
    scePrintf("Binding SIF RPC 'Comm'......");

    do {
        if (sceSifBindRpc(&ScdComm, 0x01234567, 0) < 0) {
            printf("sndtest(EE_RPC) : Error : sceSifBindRpc \n");
            assert(0);
        }

        DelayThread(0x3E8);
    } while (ScdComm.serve == NULL);

    scePrintf("[EE]");
    scePrintf("(SYS)");
    scePrintf("Done!\n");
    scePrintf("[EE]");
    scePrintf("(SYS)");
    scePrintf("Binding SIF RPC 'Stat'......");

    do {
        if (sceSifBindRpc(&ScdStat, 0x09876543, 0) < 0) {
            printf("sndtest(EE_RPC) : Error : sceSifBindRpc \n");
            assert(0);
        }

        DelayThread(0x3E8);
    } while (ScdStat.serve == NULL);

    scePrintf("[EE]");
    scePrintf("(SYS)");
    scePrintf("Done!\n");
    scePrintf("[EE]");
    scePrintf("(SYS)");
    scePrintf("Binding SIF RPC 'ThMon'......");

    do {
        if (sceSifBindRpc(&ScdThMon, 0x77755500, 0) < 0) {
            printf("sndtest(EE_RPC) : Error : sceSifBindRpc \n");
            assert(0);
        }

        DelayThread(0x3E8);
    } while (ScdThMon.serve == NULL);

    scePrintf("[EE]");
    scePrintf("(SYS)");
    scePrintf("Done!\n");
    return 0;
}

void* flSifRpcSend(u32 CmdType, void* pData, u32 DataSize) {
#if !defined(TARGET_PS2)
    // No need to send data through RPC on non-PS2 systems
    return NULL;
#endif

    s32 result;

    switch (CmdType) {
    case 0x309:
    case 3:
    case 1:
        memset(&RpcBuff, 0xFF, COMMSENDBUF_MAX);
        memset(RpcBuff.CommRecvBuf, 0xFF, COMMRECVBUF_MAX);
        memcpy(&RpcBuff, pData, MIN(DataSize, COMMSENDBUF_MAX));
        pScd = &ScdComm;
        pSendBuf = &RpcBuff;
        pRecvBuf = RpcBuff.CommRecvBuf;
        SendBufSize = COMMSENDBUF_MAX;
        RecvBufSize = COMMRECVBUF_MAX;
        break;

    case 2:
        memset(RpcBuff.StatSendBuf, 0xFF, STATSENDBUF_MAX);
        memset(RpcBuff.StatRecvBuf, 0xFF, STATRECVBUF_MAX);
        memcpy(RpcBuff.StatSendBuf, pData, MIN(DataSize, STATSENDBUF_MAX));
        pScd = &ScdStat;
        pSendBuf = RpcBuff.StatSendBuf;
        pRecvBuf = RpcBuff.StatRecvBuf;
        SendBufSize = STATSENDBUF_MAX;
        RecvBufSize = STATRECVBUF_MAX;
        break;

    case 0x63:
        memset(ThMonSendBuf, 0xFF, THMONSENDBUF_MAX);
        memset(ThMonRecvBuf, 0xFF, THMONRECVBUF_MAX);
        pScd = &ScdThMon;
        pSendBuf = ThMonSendBuf;
        pRecvBuf = ThMonRecvBuf;
        SendBufSize = THMONSENDBUF_MAX;
        RecvBufSize = THMONRECVBUF_MAX;
        break;

    default:
        printf("sndtest(EE_RPC) : Error : Unknown command.\n");
        return NULL;
    }

    while (sceSifCheckStatRpc(&pScd->rpcd) == 1) {
        // Do nothing
    }

    FlushCache(WRITEBACK_DCACHE);

    do {
        result = sceSifCallRpc(pScd, CmdType, 0, pSendBuf, SendBufSize, pRecvBuf, RecvBufSize, 0, 0);

        if (result < 0) {
            scePrintf("[EE]");
            scePrintf("(ERR)");
            scePrintf("sceSifCallRpc : error!\n");
        }

    } while (result != 0);

    return pRecvBuf;
}
