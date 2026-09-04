/**
 * @file vs_result_menu.c
 * Menus
 */

#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "core/app.h"
#include "main.h"
#include "platform/app/sdl/sdl_app.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/eff04.h"
#include "sf33rd/Source/Game/effect/eff10.h"
#include "sf33rd/Source/Game/effect/eff18.h"
#include "sf33rd/Source/Game/effect/eff23.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff40.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff45.h"
#include "sf33rd/Source/Game/effect/eff51.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/eff63.h"
#include "sf33rd/Source/Game/effect/eff64.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff91.h"
#include "sf33rd/Source/Game/effect/effa0.h"
#include "sf33rd/Source/Game/effect/effa3.h"
#include "sf33rd/Source/Game/effect/effa8.h"
#include "sf33rd/Source/Game/effect/effc4.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/io/vm_sub.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/menu/ex_data.h"
#include "sf33rd/Source/Game/message/en/msgtable_en.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/screen/entry.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/pause.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/reset.h"
#include "sf33rd/Source/Game/system/saver.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/count.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/PS2/mc/savesub.h"
#include "structs.h"

#if NETPLAY_ENABLED
#include "platform/netplay/fistbump.h"
#include "platform/netplay/netplay.h"
#include "port/sdl/netplay_screen.h"
#include "sf33rd/Source/Game/menu/netplay_menu.h"
#endif

#include "sf33rd/Source/Game/menu/menu_internal.h"

static void calculate_vs_win_percentages(u16 win_percentages[2]) {
    s16 total_battle = VS_Win_Record[0] + VS_Win_Record[1];

    if (total_battle == 0) {
        total_battle = 1;
    }
    if (VS_Win_Record[0] >= VS_Win_Record[1]) {
        win_percentages[1] = (VS_Win_Record[1] * 100) / total_battle;
        if (win_percentages[1] == 0 && VS_Win_Record[1] > 0) {
            win_percentages[1] = 1;
        }
        win_percentages[0] = 100 - win_percentages[1];
        return;
    }
    win_percentages[0] = (VS_Win_Record[0] * 100) / total_battle;
    if (win_percentages[0] == 0 && VS_Win_Record[0] > 0) {
        win_percentages[0] = 1;
    }
    win_percentages[1] = 100 - win_percentages[0];
}

static void initialize_vs_result(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_ix2;
    u16 win_percentages[2];

    s16 s4;
    s16 s3;

    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[2]++;
    task_ptr->timer = 5;
    Menu_Common_Init();
    Menu_Cursor_Y[0] = Cursor_Y_Pos[0][0];
    Menu_Cursor_Y[1] = Cursor_Y_Pos[1][0];
    Menu_Suicide[0] = 0;
    Menu_Suicide[1] = 1;
    Menu_Cursor_X[0] = 0;
    Menu_Cursor_X[1] = 0;
    Order[78] = 2;
    Order_Dir[78] = 0;
    Order_Timer[78] = 1;
    effect_66_init(91, 12, 0, 0, 71, 9, 0);
    Order[91] = 3;
    Order_Timer[91] = 1;
    effect_66_init(138, 24, 0, 0, -1, -1, -0x7FF9);
    Order[138] = 3;
    Order_Timer[138] = 1;
    effect_66_init(139, 25, 0, 0, -1, -1, -0x7FF9);
    Order[139] = 3;
    Order_Timer[139] = 1;
    effect_A0_init(0, VS_Win_Record[0], 0, 3, 0, 0, 0);
    effect_A0_init(0, VS_Win_Record[1], 1, 3, 0, 0, 0);
    calculate_vs_win_percentages(win_percentages);

    effect_A0_init(0, win_percentages[0], 2, 3, 0, 0, 0);
    effect_A0_init(0, win_percentages[1], 3, 3, 0, 0, 0);

    for (ix = 0, s4 = char_ix2 = 22; ix < 3; ix++, s3 = char_ix2++) {
        effect_91_init(0, ix, 0, 71, char_ix2, 0);
        effect_91_init(1, ix, 0, 71, char_ix2, 0);
    }

    Setup_Win_Lose_OBJ();
    Menu_Cursor_Move = 0;
}

static void start_vs_rematch(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[3]) {
    case 0:
        task_ptr->r_no[3]++;
        /* fallthrough */

    case 1:
        if (--task_ptr->timer) {
            break;
        }

        Setup_VS_Mode(task_ptr);
        G_No[1] = 12;
        G_No[2] = 1;
        // We should leave Mode_Type be, no need to reset it
        // Mode_Type = MODE_VERSUS;
        break;
    }
}

static void advance_vs_result_exit(struct _TASK* task_ptr) {
    if (--task_ptr->timer == 0) {
        task_ptr->r_no[3]++;
    }
}

static void handle_vs_result_early_state(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 0:
        System_all_clear_Level_B();
        Menu_Init(task_ptr);
        task_ptr->r_no[1] = 16;
        task_ptr->r_no[2] = 1;
        task_ptr->r_no[3] = 0;
        Sel_PL_Complete[0] = 0;
        Sel_Arts_Complete[0] = 0;
        Sel_PL_Complete[1] = 0;
        Sel_Arts_Complete[1] = 0;
        Clear_Flash_Init(4);
        break;

    case 1:
        initialize_vs_result(task_ptr);
        break;

    case 2:
        FadeOut(1, 0xFF, 8);

        if (--task_ptr->timer == 0) {
            task_ptr->r_no[2]++;
            FadeInit();
        }

        break;

    case 3:
        if (FadeIn(1, 25, 8)) {
            task_ptr->r_no[2]++;
            Suicide[3] = 0;
        }

        break;
    }
}

static void handle_vs_result_selection_state(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 4:
        if (VS_Result_Select_Sub(task_ptr, 0) == 0) {
            VS_Result_Select_Sub(task_ptr, 1);
        }

        break;

    case 5:
        if (task_ptr->r_no[3] == 0) {
            advance_vs_result_exit(task_ptr);
            break;
        }

        Exit_Sub(task_ptr, 0, 17);
        break;
    }
}

static void handle_vs_result_exit_state(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 6:
        start_vs_rematch(task_ptr);
        break;

    case 7:
    default:
#if NETPLAY_ENABLED
        Netplay_HandleMenuExit();
#endif

        if (Exit_Sub(task_ptr, 0, 0)) {
            System_all_clear_Level_B();
            BGM_Request_Code_Check(65);
        }

        break;
    }
}

void VS_Result(struct _TASK* task_ptr) {
    Clear_Flash_Sub();
    if (task_ptr->r_no[2] < 4) {
        handle_vs_result_early_state(task_ptr);
        return;
    }
    if (task_ptr->r_no[2] < 6) {
        handle_vs_result_selection_state(task_ptr);
        return;
    }
    handle_vs_result_exit_state(task_ptr);
}

void Setup_Win_Lose_OBJ() {
    s16 x[2];

    if (WINNER == 0) {
        x[0] = 26;
        x[1] = 27;
    } else {
        x[0] = 27;
        x[1] = 26;
    }

    effect_66_init(140, x[0], 0, 0, 71, 12, 0);
    Order[140] = 3;
    Order_Timer[140] = 1;
    effect_66_init(141, x[1], 0, 0, 71, 13, 0);
    Order[141] = 3;
    Order_Timer[141] = 1;
    effect_66_init(142, 26, 0, 0, 71, 14, 1);
    Order[142] = 3;
    Order_Timer[142] = 1;
    effect_66_init(143, 27, 0, 0, 71, 14, 01);
    Order[143] = 3;
    Order_Timer[143] = 1;
}

s32 VS_Result_Select_Sub(struct _TASK* task_ptr, s16 PL_id) {
    u16 sw = Check_Menu_Lever(PL_id, 0);

    if (Menu_Cursor_X[PL_id] == 0) {
        After_VS_Move_Sub(sw, PL_id, 2);

        if (VS_Result_Move_Sub(task_ptr, PL_id) != 0) {
            Pause_ID = PL_id;
            return 1;
        }
    } else if (sw == SWK_EAST) {
        IO_Result = SWK_EAST;
        VS_Result_Move_Sub(task_ptr, PL_id);
    }

    return 0;
}

static bool should_skip_vs_result_option(void) {
    return plw[0].wu.operator == 0 || plw[1].wu.operator == 0 || Mode_Type == MODE_NETWORK;
}

static u16 move_vs_cursor(u16 input, s16 cursor_id, s16 menu_max, s16 skipped_cursor) {
    if (input == SWK_UP) {
        Menu_Cursor_Y[cursor_id]--;
        if (Menu_Cursor_Y[cursor_id] < 0) {
            Menu_Cursor_Y[cursor_id] = menu_max;
        }
        if (Menu_Cursor_Y[cursor_id] == skipped_cursor) {
            Menu_Cursor_Y[cursor_id] = 0;
        }
        SE_cursor_move();
        return IO_Result = SWK_UP;
    }
    Menu_Cursor_Y[cursor_id]++;
    if (Menu_Cursor_Y[cursor_id] > menu_max) {
        Menu_Cursor_Y[cursor_id] = 0;
    }
    if (Menu_Cursor_Y[cursor_id] == skipped_cursor) {
        Menu_Cursor_Y[cursor_id] = 2;
    }
    SE_cursor_move();
    return IO_Result = SWK_DOWN;
}

static u16 pass_through_vs_input(u16 input) {
    switch (input) {
    case SWK_WEST:
    case SWK_SOUTH:
    case SWK_EAST:
    case SWK_RIGHT_TRIGGER:
    case SWK_START:
        return IO_Result = input;
    default:
        return IO_Result = 0;
    }
}

static bool is_vs_shoulder_input(u16 input) {
    switch (input) {
    case SWK_NORTH:
    case SWK_RIGHT_SHOULDER:
    case SWK_LEFT_SHOULDER:
    case SWK_LEFT_TRIGGER:
        return true;
    default:
        return false;
    }
}

static u16 pass_through_vs_shoulder_input(u16 input) {
    if (is_vs_shoulder_input(input)) {
        return IO_Result = input;
    }
    return IO_Result = 0;
}

u16 After_VS_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max) {
    s16 skip = should_skip_vs_result_option() ? 1 : 99;

    if (sw == SWK_UP) {
        return move_vs_cursor(sw, cursor_id, menu_max, skip);
    }
    if (sw == SWK_DOWN) {
        return move_vs_cursor(sw, cursor_id, menu_max, skip);
    }
    if (pass_through_vs_input(sw) != 0) {
        return IO_Result;
    }
    return pass_through_vs_shoulder_input(sw);
}

static s32 select_vs_result_option(struct _TASK* task_ptr, s16 player_id) {
    SE_selected();
    switch (Menu_Cursor_Y[player_id]) {
    case 0:
        Menu_Cursor_X[player_id] = 1;
        if (!Menu_Cursor_X[player_id ^ 1]) {
            return 0;
        }
        task_ptr->r_no[2] = 6;
        break;
    case 1:
        task_ptr->r_no[2] = 5;
        break;
    case 2:
        task_ptr->r_no[2] = 7;
        break;
    }
    task_ptr->r_no[3] = 0;
    task_ptr->timer = 15;
    return 1;
}

s32 VS_Result_Move_Sub(struct _TASK* task_ptr, s16 PL_id) {
    switch (IO_Result) {
    case SWK_SOUTH:
        return select_vs_result_option(task_ptr, PL_id);

    case SWK_EAST:
        SE_selected();

        if (Menu_Cursor_X[PL_id]) {
            Menu_Cursor_X[PL_id] = 0;
            break;
        }

        if (Menu_Cursor_Y[PL_id] == 2) {
            task_ptr->r_no[2] = 99;
            return 1;
        }

        Menu_Cursor_Y[PL_id] = 2;
        break;
    }

    return 0;
}

void Save_Replay(struct _TASK* task_ptr) {
    Menu_Cursor_X[1] = Menu_Cursor_X[0];
    Clear_Flash_Sub();

    switch (task_ptr->r_no[2]) {
    case 0:
        Setup_Save_Replay_1st(task_ptr);
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            SaveInit(SAVE_FILE_REPLAY, SAVE_MODE_SAVE);
        }
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 0;
        Order_Timer[0x4E] = 1;
        break;

    case 2:
        Setup_Save_Replay_2nd(task_ptr, 1);
        break;

    case 3:
        if (SaveMove() <= 0) {
            IO_Result = 0x200;
            Save_Replay_MC_Sub(task_ptr, 0);
        }
        break;
    }
}

void Setup_Save_Replay_1st(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[2]++;
    task_ptr->timer = 5;
    Menu_Common_Init();
    Menu_Cursor_X[0] = 0;
    Menu_Suicide[0] = 1;
    Menu_Suicide[1] = 0;
    Menu_Suicide[2] = 0;
    Menu_Suicide[3] = 0;
    Setup_BG(1, 512, 0);
    Setup_Replay_Sub(110, MENU_HEADER_REPLAY, 1);
    Setup_File_Property(1, 0xFF);
    Clear_Flash_Init(4);
}

void Setup_Save_Replay_2nd(struct _TASK* task_ptr, s16 arg1) {
    if (FadeIn(1, 25, 8)) {
        task_ptr->r_no[2]++;
        task_ptr->free[3] = 0;
        Menu_Cursor_X[0] = Setup_Final_Cursor_Pos(Menu_Cursor_X[0], 8);
    }
}

void Setup_Replay_Sub(s16 type, MenuHeader char_type, s16 master_player) {
    effect_57_init(type, char_type, 0, 63, 2);
    Order[type] = 1;
    Order_Dir[type] = 8;
    Order_Timer[type] = 1;
    effect_66_init(138, 8, master_player, 0, -1, -1, -0x7FF4);
    Order[138] = 3;
    Order_Timer[138] = 1;
}

void Return_VS_Result_Sub(struct _TASK* task_ptr) {
    Menu_Suicide[0] = 0;
    Menu_Suicide[1] = 1;
    task_ptr->r_no[1] = 16;
    task_ptr->r_no[2] = 1;
    task_ptr->r_no[3] = 0;
    task_ptr->free[0] = 0;
    Order[110] = 4;
    Order_Timer[110] = 1;
}

s32 Save_Replay_MC_Sub(struct _TASK* task_ptr, s16 /* unused */) {
    switch (IO_Result) {
    case 0x100:
        SE_selected();

        if (Menu_Cursor_X[0] == -1) {
            break;
        }

        if (vm_w.Connect[Menu_Cursor_X[0]] == 0) {
            break;
        }

        vm_w.Drive = (u8)Menu_Cursor_X[0];

        if (VM_Access_Request(6, Menu_Cursor_X[0]) == 0) {
            break;
        }

        task_ptr->free[1] = 0;
        task_ptr->free[2] = 0;
        task_ptr->r_no[0] = 3;
        return 1;

    case 0x200:
        if (Mode_Type == 5) {
            Back_to_Mode_Select(task_ptr);
        } else {
            Exit_Replay_Save(task_ptr);
        }

        return 1;
    }

    return 0;
}

void Exit_Replay_Save(struct _TASK* task_ptr) {
    if (task_ptr->r_no[1] == 17) {
        Return_VS_Result_Sub(task_ptr);
        return;
    }

    Menu_Suicide[0] = 0;
    Menu_Suicide[1] = 0;
    Menu_Suicide[2] = 1;
    task_ptr->r_no[1] = 5;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    task_ptr->free[0] = 0;
    Order[112] = 4;
    Order_Timer[112] = 4;
}

void Decide_PL(s16 PL_id) {
    plw[PL_id].wu.operator = 1;
    Operator_Status[PL_id] = 1;
    Champion = PL_id;
    plw[PL_id ^ 1].wu.operator = 0;
    Operator_Status[PL_id ^ 1] = 0;

    if (Continue_Coin[PL_id] == 0) {
        grade_check_work_1st_init(PL_id, 0);
    }
}
