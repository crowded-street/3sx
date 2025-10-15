#include "sf33rd/Source/Game/Reset.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/texgroup.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"

u8 Reset_Status[2];
u8 RESET_X;

void Reset_Init(struct _TASK* task_ptr);
void Reset_Move(struct _TASK* task_ptr);
void Reset_Wait(struct _TASK* task_ptr);
void Reset_Sleep(struct _TASK* task_ptr);
void Check_Reset();
u8 Check_SoftReset(s16 PL_id);
s32 Setup_Next_Disposal();
void Check_Reset_IO(struct _TASK* /* unused */, s16 PL_id);

void Reset_Task(struct _TASK* task_ptr) {
    void (*Main_Jmp_Tbl[4])() = { Reset_Init, Reset_Move, Reset_Wait, Reset_Sleep };
    Check_Reset_IO(task_ptr, 0);
    Check_Reset_IO(task_ptr, 1);
    Main_Jmp_Tbl[task_ptr->r_no[0]](task_ptr);
}

void Reset_Init(struct _TASK* task_ptr) {
    task_ptr->r_no[0] += 1;
    RESET_X = 0;
}

u8 nowSoftReset() {
    return RESET_X != 0;
}

void Reset_Move(struct _TASK* task_ptr) {
    RESET_X = 0;
    Check_Reset();

    if (RESET_X) {
        ToneDown(0xFF, 0);
        sound_all_off();
        task_ptr->r_no[0] = 2;
        task_ptr->free[0] = Setup_Next_Disposal();
        task_ptr->r_no[1] = 0;
        Request_LDREQ_Break();
        effect_work_init();
    }
}

void Reset_Wait(struct _TASK* task_ptr) {
    ToneDown(0xFF, 0);

    switch (task_ptr->r_no[1]) {
    case 0:
        sound_all_off();

        if (Check_LDREQ_Break() == 0) {
            task_ptr->r_no[1] += 1;
        }

        break;

    case 1:
        Soft_Reset_Sub();
        task_ptr->r_no[0] += 1;
        break;
    }
}

void Reset_Sleep(struct _TASK* task_ptr) {
    ToneDown(0xFF, 0);

    if (Pause_ID == 0) {
        if (!(p1sw_0 & 0x4000)) {
            task_ptr->r_no[0] = 0;
        }
    } else if (!(p2sw_0 & 0x4000)) {
        task_ptr->r_no[0] = 0;
    }

    if (task_ptr->r_no[0] == 0) {
        checkAdxFileLoaded();
        checkSelObjFileLoaded();
    }
}

void Check_Reset() {
    if (Forbid_Reset) {
        RESET_X = 0;
        return;
    }

    Switch_Type = 1;

    if (Check_SoftReset(0) == 0) {
        Check_SoftReset(1);
    }
}

u8 Check_SoftReset(s16 PL_id) {
    if (Reset_Status[PL_id] == 0x63) {
        Game_pause = 0x81;
        Pause_ID = PL_id;
        return RESET_X = 1;
    }

    return RESET_X = 0;
}

s32 Setup_Next_Disposal() {
    if (Reset_Bootrom) {
        return 1;
    }

    if ((G_No[0] == 1) || ((G_No[0] == 2) && (G_No[1] == 0))) {
        return 1;
    }

    return 0;
}

void Check_Reset_IO(struct _TASK* /* unused */, s16 PL_id) {
    u16 sw;
    u16 plsw;

    if (Switch_Type == 0) {
        if (PL_id) {
            plsw = p2sw_0;
        } else {
            plsw = p1sw_0;
        }
    } else {
        plsw = PLsw[PL_id][0];
    }

    sw = plsw & (SWK_START | SWK_BACK);

    if (sw == 0) {
        Reset_Status[PL_id] = 0;
        return;
    }

    switch (Reset_Status[PL_id]) {
    case 0:
        if (sw == (SWK_START | SWK_BACK)) {
            Reset_Status[PL_id] = 0x63;
            break;
        }

        if (sw & SWK_START) {
            Reset_Status[PL_id] = 0x62;
        }

        break;

    case 0x62:
        if (!(sw & SWK_START)) {
            Reset_Status[PL_id] = 0;
        }

        break;

    default:
        if (plsw != (SWK_START | SWK_BACK)) {
            Reset_Status[PL_id] = 0;
        }

        break;
    }
}
