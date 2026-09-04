/**
 * @file direction_menu.c
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

static bool should_exit_menu_selection(void) {
    return Menu_Cursor_Y[0] == 2 || IO_Result == 0x200;
}

static void handle_system_direction_input(struct _TASK* task_ptr) {
    System_Dir_Move_Sub(0);

    if (IO_Result == 0) {
        System_Dir_Move_Sub(1);
    }

    switch (IO_Result) {
    case 0x100:
        if (Menu_Cursor_Y[0] == 0) {
            break;
        }

        /* fallthrough */

    case 0x200:
        SE_selected();
        Order[0x6D] = 4;
        Order_Timer[0x6D] = 4;

        if (should_exit_menu_selection()) {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            task_ptr->r_no[1] = 1;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            task_ptr->free[3] = 1;
            Forbid_Reset = 1;
            SaveInit(SAVE_FILE_SYSTEM_DIRECTION, SAVE_MODE_SAVE);
            break;
        }

        task_ptr->r_no[2] += 1;
        task_ptr->free[0] = 0;

        break;
    }
}

void System_Direction(struct _TASK* task_ptr) {
    s16 ix;
    static const s16 menu_items[2] = { 0x2B, 0x2E };

    switch (task_ptr->r_no[2]) {
    case 0:
        Menu_in_Sub(task_ptr);
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 3;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x6D, MENU_HEADER_SYSTEM_DIRECTION, 0, 0x3F, 2);
        Order[0x6D] = 1;
        Order_Dir[0x6D] = 8;
        Order_Timer[0x6D] = 1;
        effect_04_init(1, 3, 0, 0x48);
        Convert_Buff[3][0][0] = Direction_Working[1];
        effect_64_init(0x61U, 0, 1, 0xA, 0, 0x7047, 0xB, 3, 0);
        Order[0x61] = 1;
        Order_Dir[0x61] = 4;
        Order_Timer[0x61] = 0x14;

        for (ix = 0; ix < 2; ix++) {
            effect_61_init(0, ix + 0x50, 0, 1, menu_items[ix], ix + 1, 0x7047);
            Order[ix + 0x50] = 1;
            Order_Dir[ix + 0x50] = 4;
            Order_Timer[ix + 0x50] = ix + 0x15;
        }

        Menu_Cursor_Move = 2;
        Page_Max = Check_SysDir_Page();
        break;

    case 1:
        Menu_Sub_case1(task_ptr);
        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
            Suicide[3] = 0;
        }

        break;

    case 3:
        handle_system_direction_input(task_ptr);
        break;

    default:
        Exit_Sub(task_ptr, 1, Menu_Cursor_Y[0] + 0x11);
        break;
    }
}

void System_Dir_Move_Sub(s16 PL_id) {
    u16 sw = ~plsw_01[PL_id] & plsw_00[PL_id]; // potential macro
    sw = Check_Menu_Lever(PL_id, 0);
    MC_Move_Sub(sw, 0, 2, 0xFF);
    System_Dir_Move_Sub_LR(sw, 0);
    Direction_Working[1] = Convert_Buff[3][0][0];
    Direction_Working[4] = Convert_Buff[3][0][0];
    Direction_Working[5] = Convert_Buff[3][0][0];
}

void System_Dir_Move_Sub_LR(u16 sw, s16 cursor_id) {
    if (Menu_Cursor_Y[cursor_id] != 0) {
        return;
    }

    switch (sw) {
    case 4:
        Convert_Buff[3][cursor_id][Menu_Cursor_Y[cursor_id]] -= 1;

        if (Convert_Buff[3][cursor_id][Menu_Cursor_Y[cursor_id]] < 0) {
            Convert_Buff[3][cursor_id][Menu_Cursor_Y[cursor_id]] = 1;
        }

        SE_dir_cursor_move();
        return;

    case 8:
        Convert_Buff[3][cursor_id][Menu_Cursor_Y[cursor_id]] += 1;

        if (Convert_Buff[3][cursor_id][Menu_Cursor_Y[cursor_id]] > 1) {
            Convert_Buff[3][cursor_id][Menu_Cursor_Y[cursor_id]] = 0;
        }

        SE_dir_cursor_move();
        return;
    }
}

static void update_direction_cursor_message(void) {
    SE_cursor_move();
    system_dir[1].contents[Menu_Page][Menu_Max] = 1;

    if (Menu_Cursor_Y[0] < Menu_Max) {
        Message_Data->order = 1;
        Message_Data->request = Menu_Page * 0xC + Menu_Cursor_Y[0] * 2 + 1;
        Message_Data->timer = 2;

        if (msgSysDirTbl[0]->msgNum[Menu_Page * 0xC + Menu_Cursor_Y[0] * 2 + 1] == 1) {
            Message_Data->pos_y = 0x36;
        } else {
            Message_Data->pos_y = 0x3E;
        }
    } else {
        Message_Data->order = 1;
        Message_Data->request = system_dir[1].contents[Menu_Page][Menu_Max] + 0x74;
        Message_Data->timer = 2;
        Message_Data->pos_y = 0x36;
    }
}

static void select_previous_direction_page(struct _TASK* task_ptr) {
    task_ptr->r_no[2] = 1;
    task_ptr->timer = 5;

    if (--Menu_Page < 0) {
        Menu_Page = (s8)Page_Max;
    }

    SE_dir_selected();
}

static void select_next_direction_page(struct _TASK* task_ptr) {
    task_ptr->r_no[2] = 1;
    task_ptr->timer = 5;

    if (++Menu_Page > Page_Max) {
        Menu_Page = 0;
    }

    SE_dir_selected();
}

static bool should_change_direction_page(void) {
    return Menu_Cursor_Y[0] == Menu_Max;
}

static bool is_direction_page_backward(u16 result) {
    return result == 0x80 || result == 0x800;
}

static bool is_direction_page_forward(u16 result) {
    return result == 0x40 || result == 0x400;
}

static void handle_direction_page_action(struct _TASK* task_ptr) {
    switch (system_dir[1].contents[Menu_Page][Menu_Max]) {
    case 0:
        task_ptr->r_no[2] = 1;
        task_ptr->timer = 5;

        if (--Menu_Page < 0) {
            Menu_Page = (s8)Page_Max;
        }

        SE_selected();
        return;

    case 2:
        task_ptr->r_no[2] = 1;
        task_ptr->timer = 5;

        if (++Menu_Page > Page_Max) {
            Menu_Page = 0;
        }

        SE_selected();
        return;
    }

    task_ptr->r_no[2] += 1;
    Menu_Suicide[0] = 0;
    Menu_Suicide[1] = 0;
    Menu_Suicide[2] = 1;
    SE_selected();
}

static void handle_direction_menu_input(struct _TASK* task_ptr) {
    if (IO_Result == 0x200) {
        task_ptr->r_no[2] += 1;
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 0;
        Menu_Suicide[2] = 1;
        SE_dir_selected();
        return;
    }

    if (is_direction_page_backward(IO_Result)) {
        select_previous_direction_page(task_ptr);
        return;
    }

    if (is_direction_page_forward(IO_Result)) {
        select_next_direction_page(task_ptr);
        return;
    }

    if (IO_Result == 0x100 && should_change_direction_page()) {
        handle_direction_page_action(task_ptr);
    }
}

static void initialize_direction_menu(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[2] += 1;
    task_ptr->timer = 5;
    Menu_Suicide[1] = 1;
    Menu_Suicide[2] = 0;
    Menu_Page = 0;
    Menu_Page_Buff = Menu_Page;
    Message_Data->kind_req = 3;
}

static void process_direction_menu_input(struct _TASK* task_ptr) {
    Pause_ID = 0;

    Dir_Move_Sub(task_ptr, 0);

    if (IO_Result == 0) {
        Pause_ID = 1;
        Dir_Move_Sub(task_ptr, 1);
    }

    if (Menu_Cursor_Y[1] != Menu_Cursor_Y[0]) {
        update_direction_cursor_message();
    }

    handle_direction_menu_input(task_ptr);
}

static void setup_direction_menu_page(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[2] += 1;
    Setup_Next_Page(task_ptr, 0);
}

static void wait_for_direction_menu_page(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    if (--task_ptr->timer == 0) {
        task_ptr->r_no[2] += 1;
        FadeInit();
    }
}

static void fade_in_direction_menu(struct _TASK* task_ptr) {
    if (FadeIn(1, 0x19, 8) != 0) {
        task_ptr->r_no[2] += 1;
    }
}

void Direction_Menu(struct _TASK* task_ptr) {
    Menu_Cursor_Y[1] = Menu_Cursor_Y[0];

    if (task_ptr->r_no[2] == 0) {
        initialize_direction_menu(task_ptr);
        return;
    }

    if (task_ptr->r_no[2] == 1) {
        setup_direction_menu_page(task_ptr);
    }

    if (task_ptr->r_no[2] == 2) {
        wait_for_direction_menu_page(task_ptr);
        return;
    }

    if (task_ptr->r_no[2] == 3) {
        fade_in_direction_menu(task_ptr);
        return;
    }

    if (task_ptr->r_no[2] == 4) {
        process_direction_menu_input(task_ptr);
        return;
    }

    Exit_Sub(task_ptr, 2, 5);
}

void Dir_Move_Sub(struct _TASK* task_ptr, s16 PL_id) {
    u16 sw;
    u16 ix;

    plsw_00[0] = PLsw[0][0];
    plsw_01[0] = PLsw[0][1];
    plsw_00[1] = PLsw[1][0];
    plsw_01[1] = PLsw[1][1];

    for (ix = 0; ix < 2; ix++) {
        plsw_00[ix] &= 0x4FFF;
        plsw_01[ix] &= 0x4FFF;
    }

    sw = Check_Menu_Lever(PL_id, 0);
    Dir_Move_Sub2(sw);

    if (task_ptr->r_no[1] == 0xE) {
        Ex_Move_Sub_LR(sw, PL_id);
        return;
    }

    Dir_Move_Sub_LR(sw, PL_id);
}

static u16 set_direction_io_result_low(u16 input) {
    switch (input) {
    case 0x10:
    case 0x20:
    case 0x40:
    case 0x80:
    case 0x100:
        return IO_Result = input;
    default:
        return IO_Result = 0;
    }
}

static u16 set_direction_io_result_high(u16 input) {
    bool is_page_input = input == 0x200 || input == 0x400;
    bool is_trigger_input = input == 0x800 || input == 0x4000;

    if (is_page_input || is_trigger_input) {
        return IO_Result = input;
    }
    return IO_Result = 0;
}

static u16 move_direction_cursor(u16 input) {
    if (input == 0x1) {
        Menu_Cursor_Y[0] -= 1;
        if (Menu_Cursor_Y[0] < 0) {
            Menu_Cursor_Y[0] = Menu_Max;
        }
        SE_cursor_move();
        return IO_Result = 1;
    }
    Menu_Cursor_Y[0] += 1;
    if (Menu_Cursor_Y[0] > Menu_Max) {
        Menu_Cursor_Y[0] = 0;
    }
    SE_cursor_move();
    return IO_Result = 2;
}

u16 Dir_Move_Sub2(u16 sw) {
    if (Menu_Cursor_Move > 0) {
        return 0;
    }
    if (sw == 0x1 || sw == 0x2) {
        return move_direction_cursor(sw);
    }
    if (set_direction_io_result_low(sw) != 0) {
        return IO_Result;
    }
    return set_direction_io_result_high(sw);
}

static void move_normal_direction_option(u16 input) {
    if (input == 0x4 && system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] < 0) {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = Dir_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]];
    }
    if (input == 0x8 &&
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > Dir_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 0;
    }
}

static void move_final_direction_option(u16 input, u8 previous_value) {
    if (input == 0x4 && system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] < 0) {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 0;
        IO_Result = 0x80;
        return;
    }
    if (input == 0x8 && system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > 2) {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 2;
        IO_Result = 0x400;
        return;
    }
    if (input == 0x8 && system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > 2) {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 2;
    }
    if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] != previous_value) {
        Message_Data->order = 1;
        Message_Data->request = system_dir[1].contents[Menu_Page][Menu_Max] + 0x74;
        Message_Data->timer = 2;
    }
}

static void move_direction_option(u16 input, u8 previous_value) {
    SE_dir_cursor_move();
    if (input == 0x4) {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] -= 1;
    } else {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] += 1;
    }

    if (Menu_Cursor_Y[0] != Menu_Max) {
        move_normal_direction_option(input);
        return;
    }
    move_final_direction_option(input, previous_value);
}

static void wrap_direction_option_right(void) {
    if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > Dir_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 0;
    }
}

void Dir_Move_Sub_LR(u16 sw, s16 /* unused */) {
    u8 last_pos = system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]];

    switch (sw) {
    case 0x4:
    case 0x8:
        move_direction_option(sw, last_pos);
        return;

    case 0x100:
        SE_dir_cursor_move();

        if (Menu_Cursor_Y[0] == Menu_Max) {
            return;
        } else {
            system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] += 1;
            wrap_direction_option_right();
        }

        return;
    }
}

static void setup_standard_page_items(s16* display_index) {
    s16 ix;
    s16 unused_s3;

    for (ix = 0; ix < Menu_Max; ix++, unused_s3 = *display_index += 2) {
        effect_18_init(*display_index, ix, 0, 2);
        effect_51_init(ix, ix, 2);
    }
}

static void setup_extra_page_items(s16* display_index) {
    s16 ix;
    s16 unused_s3;

    for (ix = 0; ix < Menu_Max; ix++, unused_s3 = *display_index += 2) {
        effect_C4_init(0, ix, ix, 2);
        if (Menu_Page != 0 || ix != (Menu_Max - 1)) {
            effect_C4_init(1, ix, ix, 2);
        }
    }
}

static void setup_menu_page_effects(s16 mode_type, s16* display_index) {
    if (mode_type == 0) {
        setup_standard_page_items(display_index);
    } else {
        setup_extra_page_items(display_index);
    }

    effect_40_init(mode_type, 0, 0x48, 0, 2, 1);
    effect_40_init(mode_type, 1, 0x49, 0, 2, 1);
    effect_40_init(mode_type, 2, 0x4A, 0, 2, 0);
    effect_40_init(mode_type, 3, 0x4B, 0, 2, 2);
}

static void configure_extra_direction_page(void) {
    Menu_Max = Ex_Page_Data[Menu_Page];
    save_w[1].extra_option.contents[Menu_Page][Menu_Max] = 1;
    Order_Dir[0x4E] = 1;
    effect_57_init(0x4E, MENU_HEADER_OPTION_MENU, 0, 0x45, 0);
    Order[0x73] = 3;
    Order_Dir[0x73] = 8;
    Order_Timer[0x73] = 1;
    effect_57_init(0x73, MENU_HEADER_EXTRA_OPTION, 0, 0x3F, 2);
    effect_66_init(0x5C, 0x27, 2, 0, 0x47, 0xB, 0);
    Order[0x5C] = 3;
    Order_Timer[0x5C] = 1;
    effect_66_init(0x5D, 0x28, 2, 0, 0x40, (s16)Menu_Page + 1, 0);
    Order[0x5D] = 3;
    Order_Timer[0x5D] = 1;
    Message_Data->pos_y = msgExtraTbl[0]->msgNum[Menu_Cursor_Y[0] + Menu_Page * 8] == 1 ? 0x36 : 0x3E;
    Message_Data->request = Ex_Account_Data[Menu_Page] + Menu_Cursor_Y[0];
}

static void configure_system_direction_page(s16* display_index) {
    Menu_Max = Page_Data[Menu_Page];
    system_dir[1].contents[Menu_Page][Menu_Max] = 1;
    effect_66_init(0x5B, 0x14, 2, 0, 0x47, 0xA, 0);
    Order[0x5B] = 3;
    Order_Timer[0x5B] = 1;
    Order[0x4E] = 5;
    Order_Dir[0x4E] = 3;
    effect_57_init(0x4E, MENU_HEADER_MODE_MENU, 0, 0x45, 0);
    effect_66_init(0x5C, 0x15, 2, 0, 0x47, 0xB, 0);
    Order[0x5C] = 3;
    Order_Timer[0x5C] = 1;
    effect_66_init(0x5D, 0x16, 2, 0, 0x40, (s16)Menu_Page + 1, 0);
    Order[0x5D] = 3;
    Order_Timer[0x5D] = 1;
    Message_Data->pos_y = msgSysDirTbl[0]->msgNum[Menu_Page * 0xC + Menu_Cursor_Y[0] * 2 + 1] == 1 ? 0x36 : 0x3E;
    *display_index = Menu_Page * 0xC;
    Message_Data->request = *display_index + 1;
}

void Setup_Next_Page(struct _TASK* task_ptr, u8 /* unused */) {
    s16 disp_index;
    s16 mode_type;

    Menu_Page_Buff = Menu_Page;
    effect_work_init();
    Menu_Common_Init();
    Menu_Cursor_Y[0] = 0;
    Order[0x4E] = 5;
    Order_Timer[0x4E] = 1;

    if (task_ptr->r_no[1] == 0xE) {
        mode_type = 1;
        configure_extra_direction_page();
    } else {
        mode_type = 0;
        configure_system_direction_page(&disp_index);
    }

    Menu_Cursor_Y[0] = 0;
    effect_66_init(0x8A, 0x13, 2, 0, -1, -1, -0x8000);
    Order[0x8A] = 3;
    Order_Timer[0x8A] = 1;
    Message_Data->order = 0;
    Message_Data->timer = 1;
    Message_Data->pos_x = 0;
    Message_Data->pos_z = 0x45;
    effect_45_init(0, 0, 2);

    setup_menu_page_effects(mode_type, &disp_index);
}
