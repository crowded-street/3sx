/**
 * @file manage_bonus.c
 * The bonus stage's phase machine and its scoring.
 *
 * Split out of manage.c, which was 2549 lines. Game_Management enters this
 * through Game_Manage_12th and the phases chain through each other from there;
 * the round and continue flow stayed in manage.c.
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

/* The cut animation has finished and the phase timer has run out. Nine of the
 * bonus stage's phases open with this exact test.
 *
 * `--C_Timer` sits on the right of the `&&`, so the timer is only counted down
 * once the cut is over. That short-circuit is load-bearing, which is why the
 * whole test moves together rather than being split in two. */
static s32 bonus_cut_and_timer_finished(void) {
    return Bonus_Cut_Sub() == 0 && --C_Timer == 0;
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
        Time_Over = false;
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

/* The ball stage's final score: the per-ball total, a perfect bonus for each
 * half if it was earned, and the running score, clamped at the display's
 * maximum. */
static u32 ball_stage_final_score(void) {
    u32 xx = Bonus_Game_result * 1000;

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

/* Put the result message and its two effects on screen once the cut has run. */
static void announce_bonus_result(void) {
    if (Cut_Cut_C_Timer() == 0) {
        C_No[2]++;
        C_Timer = 10;
        request_center_message(4);
        effect_58_init(6, 1, 155);
        effect_58_init(6, 60, 156);
    }
}

void Game_Manage_12_3() {
    switch (C_No[2]) {
    case 0:
        announce_bonus_result();
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

/* Pay out whichever halves of the perfect bonus were earned, clamp the score at
 * the display's maximum, and start the flash. */
static void award_perfect_bonus(void) {
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
}

static void run_bonus_perfect_result_phase(void) {
    switch (C_No[3]) {
    case 0:
        if (bonus_cut_and_timer_finished()) {
            C_No[2]++;
            C_Timer = 30;
            Bonus_Game_result = Stock_Bonus_Game_Result;
        }

        break;

    case 1:
        if (bonus_cut_and_timer_finished()) {
            C_No[3]++;
            C_Timer = 10;
            Disp_Bonus_Perfect();
        }

        break;

    case 2:
        if (bonus_cut_and_timer_finished()) {
            award_perfect_bonus();
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
}

/* Tick one unit off the bonus tally: award its thousand points and its sound,
 * or move on to the perfect bonus or the wrap-up once the tally is empty. Every
 * `break` in the original left the switch with nothing after it, so each is a
 * `return` here. */
static void count_one_bonus_hit(void) {
    if (Bonus_Game_result == 0 && !(PB_Status & 2)) {
        C_No[2] = 4;
        C_Timer = 30;
        return;
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
            return;
        }

        C_No[3] = 0;
        C_Timer = 20;
        return;
    }

    C_Timer = 3;
}

void Game_Manage_12_4() {
    switch (C_No[2]) {
    case 0:
        if (bonus_cut_and_timer_finished()) {
            C_No[2]++;
            C_Timer = 20;
            effect_08_init(&(Effect08Init){7, 0, 1, 15, 0});
            Disp_Score_Buff[0] = Bonus_Score;
            effect_14_init(0, 35, 11, 15);
        }

        break;

    case 1:
        if (bonus_cut_and_timer_finished()) {
            C_No[2]++;
            C_Timer = 1;
            Bonus_Score = 0;
        }

        break;

    case 2:
        if (bonus_cut_and_timer_finished()) {
            count_one_bonus_hit();
        }

        break;

    case 3:
        run_bonus_perfect_result_phase();
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
        if (--C_Timer == 0) {
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
        if (bonus_cut_and_timer_finished()) {
            C_No[2]++;
            C_Timer = 20;
            Score[Player_id][0] += Bonus_Score;
            effect_08_init(&(Effect08Init){7, 0, 1, 15, 0});
            Disp_Score_Buff[0] = Bonus_Score;
            effect_14_init(0, 35, 11, 15);

            if (Bonus_Game_result == 0) {
                C_No[2] = 99;
                C_Timer = 120;
            }
        }

        break;

    case 2:
        if (bonus_cut_and_timer_finished()) {
            C_No[2]++;
            C_Timer = 1;
        }

        break;

    case 3:
        if (bonus_cut_and_timer_finished()) {
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
        if (Cut_Cut_C_Timer() == 0) {
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
        effect_08_init(&(Effect08Init){6, 0, 5, 15, 0});
        Disp_Score_Buff[0] = Ball_Perfect_PTS[0][Bonus_Stage_Level];
        effect_14_init(0, 35, 15, 15);
        break;

    case 2:
        effect_08_init(&(Effect08Init){6, 0, 5, 26, 1});
        Disp_Score_Buff[1] = Ball_Perfect_PTS[1][Bonus_Stage_Level];
        effect_14_init(1, 35, 15, 26);
        break;

    case 3:
        effect_08_init(&(Effect08Init){6, 0, 5, 15, 0});
        Disp_Score_Buff[0] = Ball_Perfect_PTS[0][Bonus_Stage_Level];
        effect_14_init(0, 35, 15, 15);
        effect_08_init(&(Effect08Init){6, 0, 9, 26, 1});
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
        return ball_stage_final_score();
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
            effect_08_init(&(Effect08Init){7, 0, 1, 15, 0});
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

