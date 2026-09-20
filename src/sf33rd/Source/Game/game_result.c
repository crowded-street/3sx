/**
 * @file game_result.c
 * Game flow
 * After the fight: the result, the bonus stage and the continue.
 */

#include "sf33rd/Source/Game/game.h"
#include "common.h"
#include "main.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/demo/demo00.h"
#include "sf33rd/Source/Game/demo/demo01.h"
#include "sf33rd/Source/Game/demo/demo02.h"
#include "sf33rd/Source/Game/effect/eff35.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effj2.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/engine/bbbscom.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/manage.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plcnt2.h"
#include "sf33rd/Source/Game/engine/plcnt3.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/spgauge.h"
#include "sf33rd/Source/Game/engine/stun.h"
#include "sf33rd/Source/Game/engine/vital.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/opening/op_sub.h"
#include "sf33rd/Source/Game/opening/opening.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/screen/continue.h"
#include "sf33rd/Source/Game/screen/entry.h"
#include "sf33rd/Source/Game/screen/gameover.h"
#include "sf33rd/Source/Game/screen/next_cpu.h"
#include "sf33rd/Source/Game/screen/ranking.h"
#include "sf33rd/Source/Game/screen/sel_pl.h"
#include "sf33rd/Source/Game/screen/win.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/stage/tate00.h"
#include "sf33rd/Source/Game/system/reset.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/count.h"
#include "sf33rd/Source/Game/ui/flash_lp.h"
#include "sf33rd/Source/Game/ui/input_history.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/PS2/mc/savesub.h"
#include "structs.h"
#include "sf33rd/Source/Game/game_internal.h"

static s16 Bonus_Sub();


void Game07() {
    BG_Draw_System();
    Basic_Sub();

    switch (G_No[2]) {
    case 0:
        if (Continue_Scene() != 0) {
            G_No[1] = 6;
            G_No[2] = 0;
        }

        break;
    }

    BG_move();
}

/* The ending has finished playing and the fade has been accepted. Copied
 * character for character from the test it stood in - both operands call, and
 * the `&&` still short-circuits here exactly as it did there. */
static s32 ending_finished_and_faded() {
    return Ending_main(End_PL) && (Request_Fade(9) != 0);
}

/* The ending's set-up and its teardown, each exactly the block that stood under
 * its case label. */
static void begin_ending() {
Switch_Screen(0);
G_No[2] = 1;
Game_pause = 0;
Final_Result_id = WINNER;
WGJ_Target = WINNER;
WGJ_Win = Win_Record[WINNER];
grade_final_grade_bonus();
WGJ_Score = Continue_Coin[WINNER] + Score[WINNER][0];
Purge_mmtm_area(6);
cpExitTask(TASK_MENU);
cpExitTask(TASK_PAUSE);
}

static void finish_ending() {
G_No[1] = 6;
G_No[2] = 0;
E_No[0] = 8;
E_No[1] = 0;
E_No[2] = 0;
E_No[3] = 0;
Clear_Personal_Data(0);
Clear_Personal_Data(1);
plw[0].wu.operator = 0;
plw[1].wu.operator = 0;
Operator_Status[0] = 0;
Operator_Status[1] = 0;
Last_Player_id = Player_Number = -1;
Purge_mmtm_area(6);
System_all_clear_Level_B();
}

void Game08() {
    BG_Draw_System();

    switch (G_No[2]) {
    case 0:
        begin_ending();
        break;

    case 1:
        if (ending_finished_and_faded()) {
            G_No[2] += 1;
        }

        break;

    case 2:
        if (Check_Fade_Complete_SP() != 0) {
            G_No[2] += 1;
            G_Timer = 10;
            Suicide[4] = 1;
        }

        break;

    case 3:
        if (--G_Timer == 0) {
            finish_ending();
        }

        break;
    }

    move_effect_work(4);
}

/* The bonus game's set-up, and the stage load it waits on. Both are exactly the
 * blocks that stood under their case labels. */
static void begin_bonus_game() {
BG_Draw_System();
Switch_Screen(0);
System_all_clear_Level_B();
Bonus_Game_Flag = Bonus_Type;
Game_difficulty = 15;
Game_timer = 0;
Game_pause = 0;
Demo_Time_Stop = 0;
C_No[0] = 0;
C_No[1] = 0;
C_No[2] = 0;
C_No[3] = 0;
G_No[2] += 1;
G_Timer = 19;
Round_num = 0;
Allow_a_battle_f = 0;
Time_in_Time = 60;
init_slow_flag();
clear_hit_queue();
pcon_rno[0] = pcon_rno[1] = pcon_rno[2] = pcon_rno[3] = 0;
bbbs_com_initialize();
ca_check_flag = 1;
Bonus_Game_Work = 20;
Bonus_Game_result = 0;
Bonus_Game_ex_result = 0;
bg_work_clear();
win_lose_work_clear();

if (Bonus_Game_Flag == 0x15) {
    My_char[COM_id] = 12;
} else {
    My_char[COM_id] = My_char[Player_id];
}
}

static void load_bonus_stage() {
if (!Check_LDREQ_Queue_BG(bg_w.stage)) {
    G_Timer = 1;
} else {
    G_No[2] += 1;
    Clear_Flash_No();

    if (Bonus_Type == 0x15) {
        makeup_bonus_game_level(COM_id);
        effect_35_init(0x3C, 5);
        effect_J2_init(0x78);
        effect_35_init(0xB4, 7);
        effect_58_init(6, 0xB4, 0xA1);
    } else {
        effect_35_init(0x3C, 6);
        effect_35_init(0x78, 7);
        effect_58_init(6, 0x78, 0xA1);
    }

    TATE00();
    Switch_Screen_Init(0);
    Bonus_Sub();
}
}

/* Game09 from its fourth step on, reached from the first three's default. The
 * case labels are the original ones and the switch is on the same expression;
 * the group carries the original `default`, so a step matching none of the
 * labels still reaches it. */
static void game09_from_step_3() {
    switch (G_No[2]) {
    case 3:
        if (Bonus_Sub()) {
            G_No[2] += 1;
            Cover_Timer = 24;
            Stop_Combo = 1;
            Switch_Screen_Init(0);
        }

        break;

    case 4:
        Bonus_Sub();

        if (Switch_Screen(0) != 0) {
            G_No[2] += 1;
            G_Timer = 3;
            SE_All_Off();
            Clear_Flash_No();
            effect_work_kill_mod_plcol();
        }

        break;

    default:
        Switch_Screen(0);
        Bonus_Sub();

        if (--G_Timer == 0) {
            Cover_Timer = 24;
            Suicide[0] = 1;
            System_all_clear_Level_B();
            G_No[1] = 10;
            G_No[2] = 0;
            G_No[3] = 0;
            E_No[0] = 9;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;
        }

        break;
    }
}

void Game09() {
    switch (G_No[2]) {
    case 0:
        begin_bonus_game();

        break;

    case 1:
        BG_Draw_System();
        Switch_Screen(1);

        if (--G_Timer == 0) {
            load_bonus_stage();
        }

        break;

    case 2:
        Bonus_Sub();

        if (Switch_Screen_Revival(1) != 0) {
            G_No[2] += 1;
            Forbid_Break = 0;
        }

        break;

    default:
        game09_from_step_3();
        break;
    }

    BG_move();
}

s16 Bonus_Sub() {
    s16 x;

    mpp_w.inGame = true;
    Scene_Cut = Cut_Cut_Cut();
    Bonus_Game_Complete = 0;

    if (Game_pause != 0x81) {
        Game_timer += 1;
    }

    set_EXE_flag();
    Time_Control();

    if (Bonus_Type == 0x15) {
        Bonus_Game_Complete = Player_control_bonus();
    } else {
        Bonus_Game_Complete = Player_control_bonus2();
    }

    TATE00();
    x = 0;
    x = Game_Management();
    BG_Draw_System();
    reqPlayerDraw();
    Basic_Sub_Ex();
    hit_check_main_process();
    return x;
}

void Game10() {
    BG_Draw_System();
    Basic_Sub();
    Setup_Play_Type();

    switch (G_No[2]) {
    case 0:
        Switch_Screen(0);
        G_No[2] += 1;
        SC_No[0] = 0;
        SC_No[1] = 0;
        SC_No[2] = 0;
        SC_No[3] = 0;
        Stop_Combo = 0;
        init_slow_flag();
        break;

    case 1:
        if (After_Bonus() != 0) {
            G_No[2] += 1;
            Switch_Screen_Init(0);
        }

        break;

    default:
        After_Bonus();

        if (Switch_Screen(0) != 0) {
            Cover_Timer = 24;
            Game01_Sub();
            BGM_Stop();
            G_No[1] = 2;
            G_No[2] = 0;
            E_No[0] = 4;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;
            Bonus_Game_Flag = 0;
            Purge_texcash_of_list(3);
            Make_texcash_of_list(3);
        }

        break;
    }

    BG_move();
}

/* Once the results screen has switched away: reload the stage's textures and go
 * to the next fight, or to the bonus stage. */
static void game11_screen_switched() {
    Cover_Timer = 24;
    Game01_Sub();
    BGM_Stop();
    Purge_texcash_of_list(3);
    Make_texcash_of_list(3);

    if (Bonus_Type == 0) {
        G_No[1] = 2;
        G_No[2] = 0;
        E_No[0] = 4;
        E_No[1] = 0;
        E_No[2] = 0;
        E_No[3] = 0;
        Bonus_Game_Flag = 0;
    } else {
        G_No[1] = 9;
        G_No[2] = 0;
        G_No[3] = 0;
        E_No[0] = 4;
        E_No[1] = 0;
        E_No[2] = 0;
        E_No[3] = 0;
    }
}

/* Game11 from its fourth step on, reached from the first three's default. The
 * case labels are the original ones and the switch is on the same expression,
 * and neither switch carries a `default`. */
static void game11_from_step_3() {
    switch (G_No[2]) {
    case 3:
        G_No[2] += 1;
        SC_No[0] = 0;
        SC_No[1] = 0;
        SC_No[2] = 0;
        SC_No[3] = 0;
        Stop_Combo = 0;
        Bonus_Type = 0;
        init_slow_flag();
        Switch_Screen_Init(0);
        break;

    case 4:
        if (Switch_Screen(0) != 0) {
            G_No[2] = 1;
            Cover_Timer = 24;
        }

        break;
    }
}

void Game11() {
    BG_Draw_System();
    Basic_Sub();
    Setup_Play_Type();

    switch (G_No[2]) {
    case 0:
        Switch_Screen(0);
        G_No[2] += 1;
        SC_No[0] = 0;
        SC_No[1] = 0;
        SC_No[2] = 0;
        SC_No[3] = 0;
        Stop_Combo = 0;
        Bonus_Type = 0;
        init_slow_flag();
        break;

    case 1:
        if (Next_Q()) {
            G_No[2] += 1;
            Switch_Screen_Init(0);
        }

        break;

    case 2:
        Next_Q();

        if (Switch_Screen(0) != 0) {
            game11_screen_switched();
        }

        break;

    default:
        game11_from_step_3();
        break;
    }

    BG_move();
}
