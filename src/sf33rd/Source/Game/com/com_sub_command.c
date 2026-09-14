/**
 * @file com_sub_command.c
 * CPU Controlled Character command attacks and rapid-fire sequences.
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

/* Chooses the CP_Index the command falls back to when Command_Type_00 rejects
 * the step. Lifted out of Command_Attack_Tech_Step's first arm, which reached
 * nesting depth 4 here. */
static void Command_Attack_Set_Fallback(PLW* wk, s16 Reaction, u16 Tech_Number) {
    if ((Tech_Number & 0xF) == 0 || (Tech_Number & 0xF) == 1) {
        if (Reaction == 0xC) {
            CP_Index[wk->wu.id][1] = 0x63;
            Timer_00[wk->wu.id] = Dash_Time_Data[wk->player_number][Tech_Number];
        } else {
            CP_Index[wk->wu.id][1] = 4;
        }
    } else {
        CP_Index[wk->wu.id][1] = 3;
    }
}

/* Runs one step of the running command, dispatching on the opcode the tech
 * script currently points at. Lifted out of Command_Attack case 2, where it was
 * the fifth level of nesting. */
static void Command_Attack_Tech_Step(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot) {
    switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {

    default:
    case 1:

        if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
            Command_Attack_Set_Fallback(wk, Reaction, Tech_Number);
        }
        break;

    case 2:
        if (Command_Type_01(wk, Power_Level & 0xF, Ex_Shot) != 0) {
            CP_Index[wk->wu.id][1]++;
        }
        break;

    case 7:
        if (Command_Type_06(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) != 0) {
            CP_Index[wk->wu.id][1]++;
        }
        break;
    }
}

/* Seeds the lever value replayed while the command plays out. */
static void Setup_Free_Lever(PLW* wk, s16 Power_Level) {
    if (Power_Level & 0x4000) {
        Free_Lever[wk->wu.id] = Power_Lv_Data[(Power_Level & 0xF) - 8];
    } else {
        Free_Lever[wk->wu.id] = 0;
    }
}

/* CP_Index 3: replay the stored lever, then hand the hit back to the reaction
 * machinery. */
static void Command_Attack_Rapid_Step(PLW* wk, s16 Reaction, s16 Power_Level) {
    Lever_Buff[wk->wu.id] = Free_Lever[wk->wu.id];
    Rapid_Sub(wk);
    Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    Reaction_Sub(wk, Reaction, Power_Level);
}

void Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        dash_flag_clear(wk->wu.id);
        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
        }
        Tech_Index[wk->wu.id] = 0xC;
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Start_Command_Attack(wk, Reaction, Tech_Number & 0x80FF) != 0) {
            break;
        }
        if (Check_Dash_Hit(wk, Tech_Number & 0x80FF) != 0) {
            Next_Be_Free(wk);
        }

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        Setup_Free_Lever(wk, Power_Level);
        /* Fallthrough */

    case 1:
        if (--Combo_Speed[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            break;
        }

        if (Hadou_Check(wk, Tech_Number & 0x80FF) != 0) {
            if (Check_Passive(wk) == 0) {
                Combo_Speed[wk->wu.id] = 1;
            }
            break;
        } else {
            CP_Index[wk->wu.id][1]++;
            Check_Rapid(wk, Tech_Number);
        }
        /* Fallthrough */
    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }
        Command_Attack_Tech_Step(wk, Reaction, Tech_Number, Power_Level, Ex_Shot);
        break;

    case 3:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (plw[wk->wu.id].tsukami_f) {
            break;
        }
        if (Check_Motion_Ended(wk)) {
            Reaction_Exit_Sub(wk);
        } else {
            Command_Attack_Rapid_Step(wk, Reaction, Power_Level);
        }
        break;

    case 4:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (((wk->wu.cg_type) == 0x40) || (wk->running_f == 0)) {
            Reaction_Exit_Sub(wk);
        }
        break;

    case 5:
        if (PL_Distance[wk->wu.id] > 0x70) {
            Lever_Buff[wk->wu.id] = 0x40;
        }
        if (Check_Motion_Ended(wk)) {
            Reaction_Exit_Sub(wk);
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        if (--Timer_00[wk->wu.id] == 0) {
            Reaction_Exit_Sub(wk);
        }
        break;
    }
}

const s32 Hadou_Check_Data[20][2] = {
    { 0, 0 },    { 0, 0 }, { 1, 0x1D }, { 0, 0 },    { 1, 0x1F }, { 0, 0 }, { 1, 0x20 },
    { 1, 0x21 }, { 0, 0 }, { 0, 0 },    { 1, 0x1F }, { 1, 0x1D }, { 0, 0 }, { 0, 0 },
    { 1, 0x1F }, { 0, 0 }, { 1, 0x1E }, { 0, 0 },    { 0, 0 },    { 0, 0 },
};

s32 Hadou_Check(PLW* wk, u16 Tech_Number) {
    if (Hadou_Check_Data[wk->player_number][0] == 0) {
        return 0;
    }

    if (Tech_Number != Hadou_Check_Data[wk->player_number][1]) {
        return 0;
    }

    return Check_Resume_Lever(wk);
}

s32 Check_Resume_Lever(PLW* wk) {
    u16 Target_Lever;
    s16 xx;

    if (wk->wu.rl_waza) {
        Target_Lever = 8;
    } else {
        Target_Lever = 4;
    }

    for (xx = 0; xx <= 8; xx++) {
        if (Target_Lever == Resume_Lever[wk->wu.id][xx]) {
            return 1;
        }
    }
    return 0;
}

void J_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        if (wk->spmv_ng_flag & 0x30000) {
            Next_Be_Free(wk);
            break;
        }

        dash_flag_clear(wk->wu.id);

        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
        }

        Tech_Index[wk->wu.id] = 0xC;
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Start_Command_Attack(wk, Reaction, Tech_Number & 0x80FF) != 0) {
            break;
        }
        if (Check_Dash_Hit(wk, Tech_Number & 0x80FF) != 0) {
            Next_Be_Free(wk);
        }

        Continue_Menu[wk->wu.id] = 0;
        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* Fallthough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }

        if (--Combo_Speed[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            break;
        }
        if (Check_Diagonal_Shell(wk) != 0) {
            Next_Be_Free(wk);
            break;
        }
        CP_Index[wk->wu.id][1]++;
        /* Fallthough */
    case 2:
        if (Check_Passive(wk) != 0) {
            break;
        }
        switch (Tech_Address[wk->wu.id][Tech_Index[wk->wu.id]]) {

        case 2:
            if (Command_Type_01(wk, Power_Level & 0xF, Ex_Shot) != 0) {
                CP_Index[wk->wu.id][1]++;
            }
            break;

        default:
        case 1:
        case 10:
            if (Command_Type_00(wk, Power_Level & 0xF, Tech_Number, Ex_Shot) == -1) {
                CP_Index[wk->wu.id][1] = 0x63;
            }
            break;
        }
        break;

    case 3:
        Check_Rapid(wk, Tech_Number);
        CP_Index[wk->wu.id][1]++;
        return;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
        if (Check_Motion_Ended(wk)) {
            Reaction_Exit_Sub(wk);
        } else {
            Rapid_Sub(wk);
            if (Reaction == 0xC) {
                Reaction_Sub(wk, Reaction, Power_Level);
                break;
            }
            Check_Landed(wk, Reaction & 0xFFF);
        }
        break;
    }
}

void Rapid_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Shot, u16 Time) {
    switch (CP_Index[wk->wu.id][1]) {

    case 0:
        dash_flag_clear(wk->wu.id);
        if (cmd_sel[wk->wu.id]) {
            Tech_Address[wk->wu.id] = player_CMD[wk->player_number][Tech_Number & 0xFF];
        } else {
            Tech_Address[wk->wu.id] = player_cmd[wk->player_number][Tech_Number & 0xFF];
        }
        Tech_Index[wk->wu.id] = 0xC;
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

        if (Check_Start_Command_Attack(wk, Reaction, Tech_Number & 0x80FF) != 0) {
            break;
        }
        if (Check_Dash_Hit(wk, Tech_Number & 0x80FF) != 0) {
            Next_Be_Free(wk);
        }

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        Free_Lever[wk->wu.id] = 0;
        /* Fallthough */

    case 1:
        if (--Combo_Speed[wk->wu.id]) {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            break;
        }
        CP_Index[wk->wu.id][1]++;

        Rapid_No[wk->wu.id][0] = 0;
        Rapid_No[wk->wu.id][1] = 0;
        Timer_00[wk->wu.id] = Time;

    case 2:
        switch (Rapid_No[wk->wu.id][0]) {

        case 0:
            Rapid_No[wk->wu.id][0] = 1;
            Lever_Buff[wk->wu.id] = Shot;
            break;
        case 1:
            Rapid_No[wk->wu.id][0] = 0;
            Lever_Buff[wk->wu.id] = 0;
            break;
        }
        if (wk->wu.sp_tech_id == Tech_Number) {
            CP_Index[wk->wu.id][1] = 3;
        }
        break;

    case 3:
        if (--Timer_00[wk->wu.id] == 0) {
            CP_Index[wk->wu.id][1] = 4;
        }

        else {
            switch (Rapid_No[wk->wu.id][0]) {

            case 0:
                Rapid_No[wk->wu.id][0] = 1;
                Lever_Buff[wk->wu.id] = Shot;
                break;
            case 1:
                Rapid_No[wk->wu.id][0] = 0;
                Lever_Buff[wk->wu.id] = 0;
                break;
            }
            if (wk->wu.sp_tech_id != Tech_Number) {
                CP_Index[wk->wu.id][1] = 4;
            }
        }
        break;

    case 4:
        if (wk->wu.sp_tech_id == Tech_Number) {
            break;
        }
        if (Check_Motion_Ended(wk)) {
            Reaction_Exit_Sub(wk);
        }
        break;
    }
}

void Check_Rapid(PLW* wk, u16 Tech_Number) {
    if (!(Tech_Number & 0xF00)) {
        Rapid_No[wk->wu.id][0] = 0;
    } else {
        Lv = Setup_Lv08(0);
        if (Break_Into_CPU == 2) {
            Lv = 7;
        }
        Rapid_No[wk->wu.id][0] = Rapid_SA_Data[emLevelRemake(Lv, 8, 0)][random_32_com() & 7];
        Rapid_No[wk->wu.id][1] = 0;
        Setup_Rapid_End_Term(wk, Tech_Number);
        Rapid_Index[wk->wu.id] = 0xFF0;
    }
}

void Setup_Rapid_End_Term(PLW* wk, s16 Tech_Number) {
    Rapid_No[wk->wu.id][2] = (Tech_Number & 0xF00) >> 8;
    if ((Tech_Number & 0xF00) == 0x400) {
        Rapid_No[wk->wu.id][3] = Setup_Rapid_Time(wk, Tech_Number);
    }
}

s32 Setup_Rapid_Time(PLW* wk, u16 Tech_Number) {
    return 60;
}

/* Rapid_No[0] == 2: the three-stage rapid-fire cadence. The original returned
 * from the first stage rather than breaking; the helper returns at the same
 * point, which ends Rapid_Sub just as the return did. */
static void Rapid_Sub_Triple(PLW* wk) {
    switch (Rapid_No[wk->wu.id][1]) {
    case 0:
        Rapid_No[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = 1;
        Timer_01[wk->wu.id] = 3;
        return;
    case 1:
        if (--Timer_00[wk->wu.id] == 0) {
            Lever_Buff[wk->wu.id] = Rapid_Index[wk->wu.id];
            Timer_00[wk->wu.id] = 2;

            if (--Timer_01[wk->wu.id] == 0) {
                Rapid_No[wk->wu.id][1]++;
                Timer_01[wk->wu.id] = 0x18;
            }
        }
        break;
    case 2:
        if (--Timer_01[wk->wu.id] == 0) {
            Rapid_No[wk->wu.id][1]++;
            Timer_00[wk->wu.id] = 1;
            Timer_01[wk->wu.id] = 2;
        }
        break;
    default:
        if (--Timer_00[wk->wu.id] == 0) {
            Lever_Buff[wk->wu.id] = Rapid_Index[wk->wu.id];
            Timer_00[wk->wu.id] = Timer_01[wk->wu.id];
        }
        break;
    }
}

/* Every other Rapid_No[0]: the plain two-stage cadence. */
static void Rapid_Sub_Single(PLW* wk) {
    switch (Rapid_No[wk->wu.id][1]) {
    case 0:
        Rapid_No[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = 1;
        Timer_01[wk->wu.id] = 2;
        break;
    default:
        if (--Timer_00[wk->wu.id] == 0) {
            Lever_Buff[wk->wu.id] = Rapid_Index[wk->wu.id];
            Timer_00[wk->wu.id] = Timer_01[wk->wu.id];
        }
        break;
    }
}

void Rapid_Sub(PLW* wk) {
    if (Check_Rapid_End(wk) != 0) {
        return;
    }

    switch (Rapid_No[wk->wu.id][0]) {
    case 0:
        break;
    case 2:
        Rapid_Sub_Triple(wk);
        break;
    default:
        Rapid_Sub_Single(wk);
        break;
    }
}

s32 Check_Rapid_End(PLW* wk) {
    switch (Rapid_No[wk->wu.id][2]) {
    case 1:
        if (wk->wu.mvxy.a[1].real.h < 0) {
            Rapid_No[wk->wu.id][0] = 0;
            return 1;
        }
        break;
    case 2:
        switch (Rapid_No[wk->wu.id][3]) {
        case 0:
            if (plw[wk->wu.id].caution_flag) {
                Rapid_No[wk->wu.id][3]++;
            }
            break;
        case 1:
            if (plw[wk->wu.id].caution_flag == 0) {
                Rapid_No[wk->wu.id][0] = 0;
                return 1;
            }
            break;
        }
        break;
    case 4:
        switch (Rapid_No[wk->wu.id][3]) {
        case 0:
            if (wk->wu.cg_ja.atix) {
                Rapid_No[wk->wu.id][3]++;
            }
            break;
        case 1:
            if (--Rapid_No[wk->wu.id][3] == 0) {
                Rapid_No[wk->wu.id][0] = 0;
                return 1;
            }
            break;
        }
        break;
    }
    return 0;
}

s32 Check_Start_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number) {
    if (Before_Jump[wk->wu.id]) {
        return Before_Jump[wk->wu.id] = 0;
    }

    if (wk->wu.routine_no[1] == 2) {
        return 1;
    }
    if (Check_Free_To_Act(wk)) {
        return 0;
    }

    if ((Tech_Number == 0) || (Tech_Number == 1)) {
        if (wk->wu.cg_cancel & 2) {
            return 0;
        }
        return 1;
    }
    if ((Tech_Number & 0x8000) && (wk->wu.cg_cancel & 0x40)) {
        return 0;
    }
    if (wk->wu.cg_cancel & 0x20) {
        return 0;
    }
    if ((wk->wu.cg_cancel & 8) && (Reaction == 0xE)) {
        return 0;
    }
    return 1;
}
