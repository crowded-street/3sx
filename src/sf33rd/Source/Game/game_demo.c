/**
 * @file game_demo.c
 * Game flow
 * The attract loop, the coin check and the wait states.
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


/* The demo loop's first frame: every counter and flag the attract sequence
 * starts from. */
static void enter_demo_loop() {
    G_No[1] += 1;
    G_No[2] = 0;
    G_No[3] = 0;
    D_No[0] = 0;
    D_No[1] = 0;
    D_No[2] = 0;
    D_No[3] = 0;
    E_No[1] = 99;
    Demo_PL_Index = 0;
    Demo_Stage_Index = 0;
    Select_Demo_Index = 0;
    Insert_Y = 23;
    Demo_Flag = 0;
    Play_Mode = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;
    Present_Mode = 0;
    title_tex_flag = 0;
    Reset_Bootrom = 0;
}

/* The second half of the attract sequence, reached from the first half's
 * default; the cover timer stays the default arm. */
static void run_demo_ranking_stage(u8 stage) {
    switch (stage) {
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
            E_No[1] = 99;
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

/* The attract sequence's later stages, reached from the demo loop's new
 * default. Every case keeps its original label, and the original default -
 * the cover timer - is the default here. */
static void run_demo_attract_stage(u8 stage) {
    switch (stage) {
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
            return;
        }

        break;

    default:
        run_demo_ranking_stage(stage);
        break;
    }
}

void Loop_Demo(struct _TASK* /* unused */) {
    if (Ck_Coin()) {
        Next_Title_Sub();
        return;
    }

    switch (G_No[1]) {
    case 0:
        enter_demo_loop();
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

    default:
        run_demo_attract_stage(G_No[1]);
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
    E_No[1] = 99;
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

    if (G_No[1] != 99) {
        SsAllNoteOff();
    }

    if (Demo_Flag == 0) {
        SsRequest(106);
    }

    TexRelease(600);
    TexRelease_OP();
    System_all_clear_Level_B();
    Purge_mmtm_area(6);
    G_Timer = 0;

    for (ix = 0; ix < 4; ix++) {
        vm_w.r_no[ix] = 0;
        G_No[ix] = 0;
        E_No[ix] = 0;
        D_No[ix] = 0;
        task[TASK_INIT].r_no[ix] = 0;
    }

    G_No[0] = 2;
    E_No[0] = 1;
    task[TASK_INIT].r_no[0] = 1;
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
    cpReadyTask(TASK_ENTRY, Entry_Task);
}

/* The match clock does not run before the battle is allowed, while a demo has
 * stopped it, or during the bonus stage. */
static s32 clock_is_held(void) {
    return (Allow_a_battle_f == 0) || (Demo_Time_Stop != 0) || (Bonus_Game_Flag != 0);
}

void Time_Control() {
    count_cont_main();

    if (clock_is_held()) {
        return;
    }

    if (Game_pause == 0x81) {
        return;
    }

    if (Control_Time >= Limit_Time) {
        Control_Time = Limit_Time;
    } else if (--Time_in_Time == 0) {
        Time_in_Time = 60;
        Control_Time += 1;
    }
}

s16 Ck_Coin() {
    s16 PL_id;

    switch (G_No[3]) {
    case 0:
        PL_id = -1;

        if (~p1sw_1 & p1sw_0 & (SWK_START | SWK_ATTACKS)) {
            PL_id = 0;
        } else if (~p2sw_1 & p2sw_0 & (SWK_START | SWK_ATTACKS)) {
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
        PL_id = Check_LDREQ_Break() ? 1 : 0;
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

    if (Mode_Type != MODE_NETWORK) {
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
