/**
 * @file effl0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effl0.h"
#include "common.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

void effect_L0_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        mwk->wu.disp_flag = 0;
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 0 && Suicide[0] == 0) {
            if (Game_pause || EXE_flag) {
                break;
            }

            if (mwk->sa_stop_flag != 1) {
                ewk->wu.dir_timer--;
            }

            if (ewk->wu.dead_f == 0 && ewk->wu.dir_timer > 0 && mwk->wu.routine_no[1] != 1 &&
                mwk->wu.routine_no[1] != 2 && mwk->wu.routine_no[1] != 3 &&
                (mwk->wu.now_koc != 5 ||
                 (!(mwk->wu.kind_of_waza & 0x20) && mwk->wu.char_index != 0x40 && mwk->wu.char_index != 1))) {

                if (ewk->wu.dir_timer >= 30) {
                    break;
                }

                mwk->wu.my_bright_type = 1;

                if (ewk->wu.dir_timer < 10) {
                    mwk->wu.disp_flag = 1;
                    mwk->wu.my_bright_level = ewk->wu.dir_timer;
                } else {
                    mwk->wu.disp_flag = 2;
                    mwk->wu.my_bright_level = 13;
                    mwk->wu.my_col_mode = 0x4400;
                }

                break;
            }

            mwk->wu.disp_flag = 1;
            mwk->wu.my_bright_type = 0;
            mwk->wu.my_bright_level = 0;
            mwk->wu.my_clear_level = 0;
            mwk->wu.my_col_mode = 0x4200;
        }

        ewk->wu.routine_no[0]++;
        break;

    default:
    case 2:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_L0_init(WORK* wk, s16 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->my_master = (u32*)wk;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 210;
    ewk->wu.dir_timer = data;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code;
    ewk->wu.now_koc = wk->now_koc;
    ewk->wu.char_index = wk->char_index;
    effect_L0_move(ewk);
    return 0;
}
