/**
 * @file effj2.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effj2.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/workuser.h"

void effJ2_trans(WORK* ewk);

const CONN bbbs_nando_large[2] = { { 92, 78, 0, 0x7EC7 }, { -20, 78, 0, 0x7EC6 } };

void effect_J2_move(WORK_Other_CONN* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
            ewk->wu.old_cgnum = 0;
            break;
        }

        if (--ewk->wu.dir_timer > 0) {
            break;
        }

        ewk->wu.routine_no[0] = 1;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_cgnum = 0;
        ewk->wu.dir_timer = 60;
        ewk->wu.position_z = ewk->wu.my_priority = 27;
        ewk->conn[0].chr += Bonus_Stage_Level % 10;
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Break_Into || --ewk->wu.dir_timer <= 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
        } else {
            effJ2_trans(&ewk->wu);
        }

        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effJ2_trans(WORK* ewk) {
    ewk->position_x = bg_w.bgw[1].wxy[0].disp.pos;
    ewk->position_y = bg_w.bgw[1].wxy[1].disp.pos;
    sort_push_request3(ewk);
}

s32 effect_J2_init(s16 delay) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 192;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = 2;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x49;
    ewk->wu.dir_timer = delay;
    ewk->num_of_conn = 2;
    ewk->conn[0] = bbbs_nando_large[0];
    ewk->conn[1] = bbbs_nando_large[1];
    return 0;
}
