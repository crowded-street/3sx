/**
 * @file bg380.c
 * Second and third areas of Elena's stage from New Generation (BG150 in the NG arcade ROM).
 */

#include "sf33rd/Source/Game/stage/ng/bg380.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/ng/eff85.h"
#include "sf33rd/Source/Game/effect/ng/eff87.h"
#include "sf33rd/Source/Game/effect/ng/eff88.h"
#include "sf33rd/Source/Game/effect/ng/effc3.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

#define ANIM_WATERFALL 0 // NG effect 14, variant 0
#define ANIM_BRIDGE 1

#define WATERFALL_FRAMES 12
#define WATERFALL_TIME 5

#define LANDING_QUAKE 0x20

static void bg3800();
static void bg3800_init00();
static void bg3800_move00();
static void bg3801();
static void bg3801_init00();
static void bg3801_move00();
static void bg3802();
static void waterfall_init();
static void waterfall_update();

void BG380() {
    bgw_ptr = &bg_w.bgw[1];
    bg3801();
    bgw_ptr = &bg_w.bgw[0];
    bg3800();
    bgw_ptr = &bg_w.bgw[2];
    bg3802();
    zoom_ud_check();
    bg_pos_hosei2();
    Bg_Family_Set();
}

static void bg3800() {
    void (*bg3800_jmp[2])() = { bg3800_init00, bg3800_move00 };
    bg3800_jmp[bgw_ptr->r_no_0]();
    waterfall_update();
}

static void bg3800_init00() {
    bgw_ptr->r_no_0++;
    bgw_ptr->r_no_1 = 0;
    bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
    bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
    bgw_ptr->zuubun = 0;

    waterfall_init();
    effect_ng87_init(4);
    effect_ng88_init(1);

    if (bg_w.object_set == 3) {
        effect_ng85_init(); // Sparring fighters
    }

    effect_05_init();
    effect_06_init();
}

static void bg3800_move00() {
    bg_x_move_check();
    bg_y_move_check();
}

static void bg3801() {
    void (*bg3801_jmp[2])() = { bg3801_init00, bg3801_move00 };
    bg3801_jmp[bgw_ptr->r_no_0]();
}

static void bg3801_init00() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        bg_w.bridge.landed = 0;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->position_y = bgw_ptr->wxy[1].disp.pos = bgw_ptr->xy[1].disp.pos = 0;
        bgw_ptr->zuubun = 0;

        if (bg_w.area == 1) {
            effect_ngc3_init(1);
            bg_w.layer_frame[ANIM_BRIDGE] = 1;
        } else {
            bgw_ptr->r_no_0++;
        }

        break;

    case 1:
        if (bg_w.bridge.landed) {
            bgw_ptr->r_no_1++;
            bg_w.layer_frame[ANIM_BRIDGE] = 0;
            bg_w.quake_y_index = LANDING_QUAKE;
            // FIXME: NG plays sound 0x77 here
        }

        break;

    case 2:
        if (bg_w.quake_y_index == 0) {
            bgw_ptr->r_no_0++;
        }

        break;
    }
}

static void bg3801_move00() {
    bg_base_x_move_check();
    bg_base_y_move_check();
    bg_chase_move();
}

static void bg3802() {
    switch (bgw_ptr->r_no_0) {
    case 0:
        bgw_ptr->r_no_0++;
        bgw_ptr->old_pos_x = bgw_ptr->xy[0].disp.pos = bgw_ptr->pos_x_work = 0x200;
        bgw_ptr->hos_xy[0].cal = bgw_ptr->wxy[0].cal = bgw_ptr->xy[0].cal;
        bgw_ptr->zuubun = 0;
        bgw_ptr->fam_no = 2;
        bgw_ptr->xy[1].disp.pos = bgw_ptr->pos_y_work = 0;
        bgw_ptr->speed_x = 0xC000;
        bgw_ptr->speed_y = 0xC000;
        bgw_ptr->y_limit = bgw_ptr->y_limit2 = 0xF0;
        break;

    case 1:
        bg_x_move_check();
        bg_y_move_check();
        break;
    }

    sync_fam_set3(2);
}

static void waterfall_init() {
    bg_w.layer_frame_step[ANIM_WATERFALL] = 0;
    bg_w.layer_frame_timer[ANIM_WATERFALL] = WATERFALL_TIME;
    bg_w.layer_frame[ANIM_WATERFALL] = 0;
}

static void waterfall_update() {
    // NG's effect 14 also stops while these backgrounds are up
    if (EXE_flag || Game_pause || aku_flag || akebono_flag == 1) {
        return;
    }

    if (--bg_w.layer_frame_timer[ANIM_WATERFALL] != 0) {
        return;
    }

    bg_w.layer_frame_step[ANIM_WATERFALL] = (bg_w.layer_frame_step[ANIM_WATERFALL] + 1) % WATERFALL_FRAMES;
    bg_w.layer_frame_timer[ANIM_WATERFALL] = WATERFALL_TIME;
    bg_w.layer_frame[ANIM_WATERFALL] = bg_w.layer_frame_step[ANIM_WATERFALL];
}

bool bg380_is_bridge_falling() {
    return bg_w.area == 1 && bg_index_tbl[bg_w.stage][bg_w.area] == AREA_NG_ELENA_B && bg_w.bgw[1].r_no_0 == 0;
}
