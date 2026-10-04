/**
 * @file bg230.c
 * Gill's stage from New Generation (BG000 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg230.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

/// Zoom of the screen at the start of the intro. 64 is no zoom.
#define INTRO_ZOOM 24

/// Point the intro zooms around
#define INTRO_ZOOM_X 16
#define INTRO_ZOOM_Y 208

s8 bg230_intro_wait;
s8 bg230_intro_skips;

static void bg2300();
static void bg2300_init00();
static void bg2300_move01();
static void bg2301();
static void bg2301_init00();
static void bg2301_move01();
static void bg2304();
static void bg230_move00();

void BG230() {
    bgw_ptr = &bg_w.bgw[1];
    bg2301();
    bgw_ptr = &bg_w.bgw[0];
    bg2300();
    bgw_ptr = &bg_w.bgw[4];
    bg2304();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg2300() {
    void (*bg2300_jmp[3])() = { bg2300_init00, bg230_move00, bg2300_move01 };
    bg2300_jmp[bgw_ptr->r_no_0]();
}

static void bg2300_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0x100;
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;

    // NG starts effect 12 here to animate the lava of this layer. 3S does it with rw231-rw235 instead.
    // TODO: Port NG effect 47. NG also preloads its CGs into sprite RAM here, which 3S doesn't need.
}

static void bg2300_move01() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg2301() {
    void (*bg2301_jmp[3])() = { bg2301_init00, bg230_move00, bg2301_move01 };
    bg2301_jmp[bgw_ptr->r_no_0]();
}

static void bg2301_init00() {
    bgw_ptr->r_no_0++;

    if (bg_w.area == 0) {
        bg_app = 1;
        bg230_intro_skips = 0;
        bg230_intro_wait = 10;
        bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0x100;
        bg_w.bg_f_x = bg_w.bg_f_y = INTRO_ZOOM;
        Frame_Up(INTRO_ZOOM_X, INTRO_ZOOM_Y, 64 - INTRO_ZOOM);
    } else {
        bgw_ptr->r_no_0++;
    }

    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;

    effect_05_init();
    effect_06_init();
    effect_ng94_init_for_players(0);
}

static void bg2301_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg2304() {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0++;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->pos_y_work = 0;
        bgw_ptr->zuubun = 0;
        bgw_ptr->fam_no = 4;
        bgw_ptr->xy[0].disp.low = 0;
        bgw_ptr->xy[1].cal = 0;
        bgw_ptr->speed_x = 0xC000;
        bgw_ptr->speed_y = 0xF000;

        if (bg_w.area == 0) {
            bgw_ptr->hos_xy[1].disp.pos = bgw_ptr->wxy[1].cal = bgw_ptr->xy[1].cal = bgw_ptr->speed_y * 0x100;
        } else {
            bgw_ptr->r_no_0++;
        }

        break;

    case 1:
        bg230_move00();
        sync_fam_set3(4);
        break;

    case 2:
        bg_x_move_check();
        bg_y_move_check();
        sync_fam_set3(4);
        break;
    }
}

/// Whether an operating player has just pressed an attack button
static bool is_attack_pressed() {
    if (plw[0].wu.operator && (~p1sw_1 & p1sw_0 & SWK_ATTACKS)) {
        return true;
    }

    if (plw[1].wu.operator && (~p2sw_1 & p2sw_0 & SWK_ATTACKS)) {
        return true;
    }

    return false;
}

/// Scrolls the layer down into place and zooms out during the intro. Layer 1 drives the timer and the zoom.
/// Three attack presses skip the intro.
static void bg230_move00() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        if (bgw_ptr->fam_no == 1) {
            bg230_intro_wait--;
        }

        if (bg230_intro_wait == 0) {
            bgw_ptr->r_no_1++;
        }

        break;

    case 1:
        if (bgw_ptr->fam_no == 1) {
            if (is_attack_pressed()) {
                bg230_intro_skips++;
            }

            if (bg_w.bg_f_x != 64) {
                bg_w.bg_f_x++;
                bg_w.bg_f_y++;

                if (bg_w.bg_f_x < 64) {
                    Frame_Down(INTRO_ZOOM_X, INTRO_ZOOM_Y, 1);
                } else {
                    bg_w.bg_f_x = bg_w.bg_f_y = 64;
                    Zoomf_Init();
                }
            }
        }

        if (bg230_intro_skips >= 3) {
            bgw_ptr->xy[1].cal = bgw_ptr->pos_y_work;
            bgw_ptr->r_no_0++;
            bg_app = 0;
            bgw_ptr->r_no_1 = 0;
            Zoomf_Init();
            bg_w.bg_f_x = bg_w.bg_f_y = 64;
            break;
        }

        if (Cut_Cut_Cut()) {
            bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * 4;
        } else {
            bgw_ptr->xy[1].cal -= bgw_ptr->speed_y * 2;
        }

        if (bgw_ptr->xy[1].cal < bgw_ptr->pos_y_work) {
            Zoomf_Init();
            bgw_ptr->xy[1].cal = bgw_ptr->pos_y_work;
            bgw_ptr->r_no_0++;
            bgw_ptr->r_no_1 = 0;
            bg_app = 0;
        }

        break;
    }
}
