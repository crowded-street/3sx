/**
 * @file ioconv.c
 * Input Conversion and Processing
 */

#include "sf33rd/Source/Game/io/ioconv.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/main.h"

IO io_w;

u32 ioconv_table[24][2] = { { 0x1, 0x1 },       { 0x2, 0x2 },       { 0x4, 0x4 },       { 0x8, 0x8 },
                            { 0x100, 0x10 },    { 0x200, 0x20 },    { 0x400, 0x40 },    { 0x800, 0x80 },
                            { 0x10, 0x100 },    { 0x20, 0x200 },    { 0x40, 0x400 },    { 0x80, 0x800 },
                            { 0x2000, 0x1000 }, { 0x1000, 0x2000 }, { 0x8000, 0x4000 }, { 0x4000, 0x8000 },
                            { 0x0, 0x10000 },   { 0x0, 0x20000 },   { 0x0, 0x40000 },   { 0x0, 0x80000 },
                            { 0x0, 0x100000 },  { 0x0, 0x200000 },  { 0x0, 0x400000 },  { 0x0, 0x800000 } };

void keyConvert() {
    IOPad* pad;
    u32 currSw;
    s32 i;
    s32 j;
    s32 repeat_on = 0;

    if (Debug_w[0x2B] && mpp_w.inGame && (Game_pause == 0)) {
        repeat_on = 1;
    }

    if ((save_w[Present_Mode].extra_option.contents[0][4]) && mpp_w.inGame && (Game_pause == 0)) {
        repeat_on = 1;

        if ((task[TASK_MENU].condition == 1) && (task[TASK_MENU].r_no[0] != 10)) {
            repeat_on = 0;
        }
    }

    for (i = 0; i < 2; i++) {
        flPADSetRepeatSw(&flpad_adr[0][i], 0xFF000F, 15, 3);

        if (repeat_on) {
            flPADSetRepeatSw(&flpad_adr[0][i], 0x3FF0, 2, 1);
        } else {
            flPADSetRepeatSw(&flpad_adr[0][i], 0x3FF0, 10, 2);
        }

        pad = &io_w.data[i];
        pad->state = flpad_adr[0][i].state;
        pad->anstate = flpad_adr[0][i].anstate;
        pad->kind = flpad_adr[0][i].kind;
        pad->sw = flpad_adr[0][i].sw;
        pad->sw_old = flpad_adr[0][i].sw_old;
        pad->sw_new = flpad_adr[0][i].sw_new;
        pad->sw_off = flpad_adr[0][i].sw_off;
        pad->sw_chg = flpad_adr[0][i].sw_chg;
        pad->sw_repeat = flpad_adr[0][i].sw_repeat;
        pad->stick[0] = flpad_adr[0][i].stick[0];
        pad->stick[1] = flpad_adr[0][i].stick[1];

        if (mpp_w.useAnalogStickData) {
            if (!(flpad_adr[0][i].sw & 0xF)) {
                pad->sw |= (pad->sw >> 16) & 0xF;
                pad->sw_old |= (pad->sw_old >> 16) & 0xF;
                pad->sw_new |= (pad->sw_new >> 16) & 0xF;
                pad->sw_off |= (pad->sw_off >> 16) & 0xF;
                pad->sw_chg |= (pad->sw_chg >> 16) & 0xF;
                pad->sw_repeat |= (pad->sw_repeat >> 16) & 0xF;
            }

            if (!(flpad_adr[0][i].sw & 0xF)) {
                pad->sw |= (pad->sw >> 20) & 0xF;
                pad->sw_old |= (pad->sw_old >> 20) & 0xF;
                pad->sw_new |= (pad->sw_new >> 20) & 0xF;
                pad->sw_off |= (pad->sw_off >> 20) & 0xF;
                pad->sw_chg |= (pad->sw_chg >> 20) & 0xF;
                pad->sw_repeat |= (pad->sw_repeat >> 20) & 0xF;
            }
        }

        if (pad->kind == 0 || pad->kind == 0x8000) {
            Interface_Type[i] = 0;
        } else {
            Interface_Type[i] = 2;
        }

        io_w.sw[i] = 0;
        currSw = pad->sw;

        for (j = 0; j < 4; j++) {
            if (currSw & ioconv_table[j][1]) {
                io_w.sw[i] |= ioconv_table[j][0];
            }
        }

        for (j = 12; j < 16; j++) {
            if (currSw & ioconv_table[j][1]) {
                io_w.sw[i] |= ioconv_table[j][0];
            }
        }

        if (repeat_on) {
            currSw = pad->sw_repeat;
        }

        for (j = 4; j < 12; j++) {
            if (currSw & ioconv_table[j][1]) {
                io_w.sw[i] |= ioconv_table[j][0];
            }
        }
    }

    p1sw_buff = io_w.sw[0];
    p2sw_buff = io_w.sw[1];
}
