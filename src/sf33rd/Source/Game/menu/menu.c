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

typedef void (*MenuFunc)(struct _TASK*);

typedef struct {
    s16 pos_x;
    s8* menu;
} LetterData;

const MenuFunc Menu_Jmp_Tbl[14] = {
    After_Title,   In_Game,      Wait_Load_Save, Wait_Replay_Check, After_Title,    Suspend_Menu, Wait_Replay_Load,
    Training_Menu, After_Replay, After_Replay,   Wait_Pause_in_Tr,  Reset_Training, Reset_Replay, End_Replay_Menu,
};

u8 r_no_plus;
u8 control_player;
u8 control_pl_rno;

extern const LetterData training_letter_data[6];

void Menu_Task(struct _TASK* task_ptr) {
    if (nowSoftReset()) {
        return;
    }

    if (Interface_Type[0] == 0 || Interface_Type[1] == 0) {
        Connect_Status = 0;
    } else {
        Connect_Status = 1;
    }

    Setup_Pad_or_Stick();
    IO_Result = 0;
    Menu_Jmp_Tbl[task_ptr->r_no[0]](task_ptr);
}

void Setup_Pad_or_Stick() {
    plsw_00[0] = PLsw[0][0];
    plsw_01[0] = PLsw[0][1];
    plsw_00[1] = PLsw[1][0];
    plsw_01[1] = PLsw[1][1];
}

void After_Title(struct _TASK* task_ptr) {
    void (*AT_Jmp_Tbl[19])() = { Menu_Init,     Mode_Select,      Option_Select, Option_Select,
                                 Training_Mode, System_Direction,
#if NETPLAY_ENABLED
                                 Netplay_Menu,
#else
                                 Load_Replay,
#endif
                                 Option_Select, toSelectGame,     Game_Option,   Button_Config,
                                 Screen_Adjust, Sound_Test,       Option_Select, Extra_Option,
                                 Option_Select, VS_Result,        Save_Replay,   Direction_Menu };

    AT_Jmp_Tbl[task_ptr->r_no[1]](task_ptr);
}

void Menu_Init(struct _TASK* task_ptr) {
    s16 ix;
    s16 fade_on;

    if (Pause_Type == 2) {
        task_ptr->r_no[1] = 4;
    } else {
        task_ptr->r_no[1] = 1;
    }

    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    Menu_Cursor_Y[0] = 0;
    Menu_Cursor_Y[1] = 0;

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 0;
        Unsubstantial_BG[ix] = 0;
        Cursor_Y_Pos[0][ix] = 0;
    }

    All_Clear_Suicide();
    pulpul_stop();

    if (task_ptr->r_no[0] == 0) {
        FadeOut(1, 0xFF, 8);
        bg_etc_write_ex(2);
        Setup_Virtual_BG(0, 0x200, 0);
        Setup_BG(1, 0x200, 0);
        Setup_BG(2, 0x200, 0);
        base_y_pos = 0;

        if (task_ptr->r_no[1] != 0x12) {
            fade_on = 0;
        } else {
            fade_on = 1;
        }

        Order[0x4E] = 5;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x4E, MENU_HEADER_MODE_MENU, 0, 0x45, fade_on);
        load_any_texture_patnum(0x7F30, 0xC, 0);
    }

    cpReadyTask(TASK_SAVER, Saver_Task);
}


const u8 Setup_Index_64[10] = { 1, 2, 3, 3, 4, 5, 6, 7, 8, 8 };

const u8 Menu_Deley_Time[6] = { 15, 10, 6, 15, 15, 15 };

void Menu_Common_Init() {
    s16 ix;

    for (ix = 0; ix < 2; ix++) {
        Deley_Shot_No[ix] = 0;
        Deley_Shot_Timer[ix] = Menu_Deley_Time[Deley_Shot_No[ix]];
    }

    Menu_Cursor_Move = 0;
    r_no_plus = 0;
}

u16 Check_Menu_Lever(u8 PL_id, s16 type) {
    u16 sw;
    u16 lever;
    u16 ix;

    sw = ~plsw_01[PL_id] & plsw_00[PL_id];

    if (type) {
        sw = ~PLsw[PL_id][1] & PLsw[PL_id][0];
    }

    lever = plsw_00[PL_id] & SWK_DIRECTIONS;

    if (sw & (SWK_ATTACKS | SWK_START)) {
        return sw;
    }

    sw &= SWK_DIRECTIONS;

    if (sw) {
        return sw;
    }

    if (lever == 0) {
        Deley_Shot_No[PL_id] = 0;
        Deley_Shot_Timer[PL_id] = Menu_Deley_Time[Deley_Shot_No[PL_id]];
        return 0;
    }

    if (--Deley_Shot_Timer[PL_id] == 0) {
        if (++Deley_Shot_No[PL_id] > 2) {
            Deley_Shot_No[PL_id] = 2;
        }

        if (lever & (SWK_UP | SWK_DOWN)) {
            ix = 0;
        } else {
            ix = 3;
        }

        Deley_Shot_Timer[PL_id] = Menu_Deley_Time[Deley_Shot_No[PL_id] + ix];
        return lever;
    }

    return 0;
}

void Suspend_Menu(struct _TASK* /* unused */) {
    // Do nothing
}

void In_Game(struct _TASK* task_ptr) {
    void (*In_Game_Jmp_Tbl[5])() = { Menu_Init, Menu_Select, Button_Config_in_Game, Character_Change, Pad_Come_Out };
    In_Game_Jmp_Tbl[task_ptr->r_no[1]](task_ptr);
}

static void menu_select_confirm(struct _TASK* task_ptr) {
    s16 ix;

    switch (Menu_Cursor_Y[0]) {
    case 0: // Continue
        task_ptr->r_no[2] = 99;
        Exit_Menu = 1;
        SE_selected();
        break;

    case 1: // Button config
        SE_selected();

        switch (Mode_Type) {
        case MODE_VERSUS:
            task_ptr->r_no[1] = 3;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;

            for (ix = 0; ix < 4; ix++) {
                Menu_Suicide[ix] = 1;
            }

            cpExitTask(TASK_SAVER);
            cpExitTask(TASK_PAUSE);
            BGM_Stop();
            break;

        case MODE_REPLAY:
            task_ptr->r_no[0] = 0xC;
            task_ptr->r_no[1] = 0;
            break;

        default:
            Menu_Suicide[0] = 1;
            Menu_Suicide[1] = 1;
            Menu_Suicide[2] = 1;
            Menu_Suicide[3] = 0;
            task_ptr->r_no[1]++;
            task_ptr->r_no[2] = 0;
            task[TASK_PAUSE].r_no[2] = 3;
            break;
        }

        break;

    case 2: // Exit
        task_ptr->r_no[2]++;
        Menu_Suicide[0] = 1;
        Menu_Cursor_Y[0] = 1;
        effect_10_init(0, 0, 3, 3, 1, 0x13, 0xC);
        effect_10_init(0, 1, 0, 0, 1, 0x14, 0xF);
        effect_10_init(0, 1, 1, 1, 1, 0x1A, 0xF);
        SE_selected();
        break;
    }
}

static void initialize_pause_menu(struct _TASK* task_ptr) {
    task_ptr->r_no[2]++;
    Menu_Common_Init();
    Menu_Cursor_Y[0] = Cursor_Y_Pos[0][0];
    Menu_Suicide[0] = 0;
    Menu_Suicide[1] = 0;
    Menu_Suicide[2] = 0;
    effect_10_init(0, 0, 0, 0, 0, 0x14, 0xC);
    effect_10_init(0, 0, 2, 2, 0, 0x16, 0x10);
    switch (Mode_Type) {
    case MODE_VERSUS:
        effect_10_init(0, 0, 1, 5, 0, 0x10, 0xE);
        break;
    case MODE_REPLAY:
        effect_10_init(0, 0, 1, 4, 0, 0x15, 0xE);
        break;
    default:
        effect_10_init(0, 0, 1, 1, 0, 0x11, 0xE);
        break;
    }
}

static void handle_pause_menu_input(struct _TASK* task_ptr) {
    IO_Result = MC_Move_Sub(Check_Menu_Lever(Pause_ID, 0), 0, 2, 0xFF);
    switch (IO_Result) {
    case SWK_START:
    case SWK_EAST:
        task_ptr->r_no[2] = 99;
        Exit_Menu = 1;
        SE_selected();
        break;
    case SWK_SOUTH:
        menu_select_confirm(task_ptr);
        break;
    }
}

void Menu_Select(struct _TASK* task_ptr) {
    if (Check_Pad_in_Pause(task_ptr) != 0) {
        return;
    }

    switch (task_ptr->r_no[2]) {
    case 0:
        task_ptr->r_no[2] += 1;
        Cursor_Y_Pos[0][0] = 0;
        /* fallthrough */

    case 1:
        initialize_pause_menu(task_ptr);
        break;

    case 2:
        handle_pause_menu_input(task_ptr);
        break;

    case 3:
        Yes_No_Cursor_Move_Sub(task_ptr);
        break;
    }
}

static void move_yes_no_cursor_up(void) {
    Menu_Cursor_Y[0]--;
    if (Menu_Cursor_Y[0] < 0) {
        Menu_Cursor_Y[0] = 0;
    } else {
        SE_dir_cursor_move();
    }
}

static void move_yes_no_cursor_down(void) {
    Menu_Cursor_Y[0]++;
    if (Menu_Cursor_Y[0] > 1) {
        Menu_Cursor_Y[0] = 1;
    } else {
        SE_dir_cursor_move();
    }
}

static s32 confirm_yes_no_cursor(struct _TASK* task_ptr, u16 sw) {
    if (Menu_Cursor_Y[0] || sw == 0x200) {
        task_ptr->r_no[2] = 1;
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        Cursor_Y_Pos[0][0] = 2;
        return 1;
    }
    Soft_Reset_Sub();
    return -1;
}

s32 Yes_No_Cursor_Move_Sub(struct _TASK* task_ptr) {
    u16 sw = ~(plsw_01[Pause_ID]) & plsw_00[Pause_ID];
    switch (sw) {
    case 0x4:
        move_yes_no_cursor_up();
        break;
    case 0x8:
        move_yes_no_cursor_down();
        break;
    case 0x200:
    case 0x100:
        return confirm_yes_no_cursor(task_ptr, sw);
    }

    return 0;
}

void Button_Config_in_Game(struct _TASK* task_ptr) {
    if (Check_Pad_in_Pause(task_ptr) != 0) {
        Order[0x8A] = 3;
        Order_Timer[0x8A] = 1;
        effect_66_init(0x8A, 9, 2, 7, -1, -1, -0x3FFC);
        return;
    }

    switch (task_ptr->r_no[2]) {
    case 0:
        task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Cursor_Y[1] = 0;
        Copy_Key_Disp_Work();
        Setup_Button_Sub(6, 5, 3);
        Order[0x8A] = 3;
        Order_Timer[0x8A] = 1;
        effect_66_init(0x8B, 0xA, 3, 7, -1, -1, -0x3FFB);
        Order[0x8B] = 3;
        Order_Timer[0x8B] = 1;
        effect_66_init(0x8C, 0xB, 3, 7, -1, -1, -0x3FFB);
        Order[0x8C] = 3;
        Order_Timer[0x8C] = 1;
        break;

    case 1:
        Button_Config_Sub(0);
        Button_Exit_Check_in_Game(task_ptr, 0);
        Button_Config_Sub(1);
        Button_Exit_Check_in_Game(task_ptr, 1);
        Save_Game_Data();
        break;
    }
}

void Setup_Button_Sub(s16 x, s16 y, s16 master_player) {
    s16 ix;
    s16 s1;

    effect_10_init(0, 7, 99, 0, master_player, x + 7, y + 20);
    effect_10_init(0, 7, 99, 1, master_player, x + 29, y + 20);

    for (ix = 0; ix < 8; ix++, s1 = y += 2) {
        effect_10_init(0, 5, ix, ix, master_player, x, y);
        effect_10_init(1, 5, ix, ix, master_player, x + 22, y);
        effect_10_init(0, 2, ix, Convert_Buff[1][0][ix], master_player, x + 3, y);
        effect_10_init(1, 2, ix, Convert_Buff[1][1][ix], master_player, x + 25, y);
    }

    effect_10_init(0, 3, 8, Convert_Buff[1][0][8], master_player, x, y);
    effect_10_init(1, 3, 8, Convert_Buff[1][1][8], master_player, x + 22, y);
    effect_10_init(0, 4, 9, 0, master_player, x, y + 2);
    effect_10_init(1, 4, 9, 0, master_player, x + 22, y + 2);
    effect_10_init(0, 0, 10, 2, master_player, x, y + 4);
    effect_10_init(1, 0, 10, 2, master_player, x + 22, y + 4);
}

void Button_Exit_Check_in_Game(struct _TASK* task_ptr, s16 PL_id) {
    if (IO_Result & 0x200) {
        goto ten;
    }

    if (!(IO_Result & 0x100)) {
        return;
    }

    if (Menu_Cursor_Y[PL_id] == 10) {
    ten:
        SE_selected();
        Return_Pause_Sub(task_ptr);
        return;
    }

    if (Menu_Cursor_Y[PL_id] == 9) {
        SE_selected();
        Setup_IO_ConvDataDefault(PL_id);
    }
}

void Return_Pause_Sub(struct _TASK* task_ptr) {
    Menu_Suicide[0] = 0;
    Menu_Suicide[1] = 0;
    Menu_Suicide[2] = 0;
    Menu_Suicide[3] = 1;
    task[TASK_PAUSE].r_no[2] = 2;
    task[TASK_PAUSE].free[0] = 1;
    task_ptr->r_no[1] = 1;
    task_ptr->r_no[2] = 1;
    Cursor_Y_Pos[0][0] = 1;
    Order[138] = 3;
    Order_Timer[138] = 1;
    effect_66_init(138, 9, 2, 7, -1, -1, -0x3FFC);
}

s32 Check_Pad_in_Pause(struct _TASK* task_ptr) {
    if (Interface_Type[Pause_ID] == 0) {
        task_ptr->r_no[1] = 4;
        task[TASK_PAUSE].r_no[2] = 4;
        Menu_Suicide[0] = 1;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Menu_Suicide[3] = 1;
        return 1;
    }

    return 0;
}

void Pad_Come_Out(struct _TASK* /* unused */) {}

void bg_etc_write_ex(s16 type) {
    u8 i;

    Family_Init();
    Scrn_Pos_Init();
    Zoomf_Init();
    scr_sc = 1.0f;
    bg_w.bg_opaque = 224;
    bg_w.pos_offset = 192;

    for (i = 0; i < 7; i++) {
        bg_w.bgw[i].pos_x_work = 0;
        bg_w.bgw[i].pos_y_work = 0;
        bg_w.bgw[i].zuubun = 0;
        bg_w.bgw[i].xy[0].cal = 0;
        bg_w.bgw[i].xy[1].cal = 0;
        bg_w.bgw[i].wxy[0].cal = 0;
        bg_w.bgw[i].wxy[1].cal = 0;
        bg_w.bgw[i].hos_xy[0].cal = 0;
        bg_w.bgw[i].hos_xy[1].cal = 0;
        bg_w.bgw[i].rewrite_flag = 0;
        bg_w.bgw[i].fam_no = i;
        bg_w.bgw[i].speed_x = 0;
        bg_w.bgw[i].speed_y = 0;
        bg_w.bgw[i].r_no_1 = bg_w.bgw[i].r_no_2 = 0;
    }

    bg_w.scr_stop = 0;
    bg_w.frame_flag = 0;
    bg_w.old_chase_flag = bg_w.chase_flag = 0;
    bg_w.bg_f_x = 64;
    bg_w.bg_f_y = 64;
    bg_w.bg2_sp_x2 = bg_w.bg2_sp_x = 0;
    bg_w.max_x = 8;
    bg_w.quake_x_index = 0;
    bg_w.quake_y_index = 0;

    for (i = 0; i <= 0; i++) {
        bg_w.bgw[i].hos_xy[0].cal = bg_w.bgw[i].wxy[0].cal = bg_w.bgw[i].xy[0].cal = bg_pos_tbl2[type][i][0];
        bg_w.bgw[i].hos_xy[1].cal = bg_w.bgw[i].wxy[1].cal = bg_w.bgw[i].xy[1].cal = bg_pos_tbl2[type][i][1];
        bg_w.bgw[i].pos_y_work = bg_w.bgw[i].xy[1].disp.pos;
        bg_w.bgw[i].old_pos_x = bg_w.bgw[i].pos_x_work = bg_w.bgw[i].xy[0].disp.pos;
        bg_w.bgw[i].speed_x = msp2[type][i][0];
        bg_w.bgw[i].speed_y = msp2[type][i][1];
        bg_w.bgw[i].rewrite_flag = 0;
        bg_w.bgw[i].zuubun = 0;
        bg_w.bgw[i].frame_deff = 64;
        bg_w.bgw[i].max_x_limit = bg_w.bgw[i].speed_x * bg_w.max_x;
    }

    base_y_pos = 40;
}

static void begin_load_save_exit(struct _TASK* task_ptr) {
    if (vm_w.Request != 0) {
        return;
    }
    task_ptr->free[0] = 0;
    task_ptr->free[1]++;
    if (task_ptr->r_no[1] == 5) {
        task_ptr->free[2] = 18;
    } else {
        task_ptr->free[2] = task_ptr->r_no[1];
    }
    Exit_Sub(task_ptr, 2, task_ptr->free[2]);
}

static void set_load_save_exit_order(struct _TASK* task_ptr, s16* order) {
    switch (task_ptr->r_no[1]) {
    case 13:
        *order = 105;
        break;
    case 17:
        task_ptr->r_no[2] = 99;
        /* fallthrough */
    case 6:
        *order = 110;
        break;
    case 19:
    case 20:
        *order = 112;
        break;
    case 23:
        task_ptr->r_no[0] = 0;
        task_ptr->r_no[2] = 99;
        task_ptr->free[0] = 1;
        task_ptr->free[1] = 8;
        *order = 105;
        break;
    }
}

static void finish_load_save_exit(struct _TASK* task_ptr) {
    s16 ix;

    if (!Exit_Sub(task_ptr, 2, task_ptr->free[2])) {
        return;
    }
    task_ptr->free[1]++;
    task_ptr->timer = 1;
    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 1;
    }
    set_load_save_exit_order(task_ptr, &ix);
    Order[ix] = 4;
    Order_Timer[ix] = 1;
}

static void complete_load_save_wait(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    if (--task_ptr->timer == 0) {
        task_ptr->r_no[0] = 0;
    }
}

void Wait_Load_Save(struct _TASK* task_ptr) {
    switch (task_ptr->free[1]) {
    case 0:
        begin_load_save_exit(task_ptr);
        break;
    case 1:
        finish_load_save_exit(task_ptr);
        break;
    case 2:
        complete_load_save_wait(task_ptr);
        break;
    }
}

void Wait_Replay_Check(struct _TASK* task_ptr) {
    switch (task_ptr->free[1]) {
    case 0:
        if (vm_w.Request != 0) {
            break;
        }

        task_ptr->r_no[0] = 0;
        task_ptr->r_no[3] = 0;

        if (vm_w.Number == 0 && vm_w.New_File == 0) {
            task_ptr->r_no[2] = 3;
            break;
        }

        task_ptr->r_no[2] = 5;
        break;
    }
}

void Dummy_Setting(struct _TASK* task_ptr) {
    s16 ix;
    s16 group;
    s16 y;

    s16 s6;
    s16 s5;
    s16 s4;
    s16 s3;

    switch (task_ptr->r_no[2]) {
    case 0:
        task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Cursor_Y[1] = 0;
        Menu_Suicide[0] = 1;
        Training_Index = 2;

        for (ix = 0, s6 = y = 80; ix < 6; ix++, s5 = y += 16) {
            effect_A3_init(0, 1, ix, ix, 1, 48, y, 0);
        }

        for (ix = 0, y = 80, s4 = group = 2; ix < 4; ix++, group++, s3 = y += 16) {
            effect_A3_init(0, group, ix, ix, 1, 0xE6, y, 0);
        }

        break;

    case 1:
        Dummy_Move_Sub(task_ptr, Champion, 0, 0, 5);

        if (Menu_Cursor_Y[0] == 4 && IO_Result & 0x100) {
            Training[2].contents[0][0][0] = 0;
            Training[2].contents[0][0][1] = 0;
            Training[2].contents[0][0][2] = 0;
            Training[2].contents[0][0][3] = 0;
            SE_selected();
        }

        break;

    case 2:
        SE_selected();
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        Training_Disp_Sub(task_ptr);
        break;
    }
}

void Training_Disp_Sub(struct _TASK* task_ptr) {
    if (Mode_Type == MODE_NORMAL_TRAINING) {
        task_ptr->r_no[1] = 1;
        Training_Index = 0;
        return;
    }

    task_ptr->r_no[1] = 2;
    Training_Index = 1;
}

void Dummy_Move_Sub(struct _TASK* task_ptr, s16 PL_id, s16 id, s16 type, s16 max) {
    u16 sw = ~(plsw_01[PL_id]) & plsw_00[PL_id];

    sw = Check_Menu_Lever(PL_id, 0);
    MC_Move_Sub(sw, 0, max, 0xFF);
    Dummy_Move_Sub_LR(sw, id, type, 0);

    if (IO_Result & 0x200) {
        task_ptr->r_no[2]++;
        return;
    }

    if (IO_Result & 0x100 && Menu_Cursor_Y[0] == max) {
        task_ptr->r_no[2]++;
    }
}

const u8 Menu_Max_Data_Tr[2][2][8] = {
    { { 4, 6, 2, 2, 0, 0, 0, 0 }, { 3, 1, 1, 1, 3, 7, 0, 0 } },
    { { 2, 3, 1, 3, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } },
};

static bool is_disallowed_dummy_selection(s16 id, s16 type, s16 cursor_id) {
    return Interface_Type[Champion ^ 1] == 0 && id == 0 && type == 0 && Menu_Cursor_Y[cursor_id] == 0 &&
           Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] == 4;
}

static void adjust_dummy_option(u16 sw, s16 id, s16 type, s16 cursor_id) {
    s16 max = Menu_Max_Data_Tr[id][type][Menu_Cursor_Y[cursor_id]];
    if (sw == SWK_WEST) {
        Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]]--;
    } else {
        Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]]++;
    }
    if (Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] < 0) {
        Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = max;
    }
    if (Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] > max) {
        Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = 0;
    }
    if (is_disallowed_dummy_selection(id, type, cursor_id)) {
        Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = sw == SWK_WEST ? 3 : 0;
    }
    SE_dir_cursor_move();
}

void Dummy_Move_Sub_LR(u16 sw, s16 id, s16 type, s16 cursor_id) {
    s16 max = Menu_Max_Data_Tr[id][type][Menu_Cursor_Y[cursor_id]];

    if (max == 0) {
        return;
    }

    switch (sw) {
    case 4:
    case 8:
        adjust_dummy_option(sw, id, type, cursor_id);
        break;

    default:
        if (is_disallowed_dummy_selection(id, type, cursor_id)) {
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = 0;
        }

        break;
    }
}

static void prepare_recording_training(void) {
    Record_Data_Tr = 1;
    Training[0] = Training[2];
    Training[0].contents[1][0][2] = 1;
    Training[1] = Training[2];
    switch (Training[0].contents[1][0][0]) {
    case 0:
        control_pl_rno = 0;
        break;
    case 1:
        control_pl_rno = 1;
        break;
    case 2:
        control_pl_rno = 2;
        break;
    }
}

static void start_selected_training(struct _TASK* task_ptr) {
    if (Menu_Cursor_Y[0] == 0) {
            Play_Mode = 1;
    } else {
            Play_Mode = 3;
    }
    All_Clear_Timer();
    Check_Replay();
    if (Menu_Cursor_Y[0] == 1) {
        Replay_Status[Training_ID] = 0;
        Replay_Status[Training_ID ^ 1] = 3;
        Training[0] = Training[1];
        Training[0].contents[1][0][2] = Training[2].contents[1][0][2];
        Training[0].contents[1][0][3] = Training[2].contents[1][0][3];
        control_pl_rno = 99;
    }
    task_ptr->r_no[0] = 10;
    task_ptr->r_no[1] = 0;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    Menu_Suicide[0] = 1;
    Game_pause = 0;
    Pause_Down = 0;
    save_w[Present_Mode].Time_Limit = 60;
    count_cont_init(0);
    Training[0].contents[1][1][3] = Menu_Cursor_Y[0];
    init_omop();
    set_init_A4_flag();
    Training_Cursor = Menu_Cursor_Y[0];
}

void handle_blocking_training_selection(struct _TASK* task_ptr) {
    switch (Menu_Cursor_Y[0]) {
    case 0:
        prepare_recording_training();
        /* fallthrough */
    case 1:
        start_selected_training(task_ptr);
        break;

    case 2:
        task_ptr->r_no[1] = 7;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        Training_Cursor = 2;
        break;

    case 3:
        Training_Cursor = 3;
        /* fallthrough */

    case 4:
        task_ptr->r_no[1] = Menu_Cursor_Y[0] + 2;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        break;

    case 5:
        Training_Cursor = 5;
        Training_Exit_Sub(task_ptr);
        break;
    }

    SsBgmHalfVolume(0);
    SE_selected();
}

const LetterData training_letter_data[6] = { { 0x82, "NORMAL TRAINING" },   { 0x73, "PARRYING TRAINING" },
                                             { 0x7C, "DUMMY SETTING" },     { 0x87, "TRAINING OPTION" },
                                             { 0x7D, "RECORDING SETTING" }, { 0x8F, "BUTTON CONFIG." } };

void Training_Init_Sub(struct _TASK* task_ptr) {
    s16 ix;

    task_ptr->r_no[2]++;
    Menu_Common_Init();
    Menu_Cursor_Y[0] = Training_Cursor;

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 0;
    }
}

void Training_Exit_Sub(struct _TASK* task_ptr) {
    task_ptr->r_no[2]++;
    Menu_Suicide[0] = 1;
    Menu_Cursor_Y[0] = 1;
    effect_10_init(0, 0, 3, 6, 1, 17, 12);
    effect_10_init(0, 1, 0, 0, 1, 20, 15);
    effect_10_init(0, 1, 1, 1, 1, 26, 15);
}

static void wait_for_character_change_load(struct _TASK* task_ptr) {
    if ((task_ptr->timer -= 1) != 0) {
        return;
    }
    if (!Check_LDREQ_Break()) {
        task_ptr->r_no[2]++;
        Switch_Screen_Init(0);
        return;
    }
    task_ptr->timer = 1;
}

static void finish_character_change(struct _TASK* task_ptr) {
    s16 ix;

    if (Switch_Screen(0) == 0) {
        return;
    }
    task_ptr->r_no[2]++;
    Cover_Timer = 0x17;
    G_No[1] = 1;
    G_No[2] = 0;
    G_No[3] = 0;
    for (ix = 0; ix < 2; ix++) {
        Sel_PL_Complete[ix] = 0;
        Sel_Arts_Complete[ix] = 0;
        plw[ix].wu.operator = 1;
        Operator_Status[ix] = 1;
    }
    cpExitTask(TASK_MENU);
}

void Character_Change(struct _TASK* task_ptr) {
    if (!(Check_Pad_in_Pause(task_ptr) == 0)) {
        return;
    }

    switch (task_ptr->r_no[2]) {
    case 0:
        task_ptr->r_no[2]++;
        task_ptr->timer = 0xA;
        Game_pause = 0x81;
        break;

    case 1:
        wait_for_character_change_load(task_ptr);
        break;

    case 2:
        finish_character_change(task_ptr);
        break;
    }
}

static void clear_training_data(void) {
    s16 ix;
    s16 ix2;
    s16 ix3;

    for (ix = 0; ix < 2; ix++) {
        for (ix2 = 0; ix2 < 2; ix2++) {
            for (ix3 = 0; ix3 < 6; ix3++) {
                Training[0].contents[ix][ix2][ix3] = 0;
            }
        }
    }
}

void Default_Training_Data(s32 flag) {

    if (flag == 0) {
        if (!mpp_w.initTrainingData) {
            return;
        }

        mpp_w.initTrainingData = false;
    }

    clear_training_data();

    Training[0].contents[0][1][TRAINING_OPTION_DAMAGE] = save_w->Damage_Level;
    Training[0].contents[0][1][TRAINING_OPTION_DIFFICULTY] = save_w->Difficulty;
    save_w[Present_Mode].Damage_Level = save_w->Damage_Level;
    save_w[Present_Mode].Difficulty = save_w->Difficulty;
    Training[2] = Training[0];
    Disp_Attack_Data = 0;
}

void Default_Training_Option() {
    Training->contents[0][1][TRAINING_OPTION_SA_GAUGE] = 0;
    Training->contents[0][1][TRAINING_OPTION_ATTACK_DATA] = 0;
    Training->contents[0][1][TRAINING_OPTION_HITBOXES] = 0;
    Training->contents[0][1][TRAINING_OPTION_DAMAGE] = save_w->Damage_Level;
    Training->contents[0][1][TRAINING_OPTION_DIFFICULTY] = save_w->Difficulty;
    Training->contents[0][1][TRAINING_OPTION_INPUT_HISTORY] = 0;
    save_w[Present_Mode].Damage_Level = save_w->Damage_Level;
    save_w[Present_Mode].Difficulty = save_w->Difficulty;
    Training[2] = Training[0];
    Disp_Attack_Data = 0;
}

void Wait_Replay_Load(struct _TASK* task_ptr) {}

s32 Menu_Sub_case1(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);

    if ((task_ptr->timer -= 1) == 0) {
        task_ptr->r_no[2] += 1;
        FadeInit();
        return 1;
    }

    return 0;
}

void Back_to_Mode_Select(struct _TASK* task_ptr) {
    s16 ix;

    FadeOut(1, 0xFF, 8);
    G_No[0] = 2;
    G_No[1] = 12;
    G_No[2] = 0;
    G_No[3] = 0;
    E_No[0] = 1;
    E_No[1] = 2;
    E_No[2] = 2;
    E_No[3] = 0;
    System_all_clear_Level_B();
    Menu_Init(task_ptr);

    for (ix = 0; ix < 4; ix++) {
        task_ptr->r_no[ix] = 0;
    }

    BGM_Request_Code_Check(0x41);
}


static void handle_end_replay_selection(struct _TASK* task_ptr) {
    s16 ix;

    switch (IO_Result) {
    case 0x100:
        switch (Menu_Cursor_Y[0]) {
        case 0:
            task_ptr->r_no[0] = 0xC;
            task_ptr->r_no[1] = 0;

            for (ix = 0; ix < 4; ix++) {
                Menu_Suicide[ix] = 1;
            }

            SE_selected();
            break;

        case 1:
            task_ptr->r_no[1] += 1;
            SE_selected();
            Menu_Suicide[0] = 1;
            Menu_Cursor_Y[0] = 1;
            effect_10_init(0, 0, 3, 3, 1, 0x13, 0xE);
            effect_10_init(0, 1, 0, 0, 1, 0x14, 0x10);
            effect_10_init(0, 1, 1, 1, 1, 0x1A, 0x10);
            break;
        }

        break;
    }
}

static bool initialize_end_replay_menu(struct _TASK* task_ptr) {
    if (Allow_a_battle_f == 0) {
        return false;
    }
    task_ptr->r_no[1] += 1;
    Pause_ID = Decide_ID;
    Pause_Down = 1;
    Game_pause = 0x81;
    effect_A3_init(1, 0x16, 0x63, 0, 3, 0x82, 0x48, 1);
    effect_A3_init(1, 0x16, 0x63, 1, 3, 0x88, 0x58, 1);
    Order[0x8A] = 3;
    Order_Timer[0x8A] = 1;
    effect_66_init(0x8A, 0xA, 2, 7, -1, -1, -0x3FF6);
    return true;
}

static void setup_end_replay_menu(struct _TASK* task_ptr) {
    s16 ix;

    task_ptr->r_no[1] += 1;
    Menu_Common_Init();
    Menu_Cursor_Y[0] = 0;
    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 0;
    }
    effect_10_init(0, 0, 0, 4, 0, 0x14, 0xE);
    effect_10_init(0, 6, 1, 2, 0, 0x16, 0x10);
}

static void handle_end_replay_confirmation(struct _TASK* task_ptr) {
    switch (Yes_No_Cursor_Move_Sub(task_ptr)) {
    case 1:
        task_ptr->r_no[1] = 1;
        break;
    case -1:
        Menu_Suicide[3] = 1;
        break;
    }
}

void End_Replay_Menu(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[1]) {
    case 0:
        if (!initialize_end_replay_menu(task_ptr)) {
            break;
        }
        /* fallthrough */
    case 1:
        setup_end_replay_menu(task_ptr);
        break;

    case 2:
        MC_Move_Sub(Check_Menu_Lever(Pause_ID, 0), 0, 1, 0xFF);
        handle_end_replay_selection(task_ptr);
        break;

    case 3:
        handle_end_replay_confirmation(task_ptr);
        break;
    }
}
