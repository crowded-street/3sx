#include "sf33rd/Source/Game/debug/Debug.h"
#include "common.h"
#include "main.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/Source/Game/debug/OBJTEST.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/ioconv.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_YELLOW 0xFFFFFF00

bool debug_menu_active;
u8 Debug_ID;
s8 Debug_Index;
u8 Deley_Debug_No;
u8 Deley_Debug_Timer;
u8 Deley_Debug_No2;
u8 Deley_Debug_Timer2;
u8 Debug_Pause;
u8 sysFF;
u8 sysSLOW;
s8 Slow_Timer;
u8 check_screen_S;
u8 check_screen_L;
u8 check_time_S;
u8 check_time_L;
u32 Rec_Time[2];
u32 Record_Timer;
s16 time_check[4];
u8 time_check_ix;

// forward decls
extern s8* cpu_data[];

void Debug_Task(struct _TASK* task_ptr) {
    void (*Main_Jmp_Tbl[3])() = { Debug_Init, Debug_1st, Debug_2nd };

    Main_Jmp_Tbl[(task_ptr->r_no[0])](task_ptr);

    if (permission_player[1].ok[0] == 0) {
        permission_player[1].ok[0] = (u8)Debug_w[0x33];
    }

    if (Debug_w[0x3A]) {
        save_w[1].Extra_Option = 1;
    }
}

void Debug_Init(struct _TASK* task_ptr) {
    task_ptr->r_no[0] += 1;
    Debug_Index = 0;
    Debug_Pause = 0;

    if ((flpad_adr[0]->sw | flpad_adr[0][1].sw) & 0x4000) {
        Debug_w[0x2C] = 1;
    }
}

void Debug_1st(struct _TASK* task_ptr) {
    task_ptr->r_no[0] += 1; // Progress to Debug_2nd
    sysFF = 1;
    return;
}

void Debug_2nd(struct _TASK* task_ptr) {
#if DEBUG
    // Check for toggle: right stick click to show/hide debug menu
    if ((io_w.data[0].sw_new & SWK_RIGHT_STICK) || (io_w.data[1].sw_new & SWK_RIGHT_STICK)) {
        debug_menu_active = !debug_menu_active;
    }

    // Only show and interact with debug menu when active
    if (debug_menu_active) {
        u16 sw;
        s16 offset_y[4];

        offset_y[0] = 2;
        offset_y[1] = 6;
        offset_y[2] = 2;
        flPrintColor(-256);
        flPrintL(1, 1, "[DEBUG MODE]");
        flPrintL(14, 1, (s8*)debug_profile_name_data[Debug_ID]);

        if ((sw = Debug_Menu_Shot())) {
            if (sw == 256) {
                Debug_w[Debug_Index] = 0;
            }

            if ((sw == 32) && (--Debug_w[Debug_Index] < 0)) {
                Debug_w[Debug_Index] = debug_string_data[Debug_Index].max;
            }

            if ((sw == 16) && (++Debug_w[Debug_Index] > debug_string_data[Debug_Index].max)) {
                Debug_w[Debug_Index] = 0;
            }
        } else {
            sw = Debug_Menu_Lever();
            Debug_Move_Sub(sw);
        }

        Debug_Menu_Disp(offset_y[1], offset_y[2]);
    }
#endif
}

void Debug_Menu_Disp(u32 /* unused */, u32 /* unused */) {
#if DEBUG
    s16 side;
    s16 ix;
    s16 i;
    s16 x;
    s16 y;

    side = 0;
    ix = 0;
    x = 1;
    y = 3;

    for (; side < 3;) {
        for (i = 0; i < 24;) {
            if (Debug_Index != ix) {
                flPrintColor(COLOR_WHITE);
                flPrintL(x, y, " ");
            } else {
                flPrintColor(COLOR_YELLOW);
                flPrintL(x, y, ">"); // Arrow indicator for selected item
            }

            flPrintL(x + 1, y, debug_string_data[ix].name);
            flPrintL(x + 18, y, "%2X", Debug_w[ix]);
            i += 1;
            y += 2;
            ix++;
        }

        y = 3;
        side += 1;
        x += 21;
    }

    flPrintColor(COLOR_WHITE);
    flPrintL(1, 52, "SPR-MAX : %d", seqsGetSprMax());
#endif
}

void Debug_Move_Sub(u16 sw) {
    switch (sw) {
    case 1:
        Debug_Index -= 1;
        if (Debug_Index < 0) {
            Debug_Index = 23;
            return;
        }

        if (Debug_Index == 23) {
            Debug_Index = 47;
            return;
        }

        if (Debug_Index == 47) {
            Debug_Index = 71;
            return;
        }
        break;

    case 2:
        Debug_Index += 1;
        if (Debug_Index >= 72) {
            Debug_Index = 48;
            return;
        }

        if (Debug_Index == 24) {
            Debug_Index = 0;
            return;
        }

        if (Debug_Index == 48) {
            Debug_Index = 24;
            return;
        }
        break;

    case 4:
        Debug_Index -= 24;
        if (Debug_Index < 0) {
            Debug_Index += 72;
            return;
        }
        break;

    case 8:
        Debug_Index += 24;
        if (Debug_Index > 71) {
            Debug_Index -= 72;
            return;
        }
        break;

    case 256:
        Debug_w[Debug_Index] ^= 1;
        break;
    }
}

const u8 Debug_Deley_Time[6] = { 15, 10, 6, 15, 15, 15 };

s32 Debug_Menu_Lever() {
    u16 sw;
    u16 lever;
    u16 ix;

    lever = (io_w.data[0].sw | io_w.data[1].sw) & SWK_DIRECTIONS;
    sw = io_w.data[0].sw_new | io_w.data[1].sw_new;

    if (sw & (SWK_WEST | SWK_NORTH | SWK_SOUTH)) {
        return sw;
    }

    sw &= SWK_DIRECTIONS;

    if (sw) {
        return sw;
    }

    if (lever == 0) {
        Deley_Debug_No = 0;
        Deley_Debug_Timer = Debug_Deley_Time[Deley_Debug_No];
        return 0;
    }

    if (--Deley_Debug_Timer == 0) {
        if (++Deley_Debug_No > 2) {
            Deley_Debug_No = 2;
        }

        if (lever & (SWK_UP | SWK_DOWN)) {
            ix = 0;
        } else {
            ix = 3;
        }

        Deley_Debug_Timer = Debug_Deley_Time[Deley_Debug_No + ix];
        return lever;
    }

    return 0;
}

const u8 Debug_Deley_Time2[4] = { 15, 10, 6, 4 };

u16 Debug_Menu_Shot() {
    u16 sw;
    u16 shot;

    shot = (io_w.data[0].sw | io_w.data[1].sw) & (SWK_WEST | SWK_NORTH | SWK_SOUTH);
    sw = io_w.data[0].sw_new | io_w.data[1].sw_new;

    if (sw & SWK_WEST) {
        return sw;
    }

    if (sw & SWK_NORTH) {
        return sw;
    }

    if (sw & SWK_SOUTH) {
        return sw;
    }

    if (shot == 0) {
        Deley_Debug_No2 = 0;
        Deley_Debug_Timer2 = Debug_Deley_Time2[Deley_Debug_No2];
        return 0;
    }

    if (--Deley_Debug_Timer2 == 0) {
        if (++Deley_Debug_No2 > 3) {
            Deley_Debug_No2 = 3;
        }

        Deley_Debug_Timer2 = Debug_Deley_Time2[Deley_Debug_No2];
        return shot;
    }

    return 0;
}

s8* cpu_data[16] = { "", "FR", "AC", "BF", "FW", "BP", "PS", "GD", "SH", "SG", "DM", "FL", "FP", "CT", "WL", "CH" };
