/**
 * @file bg270.c
 * Second area of Ryu's stage from New Generation (BG040 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg270.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff18.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static void bg2700();
static void bg2700_init00();
static void bg2700_move00();
static void bg2701();
static void bg2701_init00();
static void bg2701_move00();
static void bg2702();
static void bg2702_init00();
static void bg2702_move00();

void BG270() {
    bgw_ptr = &bg_w.bgw[1];
    bg2701();
    bgw_ptr = &bg_w.bgw[0];
    bg2700();
    bgw_ptr = &bg_w.bgw[2];
    bg2702();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg2700() {
    void (*bg2700_jmp[2])() = { bg2700_init00, bg2700_move00 };
    bg2700_jmp[bgw_ptr->r_no_0]();
}

static void bg2700_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg2700_move00() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg2701() {
    void (*bg2701_jmp[2])() = { bg2701_init00, bg2701_move00 };
    bg2701_jmp[bgw_ptr->r_no_0]();
}

static void bg2701_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0x1C7;
    // FIXME: Configure line scroll

    effect_05_init();
    effect_06_init();
    effect_ng18_init(1);
    // FIXME: NG also starts effect 88 (variant 4) when the byte at 0x02012d35 is 8.
}

static void bg2701_move00() {
    bg_base_x_move_check();
    // FIXME: Port bg0401_rewrite, which swaps blocks of this layer as it scrolls past ±0x90
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg2702() {
    void (*bg2702_jmp[2])() = { bg2702_init00, bg2702_move00 };
    bg2702_jmp[bgw_ptr->r_no_0]();
}

static void bg2702_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg2702_move00() {
    bg_x_move_check();
    bg_y_move_check();
}
