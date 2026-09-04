#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/effect/effa3.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

void Dummy_Move_Sub(struct _TASK* task_ptr, s16 PL_id, s16 id, s16 type, s16 max);
void Default_Training_Data(s32 flag);
void Training_Disp_Sub(struct _TASK* task_ptr);
extern u8 control_player;
extern u8 control_pl_rno;

static void initialize_blocking_training_left_column(void) {
    s16 ix;
    s16 y;
    s16 s6;
    s16 s5;

    for (ix = 0, s6 = y = 72; ix < 6; ix++, s5 = y += 16) {
        if (ix == 2) {
            y += 20;
        }
        if (ix == 4) {
            y += 8;
        }
        effect_A3_init(1, 16, ix, ix, 1, 64, y, 0);
    }
}

static void initialize_blocking_training_right_column(void) {
    s16 ix;
    s16 group;
    s16 y;
    s16 s4;
    s16 s3;

    for (ix = 0, y = 72, s4 = group = 17; ix < 4; ix++, group++, s3 = y += 16) {
        if (ix == 2) {
            y += 20;
        }
        effect_A3_init(1, group, ix, ix, 1, 264, y, 0);
    }
}

static void initialize_blocking_training_option(struct _TASK* task_ptr) {

    task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Cursor_Y[1] = 0;
        Menu_Suicide[0] = 1;
        Training_Index = 3;
        effect_A3_init(1, 21, 99, 0, 1, 51, 56, 1);
        effect_A3_init(1, 21, 99, 1, 1, 51, 106, 1);

    initialize_blocking_training_left_column();
    initialize_blocking_training_right_column();
}

static void update_blocking_training_option(struct _TASK* task_ptr) {
    Dummy_Move_Sub(task_ptr, Champion, 1, 0, 5);

    if (Menu_Cursor_Y[0] == 4 && IO_Result & 0x100) {
        Default_Training_Data(1);
        SE_selected();
    }
}

static void finish_blocking_training_option(struct _TASK* task_ptr) {
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
}

void Blocking_Tr_Option(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 0:
        initialize_blocking_training_option(task_ptr);
        break;
    case 1:
        update_blocking_training_option(task_ptr);
        break;
    case 2:
        finish_blocking_training_option(task_ptr);
        break;
    }
}
