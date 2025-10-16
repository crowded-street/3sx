/**
 * @file plpat07.c
 * Ibuki Attacks
 */

#include "sf33rd/Source/Game/engine/plpat07.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void (*const pl07_exatt_table[18])(PLW*);

void pl07_extra_attack(PLW* wk) {
    pl07_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_PL07_SA2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    default:
        char_move(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);

        switch (wk->wu.cg_type) {
        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            break;

        case 30:
            wk->wu.mvxy.index = wk->as->data_ix;
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            break;

        case 88:
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
            break;
        }

        break;
    }
}

void Att_PL07_AT1(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);

        switch (wk->wu.cg_type) {
        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 21:
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
            break;

        case 30:
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            break;
        }

        break;
    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.routine_no[3] = 3;
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL07_AT2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        break;

    case 1:
        char_move(&wk->wu);
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->as->r_no);
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL07_AT3(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[3]++;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL07_SA3(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        /* fallthrough */

    case 1:
        jumping_union_process(&wk->wu, 2);

        if (wk->wu.routine_no[3] != 2) {
            if (wk->wu.cg_type == 20) {
                setup_mvxy_data(&wk->wu, wk->as->data_ix);
                wk->wu.cg_type = 0;
            }

            if (wk->wu.cg_type == 30) {
                setup_mvxy_data(&wk->wu, wk->as->r_no);
                wk->wu.cg_type = 0;
            }
        }

        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL07_TOKUSHUKOUDOU(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        wk->wu.mvxy.index++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
            add_mvxy_speed(&wk->wu);
            cal_mvxy_speed(&wk->wu);
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.cg_type == 40) {
            wk->wu.cg_type = 0;
            add_sp_arts_gauge_tokushu(wk);
        }

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;

    case 4:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.cg_type == 30) {
            wk->wu.cg_type = 0;
            wk->tk_dageki += 14;
            wk->tk_nage += 14;
        }

        if (wk->tk_dageki > 28) {
            wk->tk_dageki = 28;
        }

        if (wk->tk_nage > 28) {
            wk->tk_nage = 28;
        }

        if (wk->wu.cg_type == 64) {
            grade_add_personal_action(wk->wu.id);
        }

        break;
    }
}

void (*const pl07_exatt_table[18])(PLW*) = {
    Att_SHOURYUUKEN, Att_PL07_AT1, Att_CHOUCHUURENGEKI,    Att_SLIDE_and_JUMP, Att_CHOUCHUURENGEKI, Att_PL07_SA2,
    Att_PL07_AT2,    Att_PL07_AT3, Att_PL07_SA3,           Att_SLIDE_and_JUMP, Att_HOMING_JUMP,     Att_DUMMY,
    Att_DUMMY,       Att_DUMMY,    Att_PL07_TOKUSHUKOUDOU, Att_DUMMY,          Att_METAMOR_WAIT,    Att_METAMOR_REBIRTH
};
