/**
 * @file menu.c
 * Menus
 */

#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "main.h"
#include "netplay/netplay.h"
#include "port/sdl/sdl_app.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/debug/Debug.h"
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

void Default_Training_Option();
void Dummy_Move_Sub(struct _TASK* task_ptr, s16 PL_id, s16 id, s16 type, s16 max);
void Return_Pause_Sub(struct _TASK* task_ptr);
void Dummy_Move_Sub_LR(u16 sw, s16 id, s16 type, s16 cursor_id);
void Return_VS_Result_Sub(struct _TASK* task_ptr);
void Exit_Replay_Save(struct _TASK* task_ptr);
void Setup_NTr_Data(s16 ix);
static void apply_training_hitbox_display(bool force_off);
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
void Disp_Auto_Save(struct _TASK* task_ptr);
void Suspend_Menu();
void Wait_Replay_Load();
void Training_Menu(struct _TASK* task_ptr);
void After_Replay(struct _TASK* task_ptr);
void Disp_Auto_Save2(struct _TASK* task_ptr);
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
void Memory_Card(struct _TASK* task_ptr);
void Extra_Option(struct _TASK* task_ptr);
void VS_Result(struct _TASK* task_ptr);
void Save_Replay(struct _TASK* task_ptr);
void Direction_Menu(struct _TASK* task_ptr);
void Save_Direction(struct _TASK* task_ptr);
void Load_Direction(struct _TASK* task_ptr);
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
void Dir_Move_Sub(struct _TASK* task_ptr, s16 PL_id);
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
void Setup_Sound_Mode(u8 last_mode);
u16 Sound_Cursor_Sub(s16 PL_id);
u16 SD_Move_Sub_LR(u16 sw);
void Memory_Card_Sub(s16 PL_id);
void Save_Load_Menu(struct _TASK* task_ptr);
void Go_Back_MC(struct _TASK* task_ptr);
u16 Memory_Card_Move_Sub_LR(u16 sw, s16 cursor_id);
u16 After_VS_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max);
s32 VS_Result_Move_Sub(struct _TASK* task_ptr, s16 PL_id);
void DAS_1st(struct _TASK* task_ptr);
void DAS_2nd(struct _TASK* task_ptr);
void DAS_3rd(struct _TASK* task_ptr);
void DAS_4th(struct _TASK* task_ptr);
void DAS2_4th(struct _TASK* task_ptr);
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
s32 Pause_1st_Sub(struct _TASK* task_ptr);
s32 Yes_No_Cursor_Move_Sub(struct _TASK* task_ptr);
void Setup_Button_Sub(s16 x, s16 y, s16 master_player);
void Button_Exit_Check_in_Game(struct _TASK* task_ptr, s16 PL_id);
void Setup_Save_Replay_1st(struct _TASK* task_ptr);
s32 Save_Replay_MC_Sub(struct _TASK* task_ptr, s16 /* unused */);
void Button_Exit_Check_in_Tr(struct _TASK* task_ptr, s16 PL_id);
s32 VS_Result_Select_Sub(struct _TASK* task_ptr, s16 PL_id);

typedef void (*MenuFunc)(struct _TASK*);

typedef struct {
    s16 pos_x;
    s8* menu;
} LetterData;

const MenuFunc Menu_Jmp_Tbl[14] = {
    After_Title,   In_Game,      Wait_Load_Save,  Wait_Replay_Check, Disp_Auto_Save, Suspend_Menu, Wait_Replay_Load,
    Training_Menu, After_Replay, Disp_Auto_Save2, Wait_Pause_in_Tr,  Reset_Training, Reset_Replay, End_Replay_Menu,
};

// sbss
u8 r_no_plus;
u8 control_player;
u8 control_pl_rno;

// rodata
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
    void (*AT_Jmp_Tbl[21])() = { Menu_Init,        Mode_Select,   Option_Select, Option_Select,  Training_Mode,
                                 System_Direction, Load_Replay,   Option_Select, toSelectGame,   Game_Option,
                                 Button_Config,    Screen_Adjust, Sound_Test,    Memory_Card,    Extra_Option,
                                 Option_Select,    VS_Result,     Save_Replay,   Direction_Menu, Save_Direction,
                                 Load_Direction };

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
        effect_57_init(0x4E, 0, 0, 0x45, fade_on);
        load_any_texture_patnum(0x7F30, 0xC, 0);
    }

    cpReadyTask(TASK_SAVER, Saver_Task);
}

#if defined(NETPLAY_ENABLED)
// Returns true while matchmaking is pending, consuming input to cancel.
// Caller should skip normal menu logic when this returns true.
static bool check_netplay_cancelled() {
    if (!Netplay_IsMatchmakingPending()) {
        return false;
    }

    // I dont know if we want users to be able to cancel mm on their own?
    // s16 sw = (~plsw_01[0] & plsw_00[0]) | (~plsw_01[1] & plsw_00[1]);

    // if (sw & (SWK_SOUTH | SWK_EAST)) {
    //     Netplay_CancelMatchmaking();
    //     SE_selected();
    // }

    return true;
}
#endif

void Mode_Select(struct _TASK* task_ptr) {
    s16 ix;
    s16 PL_id;
    s16 loop_counter = 7;

    switch (task_ptr->r_no[2]) {
    case 0:
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

        effect_57_init(0x64, 0, 0, 0x3F, 2);
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
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            Order[0x4E] = 2;
            Order_Dir[0x4E] = 0;
            Order_Timer[0x4E] = 1;
            checkAdxFileLoaded();
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
#if defined(NETPLAY_ENABLED)
        if (check_netplay_cancelled()) {
            break;
        }
#endif
        if (Connect_Status == 0 && Menu_Cursor_Y[0] == 1) {
            Menu_Cursor_Y[0] = 2;
        } else {
            PL_id = 0;

            if (MC_Move_Sub(Check_Menu_Lever(0, 0), 0, loop_counter - 1, 1) == 0) {
                PL_id = 1;
                MC_Move_Sub(Check_Menu_Lever(1, 0), 0, loop_counter - 1, 1);
            }
        }

        switch (IO_Result) {
        case 0x100:
            switch (Menu_Cursor_Y[0]) {
            case 0:
                G_No[2] += 1;
                Mode_Type = MODE_ARCADE;
                task_ptr->r_no[0] = 5;
                cpExitTask(TASK_SAVER);
                Decide_PL(PL_id);
                break;

            case 1:
                Setup_VS_Mode(task_ptr);
                G_No[1] = 12;
                G_No[2] = 1;
                Mode_Type = MODE_VERSUS;
                cpExitTask(TASK_MENU);
                break;

            case 4:
#if defined(NETPLAY_ENABLED)
                Netplay_BeginMatchmaking();
                Netplay_BeginDirectP2P();
                break;
#endif
                /* fallthrough */

            case 2:
            case 3:
            case 5:
            case 6:
                task_ptr->r_no[2] += 1;
                task_ptr->free[0] = 0;
                task_ptr->free[1] = Menu_Cursor_Y[0] + 2;
                break;

            default:
                break;
            }

            SE_selected();
            break;
        }

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

void toSelectGame(struct _TASK* task_ptr) {
    u16 sw;

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
        imgSelectGameButton();
        sw = (~plsw_01[0] & plsw_00[0]) | (~plsw_01[1] & plsw_00[1]); // potential macro
        sw &= (SWK_SOUTH | SWK_EAST);

        if (sw != 0) {
            if (sw != (SWK_SOUTH | SWK_EAST)) {
                if (sw & SWK_SOUTH) {
                    task_ptr->free[0] = 1;
                }

                SE_selected();
                FadeInit();
                task_ptr->r_no[2] = 8;
                break;
            }
        }

        break;

    case 8:
        imgSelectGameButton();

        if (FadeOut(1, 0x19, 8) != 0) {
            if (task_ptr->free[0]) {
                task_ptr->r_no[2] = 0xA;
                sound_all_off();
                break;
            }

            task_ptr->r_no[2] = 9;
            break;
        }

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
        break;

    default:
        SDLApp_Exit();
        break;
    }
}

void imgSelectGameButton() {
    dispButtonImage2(0x74, 0x6B, 0x18, 0x20, 0x1A, 0, 4);
    dispButtonImage2(0xB2, 0x6B, 0x18, 0x20, 0x1A, 0, 5);
}

void Training_Mode(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_index;
    s16 PL_id;

    switch (task_ptr->r_no[2]) {
    case 0:
        Menu_in_Sub(task_ptr);
        mpp_w.initTrainingData = true;
        effect_57_init(0x6F, 0xB, 0, 0x3F, 2);
        Order[0x6F] = 1;
        Order_Dir[0x6F] = 8;
        Order_Timer[0x6F] = 1;
        effect_04_init(1, 5, 0, 0x48);

        ix = 0;
        char_index = 0x35;

        while (ix < 3) {
            effect_61_init(0, ix + 0x50, 0, 1, char_index, ix, 0x7047);
            Order[ix + 0x50] = 1;
            Order_Dir[ix + 0x50] = 4;
            Order_Timer[ix + 0x50] = ix + 0x14;
            ix++;
            char_index++;
        }

        Menu_Cursor_Move = 3;
        system_dir[4] = system_dir[1];
        system_dir[5] = system_dir[1];
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
        PL_id = 0;

        if (MC_Move_Sub(Check_Menu_Lever(0, 0), 0, 2, 0xFF) == 0) {
            PL_id = 1;
            MC_Move_Sub(Check_Menu_Lever(1, 0), 0, 2, 0xFF);
        }

        switch (IO_Result) {
        case 0x100:
        case 0x200:
            break;

        default:
            return;
        }

        SE_selected();

        if (Menu_Cursor_Y[0] == 2 || IO_Result == 0x200) {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            task_ptr->r_no[1] = 1;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            Order[0x6F] = 4;
            Order_Timer[0x6F] = 4;
            break;
        }

        Decide_ID = PL_id;

        if (Menu_Cursor_Y[0] == 0) {
            Mode_Type = MODE_NORMAL_TRAINING;
            Present_Mode = 4;
        } else {
            Mode_Type = MODE_PARRY_TRAINING;
            Present_Mode = 5;
        }

        Setup_VS_Mode(task_ptr);
        G_No[2] += 1;
        task_ptr->r_no[0] = 5;
        cpExitTask(TASK_SAVER);
        Champion = PL_id;
        Pause_ID = PL_id;
        Training_ID = PL_id;
        New_Challenger = PL_id ^ 1;
        cpExitTask(TASK_ENTRY);

        break;
    }
}

void Option_Select(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_index;

    switch (task_ptr->r_no[2]) {
    case 0:
        Menu_in_Sub(task_ptr);
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 0;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x4F, 1, 0, 0x3F, 2);
        Order[0x4F] = 1;
        Order_Dir[0x4F] = 8;
        Order_Timer[0x4F] = 1;

        if (save_w[Present_Mode].Extra_Option == 0) {
            effect_04_init(1, 4, 0, 0x48);

            ix = 0;
            char_index = 0x2F;

            while (ix < 6) {
                effect_61_init(0, ix + 0x50, 0, 1, char_index, ix, 0x7047);
                Order[ix + 0x50] = 1;
                Order_Dir[ix + 0x50] = 4;
                Order_Timer[ix + 0x50] = ix + 0x14;
                ix++;
                char_index++;
            }

            Menu_Cursor_Move = 6;
            break;
        }

        effect_04_init(1, 1, 0, 0x48);

        ix = 0;
        char_index = 7;

        while (ix < 7) {
            effect_61_init(0, ix + 0x50, 0, 1, char_index, ix, 0x7047);
            Order[ix + 0x50] = 1;
            Order_Dir[ix + 0x50] = 4;
            Order_Timer[ix + 0x50] = ix + 0x14;
            ix++;
            char_index++;
        }

        Menu_Cursor_Move = 7;
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
        if (save_w[Present_Mode].Extra_Option) {
            ix = 1;
        } else {
            ix = 0;
        }

        if (MC_Move_Sub(Check_Menu_Lever(0, 0), 0, ix + 5, 0xFF) == 0) {
            MC_Move_Sub(Check_Menu_Lever(1, 0), 0, ix + 5, 0xFF);
        }

        switch (IO_Result) {
        case 0x100:
        case 0x200:
            break;

        default:
            return;
        }

        SE_selected();

        if (Menu_Cursor_Y[0] == ix + 5 || IO_Result == 0x200) {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            task_ptr->r_no[1] = 1;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            Order[0x4F] = 4;
            Order_Timer[0x4F] = 4;

            if (Check_Change_Contents()) {
                if (save_w[Present_Mode].Auto_Save) {
                    task_ptr->r_no[0] = 4;
                    task_ptr->r_no[1] = 0;
                    Forbid_Reset = 1;
                    Copy_Check_w();
                    break;
                }
            }

            break;
        }

        task_ptr->r_no[2] += 1;
        task_ptr->free[0] = 0;
        X_Adjust_Buff[0] = X_Adjust;
        X_Adjust_Buff[1] = X_Adjust;
        X_Adjust_Buff[2] = X_Adjust;
        Y_Adjust_Buff[0] = Y_Adjust;
        Y_Adjust_Buff[1] = Y_Adjust;
        Y_Adjust_Buff[2] = Y_Adjust;
        break;

    default:
        Exit_Sub(task_ptr, 1, Menu_Cursor_Y[0] + 9);
        break;
    }
}

void System_Direction(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_index;

    switch (task_ptr->r_no[2]) {
    case 0:
        Menu_in_Sub(task_ptr);
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 3;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x6D, 0xA, 0, 0x3F, 2);
        Order[0x6D] = 1;
        Order_Dir[0x6D] = 8;
        Order_Timer[0x6D] = 1;
        effect_04_init(1, 3, 0, 0x48);
        Convert_Buff[3][0][0] = Direction_Working[1];
        effect_64_init(0x61U, 0, 1, 0xA, 0, 0x7047, 0xB, 3, 0);
        Order[0x61] = 1;
        Order_Dir[0x61] = 4;
        Order_Timer[0x61] = 0x14;

        ix = 0;
        char_index = 0x2B;

        while (ix < 4) {
            effect_61_init(0, ix + 0x50, 0, 1, char_index, ix + 1, 0x7047);
            Order[ix + 0x50] = 1;
            Order_Dir[ix + 0x50] = 4;
            Order_Timer[ix + 0x50] = ix + 0x15;
            ix++;
            char_index++;
        }

        Menu_Cursor_Move = 4;
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
        System_Dir_Move_Sub(0);

        if (IO_Result == 0) {
            System_Dir_Move_Sub(1);
        }

        switch (IO_Result) {
        case 0x100:
            if (Menu_Cursor_Y[0] == 0) {
                break;
            }

            // fallthrough

        case 0x200:
            SE_selected();
            Order[0x6D] = 4;
            Order_Timer[0x6D] = 4;

            if (Menu_Cursor_Y[0] == 4 || IO_Result == 0x200) {
                Menu_Suicide[0] = 0;
                Menu_Suicide[1] = 1;
                task_ptr->r_no[1] = 1;
                task_ptr->r_no[2] = 0;
                task_ptr->r_no[3] = 0;
                task_ptr->free[0] = 0;
                break;
            }

            task_ptr->r_no[2] += 1;
            task_ptr->free[0] = 0;

            break;
        }

        break;

    default:
        Exit_Sub(task_ptr, 1, Menu_Cursor_Y[0] + 0x11);
        break;
    }
}

void System_Dir_Move_Sub(s16 PL_id) {
    u16 sw = ~plsw_01[PL_id] & plsw_00[PL_id]; // potential macro
    sw = Check_Menu_Lever(PL_id, 0);
    MC_Move_Sub(sw, 0, 4, 0xFF);
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

void Direction_Menu(struct _TASK* task_ptr) {
    Menu_Cursor_Y[1] = Menu_Cursor_Y[0];

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        task_ptr->timer = 5;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Menu_Page = 0;
        Menu_Page_Buff = Menu_Page;
        Message_Data->kind_req = 3;
        break;

    case 1:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        Setup_Next_Page(task_ptr, 0);
        /* fallthrough */

    case 2:
        FadeOut(1, 0xFF, 8);

        if (--task_ptr->timer == 0) {
            task_ptr->r_no[2] += 1;
            FadeInit();
        }

        break;

    case 3:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
        }

        break;

    case 4:
        Pause_ID = 0;

        Dir_Move_Sub(task_ptr, 0);

        if (IO_Result == 0) {
            Pause_ID = 1;
            Dir_Move_Sub(task_ptr, 1);
        }

        if (Menu_Cursor_Y[1] != Menu_Cursor_Y[0]) {
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

        switch (IO_Result) {
        case 0x200:
            task_ptr->r_no[2] += 1;
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 0;
            Menu_Suicide[2] = 1;
            SE_dir_selected();
            break;

        case 0x80:
        case 0x800:
            task_ptr->r_no[2] = 1;
            task_ptr->timer = 5;

            if (--Menu_Page < 0) {
                Menu_Page = (s8)Page_Max;
            }

            SE_dir_selected();
            break;

        case 0x40:
        case 0x400:
            task_ptr->r_no[2] = 1;
            task_ptr->timer = 5;

            if (++Menu_Page > Page_Max) {
                Menu_Page = 0;
            }

            SE_dir_selected();
            break;

        case 0x100:
            if (Menu_Cursor_Y[0] == Menu_Max) {
                switch (system_dir[1].contents[Menu_Page][Menu_Max]) {
                case 0:
                    task_ptr->r_no[2] = 1;
                    task_ptr->timer = 5;

                    if (--Menu_Page < 0) {
                        Menu_Page = (s8)Page_Max;
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
                    task_ptr->r_no[2] += 1;
                    Menu_Suicide[0] = 0;
                    Menu_Suicide[1] = 0;
                    Menu_Suicide[2] = 1;
                    break;
                }

                SE_selected();
                break;
            }

            break;
        }

        break;

    default:
        Exit_Sub(task_ptr, 2, 5);
        break;
    }
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

u16 Dir_Move_Sub2(u16 sw) {
    if (Menu_Cursor_Move > 0) {
        return 0;
    }

    switch (sw) {
    case 0x1:
        Menu_Cursor_Y[0] -= 1;

        if (Menu_Cursor_Y[0] < 0) {
            Menu_Cursor_Y[0] = Menu_Max;
        }

        SE_cursor_move();
        return IO_Result = 1;

    case 0x2:
        Menu_Cursor_Y[0] += 1;

        if (Menu_Cursor_Y[0] > Menu_Max) {
            Menu_Cursor_Y[0] = 0;
        }

        SE_cursor_move();
        return IO_Result = 2;

    case 0x10:
        return IO_Result = 0x10;

    case 0x20:
        return IO_Result = 0x20;

    case 0x40:
        return IO_Result = 0x40;

    case 0x80:
        return IO_Result = 0x80;

    case 0x100:
        return IO_Result = 0x100;

    case 0x200:
        return IO_Result = 0x200;

    case 0x400:
        return IO_Result = 0x400;

    case 0x800:
        return IO_Result = 0x800;

    case 0x4000:
        return IO_Result = 0x4000;

    default:
        return IO_Result = 0;
    }
}

void Dir_Move_Sub_LR(u16 sw, s16 /* unused */) {
    u8 last_pos = system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]];

    switch (sw) {
    case 0x4:
        SE_dir_cursor_move();
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] -= 1;

        if (Menu_Cursor_Y[0] == Menu_Max) {
            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] < 0) {
                system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 0;
                IO_Result = 0x80;
                return;
            }

            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] != last_pos) {
                Message_Data->order = 1;
                Message_Data->request = system_dir[1].contents[Menu_Page][Menu_Max] + 0x74;
                Message_Data->timer = 2;
            }
        } else {
            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] < 0) {
                system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = Dir_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]];
            }
        }

        return;

    case 0x8:
        SE_dir_cursor_move();
        system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] += 1;

        if (Menu_Cursor_Y[0] == Menu_Max) {
            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > 2) {
                system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 2;
                IO_Result = 0x400;
                return;
            }

            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > 2) {
                system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 2;
            }

            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] != last_pos) {
                Message_Data->order = 1;
                Message_Data->request = system_dir[1].contents[Menu_Page][Menu_Max] + 0x74;
                Message_Data->timer = 2;
            }
        } else {
            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > Dir_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
                system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 0;
            }
        }

        return;

    case 0x100:
        SE_dir_cursor_move();

        if (Menu_Cursor_Y[0] == Menu_Max) {
            return;
        } else {
            system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] += 1;

            if (system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] > Dir_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
                system_dir[1].contents[Menu_Page][Menu_Cursor_Y[0]] = 0;
            }
        }

        return;
    }
}

void Setup_Next_Page(struct _TASK* task_ptr, u8 /* unused */) {
    s16 ix;
    s16 disp_index;
    s16 mode_type;

    s16 unused_s3;

    Menu_Page_Buff = Menu_Page;
    effect_work_init();
    Menu_Common_Init();
    Menu_Cursor_Y[0] = 0;
    Order[0x4E] = 5;
    Order_Timer[0x4E] = 1;

    if (task_ptr->r_no[1] == 0xE) {
        mode_type = 1;
        Menu_Max = Ex_Page_Data[Menu_Page];
        save_w[1].extra_option.contents[Menu_Page][Menu_Max] = 1;
        Order_Dir[0x4E] = 1;
        effect_57_init(0x4E, 1, 0, 0x45, 0);
        Order[0x73] = 3;
        Order_Dir[0x73] = 8;
        Order_Timer[0x73] = 1;
        effect_57_init(0x73, 6, 0, 0x3F, 2);
        effect_66_init(0x5C, 0x27, 2, 0, 0x47, 0xB, 0);
        Order[0x5C] = 3;
        Order_Timer[0x5C] = 1;
        effect_66_init(0x5D, 0x28, 2, 0, 0x40, (s16)Menu_Page + 1, 0);
        Order[0x5D] = 3;
        Order_Timer[0x5D] = 1;

        if ((msgExtraTbl[0]->msgNum[Menu_Cursor_Y[0] + Menu_Page * 8]) == 1) {
            Message_Data->pos_y = 0x36;
        } else {
            Message_Data->pos_y = 0x3E;
        }

        Message_Data->request = Ex_Account_Data[Menu_Page] + Menu_Cursor_Y[0];
    } else {
        mode_type = 0;
        Menu_Max = Page_Data[Menu_Page];
        system_dir[1].contents[Menu_Page][Menu_Max] = 1;
        effect_66_init(0x5B, 0x14, 2, 0, 0x47, 0xA, 0);
        Order[0x5B] = 3;
        Order_Timer[0x5B] = 1;
        Order[0x4E] = 5;
        Order_Dir[0x4E] = 3;
        effect_57_init(0x4E, 0, 0, 0x45, 0);
        effect_66_init(0x5C, 0x15, 2, 0, 0x47, 0xB, 0);
        Order[0x5C] = 3;
        Order_Timer[0x5C] = 1;
        effect_66_init(0x5D, 0x16, 2, 0, 0x40, (s16)Menu_Page + 1, 0);
        Order[0x5D] = 3;
        Order_Timer[0x5D] = 1;

        if ((msgSysDirTbl[0]->msgNum[Menu_Page * 0xC + Menu_Cursor_Y[0] * 2 + 1]) == 1) {
            Message_Data->pos_y = 0x36;
        } else {
            Message_Data->pos_y = 0x3E;
        }

        disp_index = Menu_Page * 0xC;
        Message_Data->request = disp_index + 1;
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

    for (ix = 0; ix < Menu_Max; ix++, unused_s3 = disp_index += 2) {
        if (mode_type == 0) {
            effect_18_init(disp_index, ix, 0, 2);
            effect_51_init(ix, ix, 2);
        } else {
            effect_C4_init(0, ix, ix, 2);

            if (Menu_Page != 0 || ix != (Menu_Max - 1)) {
                effect_C4_init(1, ix, ix, 2);
            }
        }
    }

    effect_40_init(mode_type, 0, 0x48, 0, 2, 1);
    effect_40_init(mode_type, 1, 0x49, 0, 2, 1);
    effect_40_init(mode_type, 2, 0x4A, 0, 2, 0);
    effect_40_init(mode_type, 3, 0x4B, 0, 2, 2);
}

void Save_Direction(struct _TASK* task_ptr) {
    Menu_Cursor_X[1] = Menu_Cursor_X[0];
    Clear_Flash_Sub();

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        task_ptr->timer = 5;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Menu_Cursor_X[0] = 0;
        Setup_BG(1, 0x200, 0);
        Setup_Replay_Sub(1, 0x70, 0xA, 2);
        Setup_File_Property(2, 0);
        Clear_Flash_Init(4);
        Message_Data->kind_req = 5;
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            SaveInit(1, 1);
        }

        break;

    case 2:
        Setup_Save_Replay_2nd(task_ptr, 2);
        break;

    case 3:
        if (SaveMove() <= 0) {
            IO_Result = 0x200;
            Load_Replay_MC_Sub(task_ptr, 0);
        }

        break;
    }
}

void Load_Direction(struct _TASK* task_ptr) {
    Menu_Cursor_X[1] = Menu_Cursor_X[0];
    Clear_Flash_Sub();

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        task_ptr->timer = 5;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Menu_Cursor_X[0] = 0;
        Setup_BG(1, 0x200, 0);
        Setup_Replay_Sub(1, 0x70, 0xA, 2);
        Setup_File_Property(2, 0);
        Clear_Flash_Init(4);
        Message_Data->kind_req = 5;
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            SaveInit(1, 0);
        }

        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
            task_ptr->free[3] = 0;
            Menu_Cursor_X[0] = Setup_Final_Cursor_Pos(0, 8);
        }

        break;

    case 3:
        if (SaveMove() <= 0) {
            IO_Result = 0x200;
            Load_Replay_MC_Sub(task_ptr, 0);
        }

        break;
    }
}

void Load_Replay(struct _TASK* task_ptr) {
    Menu_Cursor_X[1] = Menu_Cursor_X[0];
    Clear_Flash_Sub();

    switch (task_ptr->r_no[2]) {
    case 0:
        Menu_in_Sub(task_ptr);
        Menu_Cursor_X[0] = 0;
        Setup_BG(1, 0x200, 0);
        Setup_Replay_Sub(1, 0x6E, 9, 1);
        Clear_Flash_Init(4);
        Message_Data->kind_req = 5;
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            SaveInit(2, 0);
        }

        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
            task_ptr->free[3] = 0;
            Menu_Cursor_X[0] = Setup_Final_Cursor_Pos(0, 8);
        }

        break;

    case 3:
        switch (SaveMove()) {
        case 0:
            Decide_ID = 0;

            if (Interface_Type[0] == 0) {
                Decide_ID = 1;
            }

            task_ptr->r_no[2] += 1;
            task_ptr->r_no[3] = 0;
            break;

        case -1:
            IO_Result = 0x200;
            Load_Replay_MC_Sub(task_ptr, 0);
            break;
        }

        break;

    case 4:
        Load_Replay_Sub(task_ptr);
        break;
    }
}

void Load_Replay_Sub(struct _TASK* task_ptr) {
    s32 ix;

    switch (task_ptr->r_no[3]) {
    case 0:
        task_ptr->r_no[3] += 1;
        Rep_Game_Infor[0xA] = Replay_w.game_infor;
        cpExitTask(TASK_ENTRY);
        Play_Mode = 3;
        break;

    case 1:
        task_ptr->r_no[3] += 1;
        FadeInit();
        FadeOut(0, 0xFF, 8);
        Play_Type = 1;
        Mode_Type = MODE_REPLAY;
        Present_Mode = 3;
        Bonus_Game_Flag = 0;

        for (ix = 0; ix < 2; ix++) {
            plw[ix].wu.operator = Replay_w.game_infor.player_infor[ix].player_type;
            Operator_Status[ix] = Replay_w.game_infor.player_infor[ix].player_type;
            My_char[ix] = Replay_w.game_infor.player_infor[ix].my_char;
            Super_Arts[ix] = Replay_w.game_infor.player_infor[ix].sa;
            Player_Color[ix] = Replay_w.game_infor.player_infor[ix].color;
            Vital_Handicap[3][ix] = Replay_w.game_infor.Vital_Handicap[ix];
        }

        Direction_Working[3] = Replay_w.game_infor.Direction_Working;
        bg_w.stage = Replay_w.game_infor.stage;
        bg_w.area = 0;
        save_w[3].Time_Limit = Replay_w.mini_save_w.Time_Limit;
        save_w[3].Battle_Number[0] = Replay_w.mini_save_w.Battle_Number[0];
        save_w[3].Battle_Number[1] = Replay_w.mini_save_w.Battle_Number[1];
        save_w[3].Damage_Level = Replay_w.mini_save_w.Damage_Level;
        save_w[3].extra_option = Replay_w.mini_save_w.extra_option;
        system_dir[3] = Replay_w.system_dir;
        save_w[3].extra_option = Replay_w.mini_save_w.extra_option;
        save_w[3].Pad_Infor[0] = Replay_w.mini_save_w.Pad_Infor[0];
        save_w[3].Pad_Infor[1] = Replay_w.mini_save_w.Pad_Infor[1];
        save_w[3].Pad_Infor[0].Vibration = 0;
        save_w[3].Pad_Infor[1].Vibration = 0;
        cpExitTask(TASK_SAVER);
        break;

    case 2:
        FadeOut(0, 0xFF, 8);
        task_ptr->r_no[3] += 1;
        task_ptr->timer = 0xA;
        System_all_clear_Level_B();
        pulpul_stop();
        init_pulpul_work();
        bg_etc_write(2);
        bg_w.bgw[0].wxy[0].disp.pos += 0x200;
        Setup_BG(0, bg_w.bgw[0].wxy[0].disp.pos, bg_w.bgw[0].wxy[1].disp.pos);
        effect_38_init(0, 0xB, My_char[0], 1, 0);
        Order[0xB] = 3;
        Order_Timer[0xB] = 1;
        effect_38_init(1, 0xC, My_char[1], 1, 0);
        Order[0xC] = 3;
        Order_Timer[0xC] = 1;
        effect_K6_init(0, 0x23, 0x23, 0);
        Order[0x23] = 3;
        Order_Timer[0x23] = 1;
        effect_K6_init(1, 0x24, 0x23, 0);
        Order[0x24] = 3;
        Order_Timer[0x24] = 1;
        effect_39_init(0, 0x11, My_char[0], 0, 0);
        Order[0x11] = 3;
        Order_Timer[0x11] = 1;
        effect_39_init(1, 0x12, My_char[1], 0, 0);
        Order[0x12] = 3;
        Order_Timer[0x12] = 1;
        effect_K6_init(0, 0x1D, 0x1D, 0);
        Order[0x1D] = 3;
        Order_Timer[0x1D] = 1;
        effect_K6_init(1, 0x1E, 0x1D, 0);
        Order[0x1E] = 3;
        Order_Timer[0x1E] = 1;
        effect_43_init(2, 0);
        effect_75_init(0x2A, 3, 0);
        Order[0x2A] = 3;
        Order_Timer[0x2A] = 1;
        Order_Dir[0x2A] = 5;
        break;

    case 3:
        FadeOut(0, 0xFF, 8);

        if (--task_ptr->timer <= 0) {
            task_ptr->r_no[3] += 1;
            bgPalCodeOffset[0] = 0x90;
            BGM_Request(51);
            Purge_memory_of_kind_of_key(0xC);
            Push_LDREQ_Queue_Player(0, My_char[0]);
            Push_LDREQ_Queue_Player(1, My_char[1]);
            Push_LDREQ_Queue_BG((u16)bg_w.stage);
        }

        break;

    case 4:
        if (FadeIn(0, 4, 8) != 0) {
            task_ptr->r_no[3] += 1;
        }

        break;

    case 5:
        if ((Check_PL_Load() != 0) && (Check_LDREQ_Queue_BG((u16)bg_w.stage) != 0) && (adx_now_playend() != 0) &&
            (sndCheckVTransStatus(0) != 0)) {
            task_ptr->r_no[3] += 1;
            Switch_Screen_Init(0);
            init_omop();
        }

        break;

    case 6:
        if (Switch_Screen(0) != 0) {
            Game01_Sub();
            Cover_Timer = 5;
            appear_type = APPEAR_TYPE_ANIMATED;
            set_hitmark_color();
            Purge_texcash_of_list(3);
            Make_texcash_of_list(3);
            G_No[1] = 2;
            G_No[2] = 0;
            G_No[3] = 0;
            E_No[0] = 4;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;

            if (plw->wu.operator != 0) {
                Sel_Arts_Complete[0] = -1;
            }

            if (plw[1].wu.operator != 0) {
                Sel_Arts_Complete[1] = -1;
            }

            task_ptr->r_no[2] = 0;
            cpExitTask(TASK_MENU);
        }

        break;

    default:
        break;
    }
}

s32 Load_Replay_MC_Sub(struct _TASK* task_ptr, s16 PL_id) {
    u16 sw = IO_Result;

    switch (sw) {
    case 0x100:
        if ((Menu_Cursor_X[0] == -1) || (vm_w.Connect[Menu_Cursor_X[0]] == 0)) {
            break;
        }

        Pause_ID = PL_id;
        vm_w.Drive = (u8)Menu_Cursor_X[0];

        if (VM_Access_Request(6, Menu_Cursor_X[0]) == 0) {
            break;
        }

        SE_selected();
        task_ptr->free[1] = 0;
        task_ptr->free[2] = 0;
        task_ptr->r_no[0] = 3;
        return 1;

    case 0x200:
        if (task_ptr->r_no[1] == 6) {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            task_ptr->r_no[1] = 1;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            Order[0x6E] = 4;
            Order_Timer[0x6E] = 4;
        } else {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 0;
            Menu_Suicide[2] = 1;
            task_ptr->r_no[1] = 5;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            Order[0x70] = 4;
            Order_Timer[0x70] = 4;
        }

        break;
    }

    return 0;
}

const u8 Setup_Index_64[10] = { 1, 2, 3, 3, 4, 5, 6, 7, 8, 8 };

void Game_Option(struct _TASK* task_ptr) {
    s16 char_index;
    s16 ix;

    s16 unused_s3;
    s16 unused_s2;

    switch (task_ptr->r_no[2]) {
    case 0:
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
        effect_57_init(0x6A, 7, 0, 0x3F, 2);
        Order[0x6A] = 1;
        Order_Dir[0x6A] = 8;
        Order_Timer[0x6A] = 1;

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

u16 Game_Option_Sub(s16 PL_id) {
    u16 sw;
    u16 ret;

    sw = ~plsw_01[PL_id] & plsw_00[PL_id];
    sw = Check_Menu_Lever(PL_id, 0);
    ret = MC_Move_Sub(sw, 0, 0xB, 0xFF);
    ret |= GO_Move_Sub_LR(sw, 0);
    ret &= 0x20F;
    return ret;
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

void Button_Config(struct _TASK* task_ptr) {
    s16 ix;
    s16 disp_index;

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
        effect_57_init(0x6B, 2, 0, 0x3F, 2);
        Order[0x6B] = 1;
        Order_Dir[0x6B] = 8;
        Order_Timer[0x6B] = 1;

        for (ix = 0; ix < 12; ix++) {
            effect_23_init(0, ix + 0x50, 0, 2, 2, ix, 0x70A7, ix + 9, 1);
            Order[ix + 0x50] = 1;
            Order_Dir[ix + 0x50] = 4;
            Order_Timer[ix + 0x50] = ix + 0x14;
            effect_23_init(1, ix + 0x5C, 0, 2, 3, ix, 0x70A7, ix + 9, 1);
            Order[ix + 0x5C] = 1;
            Order_Dir[ix + 0x5C] = 4;
            Order_Timer[ix + 0x5C] = ix + 0x14;
        }

        for (ix = 0; ix < 9; ix++) {
            if (ix == 8) {
                disp_index = 1;
            } else {
                disp_index = 0;
            }

            effect_23_init(0, ix + 0x78, 0, 2, disp_index, ix, 0x70A7, ix, 0);
            Order[ix + 0x78] = 1;
            Order_Dir[ix + 0x78] = 4;
            Order_Timer[ix + 0x78] = ix + 0x14;
            effect_23_init(1, ix + 0x81, 0, 2, disp_index, ix, 0x70A7, ix, 0);
            Order[ix + 0x81] = 1;
            Order_Dir[ix + 0x81] = 4;
            Order_Timer[ix + 0x81] = ix + 0x14;
        }

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

void Button_Move_Sub_LR(u16 sw, s16 cursor_id) {
    s16 max;

    switch (Menu_Cursor_Y[cursor_id]) {
    case 8:
        max = 1;
        break;

    case 9:
    case 10:
        max = 0;
        break;

    default:
        max = 11;
        break;
    }

    if (max == 0) {
        return;
    }

    switch (sw) {
    case 4:
        Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] -= 1;

        if (Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] < 0) {
            Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] = max;
        }

        if (Menu_Cursor_Y[cursor_id] == 8) {
            if (Convert_Buff[1][cursor_id][8]) {
                pp_vib_on(cursor_id);
            } else {
                pulpul_stop2(cursor_id);
            }
        }

        SE_dir_cursor_move();
        break;

    case 8:
        Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] += 1;

        if (Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] > max) {
            Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] = 0;
        }

        if ((Menu_Cursor_Y[cursor_id] == 8) && (Convert_Buff[1][cursor_id][Menu_Cursor_Y[cursor_id]] == 1)) {
            pp_vib_on(cursor_id);
        }

        SE_dir_cursor_move();
        break;
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
        if (Menu_Cursor_Y[0] == 11 || IO_Result == 0x200) {
            SE_selected();
            Return_Option_Mode_Sub(task_ptr);
            Order[0x6A] = 4;
            Order_Timer[0x6A] = 4;
            return;
        }

        if (Menu_Cursor_Y[0] == 10) {
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
            return;
        }

        break;

    case 10:
        if ((Menu_Cursor_Y[PL_id] == 10) || (IO_Result == 0x200)) {
            SE_selected();
            Return_Option_Mode_Sub(task_ptr);
            Order[0x6B] = 4;
            Order_Timer[0x6B] = 4;
            return;
        }

        if (Menu_Cursor_Y[PL_id] == 9) {
            SE_selected();
            Setup_IO_ConvDataDefault(PL_id);
            Save_Game_Data();
            return;
        }

        break;

    case 13:
        if (IO_Result == 0x200) {
            SE_selected();
            Return_Option_Mode_Sub(task_ptr);
            Order[0x69] = 4;
            Order_Timer[0x69] = 4;
            return;
        }

        switch (Menu_Cursor_Y[0]) {
        case 3:
            SE_selected();
            Return_Option_Mode_Sub(task_ptr);
            Order[0x69] = 4;
            Order_Timer[0x69] = 4;
            break;

        case 0:
            SE_selected();
            task_ptr->r_no[2] = 4;
            task_ptr->r_no[3] = 0;
            break;

        case 1:
            SE_selected();
            task_ptr->r_no[2] = 5;
            task_ptr->r_no[3] = 0;
            break;

        case 2:
            task_ptr->r_no[2] = 6;
            task_ptr->r_no[3] = 0;
            break;
        }

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

void Screen_Adjust(struct _TASK* task_ptr) {
    s16 char_index;
    s16 ix;

    s16 unused_s3;
    s16 unused_s2;

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
        effect_57_init(0x65, 3, 0, 0x3F, 2);
        Order[0x65] = 1;
        Order_Dir[0x65] = 8;
        Order_Timer[0x65] = 1;

        for (ix = 0; ix < 5; ix++) {
            effect_63_init(ix + 0x66, 0, 2, ix, ix);
            Order[ix + 0x66] = 1;
            Order_Dir[ix + 0x66] = 4;
            Order_Timer[ix + 0x66] = ix + 0x14;
        }

        for (ix = 0, unused_s3 = char_index = 0xE; ix < 7; ix++, unused_s2 = char_index++) {
            effect_61_init(0, ix + 0x50, 0, 2, char_index, ix, 0x7047);
            Order[ix + 0x50] = 1;
            Order_Dir[ix + 0x50] = 4;
            Order_Timer[ix + 0x50] = ix + 0x14;
        }

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
    Convert_Buff[2][0][3] = sys_w.screen_mode;
    save_w[1].Screen_Mode = sys_w.screen_mode;
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
        sys_w.screen_mode = 1;
    }
}

void Screen_Move_Sub_LR(u16 sw) {
    s16 flag = 0;

    if (sw == 4) {
        switch (Menu_Cursor_Y[0]) {
        case 0:
            X_Adjust_Buff[2] -= 2;

            if (X_Adjust_Buff[2] < -10) {
                X_Adjust_Buff[2] = -10;
            } else {
                flag = 1;
            }

            break;

        case 1:
            Y_Adjust_Buff[2] -= 2;

            if (Y_Adjust_Buff[2] < -10) {
                Y_Adjust_Buff[2] = -10;
            } else {
                flag = 1;
            }

            break;

        case 2:
            Disp_Size_H -= 2;

            if (Disp_Size_H < 94) {
                Disp_Size_H = 94;
            } else {
                flag = 1;
            }

            break;

        case 3:
            Disp_Size_V -= 2;

            if (Disp_Size_V < 94) {
                Disp_Size_V = 94;
            } else {
                flag = 1;
            }

            break;

        case 4:
            sys_w.screen_mode = (sys_w.screen_mode + 1) & 1;
            flag = 1;
            break;
        }
    } else if (sw == 8) {
        switch (Menu_Cursor_Y[0]) {
        case 0:
            X_Adjust_Buff[2] += 2;

            if (X_Adjust_Buff[2] > 10) {
                X_Adjust_Buff[2] = 10;
            } else {
                flag = 1;
            }

            break;

        case 1:
            Y_Adjust_Buff[2] += 2;

            if (Y_Adjust_Buff[2] > 10) {
                Y_Adjust_Buff[2] = 10;
            } else {
                flag = 1;
            }

            break;

        case 2:
            Disp_Size_H += 2;

            if (Disp_Size_H > 100) {
                Disp_Size_H = 100;
            } else {
                flag = 1;
            }

            break;

        case 3:
            Disp_Size_V += 2;

            if (Disp_Size_V > 100) {
                Disp_Size_V = 100;
            } else {
                flag = 1;
            }

            break;

        case 4:
            sys_w.screen_mode = (sys_w.screen_mode + 1) & 1;
            flag = 1;
            break;
        }
    }

    if (flag) {
        SE_dir_cursor_move();
    }

    X_Adjust = X_Adjust_Buff[0] = X_Adjust_Buff[1] = X_Adjust_Buff[2];
    Y_Adjust = Y_Adjust_Buff[0] = Y_Adjust_Buff[1] = Y_Adjust_Buff[2];
}

void Sound_Test(struct _TASK* task_ptr) {
    s16 char_index;
    s16 ix;
    u8 last_mode;

    Clear_Flash_Sub();

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        task_ptr->timer = 5;
        setupAlwaysSeamlessFlag(((plsw_00[0] | plsw_00[1]) & 0x4000) != 0);
        Clear_Flash_Init(4);
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Convert_Buff[3][1][5] = 0;

        if (sys_w.sound_mode == 0) {
            Convert_Buff[3][1][0] = 0;
        } else {
            Convert_Buff[3][1][0] = 1;
        }

        if (sys_w.bgm_type == BGM_ARRANGED) {
            Convert_Buff[3][1][3] = 0;
        } else {
            Convert_Buff[3][1][3] = 1;
        }

        Convert_Buff[3][1][7] = 1;
        Order[0x4F] = 4;
        Order_Timer[0x4F] = 1;
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 2;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x72, 4, 0, 0x3F, 2);
        Order[0x72] = 1;
        Order_Dir[0x72] = 8;
        Order_Timer[0x72] = 1;
        effect_04_init(2, 6, 2, 0x48);

        {
            s32 ixSoundMenuItem[4] = { 10, 11, 11, 12 };

            for (ix = 0; ix < 4; ix++) {
                Order[ix + 0x57] = 1;
                Order_Dir[ix + 0x57] = 4;
                Order_Timer[ix + 0x57] = ix + 0x14;
                effect_64_init(ix + 0x57, 0, 2, ixSoundMenuItem[ix] + 1, ix, 0x7047, ix + 0xC, 3, 1);
            }
        }

        Order_Dir[0x78] = 0;
        effect_A8_init(0, 0x78, 0, 2, 5, 0x70A7, 0);
        Order_Dir[0x79] = 1;
        effect_A8_init(0, 0x79, 0, 2, 5, 0x70A7, 1);
        effect_A8_init(3, 0x7A, 0, 2, 5, 0x70A7, 3);
        Convert_Buff[3][1][5] = 0;
        Order_Dir[0x7B] = 0;
        effect_A8_init(2, 0x7B, 0, 2, 5, 0x70A7, 2);

        {
            s16 unused_s2;
            s16 unused_s3;

            for (ix = 0, unused_s3 = char_index = 0x3B; ix < 7; ix++, unused_s2 = char_index++) {
                effect_61_init(0, ix + 0x50, 0, 2, char_index, ix, 0x7047);
                Order[ix + 0x50] = 1;
                Order_Dir[ix + 0x50] = 4;
                Order_Timer[ix + 0x50] = ix + 0x14;
            }
        }

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
        last_mode = Convert_Buff[3][1][0];
        Sound_Cursor_Sub(0);

        if (IO_Result == 0) {
            Sound_Cursor_Sub(1);
        }

        if ((Menu_Cursor_Y[0] == 4) && (IO_Result == 0x100)) {
            SE_selected();
            Convert_Buff[3][1][0] = 0;
            Convert_Buff[3][1][1] = 0xF;
            Convert_Buff[3][1][2] = 0xF;
            Convert_Buff[3][1][3] = 0;
        }

        if (bgm_level != (s16)Convert_Buff[3][1][1]) {
            bgm_level = Convert_Buff[3][1][1];
            save_w[Present_Mode].BGM_Level = Convert_Buff[3][1][1];
            SsBgmHalfVolume(0);
        }

        if (se_level != (s16)Convert_Buff[3][1][2]) {
            se_level = Convert_Buff[3][1][2];
            setSeVolume(save_w[Present_Mode].SE_Level = Convert_Buff[3][1][2]);
        }

        save_w[Present_Mode].BgmType = Convert_Buff[3][1][3];

        if (sys_w.bgm_type != Convert_Buff[3][1][3]) {
            sys_w.bgm_type = Convert_Buff[3][1][3];
            Convert_Buff[3][1][5] = 0;
            BGM_Request_Code_Check(0x41);
        }

        Order_Dir[0x7B] = Convert_Buff[3][1][5];
        Setup_Sound_Mode(last_mode);
        Save_Game_Data();

        if (Menu_Cursor_Y[0] == 5) {
            if (IO_Result == 0x100) {
                SsRequest((u16)Order_Dir[0x7B] + 1);
                Convert_Buff[3][1][7] = 1;
                return;
            }

            if ((IO_Result == 0x200) && Convert_Buff[3][1][7]) {
                Convert_Buff[3][1][7] = 0;
                BGM_Stop();
                return;
            }
        }

        if (IO_Result == 0x200 || ((Menu_Cursor_Y[0] == 6) && (IO_Result == 0x100 || IO_Result == 0x4000))) {
            SE_selected();
            Return_Option_Mode_Sub(task_ptr);
            setupAlwaysSeamlessFlag(0);
            Order[0x72] = 4;
            Order_Timer[0x72] = 4;
            BGM_Request_Code_Check(0x41);
        }

        break;
    }
}

void Setup_Sound_Mode(u8 last_mode) {
    if (last_mode == Convert_Buff[3][1][0]) {
        return;
    }

    sys_w.sound_mode = Convert_Buff[3][1][0];
    setupSoundMode();
    SsBgmHalfVolume(0);
}

u16 Sound_Cursor_Sub(s16 PL_id) {
    u16 sw;
    u16 ret;

    sw = ~plsw_01[PL_id] & plsw_00[PL_id];
    sw = Check_Menu_Lever(PL_id, 0);
    ret = MC_Move_Sub(sw, 0, 6, 0xFF);
    ret |= SD_Move_Sub_LR(sw);
    ret &= 0x20F;
    return ret;
}

const u8 Sound_Data_Max[3][6] = { { 1, 0, 0, 1, 0, 66 }, { 1, 15, 15, 1, 0, 66 }, { 0, 15, 15, 0, 0, 0 } };

u16 SD_Move_Sub_LR(u16 sw) {
    u16 rnum;
    s16 max;
    s8 last_cursor;

    rnum = 0;

    if (Menu_Cursor_Y[0] == 4 || Menu_Cursor_Y[0] == 6) {
        return 0;
    }

    last_cursor = Convert_Buff[3][1][Menu_Cursor_Y[0]];

    switch (sw) {
    case 4:
        max = Sound_Data_Max[0][Menu_Cursor_Y[0]];

        while (1) {
            Convert_Buff[3][1][Menu_Cursor_Y[0]] -= 1;

            if (Convert_Buff[3][1][Menu_Cursor_Y[0]] < 0) {
                Convert_Buff[3][1][Menu_Cursor_Y[0]] = max;
            }

            if ((Menu_Cursor_Y[0] != 5) || (bgmSkipCheck(Convert_Buff[3][1][5] + 1) == 0)) {
                break;
            }
        }

        if (last_cursor != Convert_Buff[3][1][Menu_Cursor_Y[0]]) {
            rnum = 4;
        }

        break;

    case 8:
        max = Sound_Data_Max[1][Menu_Cursor_Y[0]];

        while (1) {
            Convert_Buff[3][1][Menu_Cursor_Y[0]] += 1;

            if (Convert_Buff[3][1][Menu_Cursor_Y[0]] > max) {
                Convert_Buff[3][1][Menu_Cursor_Y[0]] = Sound_Data_Max[2][Menu_Cursor_Y[0]];
            }

            if ((Menu_Cursor_Y[0] != 5) || (bgmSkipCheck(Convert_Buff[3][1][5] + 1) == 0)) {
                break;
            }
        }

        if (last_cursor != Convert_Buff[3][1][Menu_Cursor_Y[0]]) {
            rnum = 8;
        }

        break;
    }

    if (rnum) {
        SE_dir_cursor_move();
    }

    return rnum;
}

void Memory_Card(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_index;

    s16 unused_s3;
    s16 unused_s2;

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
        Order_Dir[0x4E] = 4;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x69, 5, 0, 0x3F, 2);
        Order[0x69] = 1;
        Order_Dir[0x69] = 8;
        Order_Timer[0x69] = 1;

        for (ix = 0, unused_s3 = char_index = 0x15; ix < 4; ix++, unused_s2 = char_index++) {
            effect_61_init(0, ix + 0x50, 1, 2, char_index, ix, 0x7047);
            Order[ix + 0x50] = 1;
            Order_Dir[ix + 0x50] = 4;
            Order_Timer[ix + 0x50] = ix + 0x14;
        }

        Menu_Cursor_Move = 4;
        effect_64_init(0x61, 1, 2, 0, 2, 0x7047, 0, 3, 0);
        Order[0x61] = 1;
        Order_Dir[0x61] = 4;
        Order_Timer[0x61] = 0x18;
        effect_66_init(0x8A, 8, 2, 1, -1, -1, -0x7FF5);
        Order[0x8A] = 3;
        Order_Timer[0x8A] = 1;
        effect_04_init(2, 2, 2, 0x48);
        Setup_File_Property(0, 0xFF);
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
        Memory_Card_Sub(0);
        Button_Exit_Check(task_ptr, 0);

        if (IO_Result == 0) {
            Memory_Card_Sub(1);
            Button_Exit_Check(task_ptr, 0);
        }

        break;

    case 4:
    case 5:
    case 6:
        Save_Load_Menu(task_ptr);
        break;
    }
}

void Save_Load_Menu(struct _TASK* task_ptr) {
    s16 ix;

    Menu_Cursor_X[1] = Menu_Cursor_X[0];

    switch (task_ptr->r_no[3]) {
    case 0:
        task_ptr->r_no[3] += 1;
        task_ptr->timer = 5;

        if (task_ptr->r_no[2] == 5) {
            SaveInit(0, 0);
        } else {
            SaveInit(0, 1);
        }

        Menu_Common_Init();
        Menu_Suicide[3] = 0;
        Target_BG_X[1] = bg_w.bgw[1].wxy[0].disp.pos + 0x180;
        Offset_BG_X[1] = 0;
        Target_BG_X[2] = bg_w.bgw[2].wxy[0].disp.pos + 0x200;
        Offset_BG_X[2] = 0;
        bg_w.bgw[2].speed_x = 0x333333;
        Next_Step = 0;
        bg_mvxy.a[0].sp = 0x266666;
        bg_mvxy.d[0].sp = 0;
        effect_58_init(0xE, 1, 1);
        effect_58_init(0, 1, 2);
        Menu_Cursor_X[0] = Setup_Final_Cursor_Pos(0, 8);
        Message_Data->kind_req = 5;
        break;

    case 1:
        if (Next_Step) {
            task_ptr->r_no[3] += 1;
            task_ptr->free[3] = 0;
        }

        break;

    case 2:
        task_ptr->r_no[3] += 1;
        Menu_Cursor_X[1] = Menu_Cursor_X[0] + 8;
        /* fallthrough */

    case 3:
        if (SaveMove() <= 0) {
            Go_Back_MC(task_ptr);
        }

        break;

    case 4:
        if (Next_Step) {
            task_ptr->r_no[2] = 3;
            task_ptr->r_no[3] = 0;

            for (ix = 0; ix < 4; ix++) {
                Message_Data[ix].order = 3;
            }

            Order[0x78] = 3;
            Order_Timer[0x78] = 1;
        }

        break;

    default:
        Exit_Sub(task_ptr, 1, Menu_Cursor_Y[0] + 7);
        break;
    }
}

void Go_Back_MC(struct _TASK* task_ptr) {
    task_ptr->r_no[3] = 4;
    Menu_Cursor_Y[0] = task_ptr->r_no[2] - 4;
    Target_BG_X[1] = bg_w.bgw[1].wxy[0].disp.pos - 0x180;
    Offset_BG_X[1] = 0;
    Target_BG_X[2] = bg_w.bgw[2].wxy[0].disp.pos - 0x200;
    Offset_BG_X[2] = 0;
    bg_w.bgw[2].speed_x = -0x333333;
    Next_Step = 0;
    bg_mvxy.a[0].sp = 0xFFD9999A;
    bg_mvxy.d[0].sp = 0;
    effect_58_init(0xE, 1, 1);
    effect_58_init(0, 1, 2);
}

s32 Setup_Final_Cursor_Pos(s8 cursor_x, s16 dir) {
    s16 ix;
    s16 check_x[2];
    s16 next_dir;

    if (cursor_x == -1) {
        cursor_x = 0;
    }

    if (vm_w.Connect[cursor_x]) {
        return cursor_x;
    }

    check_x[0] = cursor_x ^ 1;

    if (vm_w.Connect[check_x[0]]) {
        return check_x[0];
    }

    if (dir == 4) {
        next_dir = -2;
    } else {
        next_dir = 2;
    }

    check_x[0] = cursor_x;

    for (ix = 0; ix < 4; ix++) {
        check_x[0] += next_dir;

        if (check_x[0] < 0) {
            if (IO_Result == 0) {
                check_x[0] += 8;
            } else {
                return Menu_Cursor_X[1];
            }
        }

        if (check_x[0] > 7) {
            if (IO_Result == 0) {
                check_x[0] -= 8;
            } else {
                return Menu_Cursor_X[1];
            }
        }

        if (vm_w.Connect[check_x[0]]) {
            return check_x[0];
        }

        check_x[1] = check_x[0] ^ 1;

        if (vm_w.Connect[check_x[1]]) {
            return check_x[1];
        }
    }

    return -1;
}

void Memory_Card_Sub(s16 PL_id) {
    u16 sw;

    sw = ~plsw_01[PL_id] & plsw_00[PL_id];
    sw = Check_Menu_Lever(PL_id, 0);
    MC_Move_Sub(sw, 0, 3, 0xFF);

    if ((Menu_Cursor_Y[0] == 2) && !(IO_Result & 0x200)) {
        IO_Result = 0;
    }

    Memory_Card_Move_Sub_LR(sw, 0);

    if (Convert_Buff[3][0][2] == 0) {
        save_w[Present_Mode].Auto_Save = 0;
    }
}

u16 Memory_Card_Move_Sub_LR(u16 sw, s16 cursor_id) {
    s32 ret;
    s32 idx;
    s32 val;

    idx = Menu_Cursor_Y[cursor_id];

    if (idx != 2) {
        return 0;
    }

    val = Convert_Buff[3][cursor_id][idx];

    switch (sw) {
    case 4:
        val -= 1;

        if (val < 0) {
            val = 1;
        }

        SE_dir_cursor_move();
        ret = 4;
        break;

    case 8:
        val += 1;

        if (val > 1) {
            val = 0;
        }

        SE_dir_cursor_move();
        ret = 8;
        break;

    default:
        ret = 0;
        break;
    }

    Convert_Buff[3][cursor_id][idx] = val;

    if ((ret != 0) && (val == 1)) {
        IO_Result = 0x100;
        Forbid_Reset = 1;
    }

    return ret;
}

u16 MC_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max, s16 cansel_menu) {
    if (Menu_Cursor_Move > 0) {
        return 0;
    }

    switch (sw) {
    case SWK_UP:
        Menu_Cursor_Y[cursor_id] -= 1;

        if (Menu_Cursor_Y[cursor_id] < 0) {
            Menu_Cursor_Y[cursor_id] = menu_max;
        }

        if ((cansel_menu == Menu_Cursor_Y[cursor_id]) && (Connect_Status == 0)) {
            Menu_Cursor_Y[cursor_id] -= 1;
        }

        SE_cursor_move();
        return IO_Result = SWK_UP;

    case SWK_DOWN:
        Menu_Cursor_Y[cursor_id] += 1;

        if (Menu_Cursor_Y[cursor_id] > menu_max) {
            Menu_Cursor_Y[cursor_id] = 0;
        }

        if ((cansel_menu == Menu_Cursor_Y[cursor_id]) && (Connect_Status == 0)) {
            Menu_Cursor_Y[cursor_id] += 1;
        }

        SE_cursor_move();
        return IO_Result = SWK_DOWN;

    case SWK_WEST:
        return IO_Result = SWK_WEST;

    case SWK_SOUTH:
        return IO_Result = SWK_SOUTH;

    case SWK_EAST:
        return IO_Result = SWK_EAST;

    case SWK_RIGHT_TRIGGER:
        return IO_Result = SWK_RIGHT_TRIGGER;

    case SWK_START:
        return IO_Result = SWK_START;

    default:
        return IO_Result = 0;

    case SWK_NORTH:
        return IO_Result = SWK_NORTH;

    case SWK_RIGHT_SHOULDER:
        return IO_Result = SWK_RIGHT_SHOULDER;

    case SWK_LEFT_SHOULDER:
        return IO_Result = SWK_LEFT_SHOULDER;

    case SWK_LEFT_TRIGGER:
        return IO_Result = SWK_LEFT_TRIGGER;
    }
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

void Menu_Select(struct _TASK* task_ptr) {
    s16 ix;

    if (Check_Pad_in_Pause(task_ptr) != 0) {
        return;
    }

    switch (task_ptr->r_no[2]) {
    case 0:
        Pause_1st_Sub(task_ptr);
        break;

    case 1:
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

        break;

    case 2:
        IO_Result = MC_Move_Sub(Check_Menu_Lever(Pause_ID, 0), 0, 2, 0xFF);
        switch (IO_Result) {

        case 0x200:
            task_ptr->r_no[2] = 0;
            Menu_Suicide[0] = 1;
            SE_selected();
            break;

        case 0x100:
            switch (Menu_Cursor_Y[0]) {

            case 0:
                task_ptr->r_no[2] = 0;
                Menu_Suicide[0] = 1;
                SE_selected();
                break;

            case 1:
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

            case 2:
                task_ptr->r_no[2]++;
                Menu_Suicide[0] = 1;
                Menu_Cursor_Y[0] = 1;
                effect_10_init(0, 0, 3, 3, 1, 0x13, 0xC);
                effect_10_init(0, 1, 0, 0, 1, 0x14, 0xF);
                effect_10_init(0, 1, 1, 1, 1, 0x1A, 0xF);
                SE_selected();
                break;
            }

            break;
        }

        break;

    case 3:
        Yes_No_Cursor_Move_Sub(task_ptr);
        break;
    }
}

s32 Yes_No_Cursor_Move_Sub(struct _TASK* task_ptr) {
    u16 sw = ~(plsw_01[Pause_ID]) & plsw_00[Pause_ID];

    switch (sw) {
    case 0x4:
        Menu_Cursor_Y[0]--;

        if (Menu_Cursor_Y[0] < 0) {
            Menu_Cursor_Y[0] = 0;
        } else {
            SE_dir_cursor_move();
        }

        break;

    case 0x8:
        Menu_Cursor_Y[0]++;

        if (Menu_Cursor_Y[0] > 1) {
            Menu_Cursor_Y[0] = 1;
        } else {
            SE_dir_cursor_move();
        }

        break;

    case 0x200:
    case 0x100:
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

void Wait_Load_Save(struct _TASK* task_ptr) {
    s16 ix;

    switch (task_ptr->free[1]) {
    case 0:
        if (vm_w.Request != 0) {
            break;
        }

        task_ptr->free[0] = 0;
        task_ptr->free[1]++;

        if (task_ptr->r_no[1] == 5) {
            task_ptr->free[2] = 18;
        } else {
            task_ptr->free[2] = task_ptr->r_no[1];
        }

        Exit_Sub(task_ptr, 2, task_ptr->free[2]);
        break;

    case 1:
        if (!Exit_Sub(task_ptr, 2, task_ptr->free[2])) {
            break;
        }

        task_ptr->free[1]++;
        task_ptr->timer = 1;

        for (ix = 0; ix < 4; ix++) {
            Menu_Suicide[ix] = 1;
        }

        switch (task_ptr->r_no[1]) {
        case 13:
            ix = 105;
            break;

        case 17:
            task_ptr->r_no[2] = 99;
            /* fallthrough */

        case 6:
            ix = 110;
            break;

        case 19:
        case 20:
            ix = 112;
            break;

        case 23:
            ix = 105;
            task_ptr->r_no[0] = 0;
            task_ptr->r_no[2] = 99;
            task_ptr->free[0] = 1;
            task_ptr->free[1] = 8;
            break;
        }

        Order[ix] = 4;
        Order_Timer[ix] = 1;
        break;

    case 2:
        FadeOut(1, 0xFF, 8);

        if (--task_ptr->timer == 0) {
            task_ptr->r_no[0] = 0;
        }

        break;
    }
}

void Disp_Auto_Save(struct _TASK* task_ptr) {
    void (*Auto_Save_Jmp_Tbl[4])() = { DAS_1st, DAS_2nd, DAS_3rd, DAS_4th };
    Auto_Save_Jmp_Tbl[task_ptr->r_no[1]](task_ptr);
}

void DAS_1st(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);
    task_ptr->r_no[1]++;
    task_ptr->timer = 5;
    Order[0x4E] = 2;
    Order_Dir[0x4E] = 0;
    Order_Timer[0x4E] = 1;
    effect_66_init(0x8A, 8, 0, 0, -1, -1, -0x7FFD);
    Order[0x8A] = 3;
    Order_Timer[0x8A] = 1;
}

void DAS_2nd(struct _TASK* task_ptr) {
    FadeOut(1, 0xFF, 8);

    if ((task_ptr->timer -= 1) == 0) {
        task_ptr->r_no[1]++;
        FadeInit();
        SaveInit(0, 3);
    }
}

void DAS_3rd(struct _TASK* task_ptr) {
    if (FadeIn(1, 0x19, 8) != 0) {
        task_ptr->r_no[1]++;
    }
}

void DAS_4th(struct _TASK* task_ptr) {
    if (SaveMove() <= 0) {
        task_ptr->r_no[0] = 0;
        task_ptr->r_no[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        Forbid_Reset = 0;
    }
}

void Disp_Auto_Save2(struct _TASK* task_ptr) {
    void (*Auto_Save2_Jmp_Tbl[4])() = { DAS_1st, DAS_2nd, DAS_3rd, DAS2_4th };
    Auto_Save2_Jmp_Tbl[task_ptr->r_no[1]](task_ptr);
}

void DAS2_4th(struct _TASK* task_ptr) {
    if (SaveMove() <= 0) {
        G_No[2] = 6;
        cpExitTask(TASK_MENU);
        task[TASK_ENTRY].condition = 1;
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

void VS_Result(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_ix2;
    s16 total_battle;
    u16 ave[2];

    s16 s4;
    s16 s3;

    Clear_Flash_Sub();

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
        total_battle = VS_Win_Record[0] + VS_Win_Record[1];

        if (total_battle == 0) {
            total_battle = 1;
        }

        if (VS_Win_Record[0] >= VS_Win_Record[1]) {
            ave[1] = (VS_Win_Record[1] * 100) / total_battle;

            if (ave[1] == 0 && VS_Win_Record[1] > 0) {
                ave[1] = 1;
            }

            ave[0] = 100 - ave[1];
        } else {
            ave[0] = (VS_Win_Record[0] * 100) / total_battle;

            if (ave[0] == 0 && VS_Win_Record[0] > 0) {
                ave[0] = 1;
            }

            ave[1] = 100 - ave[0];
        }

        effect_A0_init(0, ave[0], 2, 3, 0, 0, 0);
        effect_A0_init(0, ave[1], 3, 3, 0, 0, 0);

        for (ix = 0, s4 = char_ix2 = 22; ix < 3; ix++, s3 = char_ix2++) {
            effect_91_init(0, ix, 0, 71, char_ix2, 0);
            effect_91_init(1, ix, 0, 71, char_ix2, 0);
        }

        Setup_Win_Lose_OBJ();
        Menu_Cursor_Move = 0;
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

    case 4:
        if (VS_Result_Select_Sub(task_ptr, 0) == 0) {
            VS_Result_Select_Sub(task_ptr, 1);
        }

        break;

    case 5:
        if (task_ptr->r_no[3] == 0) {
            if (--task_ptr->timer == 0) {
                task_ptr->r_no[3]++;
            }

            break;
        }

        Exit_Sub(task_ptr, 0, 17);
        break;

    case 6:
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

        break;

    case 7:
    default:
        Netplay_HandleMenuExit();

        if (Exit_Sub(task_ptr, 0, 0)) {
            System_all_clear_Level_B();
            BGM_Request_Code_Check(65);
        }

        break;
    }
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

u16 After_VS_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max) {
    s16 skip;

    if (plw[0].wu.operator == 0 || plw[1].wu.operator == 0 || Mode_Type == MODE_NETWORK) {
        skip = 1;
    } else {
        skip = 99;
    }

    if (Debug_w[49]) {
        skip = 99;
    }

    switch (sw) {
    case SWK_UP:
        Menu_Cursor_Y[cursor_id]--;

        if (Menu_Cursor_Y[cursor_id] < 0) {
            Menu_Cursor_Y[cursor_id] = menu_max;
        }

        if (Menu_Cursor_Y[cursor_id] == skip) {
            Menu_Cursor_Y[cursor_id] = 0;
        }

        SE_cursor_move();
        return IO_Result = SWK_UP;

    case SWK_DOWN:
        Menu_Cursor_Y[cursor_id]++;

        if (Menu_Cursor_Y[cursor_id] > menu_max) {
            Menu_Cursor_Y[cursor_id] = 0;
        }

        if (Menu_Cursor_Y[cursor_id] == skip) {
            Menu_Cursor_Y[cursor_id] = 2;
        }

        SE_cursor_move();
        return IO_Result = SWK_DOWN;

    case SWK_WEST:
        return IO_Result = SWK_WEST;

    case SWK_SOUTH:
        return IO_Result = SWK_SOUTH;

    case SWK_EAST:
        return IO_Result = SWK_EAST;

    case SWK_RIGHT_TRIGGER:
        return IO_Result = SWK_RIGHT_TRIGGER;

    case SWK_START:
        return IO_Result = SWK_START;

    default:
        return IO_Result = 0;

    case SWK_NORTH:
        return IO_Result = SWK_NORTH;

    case SWK_RIGHT_SHOULDER:
        return IO_Result = SWK_RIGHT_SHOULDER;

    case SWK_LEFT_SHOULDER:
        return IO_Result = SWK_LEFT_SHOULDER;

    case SWK_LEFT_TRIGGER:
        return IO_Result = SWK_LEFT_TRIGGER;
    }
}

s32 VS_Result_Move_Sub(struct _TASK* task_ptr, s16 PL_id) {
    switch (IO_Result) {
    case SWK_SOUTH:
        switch (Menu_Cursor_Y[PL_id]) {
        case 0:
            SE_selected();
            Menu_Cursor_X[PL_id] = 1;

            if (!Menu_Cursor_X[PL_id ^ 1]) {
                break;
            }

            task_ptr->r_no[2] = 6;
            task_ptr->r_no[3] = 0;
            task_ptr->timer = 15;
            return 1;

        case 1:
            SE_selected();
            task_ptr->r_no[2] = 5;
            task_ptr->r_no[3] = 0;
            task_ptr->timer = 15;
            return 1;

        case 2:
            SE_selected();
            task_ptr->r_no[2] = 7;
            task_ptr->r_no[3] = 0;
            task_ptr->timer = 15;
            return 1;
        }

        break;

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
            SaveInit(2, 1);
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
    Setup_Replay_Sub(1, 110, 9, 1);
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

void Setup_Replay_Sub(s16 /* unused */, s16 type, s16 char_type, s16 master_player) {
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

void Wait_Pause_in_Tr(struct _TASK* task_ptr) {
    u16 ans;
    u16 ix;

    Training_Data_Disp();
    Control_Player_Tr();

    if (End_Training) {
        Next_Be_Tr_Menu(task_ptr);
        return;
    }

    switch (task_ptr->r_no[1]) {
    case 0:
        if (Allow_a_battle_f) {
            task_ptr->r_no[1]++;

            if (Present_Mode == 4) {
                Disp_Attack_Data = Training->contents[0][1][1];
            } else {
                Disp_Attack_Data = 0;
            }
        } else {
            Disp_Attack_Data = 0;
        }

        /* fallthrough */

    case 1:
        if (Allow_a_battle_f == 0 || Extra_Break != 0) {
            return;
        }

        ans = 0;

        if (Check_Pause_Term_Tr(0)) {
            ans = Pause_Check_Tr(0);
        }

        if (ans == 0 && Check_Pause_Term_Tr(1)) {
            ans = Pause_Check_Tr(1);
        }

        switch (ans) {
        case 1:
            Setup_Tr_Pause(task_ptr);
            break;

        case 2:
            Setup_Tr_Pause(task_ptr);
            task_ptr->r_no[1] = 3;
            break;
        }

        break;

    case 2:
        if (Interface_Type[Pause_ID] == 0) {
            Setup_Tr_Pause(task_ptr);
            task_ptr->r_no[1] = 3;
            break;
        }

        if (Pause_Down) {
            Flash_1P_or_2P(task_ptr);
        }

        switch (Pause_in_Normal_Tr(task_ptr)) {
        case 1:
            task_ptr->r_no[1] = 0;
            SE_selected();
            Game_pause = 0;
            Pause = 0;
            Pause_Down = 0;
            Disp_Attack_Data = Training->contents[0][1][1];

            for (ix = 0; ix < 4; ix++) {
                Menu_Suicide[ix] = 1;
            }

            pulpul_request_again();
            SsBgmHalfVolume(0);
            break;

        case 2:
            Next_Be_Tr_Menu(task_ptr);
            break;
        }

        break;

    case 3:
        if (Interface_Type[Pause_ID] == 0) {
            dispControllerWasRemovedMessage(132, 82, 16);
            break;
        }

        Setup_Tr_Pause(task_ptr);
        break;
    }
}

void Control_Player_Tr() {
    switch (control_pl_rno) {
    case 0:
        if (control_player) {
            p2sw_0 = 0;
            break;
        }

        p1sw_0 = 0;
        break;

    case 1:
        if (control_player) {
            p2sw_0 = 2;
            break;
        }

        p1sw_0 = 2;
        break;

    case 2:
        if (control_player) {
            p2sw_0 = 1;
            break;
        }

        p1sw_0 = 1;
        break;
    }
}

void Next_Be_Tr_Menu(struct _TASK* task_ptr) {
    s16 ix;

    apply_training_hitbox_display(true);
    task_ptr->r_no[0] = 11;
    task_ptr->r_no[1] = 0;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    Allow_a_battle_f = 0;

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 1;
    }

    SsBgmHalfVolume(0);
}

s32 Check_Pause_Term_Tr(s16 PL_id) {
    if (Mode_Type == MODE_PARRY_TRAINING) {
        if (PL_id == Champion) {
            return 1;
        }

        return 0;
    }

    if (PL_id == Champion) {
        return 1;
    }

    if (Training->contents[0][1][3] == 2) {
        return 0;
    }

    if (Training->contents[0][0][0] == 4) {
        return 1;
    }

    return 0;
}

s32 Pause_Check_Tr(s16 PL_id) {
    u16 sw;

    if (plw[PL_id].wu.operator == 0) {
        return 0;
    }

    sw = ~(PLsw[PL_id][1]) & PLsw[PL_id][0];

    if (sw & SWK_START) {
        Pause_ID = PL_id;
        return 1;
    }

    if (Interface_Type[PL_id] == 0) {
        Pause_ID = PL_id;
        return 2;
    }

    return 0;
}

void Setup_Tr_Pause(struct _TASK* task_ptr) {
    task_ptr->r_no[1] = 2;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    task_ptr->free[0] = 60;
    Cursor_Y_Pos[0][0] = 0;
    Disp_Attack_Data = 0;
    Game_pause = 0x81;
    Pause_Down = 1;
    Menu_Suicide[0] = 1;
    Menu_Suicide[1] = 1;
    Menu_Suicide[2] = 0;
    Order[138] = 3;
    Order_Timer[138] = 1;
    effect_66_init(138, 9, 2, 7, -1, -1, -0x3FFC);
    SsBgmHalfVolume(1);
    spu_all_off();
}

void Flash_1P_or_2P(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[3]) {
    case 0:
        if (--task_ptr->free[0]) {
            if (Pause_ID == 0) {
                SSPutStr2(20, 9, 9, "1P PAUSE");
                break;
            } else {
                SSPutStr2(20, 9, 9, "2P PAUSE");
                break;
            }
        }

        task_ptr->r_no[3] = 1;
        task_ptr->free[0] = 0x1E;
        break;

    case 1:
        if (--task_ptr->free[0] == 0) {
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0x3C;
        }

        break;
    }
}

s32 Pause_in_Normal_Tr(struct _TASK* task_ptr) {
    s16 ix;
    u16 sw;

    Control_Player_Tr();

    switch (task_ptr->r_no[2]) {
    case 0:
        return Pause_1st_Sub(task_ptr);

    case 1:
        task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = Cursor_Y_Pos[0][0];

        for (ix = 0; ix < 4; ix++) {
            Menu_Suicide[ix] = 0;
        }

        effect_10_init(0, 6, 0, 0, 0, 20, 12);
        effect_10_init(0, 6, 1, 1, 0, 18, 14);
        effect_10_init(0, 6, 2, 2, 0, 22, 16);
        break;

    case 2:
        if (Pause_Down) {
            IO_Result = MC_Move_Sub(Check_Menu_Lever(Pause_ID, 0), 0, 2, 0xFF);
        } else {
            sw = ~PLsw[Pause_ID][1] & PLsw[Pause_ID][0];

            if (sw & SWK_ATTACKS) {
                IO_Result = SWK_WEST;
            } else {
                return 3;
            }
        }

        switch (IO_Result) {
        case SWK_EAST:
            task_ptr->r_no[2] = 0;
            Menu_Suicide[0] = 1;
            SE_selected();
            break;

        case SWK_SOUTH:
            switch (Menu_Cursor_Y[0]) {
            case 0:
                task_ptr->r_no[2] = 0;
                Menu_Suicide[0] = 1;
                SE_selected();
                break;

            case 1:
                Cursor_Y_Pos[0][0] = 0;
                return 2;

            case 2:
                task_ptr->r_no[2]++;
                SE_selected();
                Menu_Suicide[0] = 1;
                Menu_Cursor_Y[0] = 1;
                effect_10_init(0, 0, 3, 6, 1, 17, 12);
                effect_10_init(0, 1, 0, 0, 1, 20, 15);
                effect_10_init(0, 1, 1, 1, 1, 26, 15);
                break;
            }

            break;
        }

        break;

    case 3:
        sw = ~plsw_01[Pause_ID] & plsw_00[Pause_ID];

        if (Pause_Down) {
            Yes_No_Cursor_Move_Sub(task_ptr);
        }

        break;
    }

    return 0;
}

s32 Pause_1st_Sub(struct _TASK* task_ptr) {
    u16 sw = ~plsw_01[Pause_ID] & plsw_00[Pause_ID];

    if (Pause_Down) {
        SSPutStr2(17, 12, 9, "PRESS   BUTTON");
        dispButtonImage2(0xB2, 0x5B, 1, 0x13, 0xF, 0, 4);
        SSPutStr2(18, 14, 9, "TO PAUSE MENU");
    }

    if (sw & SWK_START) {
        if (((Mode_Type == MODE_NORMAL_TRAINING) || (Mode_Type == MODE_PARRY_TRAINING)) &&
            (Check_Pause_Term_Tr(Pause_ID ^ 1) != 0) && plw[Pause_ID ^ 1].wu.operator &&
            (Interface_Type[Pause_ID ^ 1] == 0)) {
            Pause_ID = Pause_ID ^ 1;
            return 0;
        }

        task_ptr->r_no[2] = 0x63;
        Exit_Menu = 1;
        SE_selected();
        return 1;
    }

    if (sw & SWK_SOUTH) {
        task_ptr->r_no[2] += 1;
        Cursor_Y_Pos[0][0] = 0;
        SE_selected();
    }

    return 0;
}

void Reset_Training(struct _TASK* task_ptr) {
    s16 ix;

    switch (task_ptr->r_no[1]) {
    case 0:
        task_ptr->r_no[1]++;
        task_ptr->timer = 10;
        Game_pause = 0x81;
        break;

    case 1:
        if (--task_ptr->timer != 0) {
            break;
        }

        if (Check_LDREQ_Break() == 0) {
            task_ptr->r_no[1]++;
            Switch_Screen_Init(0);
            break;
        }

        task_ptr->timer = 1;
        break;

    case 2:
        if (!Switch_Screen(0)) {
            break;
        }

        task_ptr->r_no[1]++;
        task_ptr->timer = 2;
        effect_work_kill(6, -1);
        move_effect_work(6);

        for (ix = 0; ix < 4; ix++) {
            C_No[ix] = 0;
        }

        C_No[0] = 1;
        G_No[2] = 5;
        G_No[3] = 0;
        seraph_flag = 0;
        BGM_No[0] = 1;
        BGM_Timer[0] = 1;
        G_Timer = 10;
        Cover_Timer = 5;
        Suicide[0] = 1;
        Suicide[6] = 1;
        judge_flag = 0;
        Lever_LR[0] = 0;
        Lever_LR[1] = 0;
        break;

    default:
        Switch_Screen(0);

        if (--task_ptr->timer != 0) {
            break;
        }

        for (ix = 0; ix < 4; ix++) {
            task_ptr->r_no[ix] = 0;
        }

        task_ptr->r_no[0] = 7;
        break;
    }
}

void Reset_Replay(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[1]) {
    case 0:
        task_ptr->r_no[1]++;
        task_ptr->timer = 10;
        Game_pause = 0x81;
        break;

    case 1:
        if (--task_ptr->timer != 0) {
            break;
        }

        if (Check_LDREQ_Break() == 0) {
            task_ptr->r_no[1]++;
            Switch_Screen_Init(0);
            break;
        }

        task_ptr->timer = 1;
        break;

    case 2:
        if (!Switch_Screen(0)) {
            break;
        }

        task_ptr->r_no[1]++;
        task_ptr->timer = 2;
        G_No[2] = 2;
        G_No[3] = 0;
        seraph_flag = 0;
        G_Timer = 10;
        Cover_Timer = 5;
        effect_work_kill_mod_plcol();
        move_effect_work(6);
        Suicide[0] = 1;
        Suicide[6] = 1;
        judge_flag = 0;
        cpExitTask(TASK_PAUSE);
        break;

    default:
        Switch_Screen(0);

        if (--task_ptr->timer == 0) {
            cpExitTask(TASK_MENU);
        }

        break;
    }
}

void Training_Menu(struct _TASK* task_ptr) {
    void (*Training_Jmp_Tbl[8])() = { Training_Init,   Normal_Training,  Blocking_Training, Dummy_Setting,
                                      Training_Option, Button_Config_Tr, Character_Change,  Blocking_Tr_Option };
    Training_Jmp_Tbl[task_ptr->r_no[1]](task_ptr);
    Akaobi();
    ToneDown(0xAA, 2);
    SSPutStr_Bigger(
        training_letter_data[Training_Index].pos_x, 0x18, 9, training_letter_data[Training_Index].menu, 1, 2, 1);
}

void Training_Init(struct _TASK* task_ptr) {
    ToneDown(0x80, 2);
    Menu_Init(task_ptr);
    task_ptr->r_no[1] = Mode_Type - 2;
    Pause_Down = 1;
    End_Training = 0;
    Demo_Time_Stop = 0;
    Disp_Cockpit = 0;

    if (Mode_Type == MODE_NORMAL_TRAINING) {
        control_player = Champion;
        control_pl_rno = 0x63;
    } else {
        control_player = Champion;
        control_pl_rno = 0;
    }

    Round_num = 0;
    PL_Wins[0] = 0;
    PL_Wins[1] = 0;
    Play_Mode = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;
}

void Normal_Training(struct _TASK* task_ptr) {
    s16 ix;
    s16 x;
    s16 y;

    s16 s2;

    Menu_Cursor_Y[1] = Menu_Cursor_Y[0];

    switch (task_ptr->r_no[2]) {
    case 0:
        Training_Init_Sub(task_ptr);
        Training_Index = 0;
        x = 120;
        y = 56;
        Training[0] = Training[2];

        for (ix = 0; ix < 8; ix++, s2 = y += 16) {
            (void)s2;

            effect_A3_init(0, 0, ix, ix, 0, x, y, 0);
        }

        break;

    case 1:
        if (Appear_end < 2) {
            break;
        }

        if (Exec_Wipe) {
            break;
        }

        MC_Move_Sub(Check_Menu_Lever(Decide_ID, 0), 0, 7, 0xFF);
        Check_Skip_Recording();
        Check_Skip_Replay(2);

        switch (IO_Result) {
        case 0x100:
            switch (Menu_Cursor_Y[0]) {
            case 0:
            case 1:
            case 2:
                if (Interface_Type[Champion ^ 1] == 0 && Training[2].contents[0][0][0] == 4) {
                    Training[2].contents[0][0][0] = 0;
                }

                task_ptr->r_no[0] = 10;
                task_ptr->r_no[1] = 0;
                task_ptr->r_no[2] = 0;
                task_ptr->r_no[3] = 0;
                Menu_Suicide[0] = 1;
                Game_pause = 0;
                Pause_Down = 0;
                Training_Disp_Work_Clear();
                CP_No[0][0] = 0;
                CP_No[1][0] = 0;
                plw[New_Challenger].wu.operator = 1;
                Operator_Status[New_Challenger] = 1;
                Setup_NTr_Data(Menu_Cursor_Y[0]);
                count_cont_init(0);

                switch (Training[0].contents[0][0][0]) {
                case 0:
                    control_pl_rno = 0;
                    control_player = New_Challenger;
                    break;

                case 1:
                    control_pl_rno = 1;
                    control_player = New_Challenger;
                    break;

                case 2:
                    control_pl_rno = 2;
                    control_player = New_Challenger;
                    break;

                case 3:
                    control_pl_rno = 99;
                    plw[New_Challenger].wu.operator = 0;
                    Operator_Status[New_Challenger] = 0;
                    break;

                case 4:
                    control_pl_rno = 99;
                    break;
                }

                All_Clear_Timer();
                Check_Replay();
                Training[0].contents[0][1][3] = Menu_Cursor_Y[0];
                init_omop();
                set_init_A4_flag();
                setup_vitality(&plw[0].wu, My_char[0] + 0);
                setup_vitality(&plw[1].wu, My_char[1] + 0);
                Setup_Training_Difficulty();
                Training_Cursor = Menu_Cursor_Y[0];
                break;

            case 3:
            case 4:
            case 5:
            case 6:
                task_ptr->r_no[1] = Menu_Cursor_Y[0];
                task_ptr->r_no[2] = 0;
                task_ptr->r_no[3] = 0;
                Training_Cursor = Menu_Cursor_Y[0];
                break;

            case 7:
                Training_Cursor = 7;
                Training_Exit_Sub(task_ptr);
            }

            SsBgmHalfVolume(0);
            SE_selected();
        }

        break;

    case 2:
        Yes_No_Cursor_Exit_Training(task_ptr, 7);
        break;

    default:
        Exit_Sub(task_ptr, 0, Menu_Cursor_Y[0] + 1);
        break;
    }
}

void Setup_NTr_Data(s16 ix) {
    switch (ix) {
    case 0:
        Play_Mode = 0;
        Replay_Status[0] = 0;
        Replay_Status[1] = 0;
        save_w[Present_Mode].Time_Limit = -1;
        save_w[Present_Mode].Damage_Level = Training[2].contents[0][1][2];
        Training[0] = Training[2];
        break;

    case 1:
        Record_Data_Tr = 1;
        Play_Mode = 1;
        Replay_Status[0] = 1;
        Replay_Status[1] = 1;
        save_w[Present_Mode].Time_Limit = 60;
        save_w[Present_Mode].Damage_Level = Training[2].contents[0][1][2];
        Training[0] = Training[2];
        Training[1] = Training[2];
        break;

    case 2:
        Play_Mode = 3;
        Replay_Status[0] = 3;
        Replay_Status[1] = 3;
        save_w[Present_Mode].Time_Limit = 60;
        save_w[Present_Mode].Damage_Level = Training[1].contents[0][1][2];
        Training[0] = Training[1];
        break;
    }

    apply_training_hitbox_display(false);
}

void Check_Skip_Replay(s16 ix) {
    if (Menu_Cursor_Y[0] != ix) {
        return;
    }

    if (Record_Data_Tr != 0) {
        return;
    }

    if (Menu_Cursor_Y[0] >= Menu_Cursor_Y[1]) {
        Menu_Cursor_Y[0]++;
        return;
    }

    Menu_Cursor_Y[0]--;
    Check_Skip_Recording();
}

void Check_Skip_Recording() {
    if (Menu_Cursor_Y[0] != 1) {
        return;
    }

    if (Training->contents[0][0][0] != 3) {
        return;
    }

    if (Menu_Cursor_Y[0] >= Menu_Cursor_Y[1]) {
        Menu_Cursor_Y[0]++;
        Check_Skip_Replay(2);
        return;
    }

    Menu_Cursor_Y[0]--;
}

void Yes_No_Cursor_Exit_Training(struct _TASK* task_ptr, s16 cursor_id) {
    u16 sw = ~(plsw_01[Decide_ID]) & plsw_00[Decide_ID];

    switch (sw) {
    case 0x4:
        Menu_Cursor_Y[0]--;

        if (Menu_Cursor_Y[0] < 0) {
            Menu_Cursor_Y[0] = 0;
            break;
        }

        SE_dir_cursor_move();
        break;

    case 0x8:
        Menu_Cursor_Y[0]++;

        if (Menu_Cursor_Y[0] > 1) {
            Menu_Cursor_Y[0] = 1;
            break;
        }

        SE_dir_cursor_move();
        break;

    case 0x200:
    case 0x100:
        SE_selected();

        if (Menu_Cursor_Y[0] || sw == 0x200) {
            task_ptr->r_no[2] = 0;
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            Cursor_Y_Pos[0][0] = cursor_id;
            break;
        }

        Soft_Reset_Sub();
        break;
    }
}

void Button_Config_Tr(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 0:
        task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Cursor_Y[1] = 0;
        Menu_Suicide[0] = 1;
        Training_Index = 5;
        Copy_Key_Disp_Work();
        Setup_Button_Sub(6, 5, 1);
        pp_operator_check_flag(0);
        break;

    case 1:
        Button_Config_Sub(0);
        Button_Exit_Check_in_Tr(task_ptr, 0);
        Button_Config_Sub(1);
        Button_Exit_Check_in_Tr(task_ptr, 1);
        Save_Game_Data();
        break;
    }
}

void Button_Exit_Check_in_Tr(struct _TASK* task_ptr, s16 PL_id) {
    if (IO_Result & 0x200) {
        goto ten;
    }

    if (!(IO_Result & 0x100)) {
        return;
    }

    if (Menu_Cursor_Y[PL_id] == 10) {
    ten:
        SE_selected();
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;

        if (Mode_Type == MODE_NORMAL_TRAINING) {
            task_ptr->r_no[1] = 1;
        } else {
            task_ptr->r_no[1] = 2;
        }

        pp_operator_check_flag(1);
        return;
    }

    if (Menu_Cursor_Y[PL_id] == 9) {
        SE_selected();
        Setup_IO_ConvDataDefault(PL_id);
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

void Training_Option(struct _TASK* task_ptr) {
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
        Training_Index = 3;

        for (ix = 0, s6 = y = 72; ix < 6; ix++, s5 = y += 16) {
            effect_A3_init(0, 6, ix, ix, 1, 48, y, 1);
        }

        for (ix = 0, y = 72, s4 = group = 7; ix < 4; ix++, group++, s3 = y += 16) {
            effect_A3_init(0, group, ix, ix, 1, 230, y, 1);
        }

        break;

    case 1:
        Dummy_Move_Sub(task_ptr, Champion, 0, 1, 5);

        if (Menu_Cursor_Y[0] == 4 && IO_Result & 0x100) {
            Default_Training_Option();
            SE_selected();
            break;
        }

        save_w[Present_Mode].Damage_Level = Training[2].contents[0][1][2];
        save_w[Present_Mode].Difficulty = Training[2].contents[0][1][3];
        break;

    case 2:
        SE_selected();
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        Training_Disp_Sub(task_ptr);
        Training[0] = Training[2];
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

const u8 Menu_Max_Data_Tr[2][2][6] = { { { 4, 6, 2, 1, 0, 0 }, { 3, 2, 3, 7, 0, 0 } },
                                       { { 2, 3, 1, 3, 0, 0 }, { 0, 0, 0, 0, 0, 0 } } };

static bool is_data_plus_hitboxes_option_selected() {
    return Training[0].contents[0][1][1] == 2;
}

static void apply_training_hitbox_display(bool force_off) {
    if (force_off || Mode_Type != MODE_NORMAL_TRAINING || !is_data_plus_hitboxes_option_selected()) {
        Set_Training_Hitbox_Display(false);
    } else {
        Set_Training_Hitbox_Display(true);
    }
}

void Dummy_Move_Sub_LR(u16 sw, s16 id, s16 type, s16 cursor_id) {
    s16 max = Menu_Max_Data_Tr[id][type][Menu_Cursor_Y[cursor_id]];

    if (max == 0) {
        return;
    }

    switch (sw) {
    case 4:
        Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]]--;

        if (Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] < 0) {
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = max;
        }

        if (Interface_Type[Champion ^ 1] == 0 && id == 0 && type == 0 && Menu_Cursor_Y[cursor_id] == 0 &&
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] == 4) {
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = 3;
        }

        SE_dir_cursor_move();
        break;

    case 8:
        Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]]++;

        if (Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] > max) {
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = 0;
        }

        if (Interface_Type[Champion ^ 1] == 0 && id == 0 && type == 0 && Menu_Cursor_Y[cursor_id] == 0 &&
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] == 4) {
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = 0;
        }

        SE_dir_cursor_move();
        break;

    default:
        if (Interface_Type[Champion ^ 1] == 0 && id == 0 && type == 0 && Menu_Cursor_Y[cursor_id] == 0 &&
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] == 4) {
            Training[2].contents[id][type][Menu_Cursor_Y[cursor_id]] = 0;
        }

        break;
    }
}

void Blocking_Training(struct _TASK* task_ptr) {
    s16 ix;
    s16 x;
    s16 y;
    s16 s2;

    Menu_Cursor_Y[1] = Menu_Cursor_Y[0];

    switch (task_ptr->r_no[2]) {
    case 0:
        Training_Init_Sub(task_ptr);
        Training_Index = 1;
        x = 112;
        y = 72;
        plw[0].wu.operator = 1;
        Operator_Status[0] = 1;
        plw[1].wu.operator = 1;
        Operator_Status[1] = 1;

        for (ix = 0; ix < 6; ix++, s2 = y += 16) {
            (void)s2;

            effect_A3_init(1, 11, ix, ix, 0, x, y, 0);
        }

        break;

    case 1:
        if (Appear_end < 2) {
            break;
        }

        if (Exec_Wipe) {
            break;
        }

        MC_Move_Sub(Check_Menu_Lever(Decide_ID, 0), 0, 5, 0xFF);
        Check_Skip_Replay(1);

        switch (IO_Result) {
        case 0x100:
            switch (Menu_Cursor_Y[0]) {
            case 0:
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

                /* fallthrough */

            case 1:
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
            break;
        }

        break;

    case 2:
        Yes_No_Cursor_Exit_Training(task_ptr, 5);
        break;

    default:
        Exit_Sub(task_ptr, 0, Menu_Cursor_Y[0] + 1);
        break;
    }
}

const LetterData training_letter_data[6] = { { 0x82, "NORMAL TRAINING" },   { 0x73, "PARRYING TRAINING" },
                                             { 0x7C, "DUMMY SETTING" },     { 0x87, "TRAINING OPTION" },
                                             { 0x7D, "RECORDING SETTING" }, { 0x8F, "BUTTON CONFIG." } };

void Blocking_Tr_Option(struct _TASK* task_ptr) {
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
        Training_Index = 3;
        effect_A3_init(1, 21, 99, 0, 1, 51, 56, 1);
        effect_A3_init(1, 21, 99, 1, 1, 51, 106, 1);

        for (ix = 0, s6 = y = 72; ix < 6; ix++, s5 = y += 16) {
            if (ix == 2) {
                y += 20;
            }

            if (ix == 4) {
                y += 8;
            }

            effect_A3_init(1, 16, ix, ix, 1, 64, y, 0);
        }

        for (ix = 0, y = 72, s4 = group = 17; ix < 4; ix++, group++, s3 = y += 16) {
            if (ix == 2) {
                y += 20;
            }

            effect_A3_init(1, group, ix, ix, 1, 264, y, 0);
        }

        break;

    case 1:
        Dummy_Move_Sub(task_ptr, Champion, 1, 0, 5);

        if (Menu_Cursor_Y[0] == 4 && IO_Result & 0x100) {
            Default_Training_Data(1);
            SE_selected();
        }

        break;

    case 2:
        SE_selected();
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        Training[0] = Training[2];

        plw[New_Challenger].wu.operator = 1;
        Operator_Status[New_Challenger] = 1;

        switch (Training[0].contents[1][0][0]) {
        case 0:
            control_pl_rno = 0;
            control_player = Champion;
            break;
        case 1:
            control_pl_rno = 1;
            control_player = Champion;
            break;
        case 2:
            control_pl_rno = 2;
            control_player = Champion;
            break;
        }

        Training_Disp_Sub(task_ptr);
        break;
    }
}

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

void Character_Change(struct _TASK* task_ptr) {
    s16 ix;

    if (Check_Pad_in_Pause(task_ptr) == 0) {
        switch (task_ptr->r_no[2]) {
        case 0:
            task_ptr->r_no[2]++;
            task_ptr->timer = 0xA;
            Game_pause = 0x81;
            break;

        case 1:
            if ((task_ptr->timer -= 1) == 0) {
                if ((Check_LDREQ_Break() == 0)) {
                    task_ptr->r_no[2]++;
                    Switch_Screen_Init(0);
                    return;
                }

                task_ptr->timer = 1;
                return;
            }
            break;

        case 2:
            if (Switch_Screen(0) != 0) {
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
            break;
        }
    }
}

void Default_Training_Data(s32 flag) {
    s16 ix;
    s16 ix2;
    s16 ix3;

    if (flag == 0) {
        if (!mpp_w.initTrainingData) {
            return;
        }

        mpp_w.initTrainingData = false;
    }

    for (ix = 0; ix < 2; ix++) {
        for (ix2 = 0; ix2 < 2; ix2++) {
            for (ix3 = 0; ix3 < 4; ix3++) {
                Training[0].contents[ix][ix2][ix3] = 0;
            }
        }
    }

    Training[0].contents[0][1][2] = save_w->Damage_Level;
    Training[0].contents[0][1][3] = save_w->Difficulty;
    save_w[Present_Mode].Damage_Level = save_w->Damage_Level;
    save_w[Present_Mode].Difficulty = save_w->Difficulty;
    Training[2] = Training[0];
    Disp_Attack_Data = 0;
}

void Default_Training_Option() {
    Training->contents[0][1][0] = 0;
    Training->contents[0][1][1] = 0;
    Training->contents[0][1][2] = save_w->Damage_Level;
    Training->contents[0][1][3] = save_w->Difficulty;
    save_w[Present_Mode].Damage_Level = save_w->Damage_Level;
    save_w[Present_Mode].Difficulty = save_w->Difficulty;
    Training[2] = Training[0];
    Disp_Attack_Data = 0;
}

void Wait_Replay_Load(struct _TASK* task_ptr) {}

void After_Replay(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_ix;

    s16 s5;
    s16 s4;
    s16 s3;
    s16 s2;

    switch (task_ptr->r_no[1]) {
    case 0:
        task_ptr->r_no[1]++;
        ToneDown(192, 32);
        Menu_Common_Init();
        Menu_Suicide[0] = 0;
        Menu_Cursor_Y[0] = 0;

        for (ix = 0, s5 = char_ix = '8'; ix < 3; ix++, s4 = char_ix++) {
            effect_61_init(0, ix + 80, 0, 0, char_ix, ix, 0x7047);
            Order[ix + 80] = 3;
            Order_Timer[ix + 80] = 1;
        }

        effect_66_init(138, 38, 0, 0, -1, -1, -0x7FF7);
        Order[138] = 3;
        Order_Timer[138] = 1;
        break;

    case 1:
        ToneDown(192, 32);
        Pause_ID = 0;

        if (MC_Move_Sub(Check_Menu_Lever(0, 0), 0, 2, 0xFF) == 0) {
            Pause_ID = 1;
            MC_Move_Sub(Check_Menu_Lever(1, 0), 0, 2, 0xFF);
        }

        switch (IO_Result) {
        case 0x100:
            SE_selected();
            task_ptr->r_no[1] = Menu_Cursor_Y[0] + 2;
            break;

        case 0x200:
            SE_selected();
            task_ptr->r_no[1] = 4;
            break;
        }

        break;

    case 4:
        ToneDown(192, 32);
        Back_to_Mode_Select(task_ptr);
        break;

    case 2:
        ToneDown(192, 32);
        task_ptr->r_no[1] = 12;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;

    case 12:
        Load_Replay_Sub(task_ptr);
        break;

    case 3:
        task_ptr->free[0] = 0;
        task_ptr->r_no[1] = 5;
        task_ptr->r_no[2] = 0;

    case 5:
        ToneDown(192, 32);

        if (Exit_Sub(task_ptr, 0, 6)) {
            Menu_Suicide[0] = 1;
            Menu_Suicide[1] = Menu_Suicide[2] = Menu_Suicide[3] = 0;
        }

        break;

    case 6:
        ToneDown(232, 32);
        switch (task_ptr->r_no[2]) {
        case 0:
            FadeOut(1, 0xFF, 8);
            task_ptr->r_no[2]++;
            task_ptr->timer = 5;
            Menu_Suicide[0] = 0;
            Menu_Common_Init();
            Menu_Cursor_X[0] = 0;
            Setup_BG(1, 512, 0);
            effect_57_init(110, 9, 0, 63, 999);
            Order[110] = 3;
            Order_Dir[110] = 8;
            Order_Timer[110] = 1;
            Setup_File_Property(1, 0xFF);
            SaveInit(2, 1);
            effect_66_init(138, 41, 0, 0, -1, -1, -0x7FF3);
            Order[138] = 3;
            Order_Timer[138] = 1;
            break;

        case 1:
            Menu_Sub_case1(task_ptr);
            break;

        case 2:
            Setup_Save_Replay_2nd(task_ptr, 1);
            break;

        case 3:
            if (SaveMove() > 0) {
                break;
            }

            task_ptr->r_no[2]++;
            /* fallthrough */

        case 4:
            Exit_Sub(task_ptr, 0, 7);
            break;
        }

        break;

    case 7:
        FadeOut(1, 0xFF, 8);
        Order[110] = 4;
        Order_Timer[110] = 1;
        Menu_Suicide[0] = 1;
        task_ptr->r_no[1]++;
        break;

    case 8:
        FadeOut(1, 0xFF, 8);
        Menu_Suicide[0] = 0;

        for (ix = 0, s3 = char_ix = '8'; ix < 3; ix++, s2 = char_ix++) {
            effect_61_init(0, ix + 80, 0, 0, char_ix, ix, 0x7047);
            Order[ix + 80] = 3;
            Order_Timer[ix + 80] = 1;
        }

        effect_66_init(138, 38, 0, 0, -1, -1, -0x7FF7);
        Order[138] = 3;
        Order_Timer[138] = 1;
        task_ptr->r_no[1]++;
        FadeInit();

    case 9:
        ToneDown(192, 32);

        if (FadeIn(1, 25, 8)) {
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[1] = 1;
        }
    }
}

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

        if (--task_ptr->timer == 0) {
            task_ptr->r_no[2]++;
            task_ptr->r_no[3] = 1;
            FadeInit();
        }

        break;

    case 3:
        if (FadeIn(1, 25, 8)) {
            task_ptr->r_no[2]++;
            break;
        }

        break;

    case 4:
        Pause_ID = 0;
        Dir_Move_Sub(task_ptr, 0);

        if (IO_Result == 0) {
            Pause_ID = 1;
            Dir_Move_Sub(task_ptr, 1);
        }

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
            task_ptr->r_no[2] = 1;
            task_ptr->timer = 5;

            if (--Menu_Page < 0) {
                Menu_Page = Page_Max;
            }

            SE_dir_selected();
            break;

        case 0x40:
        case 0x400:
            task_ptr->r_no[2] = 1;
            task_ptr->timer = 5;

            if (++Menu_Page > Page_Max) {
                Menu_Page = 0;
            }

            SE_dir_selected();
            break;

        case 0x100:
            if (Menu_Page == 0 && Menu_Cursor_Y[0] == 6) {
                save_w[Present_Mode].extra_option = save_w[0].extra_option;
                SE_selected();
                break;
            }

            if (Menu_Cursor_Y[0] != Menu_Max) {
                break;
            }

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

            break;
        }

        break;
    }
}

void Ex_Move_Sub_LR(u16 sw, s16 PL_id) {
    u8 last_pos = save_w[Present_Mode].extra_option.contents[Menu_Page][Menu_Cursor_Y[0]];

    switch (sw) {
    case 4:
        if (Menu_Page_Buff != 0 || Menu_Cursor_Y[0] != 4) {
            SE_dir_cursor_move();
        }

        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]]--;

        if (Menu_Cursor_Y[0] == Menu_Max) {
            if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] < 0) {
                save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 0;
                IO_Result = 0x80;
                break;
            }

            if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] != last_pos) {
                Message_Data->order = 1;
                Message_Data->request = save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Max] + 32;
                Message_Data->timer = 2;
            }
        } else if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] < 0) {
            save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] =
                Ex_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]];
        }

        return;

    case 8:
        if (Menu_Page_Buff != 0 || Menu_Cursor_Y[0] != 4) {
            SE_dir_cursor_move();
        }

        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]]++;

        if (Menu_Cursor_Y[0] == Menu_Max) {
            if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] > 2) {
                save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 2;
                IO_Result = 0x400;
                return;
            }

            if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] > 2) {
                save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 2;
            }

            if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] != last_pos) {
                Message_Data->order = 1;
                Message_Data->request = save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Max] + 32;
                Message_Data->timer = 2;
            }
        } else if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] >
                   Ex_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
            save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 0;
        }

        return;

    case 0x400:
        if (Interface_Type[PL_id] == 2) {
            break;
        }

    case 0x100:
        if (Menu_Page_Buff != 0 || Menu_Cursor_Y[0] != 4) {
            SE_dir_cursor_move();
        }

        if (Menu_Cursor_Y[0] == Menu_Max) {
            break;
        }

        save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]]++;

        if (save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] >
            Ex_Menu_Max_Data[Menu_Page][Menu_Cursor_Y[0]]) {
            save_w[1].extra_option.contents[Menu_Page_Buff][Menu_Cursor_Y[0]] = 0;
        }

        return;
    }
}

void End_Replay_Menu(struct _TASK* task_ptr) {
    s16 ix;
    s16 ans;

    switch (task_ptr->r_no[1]) {
    case 0:
        if (Allow_a_battle_f == 0) {
            break;
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
        /* fallthrough */

    case 1:
        task_ptr->r_no[1] += 1;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;

        for (ix = 0; ix < 4; ix++) {
            Menu_Suicide[ix] = 0;
        }

        effect_10_init(0, 0, 0, 4, 0, 0x14, 0xE);
        effect_10_init(0, 6, 1, 2, 0, 0x16, 0x10);
        break;

    case 2:
        MC_Move_Sub(Check_Menu_Lever(Pause_ID, 0), 0, 1, 0xFF);

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

        break;

    case 3:
        ans = Yes_No_Cursor_Move_Sub(task_ptr);

        switch (ans) {
        case 1:
            task_ptr->r_no[1] = 1;
            break;

        case -1:
            Menu_Suicide[3] = 1;
            break;
        }

        break;
    }
}
