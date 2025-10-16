/**
 * @file effh0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effh0.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"

const CONN bbbs_nando_small[2][2] = { { { 176, -16, 0, 32490 }, { 132, -16, 0, 32489 } },
                                      { { -104, -16, 0, 32490 }, { -148, -16, 0, 32489 } } };

void effH0_trans(WORK* ewk);

void effect_H0_move(WORK_Other_CONN* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
            ewk->wu.old_cgnum = 0;
            ewk->wu.dir_timer = 60;
            ewk->wu.position_z = ewk->wu.my_priority = 9;
            ewk->conn[0].chr = (Bonus_Stage_Level % 10) + 32490;
            break;

        case 1:
            if (--ewk->wu.dir_timer <= 0) {
                ewk->wu.disp_flag = 1;
                ewk->wu.routine_no[0] = 1;
                ewk->wu.routine_no[1] = 0;
            }
            break;
        }

        effH0_trans(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        effH0_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effH0_trans(WORK* ewk) {
    ewk->position_x = bg_w.bgw[2].wxy[0].disp.pos;
    ewk->position_y = bg_w.bgw[2].wxy[1].disp.pos;
    sort_push_request3(ewk);
}

s32 effect_H0_init(WORK* wk) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 170;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = 3;
    ewk->wu.cgromtype = 1;
    ewk->wu.type = wk->rl_flag;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 73;
    ewk->num_of_conn = 2;

    if (wk->rl_flag) {
        ix = 1;
    } else {
        ix = 0;
    }

    ewk->conn[0] = bbbs_nando_small[ix][0];
    ewk->conn[1] = bbbs_nando_small[ix][1];
    return 0;
}
