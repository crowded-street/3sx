/**
 * @file plpat02.c
 * Ryu Attacks
 */

#include "sf33rd/Source/Game/engine/plpat02.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void (*const pl02_exatt_table[18])(PLW*);

const s16 lgix_table[8] = { 0, 1, 2, 3, 3, 4, 4, 5 };

void pl02_extra_attack(PLW* wk) {
    pl02_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_DENJINHADOUKEN(PLW* wk) {
    s16 i;
    s16 lgix;

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

        if (wk->wu.now_koc == 8 && wk->wu.char_index == 13) {
            if (wk->cp->lgp > 13) {
                lgix = 5;
            } else {
                lgix = lgix_table[wk->cp->lgp / 2];
            }

            if (lgix) {
                for (i = 0; i < lgix; i++) {
                    char_move(&wk->wu);
                }
            }
        }

        break;
    }
}

void Att_PL02_TOKUSHUKOUDOU(PLW* wk) {
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
            wk->wu.routine_no[3]++;

            if (wk->tk_success < 3) {
                wk->tk_success++;
                wk->py->recover = (wk->py->recover * 110) / 100;
                grade_add_personal_action(wk->wu.id);
            }
        }

        break;

    default:
        char_move(&wk->wu);
        break;
    }
}

void (*const pl02_exatt_table[18])(PLW*) = { Att_HADOUKEN,
                                             Att_SHOURYUUKEN,
                                             Att_SENPUUKYAKU,
                                             Att_HADOUKEN,
                                             Att_SHINSHOURYUUKEN,
                                             Att_DENJINHADOUKEN,
                                             Att_KUUCHUUNICHIRINSHOU,
                                             Att_SLIDE_and_JUMP,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_DUMMY,
                                             Att_PL02_TOKUSHUKOUDOU,
                                             Att_DUMMY,
                                             Att_METAMOR_WAIT,
                                             Att_METAMOR_REBIRTH };
