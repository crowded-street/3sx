/**
 * @file eff30.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff30.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static s32 game_is_active(void) {
    return !EXE_flag && !Game_pause;
}


/* Non-zero when the effect's animation has reached the given cel. The frame
 * advance is inside the test, and behind the pause checks, exactly as the
 * original comma expression had it - char_move only runs when the game is
 * running. */
static s32 e30_animation_reached(WORK_Other* ewk, s16 cg_type) {
    return !EXE_flag && !Game_pause && (char_move(&ewk->wu), ewk->wu.cg_type == cg_type);
}

/* Show the effect with its shadow and send it on its arc. */
static void e30_start(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 0;
    ewk->wu.kage_hy = -10;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 16;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    ewk->wu.old_rno[0] = 80;
    cal_initial_speed(&ewk->wu, ewk->wu.old_rno[0], ewk->wu.old_rno[1], ewk->wu.xyz[1].disp.pos);
}

/* Fly the arc out, then switch to the landing animation. */
static void e30_fly(WORK_Other* ewk) {
    if (game_is_active()) {
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 1) {
            ewk->wu.routine_no[0]++;
            set_char_move_init(&ewk->wu, 0, 1);
        }
    }
}

/* Run the landing animation to its end, then start the settle. */
static void e30_land(WORK_Other* ewk) {
    if (e30_animation_reached(ewk, 0xFF)) {
        ewk->wu.routine_no[0]++;
        set_char_move_init(&ewk->wu, 0, 2);
    }
}

/* Nudge the shadow across once the settle reaches its marked cel. */
static void e30_settle(WORK_Other* ewk) {
    if (e30_animation_reached(ewk, 10)) {
        ewk->wu.cg_type = 0;
        ewk->wu.kage_hx -= 4;
    }
}

/* Place the effect and hand it to the renderer - the tail states 1 to 3 share. */
static void e30_sync_and_push(WORK_Other* ewk) {
    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);
}

void effect_30_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        e30_start(ewk);
        break;

    case 1:
        e30_fly(ewk);
        e30_sync_and_push(ewk);
        break;

    case 2:
        e30_land(ewk);
        e30_sync_and_push(ewk);
        break;

    case 3:
        e30_settle(ewk);
        e30_sync_and_push(ewk);
        break;

    case 4:
        ewk->wu.disp_flag = 0;
        ewk->wu.kage_flag = 0;
        ewk->wu.routine_no[0]++;
        break;

    case 5:
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_30_init(WORK* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 30;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code + 1;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = wk;
    ewk->wu.rl_flag = wk->rl_flag;

    if (wk->rl_flag) {
        if (wk->xyz[0].disp.pos < bg_w.bgw[1].wxy[0].disp.pos) {
            ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos - 256;
        } else {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos - (bg_w.pos_offset + 32);
        }

        ewk->wu.old_rno[1] = wk->xyz[0].disp.pos - 32;
    } else {
        if (wk->xyz[0].disp.pos > bg_w.bgw[1].wxy[0].disp.pos) {
            ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos + 256;
        } else {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + (bg_w.pos_offset + 32);
        }

        ewk->wu.old_rno[1] = wk->xyz[0].disp.pos + 32;
    }

    ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos - 12;
    ewk->wu.my_priority = 28;
    ewk->wu.position_z = 28;
    ewk->wu.char_table[0] = _etc3_char_table;
    ewk->wu.char_index = 0;
    ewk->wu.sync_suzi = 0;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
