#include "sf33rd/Source/Game/Game.h"
#include "common.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/BBBSCOM.h"
#include "sf33rd/Source/Game/Continue.h"
#include "sf33rd/Source/Game/DC_Ghost.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/Entry.h"
#include "sf33rd/Source/Game/Flash_LP.h"
#include "sf33rd/Source/Game/GameOver.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/HITCHECK.h"
#include "sf33rd/Source/Game/MMTMCNT.h"
#include "sf33rd/Source/Game/MTRANS.h"
#include "sf33rd/Source/Game/Manage.h"
#include "sf33rd/Source/Game/Next_CPU.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLCNT2.h"
#include "sf33rd/Source/Game/PLCNT3.h"
#include "sf33rd/Source/Game/RANKING.h"
#include "sf33rd/Source/Game/Reset.h"
#include "sf33rd/Source/Game/SE.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/TATE00.h"
#include "sf33rd/Source/Game/VITAL.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/Win.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/cmb_win.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/count.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect_init.h"
#include "sf33rd/Source/Game/ending/end_main.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/sel_pl.h"
#include "sf33rd/Source/Game/spgauge.h"
#include "sf33rd/Source/Game/stun.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

#include "sf33rd/Source/Game/demo/demo00.h"
#include "sf33rd/Source/Game/demo/demo01.h"
#include "sf33rd/Source/Game/demo/demo02.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/opening/op_sub.h"
#include "sf33rd/Source/Game/opening/opening.h"

void Wait_Auto_Load(struct _TASK* /* unused */);
void Loop_Demo(struct _TASK* /* unused */);
void Game();
void Game00();
void Game01();
void Game02();
void Game03();
void Game04();
void Game05();
void Game06();
void Game07();
void Game08();
void Game09();
void Game10();
void Game11();
void Game12();
void Check_Back_Demo();
void Game0_0();
void Game0_1();
void Game0_2();
void Next_Demo_Loop();
void Game12_0();
void Game12_1();
void Game12_2();
void Game2_0();
void Game2_1();
void Game2_2();
void Game2_3();
void Game2_4();
void Game2_5();
void Game2_6();
void Game2_7();
void Time_Control();
static s32 Check_Disp_Ranking();
s32 Disp_Ranking();
void Request_Break_Sub(s16 PL_id);
s16 Disp_Rank_Sub(s16 PL_id);
static s16 Bonus_Sub();
s16 Ck_Coin();
void Loop_Demo_Sub();
void Before_Select_Sub();

void Game_Task(struct _TASK* task_ptr) {
    s16 ix;
    s16 ff;

    void (*Main_Jmp_Tbl[3])(struct _TASK*) = { Wait_Auto_Load, Loop_Demo, Game };

    init_color_trans_req();
    ff = Process_Counter;

    if ((Usage == 7) && !Turbo) {
        ff = sysFF;
    }

    for (ix = 0; ix < ff; ix++) {
        if (ix == ff - 1) {
            No_Trans = 0;

            if (Turbo != 0 && (Process_Counter > 1) && (Turbo_Timer != 5)) {
                Play_Game = 0;
                break;
            }
        } else {
            No_Trans = 1;
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
        Check_Off_Vib();
        Check_LDREQ_Queue();
    }

    Check_Check_Screen();
    Check_Pos_BG();
    Disp_Sound_Code();
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
        cpReadyTask(MENU_TASK_NUM, Menu_Task);
        break;
    }
}

void Check_Back_Demo() {
    if (++G_Timer < 1800) {
        return;
    }

    if (G_No[1] == 12 || (G_No[2] == 2 && G_No[3] >= 2)) {
        return;
    }

    TexRelease(0x259);
    title_tex_flag = 0;
    Next_Demo_Loop();
    effect_work_init();
}

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
    if (Switch_Screen(1) != 0) {
        G_No[1] = 1;
        G_No[2] = 0;
        G_No[3] = 0;
        Control_Time = 481;
        Cover_Timer = 23;
        effect_work_init();
        cpExitTask(3);
    }
}

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

        if (Mode_Type == 0) {
            BGM_Request(53);
        } else {
            BGM_Request(66);
        }

        Break_Into = 0;
        Stop_Combo = 0;

        if (Mode_Type != 2) {
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
            Cover_Timer = 5;
            appear_type = 1;
            set_hitmark_color();

            if (Debug_w[0x1D]) {
                My_char[0] = Debug_w[0x1D] - 1;
            }

            if (Debug_w[0x1E]) {
                My_char[1] = Debug_w[0x1E] - 1;
            }

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
        return;
    }

    System_all_clear_Level_B();

    switch (Mode_Type) {
    case 0:
        Play_Mode = 0;
        Replay_Status[0] = 0;
        Replay_Status[1] = 0;
        break;

    case 1:
        for (ix = 0; ix < 2; ix++) {
            if (save_w[1].Partner_Type[ix]) {
                plw[ix].wu.operator = 0;
                Operator_Status[ix] = 0;
            }
        }

        cpExitTask(1);
        /* fallthrough */

    case 2:
        Play_Mode = 1;
        All_Clear_Random_ix();
        All_Clear_Timer();
        All_Clear_ETC();
        break;

    case 5:
        Play_Mode = 3;
        All_Clear_Timer();
        break;
    }

    Check_Replay();

    if (Demo_Flag == 0) {
        Play_Mode = 0;
        Replay_Status[0] = 0;
        Replay_Status[1] = 0;
    }

    Game_difficulty = 15;
    Game_pause = 0;
    Demo_Time_Stop = 0;
    C_No[0] = 0;
    C_No[1] = 0;
    C_No[2] = 0;
    C_No[3] = 0;
    G_No[2] = 6;
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
    setup_pos_remake_key(3);
}

void Game2_1() {
    mpp_w.inGame = 1;

    if (Game_pause != 0x81) {
        Game_timer += 1;
    }

    set_EXE_flag();
    ppgPurgeFromVRAM(5);
    Player_control();
    TATE00();
    Game_Management();
    BG_Draw_System();
    ppgPurgeFromVRAM(4);
    reqPlayerDraw();
    Basic_Sub_Ex();

    if (Disp_Cockpit) {
        Time_Control();
        vital_cont_main();
        player_face();
        player_name();
        combo_cont_main();
        stngauge_cont_main();
        spgauge_cont_main();
        Sa_frame_Write();
        Score_Sub();
        Flash_Lamp();
        Disp_Win_Record();
    }

    ppgPurgeFromVRAM(0);
    hit_check_main_process();
}

void Game2_2() {
#if defined(TARGET_PS2)
    void Bg_On_R(s32 s_prm);
#endif

    s16 i;

    BG_Draw_System();
    Switch_Screen(0);

    if (Check_LDREQ_Clear() == 0) {
        return;
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
    appear_type = 1;
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

void Game2_3() {
    Game2_1();

    if (--G_Timer == 0) {
        G_No[2] = 1;
        Clear_Flash_No();
    }
}

void Game2_4() {
    BG_Draw_System();
}

void Game2_5() {
    BG_Draw_System();

    switch (G_No[3]) {
    case 0:
        Switch_Screen(0);
        G_No[3] += 1;
        Stop_Update_Score = 0;
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
        appear_type = 0;
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

void Game03() {
    BG_Draw_System();
    move_effect_work(4);
    move_effect_work(5);
    Play_Mode = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;

    switch (G_No[2]) {
    case 0:
        if (Winner_Scene() != 0) {
            switch (Mode_Type) {
            case 1:
                G_No[2] += 1;
                Rep_Game_Infor[10].play_type = 1;
                Rep_Game_Infor[10].winner = Winner_id;
                Switch_Screen_Init(0);

                if (Country == 3) {
                    if (Screen_PAL == 8) {
                        Rep_Game_Infor[10].play_type = 3;
                    } else {
                        Rep_Game_Infor[10].play_type = 4;
                    }
                }

                break;

            case 2:
                G_No[2] = 3;
                Rep_Game_Infor[10].play_type = 2;
                Rep_Game_Infor[10].winner = Winner_id;
                Champion = Winner_id;
                New_Challenger = Loser_id;
                Switch_Screen_Init(0);
                break;

            case 5:
                G_No[2] = 5;
                cpReadyTask(MENU_TASK_NUM, Menu_Task);
                task[3].r_no[0] = 8;
                break;

            default:
                G_No[1] = 5;
                G_No[2] = 0;
                G_No[3] = 0;
                E_No[0] = 9;
                E_No[1] = 0;
                E_No[2] = 0;
                E_No[3] = 0;

                if (Battle_Q[WINNER]) {
                    G_No[1] = 0xB;
                    G_No[2] = 3;
                    G_No[3] = 0;
                }

                Cover_Timer = 24;

                if (Round_Operator[LOSER]) {
                    E_Number[LOSER][0] = 1;
                    E_Number[LOSER][1] = 0;
                    E_Number[LOSER][2] = 0;
                    E_Number[LOSER][3] = 0;
                }

                break;
            }
        }

        break;

    case 1:
        if (Switch_Screen(1) != 0) {
            G_No[2] += 1;
            E_No[0] = 1;
            E_No[1] = 2;
            E_No[2] = 2;
            E_No[3] = 0;
            Request_E_No = 0;
            cpReadyTask(MENU_TASK_NUM, Menu_Task);
            task[3].r_no[1] = 16;
            Cursor_Y_Pos[0][0] = 0;
            Cursor_Y_Pos[1][0] = 0;
            G_Timer = 4;
        }

        break;

    case 2:
        Switch_Screen(1);

        if (--G_Timer == 0) {
            Cover_Timer = 10;
            G_No[1] = 12;
            G_No[2] = 0;
            G_No[3] = 0;
        }

        break;

    case 3:
        if (Switch_Screen(1) != 0) {
            G_No[2] += 1;
            task[7].r_no[0] = 1;
            G_Timer = 4;
        }

        break;

    case 4:
        Switch_Screen(1);

        if (--G_Timer == 0) {
            // Do nothing
        }

        break;

    case 5:
        // Do nothing
        break;
    }

    BG_move();
}

void Game04() {
    s16 i;

    BG_Draw_System();
    move_effect_work(4);
    move_effect_work(5);

    switch (G_No[2]) {
    case 0:
        if (Loser_Scene() != 0) {
            if (Mode_Type == 5) {
                G_No[2] = 5;
                cpReadyTask(MENU_TASK_NUM, Menu_Task);
                task[3].r_no[0] = 8;
            } else {
                G_No[1] = 7;
                G_No[2] = 0;
                G_No[3] = 0;
                E_No[0] = 7;
                Cont_No[0] = 0;
                E_Number[LOSER][0] = 1;

                for (i = 1; i < 4; i++) {
                    E_No[i] = 0;
                    Cont_No[i] = 0;
                    E_Number[LOSER][i] = 0;
                }
            }
        }

        break;

    default:
        // Do nothing
        break;
    }

    BG_move();
}

void Game05() {
    BG_Draw_System();
    Basic_Sub();
    Setup_Play_Type();

    switch (G_No[2]) {
    case 0:
        G_No[2] += 1;
        SC_No[0] = 0;
        SC_No[1] = 0;
        SC_No[2] = 0;
        SC_No[3] = 0;

        if (Check_Bonus_Stage()) {
            SC_No[0] = 6;
        }

        Stop_Combo = 0;
        init_slow_flag();
        pulpul_stop();
        break;

    case 1:
        if (Next_CPU()) {
            G_No[2] += 1;
            Switch_Screen_Init(0);
        }

        break;

    default:
        Next_CPU();

        if (Switch_Screen(0) != 0) {
            Cover_Timer = 24;
            Purge_texcash_of_list(3);
            Make_texcash_of_list(3);

            if (Bonus_Type == 0) {
                Game01_Sub();
            }

            BGM_Stop();

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

        break;
    }

    BG_move();
}

void Game06() {
    s16 xx;

    BG_Draw_System();
    Basic_Sub_Ex();

    if (!Break_Into) {
        switch (G_No[2]) {
        case 0:
            G_No[2] += 1;
            Game_pause = 0;
            Stock_Com_Color[Player_id] = -1;
            Stock_Com_Arts[Player_id] = -1;
            Last_Player_id = -1;
            Control_Time = 481;
            E_No[0] = 8;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;

            for (xx = 0; xx < 4; xx++) {
                GO_No[xx] = 0;
            }

            make_texcash_work(13);
            break;

        case 1:
            if (Game_Over()) {
                G_Timer = 60;

                if (Check_Disp_Ranking() != 0) {
                    G_No[2] += 1;
                } else {
                    G_No[2] = 3;
                }
            }

            break;

        case 2:
            if (Disp_Ranking() != 0) {
                G_No[2] += 1;
                G_Timer = 1;
            }

            break;

        case 3:
            if (--G_Timer == 0) {
                G_No[2] += 1;
                Clear_Disp_Ranking(0);
                Clear_Disp_Ranking(1);
                Switch_Screen_Init(1);
            }

            break;

        case 4:
            if (Switch_Screen(1) != 0) {
                Cover_Timer = 24;
                Forbid_Break = 0;
                Clear_Flash_No();
                Clear_Personal_Data(LOSER);
                grade_check_work_1st_init(LOSER, 0);
                grade_check_work_1st_init(LOSER, 1);

                if (Request_Break[0] != 0 || Request_Break[1] != 0) {
                    Request_Break_Sub(0);
                    Request_Break_Sub(1);
                    G_No[1] = 1;
                    G_No[2] = 0;
                    G_No[3] = 0;
                    E_No[0] = 2;
                    E_No[1] = 0;
                    E_No[2] = 0;
                    E_No[3] = 0;
                    break;
                }

                for (xx = 0; xx < 20; xx++) {
                    save_w[Present_Mode].Ranking[xx] = Ranking_Data[xx];
                }

                if (save_w[Present_Mode].Auto_Save) {
                    G_No[2] = 5;
                    G_No[3] = 0;
                    G_Timer = 4;
                    Pause_ID = Player_id;
                    cpReadyTask(MENU_TASK_NUM, Menu_Task);
                    System_all_clear_Level_B();
                    Menu_Init(&task[3]);
                    task[3].r_no[0] = 9;
                    task[3].r_no[1] = 0;
                    Forbid_Reset = 1;
                    make_texcash_work(12);
                    Unsubstantial_BG[0] = 1;
                    Copy_Check_w();
                    cpExitTask(SAVER_TASK_NUM);
                } else {
                    G_No[2] = 6;
                }
            }

            break;

        case 5:
            if (G_No[3] == 0) {
                FadeOut(1, 0xFF, 8);

                if (--G_Timer == 0) {
                    G_No[3] = 1;
                }
            }

            break;

        case 6:
            Switch_Screen(1);
            G_No[0] = 1;
            G_No[1] = 0x63;
            G_No[2] = 0;
            G_No[3] = 0;
            E_No[0] = 0;
            E_No[1] = 0x63;
            E_No[2] = 0;
            E_No[3] = 0;
            D_No[0] = 0;
            D_No[1] = 0;
            D_No[2] = 0;
            D_No[3] = 0;
            Get_Demo_Index = 0;
            Combo_Demo_Flag = 0;
            cpReadyTask(ENTRY_TASK_NUM, Entry_Task);
            Purge_mmtm_area(5);
            Make_texcash_of_list(5);
            System_all_clear_Level_B();
            break;
        }

        BG_move();
    }
}

void Request_Break_Sub(s16 PL_id) {
    if ((Request_Break[PL_id] != 0) && (Ck_Break_Into(0, 0, PL_id) != 0)) {
        plw[PL_id].wu.operator = 1;
        Operator_Status[PL_id] = 1;
    }
}

s32 Check_Disp_Ranking() {
    s16 rank_type = Disp_Rank_Sub(0);

    if (rank_type != -1) {
        Rank_Type = rank_type;
        Present_Rank[0] = Rank_In[0][rank_type];
        Present_Rank[1] = Rank_In[1][rank_type];
        return 1;
    }

    rank_type = Disp_Rank_Sub(1);

    if (rank_type != -1) {
        Rank_Type = rank_type;
        Present_Rank[1] = Rank_In[1][rank_type];
        return 1;
    }

    return 0;
}

s16 Disp_Rank_Sub(s16 PL_id) {
    if (Request_Disp_Rank[PL_id][3] >= 0) {
        return 15;
    }

    if (Request_Disp_Rank[PL_id][2] >= 0) {
        return 10;
    }

    if (Request_Disp_Rank[PL_id][1] >= 0) {
        return 5;
    }

    if (Request_Disp_Rank[PL_id][0] >= 0) {
        return 0;
    }

    return -1;
}

s32 Disp_Ranking() {
    switch (G_No[3]) {
    case 0:
        G_No[3] += 1;
        Switch_Screen_Init(1);
        BGM_Request(57);
        break;

    case 1:
        if (Switch_Screen(1) != 0) {
            Cover_Timer = 24;
            G_No[3] += 1;
            D_No[0] = 1;
            D_No[1] = 0;
            D_No[2] = 0;
            D_No[3] = 0;
            Clear_Personal_Data(0);
            grade_check_work_1st_init(0, 0);
            grade_check_work_1st_init(0, 1);
            Clear_Personal_Data(1);
            grade_check_work_1st_init(1, 0);
            grade_check_work_1st_init(1, 1);
        }

        break;

    case 2:
        Switch_Screen(1);
        Ranking();

        if (--Cover_Timer == 0) {
            G_No[3] += 1;
            Switch_Screen_Init(1);
        }

        break;

    case 3:
        Ranking();

        if (Switch_Screen_Revival(1) != 0) {
            G_No[3] += 1;
            Forbid_Break = 0;
        }

        break;

    default:
        if (Ranking() != 0) {
            BGM_Stop();
            return 1;
        }

        break;
    }

    return 0;
}

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

void Game08() {
    BG_Draw_System();

    switch (G_No[2]) {
    case 0:
        Switch_Screen(0);
        G_No[2] = 1;
        Game_pause = 0;
        Final_Result_id = WINNER;
        WGJ_Target = WINNER;
        WGJ_Win = Win_Record[WINNER];
        grade_final_grade_bonus();
        WGJ_Score = Continue_Coin[WINNER] + Score[WINNER][0];
        Purge_mmtm_area(6);
        cpExitTask(MENU_TASK_NUM);
        cpExitTask(PAUSE_TASK_NUM);
        break;

    case 1:
        if (Ending_main(End_PL) && (Request_Fade(9) != 0)) {
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

        break;
    }

    move_effect_work(4);
}

void Game09() {
    switch (G_No[2]) {
    case 0:
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

        break;

    case 1:
        BG_Draw_System();
        Switch_Screen(1);

        if (--G_Timer == 0) {
            if (Check_LDREQ_Queue_BG((u16)bg_w.stage) == 0) {
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

        break;

    case 2:
        Bonus_Sub();

        if (Switch_Screen_Revival(1) != 0) {
            G_No[2] += 1;
            Forbid_Break = 0;
        }

        break;

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

    BG_move();
}

s16 Bonus_Sub() {
    s16 x;

    mpp_w.inGame = 1;
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

        break;

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

    BG_move();
}

void Loop_Demo(struct _TASK* /* unused */) {
    if (Ck_Coin()) {
        Next_Title_Sub();
        return;
    }

    switch (G_No[1]) {
    case 0:
        G_No[1] += 1;
        G_No[2] = 0;
        G_No[3] = 0;
        D_No[0] = 0;
        D_No[1] = 0;
        D_No[2] = 0;
        D_No[3] = 0;
        E_No[1] = 0x63;
        Demo_PL_Index = 0;
        Demo_Stage_Index = 0;
        Select_Demo_Index = 0;
        check_screen_L = 0;
        check_screen_S = 0;
        Insert_Y = 23;
        Demo_Flag = 0;
        Play_Mode = 0;
        Replay_Status[0] = 0;
        Replay_Status[1] = 0;
        Present_Mode = 0;
        title_tex_flag = 0;
        Reset_Bootrom = 0;
        break;

    case 1:
        Basic_Sub();

        if (CAPCOM_Logo() != 0) {
            Loop_Demo_Sub();
            Insert_Y = 23;
            E_No[1] = 2;
            E_Timer = 1;
            return;
        }

        break;

    case 2:
        Basic_Sub();

        if (Title()) {
            Loop_Demo_Sub();
            Insert_Y = 17;
            D_No[0] = 1;
            return;
        }

        break;

    case 3:
        if (Play_Demo() != 0) {
            Switch_Screen(1);
            Loop_Demo_Sub();
            Rank_Type = 0;
            Demo_Type = 0;
            SsAllNoteOff();
            return;
        }

        break;

    case 4:
        Basic_Sub();

        if (Ranking() != 0) {
            Switch_Screen(1);
            Loop_Demo_Sub();
            setup_pos_remake_key(3);
            return;
        }

        break;

    case 5:
        if (Play_Demo() != 0) {
            Loop_Demo_Sub();
            Demo_Type = 1;
            Rank_Type = 5;
            Demo_Type = 1;
            SsAllNoteOff();
            return;
        }

        break;

    case 6:
        Basic_Sub();

        if (Ranking() != 0) {
            Switch_Screen(1);
            Loop_Demo_Sub();
            System_all_clear_Level_B();
            Purge_mmtm_area(6);
            Game_pause = 0;
            G_No[1] = 1;
            E_No[1] = 0x63;
            return;
        }

        break;

    default:
        Switch_Screen(1);

        if (--Cover_Timer <= 0) {
            Next_Demo_Loop();
        }

        break;
    }
}

void Next_Demo_Loop() {
    G_No[0] = 1;
    G_No[1] = 1;
    G_No[2] = 0;
    D_No[0] = 0;
    D_No[1] = 0;
    D_No[2] = 0;
    D_No[3] = 0;
    E_No[0] = 0;
    E_No[1] = 0x63;
    E_No[2] = 0;
    E_No[3] = 0;
    Demo_PL_Index = 0;
    Demo_Stage_Index = 0;
    Select_Demo_Index = 0;
    Demo_Flag = 0;
    Present_Mode = 0;
    Game_pause = 0;
    Play_Mode = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;
    System_all_clear_Level_B();
    Purge_mmtm_area(6);
}

void Loop_Demo_Sub() {
    G_No[1] += 1;
    G_No[2] = 0;
    D_No[0] = 0;
    D_No[1] = 0;
    D_No[2] = 0;
    D_No[3] = 0;
    E_No[1] = 1;
    Play_Game = 0;
    pulpul_stop();
    pp_operator_check_flag(1);
}

void Next_Title_Sub() {
    s16 ix;

    if (G_No[1] != 0x63) {
        SsAllNoteOff();
    }

    if (Demo_Flag == 0) {
        SsRequest(106);
    }

    TexRelease(0x258);
    TexRelease_OP();
    System_all_clear_Level_B();
    Purge_mmtm_area(6);
    G_Timer = 0;

    for (ix = 0; ix < 4; ix++) {
        vm_w.r_no[ix] = 0;
        G_No[ix] = 0;
        E_No[ix] = 0;
        D_No[ix] = 0;
        task->r_no[ix] = 0;
    }

    G_No[0] = 2;
    E_No[0] = 1;
    task->r_no[0] = 1;
    Demo_Flag = 1;
    Game_pause = 0;
    judge_flag = 0;
    Pause_Down = 0;
    Disp_Attack_Data = 0;
    seraph_flag = 0;
    End_Training = 0;
    Forbid_Reset = 0;
    Exec_Wipe = 0;
    Present_Mode = 1;
    Insert_Y = 23;
    Before_Select_Sub();
    cpReadyTask(ENTRY_TASK_NUM, Entry_Task);
    setup_pos_remake_key(2);
}

void Time_Control() {
    count_cont_main();

    if ((Allow_a_battle_f == 0) || (Demo_Time_Stop != 0) || (Bonus_Game_Flag != 0)) {
        return;
    }

    if (Game_pause != 0x81) {
        if (Control_Time >= Limit_Time) {
            Control_Time = Limit_Time;
            return;
        }

        if (--Time_in_Time == 0) {
            Time_in_Time = 60;
            Control_Time += 1;
        }
    }
}

s16 Ck_Coin() {
    s16 PL_id;

    switch (G_No[3]) {
    case 0:
        PL_id = -1;

        if (~p1sw_1 & p1sw_0 & 0x4000) {
            PL_id = 0;
        } else if (~p2sw_1 & p2sw_0 & 0x4000) {
            PL_id = 1;
        }

        if (PL_id == -1) {
            return 0;
        }

        ToneDown(0xFF, 0);
        Request_LDREQ_Break();
        G_No[3] = 1;
        plw[PL_id].wu.operator = 1;
        Operator_Status[PL_id] = 1;
        Champion = PL_id;
        plw[PL_id ^ 1].wu.operator = 0;
        Operator_Status[PL_id ^ 1] = 0;
        return 0;

    default:
    case 1:
        ToneDown(0xFF, 0);
        PL_id = Check_LDREQ_Break();
        return PL_id ^ 1;
    }
}

void Before_Select_Sub() {
    s16 xx;

    Request_G_No = 0;
    Request_E_No = 0;
    Allow_a_battle_f = 0;
    Bonus_Type = 0;

    if (Demo_Flag == 0) {
        Control_Time = 2048;
        Round_Level = 7;
    } else {
        Control_Time = 481;
    }

    Super_Arts[0] = 0;
    Super_Arts[1] = 0;
    Exec_Wipe = 0;
    Fade_Flag = 0;
    Stock_Com_Color[0] = -1;
    Stock_Com_Arts[0] = -1;
    Stock_Com_Color[1] = -1;
    Stock_Com_Arts[1] = -1;
    Bonus_Game_Flag = 0;
    Combo_Demo_Flag = 0;
    paring_counter[0] = 0;
    paring_bonus_r[0] = 0;
    paring_counter[1] = 0;
    paring_bonus_r[1] = 0;
    Clear_Disp_Ranking(0);
    Clear_Disp_Ranking(1);
    Clear_Personal_Data(0);
    grade_check_work_1st_init(0, 0);
    grade_check_work_1st_init(0, 1);
    Clear_Personal_Data(1);
    grade_check_work_1st_init(1, 0);
    grade_check_work_1st_init(1, 1);
    Last_Player_id = Player_Number = -1;
    Round_Level = 3;
    Time_in_Time = 60;

    if (Mode_Type != 2) {
        xx = system_timer;
        Random_ix16 = xx & 0x3F;
        Random_ix32 = xx & 0x7F;
    }
}

void Wait_Auto_Load(struct _TASK* /* unused */) {
    Basic_Sub();
    BG_Draw_System();
    bg_pos_hosei_sub2(0);
    Bg_Family_Set_appoint(0);
    BG_move_Ex(0);
}
