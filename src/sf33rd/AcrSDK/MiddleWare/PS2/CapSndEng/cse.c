#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "common.h"
#include "port/sound/spu.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/eflSifRpc.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlMemMap.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRpcQueue.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlSndDrv.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlTSB.h"

#include <assert.h>
#include <stdio.h>

static CSE_SYSWORK cseSysWork __attribute__((aligned(16))); // size: 0x48, address: 0x57B260

s32 cseInitSndDrv() {
    u32 i;

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
        cseSysWork.Counter++;
        return 0;
    }

    printf("[EE]");
    printf("(DBG)");
    //"System not initialized\n"
    printf("システムが初期化されていない\n");
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

s32 cseSendBd2SpuWithId(void* ee_addr, u32 size, u32 bank, u32 id) {
    CSE_SPUID_PARAM param = {};
    bank &= 0xF;

    if (cseSysWork.SpuBankId[bank] != id) {
        cseSysWork.SpuBankId[bank] = id;
        param.cmd = 0x30000000;
        param.e_addr = (uintptr_t)ee_addr;
        param.s_addr = mlMemMapGetBankAddr(bank);
        param.size = size;

        SPU_Upload(param.s_addr, ee_addr, size);
    }

    return 0;
}

u32 cseGetIdStoredBd(u32 bank) {
    return cseSysWork.SpuBankId[bank & 0xF];
}

s32 cseMemMapSetPhdAddr(u32 bank, void* addr) {
    return mlMemMapSetPhdAddr(bank, addr);
}

s32 cseTsbSetBankAddr(u32 bank, SoundEvent* addr) {
    return mlTsbSetBankAddr(bank, addr);
}

s32 cseSeStopAll() {
    mlTsbStopAll();
    mlSeStopAll();
    return 0;
}

s32 cseSysSetMasterVolume(s32 vol) {
    return mlSysSetMasterVolume(vol);
}

s32 cseSysSetMono(u32 mono_sw) {
    return mlSysSetMono(mono_sw);
}
