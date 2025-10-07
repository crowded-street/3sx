#include "sf33rd/Source/Game/init3rd.h"
#include "sf33rd/Source/Game/DEMO00.h"
#include "sf33rd/Source/Game/DIR_DATA.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/Entry.h"
#include "sf33rd/Source/Game/GD3rd.h"
#include "sf33rd/Source/Game/Game.h"
#include "sf33rd/Source/Game/PulPul.h"
#include "sf33rd/Source/Game/Reset.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/texgroup.h"
#include "sf33rd/Source/Game/workuser.h"
#include "sf33rd/Source/PS2/mc/savesub.h"
#include "sf33rd/Source/PS2/ps2Quad.h"
#include "structs.h"

#if !defined(TARGET_PS2)
#include <string.h>
#endif

f32 Keep_Zoom_X;
s8 Test_Cursor;

const u8 Difficult_V_Data[2][2] = { { 0, 0 }, { 1, 2 } };
const Permission Permission_PL_Data = { { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                                        { { 0, 0 }, { 0, 0 } } };

void Init_Task_1st(struct _TASK* task_ptr);
void Init_Task_Aload(struct _TASK* task_ptr);
void Init_Task_2nd(struct _TASK* task_ptr);
void Init_Task_Test1(struct _TASK* task_ptr);
void Init_Task_Test2(struct _TASK* task_ptr);
void Init_Task_End(struct _TASK* task_ptr);
void Setup_Difficult_V();

void Init_Task(struct _TASK* task_ptr) {
    void (*Main_Jmp_Tbl[6])() = { Init_Task_1st, Init_Task_Aload, Init_Task_2nd,
                                  Init_Task_End, Init_Task_Test1, Init_Task_Test2 };

    Main_Jmp_Tbl[task_ptr->r_no[0]](task_ptr);
}

void Init_Task_1st(struct _TASK* task_ptr) {
    s16 ix;

    task_ptr->r_no[0] = 1;
    init_texcash_1st();
    Init_texgrplds_work();
    Init_load_on_memory_data();
    Pause_Family_On();
    Bg_TexInit();
    Scrscreen_Init();
    effect_work_init();
    Usage = 7;
    Max_vitality = 160;
    reset_NG_flag = 0;
    Break_Into = 0;
    Forbid_Break = 0;
    Extra_Break = 0;
    Demo_Flag = 0;
    Random_ix16 = 0;
    Random_ix32 = 0;
    test_flag = 0;
    ixbfw_cut = 0;
    PB_Music_Off = 0;
    No_Death = 0;
    Pause_Down = 0;
    Disp_Attack_Data = 0;
    seraph_flag = 0;
    Request_Break[0] = 0;
    Request_Break[1] = 0;
    DE_X[0] = 2;
    DE_X[1] = 0x1B;
    No_Trans = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;
    Play_Game = 0;
    Present_Mode = 1;

    for (ix = 0; ix < 4; ix++) {
        G_No[ix] = 0;
        E_No[ix] = 0;
        S_No[ix] = 0;
        Unsubstantial_BG[ix] = 0;
    }

    init_pulpul_work();
    Init_Load_Request_Queue_1st();
    Game_Data_Init();

    for (ix = 0; ix < 6; ix++) {
        system_dir[ix] = Dir_Default_Data;
        permission_player[ix] = Permission_PL_Data;

#if defined(TARGET_PS2)
        save_w[ix].extra_option.contents = save_w[0].extra_option.contents;
#else
        memcpy(&save_w[ix].extra_option.contents,
               &save_w[0].extra_option.contents,
               sizeof(save_w[ix].extra_option.contents));
#endif

        Direction_Working[ix] = 0;
        Vital_Handicap[ix][0] = 7;
        Vital_Handicap[ix][1] = 7;
        permission_player[ix].cursor_infor[0].first_x = 1;
        permission_player[ix].cursor_infor[0].first_y = 0;
        permission_player[ix].cursor_infor[1].first_x = 5;
        permission_player[ix].cursor_infor[1].first_y = 2;
    }

    Copy_Check_w();
    Direction_Working[2] = 1;
    save_w[4].Time_Limit = -1;
    save_w[5].Time_Limit = -1;
    Setup_Difficult_V();
    Setup_Limit_Time();
    Keep_Zoom_X = Screen_Zoom_X;
    Reset_Bootrom = 1;
    cpReadyTask(RESET_TASK_NUM, Reset_Task);
    Switch_Type = 0;
    Reset_Status[0] = 0;
    Reset_Status[1] = 0;
    pulpul_stop();
    Warning_Init();
    CP3toPS2DrawOff();
}

void Setup_Difficult_V() {
    u8 country;

    if (Country == 1) {
        country = 0;
    } else {
        country = 1;
    }

    CC_Value[0] = Difficult_V_Data[country][0];
    CC_Value[1] = Difficult_V_Data[country][1];
}

void Init_Task_Aload(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[1]) {
    case 0:
    case 1:
        task_ptr->r_no[1] = 2;
        SaveInit(0, 2);
        /* fallthrough */

    case 2:
        if (SaveMove() <= 0) {
            task_ptr->r_no[0] += 1;
            task_ptr->r_no[1] = 0;
            mpp_w.cutAnalogStickData = 0;
            Forbid_Reset = 1;
        }

        break;

    case 10:
        break;
    }
}

void Init_Task_2nd(struct _TASK* task_ptr) {
    if (Warning() == 0) {
        return;
    }

    task_ptr->r_no[0] += 1;
    Setup_Disp_Size(0);
    Screen_Zoom_X = Keep_Zoom_X;
    CP3toPS2DrawOn();
}

void Init_Task_Test1(struct _TASK* task_ptr) {
    u16 sw;

    switch (task_ptr->r_no[1]) {
    case 0:
        task_ptr->r_no[1] += 1;
        task_ptr->timer = 1800;
        Test_Cursor = 1;
        /* fallthrough */

    case 1:
        sw = ~p1sw_1 & p1sw_0;

        if (--task_ptr->timer <= 0) {
            sw = 0x100;
            Test_Cursor = 1;
        }

        switch (sw) {
        case 1:
            Test_Cursor -= 1;

            if (Test_Cursor < 0) {
                Test_Cursor = 0;
            } else {
                SE_cursor_move();
            }

            break;

        case 2:
            Test_Cursor += 1;

            if (Test_Cursor > 2) {
                Test_Cursor = 2;
            } else {
                SE_cursor_move();
            }

            break;

        case 0x100:
            if (Test_Cursor == 0) {
                task_ptr->r_no[0] = 4;
                task_ptr->r_no[1] = 0;
            } else {
                task_ptr->r_no[1] += 1;
            }

            SE_selected();
            break;
        }

        break;

    case 2:
        if (Test_Cursor == 2) {
            Turbo = 0;
            Screen_PAL = 0;
            Warning_Init();
        } else {
            Turbo = 5;
            Turbo_Timer = 1;
            Screen_PAL = 8;
            Warning_Init();
        }

        task_ptr->r_no[0] = 1;
        return;
    }

    Pal_Cursor_Put(Test_Cursor);
    Put_Warning(2);
}

void Init_Task_Test2(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[1]) {
    case 0:
        task_ptr->r_no[1] += 1;
        task_ptr->timer = 300;
        Turbo = 0;
        Screen_PAL = 0;
        Warning_Init();
        return;

    case 1:
        if (--task_ptr->timer <= 0) {
            task_ptr->r_no[0] = 3;
            task_ptr->r_no[1] = 0;
            Screen_PAL = 8;
            Warning_Init();
            return;
        }

    default:
        Put_Warning(3);
        return;
    }
}

void Init_Task_End(struct _TASK* task_ptr) {
    cpReadyTask(GAME_TASK_NUM, Game_Task);
    task_ptr->r_no[0] += 1;
    task_ptr->r_no[1] = 0;
    G_No[0] = 1;
    cpReadyTask(ENTRY_TASK_NUM, Entry_Task);

    if (Usage == 7) {
        cpReadyTask(DEBUG_TASK_NUM, Debug_Task);
    }

    cpExitTask(0);
    Forbid_Reset = 0;
}
