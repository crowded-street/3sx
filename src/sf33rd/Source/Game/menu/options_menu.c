/**
 * @file options_menu.c
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
extern const u8 Setup_Index_64[];

static void initialize_game_option_header(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[2] += 1;
    task_ptr->timer = 5;
    Menu_Common_Init();
    Menu_Cursor_Y[0] = 0;
    Menu_Suicide[1] = 1;
    Menu_Suicide[2] = 0;
    Menu_Cursor_Y[0] = 0;
    Menu_Cursor_Y[1] = 0;
    Order[0x4F] = 4;
    Order_Timer[0x4F] = 1;
    Order[0x4E] = 2;
    Order_Dir[0x4E] = 2;
    Order_Timer[0x4E] = 1;
    effect_57_init(0x6A, MENU_HEADER_GAME_OPTION, 0, 0x3F, 2);
    Order[0x6A] = 1;
    Order_Dir[0x6A] = 8;
    Order_Timer[0x6A] = 1;
}

static void initialize_game_options(struct _TASK* task_ptr) {
    s16 char_index;
    s16 ix;
    s16 unused_s3;
    s16 unused_s2;

    initialize_game_option_header(task_ptr);
    for (ix = 0, unused_s3 = char_index = 0x19; ix < 0xC; ix++, unused_s2 = char_index++) {
        effect_61_init(0, ix + 0x50, 0, 2, char_index, ix, 0x70A7);
        Order[ix + 0x50] = 1;
        Order_Dir[ix + 0x50] = 4;
        Order_Timer[ix + 0x50] = ix + 0x14;
    }
    Menu_Cursor_Move = 0xA;
    for (ix = 0; ix < 0xA; ix++) {
        effect_64_init(ix + 0x5D, 0, 2, Setup_Index_64[ix], ix, 0x70A7, ix + 1, 0, 0);
        Order[ix + 0x5D] = 1;
        Order_Dir[ix + 0x5D] = 4;
        Order_Timer[ix + 0x5D] = ix + 0x14;
    }
}

void Game_Option(struct _TASK* task_ptr) {

    switch (task_ptr->r_no[2]) {
    case 0:
        initialize_game_options(task_ptr);
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
        Game_Option_Sub(0);
        Button_Exit_Check(task_ptr, 0);
        Game_Option_Sub(1);
        Button_Exit_Check(task_ptr, 1);
        Save_Game_Data();
        break;

    default:
        Exit_Sub(task_ptr, 2, 5);
        break;
    }
}

static u16 option_cursor_sub(s16 player_id, s16 menu_max, u16 (*move_left_right)(u16)) {
    u16 input;
    u16 ret;

    input = ~plsw_01[player_id] & plsw_00[player_id];
    input = Check_Menu_Lever(player_id, 0);
    ret = MC_Move_Sub(input, 0, menu_max, 0xFF);
    ret |= move_left_right(input);
    ret &= 0x20F;
    return ret;
}

static u16 move_game_option(u16 input) {
    return GO_Move_Sub_LR(input, 0);
}

u16 Game_Option_Sub(s16 PL_id) {
    return option_cursor_sub(PL_id, 0xB, move_game_option);
}

const u8 Game_Option_Index_Data[10] = { 7, 3, 3, 3, 3, 1, 1, 1, 1, 1 };

u16 GO_Move_Sub_LR(u16 sw, s16 cursor_id) {
    if (Menu_Cursor_Y[cursor_id] > 9) {
        return 0;
    }

    switch (sw) {
    case 4:
        Convert_Buff[0][cursor_id][Menu_Cursor_Y[cursor_id]] -= 1;

        if (Convert_Buff[0][cursor_id][Menu_Cursor_Y[cursor_id]] < 0) {
            Convert_Buff[0][cursor_id][Menu_Cursor_Y[cursor_id]] = Game_Option_Index_Data[Menu_Cursor_Y[cursor_id]];
        }

        SE_dir_cursor_move();
        return 4;

    case 8:
        Convert_Buff[0][cursor_id][Menu_Cursor_Y[cursor_id]] += 1;

        if (Convert_Buff[0][cursor_id][Menu_Cursor_Y[cursor_id]] > Game_Option_Index_Data[Menu_Cursor_Y[cursor_id]]) {
            Convert_Buff[0][cursor_id][Menu_Cursor_Y[cursor_id]] = 0;
        }

        SE_dir_cursor_move();
        return 8;

    default:
        return 0;
    }
}

typedef struct {
    s16 count;
    s16 base_1p;
    s16 base_2p;
    s16 display_index;
    s16 final_display_index;
    s16 action_offset;
    s16 action_type;
} ButtonRowLayout;

static void initialize_button_rows(const ButtonRowLayout* layout) {
    s16 ix;
    s16 display_index;
    s16 action_index;
    for (ix = 0; ix < layout->count; ix++) {
        display_index = ix == layout->count - 1 ? layout->final_display_index : layout->display_index;
        action_index = ix + layout->action_offset;
        effect_23_init(0, ix + layout->base_1p, 0, 2, display_index, ix, 0x70A7, action_index, layout->action_type);
        Order[ix + layout->base_1p] = 1;
        Order_Dir[ix + layout->base_1p] = 4;
        Order_Timer[ix + layout->base_1p] = ix + 0x14;
        effect_23_init(1, ix + layout->base_2p, 0, 2, display_index, ix, 0x70A7, action_index, layout->action_type);
        Order[ix + layout->base_2p] = 1;
        Order_Dir[ix + layout->base_2p] = 4;
        Order_Timer[ix + layout->base_2p] = ix + 0x14;
    }
}

static void initialize_button_labels(void) {
    ButtonRowLayout layout = { 12, 0x50, 0x5C, 2, 2, 9, 1 };
    initialize_button_rows(&layout);
}

static void initialize_button_values(void) {
    ButtonRowLayout layout = { 9, 0x78, 0x81, 0, 1, 0, 0 };
    initialize_button_rows(&layout);
}

void Button_Config(struct _TASK* task_ptr) {

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        task_ptr->timer = 5;
        Menu_Common_Init();
        pp_operator_check_flag(0);
        Menu_Cursor_Y[0] = 0;
        Menu_Cursor_Y[1] = 0;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Copy_Key_Disp_Work();
        Order[0x4F] = 4;
        Order_Timer[0x4F] = 1;
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 2;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x6B, MENU_HEADER_BUTTON_CONFIG, 0, 0x3F, 2);
        Order[0x6B] = 1;
        Order_Dir[0x6B] = 8;
        Order_Timer[0x6B] = 1;

        initialize_button_labels();
        initialize_button_values();

        Menu_Cursor_Move = 0x22;
        effect_66_init(0x8A, 7, 2, 0, -1, -1, -0x7FFF);
        Order[0x8A] = 1;
        Order_Dir[0x8A] = 4;
        Order_Timer[0x8A] = 0x14;
        effect_66_init(0x8B, 8, 2, 0, -1, -1, -0x7FFF);
        Order[0x8B] = 1;
        Order_Dir[0x8B] = 4;
        Order_Timer[0x8B] = 0x14;
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
        Button_Config_Sub(0);
        Button_Exit_Check(task_ptr, 0);
        Button_Config_Sub(1);
        Button_Exit_Check(task_ptr, 1);
        Save_Game_Data();
        break;
    }
}

void Button_Config_Sub(s16 PL_id) {
    u16 sw = ~plsw_01[PL_id] & plsw_00[PL_id];
    sw = Check_Menu_Lever(PL_id, 0);
    MC_Move_Sub(sw, PL_id, 0xA, 0xFF);
    Button_Move_Sub_LR(sw, PL_id);

    if (ppwork[0].ok_dev == 0) {
        Convert_Buff[1][0][8] = 0;
    }

    if (ppwork[1].ok_dev == 0) {
        Convert_Buff[1][1][8] = 0;
    }
}

static s16 button_option_max(s16 cursor_id) {
    switch (Menu_Cursor_Y[cursor_id]) {
    case 8:
        return 1;

    case 9:
    case 10:
        return 0;

    default:
        return 11;
    }
}

static void update_button_vibration(u16 input, s16 cursor_id) {
    if (Menu_Cursor_Y[cursor_id] != 8) {
        return;
    }
    if (Convert_Buff[1][cursor_id][8]) {
        pp_vib_on(cursor_id);
    } else if (input == 4) {
        pulpul_stop2(cursor_id);
    }
}

static void adjust_button_option_left(s16 cursor_id, s16 maximum_value) {
    Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] -= 1;
    if (Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] < 0) {
        Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] = maximum_value;
    }
}

static void adjust_button_option_right(s16 cursor_id, s16 maximum_value) {
    Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] += 1;
    if (Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] > maximum_value) {
        Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] = 0;
    }
}

static void move_button_option(u16 input, s16 cursor_id, s16 maximum_value) {
    if (input == 4) {
        adjust_button_option_left(cursor_id, maximum_value);
    } else {
        adjust_button_option_right(cursor_id, maximum_value);
    }
    update_button_vibration(input, cursor_id);
    SE_dir_cursor_move();
}

void Button_Move_Sub_LR(u16 sw, s16 cursor_id) {
    s16 max = button_option_max(cursor_id);

    if (max == 0) {
        return;
    }

    switch (sw) {
    case 4:
    case 8:
        move_button_option(sw, cursor_id, max);
        break;
    }
}

static void exit_game_options(struct _TASK* task_ptr) {
    SE_selected();
    Return_Option_Mode_Sub(task_ptr);
    Order[0x6A] = 4;
    Order_Timer[0x6A] = 4;
}

static void reset_game_options(void) {
    SE_selected();
    save_w[1].Difficulty = Game_Default_Data.Difficulty;
    save_w[1].Time_Limit = Game_Default_Data.Time_Limit;
    save_w[1].Battle_Number[0] = Game_Default_Data.Battle_Number[0];
    save_w[1].Battle_Number[1] = Game_Default_Data.Battle_Number[1];
    save_w[1].Damage_Level = Game_Default_Data.Damage_Level;
    save_w[1].GuardCheck = Game_Default_Data.GuardCheck;
    save_w[1].AnalogStick = Game_Default_Data.AnalogStick;
    save_w[1].Handicap = Game_Default_Data.Handicap;
    save_w[1].Partner_Type[0] = Game_Default_Data.Partner_Type[0];
    save_w[1].Partner_Type[1] = Game_Default_Data.Partner_Type[1];
    Copy_Save_w();
}

static void check_game_option_exit(struct _TASK* task_ptr) {
    if (Menu_Cursor_Y[0] == 11 || IO_Result == 0x200) {
        exit_game_options(task_ptr);
        return;
    }
    if (Menu_Cursor_Y[0] == 10) {
        reset_game_options();
    }
}

static void check_button_config_exit(struct _TASK* task_ptr, s16 player_id) {
    if ((Menu_Cursor_Y[player_id] == 10) || (IO_Result == 0x200)) {
        SE_selected();
        Return_Option_Mode_Sub(task_ptr);
        Order[0x6B] = 4;
        Order_Timer[0x6B] = 4;
        return;
    }
    if (Menu_Cursor_Y[player_id] == 9) {
        SE_selected();
        Setup_IO_ConvDataDefault(player_id);
        Save_Game_Data();
    }
}

static void select_option_menu_item(struct _TASK* task_ptr) {
    if (IO_Result == 0x200 || Menu_Cursor_Y[0] == 2) {
        SE_selected();
        Return_Option_Mode_Sub(task_ptr);
        Order[0x69] = 4;
        Order_Timer[0x69] = 4;
        return;
    }
    if (Menu_Cursor_Y[0] != 0 && Menu_Cursor_Y[0] != 1) {
        return;
    }
    SE_selected();
    if (Menu_Cursor_Y[0] == 0) {
        task_ptr->r_no[2] = 4;
        task_ptr->r_no[3] = 0;
    } else if (Menu_Cursor_Y[0] == 1) {
        task_ptr->r_no[2] = 5;
        task_ptr->r_no[3] = 0;
    }
}

void Button_Exit_Check(struct _TASK* task_ptr, s16 PL_id) {
    switch (IO_Result) {
    case 0x200:
    case 0x100:
        break;

    default:
        return;
    }

    switch (task_ptr->r_no[1]) {
    case 9:
        check_game_option_exit(task_ptr);
        break;

    case 10:
        check_button_config_exit(task_ptr, PL_id);
        break;

    case 13:
        select_option_menu_item(task_ptr);
        break;
    }
}

void Return_Option_Mode_Sub(struct _TASK* task_ptr) {
    Menu_Suicide[1] = 0;
    Menu_Suicide[2] = 1;
    task_ptr->r_no[1] = 7;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    task_ptr->free[0] = 0;
    Cursor_Y_Pos[0][2] = Menu_Cursor_Y[0];
    Cursor_Y_Pos[1][2] = Menu_Cursor_Y[1];
}

static void initialize_screen_adjust_controls(void) {
    s16 ix;
    for (ix = 0; ix < 4; ix++) {
        effect_63_init(ix + 0x66, 0, 2, ix, ix);
        Order[ix + 0x66] = 1;
        Order_Dir[ix + 0x66] = 4;
        Order_Timer[ix + 0x66] = ix + 0x14;
    }
}

static void initialize_screen_adjust_labels(void) {
    s16 char_index;
    s16 ix;
    s16 unused_s3;
    s16 unused_s2;
    for (ix = 0, unused_s3 = char_index = 0xE; ix < 7; ix++, unused_s2 = char_index++) {
        effect_61_init(0, ix + 0x50, 0, 2, char_index, ix, 0x7047);
        Order[ix + 0x50] = 1;
        Order_Dir[ix + 0x50] = 4;
        Order_Timer[ix + 0x50] = ix + 0x14;
    }
}

void Screen_Adjust(struct _TASK* task_ptr) {

    X_Adjust = X_Adjust_Buff[0];
    X_Adjust_Buff[0] = X_Adjust_Buff[1];
    X_Adjust_Buff[1] = X_Adjust_Buff[2];
    Y_Adjust = Y_Adjust_Buff[0];
    Y_Adjust_Buff[0] = Y_Adjust_Buff[1];
    Y_Adjust_Buff[1] = Y_Adjust_Buff[2];

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        task_ptr->timer = 5;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Order[0x4F] = 4;
        Order_Timer[0x4F] = 1;
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 2;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x65, MENU_HEADER_SCREEN_ADJUST, 0, 0x3F, 2);
        Order[0x65] = 1;
        Order_Dir[0x65] = 8;
        Order_Timer[0x65] = 1;

        Convert_Buff[2][0][4] = mpp_w.language;

        initialize_screen_adjust_controls();

        effect_64_init(0x6A, 0, 2, 9, 4, 0x7047, 18, 2, 0);
        Order[0x6A] = 1;
        Order_Dir[0x6A] = 4;
        Order_Timer[0x6A] = 0x18;

        initialize_screen_adjust_labels();

        Menu_Cursor_Move = 5;
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
        Screen_Adjust_Sub(0);
        Screen_Exit_Check(task_ptr, 0);

        if (IO_Result == 0) {
            Screen_Adjust_Sub(1);
            Screen_Exit_Check(task_ptr, 0);
        }

        Save_Game_Data();
        break;
    }
}

void Screen_Adjust_Sub(s16 PL_id) {
    u16 sw;
    sw = ~plsw_01[PL_id] & plsw_00[PL_id];
    sw = Check_Menu_Lever(PL_id, 0);
    MC_Move_Sub(sw, 0, 6, 0xFF);
    Screen_Move_Sub_LR(sw);
    Convert_Buff[2][0][0] = X_Adjust_Buff[2] & 0xFF;
    Convert_Buff[2][0][1] = Y_Adjust_Buff[2] & 0xFF;
    Convert_Buff[2][0][2] = dspwhPack(Disp_Size_H, Disp_Size_V);
    save_w[1].Screen_Size = dspwhPack(Disp_Size_H, Disp_Size_V);
    Convert_Buff[2][0][4] = mpp_w.language;
}

void Screen_Exit_Check(struct _TASK* task_ptr, s16 PL_id) {
    switch (IO_Result) {
    case 0x200:
    case 0x100:
        break;

    default:
        return;
    }

    if (Menu_Cursor_Y[0] == 6 || IO_Result == 0x200) {
        SE_selected();
        Menu_Suicide[1] = 0;
        Menu_Suicide[2] = 1;
        X_Adjust = X_Adjust_Buff[2];
        Y_Adjust = Y_Adjust_Buff[2];
        Return_Option_Mode_Sub(task_ptr);

        if (task_ptr->r_no[0] == 1) {
            task_ptr->r_no[1] = 1;
        } else {
            task_ptr->r_no[1] = 7;
            Order[0x65] = 4;
            Order_Timer[0x65] = 4;
        }

        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        task_ptr->free[0] = 0;
        return;
    }

    if (Menu_Cursor_Y[PL_id] == 5) {
        SE_selected();
        X_Adjust_Buff[2] = 0;
        Y_Adjust_Buff[2] = 0;
        Disp_Size_H = 100;
        Disp_Size_V = 100;
        mpp_w.language = Get_Default_Language();
    }
}

u16 Sound_Cursor_Sub(s16 PL_id) {
    return option_cursor_sub(PL_id, 5, SD_Move_Sub_LR);
}

const u8 Sound_Data_Max[3][5] = { { 0, 0, 1, 0, 66 }, { 15, 15, 1, 0, 66 }, { 15, 15, 0, 0, 0 } };

static s16 sound_cursor_max(u16 input) {
    if (input == 4) {
        return Sound_Data_Max[0][Menu_Cursor_Y[0]];
    }
    return Sound_Data_Max[1][Menu_Cursor_Y[0]];
}

static void advance_sound_cursor(u16 input, s16 maximum_value) {
    if (input == 4) {
        Convert_Buff[3][1][Menu_Cursor_Y[0]] -= 1;
    } else {
        Convert_Buff[3][1][Menu_Cursor_Y[0]] += 1;
    }
    if (input == 4 && Convert_Buff[3][1][Menu_Cursor_Y[0]] < 0) {
        Convert_Buff[3][1][Menu_Cursor_Y[0]] = maximum_value;
    }
    if (input == 8 && Convert_Buff[3][1][Menu_Cursor_Y[0]] > maximum_value) {
        Convert_Buff[3][1][Menu_Cursor_Y[0]] = Sound_Data_Max[2][Menu_Cursor_Y[0]];
    }
}

static bool is_selectable_sound_cursor(void) {
    return (Menu_Cursor_Y[0] != 4) || (bgmSkipCheck(Convert_Buff[3][1][4] + 1) == 0);
}

static u16 move_sound_cursor(u16 input, s8 last_cursor) {
    s16 maximum_value = sound_cursor_max(input);
    while (1) {
        advance_sound_cursor(input, maximum_value);
        if (is_selectable_sound_cursor()) {
            break;
        }
    }
    return last_cursor != Convert_Buff[3][1][Menu_Cursor_Y[0]] ? input : 0;
}

u16 SD_Move_Sub_LR(u16 sw) {
    u16 rnum;
    s8 last_cursor;

    rnum = 0;

    if (Menu_Cursor_Y[0] == 3 || Menu_Cursor_Y[0] == 5) {
        return 0;
    }

    last_cursor = Convert_Buff[3][1][Menu_Cursor_Y[0]];

    switch (sw) {
    case 4:
    case 8:
        rnum = move_sound_cursor(sw, last_cursor);
        break;
    }

    if (rnum) {
        SE_dir_cursor_move();
    }

    return rnum;
}

static bool wrap_cursor_at_boundary(s16* cursor, bool below_range) {
    bool in_range;
    s16 adjustment;

    if (below_range) {
        in_range = *cursor >= 0;
        adjustment = 8;
    } else {
        in_range = *cursor <= 7;
        adjustment = -8;
    }
    if (in_range) {
        return true;
    }
    if (IO_Result == 0) {
        *cursor += adjustment;
        return true;
    }
    return false;
}

static s32 connected_cursor_at_or_pair(s16 cursor) {
    if (vm_w.Connect[cursor]) {
        return cursor;
    }
    cursor ^= 1;
    if (vm_w.Connect[cursor]) {
        return cursor;
    }
    return -1;
}

s32 Setup_Final_Cursor_Pos(s8 cursor_x, s16 dir) {
    s16 ix;
    s16 check_x;
    s16 next_dir;
    s32 connected_cursor;

    if (cursor_x == -1) {
        cursor_x = 0;
    }

    connected_cursor = connected_cursor_at_or_pair(cursor_x);
    if (connected_cursor != -1) {
        return connected_cursor;
    }

    if (dir == 4) {
        next_dir = -2;
    } else {
        next_dir = 2;
    }

    check_x = cursor_x;

    for (ix = 0; ix < 4; ix++) {
        check_x += next_dir;
        if (!wrap_cursor_at_boundary(&check_x, true)) {
            return Menu_Cursor_X[1];
        }
        if (!wrap_cursor_at_boundary(&check_x, false)) {
            return Menu_Cursor_X[1];
        }
        connected_cursor = connected_cursor_at_or_pair(check_x);
        if (connected_cursor != -1) {
            return connected_cursor;
        }
    }

    return -1;
}

static bool should_skip_menu_cursor(s16 cursor_id, s16 cancel_cursor_index) {
    return (cancel_cursor_index == Menu_Cursor_Y[cursor_id]) && (Connect_Status == 0);
}

static u16 move_menu_cursor(u16 input, s16 cursor_id, s16 menu_max, s16 cancel_cursor_index) {
    if (input == SWK_UP) {
        Menu_Cursor_Y[cursor_id] -= 1;
    } else {
        Menu_Cursor_Y[cursor_id] += 1;
    }
    if (Menu_Cursor_Y[cursor_id] < 0) {
        Menu_Cursor_Y[cursor_id] = menu_max;
    }
    if (Menu_Cursor_Y[cursor_id] > menu_max) {
        Menu_Cursor_Y[cursor_id] = 0;
    }
    if (should_skip_menu_cursor(cursor_id, cancel_cursor_index) && input == SWK_UP) {
        Menu_Cursor_Y[cursor_id] -= 1;
    }
    if (should_skip_menu_cursor(cursor_id, cancel_cursor_index) && input == SWK_DOWN) {
        Menu_Cursor_Y[cursor_id] += 1;
    }
    SE_cursor_move();
    return IO_Result = input;
}

static u16 pass_through_menu_input(u16 input) {
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

static bool is_menu_shoulder_input(u16 input) {
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

static u16 pass_through_menu_shoulder_input(u16 input) {
    if (is_menu_shoulder_input(input)) {
        return IO_Result = input;
    }
    return IO_Result = 0;
}

u16 MC_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max, s16 cansel_menu) {
    if (Menu_Cursor_Move > 0) {
        return 0;
    }

    if (sw == SWK_UP || sw == SWK_DOWN) {
        return move_menu_cursor(sw, cursor_id, menu_max, cansel_menu);
    }
    if (pass_through_menu_input(sw) != 0) {
        return IO_Result;
    }
    return pass_through_menu_shoulder_input(sw);
}

s32 Exit_Sub(struct _TASK* task_ptr, s16 cursor_ix, s16 next_routine) {
    switch (task_ptr->free[0]) {
    case 0:
        task_ptr->free[0] += 1;
        FadeInit();
        /* fallthrough */

    case 1:
        if (!FadeOut(1, 25, 8)) {
            return 0;
        }

        task_ptr->r_no[1] = next_routine;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        task_ptr->free[0] = 0;
        Cursor_Y_Pos[0][cursor_ix] = Menu_Cursor_Y[0];
        Cursor_Y_Pos[1][cursor_ix] = Menu_Cursor_Y[1];
        pulpul_stop();
        return 1;

    default:
        return 0;
    }
}
