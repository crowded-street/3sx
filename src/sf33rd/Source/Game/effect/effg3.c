/**
 * @file effg3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effg3.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

static s32 game_is_active(void) {
    return EXE_flag == 0 && Game_pause == 0;
}

/* End the effect: park it in the state that releases the work slot, and stop
 * drawing it. State 1 reaches this from three different conditions. */
static void g3_stop(WORK_Other* ewk) {
    ewk->wu.routine_no[0] = 3;
    ewk->wu.disp_flag = 0;
}

/* Set the effect up against the player it follows. */
static void g3_start(WORK_Other* ewk, PLW* pwk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.routine_no[1] = 0;
    ewk->wu.disp_flag = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x2020;
    ewk->wu.position_y = pwk->wu.position_y - 8;
    ewk->wu.position_z = pwk->wu.position_z - 4;
    set_char_move_init(&ewk->wu, 0, 0);
}

/* Non-zero when the effect is done. Before the hand-off - routine_no[1] still
 * zero - it dies with its master and hands off once the master's timer runs
 * out; after it, it dies with its own animation. The hand-off itself happens
 * here, which is why this is not a pure predicate. */
static s32 g3_finished(WORK_Other* ewk) {
    WORK_Other* mwk;

    if (ewk->wu.dead_f || Suicide[6] != 0) {
        return 1;
    }

    if (!ewk->wu.routine_no[1]) {
        mwk = (WORK_Other*)ewk->my_master;

        if (mwk->wu.dead_f) {
            return 1;
        }

        if (mwk->wu.dir_timer <= 0) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, 1);
        }

        return 0;
    }

    if (ewk->wu.cg_type == 0xFF) {
        return 1;
    }

    return 0;
}

/* Hold station beside the player until the hand-off, then just animate. */
static void g3_track_player(WORK_Other* ewk, PLW* pwk) {
    s16 adjust;

    if (game_is_active()) {
        if (!ewk->wu.routine_no[1]) {
            adjust = 80;

            if (ewk->wu.rl_flag) {
                adjust = -adjust;
            }

            ewk->wu.position_x = pwk->wu.position_x + adjust;
        }

        char_move(&ewk->wu);
    }
}

void effect_G3_move(WORK_Other* ewk) {
    PLW* pwk = (PLW*)ewk->wu.target_adrs;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        g3_start(ewk, pwk);
        /* fallthrough */

    case 1:
        if (g3_finished(ewk)) {
            g3_stop(ewk);
            break;
        }

        g3_track_player(ewk, pwk);
        sort_push_request(&ewk->wu);
        break;

    default:
    case 2:
    case 3:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_G3_init(WORK* wk, u8 data) {
    WORK_Other* ewk;
    WORK_Other* ewk2;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.type = data;
    ewk->wu.id = 163;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = wk->my_family;
    ewk->wu.cgromtype = 1;
    ewk->my_master = wk;
    ewk->master_work_id = wk->work_id;
    ewk->master_id = wk->id;
    ewk2 = (WORK_Other*)wk;
    ewk->wu.target_adrs = ewk2->my_master;
    ewk->wu.rl_flag = data;
    *ewk->wu.char_table = _effD4_char_table;
    return 0;
}
