/**
 * @file appear42.c
 * Yun's intro on his stage from New Generation (Appear_04000 in the NG arcade ROM). He skates in from the far end of
 * the street while the stage pans down, jumps off his skateboard and lands in front of the camera. The board bounces
 * away on its own (effect 124).
 */

#include "sf33rd/Source/Game/animation/ng/appear42.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/ng/eff124.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ng/bg280.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define APPEAR_KOC 10
#define RIDE_CHAR_INDEX 0
#define JUMP_OFF_CHAR_INDEX 1
#define LANDING_CHAR_INDEX 2
#define REGULAR_APPEAR_DATA 56
#define REGULAR_APPEAR_RNO 2
#define POSE_KOC 9
#define POSE_CHAR_INDEX 0x13
#define FAR_PRIORITY 100
#define START_X 592
#define START_SIZE 38
#define FULL_SIZE 63

static const s32 ride_speeds[6][4] = {
    { 0, 0, 0, 0 },
    { 0, 0x1000, -0x380, 0x2000 },
    { -0x8000, 0x4000, -0x380, 0x4000 },
    { -0xD000, 0x88000, -0x100, -0x8000 },
    { 0, 0, 0, 0 },
    { -0x8000, 0, -0x100, -0x800 },
};

static void set_ride_speeds(PLW* wk) {
    const s32* speeds = ride_speeds[wk->wu.routine_no[3]];

    wk->wu.mvxy.a[0].sp = speeds[0];
    wk->wu.mvxy.a[1].sp = speeds[1];
    wk->wu.mvxy.d[0].sp = speeds[2];
    wk->wu.mvxy.d[1].sp = speeds[3];
}

static void set_far_priority(PLW* wk) {
    wk->wu.next_z = wk->wu.position_z = FAR_PRIORITY;
}

static void grow(PLW* wk) {
    wk->wu.my_mr.size.x++;
    wk->wu.my_mr.size.y++;
}

static void ride(PLW* wk) {
    add_x_sub_ng(&wk->wu);
    add_y_sub_ng(&wk->wu);
    char_move(&wk->wu);
}

void Appear_42000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        if (!bg280_yun_intro_plays()) {
            Appear_flag[wk->wu.id ^ 1] = 0;
            appear_data_set(wk, &appear_data[REGULAR_APPEAR_DATA]);
            wk->wu.routine_no[4] = REGULAR_APPEAR_RNO;
            Appear_01000(wk);
            break;
        }

        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;
        wk->wu.char_table[APPEAR_KOC] = _ng_yun_appear_char_table;
        set_char_move_init(&wk->wu, APPEAR_KOC, RIDE_CHAR_INDEX);
        set_far_priority(wk);
        wk->wu.my_mr_flag = 1;
        wk->wu.my_mr.size.x = wk->wu.my_mr.size.y = START_SIZE;
        set_ride_speeds(wk);
        wk->wu.xyz[0].disp.pos = START_X;
        break;

    case 1:
        set_far_priority(wk);
        ride(wk);
        grow(wk);

        if (wk->wu.my_mr.size.x > 50) {
            wk->wu.routine_no[3]++;
            set_ride_speeds(wk);
        }

        break;

    case 2:
        set_far_priority(wk);
        ride(wk);

        if (wk->wu.xyz[1].disp.pos >= 201) {
            wk->wu.routine_no[3]++;
            set_ride_speeds(wk);
        }

        break;

    case 3:
        grow(wk);
        ride(wk);

        if (wk->wu.mvxy.a[1].sp < 0) {
            // NG also sets a flag here for stage objects that react to the jump (effect 122). Yun's stage has none.
            wk->wu.routine_no[3]++;
            app_counter[wk->wu.id] = 3;
        }

        break;

    case 4:
        char_move(&wk->wu);

        if (--app_counter[wk->wu.id] < 0) {
            wk->wu.routine_no[3]++;
            app_counter[wk->wu.id] = 16;
            set_ride_speeds(wk);
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (--app_counter[wk->wu.id] >= 0) {
            grow(wk);
            break;
        }

        wk->wu.routine_no[3]++;
        wk->wu.my_mr_flag = 0;
        wk->wu.my_mr.size.x = wk->wu.my_mr.size.y = FULL_SIZE;
        wk->wu.rl_flag = (wk->wu.rl_flag + 1) & 1;
        set_char_move_init(&wk->wu, APPEAR_KOC, JUMP_OFF_CHAR_INDEX);
        app_counter[wk->wu.id] = 24;
        cal_all_speed_data(&wk->wu, app_counter[wk->wu.id], wk->wu.id ? 608 : 416, 0, 1, 1);
        effect_ng124_init(wk);
        break;

    case 6:
        if (--app_counter[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            wk->wu.xyz[1].cal = 0;
            set_char_move_init(&wk->wu, APPEAR_KOC, LANDING_CHAR_INDEX);
            Appear_flag[wk->wu.id ^ 1] = 0;
            break;
        }

        add_x_sub(&wk->wu);
        add_y_sub(&wk->wu);
        char_move(&wk->wu);
        break;

    case 7:
        char_move(&wk->wu);

        if (wk->wu.cg_type) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, POSE_KOC, POSE_CHAR_INDEX);
        }

        break;

    case 8:
        char_move(&wk->wu);

        if (wk->wu.cg_type) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end++;
        }

        break;
    }
}
