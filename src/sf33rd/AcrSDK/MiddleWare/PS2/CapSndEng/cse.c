#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/eflSifRpc.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlMemMap.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRpcQueue.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlSndDrv.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlTSB.h"

#if !defined(TARGET_PS2)
#include "port/sound/spu.h"
#endif

#include <eekernel.h>

static CSE_SYSWORK cseSysWork __attribute__((aligned(16))); // size: 0x48, address: 0x57B260

#if defined(TARGET_PS2)
void __assert(const s8* file, s32 line, const s8* expr);
#define assert(e) (__assert("cse.c", 0, "0"))
#else
#include <assert.h>
#endif

s32 cseInitSndDrv() {
    u32 i;

    flSifRpcInit();
    mlRpcQueueInit();
    mlSeInitSndDrv();
    mlTsbInit();
    cseSysWork.InitializeFlag = 1;
    cseSysWork.Counter = 0;

    for (i = 0; i < SPUBANKID_MAX; i++) {
        cseSysWork.SpuBankId[i] = -1;
    }

    return 0;
}

s32 cseExecServer() {
    if (cseSysWork.InitializeFlag == 1) {
        mlTsbExecServer();
#if defined(TARGET_PS2)
        mlRpcQueueSend();
#endif
        cseSysWork.Counter++;
        return 0;
    }

    scePrintf("[EE]");
    scePrintf("(DBG)");
    //"System not initialized\n"
    scePrintf("システムが初期化されていない\n");
    return -1;
}

s32 cseTsbRequest(u16 bank, u16 code, s32 NumArgSets, ...) {
    s32 rtpc[10] = {};
    s32 i;
    s32 cmd;
    s32 prm;

    va_list vlist;
    va_start(vlist, NumArgSets);

    for (i = 0; i < NumArgSets; i++) {
        cmd = va_arg(vlist, int);
        prm = va_arg(vlist, int);

        if (cmd < 10) {
            rtpc[cmd] += prm;
        }
    }

    return mlTsbRequest(bank, code, rtpc);
}

s32 cseCheckVTransStatus(u32 vtrans_check_type) {
#if defined(SOUND_DISABLED)
    return 0;
#endif

    CSE_RPCQUEUE_RESULT param;
    CSE_VTRANS_RESULT* pResult;
    u32 numReqInQueue;
    s32 result;

    param.result = 0x20000000;
    pResult = flSifRpcSend(2, &param, sizeof(CSE_RPCQUEUE_RESULT));
    numReqInQueue = mlRpcQueueGetNumVtransReq();
    result = 0;

    if (numReqInQueue != 0) {
        result |= 1;
    }

    if (pResult->dma_remain != 0) {
        result |= 0x10;
    }

    if (pResult->sifdma_move != 0) {
        result |= 0x100;
    }

    if (pResult->vtrans_move != 0) {
        result |= 0x1000;
    }

    switch (vtrans_check_type) {
    case 0:
        if (result == 0) {
            result = 0;
        } else {
            result |= 0x80000000;
        }
        break;

    case 1:
        if (!(result & 0x111)) {
            result = 0;
        } else {
            result |= 0x80000000;
        }
        break;

    default:
        scePrintf("[EE]");
        scePrintf("(ERR)");
        scePrintf("[C.S.E] Unknown parameter in cseCheckVtransStatus().\n");
        assert(0);
        break;
    }

    return result;
}

s32 cseSendBd2SpuWithId(void* ee_addr, u32 size, u32 bank, u32 id) {
    CSE_SPUID_PARAM param = {};
    bank &= 0xF;

    if (cseSysWork.SpuBankId[bank] != id) {
        cseSysWork.SpuBankId[bank] = id;
        param.cmd = 0x30000000;
        param.e_addr = (uintptr_t)ee_addr;
        param.s_addr = mlMemMapGetBankAddr(bank);
        param.size = size;
#if defined(TARGET_PS2)
        mlRpcQueueSetData(3, &param, sizeof(CSE_SPUID_PARAM));
#else
        SPU_Upload(param.s_addr, ee_addr, size);
#endif
    }

    return 0;
}

u32 cseGetIdStoredBd(u32 bank) {
    return cseSysWork.SpuBankId[bank & 0xF];
}
