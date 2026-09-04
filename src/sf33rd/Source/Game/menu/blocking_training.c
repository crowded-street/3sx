#include "sf33rd/Source/Game/menu/menu_internal.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effa3.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "structs.h"

void handle_blocking_training_selection(struct _TASK* task_ptr);

static void initialize_blocking_training(struct _TASK* task_ptr) {
    s16 ix;
    s16 x;
    s16 y;
    s16 s2;

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
}

static void handle_blocking_training_input(struct _TASK* task_ptr) {
    if (Appear_end < 2) {
        return;
    }
    if (Exec_Wipe) {
        return;
    }

    MC_Move_Sub(Check_Menu_Lever(Decide_ID, 0), 0, 5, 0xFF);
    Check_Skip_Replay(1);

    switch (IO_Result) {
    case 0x100:
        handle_blocking_training_selection(task_ptr);
        break;
    }
}

void Blocking_Training(struct _TASK* task_ptr) {

    Menu_Cursor_Y[1] = Menu_Cursor_Y[0];

    switch (task_ptr->r_no[2]) {
    case 0:
        initialize_blocking_training(task_ptr);
        break;

    case 1:
        handle_blocking_training_input(task_ptr);
        break;

    case 2:
        Yes_No_Cursor_Exit_Training(task_ptr, 5);
        break;

    default:
        Exit_Sub(task_ptr, 0, Menu_Cursor_Y[0] + 1);
        break;
    }
}
