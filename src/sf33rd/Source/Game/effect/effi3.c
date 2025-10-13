/**
 * @file effi3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effi3.h"
#include "common.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

const I3_Data i3_data[6] = { { 3, 2, 0 }, { 1, 1, 0 }, { 2, 0, 0 }, { 2, 0, 0 }, { 2, 0, 0 }, { 2, 0, 0 } };

void effect_I3_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        bg_stop = 1;

        switch (i3_data[ewk->wu.type].sour) {
        case 1:
            if ((ewk->wu.dir_timer = ewk->wu.hit_stop) < 0) {
                ewk->wu.dir_timer = -ewk->wu.dir_timer;
            }

            break;

        case 2:
            ewk->wu.dir_timer = ewk->wu.cg_type;
            break;

        default:
            ewk->wu.dir_timer = i3_data[ewk->wu.type].tm;
            break;
        }

        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if (EXE_flag != 0 || Game_pause != 0) {
            break;
        }

        if ((i3_data[ewk->wu.type].flag & 1 && --ewk->wu.dir_timer < 0) ||
            (i3_data[ewk->wu.type].flag & 2 &&
             (ewk->wu.now_koc != mwk->now_koc || ewk->wu.char_index != mwk->char_index))) {
            ewk->wu.routine_no[0] = 2;
        }

        break;

    case 2:
    default:
        bg_stop = 0;
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_I3_init(WORK* wk, u8 tix) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 183;
    ewk->wu.work_id = 16;
    ewk->my_master = (u32*)wk;
    ewk->wu.type = tix;
    ewk->wu.cg_type = wk->cg_type;
    ewk->wu.hit_stop = wk->hit_stop;
    ewk->wu.now_koc = wk->now_koc;
    ewk->wu.char_index = wk->char_index;
    return 0;
}
