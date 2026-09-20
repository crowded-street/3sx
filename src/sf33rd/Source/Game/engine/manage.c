/**
 * @file manage.c
 * Engine management
 */

#include "sf33rd/Source/Game/engine/manage.h"
#include "common.h"
#include "constants.h"
#include "main.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
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
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/pause.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/count.h"
#include "sf33rd/Source/Game/ui/input_history.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/Game/engine/manage_internal.h"


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

static s32 boss_intro_is_due(void) {
    return Play_Type == 0 && (EM_id == 0 || (My_char[Player_id] == 0 && EM_id == 1)) &&
           !(Introduce_Boss[Player_id][1] & 0x80);
}

static s32 arcade_run_reaches_ending(void) {
    return Play_Type == 0 && Mode_Type == MODE_ARCADE &&
           PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] && VS_Index[Winner_id] > 8 &&
           plw[Winner_id].wu.operator != 0 && E_Number[Loser_id][0] != 2;
}

static s32 round_has_a_decision(void) {
    return plw[0].wu.vital_new != plw[1].wu.vital_new || Mode_Type == MODE_NORMAL_TRAINING ||
           Mode_Type == MODE_PARRY_TRAINING;
}

static s32 cockpit_is_shown(void) {
    return Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING && omop_cockpit;
}

static s32 winner_continues_freely(void) {
    return Mode_Type == MODE_VERSUS || Mode_Type == 5 || Round_Operator[WINNER];
}

static s32 combo_display_is_busy(void) {
    return cmb_all_stock[0] != 0 || cmb_calc_now[0] != 0 || cmb_calc_now[1] != 0;
}

static s32 winner_owns_the_credit(void) {
    return Round_Operator[Winner_id] != 0 || Mode_Type == MODE_VERSUS || Mode_Type == 5;
}

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

/* Note that a side is being played by a human this round. Written out once per
 * side, identical apart from the index. */
static void record_round_operator(s16 side) {
    if (plw[side].wu.operator) {
        Round_Operator[side] = 1;
        Final_Play_Type[side] = Play_Type;
    }
}

void Game_Manage_1st() {
    Switch_Screen(0);
    EXE_obroll = 0;

    if (bg_w.stage == 21 || bg_w.stage == 20) {
        C_No[0] = 11;
    } else {
        C_No[0] = 1;
    }

    appear_work_clear();
    win_sp_flag = 0;
    BGM_No[1] = 0;
    BGM_No[0] = 0;
    Appear_Q = 0;
    Clear_1Stage_Work();
    All_Clear_Suicide();
    Round_Operator[0] = 0;
    Round_Operator[1] = 0;

    record_round_operator(0);
    record_round_operator(1);

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

        if (EM_id == CHAR_Q) {
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
    } else if (Mode_Type != MODE_NETWORK) {
        cpReadyTask(TASK_PAUSE, Pause_Task);
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

/* Reset both players' separation tracking. Wait_Seek_Time cleared it in two of
 * its arms with the same pair of nested loops. */
static void clear_separate_areas(void) {
    s16 ix;
    s16 ix2;

    for (ix = 0; ix < 2; ix++) {
        for (ix2 = 0; ix2 < 3; ix2++) {
            Separate_Area[ix][ix2] = 0;
            Shell_Separate_Area[ix][ix2] = 0;
        }
    }
}

s32 Wait_Seek_Time() {

    switch (Play_Mode) {
    case 1:
        if (Mode_Type != MODE_NETWORK) {
            clear_separate_areas();

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

        clear_separate_areas();

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

    InputHistory_Clear();
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

    if (Is_Training_Mode(Mode_Type)) {
        Next_Step = 1;
    } else {
        effect_B2_init();
    }
}

/* Training mode starts a round from a clean slate rather than carrying the
 * previous one's state over. */
static void reset_training_round_state(void) {
    if (Is_Training_Mode(Mode_Type)) {
        Score[0][2] = 0;
        Score[1][2] = 0;
        Game_pause = 0;
        pcon_rno[0] = 0;
        pcon_rno[1] = 0;
        pcon_rno[2] = 0;
        pcon_rno[3] = 0;
        appear_type = APPEAR_TYPE_NON_ANIMATED;
        erase_extra_plef_work();
        compel_bg_init_position();
        win_lose_work_clear();
    }
}

/* The round actually starting, once the fade and the intro have finished:
 * arm the timers, show the boss introduction if this is the stage for it, and
 * start the round-begin effect outside demos and training. The early return is
 * the original's `break` while Next_Step is still 0. */
static void begin_the_round(void) {
    if (Next_Step == 0) {
        return;
    }

    C_No[0]++;
    C_No[1] = 0;
    C_No[2] = 0;
    Allow_a_battle_f = 1;
    vital_inc_timer = 50;
    vital_dec_timer = 40;
    sag_inc_timer[0] = sag_inc_timer[1] = 0;

    if (boss_intro_is_due()) {
        Introduce_Boss[Player_id][1] |= 128;
        Check_Stage_BGM();
    }

    if (Demo_Flag == 0 && !Is_Training_Mode(Mode_Type)) {
        effect_58_init(10, 60, 0);
    }
}

void Game_Manage_2_4() {
    switch (C_No[2]) {
    case 0:
        if (Round_num) {
            C_No[2] = 3;
            return;
        }

        C_No[2]++;
        C_Timer = 1;
        Forbid_Break = 1;
        FadeInit();
        FadeOut(0, 0xFF, 8);
        Disp_Cockpit = 1;

        reset_training_round_state();

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
        begin_the_round();
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
    if (arcade_run_reaches_ending()) {
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

        if (round_has_a_decision()) {
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
        appear_type = APPEAR_TYPE_UNKNOWN_3;
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

        if (cockpit_is_shown()) {
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

    if (cockpit_is_shown()) {
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
    if (combo_display_is_busy()) {
        return 1;
    }

    if (PL_Wins[Winner_id] < save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        return 0;
    }

    return 0;
}

void Game_Manage_7_3() {
    // NOTE: the two arms below are identical - whichever way this test goes, the
    // same countdown runs and the same early return happens, so the condition
    // has no effect. That looks like a mistake and is left exactly as it is:
    // the campaign reports what it finds and changes nothing. If one side was
    // meant to use a different timer, fixing it is a behaviour change and needs
    // a decision, not a refactor.
    //
    // Lifting the pair into a helper was tried and reverted: it moved the two
    // bumps into the helper rather than removing them, and measured flat.
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

/* Wait out the scene timer, cut short when the scene was skipped, then step
 * to the next scene. Game_Manage_7_6 and Game_Manage_8_3 are this, identically;
 * both keep their names because the jump tables take their addresses. */
static void advance_scene_after_cut() {
    if (Scene_Cut) {
        C_Timer = 1;
    }

    if (--C_Timer == 0) {
        C_No[0]++;
        C_No[1] = 0;
    }
}

void Game_Manage_7_6() {
    advance_scene_after_cut();
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

    if (winner_owns_the_credit()) {
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
    advance_scene_after_cut();
}

/* Outside versus, a winning human player who queued another battle goes
 * straight on to it rather than through the normal continue flow. */
static s32 winner_queued_another_battle(void) {
    return Play_Type != 1 && Round_Operator[WINNER] && Battle_Q[WINNER];
}

/* The match is over once the winner has enough round wins; otherwise this is
 * just the end of a round. Both `break`s left the switch with nothing after
 * them, so each is a `return` here. */
static void settle_match_or_next_round(void) {
    if (PL_Wins[Winner_id] >= save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        C_No[0]++;
        C_No[1] = 0;
        C_Timer = 75;
        cpExitTask(TASK_PAUSE);

        if (winner_queued_another_battle()) {
            C_No[0] = 10;
        }

        return;
    }

    C_No[1]++;
    C_Timer = 60;
    Stop_Combo = 1;
    BGM_Timer[1] = 1;
}

void Game_Manage_9th() {
    switch (C_No[1]) {
    case 0:
        settle_match_or_next_round();
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

/* Once the screen switch has finished, hand over to whichever screen comes
 * next - the winner continuing straight on, or the versus result. The
 * original's `break` inside the free-continue arm left the switch with nothing
 * after it, so it is a `return` here. */
static void hand_over_after_round(void) {
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
        appear_type = APPEAR_TYPE_ANIMATED;
        Continue_Coin2[WINNER] = 0;

        if (winner_continues_freely()) {
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
            return;
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
        hand_over_after_round();
        break;
    }
}

static void hand_over_to_ending_screen(void) {
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
        hand_over_to_ending_screen();
        break;
    }
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
