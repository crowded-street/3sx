/**
 * @file plpat12.c
 * Sean Attacks
 */

#include "sf33rd/Source/Game/engine/plpat12.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effd7.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void (*const pl12_exatt_table[18])(PLW*);

void pl12_extra_attack(PLW* wk) {
    pl12_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_PL12_TOKUSHUKOUDOU(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        wk->wu.mvxy.index = wk->as->data_ix;
        wk->wu.cmwk[6] = 0;
        wk->wu.cmwk[7] = 0;
        wk->tk_success++;

        if (wk->metamorphose) {
            set_char_move_init(&wk->wu, 5, wk->as->char_ix + 1);
            break;
        }

        if (effect_D7_init(wk)) {
            set_char_move_init(&wk->wu, 5, wk->as->char_ix + 1);
            break;
        }

        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
        }

        break;

    case 2:
        if (wk->wu.cmwk[7] != 0) {
            char_move_cmj4(&wk->wu);
            wk->wu.cmwk[7] = 0;
            wk->wu.mvxy.index++;
            add_sp_arts_gauge_tokushu(wk);
        }

        jumping_union_process(&wk->wu, 3);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 30) {
            wk->wu.cg_type = 0;
            wk->tk_kizetsu += 4;

            if (wk->tk_kizetsu > 12) {
                wk->tk_kizetsu = 12;
            }
        }

        break;

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 64) {
            grade_add_personal_action(wk->wu.id);
        }

        break;
    }
}

void Att_PL12_BONUS_STAGE(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        wk->wu.cmwk[6] = 0;
        wk->wu.cmwk[7] = 0;
        set_char_move_init(&wk->wu, 5, wk->wu.char_index);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[3]++;
        }

        break;

    case 2:
        if (wk->wu.cmwk[7] != 0) {
            char_move_cmj4(&wk->wu);
            wk->wu.cmwk[7] = 0;
        }

        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void (*const pl12_exatt_table[18])(PLW*) = { Att_HADOUKEN,
                                             Att_SHOURYUUREPPA,
                                             Att_SLIDE_and_JUMP,
                                             Att_ABISEGERI,
                                             Att_CHOUCHUURENGEKI,
                                             Att_SHOURYUUKEN,
                                             Att_CHOUCHUURENGEKI,
                                             Att_KUUCHUUNICHIRINSHOU,
                                             Att_SENPUUKYAKU,
                                             Att_HOMING_JUMP,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_PL12_TOKUSHUKOUDOU,
                                             Att_PL12_BONUS_STAGE,
                                             Att_METAMOR_WAIT,
                                             Att_METAMOR_REBIRTH };
