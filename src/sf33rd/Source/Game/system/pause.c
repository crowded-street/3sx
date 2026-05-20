/**
 * @file pause.c
 * Game Pause
 */

#include "sf33rd/Source/Game/system/pause.h"
#include "common.h"
#include "main.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/reset.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#define HOLD_TO_PAUSE_TIMER_MAX 60

typedef enum PauseActivationType {
    PAUSE_ACTIVATION_NONE,
    PAUSE_ACTIVATION_PRESS,
    PAUSE_ACTIVATION_HOLD,
} PauseActivationType;

u8 PAUSE_X;

static u8 hold_to_pause_timers[2] = { 0 };

void Pause_Task(struct _TASK* task_ptr);

void Pause_Check(struct _TASK* task_ptr);
void Pause_Move(struct _TASK* task_ptr);
void Pause_Sleep(struct _TASK* /* unused */);
void Pause_Die(struct _TASK* /* unused */);

void Flash_Pause(struct _TASK* task_ptr);

void Flash_Pause_Sleep(struct _TASK* /* unused */);
void Flash_Pause_1st(struct _TASK* task_ptr);
void Flash_Pause_2nd(struct _TASK* task_ptr);
void Flash_Pause_3rd(struct _TASK* /* unused */);
void Flash_Pause_4th(struct _TASK* task_ptr);

s32 Check_Pause_Term(u8 PL_id, bool ignore_input);
void Exit_Pause(struct _TASK* task_ptr);
void Setup_Pause(struct _TASK* task_ptr);
void Setup_Come_Out(struct _TASK* task_ptr);
s32 Check_Play_Status(s16 PL_id);

static PauseActivationType get_pause_activation_type() {
    switch (Mode_Type) {
    case MODE_VERSUS:
        return PAUSE_ACTIVATION_HOLD;

    case MODE_ARCADE:
        if (Round_Operator[0] != 0 && Round_Operator[1] != 0) {
            return PAUSE_ACTIVATION_HOLD;
        } else {
            return PAUSE_ACTIVATION_PRESS;
        }

    case MODE_NORMAL_TRAINING:
    case MODE_PARRY_TRAINING:
    case MODE_REPLAY:
        return PAUSE_ACTIVATION_PRESS;

    case MODE_NETWORK:
        return PAUSE_ACTIVATION_NONE;

    default:
        return PAUSE_ACTIVATION_NONE;
    }
}

static s32 handle_start_button(u8 PL_id, u16 current_sw, u16 edge_sw) {
    switch (get_pause_activation_type()) {
    case PAUSE_ACTIVATION_NONE:
        // Do nothing
        break;

    case PAUSE_ACTIVATION_PRESS:
        if (edge_sw & SWK_START) {
            Pause_Type = 1;
            return PAUSE_X = 1;
        }

        break;

    case PAUSE_ACTIVATION_HOLD:
        if ((edge_sw & SWK_START) && (hold_to_pause_timers[PL_id] == 0)) {
            hold_to_pause_timers[PL_id] = HOLD_TO_PAUSE_TIMER_MAX;
        } else if ((current_sw & SWK_START) && (hold_to_pause_timers[PL_id] > 0)) {
            hold_to_pause_timers[PL_id] -= 1;

            if (hold_to_pause_timers[PL_id] == 0) {
                hold_to_pause_timers[PL_id ^ 1] = 0;
                Pause_Type = 1;
                return PAUSE_X = 1;
            }
        } else {
            hold_to_pause_timers[PL_id] = 0;
        }

        break;
    }

    return 0;
}

void Pause_Task(struct _TASK* task_ptr) {
    void (*Main_Jmp_Tbl[4])(struct _TASK*) = { Pause_Check, Pause_Move, Pause_Sleep, Pause_Die };

    if (!nowSoftReset() && Mode_Type != MODE_NETWORK && Mode_Type != MODE_NORMAL_TRAINING &&
        Mode_Type != MODE_PARRY_TRAINING) {
        Main_Jmp_Tbl[task_ptr->r_no[0]](task_ptr);
        Flash_Pause(task_ptr);
    }
}

void Pause_Check(struct _TASK* task_ptr) {
    PAUSE_X = 0;

    if (Check_Pause_Term(0, false) == 0) {
        Check_Pause_Term(1, false);
    }

    switch (PAUSE_X) {
    case 1:
        Setup_Pause(task_ptr);
        break;

    case 2:
        Setup_Come_Out(task_ptr);
        break;
    }
}

void Pause_Move(struct _TASK* task_ptr) {
    if (Exit_Menu) {
        Exit_Pause(task_ptr);
    }
}

void Pause_Sleep(struct _TASK* /* unused */) {
    // Do nothing
}

void Pause_Die(struct _TASK* /* unused */) {
    // Do nothing
}

void Flash_Pause(struct _TASK* task_ptr) {
    void (*Flash_Jmp_Tbl[5])(struct _TASK*) = {
        Flash_Pause_Sleep, Flash_Pause_1st, Flash_Pause_2nd, Flash_Pause_3rd, Flash_Pause_4th
    };

    if (Pause_Down != 0) {
        Flash_Jmp_Tbl[task_ptr->r_no[2]](task_ptr);
    }
}

void Flash_Pause_Sleep(struct _TASK* /* unused */) {
    // Do nothing
}

void Flash_Pause_1st(struct _TASK* task_ptr) {
    if (--task_ptr->free[0] == 0) {
        task_ptr->r_no[2] = 2;
        task_ptr->free[0] = 60;
    }
}

void Flash_Pause_2nd(struct _TASK* task_ptr) {
    if (--task_ptr->free[0]) {
        if (Pause_ID == 0) {
            SSPutStr2(20, 9, 9, "1P PAUSE");
        } else {
            SSPutStr2(20, 9, 9, "2P PAUSE");
        }
    } else {
        task_ptr->r_no[2] = 1;
        task_ptr->free[0] = 30;
    }
}

void Flash_Pause_3rd(struct _TASK* /* unused */) {
    // Do nothing
}

void Flash_Pause_4th(struct _TASK* task_ptr) {
    if (Interface_Type[Pause_ID] == 0) {
        dispControllerWasRemovedMessage(132, 82, 16);
        return;
    }

    Pause_Type = 1;
    Setup_Pause(task_ptr);
}

void dispControllerWasRemovedMessage(s32 x, s32 y, s32 step) {
    SSPutStrPro(0, x, y, 9, -1, "Please reconnect");
    SSPutStrPro(0, x, y + step, 9, -1, "the controller to");

    if (Pause_ID) {
        SSPutStrPro(0, x, y + step * 2, 9, -1, "controller port 2.");
    } else {
        SSPutStrPro(0, x, y + step * 2, 9, -1, "controller port 1.");
    }
}

s32 Check_Pause_Term(u8 PL_id, bool ignore_input) {
    if (Demo_Flag == 0) {
        return 0;
    }

    if (Allow_a_battle_f == 0 || Extra_Break != 0) {
        return 0;
    }

    if (vm_w.Access != 0 || vm_w.Request != 0) {
        return PAUSE_X = 0;
    }

    if (Exec_Wipe) {
        return 0;
    }

    Pause_ID = PL_id;

    if (Check_Play_Status(PL_id) == 0) {
        return 0;
    }

    u16 current_sw = PLsw[PL_id][0];
    u16 edge_sw = ~PLsw[PL_id][1] & PLsw[PL_id][0];

    if (ignore_input) {
        current_sw = 0;
        edge_sw = 0;
    }

    const s32 return_code = handle_start_button(PL_id, current_sw, edge_sw);

    if (return_code != 0) {
        return return_code;
    }

    if (Present_Mode == 3) {
        if (Interface_Type[Decide_ID] == 0) {
            Pause_ID = Decide_ID;
            Pause_Type = 2;
            return PAUSE_X = 2;
        }
    } else if (Interface_Type[PL_id] == 0 && plw[PL_id].wu.operator) {
        Pause_Type = 2;
        return PAUSE_X = 2;
    }

    return 0;
}

void Exit_Pause(struct _TASK* task_ptr) {
    u8 ix;

    if (Present_Mode != 3 && Check_Pause_Term(Pause_ID ^ 1, true)) {
        Exit_Menu = 0;
        return;
    }

    SE_selected();
    Game_pause = 0;
    Pause = 0;
    Pause_Down = 0;

    for (ix = 0; ix < 4; ix++) {
        task_ptr->r_no[ix] = 0;
        task_ptr->free[ix] = 0;
    }

    Menu_Suicide[0] = 1;
    Menu_Suicide[1] = 1;
    Menu_Suicide[2] = 1;
    Menu_Suicide[3] = 1;
    pulpul_request_again();
    cpExitTask(TASK_SAVER);
    cpExitTask(TASK_MENU);
    SsBgmHalfVolume(0);
}

void Setup_Pause(struct _TASK* task_ptr) {
    s16 ix;

    SE_selected();
    Pause_Down = 1;
    Game_pause = 0x81;
    task_ptr->r_no[0] = 1;
    task_ptr->r_no[2] = 1;
    task_ptr->free[0] = 1;
    cpReadyTask(TASK_MENU, Menu_Task);
    task[TASK_MENU].r_no[0] = 1;
    Exit_Menu = 0;

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 0;
    }

    Order[0x8A] = 3;
    Order_Timer[0x8A] = 1;
    effect_66_init(0x8A, 9, 2, 7, -1, -1, -0x3FFC);
    SsBgmHalfVolume(1);
    spu_all_off();
}

void Setup_Come_Out(struct _TASK* task_ptr) {
    s16 ix;

    SE_selected();
    Pause_Down = 1;
    Game_pause = 0x81;
    task_ptr->r_no[0] = 1;
    task_ptr->r_no[2] = 4;
    task_ptr->free[0] = 1;
    cpReadyTask(TASK_MENU, Menu_Task);
    task[TASK_MENU].r_no[0] = 1;
    Exit_Menu = 0;

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 0;
    }

    Order[0x8A] = 3;
    Order_Timer[0x8A] = 1;
    effect_66_init(0x8A, 9, 2, 7, -1, -1, -0x3FFC);
    SsBgmHalfVolume(1);
    spu_all_off();
}

s32 Check_Play_Status(s16 PL_id) {
    if (Mode_Type != MODE_VERSUS) {
        return Round_Operator[PL_id];
    } else {
        return 1;
    }
}
