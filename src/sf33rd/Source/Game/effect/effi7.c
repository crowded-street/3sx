/**
 * @file effi7.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effi7.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

void effI7_pos_hosei(WORK_Other* ewk, WORK* mwk);

const s16 ex_sign_data[69][4] = {
    { 28, 70, 121, 1 },  { -44, 60, 121, 0 }, { -24, 40, 121, 1 },  { -12, 52, 121, 1 }, { -40, 68, 121, 1 },
    { -32, 64, 121, 0 }, { -32, 64, 121, 1 }, { -20, 88, 121, 0 },  { -16, 72, 121, 1 }, { 40, 60, 121, 0 },
    { -36, 39, 121, 1 }, { 0, 112, 121, 1 },  { -20, 60, 121, 1 },  { -16, 56, 121, 1 }, { -40, 60, 121, 1 },
    { 0, 24, 121, 1 },   { 16, 68, 121, 1 },  { -44, 60, 121, 0 },  { -24, 40, 121, 1 }, { -40, 68, 121, 1 },
    { 16, 32, 121, 1 },  { -8, 40, 121, 1 },  { 20, 36, 121, 1 },   { -8, 72, 121, 1 },  { 0, 40, 121, 1 },
    { 24, 80, 121, 0 },  { 8, 56, 121, 0 },   { 20, 20, 121, 0 },   { 20, 24, 121, 1 },  { 8, 48, 121, 1 },
    { -32, 48, 121, 1 }, { -32, 52, 121, 1 }, { -22, 60, 121, 0 },  { -32, 64, 121, 1 }, { 0, 16, 121, 1 },
    { -8, 36, 121, 1 },  { -24, 36, 121, 1 }, { -16, 40, 121, 1 },  { -16, 32, 121, 0 }, { -12, 20, 121, 0 },
    { 0, 80, 121, 0 },   { 0, 24, 121, 1 },   { -32, 80, 121, 0 },  { -8, 100, 121, 1 }, { -16, 96, 121, 1 },
    { -4, 76, 121, 1 },  { -8, 120, 121, 1 }, { -16, 112, 121, 1 }, { -38, 88, 121, 1 }, { -33, 61, 121, 1 },
    { -47, 60, 121, 0 }, { -31, 56, 121, 0 }, { 29, 88, 121, 1 },   { 31, 67, 121, 1 },  { -62, 52, 121, 1 },
    { -14, 82, 121, 1 }, { -8, 64, 121, 1 },  { -48, 64, 121, 0 },  { -48, 64, 121, 0 }, { -13, 87, 121, 1 },
    { -13, 87, 121, 1 }, { -18, 88, 121, 0 }, { -21, 78, 121, 1 },  { -52, 67, 121, 0 }, { -18, 57, 121, 0 },
    { -56, 65, 121, 0 }, { -37, 78, 121, 1 }, { -18, 58, 121, 1 },  { -42, 84, 121, 1 }
};

void effect_I7_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 0x2020;
        effI7_pos_hosei(ewk, &mwk->wu);
        ewk->wu.position_z = mwk->wu.position_z - 4;
        set_char_move_init(&ewk->wu, 0, ex_sign_data[ewk->wu.type][2]);
        sort_push_request(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || mwk->wu.routine_no[1] != 4) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            if (ewk->wu.hit_stop) {
                ewk->wu.hit_stop--;
            } else {
                char_move(&ewk->wu);
            }
        }

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (ex_sign_data[ewk->wu.type][3]) {
            effI7_pos_hosei(ewk, &mwk->wu);
        }

        ewk->wu.position_z = mwk->wu.position_z - 4;
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effI7_pos_hosei(WORK_Other* ewk, WORK* mwk) {
    ewk->wu.position_x = mwk->position_x;

    if (mwk->rl_flag) {
        ewk->wu.position_x -= ex_sign_data[ewk->wu.type][0];
    } else {
        ewk->wu.position_x += ex_sign_data[ewk->wu.type][0];
    }

    ewk->wu.position_y = mwk->position_y + ex_sign_data[ewk->wu.type][1];
}

s32 effect_I7_init(PLW* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if (test_flag) {
        return 0;
    }

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.type = data;
    ewk->wu.id = 187;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->wu.cgromtype = 1;
    ewk->my_master = wk;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    *ewk->wu.char_table = _plef_char_table;
    return 0;
}
