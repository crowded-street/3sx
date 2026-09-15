/**
 * @file com_sub_arts.c
 * CPU Controlled Character super arts, menu branching and wait states.
 *
 * Split out of com_sub.c unchanged - no function body was edited.
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

/* Commits to the super art selected for this character. Reads kind_of_arts at
 * the point of use, so callers that mutate xx first (DENJIN_Check, YAGYOU_Check)
 * see their own result. */
/* Step to the next CP state: bump the outer index and clear the three inner
 * ones. Every wait and branch function in this file ends on this block. */
static void Advance_CP_State(PLW* wk) {
    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
}

static void SA_Next_Menu(PLW* wk, s16* xx) {
    Next_Another_Menu(wk, 2, xx[plw[wk->wu.id].sa->kind_of_arts]);
}

/* The arm most characters take: one range gate, then the shared menu step. The
 * gate level is the only thing that differs between them. */
static void SA_Range_Menu(PLW* wk, s16* xx, s16 SA_No, u16 Term_No) {
    if (SA_Range_Check(wk, SA_No, Term_No) != 0) {
        return;
    }
    SA_Next_Menu(wk, xx);
}

/* The arm for player 2: the range gate, then the DENJIN charge, then the menu. */
static void SA_Denjin_Arm(PLW* wk, s16* xx, u16 SA2, u16 Term_No) {
    if (SA_Range_Check(wk, 1, Term_No) != 0) {
        return;
    }
    DENJIN_Check(wk, SA2, (u16*)&xx[2], Term_No);
    SA_Next_Menu(wk, xx);
}

/* The arm for player 5: hold off while the target is still off the ground. */
static void SA_Grounded_Arm(PLW* wk, s16* xx, u16 Term_No) {
    if (((WORK*)wk->wu.target_adrs)->xyz[1].disp.pos >= 0x10) {
        CP_Index[wk->wu.id][0]++;
        return;
    }
    SA_Range_Menu(wk, xx, 1, Term_No);
}

/* The arm for player 8: no range gate, but the super only comes out on the
 * second art once vitality is down to half. */
static void SA_Low_Vitality_Arm(PLW* wk, s16* xx) {
    if ((plw[wk->wu.id].sa->kind_of_arts == 2) && (plw[wk->wu.id].wu.vital_new <= (Max_vitality / 2))) {
        SA_Next_Menu(wk, xx);
        return;
    }
    CP_Index[wk->wu.id][0]++;
}

/* The arm for player 14: two range gates before the menu. */
static void SA_Two_Range_Arm(PLW* wk, s16* xx, u16 Term_No) {
    if (SA_Range_Check(wk, 1, Term_No) != 0) {
        return;
    }
    SA_Range_Menu(wk, xx, 2, Term_No);
}

/* Per-character super art handling, split out of SA_Term. Every path is
 * terminal. The original reached the shared Next_Another_Menu call by breaking
 * out of the switch on one path only (player_number 8, low vitality); that path
 * calls SA_Next_Menu here instead, which is the same call it fell through to. */
/* The characters whose super art is gated on range alone, and the default every
 * other character takes. The case labels are the original ones. */
static void SA_Term_Range_Case(PLW* wk, s16* xx, u16 Term_No) {
    switch (wk->player_number) {
    case 11:
        SA_Range_Menu(wk, xx, 1, Term_No);
        return;

    case 1:
        SA_Range_Menu(wk, xx, 1, Term_No);
        return;

    case 5:
        SA_Grounded_Arm(wk, xx, Term_No);
        return;

    case 6:
        SA_Range_Menu(wk, xx, 0, Term_No);
        return;

    case 14:
        SA_Two_Range_Arm(wk, xx, Term_No);
        return;

    default:
        SA_Next_Menu(wk, xx);
        return;
    }
}

/* The characters with a condition of their own; everyone else is gated on range
 * and goes to SA_Term_Range_Case. */
static void SA_Term_Player_Case(PLW* wk, s16* xx, u16 SA2, u16 Term_No) {
    switch (wk->player_number) {
    case 2:
        SA_Denjin_Arm(wk, xx, SA2, Term_No);
        return;

    case 8:
        SA_Low_Vitality_Arm(wk, xx);
        return;

    case 9:
        YAGYOU_Check(wk, &xx[1], Term_No);
        SA_Next_Menu(wk, xx);
        return;

    default:
        SA_Term_Range_Case(wk, xx, Term_No);
        return;
    }
}

/* The passive gate SA_Term opens with. Check_Passive mutates state, so it must
 * stay behind the Passive_Flag test exactly as it did. */
static s32 SA_Term_Passive_Taken(PLW* wk) {
    return ((Passive_Flag[wk->wu.id]) == 0) && (Check_Passive(wk) != 0);
}

/* Non-zero when this character has no super art to give: the slot is empty, or
 * it is mid-metamorphose. */
static s32 SA_Not_Available(PLW* wk, s16* xx) {
    return (xx[plw[wk->wu.id].sa->kind_of_arts] == -1) || plw[wk->wu.id].metamorphose;
}

/* Non-zero when neither the super art gauge nor the meter-pool flag is up. */
static s32 SA_Gauge_Empty(PLW* wk) {
    return !((plw[wk->wu.id].sa->ok) || (plw[wk->wu.id].sa->mp));
}

void SA_Term(PLW* wk, const SA_Term_Args* p) {
    s16 xx[3];

    if (SA_Term_Passive_Taken(wk)) {
        return;
    }

    xx[0] = p->SA0;
    xx[1] = p->SA1;
    xx[2] = p->SA2;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if (SA_Not_Available(wk, xx)) {
        CP_Index[wk->wu.id][0]++;
        return;
    }

    if (SA_Gauge_Empty(wk)) {
        CP_Index[wk->wu.id][0]++;
        return;
    }

    Disposal_Again[wk->wu.id] = 1;

    /* NOTE: this condition is always true - no value is both 0xFFFF and 0.
     * Preserved exactly as found; see AGENTS.md on arcade-accurate oddities. */
    if ((p->Term_No != 0xFFFF) || (p->Term_No != 0)) {
        SA_Term_Player_Case(wk, xx, p->SA2, p->Term_No);
        return;
    }

    SA_Next_Menu(wk, xx);
}

s32 DENJIN_Check(PLW* wk, u16 SA2, u16* xx, u16 Term_No) {
    if (plw[wk->wu.id].sa->kind_of_arts != 2) {
        return 0;
    }

    DENJIN_No[wk->wu.id] = Term_No;
    DENJIN_Term[wk->wu.id] = SA2;
    xx[0] = 0x37;
    return 1;
}

const u8 YAGYOU_Data[0x10] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 };

s32 YAGYOU_Check(PLW* wk, s16* xx, u16 Term_No) {
    if (plw[wk->wu.id].sa->kind_of_arts != 1) {
        return 0;
    }

    if (Term_No == 0) {
        Term_No = YAGYOU_Data[random_16_com()];
        Term_No += 0x64;
    }
    xx[0] = Term_No;
    return 1;
}

s32 SA_Range_Check(PLW* wk, s16 SA_No, u16 Range) {
    if (SA_No != plw[wk->wu.id].sa->kind_of_arts) {
        return 0;
    }

    if (Range & 0x8000) {
        if ((PL_Distance[wk->wu.id]) < (Range & 0x7FFF)) {
            CP_Index[wk->wu.id][0]++;
            return 1;
        }

    }

    else if (PL_Distance[wk->wu.id] > Range) {
        CP_Index[wk->wu.id][0]++;
        return 1;
    }

    return 0;
}

/* Step on when the gauge the caller asked about is ready, otherwise hand the
 * CPU the fallback menu. The gauge flag is the only thing Check_SA and Check_EX
 * differ in. */
static void Check_Arts_Gauge(PLW* wk, s8 Gauge_Ready, s16 Next_Action, s16 Next_Menu) {
    if (Gauge_Ready) {
        CP_Index[wk->wu.id][0]++;
    } else {
        CP_No[wk->wu.id][0] = Next_Action;
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Check_SA(PLW* wk, s16 Next_Action, s16 Next_Menu) {
    Check_Arts_Gauge(wk, plw[wk->wu.id].sa->ok, Next_Action, Next_Menu);
}

void Check_EX(PLW* wk, s16 Next_Action, s16 Next_Menu) {
    Check_Arts_Gauge(wk, plw[wk->wu.id].sa->ex, Next_Action, Next_Menu);
}

void Check_SA_Full(PLW* wk, s16 Next_Action, s16 Next_Menu) {
    Disposal_Again[wk->wu.id] = 1;

    if (wk->permited_koa & 0x40) {
        CP_Index[wk->wu.id][0]++;
    } else {
        CP_No[wk->wu.id][0] = Next_Action;
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Branch_Unit_Area(PLW* wk, const Branch_Menu_Args* p) {
    s16 xx[4];

    CP_No[wk->wu.id][0] = p->Next_Action;
    xx[0] = p->Menu_00;
    xx[1] = p->Menu_01;
    xx[2] = p->Menu_02;
    xx[3] = p->Menu_03;

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    Disposal_Again[wk->wu.id] = 1;
    Next_Another_Menu(wk, p->Next_Action, xx[Area_Number[wk->wu.id]]);
}

void Com_Random_Select(PLW* wk, const Branch_Menu_Args* p, s16 Rnd_Type) {
    s16 xx[4];
    s16 zz;

    zz = Com_Rnd_Select_Data[Rnd_Type][random_16_com()];

    xx[0] = p->Menu_00;
    xx[1] = p->Menu_01;
    xx[2] = p->Menu_02;
    xx[3] = p->Menu_03;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if (xx[zz] == 0xFF) {
        Next_End(wk);
    } else {
        Disposal_Again[wk->wu.id] = 1;
        Next_Another_Menu(wk, p->Next_Action, xx[zz]);
    }
}

/* Run the wait timer down and, when it expires, leave for the next CP state.
 * Branch_Wait_Area and Wait share this arm; the timer they loaded differs, not
 * what happens when it runs out. */
static void Wait_Timer_Step(PLW* wk) {
    if (--Timer_00[wk->wu.id]) {
        return;
    }
    Advance_CP_State(wk);

    Flip_Flag[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;

    if (CP_No[wk->wu.id][0] != 6) {
        Passive_Flag[wk->wu.id] = 0;
    }
}

void Branch_Wait_Area(PLW* wk, const Branch_Wait_Args* p) {
    s16 xx[4];

    switch (CP_Index[wk->wu.id][1]) {
    case 0:

        CP_Index[wk->wu.id][1]++;

        xx[0] = p->Time_00;
        xx[1] = p->Time_01;
        xx[2] = p->Time_02;
        xx[3] = p->Time_03;
        Timer_00[wk->wu.id] = xx[Area_Number[wk->wu.id]];
        break;

    default:
        Wait_Timer_Step(wk);
        break;
    }
}

void Wait(PLW* wk, s16 Time) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;

        if (Time == 0) {
            Timer_00[wk->wu.id] = Setup_WT_Data(wk);
        } else {
            Timer_00[wk->wu.id] = Time;
        }

        break;

    default:
        Wait_Timer_Step(wk);
        break;
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Look(PLW* wk, s16 Time) {
    Passive_Flag[wk->wu.id] = 0;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        CP_Index[wk->wu.id][1]++;

        if (Time == 0) {
            Timer_00[wk->wu.id] = Setup_LP_Data(wk);
        } else {
            Timer_00[wk->wu.id] = Time;
        }

        if (Lever_LR[wk->wu.id] & 2) {
            Timer_00[wk->wu.id] += 0x32;
        }

        break;

    default:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        Advance_CP_State(wk);

        Flip_Flag[wk->wu.id] = 0;
        Limited_Flag[wk->wu.id] = 0;
        Before_Look[wk->wu.id] = 1;
        break;
    }
}

/* CP_Index 0: latch the lever that will be replayed for the hold. */
static void Keep_Status_Begin(PLW* wk, u16 Lever_Data, s16 Option_Data) {
    CP_Index[wk->wu.id][1]++;
    dash_flag_clear(wk->wu.id);
    Timer_00[wk->wu.id] = 0xA;

    Free_Lever[wk->wu.id] = Lever_Data;
    if (Option_Data != -1) {
        Free_Lever[wk->wu.id] |= Setup_Guard_Lever(wk, Option_Data);
    }
    Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];
}

/* Replay the latched lever, and hand the pattern back once the hold expires
 * with nothing attacking. */
static void Keep_Status_Hold(PLW* wk) {
    Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];
    if (--Timer_00[wk->wu.id]) {
        return;
    }
    Timer_00[wk->wu.id] = 1;

    if (Attack_Flag[wk->wu.id] != 0) {
        return;
    }

    Advance_CP_State(wk);

    Flip_Flag[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;

    if (CP_No[wk->wu.id][0] != 6) {
        Passive_Flag[wk->wu.id] = 0;
    }
}

void Keep_Status(PLW* wk, u16 Lever_Data, s16 Option_Data) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Keep_Status_Begin(wk, Lever_Data, Option_Data);
        break;

    default:
        Keep_Status_Hold(wk);
        break;
    }
}

void VS_Jump_Guard(PLW* wk) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Guard(wk) == 0) {
            dash_flag_clear(wk->wu.id);
            CP_Index[wk->wu.id][1]++;
        }

        break;

    default:
        if (Check_Guard(wk) != 0) {
            break;
        }

        if (((WORK*)wk->wu.target_adrs)->xyz[1].disp.pos < 0x19) {
            Advance_CP_State(wk);

            Passive_Flag[wk->wu.id] = 0;
            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;
        }

        break;
    }
}

void Wait_Lie(PLW* wk, u16 Lever_Data) {
    WORK* em;

    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Rolling_Flag[wk->wu.id] = 0;

        if (Lever_Data != 0) {
            Free_Lever[wk->wu.id] = Setup_Guard_Lever(wk, 1);
            Free_Lever[wk->wu.id] |= Lever_Data & 2;
        } else {
            Free_Lever[wk->wu.id] = 0;
        }
        /* fallthrough */

    default:
        Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];

        em = (WORK*)wk->wu.target_adrs;
        if ((Check_Blow_Off(wk, em, 0) == 0) || (Lie_Flag[wk->wu.id] != 0)) {
            Advance_CP_State(wk);

            Flip_Flag[wk->wu.id] = 0;

            Limited_Flag[wk->wu.id] = 0;
        }
        break;
    }
}

void Wait_Get_Up(PLW* wk, u16 Lever_Data, s16 Option) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Rolling_Flag[wk->wu.id] = 0;

        if (Lever_Data != 0) {
            Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
            Lever_LR[wk->wu.id] |= Lever_Data & 2;
        } else {
            Lever_LR[wk->wu.id] = 0;
        }

        /* fallthrough */

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Wait_Term(wk, Option) != 0) {
            Advance_CP_State(wk);

            Disposal_Again[wk->wu.id] = 1;
            Passive_Flag[wk->wu.id] = 1;
            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;
        }
        break;
    }
}

s32 Check_Wait_Term(PLW* wk, s16 Option) {
    WORK* em;

    em = (WORK*)wk->wu.target_adrs;

    if ((em->routine_no[1] == 1) && (em->pat_status == 0x18)) {
        return 0;
    }
    if (Lie_Flag[wk->wu.id] == 0) {
        return 1;
    }
    if (Option != 0) {
        return 0;
    }
    if (em->cg_type == 0xB) {
        return 1;
    }
    return 0;
}

/* Arm the guard for the wait: the lever the guard holds, and the guard flag it
 * only raises when a lever was given. */
static void Wait_Attack_Complete_Begin(PLW* wk, u16 Lever_Data) {
    CP_Index[wk->wu.id][1]++;
    dash_flag_clear(wk->wu.id);

    if (Lever_Data != 0) {
        Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
        Lever_LR[wk->wu.id] |= Lever_Data & 2;
        Guard_Flag[wk->wu.id] = 1;
    } else {
        Lever_LR[wk->wu.id] = 0;
    }
}

/* Leave the wait once the guard is over. Passive_Flag is only cleared on the
 * unoptioned path, as before. */
static void Wait_Attack_Complete_Exit(PLW* wk, s16 Option) {
    Advance_CP_State(wk);
    Guard_Flag[wk->wu.id] = 0;

    Flip_Flag[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;
    if (Option == 0) {
        Passive_Flag[wk->wu.id] = 0;
    }
}

void Wait_Attack_Complete(PLW* wk, u16 Lever_Data, s16 Option) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Wait_Attack_Complete_Begin(wk, Lever_Data);

        /* fallthrough */

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Exit_Guard(wk, Option) == 0) {
            Wait_Attack_Complete_Exit(wk, Option);
        }
        break;
    }
}

s32 Check_Exit_Guard(PLW* wk, s16 Option) {
    WORK* em;

    if (wk->wu.routine_no[1] == 1) {
        return 1;
    }

    if (Option == 0) {
        em = (WORK*)wk->wu.target_adrs;
        if (em->routine_no[1] != 4) {
            return 0;
        }
        return 1;
    }
    return Attack_Flag[wk->wu.id];
}
