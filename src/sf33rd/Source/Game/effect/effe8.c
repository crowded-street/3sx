/**
 * @file effe8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effe8.h"
#include "common.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effe5.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

void effe8_zanzou_process(WORK_Other* ewk, PLW* mwk);
void effE8_trans(WORK_Other* ewk, PLW* mwk);

void effect_E8_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;
    WORK_Other* cwk = (WORK_Other*)ewk->wu.target_adrs;
    WORK* mtwk;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.type = ewk->wu.charset_id;
        ewk->wu.cg_att_ix = 0;
        ewk->wu.cg_hit_ix = 0;

        if (ewk->wu.type >= ewk->wu.charset_id) {
            ewk->wu.type = ewk->wu.charset_id;
            ewk->wu.routine_no[0] = 1;
        }

        effe8_zanzou_process(ewk, mwk);
        effE8_trans(ewk, mwk);
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (Game_pause == 0x81) {
            effE8_trans(ewk, mwk);
            break;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (cwk->wu.routine_no[0] != 1 || cwk->wu.routine_no[1] != 1) {
                ewk->wu.routine_no[1] = 1;
            }

            effe8_zanzou_process(ewk, mwk);
            effE8_trans(ewk, mwk);
            break;

        default:
            ewk->wu.type -= ewk->wu.rl_waza;

            if (ewk->wu.type <= 0) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            } else {
                effe8_zanzou_process(ewk, mwk);
                effE8_trans(ewk, mwk);
            }

            break;
        }

        break;

    case 2:
        if (*pcon_rno != 2) {
            mtwk = (WORK*)mwk->wu.target_adrs;

            if (mtwk->routine_no[0] == 4 && mtwk->routine_no[1] == 1 && mtwk->routine_no[3] < 2) {
                break;
            }
        }

        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effe8_zanzou_process(WORK_Other* ewk, PLW* mwk) {
    if (ewk->wu.old_rno[5]) {
        if (ewk->wu.type == 0) {
            ewk->wu.position_x = mwk->wu.position_x;
            ewk->wu.position_y = mwk->wu.position_y;
        } else {
            ewk->wu.position_x = zanzou_table[ewk->master_id][ewk->wu.type - 1].pos_x;
            ewk->wu.position_y = zanzou_table[ewk->master_id][ewk->wu.type - 1].pos_y;
        }
    } else {
        ewk->wu.position_x = zanzou_table[ewk->master_id][ewk->wu.type].pos_x;
        ewk->wu.position_y = zanzou_table[ewk->master_id][ewk->wu.type].pos_y;
    }

    ewk->wu.position_z = mwk->wu.position_z;

    if (ewk->wu.old_rno[3] == 0) {
        ewk->wu.old_cgnum = ewk->wu.cg_number = mwk->wu.cg_number;
        ewk->wu.rl_flag = mwk->wu.rl_flag;
        ewk->wu.cg_flip = mwk->wu.cg_flip;
    } else {
        ewk->wu.cg_number = zanzou_table[ewk->master_id][ewk->wu.type].cg_num;
        ewk->wu.rl_flag = zanzou_table[ewk->master_id][ewk->wu.type].flip;
        ewk->wu.cg_flip = zanzou_table[ewk->master_id][ewk->wu.type].cg_flp;
    }

    if (ewk->wu.old_rno[0]) {
        ewk->wu.position_z -= ewk->wu.rl_waza;
    } else {
        ewk->wu.position_z += ewk->wu.rl_waza;
    }

    if (ewk->wu.old_rno[4]) {
        if (ewk->wu.olc_work_ix[2] && mwk->metamorphose) {
            ewk->wu.extra_col = after_image_color[ewk->wu.old_rno[4] + ewk->wu.rl_waza - 1][(ewk->master_id + 1) & 1];
        } else {
            ewk->wu.extra_col = after_image_color[ewk->wu.old_rno[4] + ewk->wu.rl_waza - 1][ewk->master_id];
        }
    } else {
        ewk->wu.extra_col = mwk->wu.current_colcd;
    }

    if (ewk->wu.old_rno[1]) {
        get_attdata_of_illusion(ewk);
    }
}

void effE8_trans(WORK_Other* ewk, PLW* mwk) {
    if (ewk->wu.old_rno[3] == 0) {
        return;
    }

    ewk->wu.old_rno[6] = ewk->wu.rl_waza - 1;
    ewk->wu.my_bright_level = (ewk->wu.old_rno[6] * 3) + 1;

    if (mwk->wu.disp_flag != 0 && mwk->sa_stop_flag == 0) {
        sort_push_request(&ewk->wu);
    }
}

s32 effect_E8_init(WORK_Other* ek, PLW* mk, s16 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = ek->wu.disp_flag;
    ewk->wu.id = 148;
    ewk->wu.type = 0;
    ewk->wu.charset_id = data;
    ewk->wu.rl_waza = data / ek->wu.dmcal_m;

    if (ek->wu.old_rno[1]) {
        ewk->wu.work_id = 8;
    } else {
        ewk->wu.work_id = 16;
    }

    effect_e7_e8_init_union(ewk, ek, mk);
    return 0;
}
