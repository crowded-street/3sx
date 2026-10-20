/**
 * @file appear44.c
 * Yang's intro on his stage from New Generation (Appear_05000 in the NG arcade ROM).
 */

#include "sf33rd/Source/Game/animation/ng/appear44.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ng/bg280.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define APPEAR_KOC 9
#define LEAP_CHAR_INDEX 0x11
#define LANDING_CHAR_INDEX 0x12
#define BOW_CHAR_INDEX 8
#define STAND_CHAR_INDEX 0x14
#define REGULAR_APPEAR_DATA 56
#define REGULAR_APPEAR_RNO 2
#define FAR_PRIORITY 110
#define START_X 464
#define STAND_TIME 92
#define LEAP_TIME 20
#define LEAP_DISTANCE 0x60

static void set_far_priority(PLW* wk) {
    wk->wu.next_z = wk->wu.position_z = FAR_PRIORITY;
}

void Appear_44000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        if (!bg280_yang_intro_plays()) {
            Appear_flag[wk->wu.id ^ 1] = 0;
            appear_data_set(wk, &appear_data[REGULAR_APPEAR_DATA]);
            wk->wu.routine_no[4] = REGULAR_APPEAR_RNO;
            Appear_01000(wk);
            break;
        }

        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, APPEAR_KOC, STAND_CHAR_INDEX);
        set_far_priority(wk);
        wk->wu.my_mr_flag = 0;
        wk->wu.xyz[0].disp.pos = START_X;
        app_counter[wk->wu.id] = STAND_TIME;

        if (wk->wu.id == 0) {
            wk->wu.rl_flag = 0;
        }

        break;

    case 1:
        set_far_priority(wk);

        if (--app_counter[wk->wu.id] >= 0) {
            break;
        }

        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, APPEAR_KOC, LEAP_CHAR_INDEX);
        app_counter[wk->wu.id] = LEAP_TIME;

        cal_all_speed_data(
            &wk->wu,
            app_counter[wk->wu.id],
            bg_w.bgw[1].pos_x_work + (wk->wu.id ? LEAP_DISTANCE : -LEAP_DISTANCE),
            0,
            1,
            1
        );

        if (wk->wu.id == 0) {
            wk->wu.rl_flag = 1;
        }

        Appear_flag[wk->wu.id ^ 1] = 0;
        break;

    case 2:
        if (--app_counter[wk->wu.id] > 0) {
            add_x_sub_ng(&wk->wu);
            add_y_sub_ng(&wk->wu);
            break;
        }

        wk->wu.routine_no[3]++;
        wk->wu.xyz[1].cal = 0;
        set_char_move_init(&wk->wu, APPEAR_KOC, LANDING_CHAR_INDEX);
        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, APPEAR_KOC, BOW_CHAR_INDEX);
            Appear_end++;
        }

        break;

    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }

        break;
    }
}
