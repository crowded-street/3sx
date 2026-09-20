/**
 * @file effc5.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc5.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effc6.h"
#include "sf33rd/Source/Game/effect/effect.h"
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

static void display_car_C5(WORK_Other* ewk) {
    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);
}

static void initialize_car_C5(WORK_Other* ewk) {
    if (game_is_active()) {
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        Sound_SE(ewk->master_id * 0x300 + 0x134);
    }
}

static void approach_car_C5(WORK_Other* ewk) {
    if (game_is_active()) {
        char_move(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 1) {
            ewk->wu.routine_no[0]++;
            Appear_car_stop[ewk->master_id] = 1;
            set_char_move_init(&ewk->wu, 0, 9);

            if (Demo_Flag != 0) {
                SsRequestPan(0x135, 0x40, 0x40, 0, 2);
            }
        } else {
            add_x_sub(&ewk->wu);
        }
    }

    display_car_C5(ewk);
}

static void idle_car_C5(WORK_Other* ewk) {
    if (game_is_active()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.old_rno[0] = 20;
        } else if (ewk->wu.cg_type == 2) {
            demo_car_flag[ewk->master_id] = 1;
        }
    }

    display_car_C5(ewk);
}

static void set_car_departure_speed_C5(WORK_Other* ewk) {
    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = -0x20000;
        ewk->wu.mvxy.d[0].sp = -0x1000;
    } else {
        ewk->wu.mvxy.a[0].sp = 0x20000;
        ewk->wu.mvxy.d[0].sp = 0x1000;
    }
}

static void begin_car_departure_C5(WORK_Other* ewk) {
    ewk->wu.old_rno[0] = 48;
    set_car_departure_speed_C5(ewk);
}

static void continue_car_departure_C5(WORK_Other* ewk) {
    add_x_sub(&ewk->wu);
}

static void wait_for_car_departure_C5(WORK_Other* ewk) {
    (void)ewk;
}

static void update_car_departure_C5(
    WORK_Other* ewk, void (*on_expired)(WORK_Other*), void (*while_waiting)(WORK_Other*)
) {
    if (game_is_active()) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[0]++;
            on_expired(ewk);
        } else {
            while_waiting(ewk);
        }
    }

    display_car_C5(ewk);
}

static void prepare_car_departure_C5(WORK_Other* ewk) {
    update_car_departure_C5(ewk, begin_car_departure_C5, wait_for_car_departure_C5);
}

static void depart_car_C5(WORK_Other* ewk) {
    update_car_departure_C5(ewk, wait_for_car_departure_C5, continue_car_departure_C5);
}

static void hide_car_C5(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    demo_car_flag[ewk->master_id] = 0;
    ewk->wu.disp_flag = 0;
}

static void advance_car_state_C5(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
}

static void (*const car_state_handlers_C5[])(WORK_Other*) = { [0] = initialize_car_C5,   [1] = approach_car_C5,
                                                              [2] = idle_car_C5,         [3] = prepare_car_departure_C5,
                                                              [4] = depart_car_C5,       [5] = hide_car_C5,
                                                              [6] = advance_car_state_C5 };

static void dispose_car_C5(WORK_Other* ewk) {
    all_cgps_put_back(&ewk->wu);
    push_effect_work(&ewk->wu);
}

void effect_C5_move(WORK_Other* ewk) {
    if (ewk->wu.routine_no[0] < sizeof(car_state_handlers_C5) / sizeof(car_state_handlers_C5[0])) {
        car_state_handlers_C5[ewk->wu.routine_no[0]](ewk);
        return;
    }

    dispose_car_C5(ewk);
}

s32 effect_C5_init(PLW* oya, s16 reverse_f) {
    WORK_Other* ewk;
    s16 ix;
    s16 work;
    s16 id_num;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    demo_car_flag[oya->wu.id] = 0;
    Appear_car_stop[oya->wu.id] = 0;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 125;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.disp_flag = 0;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = 8;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_priority = ewk->wu.position_z = 57;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.my_col_code = oya->wu.my_col_code + 6;
    ewk->wu.sync_suzi = 0;
    ewk->master_id = oya->wu.id;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    id_num = oya->wu.id ^ reverse_f;

    if (id_num) {
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.xyz[1].cal = 0;
        ewk->wu.rl_flag = 0;
        ewk->wu.old_rno[0] = 40;
        work = (bg_w.bgw[1].pos_x_work + 192) & 0xFFFF;
        ewk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work + 320) & 0xFFFF;
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], work, 0, 1, 1 });
    } else {
        ewk->wu.xyz[1].cal = 0;
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.rl_flag = 1;
        ewk->wu.old_rno[0] = 40;
        work = (bg_w.bgw[1].pos_x_work - 192) & 0xFFFF;
        ewk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work - 320) & 0xFFFF;
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], work, 0, 1, 1 });
    }

    suzi_offset_set(ewk);
    effect_C6_init(ewk);
    return 0;
}
