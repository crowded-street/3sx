/**
 * @file com_sub_jump.c
 * CPU Controlled Character jumps and airborne attacks.
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

/* Start the jump, freeing the character if a shell is already in the way.
 * Four of the five launch arms ran these two statements back to back. */
static void Start_Jump(PLW* wk, s16 Jump_Dir) {
    Jump_Init(wk, Jump_Dir);
    if (Check_Diagonal_Shell(wk) != 0) {
        Next_Be_Free(wk);
    }
}

/* Step the pattern on and clear the three sub-indices. Unlike
 * Next_Pattern_Step in com_sub.c, the landing arms leave the flip and limited
 * flags alone. */
static void Clear_Jump_Pattern(PLW* wk) {
    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
}

/* A jump that is special-move blocked frees the character instead. Returns
 * non-zero when the caller should stop. */
static s32 Check_Jump_Blocked(PLW* wk) {
    if (wk->spmv_ng_flag & 0x30000) {
        Next_Be_Free(wk);
        return 1;
    }
    return 0;
}

/* The opening of every jump launch: passive first, then the combo timer.
 * Combo_Speed is decremented only when Check_Passive declines, exactly as the
 * original ordering did. Returns non-zero when the launch may proceed. */
static s32 Check_Combo_Ready(PLW* wk) {
    if (Check_Passive(wk) != 0) {
        return 0;
    }
    return --Combo_Speed[wk->wu.id] == 0;
}


/* Step the held command; non-zero when it has just completed. */
static s32 Run_Hi_Jump_Command(PLW* wk) {
    if (Command_Type_00(wk, 8, 0xFFFF, -1) != -1) {
        return 0;
    }

    CP_Index[wk->wu.id][1]++;
    Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
    return 1;
}

/* The command arm shared by Hi_Jump and Hi_Jump_Attack.
 *
 * Returns non-zero when Hi_Jump_Attack must return outright: that arm used a
 * bare return where every other arm breaks, so it skips the trailing lever
 * merge, and the flag preserves that. Hi_Jump discards the result - it has no
 * trailing merge, and its own arm expressed the same choice as
 * "if (!(Lever_Buff & 2)) OR in Lever_Pool". */
static s32 Hi_Jump_Attack_Command(PLW* wk) {
    if (Check_Passive(wk) != 0) {
        return 0;
    }

    if (Run_Hi_Jump_Command(wk)) {
        return 0;
    }

    if (Lever_Buff[wk->wu.id] & 2) {
        return 1;
    }
    Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];

    return 0;
}

/* The rise off the ground, once the jump has left it. Jump and Hi_Jump ran
 * this identically. */
static void Jump_Rise_To_Air(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];

    if (wk->wu.xyz[1].disp.pos <= 0) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    Check_Air_Guard(wk);
}

static void Jump_Begin(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if (Check_Passive(wk) != 0) {
        return;
    }

    if (Check_Jump_Blocked(wk)) {
        return;
    }

    if (!Check_Free_To_Act(wk)) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    hi_jump_flag_clear(wk->wu.id);
    Check_First_Menu(wk);
}

static void Jump_Launch(PLW* wk, s16 Jump_Dir) {
    if (!Check_Combo_Ready(wk)) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Start_Jump(wk, Jump_Dir);
}

/* Unlike Hi_Jump's landing, this one does not run the air guard. */
static void Jump_Land(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if (wk->wu.xyz[1].disp.pos) {
        return;
    }

    Clear_Jump_Pattern(wk);
}

void Jump(PLW* wk, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Jump_Begin(wk);
        break;

    case 1:
        Jump_Launch(wk, Jump_Dir);
        break;

    case 2:
        Jump_Rise_To_Air(wk);
        break;

    default:
        Jump_Land(wk);
        break;
    }
}

static void Hi_Jump_Begin(PLW* wk, s16 Pl_Number) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if (Check_Passive(wk) != 0) {
        return;
    }

    if (Check_Jump_Blocked(wk)) {
        return;
    }

    if (Check_Start_Hi_Jump(wk) != 0) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    if (cmd_sel[wk->wu.id]) {
        Tech_Address[wk->wu.id] = player_CMD[Pl_Number][2];
    } else {
        Tech_Address[wk->wu.id] = player_cmd[Pl_Number][2];
    }
    Check_First_Menu(wk);
}

static void Hi_Jump_Launch(PLW* wk, s16 Jump_Dir) {
    if (!Check_Combo_Ready(wk)) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Tech_Index[wk->wu.id] = 0xC;

    Start_Jump(wk, Jump_Dir);

    Lever_Buff[wk->wu.id] = 0;
}



/* Landing clears only the four CP_Index fields - not the flip and limited flags
 * that Next_Pattern_Step also clears - so it stays written out. */
static void Hi_Jump_Land(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    Check_Air_Guard(wk);

    if (wk->wu.xyz[1].disp.pos) {
        return;
    }

    Clear_Jump_Pattern(wk);
}

void Hi_Jump(PLW* wk, s16 Pl_Number, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Hi_Jump_Begin(wk, Pl_Number);
        break;

    case 1:
        Hi_Jump_Launch(wk, Jump_Dir);
        break;

    case 2:
        Hi_Jump_Attack_Command(wk);
        break;

    case 3:
        Jump_Rise_To_Air(wk);
        break;

    default:
        Hi_Jump_Land(wk);
        break;
    }
}

s32 Check_Start_Hi_Jump(PLW* wk) {
    if (Check_Free_To_Act(wk)) {
        return 0;
    }

    if (wk->wu.cg_cancel & 4) {
        return 0;
    }

    if (wk->wu.cg_cancel & 1) {
        return 0;
    }

    return 1;
}

/* True while an incoming attack is close enough to be worth guarding. */
static s32 Check_Guard_In_Range(PLW* wk, WORK* em) {
    s16 xx;

    xx = Hit_Range_Data[em->hit_range] + 0x20;
    xx += Com_Width_Data[wk->wu.id];

    return PL_Distance[wk->wu.id] <= xx;
}

/* Pick the guard skill level for this decision. Rnd is drawn here because the
 * table is indexed with it below; both are file-scope scratch. */
static void Setup_Guard_Level(PLW* wk) {
    Lv = Setup_Lv10(0);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_16_com();
    Lv += CC_Value[0];

    if (Lv >= 7) {
        Lv = 0xA;
    }

    Lv = emLevelRemake(Lv, 0xB, 1);
}

s32 Check_Air_Guard(PLW* wk) {
    WORK* em;
    s16 zz;

    em = (WORK*)wk->wu.target_adrs;

    if (Lever_LR[wk->wu.id]) {
        return Lever_LR[wk->wu.id];
    }
    if (Guard_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return Lever_LR[wk->wu.id];
    }
    if (Attack_Flag[wk->wu.id] == 0) {
        return Lever_LR[wk->wu.id];
    }

    if (!Check_Guard_In_Range(wk, em)) {
        return 0;
    }

    Guard_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
    Setup_Guard_Level(wk);

    zz = Setup_EM_Rank_Index(wk);

    if (Guard_Data[zz][Lv][Rnd] == 3) {
        return Lever_LR[wk->wu.id] = 0;
    }
    Guard_Type[wk->wu.id] = Guard_Data[zz][Lv][random_16_ex_com()];

    Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
    Lever_LR[wk->wu.id] |= 2;
    return Guard_Type[wk->wu.id] |= 0x8000;
}

/* CP_Index 0. Returns non-zero when the state advanced, which is when the
 * original fell through into case 1 in the same frame; every early exit there
 * broke out of the switch instead. */
static s32 Jump_Attack_Begin(PLW* wk, s16 Reaction, s16 Time_Data) {
    Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
    if (Check_Passive(wk) != 0) {
        return 0;
    }

    if (Check_Jump_Blocked(wk)) {
        return 0;
    }

    if ((wk->wu.routine_no[1] == 4) && (wk->wu.cg_type != 0x40)) {
        return 0;
    }

    CP_Index[wk->wu.id][1]++;
    Timer_00[wk->wu.id] = Time_Data;
    Continue_Menu[wk->wu.id] = 0;
    wk->wu.hf.hit.player = 0;
    hi_jump_flag_clear(wk->wu.id);
    Check_First_Menu(wk);

    return 1;
}

static void Jump_Attack_Launch(PLW* wk, s16 Time_Data, s16 Jump_Dir) {
    if (!Check_Combo_Ready(wk)) {
        return;
    }

    Timer_00[wk->wu.id] = Time_Data;
    CP_Index[wk->wu.id][1]++;
    dash_flag_clear(wk->wu.id);
    Start_Jump(wk, Jump_Dir);
}

static void Jump_Attack_Rise(PLW* wk) {
    if (wk->wu.xyz[1].disp.pos > 0) {
        CP_Index[wk->wu.id][1]++;
        return;
    }

    Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
}

static void Jump_Attack_Fire(PLW* wk, u16 Lever_Data) {
    Check_Air_Guard(wk);

    if (--Timer_00[wk->wu.id] != 0) {
        return;
    }

    Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
    Lever_Buff[wk->wu.id] = Lever_Data;
    CP_Index[wk->wu.id][1]++;
}

static void Jump_Attack_Land(PLW* wk, s16 Reaction) {
    Check_Air_Guard(wk);
    if (wk->wu.hf.hit.player) {
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    }
    Check_Landed(wk, Reaction & 0xFFF);
}

void Jump_Attack(PLW* wk, const Jump_Attack_Args* a) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (!Jump_Attack_Begin(wk, a->Reaction, a->Time_Data)) {
            break;
        }
        /* fallthrough */

    case 1:
        Jump_Attack_Launch(wk, a->Time_Data, a->Jump_Dir);
        break;

    case 2:
        Jump_Attack_Rise(wk);
        break;

    case 3:
        Jump_Attack_Fire(wk, a->Lever_Data);
        break;

    default:
        Jump_Attack_Land(wk, a->Reaction);
        break;
    }

    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

/* CP_Index 3: airborne approach. Each range gate that fails leaves CP_Index
 * alone so the next frame retries. Lever_Data is a by-value copy; the original
 * reassigned its own parameter here and nothing downstream read it back. */
static void Jump_Attack_Term_Approach(PLW* wk, const Jump_Term_Args* a) {
    u16 Lever_Data;

    Check_Air_Guard(wk);
    if (Check_Landed(wk, a->Reaction) != 0) {
        return;
    }

    if (Check_VS_Air_Attack(wk, a->Range_JX, a->Range_JY, a->J_Lever_Data) != 0) {
        return;
    }
    Check_Term_ABS_Distance(wk);

    if (Check_Term_Sub(wk, PL_Distance[wk->wu.id], a->Range_X) == 0) {
        return;
    }
    if (Check_Com_Add_Y(wk, wk->wu.xyz[1].disp.pos, a->Range_Y) == 0) {
        return;
    }
    if (Check_Term_Sub(wk, wk->wu.xyz[1].disp.pos, a->Range_Y) == 0) {
        return;
    }

    Lever_Data = Check_SP_Jump_Attack(wk, a->Lever_Data);
    Lever_Buff[wk->wu.id] = Lever_Data;

    CP_Index[wk->wu.id][1]++;
    Stock_Hit_Flag[wk->wu.id] = 0;
}

/* The landing opcode step shared by Jump_Attack_Term, Hi_Jump_Attack_Term,
 * ORO_JA_Term and ORO_HJA_Term - the same switch appeared verbatim in all
 * four. The inner switch has no break on its final arm in the original;
 * preserved as found. */
void Landing_Tech_Step(PLW* wk) {
    switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {
    default:
    case 1:
    case 10:
        if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
            CP_Index[wk->wu.id][1] = 0x63;
        }
    }
}

/* CP_Index 0. Non-zero when the state advanced and case 1 runs this frame. */
static s32 Jump_Attack_Term_Begin(PLW* wk, s16 Reaction) {
    Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);

    if (Check_Jump_Blocked(wk)) {
        return 0;
    }
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

/* Unlike Jump_Attack's launch, this one does not reload Timer_00. */
static void Jump_Attack_Term_Launch(PLW* wk, s16 Jump_Dir) {
    if (!Check_Combo_Ready(wk)) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    dash_flag_clear(wk->wu.id);

    Start_Jump(wk, Jump_Dir);
}

static void Jump_Attack_Term_Hold(PLW* wk, s16 Reaction) {
    Check_Air_Guard(wk);
    if (wk->wu.hf.hit.player) {
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    }
    Check_Landed(wk, Reaction & 0x7F);
}

static void Jump_Attack_Term_Finish(PLW* wk, s16 Reaction) {
    if (Check_Landed(wk, Reaction & 0x7F) == 0) {
        Landing_Tech_Step(wk);
    }
}

static void Jump_Attack_Term_Land(PLW* wk, s16 Reaction) {
    Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    Check_Landed(wk, Reaction & 0xFFF);
}

/* The airborne half of Jump_Attack_Term: everything from the rise onwards. The
 * case labels are the original ones, so this reads against the same state
 * numbers as the ground half it was lifted out of. */
static void Jump_Attack_Term_Airborne(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][1]) {

    case 2:
        Jump_Attack_Rise(wk);
        break;

    case 3:
        Jump_Attack_Term_Approach(wk, a);
        break;

    case 4:
        Jump_Attack_Term_Hold(wk, a->Reaction);
        break;

    case 5:
        Jump_Attack_Term_Finish(wk, a->Reaction);
        break;

    default:
        Jump_Attack_Term_Land(wk, a->Reaction);
        break;
    }
}

void Jump_Attack_Term(PLW* wk, const Jump_Term_Args* a) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (!Jump_Attack_Term_Begin(wk, a->Reaction)) {
            break;
        }
        /* Fallthrough */

    case 1:
        Jump_Attack_Term_Launch(wk, a->Jump_Dir);
        break;

    default:
        Jump_Attack_Term_Airborne(wk, a);
        break;
    }

    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

s32 Check_SP_Jump_Attack(PLW* wk, s16 Lever_Data) {
    u16 xx;

    if (!(Lever_Data & 0x8000)) {
        return Lever_Data;
    }

    xx = Setup_Guard_Lever(wk, 0);
    xx |= Lever_Data & 0x7FFF;
    return xx | 2;
}

/* The armed branch of Check_VS_Air_Attack: once the reflection timer expires,
 * commit to either a command attack (-1) or a plain lever press (1). Returns 0
 * while the timer is still counting, which is what the original fell through
 * to at the end of the function. */
static s32 Check_VS_Air_Attack_Fire(PLW* wk, s16 J_Lever_Data) {
    if (--Timer_01[wk->wu.id] != 0) {
        return 0;
    }

    if (J_Lever_Data & 0x4000) {
        CP_Index[wk->wu.id][1] += 2;
        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][J_Lever_Data & 0x3FFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][J_Lever_Data & 0x3FFF];
        }
        Continue_Menu[wk->wu.id] = 1;
        return -1;
    }

    Lever_Buff[wk->wu.id] = J_Lever_Data;
    CP_Index[wk->wu.id][1]++;
    Continue_Menu[wk->wu.id] = 1;
    return 1;
}

/* The target must be in one of the three airborne patterns, or off the ground,
 * for an air-to-air answer to make sense. */
static s32 Check_Target_Airborne(WORK* em) {
    return (em->pat_status == 0xE) || (em->pat_status == 0x14) || (em->pat_status == 0x1A) ||
           (em->xyz[1].disp.pos > 0);
}

s32 Check_VS_Air_Attack(PLW* wk, s16 Range_JX, s16 Range_JY, s16 J_Lever_Data) {
    WORK* em;

    if ((Range_JX == -1) && (Range_JY == -1)) {
        return 0;
    }
    if (J_Lever_Data == -1) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;
    if (!Check_Target_Airborne(em)) {
        return 0;
    }

    if (Check_Term_Sub_Air(wk, PL_Distance[wk->wu.id], Range_JX) == 0) {
        return 0;
    }
    if (Check_Term_Sub(wk, Ck_Distance_Height(wk), Range_JY) == 0) {
        return 0;
    }

    if (CP_Index[wk->wu.id][2] != 0) {
        return Check_VS_Air_Attack_Fire(wk, J_Lever_Data);
    }

    CP_Index[wk->wu.id][2]++;
    Timer_01[wk->wu.id] = Select_Reflection_Time(wk);
    Timer_01[wk->wu.id]++;

    return 0;
}

static void Hi_Jump_Attack_Start(PLW* wk, s16 Reaction, s16 Time_Data) {
    Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
    if (Check_Passive(wk) != 0) {
        return;
    }

    if (Check_Jump_Blocked(wk)) {
        return;
    }
    if (Check_Start_Hi_Jump(wk) != 0) {
        return;
    }

    Continue_Menu[wk->wu.id] = 0;
    wk->wu.hf.hit.player = 0;
    CP_Index[wk->wu.id][1]++;
    if (cmd_sel[wk->wu.id]) {
        Tech_Address[wk->wu.id] = player_CMD[wk->player_number][2];
    } else {
        Tech_Address[wk->wu.id] = player_cmd[wk->player_number][2];
    }
    Timer_00[wk->wu.id] = Time_Data;
    Check_First_Menu(wk);
}

static void Hi_Jump_Attack_Launch(PLW* wk, s16 Jump_Dir) {
    if (!Check_Combo_Ready(wk)) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Tech_Index[wk->wu.id] = 0xC;

    dash_flag_clear(wk->wu.id);
    Jump_Init(wk, Jump_Dir);
    Lever_Pool[wk->wu.id] &= 0xC;
    Lever_Buff[wk->wu.id] = 0;
    Check_Air_Guard(wk);
    if (Check_Diagonal_Shell(wk) != 0) {
        Next_Be_Free(wk);
    }
}


static void Hi_Jump_Attack_Rise(PLW* wk) {
    if (wk->wu.xyz[1].disp.pos > 0) {
        CP_Index[wk->wu.id][1]++;
        return;
    }

    Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id] | 1;
}

static void Hi_Jump_Attack_Fire(PLW* wk, s16 Reaction, u16 Lever_Data) {
    Check_Air_Guard(wk);

    if (--Timer_00[wk->wu.id] != 0) {
        return;
    }

    Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
    Lever_Buff[wk->wu.id] = Lever_Data;
    CP_Index[wk->wu.id][1] += 2;
    if (Reaction & 0x80) {
        CP_Index[wk->wu.id][1]++;
    }
}

static void Hi_Jump_Attack_Meoshi(PLW* wk) {
    Check_Air_Guard(wk);
    if (Attack_Flag[wk->wu.id]) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    if (wk->wu.hf.hit.player == 0) {
        return;
    }

    if (!(wk->wu.cg_cancel & 8)) {
        return;
    }

    Lever_Buff[wk->wu.id] = Get_Meoshi_Data(wk);
}

static void Hi_Jump_Attack_Land(PLW* wk, s16 Reaction) {
    if (wk->wu.hf.hit.player) {
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    }
    Check_Landed(wk, Reaction & 0xFFF);
}

/* Returns non-zero when the trailing lever merge must be skipped. */
static s32 Hi_Jump_Attack_Step(PLW* wk, const Jump_Attack_Args* a) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Hi_Jump_Attack_Start(wk, a->Reaction, a->Time_Data);
        break;

    case 1:
        Hi_Jump_Attack_Launch(wk, a->Jump_Dir);
        break;

    case 2:
        return Hi_Jump_Attack_Command(wk);

    case 3:
        Hi_Jump_Attack_Rise(wk);
        break;

    case 4:
        Hi_Jump_Attack_Fire(wk, a->Reaction, a->Lever_Data);
        break;

    case 6:
        Hi_Jump_Attack_Meoshi(wk);
        break;

    default:
        Hi_Jump_Attack_Land(wk, a->Reaction);
        break;
    }

    return 0;
}

void Hi_Jump_Attack(PLW* wk, const Jump_Attack_Args* a) {
    if (Hi_Jump_Attack_Step(wk, a) != 0) {
        return;
    }

    if (CP_Index[wk->wu.id][1] >= 4) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}
