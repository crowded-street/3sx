/**
 * @file eff71.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff71.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff72.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 eff71_time_tbl[8] = { 2, 8, 12, 9, 4, 6, 50, 3 };

void effect_71_move(WORK_Other* ewk) {
    s16 work;

    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] <= 0) {
                ewk->wu.routine_no[0]++;
                ewk->wu.disp_flag = 1;
                ewk->wu.old_rno[1] = 0;
                effect_72_init(ewk, 0);
                effect_72_init(ewk, 1);
            }
        }

        break;

    case 1:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.old_rno[1] = 0;
            work = random_16();
            work &= 7;
            ewk->wu.old_rno[0] = eff71_time_tbl[work];
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_71_init() {
    WORK_Other* ewk;
    s16 ix;

    if (EXE_obroll) {
        return 0;
    }

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 71;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.disp_flag = 0;
    ewk->wu.old_rno[0] = 0;
    return 0;
}
