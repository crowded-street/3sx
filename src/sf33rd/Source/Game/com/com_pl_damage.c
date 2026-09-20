/**
 * @file com_pl_damage.c
 * CPU Controlled Character
 * Taking damage: the eight damage states and the exit.
 */

#include "sf33rd/Source/Game/com/com_pl.h"
#include "common.h"
#include "sf33rd/Source/Game/com/active/active00.h"
#include "sf33rd/Source/Game/com/active/active01.h"
#include "sf33rd/Source/Game/com/active/active02.h"
#include "sf33rd/Source/Game/com/active/active03.h"
#include "sf33rd/Source/Game/com/active/active04.h"
#include "sf33rd/Source/Game/com/active/active05.h"
#include "sf33rd/Source/Game/com/active/active06.h"
#include "sf33rd/Source/Game/com/active/active07.h"
#include "sf33rd/Source/Game/com/active/active08.h"
#include "sf33rd/Source/Game/com/active/active09.h"
#include "sf33rd/Source/Game/com/active/active10.h"
#include "sf33rd/Source/Game/com/active/active11.h"
#include "sf33rd/Source/Game/com/active/active12.h"
#include "sf33rd/Source/Game/com/active/active13.h"
#include "sf33rd/Source/Game/com/active/active14.h"
#include "sf33rd/Source/Game/com/active/active15.h"
#include "sf33rd/Source/Game/com/active/active16.h"
#include "sf33rd/Source/Game/com/active/active17.h"
#include "sf33rd/Source/Game/com/active/active18.h"
#include "sf33rd/Source/Game/com/active/active19.h"
#include "sf33rd/Source/Game/com/ck_pass.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/com/follow/follow02.h"
#include "sf33rd/Source/Game/com/passive/pass00.h"
#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass02.h"
#include "sf33rd/Source/Game/com/passive/pass03.h"
#include "sf33rd/Source/Game/com/passive/pass04.h"
#include "sf33rd/Source/Game/com/passive/pass05.h"
#include "sf33rd/Source/Game/com/passive/pass06.h"
#include "sf33rd/Source/Game/com/passive/pass07.h"
#include "sf33rd/Source/Game/com/passive/pass08.h"
#include "sf33rd/Source/Game/com/passive/pass09.h"
#include "sf33rd/Source/Game/com/passive/pass10.h"
#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass12.h"
#include "sf33rd/Source/Game/com/passive/pass13.h"
#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass15.h"
#include "sf33rd/Source/Game/com/passive/pass16.h"
#include "sf33rd/Source/Game/com/passive/pass17.h"
#include "sf33rd/Source/Game/com/passive/pass18.h"
#include "sf33rd/Source/Game/com/passive/pass19.h"
#include "sf33rd/Source/Game/com/shell/shell00.h"
#include "sf33rd/Source/Game/com/shell/shell01.h"
#include "sf33rd/Source/Game/com/shell/shell03.h"
#include "sf33rd/Source/Game/com/shell/shell04.h"
#include "sf33rd/Source/Game/com/shell/shell05.h"
#include "sf33rd/Source/Game/com/shell/shell07.h"
#include "sf33rd/Source/Game/com/shell/shell11.h"
#include "sf33rd/Source/Game/com/shell/shell12.h"
#include "sf33rd/Source/Game/com/shell/shell13.h"
#include "sf33rd/Source/Game/com/shell/shell14.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/getup.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/com_pl_internal.h"


/* The level a forced break-in and the weaker side override. Four of the damage
 * states picked a level and then adjusted it these two ways; the adjustment is
 * the same block every time apart from the level a break-in forces. */
static u8 level_after_demo_overrides(PLW* wk, u8 Lv, u8 forced) {
    if (Break_Into_CPU == 2) {
        Lv = forced;
    }

    if (Demo_Flag == 0 && Weak_PL == wk->wu.id) {
        Lv = 0;
    }

    return Lv;
}

static void Damage_1st_Receive_Select(PLW* wk) {
    u8 Lv;
    u8 Rnd;
    u8 xx;

    if (wk->py->flag) {
        CP_No[wk->wu.id][1] = 9;
        return;
    }

    if (PL_Blow_Off_Data[wk->wu.routine_no[2]] == 0) {
        CP_No[wk->wu.id][1] = 1;
        return;
    }

    CP_No[wk->wu.id][2]++;
    Lv = Setup_Lv08(0);

    Lv = level_after_demo_overrides(wk, Lv, 7);

    Rnd = random_32_com();
    xx = Setup_EM_Rank_Index(wk);

    if (Receive_Data[xx][emLevelRemake(Lv, 8, 0)] > Rnd) {
        Receive_Flag[wk->wu.id] = 1;
        return;
    }
}

static void Damage_1st_Get_Up_Select(PLW* wk) {
    u8 Lv;
    u8 Rnd;
    WORK* em;

    if (wk->wu.routine_no[3] == 0) {
        CP_No[wk->wu.id][2] = 0;
        return;
    }

    Lv = Setup_Lv04(0);

    Lv = level_after_demo_overrides(wk, Lv, 3);

    Rnd = random_32_com();
    CP_No[wk->wu.id][1] = Get_Up_Data[wk->player_number][emLevelRemake(Lv, 4, 0)][Rnd] + 1;
    CP_No[wk->wu.id][2] = 0;

    if (Get_Up_Action_Check_Data[wk->player_number][CP_No[wk->wu.id][1] - 1][Area_Number[wk->wu.id]] == -1) {
        CP_No[wk->wu.id][1] = Get_Up_Action_Check_Data[wk->player_number][CP_No[wk->wu.id][1]][4];
    }

    if (CP_No[wk->wu.id][1] != 0) {
        return;
    }

    Lv = Setup_Lv10(0);

    Lv = level_after_demo_overrides(wk, Lv, 10);

    Rnd = random_16_com();
    Lv += CC_Value[0];
    Lv = emLevelRemake(Lv, 11, 1);
    em = (WORK*)wk->wu.target_adrs;

    if (EM_Rank != 0) {
        Guard_Type[wk->wu.id] = Guard_Data[17][Lv][Rnd];
    } else {
        Guard_Type[wk->wu.id] = Guard_Data[wk->player_number][Lv][Rnd];
    }

    Check_Guard_Type(wk, em);
}

void Damage_1st(PLW* wk) {
    Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 1);
    Lever_Buff[wk->wu.id] |= 2;

    switch (CP_No[wk->wu.id][2]) {
    case 0:
        Damage_1st_Receive_Select(wk);
        break;

    case 1:
        Damage_1st_Get_Up_Select(wk);
        break;
    }
}

void Damage_2nd(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    Check_Guard_Type(wk, em);

    if (wk->wu.routine_no[2] == 0x19) {
        CP_No[wk->wu.id][1] = 9;
        CP_No[wk->wu.id][2] = 0;
        return;
    }

    if (Receive_Flag[wk->wu.id] != 0 && plw[wk->wu.id].uot_cd_ok_flag != 0) {
        Lever_Buff[wk->wu.id] = 2;
    }

    if (wk->wu.routine_no[1] != 1) {
        Exit_Damage_Sub(wk);
    }
}

void Damage_3rd(PLW* /* unused */) {}

void Damage_4th(PLW* /* unused */) {}

static s32 Damage_Sub_State_Active(PLW* wk) {
    if (wk->wu.routine_no[3] == 0) {
        CP_No[wk->wu.id][1] = 0;
        CP_No[wk->wu.id][2] = 0;
        return 0;
    }

    return 1;
}

static s32 Damage_Recovery_Reached(PLW* wk) {
    if (wk->wu.routine_no[1] != 1) {
        Exit_Damage_Sub(wk);
        return 0;
    }

    return 1;
}

static void Damage_5th_Wait_Recovery(PLW* wk) {
    if (!Damage_Recovery_Reached(wk)) {
        return;
    }

    if (wk->wu.cg_type == 9) {
        CP_No[wk->wu.id][2]++;
        CP_Index[wk->wu.id][1] = 0;
    }
}

void Damage_5th(PLW* wk) {
    if (!Damage_Sub_State_Active(wk)) {
        return;
    }

    switch (CP_No[wk->wu.id][2]) {
    case 0:
        Damage_5th_Wait_Recovery(wk);
        break;

    case 1:
        if (Command_Attack_SP(wk, wk->player_number, 46, 8)) {
            CP_No[wk->wu.id][2]++;
        }

        break;

    default:
        if (wk->wu.routine_no[1] != 4 || wk->wu.cg_type == 64) {
            Exit_Damage_Sub(wk);
        }

        break;
    }
}

/* Choosing what to do on the way up: which get-up action the level and the roll
 * pick, and the super art substituted when the table says there is none. The
 * block is Damage_6th's own. */
static void pick_get_up_action(PLW* wk) {
    u8 Lv;
    u8 Rnd;

    if (Get_Up_Action_Check_Data[wk->player_number][CP_No[wk->wu.id][1] - 1][Area_Number[wk->wu.id]] == -1) {
        CP_No[wk->wu.id][1] = Get_Up_Action_Check_Data[wk->player_number][CP_No[wk->wu.id][1]][4];
    }

    CP_No[wk->wu.id][2]++;
    CP_Index[wk->wu.id][1] = 0;
    Lv = Setup_Lv04(0);

    Lv = level_after_demo_overrides(wk, Lv, 3);

    Lv = emLevelRemake(Lv, 4, 0);
    Rnd = random_32_com() & 3;
    Rnd *= 2;

    CP_Index[wk->wu.id][0] = Get_Up_Action_Tech_Data[wk->player_number][Lv][Rnd];
    CP_Index[wk->wu.id][7] = Get_Up_Action_Tech_Data[wk->player_number][Lv][Rnd + 1];

    if (CP_Index[wk->wu.id][0] == 0xFF) {
        CP_Index[wk->wu.id][0] = Get_Up_Action_Tech_Data[wk->player_number][Lv][0];
        CP_Index[wk->wu.id][7] = 8;

        if (plw[wk->wu.id].sa->ok &&
            Arts_Super_Name_Data[wk->player_number][plw[wk->wu.id].sa->kind_of_arts] != -1) {
            CP_Index[wk->wu.id][0] =
                Arts_Super_Name_Data[wk->player_number][plw[wk->wu.id].sa->kind_of_arts];
        }
    }
}

static void Damage_6th_Wait_Recovery(PLW* wk) {
    if (!Damage_Recovery_Reached(wk)) {
        return;
    }

    if (wk->wu.cg_type == 12) {
        pick_get_up_action(wk);
    }
}

void Damage_6th(PLW* wk) {

    if (!Damage_Sub_State_Active(wk)) {
        return;
    }

    if (wk->wu.routine_no[2] == 0x19) {
        CP_No[wk->wu.id][1] = 9;
        CP_No[wk->wu.id][2] = 0;
        return;
    }

    Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 1);
    Lever_Buff[wk->wu.id] |= 2;

    switch (CP_No[wk->wu.id][2]) {
    case 0:
        Damage_6th_Wait_Recovery(wk);
        break;

    case 1:
        if (Command_Attack_SP(wk, wk->player_number, CP_Index[wk->wu.id][0], CP_Index[wk->wu.id][7])) {
            CP_No[wk->wu.id][2]++;
        }

        break;

    default:
        if (Command_Attack_SP(wk, wk->player_number, CP_Index[wk->wu.id][0], CP_Index[wk->wu.id][7])) {
            Exit_Damage_Sub(wk);
        }

        break;
    }
}

static void Damage_7th_Set_Guard_Type(PLW* wk) {
    switch (CP_No[wk->wu.id][1]) {
    case 6:
        Guard_Type[wk->wu.id] = 0;
        break;

    case 7:
        Guard_Type[wk->wu.id] = 1;
        break;

    default:
        Guard_Type[wk->wu.id] = 2;
        break;
    }
}

static void Damage_7th_Check_Exit(PLW* wk) {
    WORK* em;

    em = (WORK*)wk->wu.target_adrs;
    Check_Guard_Type(wk, em);

    if (wk->wu.cg_type != 0x40 && wk->wu.routine_no[1] != 0) {
        return;
    }

    if (Attack_Flag[wk->wu.id] != 0) {
        return;
    }

    if (Attack_Flag[wk->wu.id] == 0) {
        Exit_Damage_Sub(wk);
        return;
    }

    if (wk->tsukamarenai_flag == 0) {
        Exit_Damage_Sub(wk);
    }
}

void Damage_7th(PLW* wk) {
    switch (CP_No[wk->wu.id][2]) {
    case 0:
        if (wk->wu.routine_no[1] != 1) {
            Exit_Damage_Sub(wk);
            break;
        }

        CP_No[wk->wu.id][2]++;
        Damage_7th_Set_Guard_Type(wk);
        break;

    default:
        Damage_7th_Check_Exit(wk);
        break;
    }
}

static void Damage_8th_Setup_Faint_Rapid(PLW* wk) {
    s16 Rnd;
    s16 Lv;

    CP_No[wk->wu.id][2] += 1;
    Timer_00[wk->wu.id] = 1;
    Lv = Setup_Lv08(0);

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }

    if (Demo_Flag == 0 && Weak_PL == wk->wu.id) {
        Lv = 0;
    }

    Timer_01[wk->wu.id] = Faint_Rapid_Data[emLevelRemake(Lv, 8, 0)][(Rnd = random_16_com() & 7)];
}

void Damage_8th(PLW* wk) {
    if (wk->wu.routine_no[1] != 1) {
        Exit_Damage_Sub(wk);
        return;
    }

    switch (CP_No[wk->wu.id][2]) {
    case 0:
        if (wk->wu.routine_no[2] == 0x19 && wk->wu.routine_no[3] != 0) {
            Damage_8th_Setup_Faint_Rapid(wk);
        }

        break;

    case 1:
        Lever_Buff[wk->wu.id] = Com_Rapid_Sub(wk, 0, &CP_No[wk->wu.id][3]);
        break;
    }
}

void Exit_Damage_Sub(PLW* wk) {
    Clear_Com_Flag(wk);

    if (Check_Passive(wk)) {
        return;
    }

    Next_Be_Free(wk);
}
