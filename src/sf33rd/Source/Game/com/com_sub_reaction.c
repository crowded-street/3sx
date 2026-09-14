/**
 * @file com_sub_reaction.c
 * CPU Controlled Character reactions, pattern selection and flip handling.
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

static s32 Check_Reaction_Locked(PLW* wk) {
    return plw[wk->wu.id].caution_flag || plw[wk->wu.id].tsukami_f;
}

/* Non-zero when the stocked hit flag still carries a queued follow-up above
 * its low two bits. */
static s32 Check_Stock_Hit_Follow(PLW* wk) {
    return (Stock_Hit_Flag[wk->wu.id] >> 2) != 0;
}

/* Rewinds the active pattern to the stored return point. */
static void Restore_Return_Pattern(PLW* wk) {
    CP_No[wk->wu.id][0] = Return_CP_No[wk->wu.id];
    CP_Index[wk->wu.id][0] = Return_CP_Index[wk->wu.id];
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;
    Pattern_Index[wk->wu.id] = Return_Pattern_Index[wk->wu.id];
}

/* Reaction codes 0-7: queue the follow-up named by the low 12 bits. */
static void Reaction_Follow_Sub(PLW* wk, s16 Reaction) {
    if (Check_Stock_Hit_Follow(wk)) {
        Setup_Follow(wk, Reaction & 0xFFF);
        return;
    }

    if (Stock_Hit_Flag[wk->wu.id]) {
        Reaction_Exit_Sub(wk);
    } else if (Check_Free_To_Act(wk)) {
        Setup_Follow(wk, Reaction & 0xFFF);
    }
}

/* Reaction code 13: the meoshi path, which may answer with its own attack and
 * otherwise rewinds to the stored return pattern. */
static void Reaction_Meoshi_Sub(PLW* wk, s16 Reaction, s16 Power_Level) {
    if (Check_Stock_Hit_Follow(wk)) {
        Next_End(wk);
        return;
    }

    if (wk->permited_koa & 0x10) {
        if (Check_Meoshi_Attack(wk, Reaction, Power_Level) != 0) {
            return;
        }
    }

    Last_Eftype[wk->wu.id] = -1;

    if (!Check_Free_To_Act(wk)) {
        return;
    }

    if (!Stock_Hit_Flag[wk->wu.id]) {
        Next_End(wk);
        return;
    }

    if ((CP_No[wk->wu.id][0] == 6) && (Pattern_Index[wk->wu.id] == 0)) {
        Restore_Return_Pattern(wk);
    } else {
        Reaction_Exit_Sub(wk);
    }
}

void Reaction_Sub(PLW* wk, s16 Reaction, s16 Power_Level) {
    switch (Reaction & 0x7F) {
    case 9:
        if (Stock_Hit_Flag[wk->wu.id]) {
            Reaction_Exit_Sub(wk);
        } else if (Check_Free_To_Act(wk)) {
            Next_End(wk);
        }
        break;

    case 10:
        if (Check_Stock_Hit_Follow(wk)) {
            Next_End(wk);
            break;
        }

        if (Stock_Hit_Flag[wk->wu.id] || Check_Free_To_Act(wk)) {
            Reaction_Exit_Sub(wk);
        }
        break;

    case 11:
        if (Check_Reaction_Locked(wk)) {
            break;
        }

        if (Check_Stock_Hit_Follow(wk)) {
            Next_End(wk);
            break;
        }

        if (Stock_Hit_Flag[wk->wu.id]) {
            Reaction_Exit_Sub(wk);
        } else if (Check_Free_To_Act(wk)) {
            Next_End(wk);
        }
        break;

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        Reaction_Follow_Sub(wk, Reaction);
        break;

    case 12:
    case 14:
        Reaction_Exit_Sub(wk);
        Counter_Attack[wk->wu.id] = 1;
        break;

    case 13:
        Reaction_Meoshi_Sub(wk, Reaction, Power_Level);
        break;

    default:
        if (Check_Reaction_Locked(wk)) {
            break;
        }

        if (Stock_Hit_Flag[wk->wu.id] || Check_Free_To_Act(wk)) {
            Reaction_Exit_Sub(wk);
        }
        break;
    }
}

s32 Check_Meoshi_Attack(PLW* wk, s16 Reaction, s16 Power_Level) {
    if (wk->wu.cg_meoshi == Last_Eftype[wk->wu.id]) {
        return 0;
    }

    if (wk->permited_koa & 0x10) {
        Last_Eftype[wk->wu.id] = wk->wu.cg_meoshi;

        M_Lv[wk->wu.id] = Get_Meoshi_Data(wk);

        if (Pattern_Index[wk->wu.id] != 0) {
            Return_CP_No[wk->wu.id] = CP_No[wk->wu.id][0];
            Return_CP_Index[wk->wu.id] = CP_Index[wk->wu.id][0] + 1;
            CP_Index[wk->wu.id][1] = 0;
            CP_Index[wk->wu.id][2] = 0;
            CP_Index[wk->wu.id][3] = 0;
            Return_Pattern_Index[wk->wu.id] = Pattern_Index[wk->wu.id];
        }
        CP_No[wk->wu.id][0] = 6;
        CP_Index[wk->wu.id][0] = 0;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
        Pattern_Index[wk->wu.id] = 0;
        return 1;
    }
    return 0;
}

s32 Get_Meoshi_Data(PLW* wk) {
    u16 lever;
    u16 shot;

    lever = get_meoshi_lever(wk->wu.cg_meoshi);
    shot = get_meoshi_shot(wk->wu.cg_meoshi);

    if (wk->wu.rl_flag) {
        lever ^= 0xC;
    }

    return shot | lever;
}

void Reaction_Exit_Sub(PLW* wk) {
    CP_Index[wk->wu.id][0]++;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;

    Flip_Flag[wk->wu.id] = 0;
    Counter_Attack[wk->wu.id] = 0;
    Limited_Flag[wk->wu.id] = 0;
    Before_Jump[wk->wu.id] = 0;
    if (CP_No[wk->wu.id][0] != 6) {
        Passive_Flag[wk->wu.id] = 0;
    }
}

void Check_First_Menu(PLW* wk) {
    if (CP_Index[wk->wu.id][0] == 0) {
        Combo_Speed[wk->wu.id] = 1;
    } else {
        Combo_Speed[wk->wu.id] = Select_Combo_Speed(wk);
    }
}

void Select_Active(PLW* wk) {
    s16 pl_id;

    Lv = Setup_Lv08(0);
    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Lv = emLevelRemake(Lv, 8, 0);

    Rnd = (u8)random_32_ex_com();

    if (Check_SA_Active(wk, &pl_id) != 0) {
        Lv = Setup_Lv04(0);
        if (Break_Into_CPU == 2) {
            Lv = 3;
        }
        if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
            Lv = 2;
        }

        Lv = emLevelRemake(Lv, 4, 0);

        switch (Area_Number[wk->wu.id]) {
        case 0:
            Pattern_Index[wk->wu.id] = SA_Active_A_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        case 1:
            Pattern_Index[wk->wu.id] = SA_Active_B_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        case 2:
            Pattern_Index[wk->wu.id] = SA_Active_C_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        default:
            Pattern_Index[wk->wu.id] = SA_Active_D_Unit_Data[pl_id - 1][Lv][Rnd];
            break;
        }
    } else {
        switch (Area_Number[wk->wu.id]) {
        case 0:
            Pattern_Index[wk->wu.id] = Active_A_Unit_Data[wk->player_number][Lv][Rnd];
            break;

        case 1:
            Pattern_Index[wk->wu.id] = Active_B_Unit_Data[wk->player_number][Lv][Rnd];
            break;

        case 2:
            Pattern_Index[wk->wu.id] = Active_C_Unit_Data[wk->player_number][Lv][Rnd];
            break;

        default:
            Pattern_Index[wk->wu.id] = Active_D_Unit_Data[wk->player_number][Lv][Rnd];
            break;
        }
    }

#if DEBUG
    if (debug_config.cpu_active_override) {
        Pattern_Index[wk->wu.id] = debug_config.cpu_active_override - 1;
    }
#endif
}

s32 Check_SA_Active(PLW* wk, s16* pl_id) {
    if (wk->sa->ok != -1) {
        return 0;
    }
    if (My_char[wk->wu.id] == 9) {
        if (plw[wk->wu.id].sa->kind_of_arts == 0) {
            return *pl_id = 3;
        }
        return *pl_id = 2;
    }
    if ((My_char[wk->wu.id] == 3) && (plw[wk->wu.id].sa->kind_of_arts == 2)) {
        return *pl_id = 1;
    }
    if ((My_char[wk->wu.id] == 0xA) && (plw[wk->wu.id].sa->kind_of_arts == 2)) {
        return *pl_id = 1;
    }
    if ((My_char[wk->wu.id] == 0x11) && (plw[wk->wu.id].sa->kind_of_arts == 2)) {
        return *pl_id = 4;
    }
    return 0;
}

void Setup_Follow(PLW* wk, s16 Follow_Type) {
    CP_No[wk->wu.id][0] = 3;
    CP_No[wk->wu.id][1] = Follow_Type;

    if (wk->wu.hf.hit.player == 0) {
        CP_No[wk->wu.id][2] = 0;
    } else {
        CP_No[wk->wu.id][2] = 1;
    }

    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = Select_Reflection_Time(wk);
    Timer_00[wk->wu.id]++;
}

typedef const _anon6* const_anon6_p;

static const_anon6_p Follow_Menu_1st_Unit_Data[13] = {
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM02_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data, &COM00_Flollow_1st_Unit_Data,
    &COM00_Flollow_1st_Unit_Data,
};

typedef const _anon13* const_anon13_p;

static const_anon13_p Follow_Menu_2nd_Unit_Data[13] = {
    &Com00_Follow_Menu, &Com00_Follow_Menu, &Com02_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu,
    &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu,
    &Com00_Follow_Menu, &Com00_Follow_Menu, &Com00_Follow_Menu,
};

void Decide_Follow_Menu(PLW* wk) {
    s8 xx;
    const _anon6* Menu_Add_Ptr0;
    const _anon13* Menu_Add_Ptr1;

    Menu_Add_Ptr0 = Follow_Menu_1st_Unit_Data[wk->player_number];
    Rnd = (u8)random_32_com();
    xx = Menu_Add_Ptr0->xxxx[CP_No[wk->wu.id][1]][CP_No[wk->wu.id][2]][Rnd];

    Menu_Add_Ptr1 = Follow_Menu_2nd_Unit_Data[wk->player_number];
    Pattern_Index[wk->wu.id] = Menu_Add_Ptr1->zzzz[xx][Area_Number[wk->wu.id]];
}

s32 Select_Passive(PLW* wk) {
    u16 xx;

    if (VS_Tech[wk->wu.id] == 0xB) {
        Area_Number[wk->wu.id] = Ck_Area_Shell(wk);
    }
    if (VS_Tech[wk->wu.id] == 0x1E) {
        Area_Number[wk->wu.id] = Ck_Area_Shell(wk);
    }

    Last_Attack_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
    Standing_Timer[wk->wu.id] = 0;
    Devide_Level(VS_Tech[wk->wu.id] >> 0xC);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 1;
    }
    VS_Tech[wk->wu.id] &= 0xFFF;

    Setup_Random(wk);

    switch (Area_Number[wk->wu.id]) {
    case 0:
        xx = Passive_A_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][0] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][0] = 0;
        break;

    case 1:
        xx = Passive_B_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][1] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][1] = 0;
        break;

    case 2:
        xx = Passive_C_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][2] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][2] = 0;
        break;

    default:
        xx = Passive_D_Unit_Data_04[wk->player_number][VS_Tech[wk->wu.id]][Lv][Rnd];

        if (xx == 0xFF) {
            Counter_Attack[wk->wu.id] = 0;
            Passive_Flag[wk->wu.id] = 0;
            Jump_Pass_Timer[wk->wu.id][3] = 0x78;

            return -1;
        }
        Pattern_Index[wk->wu.id] = xx;
        Jump_Pass_Timer[wk->wu.id][3] = 0;
        break;
    }

    Passive_Flag[wk->wu.id] = 1;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = Select_Reflection_Time(wk);

#if DEBUG
    if (debug_config.cpu_passive_override) {
        Pattern_Index[wk->wu.id] = debug_config.cpu_passive_override - 1;
    }
#endif

    if ((VS_Tech[wk->wu.id] == 0x19) || (VS_Tech[wk->wu.id] == 0x13) || (Timer_00[wk->wu.id] == 0)) {
        CP_No[wk->wu.id][0] = 6;
        CP_Index[wk->wu.id][0] = 0;
        CP_Index[wk->wu.id][1] = 0;
        CP_Index[wk->wu.id][2] = 0;
        CP_Index[wk->wu.id][3] = 0;
    } else {
        CP_No[wk->wu.id][0] = 5;
    }
    return 1;
}

void Devide_Level(s16 xx) {
    switch (xx) {
    case 0:
        Lv = Setup_Lv04(0);
        if (Break_Into_CPU == 2) {
            Lv = 3;
        }
        break;
    default:
        Lv = Setup_Lv08(2);
        if (Break_Into_CPU == 2) {
            Lv = 7;
        }
        break;
    }
}

void Setup_Random(PLW* wk) {
    if (VS_Tech[wk->wu.id] == 0x20) {
        Rnd = (u8)random_16_com() & 7;
        VS_Tech[wk->wu.id] = 0x1C;
    } else {
        Rnd = (u8)random_16_com();
        Rnd = Check_Dramatic(wk, wk->wu.id);
    }
}

s32 Check_Dramatic(PLW* wk, s16 PL_id) {
    if (plw[wk->wu.id].sa->ok) {
        return Rnd | 8;
    }

    if ((plw[PL_id].wu.vital_new <= 0x30) || (plw[PL_id ^ 1].wu.vital_new <= 0x30)) {
        return Rnd | 8;
    }
    return Rnd;
}

const s8 PL_Status[0xA] = { 1, 0, 0, 0, 1, 1, 0, 0, 0, 0 };

s32 Check_Passive(PLW* wk) {
    WORK* em;

    if ((Counter_Attack[wk->wu.id] != 0) || (Pierce_Menu[wk->wu.id] != 0)) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;

    if (Check_Blow_Off(wk, em, 0) != 0) {
        *CP_No[wk->wu.id] = 0xE;
        CP_No[wk->wu.id][1] = 0;
        CP_No[wk->wu.id][2] = 0;
        CP_No[wk->wu.id][3] = 0;
        return -1;
    }
    if (Check_Thrown(wk, em) != 0) {
        if (Select_Passive(wk) != -1) {
            return 1;
        }
    }

    if (Check_Shell(wk) != 0) {
        return 1;
    }
    if ((Passive_Flag[wk->wu.id]) || (Flip_Flag[wk->wu.id])) {
        return Check_Guard(wk);
    }
    if (Check_Lie(wk) == 1) {
        return 1;
    }

    if (PL_Status[em->routine_no[1]] == 0) {
        return Check_Shell(wk);
    }

    Passive_Mode = 4;

    if (Ck_Passive_Term(wk) != 0) {
        if (Select_Passive(wk) != -1) {
            return 1;
        }
    }

    if (Check_Guard(wk) != 0) {
        return 1;
    }
    if ((Passive_Flag[wk->wu.id]) || (Flip_Flag[wk->wu.id])) {
        return 0;
    }

    Passive_Mode = 0;

    if (Ck_Passive_Term(wk) != 0) {
        return Select_Passive(wk);
    }

    return 0;
}

s32 Check_Guard(PLW* wk) {
    WORK* em;
    s16 xx;
    s16 zz;

    em = (WORK*)wk->wu.target_adrs;

    if (Attack_Flag[wk->wu.id] == 0) {
        return 0;
    }

    if (Guard_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return 0;
    }

    xx = Hit_Range_Data[em->hit_range];
    xx += Com_Width_Data[wk->wu.id];

    if (PL_Distance[wk->wu.id] > xx) {
        return 0;
    }

    Lv = Setup_Lv10(0);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }
    Lv += CC_Value[0];
    if (Break_Into_CPU == 2) {
        Lv = 0xA;
    }

    Rnd = random_16_com();

    zz = Setup_EM_Rank_Index(wk);

    Lv = emLevelRemake(Lv, 0xB, 1);

    if (Guard_Data[zz][Lv][Rnd] == 3) {
        Guard_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];
        return 0;
    }

    if (Check_Flip_Term(wk, NULL) != 0) {
        Next_Be_Flip(wk, 0);
    } else {
        Next_Be_Guard(wk, em, Guard_Data[zz][Lv][random_16_ex_com()]);
    }

    return 1;
}

s32 Check_Makoto(PLW* wk) {
    if (wk->player_number != 0x10) {
        return 0;
    }
    if (wk->sa->ok != -1) {
        return 0;
    }
    if (plw[wk->wu.id].sa->kind_of_arts == 2) {
        return 1;
    }

    return 0;
}

s32 Check_Flip_Term(PLW* wk, WORK* tmw) {
    WORK* em;
    s16 xx;

    if (tmw != NULL) {
        em = tmw;
    } else {
        em = (WORK*)wk->wu.target_adrs;
    }

    if (Check_Flip_Tech(em) == 0) {
        return 0;
    }

    Lv = Setup_Lv08(0);

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_32_com();
    Rnd -= Flip_Term_Correct(wk);

    xx = Setup_EM_Rank_Index(wk);

    if (Rnd >= (Flip_Data[xx][emLevelRemake(Lv, 8, 0)])) {
        return 0;
    }

    return 1;
}

s32 Setup_EM_Rank_Index(PLW* wk) {
    if (EM_Rank != 0) {
        return 0x11;
    }

    return wk->player_number;
}

s32 Flip_Term_Correct(PLW* wk) {
    s16 xx = 0;

    if (plw[wk->wu.id].wu.vital_new < 0x31) {
        xx += 1;
    }
    if ((PL_Wins[wk->wu.id]) < (PL_Wins[wk->wu.id ^ 1])) {
        xx += 2;
    }
    if (Counter_hi < 0xF) {
        xx += 1;
    }
    if (Check_Makoto(wk) != 0) {
        xx += 20;
    }

    return xx;
}

void Next_Be_Guard(PLW* wk, WORK* em, s16 Type_Of_Guard) {
    CP_No[wk->wu.id][0] = 7;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = 10;

    dash_flag_clear(wk->wu.id);
    Guard_Type[wk->wu.id] = Type_Of_Guard;
    Check_Guard_Type(wk, em);
}

s32 Check_Flip_Tech(WORK* em) {
    s32 rnum = 1;

    switch (save_w[Present_Mode].Difficulty) {
    case 0:
        rnum = 0;
        break;
    default:
        if (em->kind_of_waza & 0xF8) {
            rnum = 0;
        }
        /* fallthrough */
    case 6:
        if (em->kind_of_waza == 0) {
            rnum = 0;
        }
        if (em->kind_of_waza == 1) {
            rnum = 0;
        }
        break;
    case 7:
        break;
    }

    return rnum;
}

void Next_Be_Flip(PLW* wk, s16 xx) {
    WORK* em;

    em = (WORK*)wk->wu.target_adrs;

    CP_No[wk->wu.id][0] = 0xC;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = 9;

    Flip_Counter[wk->wu.id] = 0;

    if (!xx) {
        Check_Flip_GO(wk, 0);
        return;
    }

    if (xx == 8) {
        SetShellFlipLever(wk);
    } else if ((em->pat_status == 0x21) || (em->pat_status == 0x20)) {
        Lever_Buff[wk->wu.id] = 2;
    } else {
        Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 0);
    }

    CP_No[wk->wu.id][2] = 1;

    Timer_01[wk->wu.id] = xx;
}

