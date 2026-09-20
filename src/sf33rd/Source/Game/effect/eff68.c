/**
 * @file eff68.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff68.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static s32 game_is_active(void) {
    return !EXE_flag && !Game_pause;
}

static s32 animation_can_advance(void) {
    return !EXE_flag && !Game_pause;
}

/* Hold until the first timer runs out, then aim at the first waypoint. */
static void e68_wait_start(WORK_Other* ewk) {
    if (game_is_active()) {
        ewk->wu.routine_no[4]--;

        if (ewk->wu.routine_no[4] < 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.routine_no[4] = 50;
            cal_all_speed_data(
                &ewk->wu, &(Motion_Target) { ewk->wu.routine_no[4], ewk->wu.old_rno[2], ewk->wu.old_rno[3], 1, 1 }
            );
            ewk->wu.char_index = ewk->wu.routine_no[6];
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }
    }
}

/* Fly to the first waypoint, then aim at the second. */
static void e68_arc_out(WORK_Other* ewk) {
    if (animation_can_advance()) {
        ewk->wu.routine_no[4]--;

        if (ewk->wu.routine_no[4] < 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.routine_no[4] = 50;
            cal_delta_speed(
                &ewk->wu, &(Motion_Target) { ewk->wu.routine_no[4], ewk->wu.old_rno[4], ewk->wu.old_rno[5], 2, 2 }
            );
            ewk->wu.char_index = ewk->wu.routine_no[6];
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
    }
}

/* Fly to the second waypoint, then aim at the third. */
static void e68_arc_back(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.routine_no[4]--;

        if (ewk->wu.routine_no[4] < 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.routine_no[4] = 40;
            cal_all_speed_data(
                &ewk->wu, &(Motion_Target) { ewk->wu.routine_no[4], ewk->wu.old_rno[6], ewk->wu.old_rno[7], 1, 1 }
            );
        }

        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        char_move(&ewk->wu);
    }
}

/* Fly to the third waypoint, then aim back at the first. */
static void e68_return(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.routine_no[4]--;

        if (ewk->wu.routine_no[4] < 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.routine_no[4] = 60;
            cal_delta_speed(
                &ewk->wu, &(Motion_Target) { ewk->wu.routine_no[4], ewk->wu.old_rno[0], ewk->wu.old_rno[1], 2, 2 }
            );
            ewk->wu.char_index = ewk->wu.routine_no[5];
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        char_move(&ewk->wu);
    }
}

/* Fly home, then start the arc again from state 2. */
static void e68_loop(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.routine_no[4]--;

        if (ewk->wu.routine_no[4] < 1) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.routine_no[4] = 50;
            cal_all_speed_data(
                &ewk->wu, &(Motion_Target) { ewk->wu.routine_no[4], ewk->wu.old_rno[2], ewk->wu.old_rno[3], 1, 1 }
            );
            ewk->wu.char_index = ewk->wu.routine_no[6];
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
    }
}

void effect_68_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        disp_pos_trans_entry(ewk);
        break;

    case 1:
        e68_wait_start(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 2:
        e68_arc_out(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 3:
        e68_arc_back(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 4:
        e68_return(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 5:
        e68_loop(ewk);
        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

const s16 eff68_data_tbl[52] = { 2, 432, 192, 504, 180, 576, 172, 522, 160, 8,  9,  73, 0,
                                 2, 592, 208, 664, 196, 736, 188, 682, 176, 8,  9,  73, 20,
                                 3, 736, 160, 808, 148, 880, 140, 826, 128, 10, 11, 83, 50,
                                 3, 624, 192, 696, 180, 768, 172, 714, 160, 10, 11, 83, 40 };

s32 effect_68_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    const s16* data_ptr = eff68_data_tbl;

    for (i = 0; i < 3; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 68;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.dead_f = 1;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 8492;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
        ewk->wu.my_family = *data_ptr++;
        ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[0] = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[1] = *data_ptr++;
        ewk->wu.old_rno[2] = *data_ptr++;
        ewk->wu.old_rno[3] = *data_ptr++;
        ewk->wu.old_rno[4] = *data_ptr++;
        ewk->wu.old_rno[5] = *data_ptr++;
        ewk->wu.old_rno[6] = *data_ptr++;
        ewk->wu.old_rno[7] = *data_ptr++;
        ewk->wu.char_index = ewk->wu.routine_no[5] = *data_ptr++;
        ewk->wu.routine_no[6] = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
        ewk->wu.routine_no[4] = *data_ptr++;
        ewk->wu.sync_suzi = 0;
        ewk->wu.char_table[0] = _brz_char_table;
        suzi_offset_set(ewk);
    }

    return 0;
}
