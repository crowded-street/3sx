#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/io/vm_sub.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/PS2/mc/savesub.h"
#include "structs.h"

void Back_to_Mode_Select(struct _TASK* task_ptr);
void Load_Replay_Sub(struct _TASK* task_ptr);
void Setup_Save_Replay_2nd(struct _TASK* task_ptr, s16 unused);
void Menu_Sub_case1(struct _TASK* task_ptr);
s32 Exit_Sub(struct _TASK* task_ptr, s16 cursor_ix, s16 next_routine);
u16 MC_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max, s16 cansel_menu);

static void initialize_after_replay(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_ix;
    s16 s5;
    s16 s4;

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
}

static void handle_after_replay_input(struct _TASK* task_ptr) {
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

}

static void prepare_replay_load(struct _TASK* task_ptr) {
    ToneDown(192, 32);
    task_ptr->r_no[1] = 12;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
}

static void prepare_replay_exit(struct _TASK* task_ptr) {
    task_ptr->free[0] = 0;
    task_ptr->r_no[1] = 5;
    task_ptr->r_no[2] = 0;
}

static void exit_after_replay(struct _TASK* task_ptr) {
    ToneDown(192, 32);
    if (Exit_Sub(task_ptr, 0, 6)) {
        Menu_Suicide[0] = 1;
        Menu_Suicide[1] = Menu_Suicide[2] = Menu_Suicide[3] = 0;
    }
}

static void save_replay(struct _TASK* task_ptr) {
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
        SaveInit(SAVE_FILE_REPLAY, SAVE_MODE_SAVE);
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
}

static void initialize_after_replay_return(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_ix;
    s16 s3;
    s16 s2;

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
}

static void finish_after_replay_return(struct _TASK* task_ptr) {
    ToneDown(192, 32);
    if (FadeIn(1, 25, 8)) {
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[1] = 1;
    }
}

static void handle_after_replay_late_state(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[1]) {
    case 5:
        exit_after_replay(task_ptr);
        break;
    case 6:
        save_replay(task_ptr);
        break;
    case 7:
        FadeOut(1, 0xFF, 8);
        Order[110] = 4;
        Order_Timer[110] = 1;
        Menu_Suicide[0] = 1;
        task_ptr->r_no[1]++;
        break;
    case 8:
        initialize_after_replay_return(task_ptr);
        finish_after_replay_return(task_ptr);
        break;
    case 9:
        finish_after_replay_return(task_ptr);
        break;
    }
}

void After_Replay(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[1]) {
    case 0:
        initialize_after_replay(task_ptr);
        break;
    case 1:
        handle_after_replay_input(task_ptr);
        break;
    case 4:
        ToneDown(192, 32);
        Back_to_Mode_Select(task_ptr);
        break;
    case 2:
        prepare_replay_load(task_ptr);
        Load_Replay_Sub(task_ptr);
        break;
    case 12:
        Load_Replay_Sub(task_ptr);
        break;
    case 3:
        prepare_replay_exit(task_ptr);
        exit_after_replay(task_ptr);
        break;
    default:
        handle_after_replay_late_state(task_ptr);
    }
}
