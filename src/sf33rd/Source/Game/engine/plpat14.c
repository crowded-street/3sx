/**
 * @file plpat14.c
 * Akuma/Gouki Attacks
 */

#include "sf33rd/Source/Game/engine/plpat14.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

s8 stop_count[2]; // FIXME: This is pretty much unused. Remove?

const s16 pl14_HYAKKI_dat[20] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 7, 6, 6, 4, 6, 14, 15, 16, 17, 18 };

void (*const pl14_exatt_table[18])(PLW*);

void pl14_extra_attack(PLW* wk) {
    pl14_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

/* The attack drifts along the facing saved when it started, not the current one,
 * so a turn mid-move does not reverse it. Both arms of Att_PL14_AT1 wrote this
 * identically. */
static void drift_along_saved_facing(PLW* wk) {
    cal_mvxy_speed(&wk->wu);

    if (wk->rl_save) {
        wk->wu.xyz[0].cal += wk->wu.mvxy.a[0].sp;
    } else {
        wk->wu.xyz[0].cal -= wk->wu.mvxy.a[0].sp;
    }

    wk->wu.xyz[1].cal += wk->wu.mvxy.a[1].sp;
}

/* The grounded markers: 10 steps the state on and falls into 20, which takes the
 * next movement row; 21 resets it. */
static void pl14_at1_ground_markers(PLW* wk) {
    char_move(&wk->wu);
    drift_along_saved_facing(wk);

    switch (wk->wu.cg_type) {
    case 10:
        wk->wu.routine_no[3]++;
        /* fallthrough */

    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 21:
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        break;
    }
}

/* The later markers. The switch is a near miss of the grounded one - it has no 10
 * arm and its 21 arm also re-sorts the depth - so the two stay apart. */
static void pl14_at1_air_markers(PLW* wk) {
    char_move(&wk->wu);
    drift_along_saved_facing(wk);
    wk->wu.rl_flag = wk->wu.rl_waza;

    if ((wk->wu.mvxy.a[0].sp != 0) && wk->old_pos_data[0] == wk->old_pos_data[1]) {
        char_move_z(&wk->wu);
    }

    switch (wk->wu.cg_type) {
    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 21:
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        char_move_z(&wk->wu);
        break;
    }
}

void Att_PL14_AT1(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        wk->rl_save = wk->wu.rl_flag;
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        stop_count[wk->wu.id] = 0;
        break;

    case 1:
        pl14_at1_ground_markers(wk);
        break;

    default:
        pl14_at1_air_markers(wk);
        break;
    }
}

void Att_PL14_AT2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);

        switch (wk->wu.cg_type) {
        case 10:
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            break;

        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 30:
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;
        }
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.cg_type == 40) {
            wk->wu.routine_no[3] = 1;
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

/* Marker 20 takes the next movement row and hands the attack to state 2. Two arms
 * of Att_PL14_AT3 wrote this identically. */
static void take_row_and_enter_state_2(PLW* wk) {
    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
    }
}

/* The opening frame. */
static void begin_pl14_at3(PLW* wk, PLW* twk) {
    wk->wu.routine_no[3]++;
    hoken_muriyari_chakuchi(wk);
    wk->wu.rl_flag = wk->wu.rl_waza;
    reset_mvxy_data(&wk->wu);
    wk->wu.mvxy.index = wk->as->r_no;
    set_char_move_init(&wk->wu, 5, wk->as->char_ix);
}

/* The wind-up: marker 20 goes to state 2, marker 30 kills the vertical speed and
 * goes to state 3, and either way the speed is applied once the state has moved. */
static void pl14_at3_wind_up(PLW* wk, PLW* twk) {
    char_move(&wk->wu);

    take_row_and_enter_state_2(wk);

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 3;
        wk->wu.cg_type = 0;
    }

    if (wk->wu.routine_no[3] != 1) {
        add_mvxy_speed(&wk->wu);
    }

}

/* While the leg has not returned to its landed state, marker 20 keeps feeding
 * movement rows and the catch box is retargeted at whichever character the
 * opponent is. The union leg and the regrab wrote this out the same way, against
 * state 1 and state 2 - that number is the one value Recipe D allows as a
 * parameter. The regrab's copy also put a redundant pair of parentheses around
 * twk->player_number; that is the same subexpression, not a second difference. */
static void pl14_at3_feed_and_track(PLW* wk, PLW* twk, s16 landed_rno) {
    if ((wk->wu.routine_no[3] != landed_rno) && (wk->wu.cg_type == 20)) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }

    if ((wk->wu.routine_no[3] != landed_rno) && wk->wu.cg_ja.caix) {
        wk->wu.cg_ja.caix = pl14_HYAKKI_dat[twk->player_number];
        wk->wu.h_cat = wk->wu.catch_adrs + wk->wu.cg_ja.caix;
    }
}

/* The union leg: marker 1 goes to the regrab, marker 20 feeds rows, and the
 * catch box is retargeted at the opponent while the union has not returned. */
static void pl14_at3_union_leg(PLW* wk, PLW* twk) {
    jumping_union_process(&wk->wu, 1);

    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 4;
    }

    pl14_at3_feed_and_track(wk, twk, 1);

}

/* The descent and its three markers. Its marker-30 block is a near miss of the
 * wind-up's - that one assigns state 3 as well - so they stay apart. */
static void pl14_at3_descent(PLW* wk, PLW* twk) {
    char_move(&wk->wu);
    cal_mvxy_speed(&wk->wu);
    add_mvxy_speed(&wk->wu);

    take_row_and_enter_state_2(wk);

    if (wk->wu.cg_type == 21) {
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 1;
    }

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }

}

/* The regrab, the near twin of the union leg against state 2 rather than 1. It
 * is extracted because AT3 is a Large Method that only clears if every arm goes;
 * see the playbook's rule on twin arms. */
static void pl14_at3_regrab(PLW* wk, PLW* twk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 2;
    }

    pl14_at3_feed_and_track(wk, twk, 2);

}

void Att_PL14_AT3(PLW* wk) {
    PLW* twk = (PLW*)wk->wu.target_adrs;

    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_pl14_at3(wk, twk);
        break;

    case 1:
        pl14_at3_wind_up(wk, twk);
        break;

    case 2:
        pl14_at3_union_leg(wk, twk);
        break;

    case 3:
        pl14_at3_descent(wk, twk);
        break;

    case 4:
        pl14_at3_regrab(wk, twk);
        break;
    }
}

void Att_PL14_TOKUSHUKOUDOU(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 40) {
            wk->wu.cg_type = 0;
            add_sp_arts_gauge_tokushu(wk);
        }

        if (wk->wu.cg_type == 64) {
            wk->wu.cg_type = 0;
            wk->tk_dageki += 14;
            wk->tk_kizetsu += 9;

            if (wk->tk_dageki > 14) {
                wk->tk_dageki = 14;
            }

            if (wk->tk_kizetsu > 9) {
                wk->tk_kizetsu = 9;
            }

            grade_add_personal_action(wk->wu.id);
        }

        break;
    }
}

void (*const pl14_exatt_table[18])(PLW*) = { Att_HADOUKEN,
                                             Att_SHOURYUUKEN,
                                             Att_SENPUUKYAKU,
                                             Att_KUUCHUUJINNCHUUWATARI,
                                             Att_SHOURYUUREPPA,
                                             Att_SLIDE_and_JUMP,
                                             Att_KUUCHUUNICHIRINSHOU,
                                             Att_PL14_AT1,
                                             Att_CHOUCHUURENGEKI,
                                             Att_PL14_AT2,
                                             Att_HADOUKEN,
                                             Att_PL14_AT3,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_PL14_TOKUSHUKOUDOU,
                                             Att_DUMMY,
                                             Att_METAMOR_WAIT,
                                             Att_METAMOR_REBIRTH };
