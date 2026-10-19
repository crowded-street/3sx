/**
 * @file bg390.c
 * Oro's cave from New Generation (BG160 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg390.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff18.h"
#include "sf33rd/Source/Game/effect/ng/eff55.h"
#include "sf33rd/Source/Game/effect/ng/eff71.h"
#include "sf33rd/Source/Game/effect/ng/eff72.h"
#include "sf33rd/Source/Game/effect/ng/eff74.h"
#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

#include <stdbool.h>

#define INTRO_HEIGHT 0xC0

static void bg3900();
static void bg3901();
static void bg3902();
static void bg390_init_layer();
static void bg3901_init00();
static void bg390_move00();
static void bg3900_move01();
static void bg3901_move01();
static void bg3902_move01();
static void bg3900_win_move();
static void bg3901_win_move();
static void bg390_demo_check();

void BG390() {
    bgw_ptr = &bg_w.bgw[1];
    bg3901();
    bgw_ptr = &bg_w.bgw[0];
    bg3900();
    bgw_ptr = &bg_w.bgw[2];
    bg3902();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg3900() {
    void (*bg3900_jmp[3])() = { bg390_init_layer, bg390_move00, bg3900_move01 };

    if (win_sp_flag) {
        bg3900_win_move();
        return;
    }

    bg3900_jmp[bgw_ptr->r_no_0]();
}

static void bg390_init_layer() {
    bg390_demo_check();
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg3900_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg3901() {
    void (*bg3901_jmp[3])() = { bg3901_init00, bg390_move00, bg3901_move01 };

    if (win_sp_flag) {
        bg3901_win_move();
        return;
    }

    bg3901_jmp[bgw_ptr->r_no_0]();
}

static void bg3901_init00() {
    bg390_init_layer();
    effect_05_init();
    effect_06_init();
    effect_ng18_init(4); // Playful kitten
    effect_ng94_init_for_players(1);
    effect_ng55_init(); // Parrot
    effect_ng71_init(); // Dog
    effect_ng72_init(); // Cat and kitten
    effect_ng74_init(); // Bats
}

static void bg3901_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3902() {
    void (*bg3902_jmp[3])() = { bg390_init_layer, bg390_move00, bg3902_move01 };

    if (win_sp_flag) {
        bg3900_win_move();
        return;
    }

    bg3902_jmp[bgw_ptr->r_no_0]();
}

static void bg3902_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg390_demo_check() {
    if ((plw[0].player_number != CHAR_ORO && plw[1].player_number != CHAR_ORO) || bg_w.area != 0 ||
        Is_Training_Mode(Mode_Type)) {
        bgw_ptr->r_no_0 = 2;
        return;
    }

    bgw_ptr->r_no_0 = 1;
    bgw_ptr->xy[1].cal = bgw_ptr->wxy[1].cal = bgw_ptr->speed_y * INTRO_HEIGHT;
}

static void bg390_move00() {
    s16 oro_id;

    if (EXE_flag || Game_pause) {
        return;
    }

    switch (bgw_ptr->r_no_1) {
    case 0:
        if (plw[0].player_number == CHAR_ORO && plw[1].player_number == CHAR_ORO) {
            oro_id = Appear_hv[0] ? 1 : 0;
        } else {
            oro_id = plw[1].player_number == CHAR_ORO;
        }

        if (Appear_free[oro_id]) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 1:
        bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * 10;
        bgw_ptr->wxy[1].cal -= bgw_ptr->speed_y * 10;

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
            bgw_ptr->xy[1].cal = 0;
            bgw_ptr->wxy[1].cal = 0;
        }

        break;
    }
}

static bool bg390_win_pan() {
    const s32 target = bgw_ptr->speed_y * INTRO_HEIGHT;

    bgw_ptr->xy[1].cal += bgw_ptr->speed_y * 10;

    if (bgw_ptr->xy[1].cal >= target) {
        bgw_ptr->xy[1].cal = target;
    }

    bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal;
    return bgw_ptr->xy[1].cal == target;
}

static void bg3901_win_move() {
    if (EXE_flag || Game_pause) {
        return;
    }

    if (bgw_ptr->r_no_1 == 0 && win_sp_flag == 2 && bg390_win_pan()) {
        bgw_ptr->r_no_1++;
        win_sp_flag = 3;
    }
}

static void bg3900_win_move() {
    if (EXE_flag || Game_pause) {
        return;
    }

    if (win_sp_flag >= 2) {
        bg390_win_pan();
    }
}
