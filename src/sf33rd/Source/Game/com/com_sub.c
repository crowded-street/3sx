/**
 * @file com_sub.c
 * CPU Controlled Character Subroutines
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

s8 Lv;
s8 Rnd;


/* Tests the routine state that gates committing to a new action or ending the
 * current reaction: any routine_no[1] other than 4, with cg_type 0x40 treated
 * as free regardless. The name describes the role this test plays at its 12
 * call sites; the original source carried no name for it. The condition is
 * copied character for character - do not simplify the boolean algebra. */
s32 Check_Free_To_Act(PLW* wk) {
    return (wk->wu.routine_no[1] != 4) || (wk->wu.cg_type == 0x40);
}

/* True once the current motion has run out: cg_type 0x40, or routine_no[1]
 * back at 0. Gates handing control back to Reaction_Exit_Sub at six sites.
 * Name describes the observed role; condition copied character for character. */
s32 Check_Motion_Ended(PLW* wk) {
    return (wk->wu.cg_type == 0x40) || (wk->wu.routine_no[1] == 0);
}

void End_Pattern(PLW* wk) {
    Next_Be_Free(wk);
}

void Next_Be_Passive(PLW* wk, s32 unused) {
    Next_Be_Free(wk);
}

void Turn_Over_On(PLW* wk) {
    Disposal_Again[wk->wu.id] = 1;
    Turn_Over[wk->wu.id] = 1;
    CP_Index[wk->wu.id][0]++;
}

void Only_Shot(PLW* wk, s16 Lever_Data) {
    Lever_Buff[wk->wu.id] = Lever_Data;
    CP_Index[wk->wu.id][0]++;
}

void Lever_On(PLW* wk, u16 LR_Lever, u16 UD_Lever) {
    CP_Index[wk->wu.id][0]++;
    Disposal_Again[wk->wu.id] = 1;
    if ((LR_Lever == 0) || (LR_Lever == 1)) {
        Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, LR_Lever);
    } else {
        Lever_LR[wk->wu.id] = 0;
    }
    Lever_LR[wk->wu.id] |= UD_Lever;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Lever_Off(PLW* wk) {
    CP_Index[wk->wu.id][0]++;
    Disposal_Again[wk->wu.id] = 1;
    Lever_LR[wk->wu.id] = 0;
}

void Pierce_On(PLW* wk) {
    Disposal_Again[wk->wu.id] = 1;
    CP_Index[wk->wu.id][0]++;
    Pierce_Menu[wk->wu.id] = 1;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Setup_DENJIN_LEVEL(PLW* wk) {
    u16 xx;

    Disposal_Again[wk->wu.id] = 1;
    if ((xx = DENJIN_No[wk->wu.id])) {
        Next_Another_Menu(wk, 2, xx);
    } else {
        Next_Another_Menu(wk, 2, Denjin_Data[Area_Number[wk->wu.id]][random_16_com()]);
    }
}

/* CP_Index 0: hold the charge, and decide whether it has run its course.
 * Lifted out of Push_Shot, where it was the else arm at nesting depth 4. */
static void Push_Shot_Charge(PLW* wk, s16 Power_Level) {
    s16 xx;

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if ((wk->wu.now_koc == 8) && (wk->wu.char_index == 0xD)) {
        xx = wk->wu.cg_ix / wk->wu.cgd_type;
        if (xx >= Power_Level) {
            CP_Index[wk->wu.id][1] = 0x63;
        }
    }

    if (Check_Exit_DENJIN(wk) != 0) {
        CP_Index[wk->wu.id][1] = 0x63;
    }
}

/* Hand the shot's hit back to the reaction machinery. */
static void Push_Shot_Reaction(PLW* wk, s16 Power_Level) {
    Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    Reaction_Sub(wk, 8, Power_Level);
}

void Push_Shot(PLW* wk, s16 Power_Level) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Motion_Ended(wk)) {
            Reaction_Exit_Sub(wk);
        } else {
            Push_Shot_Charge(wk, Power_Level);
        }
        break;
    case 1:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (Check_Exit_DENJIN(wk) != 0) {
            CP_Index[wk->wu.id][1] = 0x63;
        }
        /* fallthrough */
    default:
        if (Check_Motion_Ended(wk)) {
            Reaction_Exit_Sub(wk);
        } else {
            Push_Shot_Reaction(wk, Power_Level);
        }
        break;
    }
}

/* The area-dependent half of Check_Exit_DENJIN, lifted out of the switch that
 * sat at nesting depth 4. Returns non-zero when the charge should end.
 *
 * The two arms share an identical "falling and low enough" test. It is left
 * duplicated deliberately: folding it would be a behaviour-neutral edit the
 * refactor_guard cannot distinguish from a substituted constant. */
static s32 Check_Exit_DENJIN_Area(PLW* wk, WORK* em, s16 xx) {
    switch (CP_Index[wk->wu.id][3]) {
    case 0:
    case 1:
    case 2:
        if (em->mvxy.a[1].real.h > 0) {
            return 1;
        }
        if (em->mvxy.a[1].real.h < 0) {
            if (em->xyz[1].disp.pos < 0x29) {
                return 1;
            }
        }
        break;
    default:
        if ((em->mvxy.a[1].real.h > 0) && (xx == -1)) {
            return 1;
        }
        if (em->mvxy.a[1].real.h < 0) {
            if (em->xyz[1].disp.pos < 0x29) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

s32 Check_Exit_DENJIN(PLW* wk) {
    s16 xx;
    WORK* em;

    if (!(DENJIN_Term[wk->wu.id] & 1)) {
        if (CP_Index[wk->wu.id][1] == 0) {
            return 0;
        }
    }

    if ((DENJIN_Term[wk->wu.id] & 8)) {
        if (Attack_Flag[wk->wu.id]) {
            return 1;
        }
    }
    em = (WORK*)wk->wu.target_adrs;

    xx = 0;
    if ((em->xyz[0].disp.pos) != (em->old_pos[0])) {
        if (Check_Attack_Direction(wk, em) != 0) {
            xx = -1;
        } else {
            xx = 1;
        }
    }

    if ((DENJIN_Term[wk->wu.id] & 1) && (em->xyz[0].disp.pos != 0)) {
        if (CP_Index[wk->wu.id][2] == 0) {
            CP_Index[wk->wu.id][2]++;
            CP_Index[wk->wu.id][3] = Area_Number[wk->wu.id];
        }

        if (Check_Exit_DENJIN_Area(wk, em, xx) != 0) {
            return 1;
        }
    }
    if (xx == 0) {
        return 0;
    }

    if ((DENJIN_Term[wk->wu.id] & 2) && (xx == 1)) {
        return 1;
    }

    if ((DENJIN_Term[wk->wu.id] & 4) && (xx == -1)) {
        return 1;
    }

    if ((DENJIN_Term[wk->wu.id] & 0x20) && (Lie_Flag[wk->wu.id] == 0)) {
        return 1;
    }
    return 0;
}

/* CP_Index[3] == 0: choose the keep-away move. Option 0 rolls for it; the
 * random draw still happens only on that path. */
static void Setup_Keep_Away(PLW* wk, s16 Option) {
    if (Option == 1) {
        Setup_KA_Jump(wk);
        return;
    }
    if (Option != 0) {
        CP_Index[wk->wu.id][3] = Option + 1;
        return;
    }

    if (random_16_com() < 4) {
        Setup_KA_Jump(wk);
    } else {
        Setup_KA_Walk(wk);
    }
}

void Keep_Away(PLW* wk, s16 Target_Pos, s16 Option) {
    switch (CP_Index[wk->wu.id][3]) {

    case 0:
        Setup_Keep_Away(wk, Option);
        /* fallthrough */

    case 1:
    case 2:
        Jump(wk, CP_Index[wk->wu.id][3] - 1);
        break;

    case 3:
    case 4:
        Approach_Walk(wk, Target_Pos, CP_Index[wk->wu.id][3] - 1);
        break;
    }
}

/* True when a back jump of Back_Jump_Data would carry the character past the
 * stage edge behind them. */
static s32 Check_Back_Jump_Blocked(PLW* wk) {
    s16 xx;

    if (wk->wu.rl_waza) {
        xx = wk->wu.xyz[0].disp.pos - Back_Jump_Data[wk->player_number];
        return (bg_w.bgw[1].l_limit2 - bg_w.pos_offset) > xx;
    }

    xx = wk->wu.xyz[0].disp.pos + Back_Jump_Data[wk->player_number];
    return (bg_w.bgw[1].r_limit2 + bg_w.pos_offset) < xx;
}

void Setup_KA_Jump(PLW* wk) {
    CP_Index[wk->wu.id][3] = 2;

    if (Check_Back_Jump_Blocked(wk)) {
        CP_Index[wk->wu.id][3] = 1;
    }
}

void Setup_KA_Walk(PLW* wk) {
    CP_Index[wk->wu.id][3] = 4;
}

void Search_Back_Term(PLW* wk, s16 Move_Value, s16 Next_Action, s16 Next_Menu) {
    s32 past_edge;

    if (wk->wu.rl_waza) {
        Move_Value = wk->wu.xyz[0].disp.pos - Move_Value;
        past_edge = (bg_w.bgw[1].l_limit2 - bg_w.pos_offset) > Move_Value;
    } else {
        Move_Value = wk->wu.xyz[0].disp.pos + Move_Value;
        past_edge = ((bg_w.bgw[1].r_limit2) + (bg_w.pos_offset)) < (Move_Value);
    }

    if (past_edge) {
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    } else {
        CP_Index[wk->wu.id][0]++;
    }
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
}

void Approach_Walk(PLW* wk, s16 Target_Pos, s16 Option) {
    s16 xx;

    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Timer_00[wk->wu.id] = 0x78;
        /* fallthrough */

    case 1:
        xx = Standing_Timer[wk->wu.id];

        if (Lie_Flag[wk->wu.id] == 0) {
            if (Check_Passive(wk) != 0) {
                break;
            }
        }
        Standing_Timer[wk->wu.id] = xx;

        if (--Timer_00[wk->wu.id] == 0) {
            Next_Be_Free(wk);
        }

        else if (Check_Arrival(wk, Target_Pos, Option) != 0) {
            Disposal_Again[wk->wu.id] = 1;
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;

            if (CP_No[wk->wu.id][0] != 6) {
                Passive_Flag[wk->wu.id] = 0;
            }
        } else {
            Ck_Distance_Lv(wk);
            if (Option == 3) {
                Lever_Buff[wk->wu.id] ^= 0xC;
            }
        }
    }
}

s32 Check_Arrival(PLW* wk, s16 Target_Pos, s16 Option) {
    if (Option == 3) {
        if (Target_Pos <= PL_Distance[wk->wu.id]) {
            return 1;
        }
        return wk->micchaku_flag;
    }

    if (wk->hos_em_flag) {
        return 1;
    }
    if (Target_Pos >= PL_Distance[wk->wu.id]) {
        return 1;
    }

    return 0;
}

void Walk(PLW* wk, u16 Lever, s16 Time, s16 unused) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Timer_00[wk->wu.id] = Time;
        Timer_01[wk->wu.id] = wk->wu.rl_flag;
        Free_Lever[wk->wu.id] = Setup_Guard_Lever(wk, Lever);
        /* fallthrough */

    case 1:
        if (Lie_Flag[wk->wu.id] == 0) {
            if (Check_Passive(wk) != 0) {
                break;
            }
        }

        if (--Timer_00[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][0]++;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;

            Flip_Flag[wk->wu.id] = 0;
            Limited_Flag[wk->wu.id] = 0;

            if (*CP_No[wk->wu.id] != 6) {
                Passive_Flag[wk->wu.id] = 0;
            }
        } else {
            if ((Timer_01[wk->wu.id] != (s16)wk->wu.rl_flag) || (wk->micchaku_flag != 0) || (wk->hos_em_flag != 0)) {
                Next_Be_Free(wk);
            }
            Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];
        }
        break;
    }
}



void Short_Range_Attack(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Next_Action, s16 Next_Menu) {
    u16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) == 0) {
            CP_Index[wk->wu.id][1]++;
            Check_First_Menu(wk);

            Ck_Distance_LvJ(wk);
            xx = get_nearing_range(wk->player_number, xx = Lever_Data & 0xFF0);
            if (PL_Distance[wk->wu.id] > xx) {
                Next_Another_Menu(wk, Next_Action, Next_Menu);
            }
        }

        break;

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id]) {
            break;
        }
        Lever_Buff[wk->wu.id] = Lever_Data;
        CP_Index[wk->wu.id][1]++;
        break;

    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Reaction_Sub(wk, Reaction, 0);
        break;
    }
}

/* Exit_Term result 0: the range gates, and the pattern advance that follows
 * when they all pass. Lifted out of EM_Term, where this reached nesting depth
 * 4. Each gate broke out of the inner switch, which fell straight through to
 * the end of EM_Term - the same thing returning here does. */
static void EM_Term_Approach(PLW* wk, WORK* em, s16 Range_X, s16 Range_Y, s16 Exit_Number) {
    if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], Range_X) == 0) {
        return;
    }

    if (Exit_Number != 8) {
        if (Check_Term_Sub_Y(wk, em->xyz[1].disp.pos, Range_Y) == 0) {
            return;
        }
    } else {
        if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
            return;
        }
    }

    Disposal_Again[wk->wu.id] = 1;
    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;

    Flip_Flag[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;
}

void EM_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Exit_Number, s16 Next_Action, s16 Next_Menu) {
    WORK* em;

    em = (WORK*)wk->wu.target_adrs;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        CP_Index[wk->wu.id][1]++;
        Term_No[wk->wu.id] = 0;
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        switch (Check_Exit_Term(wk, em, Exit_Number)) {
        case 0:
            EM_Term_Approach(wk, em, Range_X, Range_Y, Exit_Number);
            break;

        case 1:
            Disposal_Again[wk->wu.id] = 1;
            Next_Another_Menu(wk, Next_Action, Next_Menu);
            break;

        case 2:
            break;

        case 3:
            Select_Passive(wk);
            break;

        default:
            Counter_Attack[wk->wu.id] = 1;
            Select_Passive(wk);
            break;
        }

        break;
    }
}

void SHELL_Term(PLW* wk, s16 Next_Command, s16 Exit_Number, s16 Next_Action, s16 Next_Menu, s16 unused) {
    WORK* em;
    WORK_Other* tmw;
    s16 xx;

    em = (WORK*)Shell_Address[wk->wu.id];
    tmw = (WORK_Other*)Shell_Address[wk->wu.id];

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        CP_Index[wk->wu.id][1]++;
        Term_No[wk->wu.id] = 0;
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (Check_Exit_Term(wk, em, Exit_Number) == 1) {
            Next_Another_Menu(wk, Next_Action, Next_Menu);
        } else {
            xx = Compute_Hit_Time(wk, tmw);
            if (xx < Shell_Dodge_Data[Next_Command][wk->player_number]) {
                Disposal_Again[wk->wu.id] = 1;
                CP_Index[wk->wu.id][0]++;
                CP_Index[wk->wu.id][1] = 0;
                CP_Index[wk->wu.id][2] = 0;
                CP_Index[wk->wu.id][3] = 0;

                Flip_Flag[wk->wu.id] = 0;
                Limited_Flag[wk->wu.id] = 0;
            }
        }
        break;
    }
}

s32 Check_Term_Sub_Air(PLW* wk, s16 Distance, s16 Range) {
    if (Range == -1) {
        return 1;
    }
    if (!(Range & 0x8000)) {
        return Distance >= Range;
    }

    Range += Correct_Unit_PL(wk);
    return Distance <= (Range & 0x7FFF);
}

s32 Check_Term_Sub(PLW* wk, s16 Distance, s16 Range) {
    if (Range == -1) {
        return 1;
    }
    if (!(Range & 0x8000)) {
        return Distance >= Range;
    }
    return Distance <= (Range & 0x7FFF);
}

const s16 Correct_VS_Air_Data[0x14] = { 0, 0x20, 0, 0, 0, 0x20, 0x20, 0, 0x20, 0, 0, 0, 0, 0x20, 0, 0, 0, 0, 0, 0 };

s32 Correct_Unit_PL(PLW* wk) {
    return Correct_VS_Air_Data[My_char[Player_id]];
}

s32 Check_Term_Sub_Y(PLW* wk, s16 Distance, s16 Range) {
    WORK* em;

    if (Range == -1) {
        return 1;
    }

    em = (WORK*)wk->wu.target_adrs;
    if (!(Range & 0x8000)) {
        return Distance >= Range;
    }
    if (em->mvxy.a[1].real.h > 0) {
        return 0;
    }
    return Distance <= (Range & 0x7FFF);
}



s32 Check_Landed(PLW* wk, s16 Reaction) {
    if ((wk->wu.old_pos[1] != 0) && (wk->wu.xyz[1].disp.pos == 0)) {
        Lever_Buff[wk->wu.id] = 0;
        if (Continue_Menu[wk->wu.id]) {
            Next_End(wk);
            Before_Jump[wk->wu.id] = 1;
            return 1;
        } else {
            Reaction_Sub(wk, Reaction, 0);

            Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
            Check_Guard(wk);

            Before_Jump[wk->wu.id] = 1;
            return 1;
        }
    }
    if ((wk->wu.old_pos[1] == 0) && (wk->wu.xyz[1].disp.pos == 0) && (wk->wu.routine_no[1] != 4)) {
        Lever_Buff[wk->wu.id] = 0;
        if (Continue_Menu[wk->wu.id]) {
            Next_End(wk);
            Before_Jump[wk->wu.id] = 1;
            return 1;
        } else {
            Reaction_Sub(wk, Reaction, 0);
            if (Check_Guard(wk) != 0) {
                return 1;
            }
            Before_Jump[wk->wu.id] = 1;
            return 1;
        }
    }
    return 0;
}

/* One shell's worth of the Check_Dash_Hit scan. The two arms differ only in
 * which Setup_Front_or_Back result disqualifies the shell; both are kept as
 * written rather than folded into one test. */
static s32 Check_Dash_Hit_Shell(PLW* wk, WORK_Other* tmw, u16 Tech_Number, s16 zz) {
    s16 blocked;

    if (Tech_Number == 0) {
        blocked = (zz == 1);
    } else {
        blocked = (zz == 0);
    }

    if (blocked) {
        return 0;
    }
    return Check_Hit_Shell(wk, tmw, Tech_Number) != 0;
}

s32 Check_Dash_Hit(PLW* wk, u16 Tech_Number) {
    WORK_Other* tmw;
    WORK* em;
    s16 i;
    s16 xx;
    s16 zz;

    if ((Tech_Number != 0) && (Tech_Number != 1)) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;
    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs((WORK*)wk, em->id, i, &tmw) == 0)) {
            return 0;
        }
        if (tmw->wu.routine_no[1] == 2) {
            continue;
        }

        xx = wk->wu.xyz[0].disp.pos - tmw->wu.xyz[0].disp.pos;
        zz = Setup_Front_or_Back(wk, xx);

        if (Check_Dash_Hit_Shell(wk, tmw, Tech_Number, zz) != 0) {
            return 1;
        }
    }

    return 0;
}

s32 Setup_Front_or_Back(PLW* wk, s16 xx) {
    if (wk->wu.rl_waza == 0) {
        return xx < 0;
    }
    return xx >= 0;
}

s32 Check_Hit_Shell(PLW* wk, WORK_Other* tmw, u16 Tech_Number) {
    s16 xx;

    if (wk->wu.rl_waza == 1) {
        Tech_Number ^= 1;
    }

    if (Tech_Number == 0) {
        xx = wk->wu.xyz[0].disp.pos - Dash_Distance_Data[wk->player_number][Tech_Number];
        return xx <= tmw->wu.xyz[0].disp.pos;
    }

    xx = wk->wu.xyz[0].disp.pos + Dash_Distance_Data[wk->player_number][Tech_Number];
    return xx >= tmw->wu.xyz[0].disp.pos;
}

void Jump_Init(PLW* wk, s16 Jump_Dir) {
    switch (Jump_Dir) {
    case 0:
        Ck_Distance_LvJ(wk);
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;
    case 2:
        Lever_Pool[wk->wu.id] = 1;
        Lever_Buff[wk->wu.id] = 1;
        break;

    default:
        Ck_Distance_LvJ(wk);
        Lever_Pool[wk->wu.id] ^= 0xC;
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;
    }
}

/* Advances CP_Index once the final lever value is settled. Lifted out of
 * Command_Type_00's else arm, where the player_number 7 test sat at nesting
 * depth 4. */
static void Command_Type_00_Advance(PLW* wk, u16 Tech_Number) {
    if (CP_No[wk->wu.id][0] == 0xA) {
        Rapid_Index[wk->wu.id] = Lever_Buff[wk->wu.id] & 0xFF0;
        Lever_Pool[wk->wu.id] = Lever_Buff[wk->wu.id] & 0xFF0;
    }
    if ((wk->player_number == 6) && ((Tech_Number) == 0x8016)) {
        CP_Index[wk->wu.id][1] = 5;
    } else {
        if ((wk->player_number == 7) && ((Tech_Number) == 0x1F)) {
            Reaction_Exit_Sub(wk);
        } else {
            CP_Index[wk->wu.id][1]++;
        }
    }
}

s32 Command_Type_00(PLW* wk, s16 Power_Level, u16 Tech_Number, s16 Ex_Shot) {
    if (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 4] != 0x1C) {
        Lever_Buff[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 3] & 0x7FFF;
        Lever_Buff[wk->wu.id] = datacmd_conpanecmd(Lever_Buff[wk->wu.id]);

        if (wk->wu.rl_waza) {
            if (Lever_Buff[wk->wu.id] & 0xC) {
                Lever_Buff[wk->wu.id] ^= 0xC;
            }
        }
        Tech_Index[wk->wu.id] += 4;
        return 1;
    } else {
        Lever_Buff[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 3] & 0x7FFF;
        if (wk->wu.rl_waza) {
            if (Lever_Buff[wk->wu.id] & 0xC) {
                Lever_Buff[wk->wu.id] ^= 0xC;
            }
        }
        if (Tech_Address[wk->wu.id][7] == 0x80) {
            return -1;
        }
        Tech_Index[wk->wu.id] = 7;

        if ((plw[wk->wu.id].sa->ex) && ((Ex_Shot == 0x70) || (Ex_Shot == 0x700))) {
            Lever_Buff[wk->wu.id] |= Ex_Shot;
        } else {
            Lever_Buff[wk->wu.id] |= renbanshot_conpaneshot(Tech_Address[wk->wu.id], Power_Level);
        }

        if ((My_char[wk->wu.id] == 2) && ((Tech_Number) == 0x8015) && (Power_Level != 8)) {
            CP_Index[wk->wu.id][0]++;
            Lever_LR[wk->wu.id] = Lever_Buff[wk->wu.id] & 0xFF0;

            if (Power_Level == 0xA) {
                CP_Index[wk->wu.id][1] = 1;
            } else {
                CP_Index[wk->wu.id][1] = 0;
            }
        } else {
            Command_Type_00_Advance(wk, Tech_Number);
        }
        return 0;
    }
}

const u16 Rolling_Lv_Data[2][9] = {
    { 4, 1, 8, 2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
    { 4, 2, 8, 1, 4, 2, 8, 1, 0xFFFF },
};

s32 Command_Type_06(PLW* wk, s16 Power_Level, u16 Tech_Number, s16 Ex_Shot) {
    s16 xx;

    xx = 0;

    if (Tech_Number & 0x8000) {
        if ((My_char[wk->wu.id] == 6) && (Super_Arts[wk->wu.id] == 0)) {
            xx = 1;
        }
    }
    Lever_Buff[wk->wu.id] = Rolling_Lv_Data[xx][CP_Index[wk->wu.id][2]];
    CP_Index[wk->wu.id][2]++;

    if ((Rolling_Lv_Data[xx][CP_Index[wk->wu.id][2]]) == 0xFFFF) {
        Lever_Buff[wk->wu.id] |= renbanshot_conpaneshot(Tech_Address[wk->wu.id], Power_Level);
        return 1;
    }

    return 0;
}

s32 Command_Type_01(PLW* wk, s16 Power_Level, s16 Ex_Shot) {
    switch (CP_Index[wk->wu.id][2]) {
    case 0:
        CP_Index[wk->wu.id][2]++;
        Timer_01[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 1] + 2;
        Lever_Pool[wk->wu.id] = Tech_Address[wk->wu.id][Tech_Index[wk->wu.id] + 3];
        Setup_Command_01(wk);

        if (wk->wu.rl_waza) {
            if (Lever_Pool[wk->wu.id] & 0xC) {
                Lever_Pool[wk->wu.id] ^= 0xC;
            }
        }
        /* fallthrough */

    case 1:
        if (wk->permited_koa & 2) {
            CP_Index[wk->wu.id][2]++;
        } else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Timer_00[wk->wu.id]++;
            break;
        }

    default:
        if (++Timer_00[wk->wu.id] < Timer_01[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        } else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
            Tech_Index[wk->wu.id] += 4;
            if (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]] == 0x1C) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

void Setup_Command_01(PLW* wk) {
    switch (Lever_Pool[wk->wu.id]) {
    case 2:
        Timer_00[wk->wu.id] = Lever_Store[wk->wu.id][0];
        break;
    default:
        Timer_00[wk->wu.id] = Lever_Store[wk->wu.id][2];
        break;
    }
}

void Check_Store_Lever(PLW* wk, u16 Tech_Number, s16 Next_Action, s16 Next_Menu) {
    s16 time;
    u16 lever;

    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if (cmd_sel[wk->wu.id]) {
        Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
    } else {
        Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
    }
    time = Tech_Address[wk->wu.id][13] + 2;
    lever = Tech_Address[wk->wu.id][15];

    if (Check_Store_Direction(wk, lever, time) != 0) {
        CP_Index[wk->wu.id][0]++;
    } else {
        Next_Another_Menu(wk, Next_Action, Next_Menu);
    }
}

s32 Check_Store_Direction(PLW* wk, u16 lever, s16 time) {
    if (wk->wu.rl_waza) {
        if (lever & (SWK_LEFT | SWK_RIGHT)) {
            lever ^= (SWK_LEFT | SWK_RIGHT);
        }
    }

    switch (lever) {
    case SWK_DOWN:
        if (time <= Lever_Store[wk->wu.id][0]) {
            return 1;
        }

        break;

    case SWK_RIGHT:
        if (time <= Lever_Store[wk->wu.id][1]) {
            return 1;
        }

        break;

    case SWK_LEFT:
        if (time <= Lever_Store[wk->wu.id][2]) {
            return 1;
        }

        break;
    }

    return 0;
}

s32 Select_Combo_Speed(PLW* wk) {
    s8 xx;
    s8 zz;

    xx = (u8)random_32_com();

    Lv = Setup_Lv18(8);
    Lv += CC_Value[0];

    if (Break_Into_CPU == 2) {
        Lv = 0x13;
    }

    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Lv = emLevelRemake(Lv, 0x14, 2);

    if ((Break_Into_CPU == 1) || (Break_Into_CPU == 2)) {
        return zz = Combo_Speed_Unit_Data[17][Lv][xx];
    }
    return zz = Combo_Speed_Unit_Data[wk->player_number][Lv][xx];
}

s32 Select_Reflection_Time(PLW* wk) {
    s8 Lv;
    s8 xx;
    s8 zz;

    xx = (u8)random_32_com();
    Lv = Setup_Lv18(save_w[Present_Mode].Difficulty + 0);
    Lv += CC_Value[0];
    if (Break_Into_CPU == 2) {
        Lv = 0x13;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Lv = emLevelRemake(Lv, 0x14, 2);

    if ((Break_Into_CPU == 1) || (Break_Into_CPU == 2)) {
        return zz = Reflection_Speed_Unit_Data[17][Lv][xx];
    }
    return zz = Reflection_Speed_Unit_Data[My_char[wk->wu.id]][Lv][xx];
}

s32 Setup_Lv04(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_04_Data[xx];

    for (i = 0; i < 3; i++) {
        if (Control_Time <= zz[i]) {
            return i;
        }
    }
    return 3;
}

s32 Setup_Lv08(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_08_Data[xx];

    for (i = 0; i < 7; i++) {
        if (Control_Time <= zz[i]) {
            break;
        }
    }
    return i;
}

s32 Setup_Lv10(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_10_Data[xx];

    for (i = 0; i < 9; i++) {
        if (Control_Time <= zz[i]) {
            break;
        }
    }
    return i;
}

s32 Setup_Lv18(s16 xx) {
    s16 i;
    s16* zz;

    zz = (s16*)&Level_18_Data[xx];

    for (i = 0; i < 17; i++) {
        if (Control_Time <= zz[i]) {
            break;
        }
    }
    return i;
}

s32 Setup_VS_Catch_Data(PLW* wk) {
    Lv = Setup_Lv08(0);
    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    return VS_Catch_Data[emLevelRemake(Lv, 8, 0)];
}

s32 Setup_LP_Data(PLW* wk) {
    Lv = Setup_Lv08(0);
    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    return LOOK_POSITION_Data[emLevelRemake(Lv, 8, 0)][random_32_com()];
}

s32 Setup_WT_Data(PLW* wk) {
    Lv = Setup_Lv04(0);
    if (Break_Into_CPU == 2) {
        Lv = 3;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    return Wait_Time_Data[emLevelRemake(Lv, 4, 0)][random_16_com() & 7];
}

void Ck_Distance(PLW* wk) {
    PL_Distance[wk->wu.id] = ((WORK*)wk->wu.target_adrs)->xyz[0].disp.pos - wk->wu.xyz[0].disp.pos;
    if (PL_Distance[wk->wu.id] < 0) {
        PL_Distance[wk->wu.id] = PL_Distance[wk->wu.id] * -1;
    }
}

s32 Ck_Distance_Height(PLW* wk) {
    s16 xx;

    xx = ((WORK*)wk->wu.target_adrs)->xyz[1].disp.pos - wk->wu.xyz[1].disp.pos;
    if (xx < 0) {
        xx = xx * -1;
    }
    return xx;
}

s32 Ck_Area(PLW* wk) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (PL_Distance[wk->wu.id] <= Separate_Area[wk->wu.id][i]) {
            return i;
        }
    }
    return 3;
}

s32 Ck_Area_Shell(PLW* wk) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (PL_Distance[wk->wu.id] <= Shell_Separate_Area[wk->wu.id][i]) {
            return i;
        }
    }

    return 3;
}

void Ck_Distance_Lv(PLW* wk) {
    PL_Distance[wk->wu.id] = ((WORK*)wk->wu.target_adrs)->xyz[0].disp.pos - wk->wu.xyz[0].disp.pos;

    if (PL_Distance[wk->wu.id] > 0) {
        Lever_Buff[wk->wu.id] = 8;
    } else {
        Lever_Buff[wk->wu.id] = 4;
        PL_Distance[wk->wu.id] = PL_Distance[wk->wu.id] * -1;
    }
}

void Ck_Distance_LvJ(PLW* wk) {
    PL_Distance[wk->wu.id] = ((WORK*)wk->wu.target_adrs)->xyz[0].disp.pos - wk->wu.xyz[0].disp.pos;

    if (PL_Distance[wk->wu.id] > 0) {
        Lever_Pool[wk->wu.id] = 9;
    } else {
        Lever_Pool[wk->wu.id] = 5;
        PL_Distance[wk->wu.id] = PL_Distance[wk->wu.id] * -1;
    }
}

void Next_End(PLW* wk) {
    if (Check_Guard(wk) != 0) {
        return;
    }

    CP_Index[wk->wu.id][0] = 0xFF;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
}

void Next_Another_Menu(PLW* wk, s16 Next_Action, u16 Next_Menu) {
    if (Next_Action != 1) {
        CP_No[wk->wu.id][0] = Next_Action;
        Pattern_Index[wk->wu.id] = Next_Menu;
        CP_Index[wk->wu.id][0] = 0;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
    } else {
        Next_Be_Free(wk);
    }
}

/* Reaction handling is skipped entirely while the character is in caution or
 * held by a throw. Both tests are read-only; the original broke out of the
 * switch on each in turn. */
s32 Ck_Distance_XX(s16 x1, s16 x2) {
    s16 xx;

    xx = x1 - x2;
    if (xx < 0) {
        xx = xx * -1;
    }

    return xx;
}

s32 Check_Behind(PLW* wk, WORK_Other* tmw) {
    if (wk->wu.rl_waza == 0) {
        return wk->wu.xyz[0].disp.pos < tmw->wu.xyz[0].disp.pos;
    }
    return wk->wu.xyz[0].disp.pos > tmw->wu.xyz[0].disp.pos;
}

void Setup_Lever_LR(PLW* wk, s16 PL_id, s16 Lever) {
    if (Lever == 0) {
        Lever_LR[PL_id] = 0;
        return;
    }

    if (Lever & 0x1000) {
        Lever_LR[PL_id] = 1;
    }

    else if (Lever & 0x2000) {
        Lever_LR[PL_id] |= 2;
    }

    if (Lever & 0x4000) {
        Lever_LR[PL_id] |= Setup_Guard_Lever(wk, 0);
    } else if (Lever & 0x8000) {
        Lever_LR[PL_id] |= Setup_Guard_Lever(wk, 1);
    }
}
