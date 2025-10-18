/**
 * @file effg4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effg4.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

const GillEffData gill_eff_data[54] = {
    { 0, 0, -1, 92 },      { 0, 0, -1, 93 },      { 6, -10, -1, 94 },    { 12, -1, -1, 95 },    { -6, -1, -1, 96 },
    { 42, 10, -1, 97 },    { -24, 0, -1, 98 },    { -28, 16, -1, 99 },   { 19, -2, -1, 100 },   { 12, 0, -1, 101 },
    { -1, 18, -1, 102 },   { -9, 7, 1, 103 },     { 5, 14, -1, 104 },    { 47, 28, -1, 105 },   { 45, 100, -1, 164 },
    { 50, 88, -1, 165 },   { 14, 93, -2, 165 },   { 14, 104, -2, 164 },  { 36, 34, -1, 164 },   { 39, 23, -1, 165 },
    { -12, 46, -2, 165 },  { -22, 56, -2, 164 },  { 54, 100, -1, 164 },  { 54, 90, -1, 165 },   { 14, 99, -2, 165 },
    { 10, 108, -2, 164 },  { 54, 101, -1, 164 },  { 54, 96, -1, 165 },   { -54, 96, -2, 165 },  { -54, 101, -2, 164 },
    { 48, 94, -1, 164 },   { 48, 88, -1, 165 },   { 0, 0, -1, 163 },     { 0, 0, -1, 163 },     { -96, 102, -1, 106 },
    { -96, 102, -1, 107 }, { -40, 124, -1, 112 }, { -40, 124, -1, 113 }, { -37, 138, -2, 108 }, { -37, 138, -2, 109 },
    { -51, 128, -2, 108 }, { -51, 128, -2, 109 }, { -48, 136, -2, 108 }, { -48, 136, -2, 109 }, { -40, 126, -2, 108 },
    { -40, 126, -2, 109 }, { -41, 130, -2, 110 }, { -41, 130, -2, 111 }, { -46, 136, -2, 110 }, { -46, 136, -2, 111 },
    { -16, 140, -2, 110 }, { -16, 140, -2, 111 }, { -40, 144, -2, 110 }, { -40, 144, -2, 111 }
};

void effect_G4_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;

        if (ewk->wu.rl_flag) {
            ewk->wu.position_x = ewk->wu.xyz[0].disp.pos + gill_eff_data[ewk->wu.type].hx;
        } else {
            ewk->wu.position_x = ewk->wu.xyz[0].disp.pos - gill_eff_data[ewk->wu.type].hx;
        }

        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos + gill_eff_data[ewk->wu.type].hy;
        ewk->wu.position_z = ewk->wu.xyz[2].disp.pos + gill_eff_data[ewk->wu.type].hz;
        set_char_move_init(&ewk->wu, 0, gill_eff_data[ewk->wu.type].chix);
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
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

s32 effect_G4_init(WORK* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 164;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.type = data;
    ewk->wu.rl_flag = wk->rl_flag;
    ewk->wu.my_family = wk->my_family;
    ewk->wu.cgromtype = wk->cgromtype;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code;

    if (data >= 38) {
        ewk->wu.my_col_code &= ~0x2000;
    }

    if (wk->rl_flag) {
        ewk->wu.type++;
    }

    ewk->my_master = wk;
    ewk->master_work_id = wk->work_id;
    ewk->master_id = wk->id;
    ewk->wu.xyz[0].disp.pos = wk->position_x;
    ewk->wu.xyz[1].disp.pos = wk->position_y;
    ewk->wu.xyz[2].disp.pos = wk->position_z;
    *ewk->wu.char_table = _plef_char_table;
    return 0;
}
