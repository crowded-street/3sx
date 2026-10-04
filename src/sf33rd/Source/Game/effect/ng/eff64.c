/**
 * @file eff64.c
 * Moving stage objects from New Generation (effect 64 in the NG arcade ROM), such as animals running across the floor
 */

#include "sf33rd/Source/Game/effect/ng/eff64.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define DISPLAY_RANGE 272

typedef struct Eff64Data {
    s16 dead_f;

    /// 0–1 cross once, 2–3 repeat after the repeat wait, 4–7 repeat after a random wait. Odd behaviours move left.
    /// 8 is moved by its animation and isn't supported yet.
    s16 behaviour;

    s16 my_family;

    /// Converted from NG's stage palette rows, which start at 0x40, to the PS2 BG palette rows, which start at 0x12C
    s16 my_col_code;

    s16 x;
    s16 y;
    s16 priority;
    s16 char_index;
    s16 animate;
    s16 sync_suzi;
    s16 end_x;
    s16 initial_wait;
    s16 repeat_wait;
    s16 rl_flag;
    s32 speed_x;
    s32 accel_x;
    s32 speed_y;
    s32 accel_y;
} Eff64Data;

static const Eff64Data ng_eff64_data_tbl[5] = {
    // Stage 2
    { 1, 8, 2, 8516, 704, 76, 85, 11, 1, 0, -440, 32, 32, 0, -0x10000, 0, 0, 0 },
    { 1, 5, 2, 300, 656, 114, 102, 2, 1, 0, -64, 40, 0, 0, -0x10000, 0, 0, 0 },
    { 1, 4, 2, 300, 448, 115, 102, 3, 1, 0, 144, 48, 0, 0, 0x10000, 0, 0, 0 },

    // Alex's stage
    { 1, 4, 2, 8520, 320, 87, 85, 34, 1, 0, 400, 32, 32, 0, 0x10000, 0, 0, 0 },
    { 1, 5, 2, 8520, 912, 87, 86, 34, 1, 0, -192, 32, 32, 1, -0x10000, 0, 0, 0 },
};

static const s16 random_wait_tbl[16] = { 1428, 2120, 1472, 512,  1860, 392,  1728, 1280,
                                         576,  640,  2048, 1840, 960,  1024, 1216, 1536 };

static const s16 random_wait_tbl2[16] = { 2708, 4168, 4032, 4608, 5956, 8584, 5824, 5376,
                                          2112, 3968, 6144, 1840, 5056, 5120, 5312, 5632 };

/// @param keep_wait Whether to keep the current wait instead of the record's initial wait
static void eff64_setup(WORK_Other* ewk, bool keep_wait) {
    const Eff64Data* data = &ng_eff64_data_tbl[ewk->wu.type];

    ewk->wu.routine_no[2]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = data->dead_f;
    ewk->wu.routine_no[1] = data->behaviour;
    ewk->wu.my_family = data->my_family;
    ewk->wu.my_col_code = data->my_col_code;
    ewk->wu.xyz[0].disp.pos = data->x;
    ewk->wu.xyz[1].disp.pos = data->y;
    ewk->wu.my_priority = ewk->wu.position_z = data->priority;
    ewk->wu.char_index = data->char_index;
    ewk->wu.hit_stop = data->animate;
    ewk->wu.sync_suzi = data->sync_suzi;
    ewk->wu.old_rno[0] = data->end_x;

    if (!keep_wait) {
        ewk->wu.old_rno[1] = data->initial_wait;
    }

    ewk->wu.old_rno[2] = data->repeat_wait;
    ewk->wu.rl_flag = (s8)data->rl_flag;
    ewk->wu.mvxy.a[0].sp = data->speed_x;
    ewk->wu.mvxy.d[0].sp = data->accel_x;
    ewk->wu.mvxy.a[1].sp = data->speed_y;
    ewk->wu.mvxy.d[1].sp = data->accel_y;
    ewk->wu.char_table[0] = char_add[bg_w.bg_index];
    suzi_offset_set(ewk);
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void eff64_wait(WORK_Other* ewk) {
    if (--ewk->wu.old_rno[1] < 0) {
        ewk->wu.routine_no[2]++;
    }
}

/// Moves the object. Unlike add_x_sub and add_y_sub, NG accelerates before moving.
/// @return Whether the object has passed its end x
static bool eff64_step(WORK_Other* ewk) {
    if (ewk->wu.hit_stop) {
        char_move(&ewk->wu);
    }

    ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;
    ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
    ewk->wu.mvxy.a[1].sp += ewk->wu.mvxy.d[1].sp;
    ewk->wu.xyz[1].cal += ewk->wu.mvxy.a[1].sp;

    if (ewk->wu.routine_no[1] & 1) {
        return ewk->wu.xyz[0].disp.pos < ewk->wu.old_rno[0];
    }

    return ewk->wu.old_rno[0] < ewk->wu.xyz[0].disp.pos;
}

static bool is_paused() {
    return EXE_flag || Game_pause || EXE_obroll;
}

/// Like disp_pos_trans_entry_rs, but with NG's wider range.
static void eff64_disp(WORK_Other* ewk) {
    const BGW* bgw = &bg_w.bgw[ewk->wu.my_family - 1];
    const s16 bg_x = (bg_w.chase_flag & 0xF) ? bgw->chase_xy[0].disp.pos : bgw->wxy[0].disp.pos;
    const s16 x = ewk->wu.xyz[0].disp.pos;

    if (obr_no_disp_check() || x < bg_x - DISPLAY_RANGE || x > bg_x + DISPLAY_RANGE) {
        return;
    }

    suzi_sync_pos_set(ewk);
    ewk->wu.position_x++;
    sort_push_request4(&ewk->wu);
}

/// Crosses once
static void eff64_move_once(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        eff64_setup(ewk, false);
        break;

    case 1:
        eff64_wait(ewk);
        break;

    case 2:
        if (!is_paused() && eff64_step(ewk)) {
            ewk->wu.routine_no[0] = 2;
        }

        eff64_disp(ewk);
        break;
    }
}

/// Crosses again after the repeat wait, or after a random one
static void eff64_move_repeat(WORK_Other* ewk, bool random_wait) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        eff64_setup(ewk, false);
        ewk->wu.routine_no[2] = 2;
        break;

    case 1:
        eff64_setup(ewk, true);
        break;

    case 2:
        eff64_wait(ewk);
        break;

    case 3:
        if (!is_paused() && eff64_step(ewk)) {
            ewk->wu.routine_no[2] = 1;

            if (!random_wait) {
                ewk->wu.old_rno[1] = ewk->wu.old_rno[2];
            } else if (ewk->wu.routine_no[1] < 6) {
                ewk->wu.old_rno[1] = random_wait_tbl[random_16()];
            } else {
                ewk->wu.old_rno[1] = random_wait_tbl2[random_16()];
            }
        }

        eff64_disp(ewk);
        break;
    }
}

void effect_ng64_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
        case 1:
            eff64_move_once(ewk);
            break;

        case 2:
        case 3:
            eff64_move_repeat(ewk, false);
            break;

        case 4:
        case 5:
        case 6:
        case 7:
            eff64_move_repeat(ewk, true);
            break;

        default:
            // Behaviour 8 (0609f2ce in the NG ROM) needs step_xy_table support
            ewk->wu.routine_no[0]++;
            break;
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng64_init(s16 variant) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG64_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.type = variant;
    ewk->wu.routine_no[1] = ng_eff64_data_tbl[variant].behaviour;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
