/**
 * @file bg280.c
 * Yun's stage from New Generation (BG050 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg280.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff18.h"
#include "sf33rd/Source/Game/effect/ng/eff23.h"
#include "sf33rd/Source/Game/effect/ng/eff87.h"
#include "sf33rd/Source/Game/effect/ng/effc1.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static void bg2800();
static void bg2800_init00();
static void bg2800_move01();
static void bg2801();
static void bg2801_init00();
static void bg2801_move01();
static void bg2802();
static void bg2802_init00();
static void bg2802_move01();
static void bg280_move00();
static bool bg280_demo_check();

void BG280() {
    bgw_ptr = &bg_w.bgw[1];
    bg2801();
    bgw_ptr = &bg_w.bgw[0];
    bg2800();
    bgw_ptr = &bg_w.bgw[2];
    bg2802();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg2800() {
    void (*bg2800_jmp[3])() = { bg2800_init00, bg280_move00, bg2800_move01 };
    bg2800_jmp[bgw_ptr->r_no_0]();
}

static void bg2800_init00() {
    bgw_ptr->r_no_1 = 0;
    bgw_ptr->zuubun = 0;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->hos_xy[0].disp.pos = bgw_ptr->wxy[0].disp.pos - bg_w.pos_offset;
    bgw_ptr->hos_xy[0].disp.low = 0;

    if (bg280_demo_check()) {
        bgw_ptr->r_no_0 = 2;
    } else {
        bgw_ptr->r_no_0++;
        bgw_ptr->free = 52;
        bgw_ptr->xy[1].cal = bgw_ptr->wxy[1].cal = 0xC40000;
    }
}

static void bg2800_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg2801() {
    void (*bg2801_jmp[3])() = { bg2801_init00, bg280_move00, bg2801_move01 };
    bg2801_jmp[bgw_ptr->r_no_0]();
}

static void bg2801_init00() {
    bgw_ptr->r_no_1 = 0;
    bgw_ptr->zuubun = 0;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->hos_xy[0].disp.pos = bgw_ptr->wxy[0].disp.pos - bg_w.pos_offset;
    bgw_ptr->hos_xy[0].disp.low = 0;

    if (bg280_demo_check()) {
        bgw_ptr->r_no_0 = 2;
    } else {
        bgw_ptr->r_no_0++;
        bgw_ptr->free = 52;
        bgw_ptr->xy[1].cal = bgw_ptr->wxy[1].cal = 0xC40000;
    }

    effect_05_init();
    effect_06_init();

    if (bg_w.area == 0) {
        effect_ng18_init(2); // Woman with a grocery bag and scared girls
        effect_ng87_init(2); // Men sitting at the table and merchant
        effect_ng23_init();  // Bird seller
        effect_ngc1_init_for_players();
    }

    // FIXME: NG also starts effects 25(0, 1, 2), 29 and 60(0) in the other areas
}

static void bg2801_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg2802() {
    void (*bg2802_jmp[3])() = { bg2802_init00, bg280_move00, bg2802_move01 };

    if (bg_w.area == 0) {
        bg2802_jmp[bgw_ptr->r_no_0]();
    }
}

static void bg2802_init00() {
    bgw_ptr->r_no_1 = 0;
    bgw_ptr->zuubun = 0;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->hos_xy[0].disp.pos = bgw_ptr->wxy[0].disp.pos - bg_w.pos_offset;
    bgw_ptr->hos_xy[0].disp.low = 0;

    if (bg280_demo_check()) {
        bgw_ptr->r_no_0 = 2;
    } else {
        bgw_ptr->r_no_0++;
        bgw_ptr->free = 52;
        bgw_ptr->xy[1].cal = bgw_ptr->wxy[1].cal = 0xC40000;
    }
}

static void bg2802_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

/// Waits, then scrolls the layer down into place during the intro
static void bg280_move00() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->free--;

        if (bgw_ptr->free < 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 1:
        bgw_ptr->xy[1].cal -= 0xA8000;
        bgw_ptr->wxy[1].cal -= 0xA8000;

        if (bgw_ptr->xy[1].disp.pos <= 0) {
            bgw_ptr->r_no_1++;
            bgw_ptr->xy[1].cal = 0;
            bgw_ptr->wxy[1].cal = 0;
        }

        break;

    case 2:
        if (Appear_end == 2) {
            bgw_ptr->r_no_0++;
            bgw_ptr->r_no_1 = 0;
        }

        break;
    }
}

/// Whether the intro is skipped. It only plays in area 0 when one of the players is Yun.
static bool bg280_demo_check() {
    if (bg_w.area != 0) {
        return true;
    }

    return (plw[0].player_number != CHAR_YUN) && (plw[1].player_number != CHAR_YUN);
}
