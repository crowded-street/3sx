/**
 * @file bg300.c
 * Dudley's stage from New Generation (BG070 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/stage/ng/bg300.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff65.h"
#include "sf33rd/Source/Game/effect/ng/eff66.h"
#include "sf33rd/Source/Game/effect/ng/eff68.h"
#include "sf33rd/Source/Game/effect/ng/eff94.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

static void bg3000();
static void bg3000_init00();
static void bg3000_move00();
static void bg3001();
static void bg3001_init00();
static void bg3001_move00();
static void bg3002();
static void bg3002_init00();
static void bg3002_move00();
static void bg300_rain_init();
static void bg300_rain_update();

typedef struct RainStep {
    s8 frame;
    s8 time;
} RainStep;

static const RainStep rain_variants[8][4] = {
    { { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 } }, { { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 } },
    { { 0, 7 }, { 1, 7 }, { 2, 7 }, { 3, 7 } }, { { 0, 7 }, { 1, 7 }, { 2, 7 }, { 3, 7 } },
    { { 4, 4 }, { 5, 4 }, { 6, 4 }, { 7, 4 } }, { { 4, 5 }, { 5, 5 }, { 6, 5 }, { 7, 5 } },
    { { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 } }, { { 4, 7 }, { 5, 7 }, { 6, 7 }, { 7, 7 } },
};

static const s8 rain_sequence[14][2] = { { 8, 1 },  { 20, 2 }, { 20, 3 }, { 24, 4 }, { 20, 5 }, { 20, 6 }, { 20, 7 },
                                         { 20, 6 }, { 20, 5 }, { 24, 4 }, { 20, 3 }, { 20, 2 }, { 20, 1 }, { 20, 0 } };

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
        bg300_rain_init();
        effect_ng65_init(); // Horse-drawn carriage
        effect_ng94_init_for_players(0);
    } else {
        effect_ng94_init_for_players(1);
        effect_ng66_init(0); // Squatting punk
        effect_ng68_init();  // Pacing punk
    }

    effect_05_init();
    effect_06_init();
}

static void bg3000_move00() {
    bg_x_move_check();
    bg_y_move_check();

    if (bg_w.area == 0 && !EXE_flag && !Game_pause) {
        bg300_rain_update();
    }
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

static const RainStep* bg300_rain_step() {
    return &rain_variants[rain_sequence[bg_w.layer_frame_sequence][1]][bg_w.layer_frame_step[0]];
}

static void bg300_rain_init() {
    bg_w.layer_frame_sequence = 0;
    bg_w.layer_frame_cycles = rain_sequence[0][0];
    bg_w.layer_frame_step[0] = 0;
    bg_w.layer_frame_timer[0] = bg300_rain_step()->time;
    bg_w.layer_frame[0] = bg300_rain_step()->frame;
}

static void bg300_rain_update() {
    bg_w.layer_frame_timer[0]--;

    if (bg_w.layer_frame_timer[0] != 0) {
        return;
    }

    bg_w.layer_frame_step[0]++;

    if (bg_w.layer_frame_step[0] > 3) {
        bg_w.layer_frame_step[0] = 0;
        bg_w.layer_frame_cycles--;

        if (bg_w.layer_frame_cycles == 0) {
            bg_w.layer_frame_sequence++;

            if (bg_w.layer_frame_sequence >= SDL_arraysize(rain_sequence)) {
                bg_w.layer_frame_sequence = 0;
            }

            bg_w.layer_frame_cycles = rain_sequence[bg_w.layer_frame_sequence][0];
        }
    }

    bg_w.layer_frame_timer[0] = bg300_rain_step()->time;
    bg_w.layer_frame[0] = bg300_rain_step()->frame;
}
