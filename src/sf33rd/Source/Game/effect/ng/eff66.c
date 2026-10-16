/**
 * @file eff66.c
 * People on Dudley's stage from New Generation who react to the fighters
 * Variant 0 is a punk squatting in the second area.
 * Variant 1 is a man with a cane (unfinished)
 */

#include "sf33rd/Source/Game/effect/ng/eff66.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

typedef enum Eff66Behaviour {
    /// Reacts whenever a fighter's drawn position is near his
    EFF66_NEAR_DRAWN,

    /// Reacts once when a fighter is near his position, then waits until he's far off screen
    EFF66_PASS_BY = 3,
} Eff66Behaviour;

typedef struct Eff66Variant {
    s16 dead_f;
    s16 my_family;
    s16 my_col_code; // Converted from NG's stage palette rows, which start at 0x40, to the PS2 BG rows from 0x12C
    s16 x;
    s16 y;
    s16 priority;
    s16 idle_char;
    bool animate_idle;
    s16 sync_suzi;
    Eff66Behaviour behaviour;
    s16 reaction_char;
    bool range_check; // Whether it's only drawn near the screen
} Eff66Variant;

static const Eff66Variant eff66_variants[2] = {
    { 1, 2, 0x2144, 208, 96, 76, 15, true, 1, EFF66_NEAR_DRAWN, 16, true }, // NG 0x2058
    { 1, 2, 0x2146, -400, 72, 75, 11, true, 1, EFF66_PASS_BY, 12, false },  // NG 0x205A
};

#define NEAR_DRAWN_RANGE 32
#define PASS_BY_RANGE 16
#define PASS_BY_RESET_DISTANCE 0x120

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static bool is_near(s16 a, s16 b, s16 range) {
    s16 distance = a - b;

    if (distance < 0) {
        distance = -distance;
    }

    return distance < range;
}

static const Eff66Variant* get_variant(WORK_Other* ewk) {
    return &eff66_variants[ewk->wu.old_rno[3]];
}

static void eff66_disp(WORK_Other* ewk) {
    if (get_variant(ewk)->range_check && !range_x_check_ng(ewk)) {
        return;
    }

    if (obr_no_disp_check()) {
        return;
    }

    suzi_sync_pos_set(ewk);
    sort_push_request4(&ewk->wu);
}

static void eff66_near_drawn(WORK_Other* ewk) {
    const Eff66Variant* variant = get_variant(ewk);

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, variant->idle_char);
        break;

    case 1:
        if (variant->animate_idle) {
            char_move(&ewk->wu);
        }

        if (is_near(plw[0].wu.position_x, ewk->wu.position_x, NEAR_DRAWN_RANGE) ||
            is_near(plw[1].wu.position_x, ewk->wu.position_x, NEAR_DRAWN_RANGE)) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, variant->reaction_char);
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[2] = 0;
        }

        break;
    }
}

static void eff66_pass_by(WORK_Other* ewk) {
    const Eff66Variant* variant = get_variant(ewk);
    const s16 x = ewk->wu.xyz[0].disp.pos;
    s16 layer_x;

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, variant->idle_char);
        break;

    case 1:
        char_move(&ewk->wu);

        if (is_near(plw[0].wu.xyz[0].disp.pos, x, PASS_BY_RANGE) ||
            is_near(plw[1].wu.xyz[0].disp.pos, x, PASS_BY_RANGE)) {
            ewk->wu.routine_no[2]++;
            set_char_move_init(&ewk->wu, 0, variant->reaction_char);
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[2]++;
        }

        break;

    case 3:
        layer_x = bg_w.bgw[ewk->wu.my_family - 1].xy[0].disp.pos;

        if (x > layer_x + PASS_BY_RESET_DISTANCE || x < layer_x - PASS_BY_RESET_DISTANCE) {
            ewk->wu.routine_no[2] = 0;
        }

        break;
    }
}

void effect_ng66_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[2] = 0;
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if (!is_paused()) {
            switch (get_variant(ewk)->behaviour) {
            case EFF66_NEAR_DRAWN:
                eff66_near_drawn(ewk);
                break;

            case EFF66_PASS_BY:
                eff66_pass_by(ewk);
                break;
            }
        }

        eff66_disp(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng66_init(s16 variant_index) {
    const Eff66Variant* variant = &eff66_variants[variant_index];
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG66_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.old_rno[3] = variant_index;
    ewk->wu.dead_f = variant->dead_f;
    ewk->wu.my_family = variant->my_family;
    ewk->wu.my_col_code = variant->my_col_code;
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = variant->x;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = variant->y;
    ewk->wu.my_priority = ewk->wu.position_z = variant->priority;
    ewk->wu.sync_suzi = variant->sync_suzi;
    ewk->wu.char_table[0] = _ng_eng_char_table;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
