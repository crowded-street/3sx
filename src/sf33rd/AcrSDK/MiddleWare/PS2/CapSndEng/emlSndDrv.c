#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlSndDrv.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlMemMap.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRefPhd.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRpcQueue.h"
#include "port/sound/emlShim.h"

// Forward decld
static s32 StartSound(CSE_PHDP* pPHDP, CSE_REQP* pREQP);

s32 mlSysSetMono(u32 mono_sw) {
    CSE_SYS_PARAM_MONO param = {};

    param.cmd = 0x10000007;
    param.mono = mono_sw & 1;

    emlShimSysSetMono(&param);

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

    emlShimSysSetVolume(&param);

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

    emlShimSeSetLfo(&param);
    return 0;
}

s32 mlSeStop(CSE_REQP* pReqp) {
    emlShimSeStop(pReqp);
    return 0;
}

s32 mlSeKeyoff(CSE_REQP* pReqp) {
    emlShimSeKeyOff(pReqp);
    return 0;
}

s32 mlSeStopAll() {
    emlShimSeStopAll();
    return 0;
}

s32 mlSeInitSndDrv() {
    emlShimInit();
    return 0;
}

s32 StartSound(CSE_PHDP* pPHDP, CSE_REQP* pREQP) {
    CSE_SYS_PARAM_SNDSTART param;

    param.cmd = 0x10000000;
    param.phdp = *pPHDP;
    param.reqp = *pREQP;

    emlShimStartSound(&param);

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
