/**
 * @file effm5.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effm5.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effm6.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static s32 game_is_active(void) {
    return !EXE_flag && !Game_pause;
}

/* The car appears once the round is running: show it, start its animation and
 * play its engine sound. */
static void m5_start_car(WORK_Other* ewk) {
    if (game_is_active()) {
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        Sound_SE(ewk->master_id * 0x300 + 0x134);
    }
}

/* Drive in until the timer runs out, then stop the car and - in the demo - pan
 * the sound. */
static void m5_drive_in(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        char_move(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 1) {
            ewk->wu.routine_no[0]++;
            Appear_car_stop[ewk->master_id] = 1;
            set_char_move_init(&ewk->wu, 0, 0x68);

            if (Demo_Flag != 0) {
                SsRequestPan(0x135, 0x40, 0x40, 0, 2);
            }
        } else {
            add_x_sub(&ewk->wu);
        }
    }
}

/* Hold while the stop animation plays; cel type 1 ends it, cel type 2 is where
 * the demo takes the car over. */
static void m5_wait_for_stop(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.old_rno[0] = 0x14;
        } else if (ewk->wu.cg_type == 2) {
            demo_car_flag[ewk->master_id] = 1;
        }
    }
}

/* Wait out the pause, then set the car moving again in its facing direction. */
static void m5_begin_drive_off(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.old_rno[0] = 0x30;

            if (ewk->wu.rl_flag) {
                ewk->wu.mvxy.a[0].sp = -0x20000;
                ewk->wu.mvxy.d[0].sp = -0x1000;
            } else {
                ewk->wu.mvxy.a[0].sp = 0x20000;
                ewk->wu.mvxy.d[0].sp = 0x1000;
            }
        }
    }
}

/* Drive off until the timer runs out. */
static void m5_drive_off(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[0]++;
        } else {
            add_x_sub(&ewk->wu);
        }
    }
}

/* Hide the car and release the demo's hold on it. */
static void m5_finish(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    demo_car_flag[ewk->master_id] = 0;
    ewk->wu.disp_flag = 0;
}

/* The states after the car has driven off: hide it, idle one frame, then hand
 * the work slot back. The case labels are the original ones. */
static void m5_teardown(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 5:
        m5_finish(ewk);
        break;

    case 6:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void effect_M5_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        m5_start_car(ewk);
        break;

    case 1:
        m5_drive_in(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        m5_wait_for_stop(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        m5_begin_drive_off(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 4:
        m5_drive_off(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    default:
        m5_teardown(ewk);
        break;
    }
}

s32 effect_M5_init(PLW* oya) {
    WORK_Other* ewk;
    s16 ix;
    s16 work;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    demo_car_flag[oya->wu.id] = 0;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 225;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.disp_flag = 0;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = 103;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_priority = ewk->wu.position_z = 57;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.my_col_code = oya->wu.my_col_code + 4;
    ewk->wu.sync_suzi = 0;
    ewk->master_id = oya->wu.id;

    if (oya->wu.id) {
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.xyz[1].cal = 0;
        ewk->wu.rl_flag = 0;
        ewk->wu.old_rno[0] = 40;
        work = (bg_w.bgw[1].pos_x_work + 168) & 0xFFFF;
        ewk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work + 320) & 0xFFFF;
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], work, 0, 1, 1 });
    } else {
        ewk->wu.xyz[1].cal = 0;
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.rl_flag = 1;
        ewk->wu.old_rno[0] = 40;
        work = (bg_w.bgw[1].pos_x_work - 168) & 0xFFFF;
        ewk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work - 320) & 0xFFFF;
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], work, 0, 1, 1 });
    }

    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    effect_M6_init(ewk);
    return 0;
}
