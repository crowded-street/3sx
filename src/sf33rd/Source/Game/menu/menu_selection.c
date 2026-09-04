/**
 * @file menu_selection.c
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

static void initialize_mode_select(struct _TASK* task_ptr, const s16 loop_counter) {
    s16 ix;

    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[2] += 1;
    task_ptr->timer = 5;
    Mode_Type = MODE_ARCADE;
    Present_Mode = 1;

    if (task[TASK_ENTRY].condition != 1) {
        E_No[0] = 1;
        E_No[1] = 2;
        E_No[2] = 2;
        E_No[3] = 0;
        cpReadyTask(TASK_ENTRY, Entry_Task);
    }

    Menu_Common_Init();

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 0;
    }

    Clear_Personal_Data(0);
    Clear_Personal_Data(1);
    Menu_Cursor_Y[0] = Cursor_Y_Pos[0][0];

    if (Menu_Cursor_Y[0] >= loop_counter) {
        Menu_Cursor_Y[0] = loop_counter - 1;
    }

    Cursor_Y_Pos[0][1] = 0;
    Cursor_Y_Pos[0][2] = 0;
    Cursor_Y_Pos[0][3] = 0;

    for (ix = 0; ix < 4; ix++) {
        Vital_Handicap[ix][0] = 7;
        Vital_Handicap[ix][1] = 7;
    }

    VS_Stage = 0x14;
    Order[0x8A] = 4;
    Order_Timer[0x8A] = 1;

    for (ix = 0; ix < 4; ix++) {
        Message_Data[ix].order = 3;
    }

    effect_57_init(0x64, MENU_HEADER_MODE_MENU, 0, 0x3F, 2);
    Order[0x64] = 1;
    Order_Dir[0x64] = 8;
    Order_Timer[0x64] = 1;
    Menu_Suicide[0] = 0;
    effect_04_init(0, 0, 0, 0x48);

    for (ix = 0; ix < loop_counter; ix++) {
        effect_61_init(0, ix + 0x50, 0, 0, (u32)ix, ix, 0x7047);
        Order[ix + 0x50] = 1;
        Order_Dir[ix + 0x50] = 4;
        Order_Timer[ix + 0x50] = ix + 0x14;
    }

    Menu_Cursor_Move = loop_counter;
}

static bool should_skip_mode_selection(void) {
    return Connect_Status == 0 && Menu_Cursor_Y[0] == 1;
}

static void select_arcade_mode(struct _TASK* task_ptr, s16 player_id) {
    G_No[2] += 1;
    Mode_Type = MODE_ARCADE;
    task_ptr->r_no[0] = 5;
    cpExitTask(TASK_SAVER);
    Decide_PL(player_id);
}

static void select_versus_mode(struct _TASK* task_ptr) {
    Setup_VS_Mode(task_ptr);
    G_No[1] = 12;
    G_No[2] = 1;
    Mode_Type = MODE_VERSUS;
    cpExitTask(TASK_MENU);
}

static void select_submenu_mode(struct _TASK* task_ptr) {
    task_ptr->r_no[2] += 1;
    task_ptr->free[0] = 0;
    task_ptr->free[1] = Menu_Cursor_Y[0] + 2;
}

static bool is_submenu_mode_selection(void) {
    switch (Menu_Cursor_Y[0]) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return true;
    default:
        return false;
    }
}

static void select_mode(struct _TASK* task_ptr, s16 player_id) {
    if (Menu_Cursor_Y[0] == 0) {
        select_arcade_mode(task_ptr, player_id);
        return;
    }
    if (Menu_Cursor_Y[0] == 1) {
        select_versus_mode(task_ptr);
        return;
    }
    if (is_submenu_mode_selection()) {
        select_submenu_mode(task_ptr);
    }
}

static void handle_mode_select_input(struct _TASK* task_ptr, const s16 loop_counter) {
    s16 player_id;

    if (should_skip_mode_selection()) {
        Menu_Cursor_Y[0] = 2;
    } else {
        player_id = 0;

        if (MC_Move_Sub(Check_Menu_Lever(0, 0), 0, loop_counter - 1, 1) == 0) {
            player_id = 1;
            MC_Move_Sub(Check_Menu_Lever(1, 0), 0, loop_counter - 1, 1);
        }
    }

    if (IO_Result == 0x100) {
        select_mode(task_ptr, player_id);
        SE_selected();
    }
}

static void handle_mode_select_load(struct _TASK* task_ptr) {
    if (task_ptr->free[3]) {
        FadeOut(1, 0xFF, 8);

        if (SaveMove() > 0) {
            return;
        }

        task_ptr->free[3] = 0;
        Forbid_Reset = 0;
    }

    if (Menu_Sub_case1(task_ptr) != 0) {
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 0;
        Order_Timer[0x4E] = 1;
        checkAdxFileLoaded();
        checkSelObjFileLoaded();
    }
}

void Mode_Select(struct _TASK* task_ptr) {
    const bool supports_exit = App_SupportsExit();
    const s16 loop_counter = supports_exit ? 7 : 6;

    switch (task_ptr->r_no[2]) {
    case 0:
        initialize_mode_select(task_ptr, loop_counter);
        break;

    case 1:
        handle_mode_select_load(task_ptr);
        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
            Suicide[3] = 0;
        }

        break;

    case 3:
        handle_mode_select_input(task_ptr, loop_counter);
        break;

    default:
        Exit_Sub(task_ptr, 0, task_ptr->free[1]);
        break;
    }
}

void Setup_VS_Mode(struct _TASK* task_ptr) {
    task_ptr->r_no[0] = 5;
    cpExitTask(TASK_SAVER);
    plw[0].wu.operator = 1;
    plw[1].wu.operator = 1;
    Operator_Status[0] = 1;
    Operator_Status[1] = 1;
    grade_check_work_1st_init(0, 0);
    grade_check_work_1st_init(0, 1);
    grade_check_work_1st_init(1, 0);
    grade_check_work_1st_init(1, 1);
    Setup_Training_Difficulty();
}

void Menu_in_Sub(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[2] += 1;
    task_ptr->timer = 5;
    Menu_Common_Init();
    Menu_Cursor_Y[0] = Cursor_Y_Pos[0][1];
    Menu_Suicide[0] = 1;
    Menu_Suicide[1] = 0;
    Order[0x64] = 4;
    Order_Timer[0x64] = 1;
}

static void handle_select_game_input(struct _TASK* task_ptr) {
    u16 input;

    imgSelectGameButton();
    input = (~plsw_01[0] & plsw_00[0]) | (~plsw_01[1] & plsw_00[1]); // potential macro
    input &= (SWK_SOUTH | SWK_EAST);

    if (input == 0 || input == (SWK_SOUTH | SWK_EAST)) {
        return;
    }
    if (input & SWK_SOUTH) {
        task_ptr->free[0] = 1;
    }
    SE_selected();
    FadeInit();
    task_ptr->r_no[2] = 8;
}

static void handle_select_game_fade_out(struct _TASK* task_ptr) {
    imgSelectGameButton();

    if (FadeOut(1, 25, 8) != 0) {
        if (task_ptr->free[0]) {
            task_ptr->r_no[2] = 10;
            sound_all_off();
        } else {
            task_ptr->r_no[2] = 9;
        }
    }
}

static void handle_select_game_active_state(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 0:
        Forbid_Reset = 1;
        Menu_in_Sub(task_ptr);
        Setup_BG(1, 0x200, 0);
        effect_66_init(0x8A, 8, 1, 0, -1, -1, -0x7FF2);
        Order[0x8A] = 3;
        Order_Timer[0x8A] = 1;
        task_ptr->free[0] = 0;
        task_ptr->timer = 0x10;
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            Message_Data->kind_req = 5;
            Message_Data->request = 0;
            Message_Data->order = 1;
            Message_Data->timer = 2;
            Message_Data->pos_x = 0;
            Message_Data->pos_y = 0xA0;
            Message_Data->pos_z = 0x18;
            effect_45_init(0, 0, 2);
        }

        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
        }

        imgSelectGameButton();
        break;

    case 3:
        handle_select_game_input(task_ptr);
        break;
    }
}

static void handle_select_game_transition_state(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 8:
        handle_select_game_fade_out(task_ptr);
        break;

    case 9:
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        task_ptr->free[0] = 0;
        FadeOut(1, 0xFF, 8);
        Forbid_Reset = 0;
        break;

    case 10:
        Exit_sound_system();
        task_ptr->r_no[2] += 1;
        FadeOut(1, 0xFF, 8);
        break;

    default:
        App_Exit();
        FadeOut(1, 0xFF, 8);
        break;
    }
}

void toSelectGame(struct _TASK* task_ptr) {
    if (task_ptr->r_no[2] < 8) {
        handle_select_game_active_state(task_ptr);
        return;
    }
    handle_select_game_transition_state(task_ptr);
}

void imgSelectGameButton() {
    dispButtonImage2(0x74, 0x6B, 0x18, 0x20, 0x1A, 0, 4, 0);
    dispButtonImage2(0xB2, 0x6B, 0x18, 0x20, 0x1A, 0, 5, 0);
}

static bool is_option_selection_input(void) {
    if (MC_Move_Sub(Check_Menu_Lever(0, 0), 0, 5, 0xFF) == 0) {
        MC_Move_Sub(Check_Menu_Lever(1, 0), 0, 5, 0xFF);
    }

    switch (IO_Result) {
    case 0x100:
    case 0x200:
        return true;

    default:
        return false;
    }
}

static void initialize_option_select(struct _TASK* task_ptr) {
    s16 ix;
    static const s16 option_items[6] = { 7, 8, 9, 10, 12, 13 };

    Menu_in_Sub(task_ptr);
    Order[0x4E] = 2;
    Order_Dir[0x4E] = 0;
    Order_Timer[0x4E] = 1;
    effect_57_init(0x4F, MENU_HEADER_OPTION_MENU, 0, 0x3F, 2);
    Order[0x4F] = 1;
    Order_Dir[0x4F] = 8;
    Order_Timer[0x4F] = 1;
    effect_04_init(1, 1, 0, 0x48);
    if (Menu_Cursor_Y[0] >= 6) {
        Menu_Cursor_Y[0] = 5;
    }
    for (ix = 0; ix < 6; ix++) {
        effect_61_init(0, ix + 0x50, 0, 1, option_items[ix], ix, 0x7047);
        Order[ix + 0x50] = 1;
        Order_Dir[ix + 0x50] = 4;
        Order_Timer[ix + 0x50] = ix + 0x14;
    }
    Menu_Cursor_Move = 6;
}

static void leave_option_select(struct _TASK* task_ptr) {
    Menu_Suicide[0] = 0;
    Menu_Suicide[1] = 1;
    task_ptr->r_no[1] = 1;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    task_ptr->free[0] = 0;
    Order[0x4F] = 4;
    Order_Timer[0x4F] = 4;
    if (Check_Change_Contents()) {
        SaveInit(SAVE_FILE_SETTINGS, SAVE_MODE_SAVE);
        task_ptr->free[3] = 1;
        Forbid_Reset = 1;
        Copy_Check_w();
    }
}

static void enter_selected_option(struct _TASK* task_ptr) {
    task_ptr->r_no[2] += 1;
    task_ptr->free[0] = 0;
    X_Adjust_Buff[0] = X_Adjust;
    X_Adjust_Buff[1] = X_Adjust;
    X_Adjust_Buff[2] = X_Adjust;
    Y_Adjust_Buff[0] = Y_Adjust;
    Y_Adjust_Buff[1] = Y_Adjust;
    Y_Adjust_Buff[2] = Y_Adjust;
}

static void handle_option_select_input(struct _TASK* task_ptr) {
    if (!is_option_selection_input()) {
        return;
    }
    SE_selected();
    if (Menu_Cursor_Y[0] == 5 || IO_Result == 0x200) {
        leave_option_select(task_ptr);
        return;
    }
    enter_selected_option(task_ptr);
}

void Option_Select(struct _TASK* task_ptr) {
    static const s16 option_routines[6] = { 9, 10, 11, 12, 14, 15 };

    switch (task_ptr->r_no[2]) {
    case 0:
        initialize_option_select(task_ptr);
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            checkSelObjFileLoaded();
        }

        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
            Suicide[3] = 0;
        }

        break;

    case 3:
        handle_option_select_input(task_ptr);
        break;

    default:
        Exit_Sub(task_ptr, 1, option_routines[Menu_Cursor_Y[0]]);
        break;
    }
}
