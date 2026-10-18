/**
 * @file bg370.c
 * Elena's stage from New Generation (BG140 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg370.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff45.h"
#include "sf33rd/Source/Game/effect/ng/eff53.h"
#include "sf33rd/Source/Game/effect/ng/eff56.h"
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
#include "sf33rd/Source/Game/system/work_sys.h"

#include <SDL3/SDL.h>

#define CLOUDS_SLOW_LINE 792
#define CLOUDS_SLOWER_LINE 835
#define CLOUDS_END_LINE 855

#define BREAK_ZOOM 0x54
#define BREAK_ZOOM_X 0xC0
#define BREAK_ZOOM_Y 0xE0
#define BREAK_ZOOM_RANGE 0x38
#define BREAK_END_Y_CAL -0x10FFFFF // y -0x110 in 16.16, plus one

static void bg3700();
static void bg3700_init00();
static void bg3700_move01();
static void bg3701();
static void bg3701_init00();
static void bg3701_move01();
static void bg3701_move02();
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

    effect_ng45_init();
    effect_ng53_init();
    effect_ng56_init();
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
    void (*bg3701_jmp[4])() = { bg3701_init00, bg370_move00, bg3701_move01, bg3701_move02 };
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

    SDL_zero(bg_w.bridge);
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static bool should_bridge_break() {
    const s16 battle_number = save_w[Present_Mode].Battle_Number[Play_Type];
    return C_No[0] > 7 && (Round_num < battle_number * 2 || PL_Wins[Winner_id] < battle_number + 1);
}

static void bg3701_move01() {
    if (should_bridge_break()) {
        bg_w.bridge.breaking = 1;
        bgw_ptr->r_no_0++;
        bg_w.bridge.timer = 0x7000; // Effect C2 ends the shaking by clearing it
        bg_w.bridge.quake = 0;
        // FIXME: NG plays sound 0x76 here
    }

    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3701_move02() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        if (--bg_w.bridge.timer < 0) {
            bg_w.bridge.timer = 0x38;
            bgw_ptr->r_no_1++;
            break;
        }

        bg_w.bridge.quake ^= 1;
        bg_w.quake_y_index = bg_w.bridge.quake ? 0x11 : 0x12;

        if (bgw_ptr->xy[0].disp.pos > bgw_ptr->pos_x_work - BREAK_ZOOM_RANGE &&
            bgw_ptr->xy[0].disp.pos < bgw_ptr->pos_x_work + BREAK_ZOOM_RANGE) {
            bg_w.bg_f_x++;
            bg_w.bg_f_y++;

            if (bg_w.bg_f_x < BREAK_ZOOM) {
                Frame_Down(BREAK_ZOOM_X, BREAK_ZOOM_Y, 1);
            }
        }

        break;

    case 1:
        if (--bg_w.bridge.timer < 1) {
            bgw_ptr->r_no_1++;
        } else {
            bgw_ptr->xy[1].cal -= 0xB000;
            bgw_ptr->wxy[1].cal -= 0xB000;
        }

        break;

    case 2:
        if (bgw_ptr->wxy[1].cal < BREAK_END_Y_CAL) {
            bgw_ptr->r_no_1++;
            bg_w.bridge.timer = 0x3C;
        } else {
            bgw_ptr->xy[1].cal -= 0xB0000;
            bgw_ptr->wxy[1].cal -= 0xB0000;
        }

        break;

    case 3:
        if (--bg_w.bridge.timer < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 4:
        // NG runs the clouds' line scroll a second time here, so they drift twice as fast
        bg370_drift_clouds();
        break;
    }
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

bool bg370_is_on_bridge() {
    return bg_w.stage == STAGE_NG_ELENA && bg_w.area == 0;
}
