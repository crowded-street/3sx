/**
 * @file ioconv.c
 * Input Conversion and Processing
 */

#include "sf33rd/Source/Game/io/ioconv.h"
#include "common.h"
#include "main.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"

IO io_w;

u32 ioconv_table[24][2] = { { 0x1, 0x1 },       { 0x2, 0x2 },       { 0x4, 0x4 },       { 0x8, 0x8 },
                            { 0x100, 0x10 },    { 0x200, 0x20 },    { 0x400, 0x40 },    { 0x800, 0x80 },
                            { 0x10, 0x100 },    { 0x20, 0x200 },    { 0x40, 0x400 },    { 0x80, 0x800 },
                            { 0x2000, 0x1000 }, { 0x1000, 0x2000 }, { 0x8000, 0x4000 }, { 0x4000, 0x8000 },
                            { 0x0, 0x10000 },   { 0x0, 0x20000 },   { 0x0, 0x40000 },   { 0x0, 0x80000 },
                            { 0x0, 0x100000 },  { 0x0, 0x200000 },  { 0x0, 0x400000 },  { 0x0, 0x800000 } };

/* One range of the switch conversion table, folded into this pad's word. The
 * three scans differ only in the range they walk, and each names its own at its
 * call site. */
static void scan_ioconv_range(u32 currSw, s32 i, s32 first, s32 limit) {
    s32 j;

    for (j = first; j < limit; j++) {
        if (currSw & ioconv_table[j][1]) {
            io_w.sw[i] |= ioconv_table[j][0];
        }
    }
}

/* Fold one analog stick's direction bits down into the digital ones, when the
 * digital pad is reporting nothing. The two copies differ only in which half of
 * the word they take. */
static void merge_analog_direction(IOPad* pad, s32 i, s32 shift) {
    if (!(flpad_adr[0][i].sw & 0xF)) {
        pad->sw |= (pad->sw >> shift) & 0xF;
        pad->sw_old |= (pad->sw_old >> shift) & 0xF;
        pad->sw_new |= (pad->sw_new >> shift) & 0xF;
        pad->sw_off |= (pad->sw_off >> shift) & 0xF;
        pad->sw_chg |= (pad->sw_chg >> shift) & 0xF;
        pad->sw_repeat |= (pad->sw_repeat >> shift) & 0xF;
    }
}

/* The three tests that decide whether the turbo repeat rate applies, each
 * copied character for character from the condition it stood in. The first sits
 * inside the same `#if DEBUG` as its only caller. */
#if DEBUG
static bool debug_turbo_active() {
    return debug_config.turbo_buttons && mpp_w.inGame && (Game_pause == 0);
}
#endif

static bool turbo_option_active() {
    return (save_w[Present_Mode].extra_option.contents[0][4]) && mpp_w.inGame && (Game_pause == 0);
}

static bool menu_task_blocks_turbo() {
    return (task[TASK_MENU].condition == 1) && (task[TASK_MENU].r_no[0] != 10);
}

/* This frame's pad state, copied across and adjusted: the analog stick folded
 * in where the option asks for it, and the interface type from the pad kind. */
static void copy_pad_state(IOPad* pad, s32 i) {
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
        merge_analog_direction(pad, i, 16);

        merge_analog_direction(pad, i, 20);
    }

    if (pad->kind == 0 || pad->kind == 0x8000) {
        Interface_Type[i] = 0;
    } else {
        Interface_Type[i] = 2;
    }
}

/* Whether the pad repeat runs at the turbo rate this frame. */
static s32 turbo_repeat_enabled() {
    s32 repeat_on = 0;

#if DEBUG
    if (debug_turbo_active()) {
        repeat_on = 1;
    }
#endif

    if (turbo_option_active()) {
        repeat_on = 1;

        if (menu_task_blocks_turbo()) {
            repeat_on = 0;
        }
    }

    return repeat_on;
}

void keyConvert() {
    IOPad* pad;
    u32 currSw;
    s32 i;
    s32 repeat_on = turbo_repeat_enabled();

    for (i = 0; i < 2; i++) {
        flPADSetRepeatSw(&flpad_adr[0][i], 0xFF000F, 15, 3);

        if (repeat_on) {
            flPADSetRepeatSw(&flpad_adr[0][i], 0x3FF0, 2, 1);
        } else {
            flPADSetRepeatSw(&flpad_adr[0][i], 0x3FF0, 10, 2);
        }

        pad = &io_w.data[i];
        copy_pad_state(pad, i);

        io_w.sw[i] = 0;
        currSw = pad->sw;

        scan_ioconv_range(currSw, i, 0, 4);

        scan_ioconv_range(currSw, i, 12, 16);

        if (repeat_on) {
            currSw = pad->sw_repeat;
        }

        scan_ioconv_range(currSw, i, 4, 12);
    }

    p1sw_buff = io_w.sw[0];
    p2sw_buff = io_w.sw[1];
}
