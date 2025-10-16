/**
 * @file plpat08.c
 * Elena Attacks
 */

#include "sf33rd/Source/Game/engine/plpat08.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls02.h"

const u8 pl08_hcs_tbl[8] = { 0, 0, 0, 1, 0, 1, 1, 1 };

void (*const pl08_exatt_table[18])(PLW*);

void pl08_extra_attack(PLW* wk) {
    pl08_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

void Att_PL08_HEALING(PLW* wk) {
    u16 cpsw;

    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cmwk[0]) {
            cpsw = (wk->cp->sw_now & 0x770);
            cpsw >>= 4;

            if (pl08_hcs_tbl[cpsw & 7] || pl08_hcs_tbl[(cpsw >> 4) & 7]) {
                wk->wu.cmwk[0] = 0;
                char_move_cmms(&wk->wu);
            }
        }

        if (pcon_dp_flag == 0) {
            switch (wk->wu.cg_type) {
            case 24:
                wk->wu.vital_new += 3;
                break;

            case 22:
                wk->wu.vital_new += 2;
                break;

            case 20:
                wk->wu.vital_new += 1;
                break;
            }
        }

        if (wk->wu.vital_new > wk->wu.vitality) {
            wk->wu.vital_new = wk->wu.vitality;
            wk->sa_healing = 1;
        }

        break;
    }
}

void Att_PL08_TOKUSHUKOUDOU(PLW* wk) {
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
            wk->tk_kizetsu += 6;

            if (wk->tk_kizetsu > 24) {
                wk->tk_kizetsu = 24;
            }

            grade_add_personal_action(wk->wu.id);
        }

        break;

    default:
        char_move(&wk->wu);
        break;
    }
}

void (*const pl08_exatt_table[18])(PLW*) = { Att_SHOURYUUKEN,   Att_SENPUUKYAKU,    Att_SENPUUKYAKU,
                                             Att_SHOURYUUREPPA, Att_SHOURYUUREPPA,  Att_PL08_HEALING,
                                             Att_DUMMY,         Att_SLIDE_and_JUMP, Att_HADOUKEN,
                                             Att_DUMMY,         Att_DUMMY,          Att_DUMMY,
                                             Att_DUMMY,         Att_DUMMY,          Att_PL08_TOKUSHUKOUDOU,
                                             Att_DUMMY,         Att_METAMOR_WAIT,   Att_METAMOR_REBIRTH };
