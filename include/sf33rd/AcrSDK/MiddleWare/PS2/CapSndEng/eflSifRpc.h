#ifndef EFL_SIF_RPC_H
#define EFL_SIF_RPC_H

#include "structs.h"
#include "types.h"

#include <sifrpc.h>

#define THMONSENDBUF_MAX 16
#define THMONRECVBUF_MAX 1024
#define COMMSENDBUF_MAX 64
#define COMMRECVBUF_MAX 64
#define STATSENDBUF_MAX 32
#define STATRECVBUF_MAX 32

typedef struct {
    // total size: 0xC0
    u8 CommSendBuf[COMMSENDBUF_MAX]; // offset 0x0, size 0x40
    u8 CommRecvBuf[COMMRECVBUF_MAX]; // offset 0x40, size 0x40
    u8 StatSendBuf[STATSENDBUF_MAX]; // offset 0x80, size 0x20
    u8 StatRecvBuf[STATRECVBUF_MAX]; // offset 0xA0, size 0x20
} CSE_RPCBUFF;

extern struct _sif_client_data ScdComm;                  // size: 0x28, address: 0x57B220
extern struct _sif_client_data ScdStat;                  // size: 0x28, address: 0x57B1F0
extern struct _sif_client_data ScdThMon;                 // size: 0x28, address: 0x57B1C0
extern struct _sif_client_data* pScd;                    // size: 0x4, address: 0x57B1B0
extern void* pSendBuf;                                   // size: 0x4, address: 0x57B1AC
extern void* pRecvBuf;                                   // size: 0x4, address: 0x57B1A8
extern u32 SendBufSize;                                  // size: 0x4, address: 0x57B1A4
extern u32 RecvBufSize;                                  // size: 0x4, address: 0x57B1A0
extern CSE_RPCBUFF RpcBuff __attribute__((aligned(64))); // size: 0xC0, address: 0x6EA140

extern u8 ThMonSendBuf[THMONSENDBUF_MAX]; // size: 0x10, address: 0x6EA100

extern u8 ThMonRecvBuf[THMONRECVBUF_MAX] __attribute__((aligned(256))); // size: 0x400, address: 0x6E9D00

s32 flSifRpcInit();
void* flSifRpcSend(u32 CmdType, void* pData, u32 DataSize);

#endif
