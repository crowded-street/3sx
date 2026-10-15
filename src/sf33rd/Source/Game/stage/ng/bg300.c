/**
 * @file bg300.c
 * Dudley's stage from New Generation (BG070 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg300.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static void bg3000();
static void bg3000_init00();
static void bg3000_move00();
static void bg3001();
static void bg3001_init00();
static void bg3001_move00();
static void bg3002();
static void bg3002_init00();
static void bg3002_move00();

void BG300() {
    bgw_ptr = &bg_w.bgw[1];
    bg3001();
    bgw_ptr = &bg_w.bgw[0];
    bg3000();
    bgw_ptr = &bg_w.bgw[2];
    bg3002();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg3000() {
    void (*bg3000_jmp[2])() = { bg3000_init00, bg3000_move00 };
    bg3000_jmp[bgw_ptr->r_no_0]();
}

static void bg3000_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;

    if (bg_w.area == 0) {
        // FIXME: Port NG effects 11 and 65 and the stage's sound 0x6D
        effect_ng94_init_for_players(0);
    } else {
        // FIXME: Port NG effects 66 (variant 0) and 68
        effect_ng94_init_for_players(1);
    }

    effect_05_init();
    effect_06_init();
}

static void bg3000_move00() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg3001() {
    void (*bg3001_jmp[2])() = { bg3001_init00, bg3001_move00 };
    bg3001_jmp[bgw_ptr->r_no_0]();
}

static void bg3001_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0x100;
    bgw_ptr->no_suzi_line = 976;
    bgw_ptr->u_line = 8;
    bgw_ptr->d_line = 40;
    bgw_ptr->start_suzi = 976;
}

static void bg3001_move00() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3002() {
    void (*bg3002_jmp[2])() = { bg3002_init00, bg3002_move00 };

    if (bg_w.area == 0) {
        bg3002_jmp[bgw_ptr->r_no_0]();
    }
}

static void bg3002_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg3002_move00() {
    bg_x_move_check();
    bg_y_move_check();
}
