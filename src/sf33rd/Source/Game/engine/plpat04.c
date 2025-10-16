/**
 * @file plpat04.c
 * Dudley Attacks
 */

#include "sf33rd/Source/Game/engine/plpat04.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls02.h"

void (*const pl04_exatt_table[18])(PLW*);

void pl04_extra_attack(PLW* wk) {
    pl04_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_PL04_TOKUSHUKOUDOU(PLW* wk) {
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

        switch (wk->wu.cg_type) {
        case 40:
            wk->wu.cg_type = 0;
            add_sp_arts_gauge_tokushu(wk);
            break;

        case 20:
            wk->wu.cg_type = 0;
            wk->tk_dageki += 8;
            break;

        case 64:
            grade_add_personal_action(wk->wu.id);
            break;
        }

        if (wk->tk_dageki > 8) {
            wk->tk_dageki = 8;
        }

        break;
    }
}

void (*const pl04_exatt_table[18])(PLW*) = { Att_SENPUUKYAKU,     Att_SENPUUKYAKU,    Att_HADOUKEN,
                                             Att_SHOURYUUREPPA,   Att_HADOUKEN2,      Att_CHOUCHUURENGEKI,
                                             Att_CHOUCHUURENGEKI, Att_SLIDE_and_JUMP, Att_SLIDE_and_JUMP,
                                             Att_HADOUKEN,        Att_NM_OKIAGARI,    Att_DUMMY,
                                             Att_HOMING_JUMP,     Att_DUMMY,          Att_PL04_TOKUSHUKOUDOU,
                                             Att_DUMMY,           Att_METAMOR_WAIT,   Att_METAMOR_REBIRTH };
