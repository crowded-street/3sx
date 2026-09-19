/**
 * @file com_sub_term.c
 * CPU Controlled Character exit-term and etc-term predicates.
 *
 * Split out of com_sub.c unchanged. These functions form a closed family: the
 * Exit_Term_* / ETC_Term_* tables and the dispatchers that walk them.
 */

#include "sf33rd/Source/Game/com/com_sub.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/active/ac0000.h"
#include "sf33rd/Source/Game/com/active/ac0001.h"
#include "sf33rd/Source/Game/com/active/ac0002.h"
#include "sf33rd/Source/Game/com/active/ac0003.h"
#include "sf33rd/Source/Game/com/active/ac0004.h"
#include "sf33rd/Source/Game/com/ck_pass.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_datu.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/com/follow/fl_com00.h"
#include "sf33rd/Source/Game/com/follow/fl_com02.h"
#include "sf33rd/Source/Game/com/passive/pass0000.h"
#include "sf33rd/Source/Game/com/passive/pass0001.h"
#include "sf33rd/Source/Game/com/passive/pass0002.h"
#include "sf33rd/Source/Game/com/passive/pass0003.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/screen/vs_shell.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"
#include "sf33rd/Source/Game/com/com_sub_internal.h"

const Term_Tbl_t Exit_Term_Tbl[9] = { Exit_Term_0000, Exit_Term_0001, Exit_Term_0002, Exit_Term_0003, Exit_Term_0004,
                                      Exit_Term_0005, Exit_Term_0006, Exit_Term_0007, Exit_Term_0008 };

s32 Check_Exit_Term(PLW* wk, WORK* em, s16 Exit_No) {
    s16 xx;

    if (VS_Jump_Term(wk, em, &xx) != 0) {
        return xx;
    }
    return Exit_Term_Tbl[Exit_No](wk, em);
}

/* Each of these answers one opponent's jump-in: zero when this character has no
 * counter to take, otherwise the menu it wants, written through xx as the
 * original did. The arms that fell out of the switch return zero here. */
static s32 VS_Jump_Cross_Chop(PLW* wk, WORK* em, s16* xx) {
    if (Check_F_Cross_Chop(wk, em, 0xF) != 0) {
        return *xx = 3;
    }
    return 0;
}

static s32 VS_Jump_Anti_Air_2b(PLW* wk, WORK* em, s16* xx) {
    if (Check_Special_Technique(wk, em, &(SP_Tech_Args){0, 0, 0x2b, -1, -1}) != 0) {
        return *xx = 2;
    }
    return 0;
}

static s32 VS_Jump_Anti_Air_2A(PLW* wk, WORK* em, s16* xx) {
    if (Check_Special_Technique(wk, em, &(SP_Tech_Args){0, 0, 0x2A, -1, -1}) != 0) {
        return *xx = 2;
    }
    if (Check_Limited_Jump_Attack(wk, em, 0x14, 4) != 0) {
        VS_Tech[wk->wu.id] = 0xF;
        return *xx = 3;
    }
    return 0;
}

static s32 VS_Jump_Limited_Attack(PLW* wk, WORK* em, s16* xx) {
    if (Check_Limited_Jump_Attack(wk, em, 0x14, 5) != 0) {
        VS_Tech[wk->wu.id] = 0xF;
        return *xx = 3;
    }
    return 0;
}

static s32 VS_Jump_Anti_Air_2C(PLW* wk, WORK* em, s16* xx) {
    if (Check_Special_Technique(wk, em, &(SP_Tech_Args){0xF, 8, 0x2C, 1, -1}) != 0) {
        return *xx = 3;
    }
    return 0;
}

s32 VS_Jump_Term(PLW* wk, WORK* em, s16* xx) {
    if (Attack_Flag[wk->wu.id] == 0) {
        return 0;
    }

    switch (My_char[em->id]) {
    case 1:
        return VS_Jump_Cross_Chop(wk, em, xx);

    case 3:
    case 10:
        return VS_Jump_Anti_Air_2b(wk, em, xx);

    case 5:
        return VS_Jump_Anti_Air_2A(wk, em, xx);

    case 8:
        return VS_Jump_Limited_Attack(wk, em, xx);

    case 9:
        return VS_Jump_Anti_Air_2C(wk, em, xx);
    }
    return 0;
}

s32 Exit_Term_0000(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0x1E;
        break;
    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0001(PLW* wk, WORK* em) {
    return 1;
}

s32 Exit_Term_0002(PLW* wk, WORK* em) {
    if (em->routine_no[1] == 2) {
        return 1;
    }
    if (em->id != 0xD) {
        return 1;
    }
    return 0;
}

/* The wait these four exit Terms share: load a timer on the first frame, then
 * leave once it runs out unless a SHINRYU is on its way. The wait length is the
 * only thing that differs between them. */
static s32 Exit_Term_Timed(PLW* wk, s16 Time) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = Time;
        break;
    default:
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        if (Check_SHINRYU(wk)) {
            return 0;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

s32 Exit_Term_0003(PLW* wk, WORK* em) {
    return Exit_Term_Timed(wk, 0xA);
}

s32 Exit_Term_0004(PLW* wk, WORK* em) {
    return Exit_Term_Timed(wk, 1);
}

s32 Exit_Term_0005(PLW* wk, WORK* em) {
    return Exit_Term_Timed(wk, 5);
}

s32 Exit_Term_0006(PLW* wk, WORK* em) {
    return Exit_Term_Timed(wk, 0x3C);
}

s32 Exit_Term_0007(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0x1E;
        /* fallthrough */
    default:
        if (Check_Drop_Term(em, 0x28) != 0) {
            if (Area_Number[wk->wu.id] >= 3) {
                return 1;
            }
            VS_Tech[wk->wu.id] = 0x1A;
            return -1;
        }
        if (--Timer_00[wk->wu.id]) {
            break;
        }
        Timer_00[wk->wu.id] = 1;
        return 1;
    }

    return 0;
}

/* Non-zero once the character is back on the ground, was already there last
 * frame, and is not in the airborne routine. */
static s32 Settled_On_Ground(PLW* wk) {
    return (wk->wu.old_pos[1] == 0) && (wk->wu.xyz[1].disp.pos == 0) && (wk->wu.routine_no[1] != 4);
}

s32 Exit_Term_0008(PLW* wk, WORK* em) {
    switch (Term_No[wk->wu.id]) {
    case 0:
        Term_No[wk->wu.id]++;
        Timer_00[wk->wu.id] = 0xb4;
        /* fallthough */
    default:
        if (Settled_On_Ground(wk)) {
            return 1;
        }
        if (--Timer_00[wk->wu.id] == 0) {
            return 1;
        }
    }

    return 0;
}

s32 Check_Drop_Term(WORK* em, s16 Y) {
    if (em->mvxy.a[1].real.h >= 0) {
        return 0;
    }
    if (em->xyz[1].disp.pos >= Y) {
        return 0;
    }
    return 1;
}

s32 Check_SHINRYU(PLW* wk) {
    PLW* em;

    em = (PLW*)wk->wu.target_adrs;

    if (em->sa->ok != -1) {
        return 0;
    }
    if (My_char[em->wu.id] != 0xB) {
        return 0;
    }
    if (plw[em->wu.id].sa->kind_of_arts != 1) {
        return 0;
    }
    return 1;
}

const Term_Tbl_t ETC_Term_Tbl[10] = { ETC_Term_0000, ETC_Term_0001, ETC_Term_0002, ETC_Term_0003, ETC_Term_0004,
                                      ETC_Term_0005, ETC_Term_0006, ETC_Term_0007, ETC_Term_0008, ETC_Term_0009 };

/* Take the fight over from the demo CPU: step to the next CP state and clear
 * the per-state flags. */
static void BOSS_Break_In(PLW* wk) {
    Disposal_Again[wk->wu.id] = 1;
    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;

    Flip_Flag[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;
}

/* Leave the fight to the menu the caller named. */
static void BOSS_Hand_Over(PLW* wk, u32 Next_Action, u16 Next_Menu) {
    Disposal_Again[wk->wu.id] = 1;
    Next_Another_Menu(wk, Next_Action, Next_Menu);
}

void Check_BOSS(PLW* wk, u32 Next_Action, u16 Next_Menu) {
    if (Break_Into_CPU == 1) {
        BOSS_Break_In(wk);
    } else {
        BOSS_Hand_Over(wk, Next_Action, Next_Menu);
    }
}

void Check_BOSS_EX(PLW* wk, u32 Next_Action, u16 Next_Menu) {
    if (Break_Into_CPU != 1) {
        BOSS_Break_In(wk);
    } else {
        BOSS_Hand_Over(wk, Next_Action, Next_Menu);
    }
}

void ETC_Term(PLW* wk, s16 Exit_No, u32 Next_Action, u16 Next_Menu) {
    s16 xx;
    WORK* em = (WORK*)wk->wu.target_adrs;

    xx = ETC_Term_Tbl[Exit_No](wk, em);

    if (xx == -1) {
        return;
    }

    if (xx) {
        BOSS_Break_In(wk);
    } else {
        BOSS_Hand_Over(wk, Next_Action, Next_Menu);
    }
}

s32 ETC_Term_0000(PLW* wk, WORK* em) {
    if (Check_VS_Squat(wk, em, &(VS_Squat_Args){0x1D, 0x21, 0x20}) != 0) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0001(PLW* wk, WORK* em) {
    if (wk->sa->ok != -1) {
        return 1;
    }
    if (My_char[wk->wu.id] != 9) {
        return 1;
    }
    if (plw[wk->wu.id].sa->kind_of_arts) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0002(PLW* wk, WORK* em) {
    if (plw[wk->wu.id].wu.vital_new < 0x31) {
        return 1;
    }
    return 0;
}

/* Non-zero when the opponent is in none of the three pat states this Term
 * waits out. */
static s32 Opponent_Pat_Status_Clear(WORK* em) {
    return (em->pat_status != 0x20) && (em->pat_status != 0x21) && (em->pat_status != 2);
}

s32 ETC_Term_0003(PLW* wk, WORK* em) {
    if (Opponent_Pat_Status_Clear(em)) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0004(PLW* wk, WORK* em) {
    if (em->vital_new >= 0x50) {
        return 0;
    }
    if (plw[wk->wu.id].wu.vital_new < 0x78) {
        return 0;
    }
    return 1;
}

s32 ETC_Term_0005(PLW* wk, WORK* em) {
    if (em->vital_new < plw[wk->wu.id].wu.vital_new) {
        return 1;
    }
    return 0;
}

s32 ETC_Term_0006(PLW* wk, WORK* em) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        CP_Index[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = 0x78;
        /* fallthrough */
    case 1:
        if (--Timer_00[wk->wu.id] == 0) {
            return 1;
        }
        if (PL_Distance[wk->wu.id] < 0x70) {
            return 1;
        }
        Lever_Buff[wk->wu.id] = 0x40;
        return -1;
    }
    return -1;
}

s32 ETC_Term_0007(PLW* wk, WORK* em) {
    if (plw[wk->wu.id].sa->kind_of_arts == 2) {
        return 1;
    }

    return 0;
}

s32 ETC_Term_0008(PLW* wk, WORK* em) {
    if (plw[wk->wu.id].sa->kind_of_arts == 1) {
        return 1;
    }

    return 0;
}

s32 ETC_Term_0009(PLW* wk, WORK* em) {
    if (plw[wk->wu.id].sa->kind_of_arts == 0) {
        return 1;
    }

    return 0;
}

s32 emLevelRemake(s32 now, s32 max, s32 exd) {
    s32 RemakeLevelForDifficulty[8] = { -30, -10, 0, 0, 0, 0, 20, 60 };

    now += (max - exd) * RemakeLevelForDifficulty[save_w[Present_Mode].Difficulty] / 100;

    if (now < 0) {
        now = 0;
    }
    if (now >= max) {
        now = max - 1;
    }

    return now;
}

s32 emGetMaxBlocking() {
    s32 RapidBlockingTimes[8] = { 2, 2, 3, 3, 3, 4, 6, 10 };

    return RapidBlockingTimes[save_w[Present_Mode].Difficulty];
}
