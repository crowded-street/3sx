#include "sf33rd/Source/Game/debug/Debug.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/Source/Game/MTRANS.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/debug/Deb_Data.h"
#include "sf33rd/Source/Game/debug/Debug_ID.h"
#include "sf33rd/Source/Game/debug/Nakai.h"
#include "sf33rd/Source/Game/debug/OBJTEST.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/io/ioconv.h"

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_YELLOW 0xFFFFFF00

// sbss
s8 Debug_w[72];
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

    Disp_Free_work();
    Disp_Random();
}

void Debug_Init(struct _TASK* task_ptr) {
    u8* ptr;
    u16 ix;

    task_ptr->r_no[0] += 1;
    Debug_Index = 0;
    Debug_Pause = 0;
    ptr = (u8*)NAKAI_debug_data;
    Debug_Index = NAKAI_debug_data[72];

    for (ix = 0; ix < 72; ptr++) {
        Debug_w[ix] = *ptr;
        ix += 1;
    }

    if ((flpad_adr[0]->sw | flpad_adr[0][1].sw) & 0x4000) {
        Debug_w[0x2C] = 1;
    }
}

void Debug_1st(struct _TASK* task_ptr) {
    sysFF = 1;
    return;
}

void Debug_2nd(struct _TASK* task_ptr) {
#if defined(TARGET_PS2)
    void Debug_Menu_Disp(s16, s16);
#endif

    u16 sw;
    s16 offset_y[4];

    offset_y[0] = 2;
    offset_y[1] = 6;
    offset_y[2] = 2;
    flPrintColor(-256);
    flPrintL(1, 1, "[DEBUG MODE]");
    flPrintL(14, 1, (s8*)debug_name_data[Debug_ID]);

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

    if (io_w.data[1].sw_new & 0x1000) {
        mpp_w.sysStop = 1;
        task_ptr->r_no[0] = 1;
        cpRevivalTask();
    }
}

void Debug_Menu_Disp(u32 /* unused */, u32 /* unused */) {
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
            } else {
                flPrintColor(COLOR_YELLOW);
            }

            flPrintL(x, y, debug_string_data[ix].name);
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

    lever = io_w.data[1].sw & 0xF;
    sw = io_w.data[1].sw_new;

    if (sw & 0x130) {
        return sw;
    }

    sw &= 0xF;

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

        if (lever & 3) {
            ix = 0;
        } else {
            ix = 3 & 0xFFFF;
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

    shot = io_w.data[1].sw & 0x130;
    sw = io_w.data[1].sw_new;

    if (sw & 16) {
        return sw;
    }

    if (sw & 32) {
        return sw;
    }

    if (sw & 256) {
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

void Check_Check_Screen() {
    s16 ix;

    if ((test_flag) || (Debug_w[70] != -16)) {
        return;
    }

    switch (check_screen_S) {
    case 0:
        if (~(p1sw_1) & (p1sw_0) & 0x40) {
            check_screen_S += 1;
        }

        break;

    case 1:
        if (~(p1sw_1) & (p1sw_0) & 0x40) {
            check_screen_S += 1;
            check_time_S = 10;
        } else {
            flPrintL(1, 32, "%04X", p1sw_0);
            flPrintL(1, 33, "%04X", p2sw_0);
            flPrintL(1, 34, "%04X", p3sw_0);
            flPrintL(1, 35, "%04X", p4sw_0);
            SSPutStr(0, 20, 9, "0123456789");
            SSPutStr(10, 20, 9, "0123456789");
            SSPutStr(20, 20, 9, "0123456789");
            SSPutStr(30, 20, 9, "0123456789");
            SSPutStr(40, 20, 9, "0123456789");
            SSPutStr(50, 20, 9, "0123456789");
            SSPutStr(0, 6, 9, "0123456789");
            SSPutStr(10, 6, 9, "0123456789");
            SSPutStr(20, 6, 9, "0123456789");
            SSPutStr(30, 6, 9, "0123456789");
            SSPutStr(40, 6, 9, "0123456789");
            SSPutStr(50, 6, 9, "0123456789");
        }

        break;

    default:
        if (--check_time_S == 0) {
            check_screen_S = 0;
        }

        break;
    }

    switch (check_screen_L) {
    case 0:
        if (~(p1sw_1) & (p1sw_0) & 0x400) {
            check_screen_L += 1;
            return;
        }

        break;

    case 1:
        if (~(p1sw_1) & (p1sw_0) & 0x400) {
            check_screen_L += 1;
            check_time_L = 10;
            return;
        }

        for (ix = 0; ix < 3; ix++) {
            SSPutStr(41, (ix * 10), 9, "0");
            SSPutStr(41, ((ix * 10) + 1), 9, "1");
            SSPutStr(41, ((ix * 10) + 2), 9, "2");
            SSPutStr(41, ((ix * 10) + 3), 9, "3");
            SSPutStr(41, ((ix * 10) + 4), 9, "4");
            SSPutStr(41, ((ix * 10) + 5), 9, "5");
            SSPutStr(41, ((ix * 10) + 6), 9, "6");
            SSPutStr(41, ((ix * 10) + 7), 9, "7");
            SSPutStr(41, ((ix * 10) + 8), 9, "8");
            SSPutStr(41, ((ix * 10) + 9), 9, "9");
            SSPutStr(9, (ix * 10), 9, "0");
            SSPutStr(9, ((ix * 10) + 1), 9, "1");
            SSPutStr(9, ((ix * 10) + 2), 9, "2");
            SSPutStr(9, ((ix * 10) + 3), 9, "3");
            SSPutStr(9, ((ix * 10) + 4), 9, "4");
            SSPutStr(9, ((ix * 10) + 5), 9, "5");
            SSPutStr(9, ((ix * 10) + 6), 9, "6");
            SSPutStr(9, ((ix * 10) + 7), 9, "7");
            SSPutStr(9, ((ix * 10) + 8), 9, "8");
            SSPutStr(9, ((ix * 10) + 9), 9, "9");
        }

        return;

    default:
        if (--check_time_L == 0) {
            check_screen_L = 0;
        }

        break;
    }
}

void Disp_Lever(u16* /* unused */, u32 /* unused */, u32 /* unused */) {
    // do nothing
}

void Check_Pos_OBJ(WORK_Other* ewk) {
    if (Debug_w[0x46]) {
        if (p4sw_0 & 1) {
            ewk->wu.xyz[1].disp.pos += 3;
        }

        if (p4sw_0 & 2) {
            ewk->wu.xyz[1].disp.pos -= 3;
        }

        if (p4sw_0 & 8) {
            ewk->wu.xyz[0].disp.pos += 4;
        }

        if (p4sw_0 & 4) {
            ewk->wu.xyz[0].disp.pos -= 4;
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;

        flPrintColor(COLOR_YELLOW);
        flPrintL(2, 13, "%4X", ewk->wu.xyz[0].disp.pos);
        flPrintL(9, 13, "%4X", ewk->wu.position_x);
        flPrintL(2, 14, "%4X", ewk->wu.xyz[1].disp.pos);
        flPrintL(9, 14, "%4X", ewk->wu.position_y);
        flPrintL(2, 15, "%4X", ewk->wu.position_z);
    }
}

void Check_Pos_OBJ2(WORK_Other* ewk) {
    if (Debug_w[0x46]) {
        if (p4sw_0 & 1) {
            ewk->wu.position_y += 3;
        }

        if (p4sw_0 & 2) {
            ewk->wu.position_y -= 3;
        }

        if (p4sw_0 & 8) {
            ewk->wu.position_x += 4;
        }

        if (p4sw_0 & 4) {
            ewk->wu.position_x -= 4;
        }

        flPrintColor(COLOR_WHITE);
        flPrintL(12, 13, "%4X", ewk->wu.xyz[0].disp.pos);
        flPrintL(19, 13, "%4X", ewk->wu.position_x);
        flPrintL(12, 14, "%4X", ewk->wu.xyz[1].disp.pos);
        flPrintL(19, 14, "%4X", ewk->wu.position_y);
        flPrintL(12, 15, "%4X", ewk->wu.position_z);
    }
}

void Check_Pos_BG() {
    if ((Debug_w[0x46]) == 9) {
        if (p3sw_0 & 1) {
            bg_w.bgw[Debug_w[0x3E]].xy[1].disp.pos += 1;
        }

        if (p3sw_0 & 2) {
            bg_w.bgw[Debug_w[0x3E]].xy[1].disp.pos -= 1;
        }

        if (p3sw_0 & 8) {
            bg_w.bgw[Debug_w[0x3E]].wxy[0].disp.pos += 1;
        }

        if (p3sw_0 & 4) {
            bg_w.bgw[Debug_w[0x3E]].wxy[0].disp.pos -= 1;
        }

        flPrintColor(COLOR_YELLOW);
        flPrintL(39, 13, "BG POSITION:");
        flPrintL(51, 13, "%1X", Debug_w[0x3E]);
        flPrintL(48, 14, "%4X", bg_w.bgw[Debug_w[0x3E]].wxy[0].disp.pos);
        flPrintL(48, 15, "%4X", bg_w.bgw[Debug_w[0x3E]].xy[1].disp.pos);
    }
}

s32 Check_Exit_Check() {
    u16 sw = (p2sw_0 & 0xFF0);

    if (sw == 32) {
        return 1;
    }

    return 0;
}

void Disp_Rec_Time(s16 PL_id, u32 time) {
    u32 time_buff;
    u8 rec_time[3];
    s16 offset_y;

    if (Debug_w[0x46]) {
        flPrintColor(COLOR_WHITE);

        if (PL_id) {
            offset_y = 1;
        } else {
            offset_y = 0;
        }

        time_buff = time;
        rec_time[0] = time_buff / 216000;
        time_buff -= rec_time[0] * 216000;
        rec_time[1] = time_buff / 3600;
        time_buff -= rec_time[1] * 3600;
        rec_time[2] = time_buff / 60;
        flPrintL(2, offset_y + 12, "TIME:  h  m  s");
        flPrintL(7, offset_y + 12, "%2d", rec_time[0]);
        flPrintL(10, offset_y + 12, "%2d", rec_time[1]);
        flPrintL(13, offset_y + 12, "%2d", rec_time[2]);
    }
}

void Disp_Mode(PLW* wk) {
    s16 x;
    s16 offset_y = 0;

    if (Debug_w[0x38]) {
        flPrintColor(COLOR_YELLOW);

        if (wk->wu.id) {
            x = 49;
        } else {
            x = 1;
        }

        flPrintL(x, offset_y + 16, "%3X", Control_Time);
        flPrintL(x + 1, offset_y + 17, cpu_data[CP_No[wk->wu.id][0]]);
        flPrintL(x, offset_y + 18, "%3d", Pattern_Index[wk->wu.id]);
    }
}

void Disp_Free_work() {
    if (Debug_w[0x39]) {
        flPrintColor(COLOR_WHITE);
        flPrintL(1, 8, "%3d", frwctr);
        flPrintL(1, 9, "%3d", frwctr_min);
    }
}

void Disp_Random() {
    s16 offset_y = -4;

    if (Debug_w[0x3B]) {
        flPrintColor(COLOR_YELLOW);
        flPrintL(8, offset_y + 32, "%4X", Random_ix16_com);
        flPrintL(8, offset_y + 33, "%4X", Random_ix32_com);
        flPrintL(8, offset_y + 34, "%4X", Random_ix16_ex_com);
        flPrintL(8, offset_y + 35, "%4X", Random_ix32_ex_com);
        flPrintL(14, offset_y + 32, "%4X", Random_ix16);
        flPrintL(14, offset_y + 33, "%4X", Random_ix32);
        flPrintL(14, offset_y + 34, "%4X", Random_ix16_ex);
        flPrintL(14, offset_y + 35, "%4X", Random_ix32_ex);
        flPrintL(20, offset_y + 32, "%4X", plw->wu.position_x);
        flPrintL(20, offset_y + 33, "%4X", plw->wu.position_y);
        flPrintL(20, offset_y + 34, "%4X", plw[1].wu.position_x);
        flPrintL(20, offset_y + 35, "%4X", plw[1].wu.position_y);
        flPrintL(26, offset_y + 32, "%4X", Turbo_Timer);
        flPrintL(26, offset_y + 33, "%4X", players_timer);
        flPrintL(26, offset_y + 34, "%4X", system_timer);
        flPrintL(26, offset_y + 35, "%4X", Game_timer);
        flPrintL(32, offset_y + 32, "%4X", time_check[0]);
        flPrintL(32, offset_y + 33, "%4X", time_check[1]);
        flPrintL(32, offset_y + 34, "%4X", time_check[2]);
        flPrintL(32, offset_y + 35, "%4X", time_check[3]);
        flPrintL(32, offset_y + 29, "%4X", bg_w.bgw[1].wxy[0].disp.pos);
        flPrintL(32, offset_y + 30, "%4X", bg_w.bgw[1].xy[0].disp.pos);
    }
}

// sdata
s8* cpu_data[16] = { "", "FR", "AC", "BF", "FW", "BP", "PS", "GD", "SH", "SG", "DM", "FL", "FP", "CT", "WL", "CH" };
