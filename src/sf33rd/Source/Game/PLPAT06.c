#include "sf33rd/Source/Game/PLPAT06.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/PLPAT.h"
#include "sf33rd/Source/Game/PLPATUNI.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/PLS02.h"

void (*const pl06_exatt_table[18])(PLW*);

void pl06_extra_attack(PLW* wk) {
    pl06_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_PL06_HASHIRI_NAGE(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void setup_mvxy_data(WORK * wk, u32 ix);
#endif

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
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        if (wk->wu.routine_no[3] != 3 && wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;

    case 4:
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

        break;
    }
}

void Att_PL06_TOKUSHUKOUDOU(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    void grade_add_personal_action(s32 ix);
#endif

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
