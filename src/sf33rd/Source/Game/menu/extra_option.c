/**
 * @file menu.c
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

void Default_Training_Option();
void Dummy_Move_Sub(struct _TASK* task_ptr, s16 PL_id, s16 id, s16 type, s16 max);
void Return_Pause_Sub(struct _TASK* task_ptr);
void Dummy_Move_Sub_LR(u16 sw, s16 id, s16 type, s16 cursor_id);
void Return_VS_Result_Sub(struct _TASK* task_ptr);
void Exit_Replay_Save(struct _TASK* task_ptr);
void Setup_NTr_Data(s16 ix);
s32 Check_Pad_in_Pause(struct _TASK* task_ptr);
void Next_Be_Tr_Menu(struct _TASK* task_ptr);
void Yes_No_Cursor_Exit_Training(struct _TASK* task_ptr, s16 cursor_id);
void Check_Skip_Recording();
void Check_Skip_Replay(s16 ix);
void Setup_Tr_Pause(struct _TASK* task_ptr);
void Control_Player_Tr();
s32 Pause_Check_Tr(s16 PL_id);
void Setup_Win_Lose_OBJ();
s32 Pause_in_Normal_Tr(struct _TASK* task_ptr);
void Training_Disp_Sub(struct _TASK* task_ptr);

// forward decls
void After_Title(struct _TASK* task_ptr);
void In_Game(struct _TASK* task_ptr);
void Wait_Load_Save(struct _TASK* task_ptr);
void Wait_Replay_Check(struct _TASK* task_ptr);
void Suspend_Menu();
void Wait_Replay_Load();
void Training_Menu(struct _TASK* task_ptr);
void After_Replay(struct _TASK* task_ptr);
void Wait_Pause_in_Tr(struct _TASK* task_ptr);
void Reset_Training(struct _TASK* task_ptr);
void Reset_Replay(struct _TASK* task_ptr);
void End_Replay_Menu(struct _TASK* task_ptr);
void Mode_Select(struct _TASK* task_ptr);
void Option_Select(struct _TASK* task_ptr);
void Training_Mode(struct _TASK* task_ptr);
void System_Direction(struct _TASK* task_ptr);
void Load_Replay(struct _TASK* task_ptr);
void toSelectGame(struct _TASK* task_ptr);
void Game_Option(struct _TASK* task_ptr);
void Button_Config(struct _TASK* task_ptr);
void Screen_Adjust(struct _TASK* task_ptr);
void Sound_Test(struct _TASK* task_ptr);
void Extra_Option(struct _TASK* task_ptr);
void VS_Result(struct _TASK* task_ptr);
void Save_Replay(struct _TASK* task_ptr);
void Direction_Menu(struct _TASK* task_ptr);
void Setup_VS_Mode(struct _TASK* task_ptr);
void Setup_Next_Page(struct _TASK* task_ptr, u8 /* unused */);
void Load_Replay_Sub(struct _TASK* task_ptr);
void Button_Exit_Check(struct _TASK* task_ptr, s16 PL_id);
void Back_to_Mode_Select(struct _TASK* task_ptr);
void Flash_1P_or_2P(struct _TASK* task_ptr);

void bg_etc_write_ex(s16 type);
void Decide_PL(s16 PL_id);
void imgSelectGameButton();
void jmpRebootProgram();
s32 Check_Pause_Term_Tr(s16 PL_id);

void Menu_in_Sub(struct _TASK* task_ptr);
s32 Exit_Sub(struct _TASK* task_ptr, s16 cursor_ix, s16 next_routine);
u16 MC_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max, s16 cansel_menu);
s32 Menu_Sub_case1(struct _TASK* task_ptr);
void System_Dir_Move_Sub(s16 PL_id);
void System_Dir_Move_Sub_LR(u16 sw, s16 cursor_id);
u16 Dir_Move_Sub2(u16 sw);
void Dir_Move_Sub_LR(u16 sw, s16 /* unused */);
void Ex_Move_Sub_LR(u16 sw, s16 PL_id);
u16 Game_Option_Sub(s16 PL_id);
u16 GO_Move_Sub_LR(u16 sw, s16 cursor_id);
void Button_Config_Sub(s16 PL_id);
void Button_Move_Sub_LR(u16 sw, s16 cursor_id);
void Return_Option_Mode_Sub(struct _TASK* task_ptr);
void Screen_Adjust_Sub(s16 PL_id);
void Screen_Exit_Check(struct _TASK* task_ptr, s16 PL_id);
void Screen_Move_Sub_LR(u16 sw);
u16 Sound_Cursor_Sub(s16 PL_id);
u16 SD_Move_Sub_LR(u16 sw);
u16 After_VS_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max);
s32 VS_Result_Move_Sub(struct _TASK* task_ptr, s16 PL_id);
void Training_Init(struct _TASK* task_ptr);
void Menu_Select(struct _TASK* task_ptr);
void Button_Config_in_Game(struct _TASK* task_ptr);
void Character_Change(struct _TASK* task_ptr);
void Pad_Come_Out(struct _TASK* task_ptr);
void Normal_Training(struct _TASK* task_ptr);
void Blocking_Training(struct _TASK* task_ptr);
void Dummy_Setting(struct _TASK* task_ptr);
void Training_Option(struct _TASK* task_ptr);
void Button_Config_Tr(struct _TASK* task_ptr);
void Blocking_Tr_Option(struct _TASK* task_ptr);
void Training_Init_Sub(struct _TASK* task_ptr);
void Training_Exit_Sub(struct _TASK* task_ptr);
void Menu_Init(struct _TASK* task_ptr);
s32 Check_Pad_in_Pause(struct _TASK* task_ptr);
s32 Yes_No_Cursor_Move_Sub(struct _TASK* task_ptr);
void Setup_Button_Sub(s16 x, s16 y, s16 master_player);
void Button_Exit_Check_in_Game(struct _TASK* task_ptr, s16 PL_id);
void Setup_Save_Replay_1st(struct _TASK* task_ptr);
s32 Save_Replay_MC_Sub(struct _TASK* task_ptr, s16 /* unused */);
void Button_Exit_Check_in_Tr(struct _TASK* task_ptr, s16 PL_id);
s32 VS_Result_Select_Sub(struct _TASK* task_ptr, s16 PL_id);
void Setup_Replay_Sub(s16 type, MenuHeader char_type, s16 master_player);

static void update_extra_option_message() {
    if (Menu_Cursor_Y[1] != Menu_Cursor_Y[0]) {
        SE_cursor_move();
        save_w[Present_Mode].extra_option.contents[Menu_Page][Menu_Max] = 1;

        if (Menu_Cursor_Y[0] < Menu_Max) {
            Message_Data->order = 1;
            Message_Data->request = Ex_Account_Data[Menu_Page] + Menu_Cursor_Y[0];
            Message_Data->timer = 2;

            if (msgExtraTbl[0]->msgNum[Menu_Cursor_Y[0] + (Menu_Page * 8)] == 1) {
                Message_Data->pos_y = 54;
            } else {
                Message_Data->pos_y = 62;
            }
        } else {
            Message_Data->order = 1;
            Message_Data->request = save_w[Present_Mode].extra_option.contents[Menu_Page][Menu_Max] + 32;
            Message_Data->timer = 2;
            Message_Data->pos_y = 54;
        }
    }
}

static void handle_extra_option_page_action(struct _TASK* task_ptr) {
    switch (save_w[Present_Mode].extra_option.contents[Menu_Page][Menu_Max]) {
    case 0:
        task_ptr->r_no[2] = 1;
        task_ptr->timer = 5;

        if (--Menu_Page < 0) {
            Menu_Page = Page_Max;
        }

        break;

    case 2:
        task_ptr->r_no[2] = 1;
        task_ptr->timer = 5;

        if (++Menu_Page > Page_Max) {
            Menu_Page = 0;
        }

        break;

    default:
        Return_Option_Mode_Sub(task_ptr);
        save_w[4].extra_option = save_w[1].extra_option;
        save_w[5].extra_option = save_w[1].extra_option;
        Order[115] = 4;
        Order_Timer[115] = 4;
        break;
    }

    SE_selected();
}

static void select_previous_extra_option_page(struct _TASK* task_ptr) {
    task_ptr->r_no[2] = 1;
    task_ptr->timer = 5;

    if (--Menu_Page < 0) {
        Menu_Page = Page_Max;
    }

    SE_dir_selected();
}

static void select_next_extra_option_page(struct _TASK* task_ptr) {
    task_ptr->r_no[2] = 1;
    task_ptr->timer = 5;

    if (++Menu_Page > Page_Max) {
        Menu_Page = 0;
    }

    SE_dir_selected();
}

static void read_second_extra_option_input(struct _TASK* task_ptr) {
    if (IO_Result == 0) {
        Pause_ID = 1;
        Dir_Move_Sub(task_ptr, 1);
    }
}

static void reset_extra_options(void) {
    save_w[Present_Mode].extra_option = save_w[0].extra_option;
    SE_selected();
}

static bool should_reset_extra_options(void) {
    return Menu_Page == 0 && Menu_Cursor_Y[0] == 6;
}

static bool should_handle_extra_option_page_action(void) {
    return Menu_Cursor_Y[0] == Menu_Max;
}

static void handle_extra_option_selection(struct _TASK* task_ptr) {
    if (should_reset_extra_options()) {
        reset_extra_options();
        return;
    }

    if (!should_handle_extra_option_page_action()) {
        return;
    }

    handle_extra_option_page_action(task_ptr);
}

static void advance_extra_option_fade(struct _TASK* task_ptr) {
    if (--task_ptr->timer == 0) {
        task_ptr->r_no[2]++;
        task_ptr->r_no[3] = 1;
        FadeInit();
    }
}

static void handle_extra_option_input(struct _TASK* task_ptr) {
    Pause_ID = 0;
    Dir_Move_Sub(task_ptr, 0);
    read_second_extra_option_input(task_ptr);

    update_extra_option_message();

    switch (IO_Result) {
    case 0x200:
        Return_Option_Mode_Sub(task_ptr);
        Order[115] = 4;
        Order_Timer[115] = 4;
        save_w[4].extra_option = save_w[1].extra_option;
        save_w[5].extra_option = save_w[1].extra_option;
        SE_dir_selected();
        break;

    case 0x80:
    case 0x800:
        select_previous_extra_option_page(task_ptr);
        break;

    case 0x40:
    case 0x400:
        select_next_extra_option_page(task_ptr);
        break;

    case 0x100:
        handle_extra_option_selection(task_ptr);
        break;
    }
}

void Extra_Option(struct _TASK* task_ptr) {
    Menu_Cursor_Y[1] = Menu_Cursor_Y[0];

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2]++;
        task_ptr->r_no[3] = 0;
        task_ptr->timer = 5;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Menu_Page = 0;
        Page_Max = 3;
        Menu_Page_Buff = Menu_Page;
        Message_Data->kind_req = 4;
        break;

    case 1:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2]++;
        Setup_Next_Page(task_ptr, task_ptr->r_no[3]);
        /* fallthrough */

    case 2:
        FadeOut(1, 0xFF, 8);
        advance_extra_option_fade(task_ptr);

        break;

    case 3:
        if (FadeIn(1, 25, 8)) {
            task_ptr->r_no[2]++;
            break;
        }

        break;

    case 4:
        handle_extra_option_input(task_ptr);
        break;
    }
}

static bool is_extra_option_page_item(void) {
    return Menu_Page_Buff != 0 || Menu_Cursor_Y[0] != 4;
}

static void adjust_extra_option_value(u16 sw) {
    if (sw == SWK_WEST) {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]]--;
    } else {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]]++;
    }
}

static void wrap_regular_extra_option(u16 sw) {
    if (sw == SWK_WEST && save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] < 0) {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] =
            Ex_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]];
    }
    if (sw == SWK_EAST && save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] >
                       Ex_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 0;
    }
}

static void finish_final_extra_option(u16 sw, u8 last_pos) {
    if (sw == SWK_WEST && save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] < 0) {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 0;
        IO_Result = 0x80;
        return;
    }
    if (sw == SWK_EAST && save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] > 2) {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 2;
        IO_Result = 0x400;
        return;
    }
    if (sw == SWK_EAST && save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] > 2) {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 2;
    }
    if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] != last_pos) {
        Message_Data->order = 1;
        Message_Data->request = save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Max] + 32;
        Message_Data->timer = 2;
    }
}

static void move_extra_option(u16 sw, u8 last_pos) {
    if (is_extra_option_page_item()) {
        SE_dir_cursor_move();
    }
    adjust_extra_option_value(sw);
    if (Menu_Cursor_Y[0] != Menu_Max) {
        wrap_regular_extra_option(sw);
        return;
    }
    finish_final_extra_option(sw, last_pos);
}

static void advance_extra_option_value(void) {
    if (is_extra_option_page_item()) {
        SE_dir_cursor_move();
    }
    save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]]++;
    if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] >
        Ex_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 0;
    }
}

void Ex_Move_Sub_LR(u16 sw, s16 PL_id) {
    u8 last_pos = save_w[Present_Mode].extra_option.contents[Menu_Page][Menu_Cursor_Y[0]];

    switch (sw) {
    case 4:
    case SWK_EAST:
        move_extra_option(sw, last_pos);
        return;

    case 0x400:
        if (Interface_Type[PL_id] == 2) {
            break;
        }

    case 0x100:
        if (Menu_Cursor_Y[0] == Menu_Max) {
            break;
        }
        advance_extra_option_value();
        return;
    }
}
