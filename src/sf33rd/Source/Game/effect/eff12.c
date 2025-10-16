/**
 * @file eff12.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff12.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 scr_obj_num12[6] = { 1, 1, 2, 3, 3, 2 };

const s16 eff12_data_tbl0[8] = { 0, 1, 300, 512, 128, 86, 8, 0 };

const s16 eff12_data_tbl1[8] = { 0, 2, 8492, 511, 56, 80, 2, 0 };

const s16 eff12_data_tbl2[16] = { 0, 2, 8492, 559, 64, 72, 2, 0, 0, 2, 8492, 463, 112, 72, 4, 0 };

const s16 eff12_data_tbl3[24] = { 0,  1,  300, 496, 64, 86, 2,    0,   0,  2,  8492, 607,
                                  96, 83, 8,   0,   0,  2,  8492, 607, 96, 83, 26,   0 };

const s16 eff12_data_tbl4[24] = { 0,   2,  300, 511, 0, 11, 5,   0,   0, 2,  8492, 560,
                                  168, 79, 9,   0,   0, 2,  300, 512, 0, 76, 10,   0 };

const s16 eff12_data_tbl5[16] = { 0, 2, 8492, 575, 80, 74, 20, 0, 0, 2, 8492, 464, 64, 72, 19, 0 };

const s16* scr_obj_data12[6] = { eff12_data_tbl0, eff12_data_tbl1, eff12_data_tbl2,
                                 eff12_data_tbl3, eff12_data_tbl4, eff12_data_tbl5 };

void effect_12_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_12_init(s16 type) {
    WORK_Other* ewk;
    s16 ix;
    s16 lp_cnt = scr_obj_num12[type];
    s16 i;
    const s16* data_ptr;

    if (lp_cnt == 0) {
        return 0;
    }

    data_ptr = scr_obj_data12[type];

    for (i = 0; i < lp_cnt; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 12;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.type = i;
        ewk->wu.dead_f = *data_ptr++;
        ewk->wu.my_family = *data_ptr++;
        ewk->wu.my_col_code = *data_ptr++;
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
        ewk->wu.char_index = *data_ptr++;
        ewk->wu.sync_suzi = *data_ptr++;
        ewk->wu.char_table[0] = char_add[bg_w.bg_index];
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

        if (type == 3 && i == 2) {
            ewk->wu.my_mr_flag = 1;
            ewk->wu.my_mr.size.x = 127;
            ewk->wu.my_mr.size.y = 127;
        }
    }

    return 0;
}
