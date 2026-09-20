/**
 * @file appear_late.c
 * Character Appear Animation, states 20000 and above
 *
 * Split out of appear.c.
 */

#include "common.h"
#include "sf33rd/Source/Game/animation/app_data.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/eff09.h"
#include "sf33rd/Source/Game/effect/eff15.h"
#include "sf33rd/Source/Game/effect/eff46.h"
#include "sf33rd/Source/Game/effect/eff86.h"
#include "sf33rd/Source/Game/effect/eff97.h"
#include "sf33rd/Source/Game/effect/effc5.h"
#include "sf33rd/Source/Game/effect/effc8.h"
#include "sf33rd/Source/Game/effect/effm0.h"
#include "sf33rd/Source/Game/effect/effm1.h"
#include "sf33rd/Source/Game/effect/effm5.h"
#include "sf33rd/Source/Game/effect/effm7.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void Appear_20000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x15);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);
    }
}

void Appear_21000(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;
        work = random_16();
        work &= 7;
        if (work == 6 || work == 7) {
            appear_data_set(wk, (APPEAR_DATA*)appear_data + 24);
            set_char_move_init(&wk->wu, 9, 0xE);
        } else {
            set_char_move_init(&wk->wu, 9, work + 8);
        }
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
    }
}

void Appear_22000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        wk->wu.cmwk[1] = 0;
        set_char_move_init(&wk->wu, 9, 0);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 9, 8);
        break;

    case 2:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);
    }
}

void Appear_23000(PLW* wk) {
    s16 work;
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;
        work = random_16();
        work &= 3;
        wk->wu.cmwk[1] = 0;
        set_char_move_init(&wk->wu, 9, work + 4);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        if ((wk->wu.cmwk[1]) && wk->wu.cg_type == 9) {
            wk->wu.routine_no[3] += 1;
            set_char_move_init(&wk->wu, 9, wk->wu.char_index + 8);
            return;
        } else {
            break;
        }
    case 2:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
        break;
    }
}

void Appear_24000(PLW* wk) {
    if (!wk->wu.operator) {
        if (wk->wu.id) {
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0xA8;
        } else {
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - 0x90;
        }
    }

    wk->wu.routine_no[2] = 1;
    wk->wu.routine_no[3] = 0;
}

void Appear_25000(PLW* wk) {
    if (!wk->wu.operator) {
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work;
    }

    wk->wu.routine_no[2] = 1;
    wk->wu.routine_no[3] = 0;
}

const s16 smoke_check[] = { 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 };

/* The second half of this entrance: everything from the rise back up onwards.
 * The case labels are the original ones, so the states still read as the same
 * numbers. */
static void launch_appear_26000(PLW* wk) {
    wk->wu.routine_no[3]++;
    appear_work[wk->wu.id] = 0x14;

    if (wk->wu.id) {
        cal_all_speed_data(&wk->wu, &(Motion_Target) { appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work + 88, 0, 0, 1 });
    } else {
        cal_all_speed_data(&wk->wu, &(Motion_Target) { appear_work[wk->wu.id], bg_w.bgw[1].pos_x_work - 88, 0, 0, 1 });
    }
}

static void land_appear_26000(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.hit_quake = 0x18;

    if (wk->wu.id == 0) {
        effect_86_init(0);
    }
    Sound_SE(0x109);
}

static void step_appear_26000_rise(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 4:
        char_move(&wk->wu);
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0x10, 3, 0);
            wk->wu.xyz[1].cal = 0;
        } else {
            add_x_sub(&wk->wu);
            add_y_sub(&wk->wu);
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            wk->wu.rl_flag ^= 1;
            wk->wu.routine_no[3]++;
        }

        break;

    case 6:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);

        break;
    }
}

/* The middle states of this entrance, reached from the first level's default
 * and reaching the last level through its own. The case labels are the
 * original ones, so the states still read as the same numbers. */
static void step_appear_26000_impact(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 2:
        char_move(&wk->wu);
        appear_work[wk->wu.id]--;
        add_x_sub(&wk->wu);
        add_y_sub(&wk->wu);

        if (wk->wu.xyz[1].disp.pos < 0x41) {
            land_appear_26000(wk);
        }

        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.hit_quake < 1) {
            wk->wu.routine_no[3]++;
            Appear_free[wk->wu.id] = 1;
        }

        break;

    default:
        step_appear_26000_rise(wk);
        break;
    }
}

void Appear_26000(PLW* wk) {
    // s32 effect_86_init(s16 type86);

    switch (wk->wu.routine_no[3]) {
    case 0:
        bg_app_stop = 1;
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x10);
        appear_work[wk->wu.id] = 0x14;
        Appear_free[wk->wu.id] = 0;
        break;

    case 1:
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] < 1) {
            launch_appear_26000(wk);
        }

        break;

    default:
        step_appear_26000_impact(wk);
        break;
    }
}

/* The second half of this entrance: everything from the partner's cue
 * onwards. The case labels are the original ones, so the states still read as
 * the same numbers. */
static void step_appear_28000_dismount(PLW* wk, s16 id_w) {
    switch (wk->wu.routine_no[3]) {
    case 3:
        if (plw[id_w].wu.routine_no[3] >= 3) {
            wk->wu.routine_no[3]++;
            appear_work[wk->wu.id] = 20;
        }

        break;

    case 4:
        appear_work[wk->wu.id]--;

        if (appear_work[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 17, 15, 0);
        }

        break;

    case 5:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);

        break;
    }
}

void Appear_28000(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        Appear_car_stop[id_w] = 0;
        set_char_move_init(&wk->wu, 9, 17);
        bg_app_stop = 1;
        break;

    case 1:
        if (Appear_car_stop[id_w]) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 17, 2, 0);
        }

        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            wk->wu.routine_no[3]++;
        }

        break;

    default:
        step_appear_28000_dismount(wk, id_w);
        break;
    }
}

static void start_appear_29000(PLW* wk) {
    s16 work;

    wk->wu.routine_no[3]++;
    bg_app_stop = 1;
    wk->wu.disp_flag = 1;
    wk->wu.cmwk[1] = 0;
    wk->wu.cmwk[2] = 0;
    work = random_16();
    work &= 3;
    wk->wu.cmwk[2] = work;
    set_char_move_init(&wk->wu, 9, 0);
    work = random_16();

    if (work & 1) {
        effect_09_init2(&wk->wu, 0x19);
    }
    if (8 < work) {
        effect_09_init2(&wk->wu, 0x1b);
    }

    animal_decide(wk);
}

static void choose_appear_29000_entry(PLW* wk) {
    switch (wk->wu.cmwk[2]) {
    case 0:
    case 1:
        wk->wu.routine_no[3] = 2;

        if (wk->wu.id) {
            wk->wu.mvxy.a[0].sp = 0xffff0000;
        } else {
            wk->wu.mvxy.a[0].sp = 0x10000;
        }

        set_char_move_init(&wk->wu, 9, 8);
        break;

    case 2:
        wk->wu.routine_no[3] = 3;

        if (wk->wu.id) {
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0xd8;
        } else {
            wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + -0xd8;
        }

        set_char_move_init(&wk->wu, 9, 10);
        break;

    case 3:
        wk->wu.routine_no[3] = 4;
        set_char_move_init(&wk->wu, 9, 0xb);
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.d[1].sp = 0xffff8000;
        wk->wu.xyz[1].disp.pos = 0xb0;
        app_counter[wk->wu.id] = 0x20;

        if (wk->wu.id) {
            cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0);
        } else {
            cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0);
        }
    }
}

static void walk_appear_29000_in(PLW* wk) {
    char_move(&wk->wu);
    add_x_sub(&wk->wu);

    if (wk->wu.id) {
        if (!(wk->wu.xyz[0].disp.pos <= (bg_w.bgw[1].pos_x_work + 0x58))) {
            return;
        }
    } else if (wk->wu.xyz[0].disp.pos < (bg_w.bgw[1].pos_x_work - 0x58)) {
        return;
    }

    mark_appear_finished(wk);
}

/* The later states of this entrance. The case labels are the original ones, so
 * the states still read as the same numbers. */
static void step_appear_29000_settle(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 3:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);
        break;

    case 4:
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 1) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0xb, 5, 0);
            wk->wu.xyz[1].disp.pos = 0;
        } else {
            add_x_sub(&wk->wu);
            add_y_sub(&wk->wu);
        }

        break;

    case 5:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);

        break;
    }
}

void Appear_29000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        start_appear_29000(wk);
        break;

    case 1:
        if (wk->wu.cmwk[1]) {
            choose_appear_29000_entry(wk);
        }

        break;

    case 2:
        walk_appear_29000_in(wk);
        break;

    default:
        step_appear_29000_settle(wk);
        break;
    }
}

void animal_decide(PLW* wk) {
    u8 work2;
    s16 work = random_16();

    work2 = animal_decide_tbl[work];
    don_appear_check(wk);

    switch (work2) {
    case 0:
        break;

    case 1:
        effect_M0_init(wk->wu.rl_flag, 0);
        effect_M0_init(wk->wu.rl_flag, 1);
        break;

    default:
        effect_M0_init(wk->wu.rl_flag, work2);
        break;
    }

    return;
}

void don_appear_check(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;

    if (plw[id_w].player_number == 7) {
        effect_M0_init(wk->wu.rl_flag, 6);
    }
}

void Appear_30000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        random_16(); // Although the result is unused, I'm keeping the call, because it changes RNG state
        appear_data_set(wk, (APPEAR_DATA*)appear_data + 24);
        set_char_move_init(&wk->wu, 9, 0xE);
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
    }
}

void Appear_31000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;

        if (Appear_flag[wk->wu.id]) {
            appear_data_set(wk, (APPEAR_DATA*)appear_data);
            Appear_00000(wk);
        } else {
            set_char_move_init(&wk->wu, 9, 8);
        }

        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);

        break;
    }
}

void Appear_32000(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;
        work = random_16();
        work &= 7;
        set_char_move_init(&wk->wu, 9, work + 8);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);
        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
            Appear_end += 1;
        }
    }
}

void Appear_33000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 9, 0xC);
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.d[1].sp = -0x8000;
        wk->wu.xyz[1].disp.pos = 0x50;
        app_counter[wk->wu.id] = 0x2A;

        if (wk->wu.id) {
            cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0);
        } else {
            cal_initial_speed(&wk->wu, app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0);
        }

        bg_app_stop = 1;
        don_appear_check(wk);
        break;

    case 1:
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0xC, 2, 0);
            wk->wu.xyz[1].disp.pos = 0;
            return;
        }

        add_x_sub(&wk->wu);
        add_y_sub(&wk->wu);
        break;

    case 2:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);
    }
}

void Appear_34000(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        wk->wu.disp_flag = 1;

        work = random_16();
        work &= 7;
        set_char_move_init(&wk->wu, 9, work);

        switch (work) {
        case 0:
        case 2:
        case 6:
        case 7:
            if (wk->wu.id) {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0x71;
            } else {
                wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work - 0x71;
            }
        }
        bg_app_stop = 1;
        break;

    default:
        Appear_01000(wk);
    }
}

/* The second half of this entrance: the slide in and the settle. The case
 * labels are the original ones, so the states still read as the same numbers. */
static void step_appear_36000_settle(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 3:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            set_char_move_init2(&wk->wu, 9, 0x11, 0x0A, 0);
            wk->wu.next_z = wk->wu.my_priority;
        } else {
            add_x_sub(&wk->wu);
        }

        break;

    case 4:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);

        break;
    }
}

/* The middle state of this entrance, reached from the first level's default
 * and reaching the last level through its own. The case label is the original
 * one, so the state still reads as the same number. */
static void step_appear_36000_slide(PLW* wk, s16 id_w) {
    switch (wk->wu.routine_no[3]) {
    case 2:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] <= 0) {
            wk->wu.routine_no[3]++;
            app_counter[wk->wu.id] = 0x16;

            if (wk->wu.id) {
                cal_all_speed_data(
                    &wk->wu, &(Motion_Target) { app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0, 2, 0 }
                );
            } else {
                cal_all_speed_data(
                    &wk->wu, &(Motion_Target) { app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0, 2, 0 }
                );
            }
        }

        wk->wu.next_z = plw[id_w].wu.my_priority;
        break;

    default:
        step_appear_36000_settle(wk);
        break;
    }
}

void Appear_36000(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x10);
        bg_app_stop = 1;
        break;

    case 1:
        char_move(&wk->wu);

        if (plw[id_w].wu.cmwk[0] == 3) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x11);
            app_counter[wk->wu.id] = 0x10;
            wk->wu.next_z = plw[id_w].wu.my_priority;
        }

        break;

    default:
        step_appear_36000_slide(wk, id_w);
        break;
    }
}

const u8 animal_decide_tbl[] = { 0, 1, 2, 3, 4, 5, 0, 2, 0, 1, 2, 3, 4, 5, 0, 0 };

/* The second half of this entrance: everything from the priority swap onwards.
 * The case labels are the original ones, so the states still read as the same
 * numbers. */
static void dismount_appear_37000(PLW* wk) {
    wk->wu.routine_no[3]++;
    set_char_move_init(&wk->wu, 0, 3);
    app_counter[wk->wu.id] = 0x2a;

    if (wk->wu.id) {
        cal_all_speed_data(
            &wk->wu, &(Motion_Target) { app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work + 0x58, 0, 0, 0 }
        );
    } else {
        cal_all_speed_data(
            &wk->wu, &(Motion_Target) { app_counter[wk->wu.id], bg_w.bgw[1].pos_x_work - 0x58, 0, 0, 0 }
        );
    }

    wk->wu.next_z = wk->wu.my_priority;
}

static void step_appear_37000_ride(PLW* wk, s16 id_w) {
    switch (wk->wu.routine_no[3]) {
    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 9) {
            wk->wu.routine_no[3]++;
            wk->wu.cmwk[0] = 3;
            wk->wu.next_z = plw[id_w].wu.my_priority;
        }

        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            dismount_appear_37000(wk);
        } else {
            wk->wu.next_z = plw[id_w].wu.my_priority;
        }

        break;

    case 6:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 1) {
            mark_appear_finished(wk);
        } else {
            add_x_sub(&wk->wu);
        }

        break;
    }
}

/* The middle states of this entrance, reached from the first level's default
 * and reaching the last level through its own. The case labels are the
 * original ones, so the states still read as the same numbers. */
static void step_appear_37000_mount(PLW* wk, s16 id_w) {
    switch (wk->wu.routine_no[3]) {
    case 2:
        char_move(&wk->wu);

        if (wk->wu.cmwk[0] == 2) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x12);
        }

        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            set_char_move_init(&wk->wu, 9, 0x13);
        }

        break;

    default:
        step_appear_37000_ride(wk, id_w);
        break;
    }
}

void Appear_37000(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        set_char_move_init(&wk->wu, 9, 0x11);
        bg_app_stop = 1;
        wk->wu.cmwk[0] = 0;
        effect_M1_init(&wk->wu);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
            wk->wu.cmwk[0] = 1;
        }
        break;

    default:
        step_appear_37000_mount(wk, id_w);
        break;
    }
}

void Appear_38000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        bg_app_stop = 1;
        wk->wu.disp_flag = 1;

        if (wk->wu.id) {
            set_char_move_init(&wk->wu, 9, 0x14);
            return;
        }

        set_char_move_init(&wk->wu, 9, 0x13);
        break;

    case 1:
        char_move(&wk->wu);

        finish_appear_on_last_frame(wk);
    }
}

static void start_appear_39000(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.disp_flag = 1;

    if (Gill_Appear_Flag) {
        appear_data_set(wk, (APPEAR_DATA*)appear_data);
        Appear_00000(wk);
        return;
    }

    bg_app_stop = 1;
    set_char_move_init(&wk->wu, 0, 2);

    if (wk->wu.id) {
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 0x200;
    }

    setup_mvxy_data(&wk->wu, 0);
    wk->wu.mvxy.a[0].sp >>= 1;
    add_mvxy_speed(&wk->wu);
    wk->wu.mvxy.a[0].sp *= 2;
}

static void arrive_appear_39000(PLW* wk) {
    if (!wk->wu.id) {
        if (wk->wu.xyz[0].disp.pos > (bg_w.bgw[1].pos_x_work - 88)) {
            wk->wu.routine_no[3] += 1;
            wk->wu.xyz[0].disp.pos = (bg_w.bgw[1].pos_x_work - 88);
        }

        return;
    }

    if (wk->wu.xyz[0].disp.pos < (bg_w.bgw[1].pos_x_work + 88)) {
        wk->wu.routine_no[3]++;
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].pos_x_work + 88;
    }
}

void Appear_39000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        start_appear_39000(wk);
        break;

    case 1:
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        char_move(&wk->wu);

        arrive_appear_39000(wk);

        break;

    case 2:
        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 0;
        Appear_end += 1;
        break;
    }
}

void Appear_41000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.disp_flag = 1;
        bg_app_stop = 1;
        set_char_move_init(&wk->wu, 0, 0);
        app_counter[wk->wu.id] = 0x78;
        effect_M7_init(wk);
        break;

    case 1:
        char_move(&wk->wu);
        app_counter[wk->wu.id]--;

        if (app_counter[wk->wu.id] < 0) {
            mark_appear_finished(wk);
        }

        break;
    }
}

void gouki_appear(PLW* wk) {
    if (!wk->wu.cmwk[0]) {
        char_move(&wk->wu);
        return;
    }

    switch (wk->wu.routine_no[6]) {
    case 0:
        wk->wu.routine_no[6]++;
        set_char_move_init(&wk->wu, 1, 0x3C);
        char_move_z(&wk->wu);
        wk->wu.xyz[1].disp.pos = -6;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[6]++;
        }

        break;

    case 2:
        break;
    }
}
