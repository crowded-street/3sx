/**
 * @file efff8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/efff8.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

// forward declaration

const s16 paring_b_mark_data[3][20][2];

void effect_F8_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[2].disp.pos = 26;
        ewk->wu.next_z = mwk->position_z;
        if (mwk->rl_flag) {
            ewk->wu.position_x = mwk->position_x + paring_b_mark_data[ewk->wu.direction][ewk->master_player][0];
        } else {
            ewk->wu.position_x = mwk->position_x - paring_b_mark_data[ewk->wu.direction][ewk->master_player][0];
        }

        ewk->wu.position_y = mwk->position_y + paring_b_mark_data[ewk->wu.direction][ewk->master_player][1];
        if (ewk->wu.position_z == ewk->wu.xyz[2].disp.pos) {
            ewk->wu.position_z = ewk->wu.next_z;
        } else {
            ewk->wu.position_z = ewk->wu.xyz[2].disp.pos;
        }

        set_char_move_init(&ewk->wu, 0, 3);
        sort_push_request(&ewk->wu);
        return;

    case 1:
        if ((ewk->wu.dead_f == 1) || (Suicide[0] != 0)) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] += 1;
            return;
        }

        if ((EXE_flag == 0) && (Game_pause == 0)) {
            char_move(&ewk->wu);
            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] += 1;
                return;
            }
        }

        sort_push_request(&ewk->wu);
        return;

    case 2:
        ewk->wu.routine_no[0] = 3;
        return;

    default:
        push_effect_work(&ewk->wu);
        return;
    }
}

s32 effect_F8_init(PLW* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(2)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 158;
    ewk->wu.work_id = 64;
    ewk->wu.rl_flag = wk->wu.rl_flag;
    ewk->wu.direction = data;
    ewk->wu.my_mts = 14;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 0x2020;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->my_master = wk;
    ewk->master_id = wk->wu.id;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_player = wk->player_number;
    ewk->wu.char_table[0] = _ef01_char_table;
    return 0;
}

const s16 paring_b_mark_data[3][20][2] = {
    { { 28, 100 }, { 12, 88 }, { 10, 84 }, { 8, 76 },  { 14, 88 }, { 16, 86 }, { 24, 104 },
      { 8, 70 },   { 20, 82 }, { 28, 70 }, { 8, 76 },  { 10, 84 }, { 10, 84 }, { 28, 100 },
      { 10, 84 },  { 10, 72 }, { 27, 64 }, { 28, 93 }, { 28, 71 }, { 24, 80 } },
    { { 12, 52 }, { 26, 52 }, { 16, 50 }, { 10, 48 }, { 24, 60 }, { 24, 66 }, { 26, 68 },
      { 16, 46 }, { 28, 44 }, { 26, 44 }, { 10, 48 }, { 16, 50 }, { 16, 50 }, { 12, 52 },
      { 16, 50 }, { 0, 0 },   { 27, 39 }, { 35, 44 }, { 18, 57 }, { 27, 43 } },
    { { 40, 92 }, { 42, 62 }, { 28, 76 }, { 28, 76 }, { 38, 68 }, { 30, 84 }, { 28, 108 },
      { 28, 56 }, { 38, 52 }, { 40, 60 }, { 28, 76 }, { 32, 48 }, { 32, 48 }, { 40, 92 },
      { 28, 76 }, { 28, 64 }, { 28, 76 }, { 52, 95 }, { 20, 97 }, { 26, 77 } }
};
