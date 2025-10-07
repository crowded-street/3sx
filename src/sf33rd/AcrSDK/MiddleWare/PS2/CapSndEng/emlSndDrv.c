#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlSndDrv.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/eflSifRpc.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlMemMap.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRefPhd.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRpcQueue.h"

#if !defined(TARGET_PS2)
#include "port/sound/emlShim.h"
#endif

#include <eekernel.h>

s32 mlSysSetMono(u32 mono_sw) {
    CSE_SYS_PARAM_MONO param = {};

    param.cmd = 0x10000007;
    param.mono = mono_sw & 1;
    mlRpcQueueSetData(1, &param, sizeof(CSE_SYS_PARAM_MONO));
    return 0;
}

s32 mlSysSetMasterVolume(s32 vol) {
    return mlSysSetBankVolume(0xFF, vol);
}

s32 mlSysSetBankVolume(s32 bank, s32 vol) {
    CSE_SYS_PARAM_BANKVOL param = {};

    param.cmd = 0x10000009;
    param.bank = bank == 0xFF ? bank : bank & 0xF;
    param.vol = vol & 0x7F;
#if defined(TARGET_PS2)
    mlRpcQueueSetData(1, &param, sizeof(CSE_SYS_PARAM_BANKVOL));
#else
    emlShimSysSetVolume(&param);
#endif
    return 0;
}

s32 mlSeSetLfo(CSE_REQP* pReqp, u16 pmd_speed, u16 pmd_depth, u16 amd_speed, u16 amd_depth) {
    CSE_SYS_PARAM_LFO param = {};

    param.cmd = 0x10000004;
    param.reqp = *pReqp;
    param.pmd_speed = pmd_speed;
    param.pmd_depth = pmd_depth;
    param.amd_speed = amd_speed;
    param.amd_depth = amd_depth;
    mlRpcQueueSetData(1, &param, sizeof(CSE_SYS_PARAM_LFO));
    return 0;
}

s32 mlSeStop(CSE_REQP* pReqp) {
#if defined(TARGET_PS2)
    return SendSeChange(pReqp, 0x10000002);
#else
    emlShimSeStop(pReqp);
    return 0;
#endif
}

s32 mlSeKeyoff(CSE_REQP* pReqp) {
#if defined(TARGET_PS2)
    return SendSeChange(pReqp, 0x10000001);
#else
    emlShimSeKeyOff(pReqp);
    return 0;
#endif
}

s32 mlSeStopAll() {
    CSE_REQP reqp = {};
#if defined(TARGET_PS2)
    return SendSeChange(&reqp, 0x10000002);
#else
    emlShimSeStopAll();
    return 0;
#endif
}

s32 mlSeInitSndDrv() {
#if defined(TARGET_PS2)
    flSifRpcSend(0x309, NULL, 0);
#else
    emlShimInit();
#endif
    return 0;
}

s32 StartSound(CSE_PHDP* pPHDP, CSE_REQP* pREQP) {
    CSE_SYS_PARAM_SNDSTART param;

    param.cmd = 0x10000000;
    param.phdp = *pPHDP;
    param.reqp = *pREQP;
#if defined(TARGET_PS2)
    mlRpcQueueSetData(1, &param, sizeof(CSE_SYS_PARAM_SNDSTART));
#else
    emlShimStartSound(&param);
#endif
    return 0;
}

s32 PlaySe(CSE_REQP* pReqp, u16 bank, u16 prog) {
    _ps2_head_chunk* pHEAD;
    CSE_PHDPADDR PhdPAddr = {};
    CSE_PHDP phdp;
    s32 NumSplit;
    s32 i;
    s32 result;

    pHEAD = mlMemMapGetPhdAddr(bank);
    NumSplit = GetNumSplit(pHEAD, prog);

    for (i = 0; i < NumSplit; i++) {
        result = GetPhdParam(&PhdPAddr, pHEAD, prog, pReqp->note, i);

        if (result >= 0) {
            CalcPhdParam(&phdp, &PhdPAddr, pReqp->note, mlMemMapGetBankAddr(bank));
            StartSound(&phdp, pReqp);
        }
    }

    return 0;
}

s32 CheckReqFlags(CSE_REQP* pReqp) {
    if (pReqp->flags & 1) {
        pReqp->prio &= 0x7F;
    } else {
        pReqp->flags |= 1;
        pReqp->prio = 0x7F;
    }

    if (pReqp->flags & 0x20) {
        if (!(pReqp->flags & 0xE)) {
            return -1;
        }
    }

    else if (pReqp->flags & 0x40) {
        return -1;
    }

    if (pReqp->flags & 0x10) {
        pReqp->note &= 0x7F;
    }

    return 0;
}

s32 SendSeChange(CSE_REQP* pReqp, s32 cmd) {
    CSE_SYS_PARAM_SECHANGE param = {};

    switch (cmd) {
    default:
        scePrintf("[EE]");
        scePrintf("(DBG)");
        scePrintf(" : Match UNKNOWN Command!?\n");
        return -1;

    case 0x10000003:
    case 0x10000002:
    case 0x10000001:
        param.cmd = cmd;
        param.reqp = *pReqp;

        if (CheckReqFlags(&param.reqp) == -1) {
            return -1;
        }

        mlRpcQueueSetData(1, &param, sizeof(CSE_SYS_PARAM_SECHANGE));
        return 0;
    }
}
