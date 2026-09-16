/**
 * @file manage_result.c
 * Scoring, grading, win records and the continue flow.
 *
 * Split out of manage.c, which was 2038 lines even after the bonus stage left.
 * The Game_Manage_* phase machine stayed there and calls into these.
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
#include "sf33rd/Source/Game/engine/manage_internal.h"

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

        return 1;
    }

    return 0;
}

s32 Check_Ending_Sub() {
    if (VS_Index[WINNER] > 9) {
        return 1;
    }

#if DEBUG
    if (debug_config.skip_to_ending) {
        return 1;
    }
#endif

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

/* A human player beating a CPU opponent in the arcade run: record the opponent,
 * advance the ladder, and raise the difficulty a step. The original returned
 * from Update_VS_Data at the end of this block either way, which is the return
 * at the call site. */
static void record_arcade_win(void) {
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
            record_arcade_win();
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

/* One step of the music fading back in: raise the volume every other frame
 * until it reaches zero, then stop fading. */
static void step_bgm_fade_in(void) {
    if (--BGM_Timer[1] == 0) {
        BGM_Timer[1] = 2;

        if (++BGM_Vol == 0) {
            BGM_No[1] = 0;
        }
    }

    if (!Music_Fade) {
        SsBgmControl(0, BGM_Vol);
    }
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
        step_bgm_fade_in();
        break;
    }
}

/* Start the stage's music. One opponent gets a fixed track rather than the
 * stage's own; that is the original's special case and its number. */
static void start_stage_bgm(void) {
    BGM_No[0] = 0;

    if (Play_Type == 0 && EM_id == 17) {
        Stage_BGM(17, Round_num);
        return;
    }

    Stage_BGM(bg_w.stage, Round_num);
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
        start_stage_bgm();
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

/* A human loser is offered a continue. Outside arcade the side stays marked as
 * a player whatever the continue does, which is the original's shape. */
static void offer_continue_to_loser(void) {
    if (plw[LOSER].wu.operator) {
        Loser_Sub();

        if (Mode_Type != MODE_ARCADE) {
            plw[LOSER].wu.operator = 1;
        }

        Be_Continue();
    }
}

/* In versus, keep the winner's best grade of the session. */
static void record_versus_best_grade(void) {
    s8 grade;

    if (Play_Type == 1) {
        grade = judge_item[Winner_id][1].grade;

        if (grade > Best_Grade[Winner_id]) {
            Best_Grade[Winner_id] = grade;
        }
    }
}

void Quick_Entry() {
    if (Check_Entry_Again()) {
        Forbid_Break = 0;
        Extra_Break = 0;
    }

    if (PL_Wins[Winner_id] < save_w[Present_Mode].Battle_Number[Play_Type] + 1) {
        return;
    }

    offer_continue_to_loser();
    record_versus_best_grade();
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
    } else if (Round_Operator[Winner_id]) {
        effect_56_init(5, 1);
        SsRequest(141);
    } else {
        effect_56_init(6, 1);
        SsRequest(142);
    }
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
    } else {
        Time_Bonus[Winner_id] += round_timer * 300;
    }
}

/* The seven conditions that rule a CPU break-in out, in the order the original
 * tested them. `||` short-circuits left to right, so each is still reached only
 * when the earlier ones were false, and none of them has a side effect. */
static s32 break_in_is_ruled_out(s16 PL_id) {
    return (Round_Result & 0x8000) || Break_Com[PL_id][17] || Continue_Coin[PL_id] ||
           (VS_Index[PL_id] < 7 || VS_Index[PL_id] >= 9) || Straight_Flag[PL_id] ||
           (judge_final[Player_id][0].sp_point < 2) ||
           (Super_Arts_Finish[PL_id] < BIC_SA_Data[0][save_w[Present_Mode].Battle_Number[Play_Type]]);
}

s32 Check_Break_Into_CPU(s16 PL_id) {
    Break_Into_CPU = 0;
    Battle_Q[PL_id] = 0;

    if (break_in_is_ruled_out(PL_id)) {
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

/* Once the screen switch has finished, hand control to the ending sequence. */
