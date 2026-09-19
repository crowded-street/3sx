/**
 * @file game_round.c
 * Game flow
 * The fight itself: the round states and the ranking display.
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

static s32 Check_Disp_Ranking();


/* The round states: the intro, the fight itself, and the decision. */
static void game03_state(void) {
    switch (G_No[2]) {
    case 0:
        if (!Winner_Scene()) {
            break;
        }

        switch (Mode_Type) {
        case MODE_VERSUS:
        case MODE_NETWORK:
            G_No[2] += 1;
            Rep_Game_Infor[10].play_type = 1;
            Rep_Game_Infor[10].winner = Winner_id;
            Switch_Screen_Init(0);

            if (Country == 3) {
                Rep_Game_Infor[10].play_type = 4;
            }

            break;

            // case MODE_NETWORK:
            // G_No[2] = 3;
            // Rep_Game_Infor[10].play_type = 2;
            // Rep_Game_Infor[10].winner = Winner_id;
            // Champion = Winner_id;
            // New_Challenger = Loser_id;
            // Switch_Screen_Init(0);
            // break;

        case MODE_REPLAY:
            G_No[2] = 5;
            cpReadyTask(TASK_MENU, Menu_Task);
            task[TASK_MENU].r_no[0] = 8;
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
                G_No[1] = 11;
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

        break;

    case 1:
        if (!Switch_Screen(1)) {
            break;
        }

        G_No[2] += 1;
        E_No[0] = 1;
        E_No[1] = 2;
        E_No[2] = 2;
        E_No[3] = 0;
        Request_E_No = 0;
        cpReadyTask(TASK_MENU, Menu_Task);
        task[TASK_MENU].r_no[1] = 16;
        Cursor_Y_Pos[0][0] = 0;
        Cursor_Y_Pos[1][0] = 0;
        G_Timer = 4;
        break;

    case 2:
        Switch_Screen(1);

        if (--G_Timer) {
            break;
        }

        Cover_Timer = 10;
        G_No[1] = 12;
        G_No[2] = 0;
        G_No[3] = 0;

        break;

    case 3:
        if (!Switch_Screen(1)) {
            break;
        }

        G_No[2] += 1;
        task[7].r_no[0] = 1;
        G_Timer = 4;
        break;

    case 4:
        Switch_Screen(1);

        if (--G_Timer) {
            break;
        }

        // Do nothing
        break;

    case 5:
        // Do nothing
        break;
    }
}

void Game03() {
    BG_Draw_System();
    move_effect_work(4);
    move_effect_work(5);
    Play_Mode = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;

    game03_state();

    BG_move();
}

/* The between-rounds states. */
static void game04_state(void) {
    s16 i;

    switch (G_No[2]) {
    case 0:
        if (Loser_Scene() != 0) {
            if (Mode_Type == 5) {
                G_No[2] = 5;
                cpReadyTask(TASK_MENU, Menu_Task);
                task[TASK_MENU].r_no[0] = 8;
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
}

void Game04() {
    BG_Draw_System();
    move_effect_work(4);
    move_effect_work(5);

    game04_state();

    BG_move();
}

/* The continue states. */
static void game05_state(void) {
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
}

void Game05() {
    BG_Draw_System();
    Basic_Sub();
    Setup_Play_Type();

    game05_state();

    BG_move();
}

/* The end-of-match states: game over, the ranking display, and the switch to the next screen. */
static void game06_state(void) {
    s16 xx;

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

            G_No[2] = 5;
            G_No[3] = 0;
            G_Timer = 4;
            Pause_ID = Player_id;
            System_all_clear_Level_B();
            Forbid_Reset = 1;
            Copy_Check_w();
            cpExitTask(TASK_SAVER);
        }

        break;

    case 5:
        if (G_No[3] == 0) {
            FadeOut(1, 0xFF, 8);

            if (--G_Timer == 0) {
                G_No[3] = 1;
                SaveInit(SAVE_FILE_SETTINGS, SAVE_MODE_SAVE);
            }
        } else if (SaveMove() <= 0) {
            Forbid_Reset = 0;
            G_No[2] = 6;
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
        cpReadyTask(TASK_ENTRY, Entry_Task);
        Purge_mmtm_area(5);
        Make_texcash_of_list(5);
        System_all_clear_Level_B();
        break;
    }
}

void Game06() {
    BG_Draw_System();
    Basic_Sub_Ex();

    if (!Break_Into) {
        game06_state();

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
