/**
 * @file se.c
 * Sound Effect and Background Music Request Handler
 */

#include "sf33rd/Source/Game/sound/se.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sound/se_data.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "structs.h"

#define SDEB_SIZE 8

u8 gSeqStatus[1];
SoundPatch sdeb[SDEB_SIZE];

s16 bgm_selectorDC[8] = { 0, 1, 2, 1, 2, 1, 2, 1 };
s16 bgm_selectorAC[8] = { 0, 1, 0, 1, 0, 1, 0, 1 };
s16* bgm_selector[2] = { bgm_selectorDC, bgm_selectorAC };

const u16 BGM_Stage_Data[22] = { 46, 1, 13, 34, 31, 4, 7, 16, 25, 28, 34, 1, 28, 43, 22, 10, 19, 40, 4, 37, 61, 62 };
const s16 SE_Shock_Data[7] = { 285, 286, 287, 288, 289, 305, 306 };
const s16 Finish_SE_Data[2][7] = { { 305, 306, 285, 286, 287, 288, 272 }, { 292, 293, 290, 291, 287, 288, 272 } };

void Stage_BGM(u16 Stage_Number, u16 Round_Number) {
    u16 code;

    if (Mode_Type == MODE_ARCADE && Play_Type == 0 && My_char[COM_id] == 17 && Bonus_Game_Flag == 0) {
        code = BGM_Stage_Data[17] + bgm_selector[sys_w.bgm_type][Round_Number & 7];
    } else {
        code = BGM_Stage_Data[Stage_Number] + bgm_selector[sys_w.bgm_type][Round_Number & 7];
    }

    *gSeqStatus = 0;

    if (code == 0x2E && gill_appear_check() == 0) {
        Go_BGM();
        return;
    }

    SsRequest(code);
}

void Sound_SE(s16 Code) {
    SsRequest(Code);
}

void BGM_Request(s16 Code) {
    SsRequest(Code);
}

void BGM_Request_Code_Check(u16 Code) {
    SsRequest_CC(Code);
}

void BGM_Stop() {
    SsBgmOff();
}

void SE_All_Off() {
    spu_all_off();
}

void Se_Dummy(WORK_Other* ewk, u16 Code) {
    SoundPatchConfig rmc;

    rmc.ptix = 0;
    rmc.bank = 0;
    rmc.port = 0;
    rmc.code = 0;

    Store_Sound_Code(Code, &rmc);
}

void Se_Shock(WORK_Other* ewk, u16 Code) {
    PLW* em;
    s16 xx;
    s16 zz;
    s16 uid;

    s16 assign1;

    Code = Check_Bonus_SE(Code);

    if (ewk->wu.work_id == 1) {
        em = (PLW*)ewk->wu.target_adrs;
        uid = ewk->wu.id;
    } else {
        em = (PLW*)((PLW*)ewk->my_master)->wu.target_adrs;
        uid = ewk->master_id;
    }

    if (em->wu.work_id == 1 && em->wu.vital_new < 0) {
        for (xx = 0, assign1 = zz = 0x27; xx < 7; xx++) {
            if (Code == SE_Shock_Data[xx]) {
                zz = 0;
                break;
            }
        }
        Code += zz;
    }

    if (Code) {
        Code += uid * 0x300;
    }

    xx = Get_Position((PLW*)ewk);
    SsRequestPan(Code, xx, xx, 0, 2);
}

void Se_Myself(WORK_Other* ewk, u16 Code) {
    s16 xx;
    s16 uid = ewk->wu.id;

    if ((ewk->wu.work_id == 1) || (uid = (ewk->master_id), uid < 2)) {
        if (Code) {
            Code += uid * 0x300;
        }

        xx = Get_Position((PLW*)ewk);
        SsRequestPan(Code, xx, xx, 0, 2);
    }
}

void Se_Myself_Die(WORK_Other* ewk, u16 Code) {
    s16 xx;

    if ((ewk->wu.work_id == 1) && (ewk->wu.vital_new >= 0)) {
        if (Code) {
            Code += ewk->wu.id * 0x300;
        }
        xx = Get_Position((PLW*)ewk);
        SsRequestPan(Code, xx, xx, 0, 2);
    }
}

void Se_Let(WORK_Other* ewk, u16 Code) {
    s16 xx;
    s16 uid;

    Code = Check_Bonus_SE(Code);

    if (ewk->wu.work_id == 1) {
        uid = ewk->wu.id;
    } else {
        uid = ewk->master_id;
    }

    if (Code) {
        Code += uid * 0x300;
    }

    xx = Get_Position((PLW*)ewk);
    SsRequestPan(Code, xx, xx, 0, 2);
}

void Se_Let_SP(WORK_Other* ewk, u16 Code) {
    PLW* em;
    s16 xx;
    s16 uid;

    if (ewk->wu.work_id == 1) {
        uid = ewk->wu.id;
    } else {
        uid = ewk->master_id;
    }

    em = (PLW*)ewk->wu.target_adrs;

    if ((em->wu.work_id == 1) && (em->wu.vital_new < 0)) {
        if (Code == 0x14B) {
            Code = 0x158;
        }
        if (Code == 0x13A) {
            Code = 0x15A;
        }
    }

    if (Code) {
        Code += uid * 0x300;
    }

    xx = Get_Position((PLW*)ewk);
    SsRequestPan(Code, xx, xx, 0, 2);
}

void Call_Se(WORK_Other* ewk, u16 Code) {
    s16 xx;

    xx = Get_Position((PLW*)ewk);
    SsRequestPan(Code, xx, xx, 0, 2);
}

void Se_Term(WORK_Other* ewk, u16 Code) {
    s16 xx;

    if (ewk->wu.work_id != 1) {
        return;
    }

    if ((ewk->wu.mvxy.a[1].sp < 0) && (ewk->wu.xyz[1].disp.pos <= 64)) {
        return;
    }

    if (Code) {
        Code += ewk->wu.id * 0x300;
    }

    xx = Get_Position((PLW*)ewk);
    SsRequestPan(Code, xx, xx, 0, 2);
}

void Finish_SE() {
    PLW* wk;
    s16 xx;
    s16 Code;

    Code = Check_Finish_SE();

    if (Code == -1) {
        return;
    }

    wk = &gs.plw[Winner_id];

    if (Code) {
        Code += (wk->wu.id * 0x300);
    }

    xx = Get_Position(wk);
    SsRequestPan(Code, xx, xx, 0, 2);
}

s32 Check_Finish_SE() {
    s16 xx;

    for (xx = 0; xx < 7; xx++) {
        if (Last_Called_SE == Finish_SE_Data[0][xx]) {
            return Finish_SE_Data[1][xx];
        }
    }

    return -1;
}

u16 Get_Position(PLW* wk) {
    u16 xx;
    u16 yy;

    xx = get_center_position();
    xx -= 0xC0;
    xx = wk->wu.position_x - xx;
    xx /= 4;
    xx += 16;

    if (xx < 0x70) {
        return xx;
    }

    yy = get_center_position();

    if (yy > wk->wu.position_x) {
        return 0x10;
    }

    return 0x70;
}

u16 Check_Bonus_SE(u16 Code) {
    if ((Bonus_Game_Flag == 0) || (Bonus_Type != 20)) {
        return Code;
    }

    if (Code < 0x100) {
        return Code;
    }

    if (Code >= 0x760) {
        return Code;
    }

    return Bonus_Voice_Data[Code - 0x100];
}

void Store_Sound_Code(u16 code, SoundPatchConfig* rmc) {
    s16 i;

    for (i = 7; i > 0; i--) {
        sdeb[i] = sdeb[i - 1];
    }

    sdeb->cp3code = code;
    sdeb->rmc = *rmc;
}

void Disp_Sound_Code() {
    s16 i;

    if (Debug_w[4] == 0) {
        return;
    }

    flPrintColor(0xFFFFFFFF);

    for (i = 0; i < SDEB_SIZE; i++) {
        flPrintL(2, i * 2 + 11, "%3X", sdeb[i].cp3code);
        flPrintL(50, i * 2 + 11, "%2X", sdeb[i].rmc.ptix);
        flPrintL(53, i * 2 + 11, "%2X", sdeb[i].rmc.bank);
        flPrintL(56, i * 2 + 11, "%2X", sdeb[i].rmc.port);
        flPrintL(59, i * 2 + 11, "%3X", sdeb[i].rmc.code);
    }

    flPrintColor(0xFFFFFF8F);
    flPrintL(2, 9, "CODE  KDCM PORT BKCD PRIO VOL  POS  PICH NEXT   PX BK PO CODE");
}
