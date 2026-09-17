/**
 * @file plpat20.c
 * Remy Attacks
 */

#include "sf33rd/Source/Game/engine/plpat20.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void (*const pl20_exatt_table[18])(PLW*);

void pl20_extra_attack(PLW* wk) {
    pl20_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

/* The travelling frames, written identically in Att_PL20_AT1 and Att_PL20_AT3:
 * marker 20 takes the next row, 25 adds to it, 30 loads the data row and falls
 * through into 35, which takes the next row and hands over to state 2. */
static void pl20_travel_markers(PLW* wk) {
    char_move(&wk->wu);
    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);

    switch (wk->wu.cg_type) {
    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 25:
        add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 30:
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
        /* fallthrough */

    case 35:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
        break;
    }
}

/* After the union, marker 20 takes the next row and sends the attack round
 * again. The same two functions wrote this identically too. */
static void pl20_restart_on_marker_20(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 1;
        wk->wu.cg_type = 0;
    }
}

void Att_PL20_AT1(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        pl20_travel_markers(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        pl20_restart_on_marker_20(wk);
        break;
    }
}

/* Marker 30 loads the descent row and steps the state on. */
static void pl20_at2_load_descent(PLW* wk) {
    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
    }
}

/* Once the union hands over to state 4 the vertical speed is flattened and a kop
 * of 2 is stepped back to 1. */
static void pl20_at2_settle_on_land(PLW* wk) {
    if (wk->wu.routine_no[3] == 4) {
        if (wk->wu.mvxy.kop[0] == 2) {
            wk->wu.mvxy.kop[0] = 1;
        }

        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = 0;
    }
}

/* The descent, which marker 20 ends. State 4 falls into this, so the helper is
 * reached from both arms exactly as the original fell through. */
static void pl20_at2_descend(PLW* wk) {
    cal_mvxy_speed(&wk->wu);
    add_mvxy_speed(&wk->wu);
    char_move(&wk->wu);

    if (wk->wu.cg_type == 20) {
        wk->wu.routine_no[3]++;
        reset_mvxy_data(&wk->wu);
    }
}

void Att_PL20_AT2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->r_no);
        wk->wu.mvxy.index++;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            add_mvxy_speed(&wk->wu);
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 4);
        pl20_at2_load_descent(wk);
        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        pl20_at2_settle_on_land(wk);
        break;

    case 4:
        wk->wu.routine_no[3]++;
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        /* fallthrough */

    case 5:
        pl20_at2_descend(wk);
        break;

    default:
        char_move(&wk->wu);
    }
}

void Att_PL20_AT3(PLW* wk) {
    PLW* emwk;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        reset_mvxy_data(&wk->wu);
        emwk = (PLW*)wk->wu.target_adrs;

        if (emwk->wu.hit_mark_y < 32) {
            set_char_move_init(&wk->wu, 5, 55);
        }

        break;

    case 1:
        pl20_travel_markers(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        pl20_restart_on_marker_20(wk);
        break;
    }
}

/* The taunt's markers: 40 pays the super-art gauge, 64 adds six to the stun bonus
 * against a ceiling of 24 and grades the personal action. */
static void pl20_taunt_markers(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 40) {
        wk->wu.cg_type = 0;
        add_sp_arts_gauge_tokushu(wk);
    }

    if (wk->wu.cg_type == 64) {
        wk->wu.routine_no[3]++;
        wk->tk_kizetsu += 6;

        if (wk->tk_kizetsu > 24) {
            wk->tk_kizetsu = 24;
        }

        grade_add_personal_action(wk->wu.id);
    }
}

void Att_PL20_TOKUSHUKOUDOU(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        pl20_taunt_markers(wk);
        break;

    default:
        char_move(&wk->wu);
        break;
    }
}

void (*const pl20_exatt_table[18])(PLW*) = { Att_HADOUKEN,    Att_PL20_AT1,     Att_PL20_AT2,
                                             Att_HOMING_JUMP, Att_HADOUKEN,     Att_PL20_AT3,
                                             Att_DUMMY,       Att_DUMMY,        Att_DUMMY,
                                             Att_DUMMY,       Att_DUMMY,        Att_DUMMY,
                                             Att_DUMMY,       Att_DUMMY,        Att_PL20_TOKUSHUKOUDOU,
                                             Att_DUMMY,       Att_METAMOR_WAIT, Att_METAMOR_REBIRTH };
