/**
 * @file effc1.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc1.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static s32 game_is_active(void) {
    return !EXE_flag && !Game_pause;
}

/* Where the effect is aimed: at the master itself for char_index 67, otherwise
 * to the side it faces. */
static s16 c1_target_x(const WORK* oya_ptr) {
    if (oya_ptr->char_index == 67) {
        return oya_ptr->xyz[0].disp.pos;
    }

    if (oya_ptr->rl_flag) {
        return oya_ptr->xyz[0].disp.pos + 74;
    }

    return oya_ptr->xyz[0].disp.pos - 74;
}

/* Show the effect, give it a shadow and send it towards its target. */
static void c1_start(WORK_Other* ewk, WORK* oya_ptr) {
    ewk->wu.routine_no[0]++;
    ewk->wu.disp_flag = 1;
    setup_shadow_of_the_Effy(&ewk->wu);
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    ewk->wu.old_rno[0] = 64;

    cal_all_speed_data(
        &ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], c1_target_x(oya_ptr), ewk->wu.xyz[1].disp.pos, 2, 2 }
    );
}

/* Travel until the effect is close enough to the master, then sound it and
 * start the depth move. */
static void c1_approach(WORK_Other* ewk, WORK* oya_ptr) {
    s16 work;

    if (game_is_active()) {
        ewk->wu.old_rno[0]--;
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        work = ewk->wu.xyz[0].disp.pos - oya_ptr->xyz[0].disp.pos;

        if (work < 0) {
            work = -work;
        }

        if (work < 0x91) {
            ewk->wu.routine_no[0]++;
            Sound_SE((ewk->master_id * 0x300) + 0x15E);
            char_move_z(&ewk->wu);
        }
    }
}

/* Keep moving while the timer runs, then switch to the finishing animation -
 * a different one for char_index 67. */
static void c1_close_in(WORK_Other* ewk, WORK* oya_ptr) {
    if (!EXE_flag && !Game_pause) {
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[0]++;

            if (oya_ptr->char_index == 67) {
                set_char_move_init(&ewk->wu, 0, 37);
            } else {
                set_char_move_init(&ewk->wu, 0, 38);
            }
        }
    }
}

/* Run the finishing animation out. */
static void c1_finish(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        char_move(&ewk->wu);
    }
}

/* Place the effect and hand it to the renderer - the tail states 1 to 3 share. */
static void c1_sync_and_push(WORK_Other* ewk) {
    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);
}

void effect_C1_move(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        c1_start(ewk, oya_ptr);
        break;

    case 1:
        c1_approach(ewk, oya_ptr);
        c1_sync_and_push(ewk);
        break;

    case 2:
        c1_close_in(ewk, oya_ptr);
        c1_sync_and_push(ewk);
        break;

    case 3:
        c1_finish(ewk);
        c1_sync_and_push(ewk);
        break;
    }
}

s32 effect_C1_init(WORK* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 121;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code + 6;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = wk;
    ewk->wu.rl_flag = wk->rl_flag;

    if (wk->rl_flag) {
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + (bg_w.pos_offset + 16);
    } else {
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos - (bg_w.pos_offset + 16);
    }

    ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos - 16;
    ewk->wu.my_priority = wk->my_priority - 12;
    ewk->wu.position_z = ewk->wu.my_priority - 12;
    *ewk->wu.char_table = _etc2_char_table;
    ewk->wu.char_index = 36;
    ewk->wu.sync_suzi = 0;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
