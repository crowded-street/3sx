/**
 * @file plpat06.c
 * Hugo Attacks
 */

#include "sf33rd/Source/Game/engine/plpat06.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void (*const pl06_exatt_table[18])(PLW*);

void pl06_extra_attack(PLW* wk) {
    pl06_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

/* The run's markers: 20 takes the next row, 30 jumps to the data row and hands
 * over to the union, 40 resets, 60 and 70 go to the throw. */
static void pl06_run_markers(PLW* wk) {
    char_move(&wk->wu);

    switch (wk->wu.cg_type) {
    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 30:
        wk->wu.mvxy.index = wk->as->data_ix;
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
        break;

    case 40:
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        break;

    case 60:
        wk->wu.routine_no[3] = 4;
        wk->wu.cg_type = 0;
        break;

    case 70:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 4;
        wk->wu.cg_type = 0;
        break;
    }

    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);
}

/* While the union has not landed, marker 30 still feeds rows. The arm's `break`
 * left the switch with nothing after it. */
static void pl06_union_step(PLW* wk) {
    if (wk->wu.routine_no[3] != 3 && wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }
}

/* The throw's markers. The switch is a near miss of the run's - it has 50 and 71
 * where that one has 60 and 70 - so the two stay apart, and a grab on either side
 * cuts the throw back to the run. */
static void pl06_throw_markers(PLW* wk) {
    char_move(&wk->wu);

    switch (wk->wu.cg_type) {
    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 30:
        wk->wu.mvxy.index = wk->as->data_ix;
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
        break;

    case 40:
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        break;

    case 50:
        wk->wu.routine_no[3] = 1;
        wk->wu.cg_type = 0;
        break;

    case 71:
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 1;
        break;
    }

    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);

    if (wk->wu.routine_no[3] != 1 && wk->hos_fi_flag | wk->hos_em_flag) {
        char_move_cmj4(&wk->wu);
        wk->wu.routine_no[3] = 1;
    }
}

void Att_PL06_HASHIRI_NAGE(PLW* wk) {
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
        pl06_run_markers(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        pl06_union_step(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;

    case 4:
        pl06_throw_markers(wk);
        break;
    }
}

/* The taunt's markers: 40 pays the super-art gauge, then 20 and 30 each add to
 * their own bonuses against their own ceilings and 64 grades the action. The 40
 * test sits outside the switch in the original, and clearing cg_type there is
 * what stops that frame reaching any switch arm - so it stays where it was. */
static void pl06_taunt_markers(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 40) {
        wk->wu.cg_type = 0;
        add_sp_arts_gauge_tokushu(wk);
    }

    switch (wk->wu.cg_type) {
    case 20:
        wk->wu.cg_type = 0;
        wk->tk_nage += 8;

        if (wk->tk_nage > 8) {
            wk->tk_nage = 8;
        }

        break;

    case 30:
        wk->wu.cg_type = 0;
        wk->tk_dageki += 6;
        wk->tk_konjyou += 2;

        if (wk->tk_dageki > 6) {
            wk->tk_dageki = 6;
        }

        if (wk->tk_konjyou > 8) {
            wk->tk_konjyou = 8;
        }

        break;

    case 64:
        grade_add_personal_action(wk->wu.id);
        break;
    }
}

void Att_PL06_TOKUSHUKOUDOU(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        pl06_taunt_markers(wk);
        break;
    }
}

void (*const pl06_exatt_table[18])(PLW*) = { Att_HADOUKEN2,
                                             Att_HADOUKEN2,
                                             Att_CHOUCHUURENGEKI,
                                             Att_HADOUKEN2,
                                             Att_SHOURYUUKEN,
                                             Att_SLIDE_and_JUMP,
                                             Att_HADOUKEN2,
                                             Att_SHOURYUUKEN,
                                             Att_PL06_HASHIRI_NAGE,
                                             Att_PL06_HASHIRI_NAGE,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_PL06_TOKUSHUKOUDOU,
                                             Att_DUMMY,
                                             Att_METAMOR_WAIT,
                                             Att_METAMOR_REBIRTH };
