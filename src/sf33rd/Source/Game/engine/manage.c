/**
 * @file manage.c
 * Engine management
 */

#include "sf33rd/Source/Game/engine/manage.h"
#include "common.h"
#include "sf33rd/Source/Game/Pause.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/count.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/eff08.h"
#include "sf33rd/Source/Game/effect/eff14.h"
#include "sf33rd/Source/Game/effect/eff35.h"
#include "sf33rd/Source/Game/effect/eff56.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff81.h"
#include "sf33rd/Source/Game/effect/eff84.h"
#include "sf33rd/Source/Game/effect/eff92.h"
#include "sf33rd/Source/Game/effect/effb2.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effg0.h"
#include "sf33rd/Source/Game/effect/effj4.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

void Game_Manage_1st();
void Clear_1Stage_Work();
void Game_Manage_2nd();
void Game_Manage_2_0();
void Game_Manage_2_1();
void Game_Manage_2_2();
void Game_Manage_2_3();
void Game_Manage_2_4();
void Game_Manage_3rd();
void setFinishType();
void Game_Manage_4th();
void Game_Manage_5th();
void Game_Manage_5_0();
void Game_Manage_5_1();
void Game_Manage_5_2();
void Game_Manage_5_3();
void Game_Manage_5_4();
void Game_Manage_5_5();
void Game_Manage_5_6();
void Game_Manage_5_7();
void Game_Manage_6th();
void Game_Manage_7th();
void Game_Manage_7_0();
void Game_Manage_7_1();
void Game_Manage_7_2();
s32 Check_Disp_Combo();
void Game_Manage_7_3();
void Game_Manage_7_4();
void Game_Manage_7_5();
void Game_Manage_7_6();
void Game_Manage_7_7();
void Game_Manage_7_8();
void Game_Manage_7_9();
void Game_Manage_8th();
void Game_Manage_8_0();
void Game_Manage_8_1();
void Game_Manage_81_0();
void Game_Manage_81_1();
void Game_Manage_81_2();
void Game_Manage_81_3();
void Game_Manage_8_2();
void Game_Manage_8_3();
void Game_Manage_9th();
void Game_Manage_10th();
void Check_Naming(s16 PL_id);
s32 Check_Ending();
s32 Check_Ending_Sub();
void Additional_Bonus(s16 PL_id);
u32 Setup_Comp_Bonus();
void request_center_message(s16 Kind_of_Message);
void Setup_Win_Mark();
void Check_Perfect(s16 PL_id);
void Update_VS_Data();
void BGM_Fade_Sub();
void BGM_Control();
void Setup_BGM_Fade_In(s16 Time);
void Check_Stage_BGM();
void Control_Music_Fade(s16 Time);
void Check_Conclusion_Type();
void chkComWins();
void Update_BI_Term();
void Ck_Win_Record();
void Update_Level_Control();
s32 Judge_Next_Disposal();
void Quick_Entry();
s32 Check_Entry_Again();
void Loser_Sub();
void Be_Continue();
void Disp_Winner();
void Pool_Score(s16 PL_id);
s32 Check_Break_Into_CPU(s16 PL_id);
void Judge_Winner();
s32 Check_Disp_Winner();
void Check_Fade_Out_BGM(s16 Time);
s32 Check_BI_Grade(s16 PL_id);
void Game_Manage_11th();
void Game_Manage_12th();
void Game_Manage_12_0();
void Game_Manage_12_1();
void Game_Manage_12_2();
void Game_Manage_12_3();
void Game_Manage_12_4();
void Game_Manage_12_5();
void Game_Manage_12_7();
void Game_Manage_12_8();
u8 Check_Bonus_Perfect();
void Disp_Bonus_Perfect();
void Flash_Bonus_Perfect();
u32 Setup_Final_Score(s16 Type);
s32 Bonus_Cut_Sub();
s16 Check_Time_Over();
void complete_victory_pause();
void Game_Manage_13th();

u8 Disp_Bonus_Contents;
s8 MANAGE_X;

const u32 Comp_Bonus_Data[11] = { 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000, 110000, 120000, 130000 };

const u8 BIC_SA_Data[2][4] = { { 3, 5, 7, 9 }, { 1, 1, 1, 1 } };

const u32 Ball_Perfect_PTS[2][5] = { { 20000, 30000, 50000, 80000, 120000 }, { 10000, 20000, 40000, 80000, 160000 } };

s32 Game_Management() {
    void (*Management_Jmp_Tbl[13])() = { Game_Manage_1st, Game_Manage_2nd,  Game_Manage_3rd,  Game_Manage_4th,
                                         Game_Manage_5th, Game_Manage_6th,  Game_Manage_7th,  Game_Manage_8th,
                                         Game_Manage_9th, Game_Manage_10th, Game_Manage_11th, Game_Manage_12th,
                                         Game_Manage_13th };

    if (Break_Into) {
        return 0;
    }

    MANAGE_X = 0;
    Management_Jmp_Tbl[C_No[0]]();
    BGM_Fade_Sub();
    BGM_Control();
    return MANAGE_X;
}

void Game_Manage_1st() {
    Switch_Screen(0);
    EXE_obroll = 0;

    if (bg_w.stage == 21 || bg_w.stage == 20) {
        C_No[0] = 11;
    } else {
        C_No[0] = 1;
    }

    win_pause_go = 0;
    appear_work_clear();
    win_sp_flag = 0;
    BGM_No[1] = 0;
    BGM_No[0] = 0;
    Appear_Q = 0;
    Clear_1Stage_Work();
    All_Clear_Suicide();
    Round_Operator[0] = 0;
    Round_Operator[1] = 0;

    if (plw[0].wu.operator) {
        Round_Operator[0] = 1;
        Final_Play_Type[0] = Play_Type;
    }

    if (plw[1].wu.operator) {
        Round_Operator[1] = 1;
        Final_Play_Type[1] = Play_Type;
    }

    Battle_Q[0] = 0;
    Battle_Q[1] = 0;

    if (Play_Type == 0) {
        Control_Time = SC_Personal_Time[Player_id];
        paring_ctr_ori[Player_id] = paring_ctr_vs[0][Player_id] = 0;
        Stage_Stock_Score[Player_id] = Score[Player_id][0];
        Request_Disp_Rank[COM_id][0] = -1;
        Request_Disp_Rank[COM_id][1] = -1;
        Request_Disp_Rank[COM_id][2] = -1;
        Request_Disp_Rank[COM_id][3] = -1;

        if (EM_id == 17) {
            Break_Into_CPU = 2;
        } else {
            Break_Into_CPU = 0;
        }
    }

    eff_hit_flag_clear();
    Check_Stage_BGM();
    Pause_Family_On();
    Fade_Flag = 0;
    Clear_Flash_No();
    seraph_flag = 0;
    grade_check_work_stage_init(0);
    grade_check_work_stage_init(1);

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        cpReadyTask(TASK_MENU, Menu_Task);
        task[TASK_MENU].r_no[0] = 7;
        plw[New_Challenger].wu.operator = 0;
        Operator_Status[New_Challenger] = 0;
        Lever_LR[0] = 0;
        Lever_LR[1] = 0;
        return;
    }

    if (Mode_Type != MODE_NETWORK) {
        cpReadyTask(TASK_PAUSE, Pause_Task);
        setup_pos_remake_key(3);
    }
}

void Clear_1Stage_Work() {
    s16 xx;

    for (xx = 0; xx < 2; xx++) {
        Vital_Bonus[xx] = 0;
        Time_Bonus[xx] = 0;
        Perfect_Bonus[xx] = 0;
        Perfect_Counter[xx] = 0;
        Stage_SA_Finish[xx] = 0;
        Stage_Lost_Round[xx] = 0;
        Stage_Perfect_Finish[xx] = 0;
        Stage_Cheap_Finish[xx] = 0;
        Stage_Time_Finish[xx] = 0;
    }

    Disp_Cockpit = 0;
}

void Game_Manage_2nd() {
    void (*SC2_Jmp_Tbl[5])() = { Game_Manage_2_0, Game_Manage_2_1, Game_Manage_2_2, Game_Manage_2_3, Game_Manage_2_4 };

    SC2_Jmp_Tbl[C_No[1]]();
}

void Game_Manage_2_0() {
    Switch_Screen(0);
    request_message = 0;
    SA_shadow_on = 0;

    if (Demo_Flag == 0) {
        C_No[1] = 2;
        return;
    }

    if (--Cover_Timer > 0) {
        return;
    }

    if (Wait_Seek_Time() == 0) {
        Cover_Timer = 1;
        return;
    }

    C_No[1]++;
    Switch_Screen_Init(0);
}

s32 Wait_Seek_Time() {
    s16 ix;
    s16 ix2;

    switch (Play_Mode) {
    case 1:
        if (Mode_Type != MODE_NETWORK) {
            for (ix = 0; ix < 2; ix++) {
                for (ix2 = 0; ix2 < 3; ix2++) {
                    Separate_Area[ix][ix2] = 0;
                    Shell_Separate_Area[ix][ix2] = 0;
                }
            }

            return 1;
        }

        Lag_Ptr[0] = Lag_Timer;
        Lag_Ptr++;
        Lag_Timer = 1;
        return 1;

    case 3:
        if (Mode_Type == MODE_NORMAL_TRAINING) {
            return 1;
        }

        if (Mode_Type == MODE_PARRY_TRAINING) {
            return 1;
        }

        for (ix = 0; ix < 2; ix++) {
            for (ix2 = 0; ix2 < 3; ix2++) {
                Separate_Area[ix][ix2] = 0;
                Shell_Separate_Area[ix][ix2] = 0;
            }
        }

        if (--Lag_Timer == 0) {
            Lag_Timer = Lag_Ptr[0];
            Lag_Ptr++;
            return 1;
        }

        return 0;

    default:
        return 1;
    }
}

void Game_Manage_2_1() {
    switch (C_No[2]) {
    case 0:
        if (!Switch_Screen_Revival(0)) {
            break;
        }

        if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
            C_No[2]++;
            break;
        }

        C_No[1]++;

        break;

    case 1:
        if (task[TASK_MENU].r_no[0] == 10) {
            C_No[1]++;
            C_No[2] = 0;
        }

        break;
    }
}

void Game_Manage_2_2() {
    s16 ix;

    Suicide[0] = 0;
    Suicide[6] = 0;

    for (ix = 0; ix < 4; ix++) {
        Message_Suicide[ix] = 0;
    }

    if (effect_84_init()) {
        return;
    }

    C_No[1]++;
    Forbid_Break = 0;
    Extra_Break = 0;
    Complete_Victory = 0;
    Conclusion_Flag = 0;
    Perfect_Flag = 0;
    Round_Result = 0;
    Reserve_Cut = 0;
    Next_Step = 0;
    judge_flag = 0;
    Stop_Combo = 0;

    if (Demo_Flag) {
        Stop_SG = 0;
    }

    Complete_Judgement = 0;
    Music_Fade = 0;
    Pause_Hit_Marks = 0;
    count_end = 0;
    sag_inc_timer[0] = sag_inc_timer[1] = 0;
    CP_No[0][0] = 0;
    CP_No[1][0] = 0;
    Stock_Score[0] = Score[0][0];
    Stock_Score[1] = Score[1][0];
    grade_check_work_round_init(0);
    grade_check_work_round_init(1);
}

void Game_Manage_2_3() {
    if (Appear_end < 2) {
        return;
    }

    if (bg_app) {
        return;
    }

    appear_work_clear();
    win_sp_flag = 0;

    if (pcon_rno[0] != 0) {
        return;
    }

    if (pcon_rno[1] != 1) {
        return;
    }

    C_No[1]++;
    effect_B2_init();
}

void Game_Manage_2_4() {
    switch (C_No[2]) {
    case 0:
        if (Round_num) {
            C_No[2] = 3;
            return;
        }

        C_No[2]++;
        C_Timer = 3;
        Forbid_Break = 1;
        FadeInit();
        FadeOut(0, 0xFF, 8);
        Disp_Cockpit = 1;

        if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
            Score[0][2] = 0;
            Score[1][2] = 0;
            Game_pause = 0;
            pcon_rno[0] = 0;
            pcon_rno[1] = 0;
            pcon_rno[2] = 0;
            pcon_rno[3] = 0;
            appear_type = 0;
            erase_extra_plef_work();
            compel_bg_init_position();
            win_lose_work_clear();
        }

        break;

    case 1:
        FadeOut(0, 0xFF, 8);

        if (--C_Timer == 0) {
            C_No[2]++;
            Clear_Flash_No();
        }

        break;

    case 2:
        C_No[2]++;
        Forbid_Break = 0;
        break;

    case 3:
        if (Next_Step == 0) {
            break;
        }

        C_No[0]++;
        C_No[1] = 0;
        C_No[2] = 0;
        Allow_a_battle_f = 1;
        vital_inc_timer = 50;
        vital_dec_timer = 40;
        sag_inc_timer[0] = sag_inc_timer[1] = 0;

        if (Play_Type == 0 && (EM_id == 0 || (My_char[Player_id] == 0 && EM_id == 1)) &&
            !(Introduce_Boss[Player_id][1] & 0x80)) {
            Introduce_Boss[Player_id][1] |= 128;
            Check_Stage_BGM();
        }

        if (Demo_Flag == 0) {
            effect_58_init(10, 60, 0);
        }

        break;
    }
}

void Game_Manage_3rd() {
    if (Demo_Flag == 0) {
        return;
    }

    if (Conclusion_Flag == 0) {
        return;
    }

    C_No[0]++;
    Forbid_Break = -1;
    Allow_a_battle_f = 0;
    count_end = 1;
    Check_Conclusion_Type();
}

void setFinishType() {
    if (Play_Type == 0 && Mode_Type == MODE_ARCADE &&
        PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] && VS_Index[Winner_id] > 8 &&
        plw[Winner_id].wu.operator != 0 && E_Number[Loser_id][0] != 2) {
        E_Number[Loser_id][0] = 99;
    }

    Update_BI_Term();
}

void Game_Manage_4th() {
    switch (Conclusion_Type) {
    case 0:
        C_No[0] = 6;
        Setup_Win_Mark();
        Check_Perfect(Winner_id);
        setFinishType();
        PL_Wins[Winner_id]++;
        Update_Level_Control();
        Update_VS_Data();
        Ck_Win_Record();
        break;

    case 1:
        SsRequest(121);
        SsRequest(139);

        if (Judge_Next_Disposal()) {
            C_No[0] = 4;
            break;
        }

        C_No[0] = 5;
        Round_Result |= 1024;
        setFinishType();
        win_type[0][PL_Wins[0]] = 5;
        win_type[1][PL_Wins[1]] = 5;
        PL_Wins[0]++;
        PL_Wins[1]++;

        if (PL_Wins[0] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
            Winner_id = 0;
            Loser_id = 1;
            Update_VS_Data();
            Ck_Win_Record();
            break;
        }

        if (PL_Wins[1] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
            Winner_id = 1;
            Loser_id = 0;
            Update_VS_Data();
            Ck_Win_Record();
        }

        break;

    default:
        SsRequest(143);

        if (plw[0].wu.vital_new != plw[1].wu.vital_new || Mode_Type == MODE_NORMAL_TRAINING ||
            Mode_Type == MODE_PARRY_TRAINING) {
            C_No[0] = 6;
            Round_Result |= 1;
            setFinishType();
            win_type[Winner_id][PL_Wins[Winner_id]] = 1;
            Check_Perfect(Winner_id);
            PL_Wins[Winner_id]++;
            Update_Level_Control();
            Update_VS_Data();
            Ck_Win_Record();
            break;
        }

        C_No[0] = 4;
        break;
    }
}

void Game_Manage_5th() {
    void (*SC5_Jmp_Tbl[8])() = { Game_Manage_5_0, Game_Manage_5_1, Game_Manage_5_2, Game_Manage_5_3,
                                 Game_Manage_5_4, Game_Manage_5_5, Game_Manage_5_6, Game_Manage_5_7 };

    SC5_Jmp_Tbl[C_No[1]]();
}

void Game_Manage_5_0() {
    if (Complete_Victory) {
        C_No[1]++;
        C_Timer = 30;
        Event_Judge_Gals = 0;
    }
}

void Game_Manage_5_1() {
    if (Button_Cut_EX(&C_Timer, 10)) {
        C_No[1]++;
        request_center_message(3);
        SsRequest(154);
    }
}

void Game_Manage_5_2() {
    if (!request_message) {
        C_No[1]++;
        C_Timer = 30;
    }
}

void Game_Manage_5_3() {
    if (Button_Cut_EX(&C_Timer, 10)) {
        C_No[1]++;
        Judge_Winner();
        chkComWins();
        Stop_Combo = 1;
        Switch_Screen_Init(0);
        SsBgmFadeOut(2048);
    }
}

void Game_Manage_5_4() {
    if (Switch_Screen(0)) {
        C_No[1]++;
        Cover_Timer = 5;
        Suicide[6] = 1;
        judge_flag = 1;
        effect_J4_init(0xFF);
        compel_bg_init_position();
        pcon_rno[0] = 0;
        pcon_rno[1] = 0;
        pcon_rno[2] = 0;
        pcon_rno[3] = 0;
        appear_type = 3;
    }
}

void Game_Manage_5_5() {
    Switch_Screen(0);

    if (--Cover_Timer == 0) {
        C_No[1]++;
        pcon_rno[1] = 3;
        pcon_rno[2] = 1;
        Clear_Flash_No();
        Switch_Screen_Init(0);
    }
}

void Game_Manage_5_6() {
    if (Switch_Screen_Revival(0)) {
        C_No[1]++;
        C_Timer = 60;
        Stop_SG = 0;
        BGM_No[0] = 3;
        BGM_Timer[0] = 1;
    }
}

void Game_Manage_5_7() {
    if (--C_Timer != 0) {
        return;
    }

    if (Wait_Seek_Time() == 0) {
        C_Timer = 1;
        return;
    }

    C_No[0] = 6;
    C_No[1] = 7;
    C_Timer = 30;
    Fade_Half_Flag = 1;
    Complete_Judgement = 1;
    Round_Result |= 0x8000;
    win_type[Winner_id][PL_Wins[Winner_id]] = 6;
    setFinishType();
    Check_Perfect(Winner_id);
    PL_Wins[Winner_id]++;
    Update_Level_Control();
    Update_VS_Data();
}

void Game_Manage_6th() {
    switch (C_No[1]) {
    case 0:
        if (!Complete_Victory) {
            break;
        }

        C_No[1]++;
        C_Timer = 60;
        pcon_rno[1] = 3;
        pcon_rno[2] = 0;
        grade_makeup_round_para_dko();

        if (Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING && omop_cockpit) {
            effect_58_init(6, 1, Winner_id + 100);
            effect_92_init(0, PL_Wins[0] - 1);
            effect_92_init(1, PL_Wins[1] - 1);
            break;
        }

        break;

    case 1:
        if (--C_Timer != 0) {
            break;
        }

        if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
            C_No[0] = 12;
            End_Training = 1;
            break;
        }

        C_No[0] = 7;
        C_No[1] = 0;
        Round_num++;
        Quick_Entry();
        break;
    }
}

void Game_Manage_7th() {
    void (*SC7_Jmp_Tbl[10])() = { Game_Manage_7_0, Game_Manage_7_1, Game_Manage_7_2, Game_Manage_7_3, Game_Manage_7_4,
                                  Game_Manage_7_5, Game_Manage_7_6, Game_Manage_7_7, Game_Manage_7_8, Game_Manage_7_9 };

    SC7_Jmp_Tbl[C_No[1]]();
}

void Game_Manage_7_0() {
    if (!Complete_Victory) {
        return;
    }

    C_No[1]++;
    C_Timer = 1;
    grade_makeup_round_parameter(Winner_id);

    if (Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING && omop_cockpit) {
        effect_58_init(6, 1, Winner_id + 100);
        effect_92_init(Winner_id, PL_Wins[Winner_id] - 1);
    }
}

void Game_Manage_7_1() {
    if (--C_Timer == 0) {
        C_No[1]++;
        C_Timer = 10;
    }
}

void Game_Manage_7_2() {
    if (!Button_Cut_EX(&C_Timer, 0x7FFF)) {
        return;
    }

    if (Check_Disp_Combo()) {
        C_Timer = 1;
        return;
    }

    C_No[1]++;

    if (Check_Disp_Winner() == 0) {
        C_Timer = 50;
    } else {
        Disp_Winner();
        C_Timer = 90;
    }

    if (Round_Operator[Winner_id] == 0 && Perfect_Flag == 0) {
        Check_Fade_Out_BGM(182);
    }
}

s32 Check_Disp_Combo() {
    if (cmb_all_stock[0] != 0 || cmb_calc_now[0] != 0 || cmb_calc_now[1] != 0) {
        return 1;
    }

    if (PL_Wins[Winner_id] < save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        return 0;
    }

    return 0;
}

void Game_Manage_7_3() {
    if (Play_Type == 0 && Perfect_Flag == 0) {
        if (--C_Timer) {
            return;
        }
    } else {
        if (--C_Timer) {
            return;
        }
    }

    Message_Suicide[1] = 1;

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        C_No[0] = 12;
        End_Training = 1;
        return;
    }

    if (Perfect_Flag) {
        C_No[1]++;
        C_Timer = 10;
        return;
    }

    C_No[0]++;
    C_No[1] = 0;
    Event_Judge_Gals = -1;
}

void Game_Manage_7_4() {
    if (--C_Timer == 0) {
        C_No[1]++;
        request_center_message(4);
        effect_58_init(6, 1, 155);
        effect_58_init(6, 60, 156);
    }
}

void Game_Manage_7_5() {
    if (!request_message) {
        C_No[1]++;
        C_Timer = 6;
        Event_Judge_Gals = -1;
    }
}

void Game_Manage_7_6() {
    if (Scene_Cut) {
        C_Timer = 1;
    }

    if (--C_Timer == 0) {
        C_No[0]++;
        C_No[1] = 0;
    }
}

void Game_Manage_7_7() {
    if (--C_Timer == 0) {
        C_No[1]++;
        Event_Judge_Gals = 3;
    }
}

void Game_Manage_7_8() {
    if (Event_Judge_Gals == 0) {
        C_No[1]++;
        C_Timer = 30;
        Ck_Win_Record();
    }
}

void Game_Manage_7_9() {
    if (--C_Timer == 0) {
        C_No[1] = 0;
    }
}

void Game_Manage_8th() {
    void (*SC8_Jmp_Tbl[4])() = { Game_Manage_8_0, Game_Manage_8_1, Game_Manage_8_2, Game_Manage_8_3 };

    SC8_Jmp_Tbl[C_No[1]]();
}

void Game_Manage_8_0() {
    Round_num++;
    Quick_Entry();
    Stop_Update_Score = 1;

    if (Round_Operator[Winner_id] != 0 || Mode_Type == MODE_VERSUS || Mode_Type == 5) {
        Pool_Score(Winner_id);

        if (PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
            C_No[1]++;
            Additional_Bonus(WINNER);
            grade_makeup_stage_parameter(WINNER);
            grade_makeup_stage_parameter(LOSER);
            Check_Break_Into_CPU(WINNER);
            return;
        }

        C_No[1] = 3;
        C_Timer = 1;
        return;
    }

    C_No[1] = 3;
    C_Timer = 30;

    if (PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        grade_makeup_stage_parameter(WINNER);
        grade_makeup_stage_parameter(LOSER);
    }
}

void Game_Manage_8_1() {
    void (*SC81_Jmp_Tbl[4])() = { Game_Manage_81_0, Game_Manage_81_1, Game_Manage_81_2, Game_Manage_81_3 };

    SC81_Jmp_Tbl[C_No[2]]();
}

void Game_Manage_81_0() {
    s16 time;
    s16 pos_id;
    s16 pos_id2;

    Check_Fade_Out_BGM(546);
    C_No[2]++;
    C_Timer = 20;
    Forbid_Break = -1;
    pos_id = 0;
    pos_id2 = 0;
    time = 1;
    Order[74] = 1;
    Order_Timer[74] = time;
    Order_Dir[74] = pos_id++;
    effect_76_init(74);
    time += 5;

    if (Perfect_Flag) {
        Order[76] = 1;
        Order_Timer[76] = time;
        Order_Dir[76] = pos_id++;
        effect_76_init(76);
        Order[81] = 0;
        effect_G0_init(81, time, Perfect_Bonus[Winner_id], pos_id2++);
        time += 5;
    }

    Order[78] = 1;
    Order_Timer[78] = time;
    Order_Dir[78] = pos_id++;
    effect_76_init(78);
    Order[83] = 0;
    effect_G0_init(83, time, Vital_Bonus[Winner_id], pos_id2++);
    time += 5;
    Order[79] = 1;
    Order_Timer[79] = time;
    Order_Dir[79] = pos_id++;
    effect_76_init(79);
    Order[84] = 0;
    effect_G0_init(84, time, Time_Bonus[Winner_id], pos_id2++);
    time += 5;
    Order[75] = 1;
    Order_Timer[75] = time;
    Order_Dir[75] = pos_id++;
    effect_76_init(75);
    Order[80] = 0;
    Order_Dir[80] = 1;
    effect_G0_init(80, time, Complete_Bonus, pos_id2);
}

void Game_Manage_81_1() {
    if (Order_Dir[80] == 0) {
        C_No[2]++;
        C_Timer = 20;
    }
}

void Game_Manage_81_2() {
    if (Scene_Cut) {
        C_Timer = 1;
    }

    if (--C_Timer != 0) {
        return;
    }

    C_No[2]++;
    Stop_Update_Score = 0;
    Order_Dir[80] = 1;
    Order[81] = 1;
    Order[83] = 1;
    Order[84] = 1;
    Order[80] = 1;
    Sound_SE(100);
}

void Game_Manage_81_3() {
    if (Order_Dir[80] == 0) {
        C_No[1]++;
        C_No[2] = 0;
        C_Timer = 50;
    }
}

void Game_Manage_8_2() {
    if (Request_Break[Winner_id ^ 1]) {
        C_Timer = 1;
    }

    if (Scene_Cut) {
        C_Timer = 1;
    }

    if (--C_Timer != 0) {
        return;
    }

    if (Check_Entry_Again()) {
        Forbid_Break = 0;
    }

    Disp_Cockpit = 0;
    Suicide[2] = 1;
    gauge_stop_flag[0] = 1;
    gauge_stop_flag[1] = 1;
    C_No[0]++;
    C_No[1] = 0;
    C_Timer = 30;
}

void Game_Manage_8_3() {
    if (Scene_Cut) {
        C_Timer = 1;
    }

    if (--C_Timer == 0) {
        C_No[0]++;
        C_No[1] = 0;
    }
}

void Game_Manage_9th() {
    switch (C_No[1]) {
    case 0:
        if (PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
            C_No[0]++;
            C_No[1] = 0;
            C_Timer = 75;
            cpExitTask(TASK_PAUSE);

            if (Play_Type != 1 && Round_Operator[WINNER] && Battle_Q[WINNER]) {
                C_No[0] = 10;
            }

            break;
        }

        C_No[1]++;
        C_Timer = 60;
        Stop_Combo = 1;
        BGM_Timer[1] = 1;
        break;

    case 1:
        if (Scene_Cut) {
            C_Timer = 1;
        }

        if (--C_Timer > 0) {
            break;
        }

        C_No[1]++;
        Game_pause = 1;
        Switch_Screen_Init(0);

        if (judge_flag) {
            SsBgmFadeOut(2048);
        }

        /* fallthrough */

    default:
        if (Switch_Screen(0)) {
            BGM_No[0] = 1;
            BGM_Timer[0] = 1;
            G_No[2] = 5;
            G_No[3] = 0;
            G_Timer = 4;
            Cover_Timer = 5;
            C_No[0] = 1;
            C_No[1] = C_No[2] = C_No[3] = 0;
            Suicide[0] = 1;
            Suicide[6] = 1;
            judge_flag = 0;
        }

        break;
    }
}

void Game_Manage_10th() {
    switch (C_No[1]) {
    case 0:
        if (Button_Cut_EX(&C_Timer, 0x7FFF)) {
            C_No[1]++;
            Cover_Timer = 25;
            pulpul_stop();
            Stop_Combo = 1;
            Game_pause = 1;
            Switch_Screen_Init(0);
        }

        break;

    case 1:
        if (Switch_Screen(0)) {
            effect_work_quick_init();
            judge_flag = 0;
            SE_All_Off();
            Check_Naming(0);
            Check_Naming(1);
            pcon_rno[0] = 0;
            pcon_rno[1] = 0;
            pcon_rno[2] = 0;
            pcon_rno[3] = 0;
            appear_type = 1;
            Continue_Coin2[WINNER] = 0;

            if (Mode_Type == MODE_VERSUS || Mode_Type == 5 || Round_Operator[WINNER]) {
                G_No[1] = 3;
                G_No[2] = 0;
                G_No[3] = 0;
                M_No[0] = 0;
                M_No[1] = 0;
                M_No[2] = 0;
                M_No[3] = 0;
                E_No[0] = 5;
                E_No[1] = 0;
                E_No[2] = 0;
                E_No[3] = 0;
                Check_Ending();
                Continue_Coin2[WINNER] = 0;
                Clear_Flash_No();
                break;
            }

            G_No[1] = 4;
            G_No[2] = 0;
            G_No[3] = 0;
            M_No[0] = 0;
            M_No[1] = 0;
            M_No[2] = 0;
            M_No[3] = 0;
            E_No[0] = 6;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;
            E_07_Flag[0] = 0;
            E_07_Flag[1] = 0;
            Clear_Flash_No();
        }

        break;
    }
}

void Check_Naming(s16 PL_id) {
    if (Mode_Type != MODE_ARCADE) {
        return;
    }

    if (E_Number[PL_id][0] == 2) {
        return;
    }

    if (E_Number[PL_id][0] == 3) {
        return;
    }

    Rank_In[PL_id][0] = -1;
    Rank_In[PL_id][1] = -1;
    Rank_In[PL_id][2] = -1;
    Rank_In[PL_id][3] = -1;
}

s32 Check_Ending() {
    s16 xx;

    if (Play_Type == 1) {
        return 0;
    }

    if (Check_Ending_Sub()) {
        G_No[1] = 8;
        G_No[2] = 0;
        E_No[0] = 10;
        Break_Com[WINNER][0] = 1;
        Extra_Break = 0;
        Pause_ID = WINNER;
        End_PL = My_char[WINNER];
        plw[WINNER].wu.operator = 0;
        Operator_Status[WINNER] = 0;
        SsBgmControl(0, 0);
        Control_Time = 481;
        Ending_init();
        Stock_My_char[WINNER] = My_char[WINNER];
        Stock_Player_Color[WINNER] = Player_Color[WINNER];

        if (Direction_Working[Present_Mode]) {
            return 1;
        }

        if (Check_Extra_Setting() == 0) {
            for (xx = 1; xx < 5; xx++) {
                save_w[xx].PL_Color[0][My_char[WINNER]] = 1;
                save_w[xx].PL_Color[1][My_char[WINNER]] = 1;
            }

            if (My_char[WINNER] == 0) {
                save_w[Present_Mode].Extra_Option = 1;
            }
        }

        return 1;
    }

    return 0;
}

s32 Check_Ending_Sub() {
    if (VS_Index[WINNER] > 9) {
        return 1;
    }

    if (Debug_w[47]) {
        return 1;
    }

    return 0;
}

void Additional_Bonus(s16 PL_id) {
    Complete_Bonus = Setup_Comp_Bonus();
    Score[PL_id][Play_Type] += Perfect_Bonus[Winner_id];
    Score[PL_id][Play_Type] += Vital_Bonus[Winner_id];
    Score[PL_id][Play_Type] += Time_Bonus[Winner_id];
    Score[PL_id][Play_Type] += Complete_Bonus;

    if (Score[PL_id][Play_Type] >= 99999900) {
        Score[PL_id][Play_Type] = 99999900;
    }
}

u32 Setup_Comp_Bonus() {
    u32 xx;
    u16 zz;

    if (Play_Type == 1) {
        if (PL_Wins[Loser_id]) {
            return 0;
        }

        return 30000;
    }

    if (PL_Wins[Loser_id]) {
        return Straight_Counter[Winner_id] = 0;
    }

    Straight_Counter[Winner_id]++;

    if (Straight_Counter[Winner_id] >= 12) {
        Straight_Counter[Winner_id] = 11;
    }

    zz = Straight_Counter[Winner_id];
    xx = Comp_Bonus_Data[zz - 1];
    return xx;
}

void request_center_message(s16 Kind_of_Message) {
    request_message = 1;
    message_index = Kind_of_Message;
}

void Setup_Win_Mark() {
    if (Round_Result & 0x200) {
        win_type[Winner_id][PL_Wins[Winner_id]] = 7;
        SsRequest(121);
        SsRequest(140);
        Finish_SE();
        return;
    }

    if (Round_Result & 0x180) {
        Control_Music_Fade(150);
        win_type[Winner_id][PL_Wins[Winner_id]] = 4;
        SsRequest(140);
        Finish_SE();
        return;
    }

    if (Round_Result & 0x800) {
        if (Shin_Gouki_BGM == 0) {
            Control_Music_Fade(150);
        } else {
            Shin_Gouki_BGM = 0;
        }

        win_type[Winner_id][PL_Wins[Winner_id]] = 4;
        SsRequest(0x8CU);
        Finish_SE();
        return;
    }

    win_type[Winner_id][PL_Wins[Winner_id]] = 1;
    SsRequest(121);
    SsRequest(140);
    Finish_SE();
}

void Check_Perfect(s16 PL_id) {
    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        return;
    }

    if (plw[PL_id].wu.vitality != plw[PL_id].wu.vital_new) {
        return;
    }

    Perfect_Flag = 1;
    Perfect_Counter[Winner_id]++;
    Round_Result |= 2;
    win_type[PL_id][PL_Wins[PL_id]] = 3;
}

void Update_VS_Data() {
    if (PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        WINNER = Winner_id;
        LOSER = Loser_id;
        Stock_My_char[LOSER] = My_char[LOSER];
        Stock_Player_Color[LOSER] = Player_Color[LOSER];

        if (Play_Type != 0) {
            return;
        }

        if (Round_Operator[WINNER]) {
            SC_Personal_Time[WINNER] = Control_Time;
            Stage_Continue[WINNER] = 0;
            Request_Disp_Rank[LOSER][0] = -1;
            Request_Disp_Rank[LOSER][1] = -1;
            Request_Disp_Rank[LOSER][2] = -1;
            Request_Disp_Rank[LOSER][3] = -1;
            Stock_Com_Color[WINNER] = -1;
            Stock_Com_Arts[WINNER] = -1;
            EM_History[WINNER][VS_Index[WINNER]] = EM_id;
            Result_Timer[WINNER] += 30;

            if (EM_id == 17) {
                Break_Com[WINNER][EM_id] = (s8)(VS_Index[WINNER]);
            } else {
                VS_Index[WINNER]++;
                Break_Com[WINNER][EM_id] = 1;
            }

            if (PL_Wins[LOSER]) {
                Straight_Counter[WINNER] = 0;
                Straight_Flag[WINNER] = 1;
            }

            if (++Round_Level <= 7) {
                return;
            }

            Round_Level = 7;
            return;
        }

        Score[LOSER][0] = Stage_Stock_Score[LOSER];
        SC_Personal_Time[LOSER] = Control_Time;
        Win_Record[LOSER] = 0;
        Straight_Counter[LOSER] = 0;
        Straight_Flag[LOSER] = 1;
        return;
    }

    Score[Loser_id][0] = Stock_Score[Loser_id];
}

void BGM_Fade_Sub() {
    switch (BGM_No[1]) {
    case 1:
        if (--BGM_Timer[1] == 0) {
            BGM_No[1]++;
            BGM_Timer[1] = 1;
            BGM_Vol = -128;
        }

        break;

    case 0:
        break;

    default:
        if (--BGM_Timer[1] == 0) {
            BGM_Timer[1] = 2;

            if (++BGM_Vol == 0) {
                BGM_No[1] = 0;
            }
        }

        if (!Music_Fade) {
            SsBgmControl(0, BGM_Vol);
        }

        break;
    }
}

void BGM_Control() {
    switch (BGM_No[0]) {
    case 0:
        return;

    case 1:
        if (--BGM_Timer[0] == 0) {
            BGM_No[0]++;
        }

        /* fallthrough */

    case 2:
        BGM_No[0] = 0;

        if (Play_Type == 0 && EM_id == 17) {
            Stage_BGM(17, Round_num);
            break;
        }

        Stage_BGM(bg_w.stage, Round_num);
        break;

    case 3:
        if (--BGM_Timer[0] == 0) {
            BGM_No[0]++;
        }

        /* fallthrough */

    case 4:
        BGM_No[0] = 0;
        BGM_Request(56);
        break;
    }
}

void Setup_BGM_Fade_In(s16 Time) {
    if (!PB_Music_Off) {
        BGM_No[1] = 1;
        BGM_Timer[1] = Time;
    }
}

void Check_Stage_BGM() {
    if (Play_Type == 1) {
        Stage_BGM(bg_w.stage, Round_num);
        return;
    }

    switch (EM_id) {
    case 1:
        if (My_char[Player_id] != 0) {
            Stage_BGM(bg_w.stage, Round_num);
        }

        /* fallthrough */

    case 0:
        if (Introduce_Boss[Player_id][1] & 0x80) {
            Stage_BGM(bg_w.stage, Round_num);
        }

        break;

    case 17:
        Stage_BGM(17, Round_num);
        break;

    default:
        Stage_BGM(bg_w.stage, Round_num);
        break;
    }
}

void Control_Music_Fade(s16 Time) {
    Setup_BGM_Fade_In(Time);
    SsBgmControl(0, -128);
}

void Check_Conclusion_Type() {
    if (Play_Type == 1) {
        return;
    }

    switch (Conclusion_Type) {
    case 0:
        chkComWins();
        break;

    case 1:
        Lost_Round[Player_id]++;
        Stage_Lost_Round[Player_id]++;
        break;

    case 2:
        if (plw[0].wu.vital_new != plw[1].wu.vital_new) {
            Stage_Time_Finish[Winner_id]++;
            chkComWins();
        }

        break;
    }
}

void chkComWins() {
    if (Round_Operator[Winner_id] == 0) {
        Lost_Round[Loser_id]++;
        Stage_Lost_Round[Loser_id]++;
    }
}

void Update_BI_Term() {
    if (Play_Type == 1) {
        return;
    }

    if (plw[Winner_id].sa_healing) {
        Super_Arts_Finish[Winner_id]++;
        Stage_SA_Finish[Winner_id]++;
        return;
    }

    if (plw[Winner_id].wu.vitality == plw[Winner_id].wu.vital_new) {
        Perfect_Finish[Winner_id]++;
        Stage_Perfect_Finish[Winner_id]++;

        if (Round_Result & 0x980) {
            Super_Arts_Finish[Winner_id]++;
            Stage_SA_Finish[Winner_id]++;
        }

        return;
    }

    if (Round_Result & 0x200) {
        Cheap_Finish[Winner_id]++;
        Stage_Cheap_Finish[Winner_id]++;
        return;
    }

    if (Round_Result & 0x980) {
        Super_Arts_Finish[Winner_id]++;
        Stage_SA_Finish[Winner_id]++;
    }
}

void Ck_Win_Record() {
    if (PL_Wins[Winner_id] < save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        return;
    }

    switch (Mode_Type) {
    case MODE_ARCADE:
        if (Play_Type == 1) {
            Win_Record[Loser_id] = 0;

            if (++Win_Record[Winner_id] > 999) {
                Win_Record[Winner_id] = 999;
            }

            Stock_Win_Record[Winner_id] = Win_Record[Winner_id];
        }

        break;

    case MODE_VERSUS:
        if (++VS_Win_Record[Winner_id] > 999) {
            VS_Win_Record[Winner_id] = 999;
        }

        break;

    default:
        // Do nothing
        break;
    }
}

void Update_Level_Control() {
    if (Round_Operator[Winner_id]) {
        if ((Round_Operator[Loser_id]) != 0) {
            return;
        }

        Control_Time += 40;

        if (Control_Time > Limit_Time) {
            Control_Time = Limit_Time;
        }

        return;
    }

    if ((Control_Time -= 40) < 0) {
        Control_Time = 0;
    }
}

s32 Judge_Next_Disposal() {
    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        return 0;
    }

    if (PL_Wins[0] != PL_Wins[1]) {
        return 0;
    }

    if (PL_Wins[0] >= save_w[Present_Mode].Battle_Number[Play_Type]) {
        return 1;
    }

    return 0;
}

void Quick_Entry() {
    s8 grade;

    if (Check_Entry_Again()) {
        Forbid_Break = 0;
        Extra_Break = 0;
    }

    if (PL_Wins[Winner_id] < save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        return;
    }

    if (plw[LOSER].wu.operator) {
        Loser_Sub();

        if (Mode_Type != MODE_ARCADE) {
            plw[LOSER].wu.operator = 1;
        }

        Be_Continue();
    }

    if (Play_Type == 1) {
        grade = judge_item[Winner_id][1].grade;

        if (grade > Best_Grade[Winner_id]) {
            Best_Grade[Winner_id] = grade;
        }
    }
}

s32 Check_Entry_Again() {
    if (Battle_Q[Winner_id]) {
        return 0;
    }

    if (Play_Type == 1) {
        return 1;
    }

    if (VS_Index[WINNER] >= 6) {
        return 0;
    }

    if (VS_Index[WINNER] < 10) {
        return 1;
    }

    return 0;
}

void Loser_Sub() {
    plw[LOSER].wu.operator = 0;
    Operator_Status[LOSER] = 0;
    Sel_PL_Complete[LOSER] = 0;
    Sel_Arts_Complete[LOSER] = 0;

    if (Play_Type == 0) {
        if (--Round_Level < 0) {
            Round_Level = 0;
        }

        Stage_Continue[LOSER]++;
    }
}

void Be_Continue() {
    if (Mode_Type != MODE_ARCADE) {
        return;
    }

    Continue_Count_Down[LOSER] = 0;
    Continue_Count[LOSER] = 9;
    E_Number[LOSER][0] = 5;
    E_Number[LOSER][0] = 5;
    E_Number[LOSER][1] = 0;
    E_Number[LOSER][2] = 0;
    E_Number[LOSER][3] = 0;
}

void Disp_Winner() {
    if (Play_Type == 1) {
        effect_56_init(My_char[Winner_id] + 7, 1);
        SsRequest(141);
        return;
    }

    if (Round_Operator[Winner_id]) {
        effect_56_init(5, 1);
        SsRequest(141);
        return;
    }

    effect_56_init(6, 1);
    SsRequest(142);
}

void Pool_Score(s16 PL_id) {
    u32 Score_Buff;

    if (Perfect_Flag) {
        Perfect_Bonus[Winner_id] += 50000;
    }

    Score_Buff = plw[PL_id].wu.vital_new * 100 / Max_vitality;
    Score_Buff *= 500;
    Vital_Bonus[Winner_id] += Score_Buff;

    if (save_w[Present_Mode].Time_Limit == -1) {
        Time_Bonus[Winner_id] = 0;
        return;
    }

    Time_Bonus[Winner_id] += round_timer.size.half.h * 300;
}

s32 Check_Break_Into_CPU(s16 PL_id) {
    if (Debug_w[70] == 9) {
        Break_Into_CPU = 2;
        return Battle_Q[PL_id] = 1;
    }

    Break_Into_CPU = 0;
    Battle_Q[PL_id] = 0;

    if (Round_Result & 0x8000) {
        return 0;
    }

    if (Break_Com[PL_id][17]) {
        return 0;
    }

    if (Continue_Coin[PL_id]) {
        return 0;
    }

    if (VS_Index[PL_id] < 7 || VS_Index[PL_id] >= 9) {
        return 0;
    }

    if (Straight_Flag[PL_id]) {
        return 0;
    }

    if (judge_final[Player_id][0].sp_point < 2) {
        return 0;
    }

    if (Super_Arts_Finish[PL_id] < BIC_SA_Data[0][save_w[Present_Mode].Battle_Number[Play_Type]]) {
        return 0;
    }

    if (Check_BI_Grade(PL_id)) {
        Break_Into_CPU = 2;
        return Battle_Q[PL_id] = 1;
    }

    return 0;
}

void Judge_Winner() {
    grade_makeup_judgement_gals();

    if (judge_gals[0].grade == judge_gals[1].grade) {
        if (Play_Type == 0) {
            Winner_id = Player_id;
            Loser_id = COM_id;
            return;
        }

        Winner_id = Champion;
        Loser_id = Champion ^ 1;
        return;
    }

    if (judge_gals[0].grade > judge_gals[1].grade) {
        Winner_id = 0;
        Loser_id = 1;
        return;
    }

    Winner_id = 1;
    Loser_id = 0;
}

s32 Check_Disp_Winner() {
    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        return Disp_Win_Name = 0;
    }

    if (PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        return Disp_Win_Name = 1;
    }

    if (Conclusion_Type == 0) {
        return Disp_Win_Name = 0;
    }

    return Disp_Win_Name = 1;
}

void Check_Fade_Out_BGM(s16 Time) {
    if (Music_Fade) {
        return;
    }

    if (PL_Wins[Winner_id] < save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        return;
    }

    Music_Fade = 1;
    SsBgmFadeOut(Time);
}

s32 Check_BI_Grade(s16 PL_id) {
    s16 ix;

    for (ix = 0; ix < VS_Index[PL_id]; ix++) {
        if (judge_final[PL_id][0].vs_cpu_grade[ix] < 9) {
            return 0;
        }
    }

    return 1;
}

void Game_Manage_11th() {
    switch (C_No[1]) {
    case 0:
        Forbid_Break = -1;
        C_No[1]++;
        EM_Rank = 2;
        Q_Country = Battle_Country;
        C_Timer = 90;
        effect_81_init(30);
        break;

    case 1:
        if (--C_Timer == 0) {
            C_No[1]++;
            C_Timer = 150;
            C_Timer = 60;
        }

        break;

    case 2:
        if (--C_Timer == 0) {
            C_No[1]++;
        }

        break;

    case 3:
        C_No[1]++;
        Switch_Screen_Init(0);
        break;

    case 4:
        if (Switch_Screen(0)) {
            G_No[1] = 11;
            G_No[2] = 0;
            G_No[3] = 0;
            E_No[0] = 9;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;
            effect_work_kill_mod_plcol();
            Cover_Timer = 21;
        }

        break;
    }
}

void Game_Manage_12th() {
    void (*SC12_Jmp_Tbl[10])() = { Game_Manage_12_0, Game_Manage_12_1, Game_Manage_12_2, Game_Manage_12_3,
                                   Game_Manage_12_4, Game_Manage_12_5, Game_Manage_12_1, Game_Manage_12_7,
                                   Game_Manage_12_8, Game_Manage_12_5 };

    SC12_Jmp_Tbl[C_No[1]]();

    if (Bonus_Type == 20) {
        bcounter_write();
    }
}

void Game_Manage_12_0() {
    s16 ix;

    Suicide[0] = 0;
    Suicide[6] = 0;
    Suicide[5] = 0;

    if (effect_84_init()) {
        return;
    }

    C_No[1]++;
    Extra_Break = 0;
    request_message = 0;
    Complete_Victory = 0;
    Conclusion_Flag = 0;
    Perfect_Flag = 0;
    Round_Result = 0;
    Reserve_Cut = 0;
    Next_Step = 0;
    judge_flag = 0;
    Stop_Combo = 0;

    if (Demo_Flag) {
        Stop_SG = 0;
    }

    Complete_Judgement = 0;
    Music_Fade = 0;
    Round_Operator[0] = plw[0].wu.operator;
    Round_Operator[1] = plw[1].wu.operator;
    CP_No[0][0] = 0;
    CP_No[1][0] = 0;

    for (ix = 0; ix < 4; ix++) {
        Message_Suicide[ix] = 0;
    }

    Stock_Score[Player_id] = Score[Player_id][0];

    if (Bonus_Type == 20) {
        C_No[1] = 6;
        Time_Stop = 1;
        Time_Over = 0;
        Exit_No = 0;
        Unit_Of_Timer = 0;
        setup_bonus_car_parts();
        bcount_cont_init();
    }
}

void Game_Manage_12_1() {
    bcount_cont_main();

    if (Next_Step != 0) {
        C_No[1]++;
        C_No[2] = 0;
        C_No[3] = 0;
        Allow_a_battle_f = 1;
        Disp_Bonus_Contents = 0;
    }
}

void Game_Manage_12_2() {
    bcount_cont_main();

    if (!Bonus_Game_Complete) {
        return;
    }

    C_No[1]++;
    C_No[2] = 0;
    C_Timer = 30;
    Allow_a_battle_f = 0;
    Forbid_Break = -1;
    Completion_Bonus[Player_id][1] = -128;
    Stock_Bonus_Game_Result = Bonus_Game_result;
    Bonus_Score = 0;
    Final_Bonus_Score = Setup_Final_Score(21);
    effect_58_init(6, 10, 169);
    grade_makeup_bonus_parameter(Player_id);

    if (Check_Bonus_Perfect()) {
        C_Timer = 20;
    } else {
        C_No[1] = 4;
    }

    cpExitTask(TASK_PAUSE);
}

void Game_Manage_12_3() {
    switch (C_No[2]) {
    case 0:
        if (Cut_Cut_C_Timer() == 0) {
            C_No[2]++;
            C_Timer = 10;
            request_center_message(4);
            effect_58_init(6, 1, 155);
            effect_58_init(6, 60, 156);
        }

        break;

    case 1:
        if (request_message == 0) {
            C_No[2]++;
            C_Timer = 6;
        }

        break;

    case 2:
        if (--C_Timer == 0) {
            C_No[2]++;
            C_Timer = 20;
        }

        break;

    case 3:
        if (Cut_Cut_C_Timer() == 0) {
            C_No[1]++;
            C_No[2] = 0;
            C_No[3] = 0;
            C_Timer = 30;
        }

        break;
    }
}

void Game_Manage_12_4() {
    switch (C_No[2]) {
    case 0:
        if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
            C_No[2]++;
            C_Timer = 20;
            effect_08_init(7, 0, 1, 15, 0);
            Disp_Score_Buff[0] = Bonus_Score;
            effect_14_init(0, 35, 11, 15);
        }

        break;

    case 1:
        if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
            C_No[2]++;
            C_Timer = 1;
            Bonus_Score = 0;
        }

        break;

    case 2:
        if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
            if (Bonus_Game_result == 0 && !(PB_Status & 2)) {
                C_No[2] = 4;
                C_Timer = 30;
                break;
            }

            if (Bonus_Game_result == 0) {
                Bonus_Game_result = 1;
            } else {
                Bonus_Score += 1000;
                Score[Player_id][0] += 1000;
                Disp_Score_Buff[0] = Bonus_Score;
                Sound_SE(100);
            }

            if (--Bonus_Game_result == 0) {
                C_No[2]++;

                if (PB_Status) {
                    C_No[3] = 1;
                    C_Timer = 10;
                    break;
                }

                C_No[3] = 0;
                C_Timer = 20;
                break;
            }

            C_Timer = 3;
        }

        break;

    case 3:
        switch (C_No[3]) {
        case 0:
            if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
                C_No[2]++;
                C_Timer = 30;
                Bonus_Game_result = Stock_Bonus_Game_Result;
            }

            break;

        case 1:
            if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
                C_No[3]++;
                C_Timer = 10;
                Disp_Bonus_Perfect();
            }

            break;

        case 2:
            if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
                C_No[3]++;
                C_Timer = 40;

                if (PB_Status & 1) {
                    Score[Player_id][0] += Ball_Perfect_PTS[0][Bonus_Stage_Level];
                }

                if (PB_Status & 2) {
                    Score[Player_id][0] += Ball_Perfect_PTS[1][Bonus_Stage_Level];
                }

                if (Score[Player_id][0] >= 99999900) {
                    Score[Player_id][0] = 99999900;
                }

                Flash_Bonus_Perfect();
                break;
            }

            break;

        default:
            if (--C_Timer == 0) {
                C_No[2]++;
                C_Timer = 30;
            }

            break;
        }

        break;

    default:
        if (Cut_Cut_C_Timer() == 0) {
            C_No[1]++;
            C_No[2] = 0;
            C_No[3] = 0;
            C_Timer = 10;
            Forbid_Break = 0;
            Suicide[5] = 1;
            Check_Fade_Out_BGM(546);
        }

        break;
    }
}

void Game_Manage_12_5() {
    switch (C_No[2]) {
    case 0:
        if (Debug_w[70] != 2 && --C_Timer == 0) {
            C_No[2]++;
            C_Timer = 20;
        }

        break;

    case 1:
        if (Scene_Cut) {
            C_Timer = 1;
        }

        if (--C_Timer == 0) {
            C_No[2]++;
        }

        break;

    default:
        MANAGE_X = 1;
        break;
    }
}

void Game_Manage_12_7() {
    bcount_cont_main();

    if (Check_Time_Over()) {
        return;
    }

    if (!Bonus_Game_Complete) {
        return;
    }

    C_No[1]++;
    C_No[2] = 0;
    C_No[3] = 0;
    C_Timer = 30;
    Allow_a_battle_f = 0;
    Forbid_Break = -1;
    Completion_Bonus[Player_id][0] = -128;
    Final_Bonus_Score = Setup_Final_Score(20);
    grade_makeup_bonus_parameter(Player_id);
    effect_58_init(6, 10, 169);
}

void Game_Manage_12_8() {
    switch (C_No[2]) {
    case 0:
        switch (C_No[3]) {
        case 0:
            Next_Step = 0;

            if (effect_35_init(60, 10) == 0) {
                C_No[3]++;
            }

            break;

        case 1:
            if (Next_Step) {
                C_No[3]++;
                C_Timer = 20;
            }

            break;

        case 2:
            if (C_Timer < 11 && Scene_Cut) {
                C_Timer = 1;
            }

            if (--C_Timer == 0) {
                C_No[2]++;
                C_No[3] = 0;
                C_Timer = 30;
            }

            break;
        }

        break;

    case 1:
        if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
            C_No[2]++;
            C_Timer = 20;
            Score[Player_id][0] += Bonus_Score;
            effect_08_init(7, 0, 1, 15, 0);
            Disp_Score_Buff[0] = Bonus_Score;
            effect_14_init(0, 35, 11, 15);

            if (Bonus_Game_result == 0) {
                C_No[2] = 99;
                C_Timer = 120;
            }
        }

        break;

    case 2:
        if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
            C_No[2]++;
            C_Timer = 1;
        }

        break;

    case 3:
        if (Bonus_Cut_Sub() == 0 && --C_Timer == 0) {
            if (bcounter_down(0) == 0) {
                C_No[2]++;
                C_Timer = 30;
                C_Timer = 3;
                Bonus_Score += 1000;
                Score[Player_id][0] += 1000;
                Disp_Score_Buff[0] = Bonus_Score;
                Sound_SE(100);
                break;
            }

            C_Timer = 3;
            Bonus_Score += 1000;
            Score[Player_id][0] += 1000;
            Disp_Score_Buff[0] = Bonus_Score;
            Sound_SE(100);
        }

        break;

    case 4:
        if (--C_Timer == 0) {
            C_No[2]++;
            C_Timer = 30;
        }

        break;

    default:
        if (Debug_w[70] != 2 && Cut_Cut_C_Timer() == 0) {
            C_No[1]++;
            C_No[2] = 0;
            C_No[3] = 0;
            C_Timer = 10;
            Forbid_Break = 0;
            Suicide[5] = -128;
            Check_Fade_Out_BGM(546);
        }

        break;
    }
}

u8 Check_Bonus_Perfect() {
    PB_Status = 0;

    if (Stock_Bonus_Game_Result >= 20) {
        PB_Status |= 1;
    }

    if (Bonus_Game_ex_result >= 20) {
        PB_Status |= 2;
    }

    return PB_Status;
}

void Disp_Bonus_Perfect() {
    switch (PB_Status) {
    case 1:
        effect_08_init(6, 0, 5, 15, 0);
        Disp_Score_Buff[0] = Ball_Perfect_PTS[0][Bonus_Stage_Level];
        effect_14_init(0, 35, 15, 15);
        break;

    case 2:
        effect_08_init(6, 0, 5, 26, 1);
        Disp_Score_Buff[1] = Ball_Perfect_PTS[1][Bonus_Stage_Level];
        effect_14_init(1, 35, 15, 26);
        break;

    case 3:
        effect_08_init(6, 0, 5, 15, 0);
        Disp_Score_Buff[0] = Ball_Perfect_PTS[0][Bonus_Stage_Level];
        effect_14_init(0, 35, 15, 15);
        effect_08_init(6, 0, 9, 26, 1);
        Disp_Score_Buff[1] = Ball_Perfect_PTS[1][Bonus_Stage_Level];
        effect_14_init(1, 35, 19, 26);
        break;
    }

    SsRequest(Winner_id + 102);
}

void Flash_Bonus_Perfect() {
    switch (PB_Status) {
    case 1:
        Suicide[5] = 1;
        break;

    case 2:
        Suicide[5] = 1;
        break;

    case 3:
        Suicide[5] = 1;
        break;
    }
}

u32 Setup_Final_Score(s16 Type) {
    u32 xx;

    if (Type == 21) {
        xx = Bonus_Game_result * 1000;

        if (Stock_Bonus_Game_Result >= 20) {
            xx += Ball_Perfect_PTS[0][Bonus_Stage_Level];
        }

        if (Bonus_Game_ex_result >= 20) {
            xx += Ball_Perfect_PTS[1][Bonus_Stage_Level];
        }

        xx += Score[Player_id][0];

        if (xx >= 99999900) {
            xx = 99999900;
        }

        return xx;
    }

    switch (Bonus_Game_result) {
    case 2:
        xx = 30000;
        break;

    case 3:
        xx = 50000;
        break;

    default:
        xx = 0;
        break;
    }

    Bonus_Score = xx;
    xx += Counter_hi * 1000;
    Bonus_Score_Plus = xx;
    xx += Score[Player_id][0];

    if (xx >= 99999900) {
        xx = 99999900;
    }

    return xx;
}

s32 Bonus_Cut_Sub() {
    if (Scene_Cut) {
        Sound_SE(100);
        Bonus_Game_result = 0;
        Score[Player_id][0] = Final_Bonus_Score;

        if (Score[Player_id][0] >= 99999900) {
            Score[Player_id][0] = 99999900;
        }

        if (Disp_Bonus_Contents == 0) {
            effect_08_init(7, 0, 1, 15, 0);
        }

        if (Bonus_Type == 21) {
            if (Disp_Bonus_Contents == 0) {
                Disp_Score_Buff[0] = Stock_Bonus_Game_Result * 1000;
                effect_14_init(0, 35, 11, 15);
            }

            Disp_Bonus_Perfect();
            Flash_Bonus_Perfect();
            C_No[2] = 3;
            C_No[3] = 99;
            return C_Timer = 90;
        }

        bcounter_down(1);

        if (Disp_Bonus_Contents == 0) {
            Disp_Score_Buff[0] = Bonus_Score_Plus;
            effect_14_init(0, 35, 11, 15);
        }

        C_No[2] = 4;
        C_No[3] = 99;
        return C_Timer = 90;
    }

    return 0;
}

s16 Check_Time_Over() {
    s16 return_x = 0;

    switch (C_No[2]) {
    case 0:
        if (Time_Over) {
            C_No[2]++;
            C_Timer = 60;
            request_center_message(2);
            SsRequest(143);
            return_x = 1;
        }

        break;

    case 1:
        if (--C_Timer == 0) {
            C_No[2]++;
            Game_pause = 0;
            Suicide[5] = 1;
        }

        break;
    }

    return return_x;
}

void complete_victory_pause() {
    Complete_Victory = 1;
}

void Game_Manage_13th() {};
