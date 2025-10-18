/**
 * @file effc7.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc7.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

void effc7_sort_push(WORK* ewk, WORK*);

const s16 paring_mark_data[3][20][2] = {
    { { 64, 96 }, { 66, 82 }, { 50, 72 }, { 32, 60 }, { 36, 82 }, { 42, 70 }, { 34, 100 },
      { 44, 74 }, { 38, 52 }, { 50, 72 }, { 32, 60 }, { 50, 72 }, { 50, 72 }, { 64, 96 },
      { 50, 72 }, { 26, 76 }, { 34, 74 }, { 24, 90 }, { 28, 59 }, { 34, 86 } },
    { { 60, 40 }, { 32, 44 }, { 28, 34 }, { 22, 40 }, { 51, 18 }, { 28, 44 }, { 36, 40 },
      { 22, 36 }, { 33, 23 }, { 38, 43 }, { 22, 40 }, { 28, 34 }, { 28, 34 }, { 60, 40 },
      { 28, 34 }, { 23, 41 }, { 35, 25 }, { 32, 50 }, { 23, 43 }, { 35, 22 } },
    { { 40, 92 }, { 42, 62 }, { 28, 76 }, { 28, 76 }, { 38, 68 }, { 30, 84 }, { 28, 116 },
      { 28, 56 }, { 38, 52 }, { 40, 60 }, { 28, 76 }, { 28, 76 }, { 28, 76 }, { 40, 92 },
      { 28, 76 }, { 25, 77 }, { 34, 79 }, { 47, 74 }, { 21, 84 }, { 22, 73 } }
};

void effect_C7_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[2].disp.pos = 26;
        ewk->wu.next_z = mwk->position_z;

        if (mwk->rl_flag) {
            ewk->wu.position_x = mwk->position_x + paring_mark_data[ewk->wu.direction][ewk->master_player][0];
        } else {
            ewk->wu.position_x = mwk->position_x - paring_mark_data[ewk->wu.direction][ewk->master_player][0];
        }

        ewk->wu.position_y = mwk->position_y + paring_mark_data[ewk->wu.direction][ewk->master_player][1];

        if (ewk->wu.position_z == ewk->wu.xyz[2].disp.pos) {
            ewk->wu.position_z = ewk->wu.next_z;
        } else {
            ewk->wu.position_z = ewk->wu.xyz[2].disp.pos;
        }

        set_char_move_init(&ewk->wu, 0, 0);
        effc7_sort_push(&ewk->wu, mwk);
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0]++;
                break;
            }
        }

        effc7_sort_push(&ewk->wu, mwk);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effc7_sort_push(WORK* ewk, WORK* /* unused */) {
    sort_push_request8(ewk);
}

s32 effect_C7_init(PLW* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(2)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 127;
    ewk->wu.work_id = 64;
    ewk->wu.rl_flag = wk->wu.rl_flag;
    ewk->wu.direction = data;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x2020;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->my_master = wk;
    ewk->master_id = wk->wu.id;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_player = wk->player_number;
    *ewk->wu.char_table = _ef01_char_table;
    return 0;
}
