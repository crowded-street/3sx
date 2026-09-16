/**
 * @file eff09_animation.c
 * Effect 09 animation-driven phases.
 */

#include "sf33rd/Source/Game/effect/eff09_animation.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

extern const s16 eff09_data2[43][8];

static s32 eff09_9000_animation_updates_enabled(const WORK_Other* ewk) {
    return !EXE_flag && !Game_pause && ewk->wu.hit_stop;
}

static void start_eff09_animation_effect(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
}

static void place_eff09_animation_effect(WORK_Other* ewk) {
    if (ewk->wu.rl_flag) {
        ewk->wu.xyz[0].disp.pos -= 6;
    } else {
        ewk->wu.xyz[0].disp.pos -= 2;
    }

    ewk->wu.rl_flag = 0;
    ewk->wu.xyz[1].disp.pos += base_y_pos;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void advance_eff09_animation_step(WORK_Other* ewk) {
    if (eff09_9000_animation_updates_enabled(ewk)) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        }
    }
}

static void advance_eff09_animation(WORK_Other* ewk) {
    advance_eff09_animation_step(ewk);
    disp_pos_trans_entry_rs(ewk);
}

void eff09_9000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;

        place_eff09_animation_effect(ewk);
        break;

    case 1:
        if (eff09_9000_animation_updates_enabled(ewk)) {
            char_move(&ewk->wu);
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_10000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;

    place_eff09_animation_effect(ewk);
}

void eff09_10000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_10000(ewk);
        break;

    case 1:
        advance_eff09_animation(ewk);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}


void eff09_14000(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;

    if (ewk->wu.rl_flag) {
        ewk->wu.xyz[0].disp.pos = oya_ptr->xyz[0].disp.pos - eff09_data2[ewk->wu.type][2];
    } else {
        ewk->wu.xyz[0].disp.pos = oya_ptr->xyz[0].disp.pos + eff09_data2[ewk->wu.type][2];
    }

    ewk->wu.xyz[1].disp.pos = oya_ptr->xyz[1].disp.pos + eff09_data2[ewk->wu.type][3];
    ewk->wu.xyz[1].disp.pos += base_y_pos;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        start_eff09_animation_effect(ewk);
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        advance_eff09_animation(ewk);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff09_15000(WORK_Other* ewk) {
    WORK* oya_ptr;

    if (obr_no_disp_check()) {
        return;
    }

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        start_eff09_animation_effect(ewk);
        ewk->wu.rl_flag = 0;
        ewk->wu.xyz[1].disp.pos += base_y_pos;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (eff09_9000_animation_updates_enabled(ewk)) {
            char_move(&ewk->wu);

            if (oya_ptr->cg_type == 9) {
                ewk->wu.routine_no[1]++;
                ewk->wu.disp_flag = 0;
            }
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff09_16000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        start_eff09_animation_effect(ewk);
        ewk->wu.xyz[0].disp.pos += 2;
        ewk->wu.xyz[1].disp.pos += base_y_pos;
        ewk->wu.rl_flag = 0;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        advance_eff09_animation_step(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request4(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}
