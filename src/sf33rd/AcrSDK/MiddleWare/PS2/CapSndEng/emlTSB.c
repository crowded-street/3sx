#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlTSB.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlSndDrv.h"

#include <memory.h>
#include <stdio.h>

// Forward decls
static s32 mlTsbKeyOn(SoundEvent* pTSB, CSE_REQP* pReqp, u32 bank, u32 prog);
static void mlTsbSetToReqp(CSE_REQP* pReqp, SoundEvent* pTSB, u16 bank);
static s32 mlTsbInitEchoWork();
static s32 mlTsbMoveEchoWork();
static CSE_ECHOWORK* mlTsbPickupEchoWork(u32 index);
static s32 mlTsbCreateEcho(u32 bank, u32 code, s32* pRtpc);
static s32 mlTsbStopEcho(u32 bank, u32 code);
static s32 mlTsbStopEchoAll();

SoundEvent* gpTsb[TSB_MAX];
CSE_ECHOWORK EchoWork[ECHOWORK_MAX];

s32 mlTsbInit() {
    mlTsbInitEchoWork();
    return 0;
}

s32 mlTsbExecServer() {
    mlTsbMoveEchoWork();
    return 0;
}

s32 mlTsbStopAll() {
    mlTsbStopEchoAll();
    return 0;
}

s32 mlTsbSetBankAddr(u32 bank, SoundEvent* addr) {
    gpTsb[bank] = addr;
    return 0;
}

SoundEvent* mlTsbGetDataAdrs(u32 bank, u32 code) {
    return &gpTsb[bank][code];
}

s32 mlTsbKeyOn(SoundEvent* pTSB, CSE_REQP* pReqp, u32 bank, u32 prog) {
    if (pTSB->flags & 2) {
        pReqp->flags = pTSB->flags & ~8;
    } else {
        pReqp->flags = pTSB->flags | 8;
    }

    PlaySe(pReqp, bank, prog);
    return 0;
}

s32 mlTsbRequest(u16 bank, u16 code, s32* aRtpc) {
    CSE_REQP reqp = {};
    SoundEvent* pTSB;

    while (1) {
        pTSB = mlTsbGetDataAdrs(bank, code);
        mlTsbSetToReqp(&reqp, pTSB, bank);
        reqp.note += aRtpc[1];
        reqp.id1 += aRtpc[2];
        reqp.id2 += aRtpc[3];
        reqp.prio += aRtpc[4];
        reqp.vol += aRtpc[5];
        reqp.pan += aRtpc[6];
        reqp.pitch += aRtpc[7];
        reqp.kofftime += aRtpc[8];
        reqp.limit += aRtpc[9];

        switch (pTSB->cmd) {
        case 0:
        case 4:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        default:
            // Do nothing
            break;

        case 6:
            mlTsbCreateEcho(bank, code, aRtpc);
            break;

        case 7:
            mlTsbStopEcho(bank, code);
            break;

        case 1:
            mlTsbKeyOn(pTSB, &reqp, bank, pTSB->prog + aRtpc[0]);
            break;

        case 2:
            mlSeKeyoff(&reqp);
            break;

        case 3:
            mlSeStop(&reqp);
            break;

        case 5:
            mlSeSetLfo(&reqp, pTSB->param0, pTSB->param1, pTSB->param2, pTSB->param3);
            break;
        }

        if (((pTSB->link) != 0xFFFF) && (pTSB->link != code)) {
            code = (u16)pTSB->link;
        } else {
            break;
        }
    }
    return 0;
}

void mlTsbSetToReqp(CSE_REQP* pReqp, SoundEvent* pTSB, u16 bank) {
    pReqp->flags = pTSB->flags;
    pReqp->bank = bank;
    pReqp->note = pTSB->note;
    pReqp->vol = pTSB->vol;
    pReqp->pan = pTSB->pan;
    pReqp->pitch = pTSB->pitch;
    pReqp->prio = pTSB->prio;
    pReqp->id1 = pTSB->id1;
    pReqp->id2 = pTSB->id2;
    pReqp->kofftime = pTSB->kofftime;
    pReqp->attr = pTSB->attr;
    pReqp->limit = pTSB->limit;
}

s32 mlTsbInitEchoWork() {
    u32 i;

    for (i = 0; i < ECHOWORK_MAX; i++) {
        memset(&EchoWork[i], 0, sizeof(CSE_ECHOWORK));
    }

    return 0;
}

s32 mlTsbMoveEchoWork() {
    u32 i;
    CSE_ECHOWORK* pEchoWork;
    SoundEvent* pTSB;
    CSE_REQP reqp = {};

    for (i = 0; i < ECHOWORK_MAX; i++) {
        pEchoWork = &EchoWork[i];

        if (pEchoWork->BeFlag == 1) {
            pEchoWork->CurrInterval--;

            if (pEchoWork->CurrInterval == 0) {
                pTSB = mlTsbGetDataAdrs(pEchoWork->Bank, pEchoWork->Code);
                mlTsbSetToReqp(&reqp, pTSB, pEchoWork->Bank);
                reqp.note += pEchoWork->Rtpc[1];
                reqp.id1 += pEchoWork->Rtpc[2];
                reqp.id2 += pEchoWork->Rtpc[3];
                reqp.prio += pEchoWork->Rtpc[4];
                reqp.vol += pEchoWork->Rtpc[5];
                reqp.pan += pEchoWork->Rtpc[6];
                reqp.pitch += pEchoWork->Rtpc[7];
                reqp.kofftime += pEchoWork->Rtpc[8];
                reqp.limit += pEchoWork->Rtpc[9];
                mlTsbKeyOn(pTSB, &reqp, pEchoWork->Bank, (pTSB->prog + pEchoWork->Rtpc[0]));
                pEchoWork->CurrTimes--;

                if (pEchoWork->CurrTimes == 0) {
                    pEchoWork->BeFlag = 0;
                } else {
                    pEchoWork->CurrInterval = pEchoWork->Interval;

                    if (pEchoWork->CurrTimes == (pEchoWork->Times - 1)) {
                        pEchoWork->Rtpc[5] -= pEchoWork->VolDec1st;
                    } else {
                        pEchoWork->Rtpc[5] -= pEchoWork->VolDec;
                    }

                    if (pEchoWork->Rtpc[5] < -127) {
                        pEchoWork->BeFlag = 0;
                    }
                }
            }
        }
    }

    return 0;
}

CSE_ECHOWORK* mlTsbPickupEchoWork(u32 index) {
    memset(&EchoWork[index], 0, sizeof(CSE_ECHOWORK));
    EchoWork[index].BeFlag = 1;
    return &EchoWork[index];
}

s32 mlTsbCreateEcho(u32 bank, u32 code, s32* pRtpc) {
    CSE_ECHOWORK* pEchoWork;
    SoundEvent* pTsb;

    pTsb = mlTsbGetDataAdrs(bank, code);
    pEchoWork = mlTsbPickupEchoWork(pTsb->id1 & 0xF);

    if (pEchoWork == NULL) {
        return -1;
    }

    pEchoWork->Bank = bank;
    pEchoWork->Code = code;
    pEchoWork->Interval = pTsb->param0;
    pEchoWork->VolDec1st = pTsb->param1;
    pEchoWork->VolDec = pTsb->param2;
    pEchoWork->Times = pTsb->param3;
    pEchoWork->CurrInterval = 1;
    pEchoWork->CurrTimes = pEchoWork->Times;
    memcpy(pEchoWork->Rtpc, pRtpc, sizeof(pEchoWork->Rtpc));
    return 0;
}

s32 mlTsbStopEcho(u32 bank, u32 code) {
    SoundEvent* pTsb = mlTsbGetDataAdrs(bank, code);
    EchoWork[pTsb->id1 & 0xF].BeFlag = 0;
    return 0;
}

s32 mlTsbStopEchoAll() {
    u32 i;

    for (i = 0; i < ECHOWORK_MAX; i++) {
        EchoWork[i].BeFlag = 0;
    }

    return 0;
}
