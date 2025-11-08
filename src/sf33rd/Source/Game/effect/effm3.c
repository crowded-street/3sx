/**
 * @file effm3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effm3.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"

const s16 M3_bahn_data[5] = { 16, 10, 78, 0, -512 };

void effM3_trans(WORK* ewk);

void effect_M3_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] = 1;
        ewk->wu.disp_flag = 0;
        ewk->wu.my_mts = 13;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 0x90;
        ewk->wu.my_family = 3;
        ewk->wu.position_z = 60 - (ewk->wu.type + 2);
        ewk->wu.dmcal_m = M3_bahn_data[0];
        ewk->wu.dmcal_d = M3_bahn_data[1];
        ewk->wu.dir_timer = M3_bahn_data[2];
        ewk->wu.old_cgnum = 0;
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[2] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (!(Next_Step & 1)) {
                break;
            }

            ewk->wu.routine_no[1]++;
            ewk->wu.mvxy.a[0].real.h = 64;
            ewk->wu.mvxy.a[0].real.l = -1;
            ewk->wu.mvxy.d[0].real.h = -1;
            ewk->wu.mvxy.d[0].real.l = M3_bahn_data[4] * 16;
            ewk->wu.mvxy.kop[0] = 1;
            ewk->wu.my_mr_flag = 1;
            /* fallthrough */

        case 1:
            if (--ewk->wu.dir_timer >= 0) {
                break;
            }

            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            /* fallthrough */

        case 2:
            cal_mvxy_speed(&ewk->wu);
            ewk->wu.mvxy.d[0].sp = (ewk->wu.mvxy.d[0].sp * ewk->wu.dmcal_m) / ewk->wu.dmcal_d;

            if (!ewk->wu.mvxy.a[0].real.h) {
                ewk->wu.routine_no[1]++;

                if (ewk->wu.type == 0) {
                    Next_Step = 0;
                }
            }

            break;

        default:
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        ewk->wu.my_mr.size.x = ewk->wu.my_mr.size.y = ewk->wu.mvxy.a[0].real.h + 63;
        effM3_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effM3_trans(WORK* ewk) {
    ewk->position_x = bg_w.bgw[ewk->my_family - 1].wxy[0].disp.pos;
    ewk->position_y = bg_w.bgw[ewk->my_family - 1].wxy[1].disp.pos;
    ewk->position_x += ewk->xyz[0].disp.pos;
    ewk->position_y += ewk->xyz[1].disp.pos;
    sort_push_request4(ewk);
}

s32 effect_M3_init(WORK_Other_CONN* wk, s16 num) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 223;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.type = num;
    ewk->wu.xyz[0].disp.pos = wk->conn[num].nx;
    ewk->wu.xyz[1].disp.pos = wk->conn[num].ny + 40;
    ewk->wu.cg_number = wk->conn[num].chr;
    return 0;
}
