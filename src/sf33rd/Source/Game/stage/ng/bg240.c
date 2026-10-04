/**
 * @file bg240.c
 * Alex's stage from New Generation (BG010 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg240.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff64.h"
#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static void bg2400();
static void bg2400_init00();
static void bg2400_move01();
static void bg2401();
static void bg2401_init00();
static void bg2401_move01();
static void bg2402();
static void bg2402_init00();
static void bg2402_move01();
static void bg2404();
static void bg240_move00();

void BG240() {
    bgw_ptr = &bg_w.bgw[1];
    bg2401();
    bgw_ptr = &bg_w.bgw[0];
    bg2400();
    bgw_ptr = &bg_w.bgw[2];
    bg2402();
    bgw_ptr = &bg_w.bgw[4];
    bg2404();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg2400() {
    void (*bg2400_jmp[3])() = { bg2400_init00, bg240_move00, bg2400_move01 };
    bg2400_jmp[bgw_ptr->r_no_0]();
}

static void bg2400_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0x10A;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].disp.pos = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg2400_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg2401() {
    void (*bg2401_jmp[3])() = { bg2401_init00, bg240_move00, bg2401_move01 };
    bg2401_jmp[bgw_ptr->r_no_0]();
}

static void bg2401_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0x10A;
        bg_app = 1;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;

    effect_05_init();
    effect_06_init();
    // effect_69_init();
    effect_ng64_init(3);
    effect_ng64_init(4);
    effect_ng94_init_for_players(1);
}

static void bg2401_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg2402() {
    void (*bg2402_jmp[3])() = { bg2402_init00, bg240_move00, bg2402_move01 };
    bg2402_jmp[bgw_ptr->r_no_0]();
}

static void bg2402_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0x10A;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg2402_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg2404() {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0++;
        bgw_ptr->speed_x = 0x8000;
        bgw_ptr->speed_y = 0xA000;
        bgw_ptr->fam_no = 3;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->zuubun = 0;
        bgw_ptr->xy[0].disp.low = 0;

        if (bg_w.area == 0) {
            bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0x10A;
        } else {
            bgw_ptr->pos_y_work = 0;
            bgw_ptr->xy[1].disp.pos = 0;
        }

        break;

    case 1:
        bg240_move00();
        break;

    case 2:
        bg_x_move_check();
        bg_y_move_check();
        sync_fam_set3(4);
        break;
    }
}

/// Scrolls the layer down into place during the intro
static void bg240_move00() {
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
