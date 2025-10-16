/**
 * @file effg3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effg3.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void effect_G3_move(WORK_Other* ewk) {
    WORK_Other* mwk;
    PLW* pwk = (PLW*)ewk->wu.target_adrs;
    s16 adjust;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.disp_flag = 1;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 0x2020;
        ewk->wu.position_y = pwk->wu.position_y - 8;
        ewk->wu.position_z = pwk->wu.position_z - 4;
        set_char_move_init(&ewk->wu, 0, 0);
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f || Suicide[6] != 0) {
            ewk->wu.routine_no[0] = 3;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (!ewk->wu.routine_no[1]) {
            mwk = (WORK_Other*)ewk->my_master;

            if (mwk->wu.dead_f) {
                ewk->wu.routine_no[0] = 3;
                ewk->wu.disp_flag = 0;
                break;
            }

            if (mwk->wu.dir_timer <= 0) {
                ewk->wu.routine_no[1]++;
                set_char_move_init(&ewk->wu, 0, 1);
            }
        } else if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[0] = 3;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            if (!ewk->wu.routine_no[1]) {
                adjust = 80;

                if (ewk->wu.rl_flag) {
                    adjust = -adjust;
                }

                ewk->wu.position_x = pwk->wu.position_x + adjust;
            }

            char_move(&ewk->wu);
        }

        sort_push_request(&ewk->wu);
        break;

    default:
    case 2:
    case 3:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_G3_init(WORK* wk, u8 data) {
    WORK_Other* ewk;
    WORK_Other* ewk2;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.type = data;
    ewk->wu.id = 163;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = wk->my_family;
    ewk->wu.cgromtype = 1;
    ewk->my_master = (u32*)wk;
    ewk->master_work_id = wk->work_id;
    ewk->master_id = wk->id;
    ewk2 = (WORK_Other*)wk;
    ewk->wu.target_adrs = ewk2->my_master;
    ewk->wu.rl_flag = data;
    *ewk->wu.char_table = _effD4_char_table;
    return 0;
}
