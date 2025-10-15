/**
 * @file eff44.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff44.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 scr_obj_num44[10] = { 2, 1, 2, 7, 1, 3, 3, 3, 4, 2 };

const s16 eff44_data_tbl0[16] = { 0, 7, 300, 576, 43, 88, 14, 0, 0, 7, 300, 640, 43, 88, 15, 0 };

const s16 eff44_data_tbl1[8] = { 0, 2, 300, 607, 156, 80, 0, 0 };

const s16 eff44_data_tbl2[16] = { 0, 2, 300, 495, 48, 80, 5, 0, 0, 2, 300, 479, 48, 79, 8, 0 };

const s16 eff44_data_tbl3[56] = { 0,    2,   8492, 672, 208,  82,  3,    0,   0,    2,   8492, 741, 183,  82,
                                  4,    0,   0,    2,   8492, 780, 161,  82,  5,    0,   0,    2,   8492, 809,
                                  161,  82,  6,    0,   0,    2,   8492, 159, 80,   80,  9,    0,   0,    2,
                                  8492, 792, 72,   82,  10,   0,   0,    2,   8492, 654, 64,   82,  11,   0 };

const s16 eff44_data_tbl4[8] = { 0, 2, 300, 735, 80, 82, 23, 0 };

const s16 eff44_data_tbl5[24] = { 0,  2,  8492, 223, 112, 82, 1,    0,   0,  2,  8492, 208,
                                  32, 82, 3,    0,   0,   2,  8492, 320, 32, 82, 4,    0 };

const s16 eff44_data_tbl6[24] = { 0,   2,  8492, 336, 147, 82, 6,    0,   0,   2,  8492, 863,
                                  104, 82, 7,    0,   0,   2,  8492, 863, 104, 82, 25,   0 };

const s16 eff44_data_tbl7[40] = { 0,  2,  8492, 640, 32,  81,  1,   0,   0,  2,  300, 192, 32,   81,
                                  3,  0,  0,    2,   300, 200, 88,  80,  4,  0,  0,   2,   8492, 720,
                                  80, 83, 5,    0,   0,   2,   300, 286, 64, 82, 6,   0 };

const s16 eff44_data_tbl8[32] = { 0, 2, 8492, 240, 80, 72, 16, 0, 0, 2, 8492, 319, 80, 72, 17, 0,
                                  0, 2, 8492, 401, 95, 72, 18, 0, 0, 2, 8492, 864, 80, 72, 21, 0 };

const s16 eff44_data_tbl9[16] = { 0, 2, 8492, 720, 80, 83, 5, 0, 0, 2, 300, 286, 64, 82, 6, 0 };

const s16* scr_obj_data44[10] = { eff44_data_tbl0, eff44_data_tbl1, eff44_data_tbl2, eff44_data_tbl3, eff44_data_tbl4,
                                  eff44_data_tbl5, eff44_data_tbl6, eff44_data_tbl7, eff44_data_tbl8, eff44_data_tbl9 };

void effect_44_move(WORK_Other* ewk) {
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

        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_44_init(s16 type) {
    WORK_Other* ewk;
    s16 ix;
    s16 lp_cnt = scr_obj_num44[type];
    s16 i;
    const s16* data_ptr;

    if (lp_cnt == 0) {
        return 0;
    }

    data_ptr = scr_obj_data44[type];

    for (i = 0; i < lp_cnt; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 44;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;

        if (type == 2) {
            ewk->wu.hit_stop = -77;
        }

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
        suzi_offset_set(ewk);
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

        if (type == 6 && i == 2) {
            ewk->wu.my_mr_flag = 1;
            ewk->wu.my_mr.size.x = 127;
            ewk->wu.my_mr.size.y = 127;
            ewk->wu.xyz[0].disp.pos -= 4;
            ewk->wu.xyz[1].disp.pos += 2;
        }
    }

    return 0;
}
