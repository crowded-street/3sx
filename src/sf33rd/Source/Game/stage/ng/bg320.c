/**
 * @file bg320.c
 * Necro's stage from New Generation (BG090 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg320.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff07.h"
#include "sf33rd/Source/Game/effect/ng/eff18.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"

#define INTRO_START_LEFT 250
#define INTRO_START_RIGHT 776
#define LAYER_2_SPEED 5

static void bg320_init00();
static void bg320_move00();
static void bg3200();
static void bg3200_init01();
static void bg3200_move01();
static void bg3201();
static void bg3201_init01();
static void bg3201_move01();
static void bg3202();
static void bg3202_init01();
static void bg3202_move01();
static bool bg320_demo_check();

void BG320() {
    bgw_ptr = &bg_w.bgw[1];
    bg3201();
    bgw_ptr = &bg_w.bgw[0];
    bg3200();
    bgw_ptr = &bg_w.bgw[2];
    bg3202();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg3200() {
    void (*bg3200_jmp[4])() = { bg320_init00, bg3200_init01, bg320_move00, bg3200_move01 };
    bg3200_jmp[bgw_ptr->r_no_0]();
}

static void bg3200_init01() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bg_app = 1;

        if (bg320_demo_check()) {
            bg_w.intro_direction = 1;
            bgw_ptr->xy[0].disp.pos = INTRO_START_LEFT;
        } else {
            bg_w.intro_direction = 0;
            bgw_ptr->xy[0].disp.pos = INTRO_START_RIGHT;
        }

        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->zuubun = 0;
    effect_05_init();
    effect_06_init();
    effect_ng07_init();
    effect_ng18_init(5);
}

static void bg3200_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg3201() {
    void (*bg3201_jmp[4])() = { bg320_init00, bg3201_init01, bg320_move00, bg3201_move01 };
    bg3201_jmp[bgw_ptr->r_no_0]();
}

static void bg3201_init01() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->xy[0].disp.pos = bg320_demo_check() ? INTRO_START_LEFT : INTRO_START_RIGHT;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->zuubun = 0;
}

static void bg3201_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3202() {
    void (*bg3202_jmp[3])() = { bg320_init00, bg3202_init01, bg3202_move01 };
    bg3202_jmp[bgw_ptr->r_no_0]();
}

static void bg3202_init01() {
    bgw_ptr->r_no_0++;
}

static void bg3202_move01() {
    if (!EXE_flag && !Game_pause) {
        const s16 delta = bg_w.bgw[1].wxy[0].disp.pos - bg_w.bgw[1].old_pos_x;

        // The layer only follows the camera when it moves left, which keeps the mountains moving left
        if (delta < 0) {
            bgw_ptr->xy[0].disp.pos += delta;
            bgw_ptr->wxy[0].disp.pos += delta;
        }

        bgw_ptr->xy[0].disp.pos -= LAYER_2_SPEED;
        bgw_ptr->wxy[0].disp.pos -= LAYER_2_SPEED;
    }

    bg_y_move_check();
}

static void bg320_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
}

static void bg320_move00() {
    const int speed = Cut_Cut_Cut() ? 4 : 2;

    if (bg_w.intro_direction == 0) {
        bgw_ptr->wxy[0].cal -= bgw_ptr->speed_x * speed;

        if (bgw_ptr->wxy[0].disp.pos >= 0x200) {
            return;
        }
    } else {
        bgw_ptr->wxy[0].cal += bgw_ptr->speed_x * speed;

        if (bgw_ptr->wxy[0].disp.pos <= 0x200) {
            return;
        }
    }

    bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal = 0x200 << 16;
    bgw_ptr->r_no_0++;
    bg_app = 0;
}

static bool bg320_demo_check() {
    if (plw[0].player_number == CHAR_NECRO && plw[1].player_number == CHAR_NECRO) {
        return !Appear_hv[0];
    }

    return !(plw[0].player_number != CHAR_NECRO && plw[1].player_number == CHAR_NECRO);
}
