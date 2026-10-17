/**
 * @file bg340.c
 * Ibuki's stage from New Generation (BG110 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg340.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff82.h"
#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

#define INTRO_ZOOM 80
#define INTRO_ZOOM_X 192
#define INTRO_ZOOM_Y 224

static const s8 sway_frames[20] = { 0, 1, 2, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 1, 0, 1, 0, 1, 0, 1 };
static const s8 sway_times[20] = { 11, 11, 10, 9, 10, 11, 11, 10, 9, 9, 9, 9, 9, 10, 11, 11, 11, 11, 11, 11 };

static void bg3400();
static void bg3400_init00();
static void bg3400_move00();
static void bg3401();
static void bg3401_init00();
static void bg3401_move00();
static void bg3401_move01();
static void bg3402();
static void bg3402_init00();
static void bg3404();
static void bg3404_init00();
static void bg3404_move00();
static void bg340_init_layer();
static void bg340_move_layer();
static void bg340_sway_init();
static void bg340_sway_update();
static s8 bg340_sway_time();
static bool is_cpu_ibuki_present();

void BG340() {
    bgw_ptr = &bg_w.bgw[1];
    bg3401();
    bgw_ptr = &bg_w.bgw[0];
    bg3400();
    bgw_ptr = &bg_w.bgw[2];
    bg3402();
    bgw_ptr = &bg_w.bgw[4];
    bg3404();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg3400() {
    void (*bg3400_jmp[2])() = { bg3400_init00, bg3400_move00 };
    bg3400_jmp[bgw_ptr->r_no_0]();
}

static void bg3400_init00() {
    bg340_init_layer();
}

static void bg3400_move00() {
    bg340_move_layer();

    if (!EXE_flag && !Game_pause) {
        bg340_sway_update();
    }
}

static void bg3401() {
    void (*bg3401_jmp[3])() = { bg3401_init00, bg3401_move00, bg3401_move01 };
    bg3401_jmp[bgw_ptr->r_no_0]();
}

static void bg3401_init00() {
    bg340_init_layer();
    effect_05_init();
    effect_06_init();

    bg340_sway_init();

    switch (bg_w.area) {
    case 0:
        effect_ng82_init(); // Spectators

        if (is_cpu_ibuki_present()) {
            bg_w.bg_f_x = bg_w.bg_f_y = INTRO_ZOOM;
            Frame_Down(INTRO_ZOOM_X, INTRO_ZOOM_Y, INTRO_ZOOM - 64);
        } else {
            bgw_ptr->r_no_0++;
        }

        break;

    case 1:
        bgw_ptr->r_no_0++;
        // NG starts a second effect 48 for layer 1 here, which does the same as the first one.
        effect_ng82_init(); // Spectators
        // FIXME: NG effects 75 and 83
        effect_ng94_init_for_players(0);
        break;

    case 2:
        bgw_ptr->r_no_0++;
        effect_ng94_init_for_players(1);
        // FIXME: NG effects 83 and 20
        break;
    }

    // FIXME: NG effect 14 (variants 4-10)
}

static void bg3401_move00() {
    if (C_No[1] <= 3 && !Allow_a_battle_f) {
        return;
    }

    bg_w.bg_f_x--;
    bg_w.bg_f_y--;

    if (bg_w.bg_f_x > 64) {
        Frame_Up(INTRO_ZOOM_X, INTRO_ZOOM_Y, 1);
        return;
    }

    bgw_ptr->r_no_0++;
    Zoomf_Init();
    bg_w.bg_f_x = bg_w.bg_f_y = 64;
}

static void bg3401_move01() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3402() {
    void (*bg3402_jmp[2])() = { bg3402_init00, bg340_move_layer };
    bg3402_jmp[bgw_ptr->r_no_0]();
}

static void bg3402_init00() {
    bg340_init_layer();
}

static void bg3404() {
    void (*bg3404_jmp[2])() = { bg3404_init00, bg3404_move00 };
    bg3404_jmp[bgw_ptr->r_no_0]();
}

static void bg3404_init00() {
    bg340_init_layer();
    bgw_ptr->fam_no = 4;
    bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xA0;
    bgw_ptr->pos_y_work = 0;
    bgw_ptr->xy[1].disp.pos = 0;
    bgw_ptr->speed_x = 0xE000;
    bgw_ptr->speed_y = 0x10000;
    sync_fam_set3(4);
}

static void bg3404_move00() {
    bg_x_move_check();
    bg_y_move_check();
    sync_fam_set3(4);
}

static void bg340_init_layer() {
    bgw_ptr->r_no_0++;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;
}

static void bg340_move_layer() {
    bg_x_move_check();
    bg_y_move_check();
}

static bool is_cpu_ibuki_present() {
    for (int i = 0; i < 2; i++) {
        if (plw[i].player_number == CHAR_IBUKI && !plw[i].wu.operator) {
            return true;
        }
    }

    return false;
}

static s8 bg340_sway_time() {
    const s8 speed_up[3] = { 0, 2, 3 };
    return sway_times[bg_w.layer_frame_step] - speed_up[bg_w.area];
}

static void bg340_sway_init() {
    bg_w.layer_frame_step = 0;
    bg_w.layer_frame_timer = bg340_sway_time();
    bg_w.layer_frame = sway_frames[0];
}

static void bg340_sway_update() {
    bg_w.layer_frame_timer--;

    if (bg_w.layer_frame_timer != 0) {
        return;
    }

    bg_w.layer_frame_step++;

    if (bg_w.layer_frame_step >= SDL_arraysize(sway_frames)) {
        bg_w.layer_frame_step = 0;
    }

    bg_w.layer_frame_timer = bg340_sway_time();
    bg_w.layer_frame = sway_frames[bg_w.layer_frame_step];
}
