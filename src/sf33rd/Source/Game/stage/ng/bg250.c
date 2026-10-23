/**
 * @file bg250.c
 * Sean's snowy New York street from New Generation (BG020 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg250.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff25.h"
#include "sf33rd/Source/Game/effect/ng/eff44.h"
#include "sf33rd/Source/Game/effect/ng/eff61.h"
#include "sf33rd/Source/Game/effect/ng/eff64.h"
#include "sf33rd/Source/Game/effect/ng/eff87.h"
#include "sf33rd/Source/Game/effect/ng/eff88.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"

#define INTRO_HEIGHT 0x10A

static const s32 sync_speeds[2][2] = { { 0xE800, 0x10000 }, { 0xCE00, 0x10000 } };

static void bg2500();
static void bg2500_init00();
static void bg2500_move01();
static void bg2501();
static void bg2501_init00();
static void bg2501_move01();
static void bg250_sync(s16 index);
static void bg250_move00();

void BG250() {
    bgw_ptr = &bg_w.bgw[1];
    bg2501();
    bgw_ptr = &bg_w.bgw[0];
    bg2500();
    bgw_ptr = &bg_w.bgw[2];
    bg250_sync(0);
    bgw_ptr = &bg_w.bgw[4];
    bg250_sync(1);
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg250_init_intro() {
    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * INTRO_HEIGHT;
    } else {
        bgw_ptr->r_no_0++;
    }
}

static void bg2500() {
    void (*bg2500_jmp[3])() = { bg2500_init00, bg250_move00, bg2500_move01 };
    bg2500_jmp[bgw_ptr->r_no_0]();
}

static void bg2500_init00() {
    bgw_ptr->r_no_0++;
    bg250_init_intro();
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg2500_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg2501() {
    void (*bg2501_jmp[3])() = { bg2501_init00, bg250_move00, bg2501_move01 };
    bg2501_jmp[bgw_ptr->r_no_0]();
}

static void bg2501_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bg_app = 1;
    }

    bg250_init_intro();
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;

    effect_05_init();
    effect_06_init();
    effect_ng44_init(); // Buildings
    effect_ng87_init(1);
    effect_ng88_init(2);

    switch (bg_w.object_set) {
    case 0:
        effect_ng61_init(); // Spectators
        break;

    case 1:
    case 2:
        effect_ng64_init(0);
        break;

    case 3:
        effect_25_init(3);
        break;
    }

    effect_ng64_init(1);
    effect_ng64_init(2);
    effect_25_init(4);
}

static void bg2501_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg250_sync(s16 index) {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0++;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->xy[1].disp.pos = bgw_ptr->pos_y_work = 0;
        bgw_ptr->zuubun = 0;
        bgw_ptr->fam_no = index == 0 ? 2 : 4;
        bgw_ptr->xy[0].disp.low = bgw_ptr->xy[1].disp.low = 0;
        bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0x100;
        bgw_ptr->speed_x = sync_speeds[index][0];
        bgw_ptr->speed_y = sync_speeds[index][1];
        bg250_init_intro();
        break;

    case 1:
        bg250_move00();
        break;

    case 2:
        bg_x_move_check();
        bg_y_move_check();
        break;
    }

    sync_fam_set3(bgw_ptr->fam_no);
}

static void bg250_move00() {
    bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * (Cut_Cut_Cut() ? 6 : 4);

    if (bgw_ptr->xy[1].cal < 0) {
        bgw_ptr->xy[1].cal = 0;
        bgw_ptr->r_no_0++;
        bg_app = 0;
    }
}
