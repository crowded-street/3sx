/**
 * @file com_sub_air_term.c
 * CPU Controlled Character airborne attack Term state machines.
 *
 * Split out of com_sub_jump.c unchanged - no function body was edited.
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

/* The rise arm the two ORO Terms share. */
static void ORO_Air_Rise(PLW* wk) {
    if (wk->wu.xyz[1].disp.pos > 0) {
        CP_Index[wk->wu.id][1]++;
        return;
    }

    Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
    Timer_00[wk->wu.id] = 2;
}

/* The height gates that apply only when a lever was given. Non-zero when one
 * of them failed. */
static s32 ORO_Air_Climb_Blocked(PLW* wk, const ORO_Air_Term_Args* a) {
    if (Check_Landed(wk, a->Reaction) != 0) {
        return 1;
    }
    if (Check_VS_Air_Attack(wk, a->RJX, a->RJY, a->JLD) != 0) {
        return 1;
    }
    return Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, a->JY) == 0;
}

/* Take the second jump, if the character is high enough and has one left. */
static void ORO_Air_Second_Jump(PLW* wk, s16 Jump_Dir2, u16 Lever_Data) {
    if ((wk->air_jump_ok_time != 0) || (wk->wu.position_y < 0x30)) {
        return;
    }

    Jump_Init(wk, Jump_Dir2);

    if ((Lever_Data) == 0xFFFF) {
        CP_Index[wk->wu.id][1] += 2;
    } else {
        CP_Index[wk->wu.id][1]++;
    }
}

/* The second jump: the range gates only apply when a lever was given, and the
 * step taken afterwards depends on the same thing. */
static void ORO_Air_Climb(PLW* wk, const ORO_Air_Term_Args* a) {
    Check_Air_Guard(wk);

    if ((a->Lever_Data != 0xFFFF) && ORO_Air_Climb_Blocked(wk, a)) {
        return;
    }

    ORO_Air_Second_Jump(wk, a->Jump_Dir2, a->Lever_Data);
}

/* Commit to the attack once the approach gates pass. */
static void ORO_Air_Strike(PLW* wk, const ORO_Air_Term_Args* a) {
    Check_Air_Guard(wk);

    if (Attack_Range_Gates(wk, a->Reaction, a->RX, a->RY, a->RJX, a->RJY, a->JLD) == 0) {
        return;
    }

    Lever_Buff[wk->wu.id] = a->Lever_Data;

    CP_Index[wk->wu.id][1]++;
    Stock_Hit_Flag[wk->wu.id] = 0;
}

/* Hold while airborne: latch any hit, then watch for the landing. The three
 * air Terms differ only in the reaction mask they use here. */
static void Air_Term_Hold(PLW* wk, s16 Reaction, s16 mask) {
    if (wk->wu.hf.hit.player) {
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    }
    Check_Landed(wk, Reaction & mask);
}

/* Run the landing opcode once down. Callers pass the reaction already masked,
 * because they do not all mask it the same way. */
static void Air_Term_Land(PLW* wk, s16 reaction) {
    if (Check_Landed(wk, reaction) != 0) {
        return;
    }

    Landing_Tech_Step(wk);
}

/* The unconditional landing arm shared by all three air Terms. */
static void Air_Term_End(PLW* wk, s16 Reaction) {
    Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    Check_Landed(wk, Reaction & 0xFFF);
}

/* Step the state on and load the hi-jump tech script. Shared by
 * Hi_Jump_Attack_Term and ORO_HJA_Term; only what precedes it differs. */
static void Air_Term_Load_Tech(PLW* wk) {
    CP_Index[wk->wu.id][1]++;
    if (cmd_sel[wk->wu.id]) {
        Tech_Address[wk->wu.id] = player_CMD[wk->player_number][2];
    } else {
        Tech_Address[wk->wu.id] = player_cmd[wk->player_number][2];
    }
    Check_First_Menu(wk);
}

/* The shell check every launch arm runs last. */
static void Free_On_Diagonal_Shell(PLW* wk) {
    if (Check_Diagonal_Shell(wk) != 0) {
        Next_Be_Free(wk);
    }
}

/* The opening the two hi-jump attack Terms share: the passive, special-move and
 * hi-jump gates, then the menu reset. Non-zero when one of the gates stopped the
 * opening. Only the statement before it and the hit reset after it differ
 * between the two callers. */
static s32 HJA_Term_Opening_Blocked(PLW* wk) {
    if (Check_Passive(wk) != 0) {
        return 1;
    }

    if (wk->spmv_ng_flag & 0x30000) {
        Next_Be_Free(wk);
        return 1;
    }
    if (Check_Start_Hi_Jump(wk) != 0) {
        return 1;
    }

    Continue_Menu[wk->wu.id] = 0;
    return 0;
}

static void HJA_Term_Begin(PLW* wk, s16 Reaction) {
    Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
    if (HJA_Term_Opening_Blocked(wk)) {
        return;
    }

    wk->wu.hf.hit.player = 0;
    Air_Term_Load_Tech(wk);
}

static void HJA_Term_Launch(PLW* wk, s16 Jump_Dir) {
    if (Check_Passive(wk) != 0) {
        return;
    }
    if (--Combo_Speed[wk->wu.id] != 0) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Tech_Index[wk->wu.id] = 0xC;

    dash_flag_clear(wk->wu.id);
    Jump_Init(wk, Jump_Dir);
    Lever_Pool[wk->wu.id] &= 0xC;
    Lever_Buff[wk->wu.id] = 0;
    Free_On_Diagonal_Shell(wk);
}

/* Non-zero when Hi_Jump_Attack_Term must return outright: this arm used a bare
 * return where the others break, so it skips the trailing lever merge. */
static s32 HJA_Term_Command(PLW* wk) {
    if (Check_Passive(wk) != 0) {
        return 0;
    }

    if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
        CP_Index[wk->wu.id][1]++;
        Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
        return 0;
    }

    if (Lever_Buff[wk->wu.id] & 2) {
        return 1;
    }
    Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];

    return 0;
}

static void HJA_Term_Rise(PLW* wk) {
    if (wk->wu.xyz[1].disp.pos > 0) {
        CP_Index[wk->wu.id][1]++;
        return;
    }

    Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id] | 1;
}

static void HJA_Term_Approach(
    PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Range_JX, s16 Range_JY, u16 J_Lever_Data
) {
    Check_Air_Guard(wk);
    if (Check_Landed(wk, Reaction) != 0) {
        return;
    }

    if (Check_VS_Air_Attack(wk, Range_JX, Range_JY, J_Lever_Data) != 0) {
        return;
    }

    if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], Range_X) == 0) {
        return;
    }
    if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
        return;
    }
    if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, Range_Y) == 0) {
        return;
    }

    Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
    Lever_Buff[wk->wu.id] = Lever_Data;
    CP_Index[wk->wu.id][1]++;
    if (Reaction & 0x80) {
        CP_Index[wk->wu.id][1] = 8;
    }
}

static void HJA_Term_Recover(PLW* wk) {
    if (--Combo_Speed[wk->wu.id]) {
        return;
    }

    Lever_Buff[wk->wu.id] = Tech_Address[wk->wu.id][8];
    CP_Index[wk->wu.id][1]++;
}

static void HJA_Term_Meoshi(PLW* wk) {
    if (Attack_Flag[wk->wu.id]) {
        return;
    }

    CP_Index[wk->wu.id][1] = 8;
    if (wk->wu.hf.hit.player == 0) {
        return;
    }

    if (wk->wu.cg_cancel & 8) {
        Lever_Buff[wk->wu.id] = Get_Meoshi_Data(wk);
    }
}

/* The airborne half of Hi_Jump_Attack_Term: everything from the approach
 * onwards. The case labels are the original ones, so this reads against the same
 * state numbers as the ground half it was lifted out of. None of these arms can
 * skip the trailing lever merge, so this returns nothing. */
static void HJA_Term_Airborne(
    PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Range_JX, s16 Range_JY, u16 J_Lever_Data
) {
    switch (CP_Index[wk->wu.id][1]) {

    case 4:
        HJA_Term_Approach(wk, Range_X, Range_Y, Reaction, Lever_Data, Range_JX, Range_JY, J_Lever_Data);
        break;

    case 5:
        Air_Term_Hold(wk, Reaction, 0xFFF);
        break;

    case 6:
        Air_Term_Land(wk, Reaction);
        break;

    case 7:
        HJA_Term_Recover(wk);
        break;

    case 8:
        Air_Term_End(wk, Reaction);
        break;

    default:
        HJA_Term_Meoshi(wk);
        break;
    }
}

/* Non-zero when the trailing lever merge must be skipped. */
static s32 HJA_Term_Step(
    PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX, s16 Range_JY,
    u16 J_Lever_Data
) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        HJA_Term_Begin(wk, Reaction);
        break;

    case 1:
        HJA_Term_Launch(wk, Jump_Dir);
        break;

    case 2:
        return HJA_Term_Command(wk);

    case 3:
        HJA_Term_Rise(wk);
        break;

    default:
        HJA_Term_Airborne(wk, Range_X, Range_Y, Reaction, Lever_Data, Range_JX, Range_JY, J_Lever_Data);
        break;
    }

    return 0;
}

void Hi_Jump_Attack_Term(
    PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX, s16 Range_JY,
    u16 J_Lever_Data
) {
    if (HJA_Term_Step(wk, Range_X, Range_Y, Reaction, Lever_Data, Jump_Dir, Range_JX, Range_JY, J_Lever_Data) != 0) {
        return;
    }

    if (CP_Index[wk->wu.id][1] >= 4) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

s32 Check_Term_ABS_Distance(PLW* wk) {
    if (Turn_Over[wk->wu.id]) {
        return 1;
    }

    if (My_char[wk->wu.id] == 5) {
        return 0;
    }

    if (PL_Distance[wk->wu.id] < 0x31) {
        return 1;
    }

    if (wk->wu.mvxy.a[1].real.h >= 0) {
        return 0;
    }

    if (wk->wu.xyz[1].disp.pos < 0x31) {
        return 1;
    }

    return 0;
}

s32 Check_Com_Add_Y(PLW* wk, s16 Pos_Y, s16 Range) {
    if (Range == -1) {
        return 1;
    }
    if (!(Range & 0x8000)) {
        return Pos_Y >= Range;
    }
    if (wk->wu.mvxy.a[1].real.h >= 0) {
        return 0;
    }
    return Pos_Y <= (Range & 0x7FFF);
}

/* The approach gates every airborne attack Term function runs before it commits.
 * Returns 0 as soon as one fails, which is where each caller used to break.
 *
 * Check_Landed and Check_VS_Air_Attack mutate state, so the order and the
 * short-circuiting here are load-bearing - they match the original exactly.
 * Check_Air_Guard is deliberately left at the call sites: one caller sets
 * Stock_Hit_Flag between it and the first gate. */
s32 Attack_Range_Gates(PLW* wk, s16 Reaction, s16 RX, s16 RY, s16 RJX, s16 RJY, u16 JLD) {
    if (Check_Landed(wk, Reaction) != 0) {
        return 0;
    }

    if (Check_VS_Air_Attack(wk, RJX, RJY, JLD) != 0) {
        return 0;
    }
    if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], RX) == 0) {
        return 0;
    }
    if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
        return 0;
    }
    if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, RY) == 0) {
        return 0;
    }
    return 1;
}

static void ORO_JA_Term_Launch(PLW* wk, s16 Jump_Dir) {
    if (Check_Passive(wk) != 0) {
        return;
    }
    if (--Combo_Speed[wk->wu.id] != 0) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    dash_flag_clear(wk->wu.id);

    Jump_Init(wk, Jump_Dir);
    Check_Air_Guard(wk);
    Free_On_Diagonal_Shell(wk);
}

/* CP_Index 0. Non-zero when the state advanced and case 1 runs this frame;
 * every early exit here broke out of the switch instead. */
static s32 ORO_JA_Term_Begin(PLW* wk, s16 Reaction) {
    if (wk->spmv_ng_flag & 0x30000) {
        Next_Be_Free(wk);
        return 0;
    }

    Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);

    if ((wk->wu.routine_no[1] == 4) && (wk->wu.cg_type != 0x40)) {
        return 0;
    }

    hi_jump_flag_clear(wk->wu.id);
    Continue_Menu[wk->wu.id] = 0;

    wk->wu.hf.hit.player = 0;
    CP_Index[wk->wu.id][1]++;
    Check_First_Menu(wk);

    return 1;
}

void ORO_JA_Term(PLW* wk, const ORO_Air_Term_Args* a) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (!ORO_JA_Term_Begin(wk, a->Reaction)) {
            break;
        }
        /* fallthrough */

    case 1:
        ORO_JA_Term_Launch(wk, a->Jump_Dir);
        break;

    case 2:
        ORO_Air_Rise(wk);
        break;

    case 3:
        ORO_Air_Climb(wk, a);
        break;

    case 4:
        ORO_Air_Strike(wk, a);
        break;

    case 5:
        Air_Term_Hold(wk, a->Reaction, 0x7F);
        break;

    case 6:
        Air_Term_Land(wk, a->Reaction & 0x7F);
        break;

    default:
        Air_Term_End(wk, a->Reaction);
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

static void ORO_HJA_Term_Begin(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if (HJA_Term_Opening_Blocked(wk)) {
        return;
    }

    Air_Term_Load_Tech(wk);
}

/* NOTE: the combo test reads Combo_Speed[wk->wu.id == 0] - the bracket encloses
 * the comparison, so it decrements Combo_Speed[0] or [1] according to whether
 * the id is zero rather than testing --Combo_Speed[id] == 0 as the sibling
 * Terms do. Copied verbatim; the same shape appears in
 * Hi_Jump_Command_Attack_Term. */
static void ORO_HJA_Term_Launch(PLW* wk, s16 Jump_Dir) {
    if (Check_Passive(wk) != 0) {
        return;
    }
    if (!(--Combo_Speed[wk->wu.id == 0])) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Tech_Index[wk->wu.id] = 0xC;

    Jump_Init(wk, Jump_Dir);
    Lever_Pool[wk->wu.id] &= 0xC;
    Lever_Buff[wk->wu.id] = 0;
    Check_Air_Guard(wk);
    Free_On_Diagonal_Shell(wk);
}

static void ORO_HJA_Term_Arm(PLW* wk) {
    if (Check_Passive(wk) != 0) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Lever_Buff[wk->wu.id] = 2;
    Lever_Pool[wk->wu.id] |= 1;
}

/* The airborne half of ORO_HJA_Term: everything from the rise onwards. The case
 * labels are the original ones, so this reads against the same state numbers as
 * the ground half it was lifted out of. */
static void ORO_HJA_Term_Airborne(PLW* wk, const ORO_Air_Term_Args* a) {
    switch (CP_Index[wk->wu.id][1]) {

    case 3:
        ORO_Air_Rise(wk);
        break;

    case 4:
        ORO_Air_Climb(wk, a);
        break;

    case 5:
        ORO_Air_Strike(wk, a);
        break;

    case 6:
        Air_Term_Hold(wk, a->Reaction, 0x7F);
        break;

    case 7:
        Air_Term_Land(wk, a->Reaction & 0x7F);
        break;

    default:
        Air_Term_End(wk, a->Reaction);
        break;
    }
}

void ORO_HJA_Term(PLW* wk, const ORO_Air_Term_Args* a) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        ORO_HJA_Term_Begin(wk);
        break;

    case 1:
        ORO_HJA_Term_Launch(wk, a->Jump_Dir);
        break;

    case 2:
        ORO_HJA_Term_Arm(wk);
        break;

    default:
        ORO_HJA_Term_Airborne(wk, a);
        break;
    }

    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}
