/**
 * @file bg260.c
 * Hot Spring Inn Entrance, Ryu's stage from New Generation (BG030 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg260.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static void bg2600();
static void bg2600_init00();
static void bg2600_move01();
static void bg2601();
static void bg2601_init00();
static void bg2601_move01();
static void bg2602();
static void bg2602_init00();
static void bg2602_move01();
static void bg260_move00();

void BG260() {
    bgw_ptr = &bg_w.bgw[1];
    bg2601();
    bgw_ptr = &bg_w.bgw[0];
    bg2600();
    bgw_ptr = &bg_w.bgw[2];
    bg2602();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg2600() {
    void (*bg2600_jmp[3])() = { bg2600_init00, bg260_move00, bg2600_move01 };
    bg2600_jmp[bgw_ptr->r_no_0]();
}

static void bg2600_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0xB0;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg2600_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg2601() {
    void (*bg2601_jmp[3])() = { bg2601_init00, bg260_move00, bg2601_move01 };
    bg2601_jmp[bgw_ptr->r_no_0]();
}

static void bg2601_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0xB0;
        bg_app = 1;
    } else {
        bgw_ptr->r_no_0++;
    }

    // Unlike the other layers, this one is centred on 0. The nonzero zuubun makes the chase use the signed abs_x.
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0xCC;

    // NG also sets up line scroll for this layer here (no_suzi_line 0x3C7, u_line 0x10, d_line 0x28) and updates it
    // in move01. Our renderer has no line scroll.
    // NG then loads palette 0x66 and preloads CGs 0x41C0 and 0x7718 into sprite RAM, which we don't need.
    effect_05_init();
    effect_06_init();

    // TODO: NG starts effects 18 (0), 14 (1, 2, 3) and 34 here
}

static void bg2601_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg2602() {
    void (*bg2602_jmp[3])() = { bg2602_init00, bg260_move00, bg2602_move01 };
    bg2602_jmp[bgw_ptr->r_no_0]();
}

static void bg2602_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0xB0;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg2602_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

/// Scrolls the layer down into place during the intro
static void bg260_move00() {
    if (Cut_Cut_Cut()) {
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * 4;
    } else {
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * 2;
    }

    if (bgw_ptr->xy[1].cal < 0) {
        bgw_ptr->xy[1].cal = 0;
        bgw_ptr->r_no_0++;
        bg_app = 0;
    }
}
