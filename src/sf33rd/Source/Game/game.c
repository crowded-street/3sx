/**
 * @file game.c
 * Game flow
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
static void Set_Appear_Type_For_Mode() {
    appear_type = Is_Training_Mode(Mode_Type) ? APPEAR_TYPE_NON_ANIMATED : APPEAR_TYPE_ANIMATED;
}

void Game_Task(struct _TASK* task_ptr) {
    s16 ix;

    void (*Main_Jmp_Tbl[3])(struct _TASK*) = { Wait_Auto_Load, Loop_Demo, Game };

    if (!No_Trans) {
        init_color_trans_req();
    }

    Play_Game = 0;

    if (Game_pause != 0x81) {
        system_timer += 1;
    }

    init_texcash_before_process();
    seqsBeforeProcess();

    if (nowSoftReset() == 0) {
        Main_Jmp_Tbl[G_No[0]](task_ptr);
    }

    seqsAfterProcess();
    texture_cash_update();
    move_pulpul_work();
    Check_LDREQ_Queue();
}

void Game() {
    void (*Game_Jmp_Tbl[13])() = { Game00, Game01, Game02, Game03, Game04, Game05, Game06,
                                   Game07, Game08, Game09, Game10, Game11, Game12 };

    if (G_No[1] == 2 || G_No[1] == 9) {
        Play_Game = 1;
    } else if (G_No[1] == 8) {
        Play_Game = 2;
    }

    Game_Jmp_Tbl[G_No[1]]();
}

void Game00() {
    void (*Game00_Jmp_Tbl[3])() = { Game0_0, Game0_1, Game0_2 };

    Game00_Jmp_Tbl[G_No[2]]();
    njSetBackColor(0, 0, 0);
    BG_Draw_System();
    Basic_Sub();
    Check_Back_Demo();
}

void Game0_0() {
    if (Title_At_a_Dash() != 0) {
        G_No[2] += 1;
    }
}

void Game0_1() {
    Disp_Copyright();
    TITLE_Move(1);

    if (Request_G_No) {
        G_No[2] += 1;
    }
}

void Game0_2() {
    switch (G_No[3]) {
    case 0:
        Disp_Copyright();
        TITLE_Move(1);
        G_No[3] += 1;
        Switch_Screen_Init(1);
        break;

    case 1:
        if (Switch_Screen(1) != 0) {
            G_No[3] += 1;
            Cover_Timer = 23;
            return;
        }

        TITLE_Move(1);
        Disp_Copyright();
        break;

    case 2:
        FadeOut(1, 0xFF, 8);
        G_No[3] += 1;
        break;

    case 3:
        FadeOut(1, 0xFF, 8);
        G_No[3] += 1;
        TexRelease(601);
        title_tex_flag = 0;
        break;

    case 4:
        FadeOut(1, 0xFF, 8);
        G_No[3] += 1;
        Purge_mmtm_area(2);
        Make_texcash_of_list(2);
        break;

    case 5:
        FadeOut(1, 0xFF, 8);
        BGM_Request(65);
        G_No[1] = 0xC;
        G_No[2] = 0;
        G_No[3] = 0;
        cpReadyTask(TASK_MENU, Menu_Task);
        break;
    }
}

/* The attract demo does not cut in during the ranking screen, nor once the
 * result has reached its third step. */
static s32 back_demo_is_barred(void) {
    return G_No[1] == 12 || (G_No[2] == 2 && G_No[3] >= 2);
}

void Check_Back_Demo() {
    if (++G_Timer < 1800) {
        return;
    }

    if (back_demo_is_barred()) {
        return;
    }

    TexRelease(601);
    title_tex_flag = 0;
    Next_Demo_Loop();
    effect_work_init();
}

/// Screen transition to character select
void Game12() {
    void (*Game12_Jmp_Tbl[3])() = { Game12_0, Game12_1, Game12_2 };

    Game12_Jmp_Tbl[G_No[2]]();
    BG_Draw_System();
    Basic_Sub();
    bg_pos_hosei_sub2(0);
    bg_pos_hosei_sub2(1);
    bg_pos_hosei_sub2(2);
    Bg_Family_Set_appoint(0);
    Bg_Family_Set_appoint(1);
    Bg_Family_Set_appoint(2);
    BG_move_Ex(0);
}

void Game12_0() {
    // Do nothing
}

void Game12_1() {
    G_No[2] += 1;
    Switch_Screen_Init(1);
    SsBgmFadeOut(0x1000);
}

void Game12_2() {
    if (!Switch_Screen(1)) {
        // Transition is still running, can't proceed
        return;
    }

    // Proceed to character select
    G_No[1] = 1;
    G_No[2] = 0;
    G_No[3] = 0;
    Control_Time = 481;
    Cover_Timer = 23;
    effect_work_init();
    cpExitTask(TASK_MENU);
}

/// Character select
void Game01() {
    BG_Draw_System();
    Basic_Sub();
    Setup_Play_Type();

    switch (G_No[2]) {
    case 0:
        Switch_Screen(1);
        G_No[2] += 1;
        S_No[0] = 0;
        S_No[1] = 0;
        S_No[2] = 0;
        S_No[3] = 0;
        SsBgmHalfVolume(0);

        if (Mode_Type == MODE_ARCADE) {
            BGM_Request(53);
        } else {
            BGM_Request(66);
        }

        Break_Into = 0;
        Stop_Combo = 0;

        if (Mode_Type != MODE_NETWORK) {
            Random_ix32 = Interrupt_Timer;
            Random_ix32_ex = Interrupt_Timer;
        } else {
            Setup_Net_Random_ix();
            All_Clear_Timer();
        }

        init_slow_flag();
        System_all_clear_Level_B();
        pulpul_stop();
        init_pulpul_work();
        break;

    case 1:
        Switch_Screen(1);
        G_No[2] += 1;
        break;

    case 2:
        if (Select_Player()) {
            G_No[2] += 1;
            Bonus_Game_Flag = 0;
            Switch_Screen_Init(0);
        }

        break;

    default:
        Select_Player();

        if (Switch_Screen(0) != 0) {
            Game01_Sub();
            Cover_Timer = 24;
            Set_Appear_Type_For_Mode();
            set_hitmark_color();

#if DEBUG
            if (debug_config.character_override[0]) {
                My_char[0] = debug_config.character_override[0] - 1;
            }

            if (debug_config.character_override[1]) {
                My_char[1] = debug_config.character_override[1] - 1;
            }
#endif

            Purge_texcash_of_list(3);
            Make_texcash_of_list(3);

            if (Demo_Flag) {
                G_No[1] = 2;
                G_No[2] = 0;
                G_No[3] = 0;
                E_No[0] = 4;
                E_No[1] = 0;
                E_No[2] = 0;
                E_No[3] = 0;
            } else {
                Demo_Time_Stop = 1;
                plw[0].wu.operator = 0;
                Operator_Status[0] = 0;
                plw[1].wu.operator = 0;
                Operator_Status[1] = 0;
            }

            if (plw[0].wu.operator != 0) {
                Sel_Arts_Complete[0] = -1;
            }

            if (plw[1].wu.operator != 0) {
                Sel_Arts_Complete[1] = -1;
            }

            if ((plw[0].wu.operator != 0) && (plw[1].wu.operator != 0)) {
                Play_Type = 1;
            } else {
                Play_Type = 0;
            }
        }

        break;
    }

    BG_move();
}

void Game02() {
    void (*Game02_Jmp_Tbl[8])() = { Game2_0, Game2_1, Game2_2, Game2_3, Game2_4, Game2_5, Game2_6, Game2_7 };

    Scene_Cut = Cut_Cut_Cut();
    Game02_Jmp_Tbl[G_No[2]]();
    BG_move_Ex(3);
}

void Game2_0() {
    s16 ix;

    BG_Draw_System();
    Switch_Screen(0);

    if (Check_LDREQ_Clear() == 0) {
        fatal_error("Load queue failed to drain in time");
    }

    System_all_clear_Level_B();

    switch (Mode_Type) {
    case MODE_ARCADE:
        Play_Mode = 0;
        Replay_Status[0] = 0;
        Replay_Status[1] = 0;
        break;

    case MODE_VERSUS:
        for (ix = 0; ix < 2; ix++) {
            if (save_w[1].Partner_Type[ix]) {
                plw[ix].wu.operator = 0;
                Operator_Status[ix] = 0;
            }
        }

        cpExitTask(TASK_ENTRY);
        /* fallthrough */

    case MODE_NETWORK:
        Play_Mode = 1;
        All_Clear_Random_ix();
        All_Clear_Timer();
        All_Clear_ETC();
        break;

    case MODE_REPLAY:
        Play_Mode = 3;
        All_Clear_Timer();
        break;

    default:
        // Do nothing
        break;
    }

    Check_Replay();

    if (Demo_Flag == 0) {
        Play_Mode = 0;
        Replay_Status[0] = 0;
        Replay_Status[1] = 0;
    }

    Game_difficulty = 15;
    Game_timer = 0;
    Game_pause = 0;
    Demo_Time_Stop = 0;
    C_No[0] = 0;
    C_No[1] = 0;
    C_No[2] = 0;
    C_No[3] = 0;
    G_No[2] = 3;
    G_Timer = 10;
    Round_num = 0;
    Keep_Grade[0] = 0;
    Keep_Grade[1] = 0;

    if (Win_Record[0]) {
        Keep_Grade[0] = grade_get_my_grade(0) + 1;
    }

    if (Win_Record[1]) {
        Keep_Grade[1] = grade_get_my_grade(1) + 1;
    }

    Allow_a_battle_f = 0;
    Time_in_Time = 60;
    init_slow_flag();
    clear_hit_queue();
    pcon_rno[0] = pcon_rno[1] = pcon_rno[2] = pcon_rno[3] = 0;
    ca_check_flag = 1;
    bg_work_clear();
    win_lose_work_clear();
    player_face_init();
    TATE00();
}

static bool should_render_input_history() {
    return Mode_Type == MODE_NORMAL_TRAINING && Training[0].contents[0][1][TRAINING_OPTION_INPUT_HISTORY];
}

/// Main gameplay routine
void Game2_1() {
    mpp_w.inGame = true;

    if (Game_pause != 0x81) {
        Game_timer += 1;
    }

    set_EXE_flag();
    ppgPurgeFromVRAM(5);

    if (Disp_Cockpit) {
        Time_Control();
    }

    Player_control();

    if (Disp_Cockpit) {
        vital_cont_main();
        combo_cont_main();
    }

    TATE00();
    Game_Management();
    BG_Draw_System();
    ppgPurgeFromVRAM(4);
    reqPlayerDraw();
    Basic_Sub_Ex();

    if (Disp_Cockpit) {
        player_face();
        player_name();
        stngauge_cont_main();
        spgauge_cont_main();
        Sa_frame_Write();
        Score_Sub();
        Flash_Lamp();
        Disp_Win_Record();

        if (should_render_input_history()) {
            InputHistory_Render();
        }
    }

    ppgPurgeFromVRAM(0);
    hit_check_main_process();
}

void Game2_2() {
    s16 i;

    BG_Draw_System();
    Switch_Screen(0);

    if (Check_LDREQ_Clear() == 0) {
        fatal_error("Load queue failed to drain in time");
    }

    SsBgmHalfVolume(0);
    All_Clear_Timer();
    Check_Replay();
    Game_difficulty = 15;
    Game_timer = 0;
    Game_pause = 0;
    Demo_Time_Stop = 0;
    C_No[0] = 0;
    C_No[1] = 0;
    C_No[2] = 0;
    C_No[3] = 0;
    G_Timer = 10;
    Round_num = 0;
    Keep_Grade[0] = 0;
    Keep_Grade[1] = 0;

    if (Win_Record[0]) {
        Keep_Grade[0] = grade_get_my_grade(0) + 1;
    }

    if (Win_Record[1]) {
        Keep_Grade[1] = grade_get_my_grade(1) + 1;
    }

    Allow_a_battle_f = 0;
    Time_in_Time = 60;
    init_slow_flag();
    effect_work_quick_init();
    clear_hit_queue();
    pcon_rno[0] = pcon_rno[1] = pcon_rno[2] = pcon_rno[3] = 0;
    ca_check_flag = 1;
    bg_work_clear();
    win_lose_work_clear();
    player_face_init();
    Game01_Sub();
    Set_Appear_Type_For_Mode();
    TATE00();

    for (i = 0; i < 3; i++) {
        if (stage_bgw_number[bg_w.stage][i] > 0) {
            Bg_On_R(1 << i);
        }
    }

    if (bg_w.stage == 7) {
        Bg_On_R(4);
    }

    G_No[2] = 7;
}

void Game2_3() { // 🟢
    Game2_1();

    if (--G_Timer == 0) {
        G_No[2] = 1;
        Clear_Flash_No();
    }
}

void Game2_4() {
    BG_Draw_System();
}

/// Rounds 2, 3, ... routine
void Game2_5() {
    BG_Draw_System();

    switch (G_No[3]) {
    case 0:
        Switch_Screen(0);
        G_No[3] += 1;
        Stop_Update_Score = 0;
        HUD_Shift_Init();
        vital_cont_init();
        count_cont_init(0);
        stngauge_cont_init();
        stngauge_work_clear();
        combo_cont_init();
        count_cont_init(1);
        Score[0][2] = 0;
        Score[1][2] = 0;
        Suicide[0] = 1;
        Game_pause = 0;
        pcon_rno[0] = 0;
        pcon_rno[1] = 0;
        pcon_rno[2] = 0;
        pcon_rno[3] = 0;
        appear_type = APPEAR_TYPE_NON_ANIMATED;
        erase_extra_plef_work();
        compel_bg_init_position();
        win_lose_work_clear();
        TATE00();
        break;

    default:
        Game2_1();

        if (--G_Timer == 0) {
            G_No[2] = 1;
            Clear_Flash_No();
        }

        break;
    }
}

void Game2_6() {
    BG_Draw_System();
    Switch_Screen(0);

    if (Wait_Seek_Time() != 0) {
        G_No[2] = 3;
        TATE00();
    }
}

void Game2_7() {
    BG_Draw_System();
    Switch_Screen(0);

    if (Wait_Seek_Time() != 0) {
        G_No[2] = 3;
    }
}

void Game01_Sub() {
    Disp_Cockpit = 0;
    Stop_Update_Score = 0;
    HUD_Shift_Init();
    vital_cont_init();
    count_cont_init(0);
    Score[0][1] = 0;
    Score[0][2] = 0;
    Score[1][1] = 0;
    Score[1][2] = 0;
    PL_Wins[0] = 0;
    PL_Wins[1] = 0;
    combo_cont_init();
    Clear_Win_Type();
    Lamp_No = 0;
    set_kizetsu_status(0);
    set_kizetsu_status(1);
    set_super_arts_status(0);
    set_super_arts_status(1);

    if (Demo_Flag && (sag_ikinari_max() != 0)) {
        spgauge_cont_init();
    } else {
        spgauge_cont_demo_init();
    }

    stngauge_cont_init();
}

