#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effa3.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "structs.h"

void start_normal_training_mode(struct _TASK* task_ptr);
void Training_Init_Sub(struct _TASK* task_ptr);
void Check_Skip_Recording();
void Check_Skip_Replay(s16 ix);
void Training_Exit_Sub(struct _TASK* task_ptr);
void Yes_No_Cursor_Exit_Training(struct _TASK* task_ptr, s16 cursor_id);
void Exit_Sub(struct _TASK* task_ptr, s16 cursor_ix, s16 next_routine);
u16 MC_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max, s16 cansel_menu);

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
                start_normal_training_mode(task_ptr);
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
