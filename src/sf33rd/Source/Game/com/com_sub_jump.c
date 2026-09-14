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

    if (wk->spmv_ng_flag & 0x30000) {
        Next_Be_Free(wk);
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
    if (Check_Passive(wk) != 0) {
        return;
    }

    if (--Combo_Speed[wk->wu.id] != 0) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Jump_Init(wk, Jump_Dir);
    if (Check_Diagonal_Shell(wk) != 0) {
        Next_Be_Free(wk);
    }
}

/* Unlike Hi_Jump's landing, this one does not run the air guard. */
static void Jump_Land(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if (wk->wu.xyz[1].disp.pos) {
        return;
    }

    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
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

    if (wk->spmv_ng_flag & 0x30000) {
        Next_Be_Free(wk);
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
    if (Check_Passive(wk) != 0) {
        return;
    }
    if (--Combo_Speed[wk->wu.id] != 0) {
        return;
    }

    CP_Index[wk->wu.id][1]++;
    Tech_Index[wk->wu.id] = 0xC;

    Jump_Init(wk, Jump_Dir);
    if (Check_Diagonal_Shell(wk) != 0) {
        Next_Be_Free(wk);
    }

    Lever_Buff[wk->wu.id] = 0;
}

static void Hi_Jump_Command(PLW* wk) {
    if (Check_Passive(wk) != 0) {
        return;
    }

    if (Command_Type_00(wk, 8, 0xFFFF, -1) == -1) {
        CP_Index[wk->wu.id][1]++;
        Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
        return;
    }

    if (!(Lever_Buff[wk->wu.id] & 2)) {
        Lever_Buff[wk->wu.id] |= Lever_Pool[wk->wu.id];
    }
}


/* Landing clears only the four CP_Index fields - not the flip and limited flags
 * that Next_Pattern_Step also clears - so it stays written out. */
static void Hi_Jump_Land(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    Check_Air_Guard(wk);

    if (wk->wu.xyz[1].disp.pos) {
        return;
    }

    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
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
        Hi_Jump_Command(wk);
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

s32 Check_Air_Guard(PLW* wk) {
    WORK* em;
    s16 xx;
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

    xx = Hit_Range_Data[em->hit_range] + 0x20;
    xx += Com_Width_Data[wk->wu.id];
    if (PL_Distance[wk->wu.id] > xx) {
        return 0;
    }

    Guard_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
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

    zz = Setup_EM_Rank_Index(wk);

    if (Guard_Data[zz][Lv][Rnd] == 3) {
        return Lever_LR[wk->wu.id] = 0;
    }
    Guard_Type[wk->wu.id] = Guard_Data[zz][Lv][random_16_ex_com()];

    Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
    Lever_LR[wk->wu.id] |= 2;
    return Guard_Type[wk->wu.id] |= 0x8000;
}

void Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }

        if ((wk->wu.routine_no[1] == 4) && (wk->wu.cg_type != 0x40)) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = Time_Data;
        Continue_Menu[wk->wu.id] = 0;
        wk->wu.hf.hit.player = 0;
        hi_jump_flag_clear(wk->wu.id);
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        Timer_00[wk->wu.id] = Time_Data;
        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);
        Jump_Init(wk, Jump_Dir);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        }

        else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        }

        break;

    case 3:
        Check_Air_Guard(wk);

        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        Lever_Data = Check_SP_Jump_Attack(wk, Lever_Data);
        Lever_Buff[wk->wu.id] = Lever_Data;
        CP_Index[wk->wu.id][1]++;

        break;

    default:
        Check_Air_Guard(wk);
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0xFFF);
        break;
    }
    if (CP_Index[wk->wu.id][1] >= 3) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

/* CP_Index 3: airborne approach. Each range gate that fails leaves CP_Index
 * alone so the next frame retries. Lever_Data is a by-value copy; the original
 * reassigned its own parameter here and nothing downstream read it back. */
static void Jump_Attack_Term_Approach(
    PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Range_JX, s16 Range_JY, s16 J_Lever_Data
) {
    Check_Air_Guard(wk);
    if (Check_Landed(wk, Reaction) != 0) {
        return;
    }

    if (Check_VS_Air_Attack(wk, Range_JX, Range_JY, J_Lever_Data) != 0) {
        return;
    }
    Check_Term_ABS_Distance(wk);

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

void Jump_Attack_Term(
    PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX, s16 Range_JY,
    s16 J_Lever_Data
) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);

        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }
        if ((wk->wu.routine_no[1] == 4) && (wk->wu.cg_type != 0x40)) {
            break;
        }

        hi_jump_flag_clear(wk->wu.id);
        Continue_Menu[wk->wu.id] = 0;

        wk->wu.hf.hit.player = 0;
        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* Fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (--Combo_Speed[wk->wu.id] != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        dash_flag_clear(wk->wu.id);

        Jump_Init(wk, Jump_Dir);
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
        }

        break;

    case 2:
        if (wk->wu.xyz[1].disp.pos > 0) {
            CP_Index[wk->wu.id][1]++;
        } else {
            Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        }
        break;

    case 3:
        Jump_Attack_Term_Approach(wk, Range_X, Range_Y, Reaction, Lever_Data, Range_JX, Range_JY, J_Lever_Data);
        break;

    case 4:
        Check_Air_Guard(wk);
        if (wk->wu.hf.hit.player) {
            Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        }
        Check_Landed(wk, Reaction & 0x7F);
        break;

    case 5:
        if (Check_Landed(wk, Reaction & 0x7F) == 0) {
            Landing_Tech_Step(wk);
        }
        break;
    default:
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        Check_Landed(wk, Reaction & 0xFFF);
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

s32 Check_VS_Air_Attack(PLW* wk, s16 Range_JX, s16 Range_JY, s16 J_Lever_Data) {
    WORK* em;

    if ((Range_JX == -1) && (Range_JY == -1)) {
        return 0;
    }
    if (J_Lever_Data == -1) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;
    if ((em->pat_status != 0xE) && (em->pat_status != 0x14) && (em->pat_status != 0x1A) && (em->xyz[1].disp.pos <= 0)) {
        return 0;
    }

    if (Check_Term_Sub_Air(wk, PL_Distance[wk->wu.id], Range_JX) == 0) {
        return 0;
    }
    if (Check_Term_Sub(wk, Ck_Distance_Height(wk), Range_JY) != 0) {
        switch (CP_Index[wk->wu.id][2]) {
        case 0:
            CP_Index[wk->wu.id][2]++;
            Timer_01[wk->wu.id] = Select_Reflection_Time(wk);
            Timer_01[wk->wu.id]++;
            break;
        default:
            return Check_VS_Air_Attack_Fire(wk, J_Lever_Data);
        }
    }

    return 0;
}

static void Hi_Jump_Attack_Start(PLW* wk, s16 Reaction, s16 Time_Data) {
    Setup_Lever_LR(wk, wk->wu.id, Reaction & 0xF000);
    if (Check_Passive(wk) != 0) {
        return;
    }

    if (wk->spmv_ng_flag & 0x30000) {
        Next_Be_Free(wk);
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
    Check_Air_Guard(wk);
    if (Check_Diagonal_Shell(wk) != 0) {
        Next_Be_Free(wk);
    }
}

/* Returns non-zero when Hi_Jump_Attack must return outright. This arm used a
 * bare return where every other arm breaks, so it skips the trailing lever
 * merge - that difference is preserved through this flag. */
static s32 Hi_Jump_Attack_Command(PLW* wk) {
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
static s32 Hi_Jump_Attack_Step(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        Hi_Jump_Attack_Start(wk, Reaction, Time_Data);
        break;

    case 1:
        Hi_Jump_Attack_Launch(wk, Jump_Dir);
        break;

    case 2:
        return Hi_Jump_Attack_Command(wk);

    case 3:
        Hi_Jump_Attack_Rise(wk);
        break;

    case 4:
        Hi_Jump_Attack_Fire(wk, Reaction, Lever_Data);
        break;

    case 6:
        Hi_Jump_Attack_Meoshi(wk);
        break;

    default:
        Hi_Jump_Attack_Land(wk, Reaction);
        break;
    }

    return 0;
}

void Hi_Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir) {
    if (Hi_Jump_Attack_Step(wk, Reaction, Time_Data, Lever_Data, Jump_Dir) != 0) {
        return;
    }

    if (CP_Index[wk->wu.id][1] >= 4) {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}
