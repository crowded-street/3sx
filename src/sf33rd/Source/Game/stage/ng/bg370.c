/**
 * @file bg370.c
 * Elena's stage from New Generation (BG140 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg370.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/effc2.h"
#include "sf33rd/Source/Game/effect/ng/effc3.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"

#define CLOUDS_SLOW_LINE 792
#define CLOUDS_SLOWER_LINE 835
#define CLOUDS_END_LINE 855

static void bg3700();
static void bg3700_init00();
static void bg3700_move01();
static void bg3701();
static void bg3701_init00();
static void bg3701_move01();
static void bg3702();
static void bg370_move00();
static void bg370_drift_clouds();

void BG370() {
    bgw_ptr = &bg_w.bgw[1];
    bg3701();
    bgw_ptr = &bg_w.bgw[0];
    bg3700();
    bgw_ptr = &bg_w.bgw[2];
    bg3702();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg3700() {
    void (*bg3700_jmp[3])() = { bg3700_init00, bg370_move00, bg3700_move01 };
    bg3700_jmp[bgw_ptr->r_no_0]();
    bg370_drift_clouds();
}

static void bg3700_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0xB0;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->r_no_1 = 0;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0x10;
    bgw_ptr->no_suzi_line = 512;
    bgw_ptr->u_line = 512;
    bgw_ptr->d_line = 96;
    bgw_ptr->start_suzi = 512;
    bg_w.line_scroll_frames = 0;

    // FIXME: NG effects 45, 53 and 56 start here
    effect_ngc2_init();
    effect_ngc3_init(0);
    effect_05_init();
    effect_06_init();
}

static void bg3700_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg3701() {
    void (*bg3701_jmp[3])() = { bg3701_init00, bg370_move00, bg3701_move01 };
    bg3701_jmp[bgw_ptr->r_no_0]();
}

static void bg3701_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bg_app = 1;
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0xB0;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg3701_move01() {
    // FIXME: NG breaks the bridge here
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3702() {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0++;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->zuubun = 0;
        bgw_ptr->fam_no = 2;
        bgw_ptr->xy[1].disp.pos = bgw_ptr->pos_y_work = 0;
        bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0x60;
        bgw_ptr->speed_x = 0xC000;
        bgw_ptr->speed_y = 0xC000;

        if (bg_w.area == 0) {
            bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0xB0;
        } else {
            bgw_ptr->r_no_0++;
        }

        break;

    case 1:
        bg370_move00();
        break;

    case 2:
        bg_x_move_check();
        bg_y_move_check();
        break;
    }

    sync_fam_set3(2);
}

static void bg370_move00() {
    if (plw[0].player_number == CHAR_ELENA && plw[1].player_number == CHAR_ELENA) {
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * 4;
    } else {
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * (Cut_Cut_Cut() ? 4 : 2);
    }

    if (bgw_ptr->xy[1].cal < bgw_ptr->pos_y_work) {
        bgw_ptr->xy[1].cal = bgw_ptr->pos_y_work;
        bgw_ptr->r_no_0++;
        bg_app = 0;
    }
}

static void bg370_drift_clouds() {
    if (!EXE_flag && !Game_pause) {
        bg_w.line_scroll_frames++;
    }
}

s16 bg370_line_scroll(s16 bgnm, s16 line) {
    u32 speed;

    if (bgnm != 0 || line >= CLOUDS_END_LINE) {
        speed = 0;
    } else if (line >= CLOUDS_SLOWER_LINE) {
        speed = 0x1000;
    } else if (line >= CLOUDS_SLOW_LINE) {
        speed = 0x3000;
    } else {
        speed = 0x5000;
    }

    return (s16)(((LINE_SCROLL_BASE << 16) + bg_w.line_scroll_frames * speed) >> 16);
}
