/**
 * @file efff0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/efff0.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"

void effect_F0_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    if (!ewk->wu.routine_no[0]) {
        if ((ewk->wu.dead_f == 1) || (ewk->wu.dir_old != mwk->now_koc)) {
            ewk->wu.disp_flag = 0;
            push_effect_work(&ewk->wu);
            return;
        }

        ewk->wu.disp_flag = ((mwk->cg_number >= 0x1B59) && (mwk->cg_number < 0x1B5E)) ? 1 : 0;
        ewk->wu.cg_number = mwk->cg_number;
        ewk->wu.position_x = mwk->position_x;
        ewk->wu.position_y = mwk->position_y;
        ewk->wu.position_z = mwk->position_z - 1;
        ewk->wu.rl_flag = mwk->rl_flag;
        ewk->wu.my_col_code = mwk->my_col_code;
        sort_push_request(&ewk->wu);
        return;
    }

    push_effect_work(&ewk->wu);
}

s32 effect_F0_init(WORK* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 150;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = wk;
    ewk->master_work_id = wk->work_id;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = wk->cgromtype;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.disp_flag = 0;
    ewk->wu.dir_old = wk->now_koc;
    effect_F0_move(ewk);
    return 0;
}
