#include "sf33rd/Source/Game/menu/menu_internal.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/effect/eff04.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/vm_sub.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/saver.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "structs.h"

static bool should_exit_menu_selection(void) {
    return Menu_Cursor_Y[0] == 2 || IO_Result == 0x200;
}

static void initialize_training_mode(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_index;

    Menu_in_Sub(task_ptr);
        mpp_w.initTrainingData = true;
        effect_57_init(0x6F, MENU_HEADER_TRAINING, 0, 0x3F, 2);
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
}

static void select_training_mode(struct _TASK* task_ptr) {
    s16 player_id = 0;

    if (MC_Move_Sub(Check_Menu_Lever(0, 0), 0, 2, 0xFF) == 0) {
        player_id = 1;
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

    if (should_exit_menu_selection()) {
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        task_ptr->free[0] = 0;
        Order[0x6F] = 4;
        Order_Timer[0x6F] = 4;
        return;
    }

    Decide_ID = player_id;

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
    Champion = player_id;
    Pause_ID = player_id;
    Training_ID = player_id;
    New_Challenger = player_id ^ 1;
    cpExitTask(TASK_ENTRY);
}

void Training_Mode(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 0:
        initialize_training_mode(task_ptr);
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
        select_training_mode(task_ptr);
        break;
    }
}
