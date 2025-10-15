#include "sf33rd/Source/Game/Pause.h"
#include "common.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/Reset.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"

// sbss
u8 PAUSE_X;
u8 Stock_Turbo_Timer;
u8 Stock_Process_Counter;

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

s32 Check_Pause_Term(u16 sw, u8 PL_id);
void Exit_Pause(struct _TASK* task_ptr);
void Setup_Pause(struct _TASK* task_ptr);
void Setup_Come_Out(struct _TASK* task_ptr);
s32 Check_Play_Status(s16 PL_id);

void Pause_Task(struct _TASK* task_ptr) {
    void (*Main_Jmp_Tbl[4])(struct _TASK*) = { Pause_Check, Pause_Move, Pause_Sleep, Pause_Die };

    if (!nowSoftReset() && Mode_Type != MODE_NETWORK && Mode_Type != MODE_NORMAL_TRAINING && Mode_Type != MODE_PARRY_TRAINING) {
        Main_Jmp_Tbl[task_ptr->r_no[0]](task_ptr);
        Flash_Pause(task_ptr);
    }
}

void Pause_Check(struct _TASK* task_ptr) {
    PAUSE_X = 0;

    if (Check_Pause_Term(~(PLsw[0][1]) & (PLsw[0][0]), 0) == 0) {
        Check_Pause_Term(~(PLsw[1][1]) & (PLsw[1][0]), 1);
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

void Pause_Sleep(struct _TASK* /* unused */) {};

void Pause_Die(struct _TASK* /* unused */) {};

void Flash_Pause(struct _TASK* task_ptr) {
    void (*Flash_Jmp_Tbl[5])(
        struct _TASK*) = { Flash_Pause_Sleep, Flash_Pause_1st, Flash_Pause_2nd, Flash_Pause_3rd, Flash_Pause_4th };

    if (Pause_Down != 0) {
        Flash_Jmp_Tbl[task_ptr->r_no[2]](task_ptr);
    }
}

void Flash_Pause_Sleep(struct _TASK* /* unused */) {}

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
            return;
        }

        SSPutStr2(20, 9, 9, "2P PAUSE");
        return;
    }

    task_ptr->r_no[2] = 1;
    task_ptr->free[0] = 30;
}

void Flash_Pause_3rd(struct _TASK* /* unused */) {}

void Flash_Pause_4th(struct _TASK* task_ptr) {
    if (Interface_Type[Pause_ID] == 0) {
        dispControllerWasRemovedMessage(0x84, 0x52, 0x10);
        return;
    }

    Pause_Type = 1;
    Setup_Pause(task_ptr);
}

void dispControllerWasRemovedMessage(s32 x, s32 y, s32 step) {
    SSPutStrPro(0, x, y, 9, -1, "Please reconnect");
    SSPutStrPro(0, x, (y + step), 9, -1, "the controller to");

    if (Pause_ID) {
        SSPutStrPro(0, x, (y + (step * 2)), 9, -1, "controller port 2.");
        return;
    }

    SSPutStrPro(0, x, (y + (step * 2)), 9, -1, "controller port 1.");
}

s32 Check_Pause_Term(u16 sw, u8 PL_id) {
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

    if (sw & 0x4000) {
        Pause_Type = 1;
        return PAUSE_X = 1;
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

    if (Present_Mode != 3 && Check_Pause_Term(0, (Pause_ID ^ 1))) {
        Exit_Menu = 0;
        return;
    }

    SE_selected();
    Game_pause = 0;
    Pause = 0;
    Pause_Down = 0;
    Turbo_Timer = Stock_Turbo_Timer;
    Process_Counter = Stock_Process_Counter;

    for (ix = 0; ix < 4; ix++) {
        task_ptr->r_no[ix] = 0;
        task_ptr->free[ix] = 0;
    }

    Menu_Suicide[0] = 1;
    Menu_Suicide[1] = 1;
    Menu_Suicide[2] = 1;
    Menu_Suicide[3] = 1;
    pulpul_request_again();
    cpExitTask(6);
    cpExitTask(3);
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
    Stock_Turbo_Timer = Turbo_Timer;
    Stock_Process_Counter = Process_Counter;
    cpReadyTask(3U, Menu_Task);
    task[3].r_no[0] = 1;
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
    Stock_Turbo_Timer = Turbo_Timer;
    Stock_Process_Counter = Process_Counter;
    cpReadyTask(3, Menu_Task);
    task[3].r_no[0] = 1;
    Exit_Menu = 0;

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[(ix)] = 0;
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
    }

    return 1;
}
