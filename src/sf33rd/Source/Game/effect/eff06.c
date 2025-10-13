/**
 * @file eff06.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff06.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

const s16 scr_obj_num6[22] = { 1, 6, 4, 0, 3, 1, 0, 2, 8, 3, 0, 4, 3, 0, 1, 1, 4, 0, 1, 1, 0, 0 };

const s16 st0000_data_tbl[8] = { 0, 2, 8492, 640, 48, 82, 0, 0 };

const s16 st0100_data_tbl[48] = { 0, 1, 300,  648, 48, 86, 1,  0, 0, 1, 300,  728, 48, 86, 2,  0,
                                  0, 1, 300,  792, 48, 86, 3,  0, 0, 2, 8492, 648, 16, 10, 18, 0,
                                  0, 2, 8492, 416, 48, 70, 20, 0, 0, 2, 8492, 264, 48, 72, 21, 0 };

const s16 st0B00_data_tbl[32] = { 0, 1, 300, 648, 48, 86, 1, 0, 0, 1, 300, 728, 48,  86, 2,  0,
                                  0, 1, 300, 792, 48, 86, 3, 0, 0, 2, 300, 463, 193, 83, 22, 0 };

const s16 st0200_data_tbl[32] = { 0, 6, 8492, 160, 64, 86, 0, 0, 0, 6, 8492, 224, 64, 86, 1, 0,
                                  0, 6, 8492, 336, 64, 86, 2, 0, 0, 6, 8492, 432, 64, 86, 3, 0 };

const s16 st0A00_data_tbl[48] = { 0, 3, 300, 448, 16, 88, 9,  0, 0, 3, 300, 528, 16, 88, 11, 0,
                                  0, 3, 300, 608, 16, 88, 12, 0, 0, 3, 300, 688, 16, 88, 13, 0,
                                  0, 3, 300, 768, 16, 88, 14, 0, 0, 3, 300, 848, 16, 88, 15, 0 };

const s16 st0400_data_tbl[24] = { 0,  2,  8492, 672, 24, 10, 0,   0,   0, 2,  300, 416,
                                  72, 82, 1,    0,   0,  2,  300, 192, 0, 10, 12,  0 };

const s16 st0500_data_tbl[56] = { 0,   3, 8492, 432, 0,   82,  8, 0,   0,   2, 300, 160, 0,   84,  0, 0,   0,   2, 300,
                                  256, 0, 84,   1,   0,   0,   2, 300, 352, 0, 84,  2,   0,   0,   2, 300, 672, 0, 84,
                                  5,   0, 0,    2,   300, 768, 0, 84,  6,   0, 0,   2,   300, 864, 0, 84,  7,   0 };

const s16 st1300_data_tbl[56] = { 0, 3,  8492, 432, 0, 82, 20,  0,   0, 2,  300, 160, 0, 84, 12,  0,   0, 2,  300, 256,
                                  0, 84, 13,   0,   0, 2,  300, 352, 0, 84, 14,  0,   0, 2,  300, 672, 0, 84, 17,  0,
                                  0, 2,  300,  768, 0, 84, 18,  0,   0, 2,  300, 864, 0, 84, 19,  0 };

const s16 st0700_data_tbl[16] = { 0, 6, 8492, 288, 80, 93, 0, 0, 0, 6, 8492, 416, 48, 93, 1, 0 };

const s16 st0800_data_tbl[88] = { 0,   7,   8492, 256,  43,   88,   10,   0,    0,   7,  8492, 384,  43,   88,   11,
                                  0,   0,   7,    8492, 752,  43,   88,   16,   0,   0,  2,    8492, 192,  48,   86,
                                  0,   0,   0,    2,    8492, 576,  48,   86,   2,   0,  0,    2,    8492, 800,  48,
                                  86,  3,   0,    0,    6,    8492, 640,  128,  85,  6,  0,    0,    6,    8492, 368,
                                  160, 85,  7,    0,    0,    2,    8492, 336,  88,  86, 6,    0,    0,    6,    8492,
                                  514, 136, 86,   8,    0,    0,    6,    8492, 722, 56, 86,   9,    0 };

const s16 st0900_data_tbl[24] = { 0,  3,  8492, 464, 64, 86, 6,    0,   0,  7,  8492, 512,
                                  96, 88, 7,    0,   0,  6,  8492, 832, 16, 10, 5,    0 };

const s16 st0c00_data_tbl[24] = { 0,  3,  8492, 464, 64, 86, 0,    0,   0,  7,  8492, 512,
                                  96, 88, 1,    0,   0,  6,  8492, 832, 16, 10, 5,    0 };

const s16 st0e00_data_tbl[8] = { 0, 2, 8492, 592, 16, 10, 0, 0 };

const s16 st1000_data_tbl[16] = { 0, 2, 8492, 448, 16, 14, 1, 0, 0, 2, 8492, 928, 32, 72, 3, 0 };

const s16 st1100_data_tbl[32] = { 0, 1, 8492, 256, 64,  90, 0, 0, 0, 1, 8492, 384, 128, 90, 1, 0,
                                  0, 1, 8492, 624, 128, 90, 4, 0, 0, 3, 8492, 784, 128, 10, 7, 0 };

const s16 st1400_data_tbl[8] = { 0, 3, 300, 496, 64, 88, 4, 0 };

const s16* scr_obj_data6[22] = { st0000_data_tbl,  st0100_data_tbl,  st0200_data_tbl, stg_dum_data_tbl, st0400_data_tbl,
                                 st0500_data_tbl,  stg_dum_data_tbl, st0700_data_tbl, st0800_data_tbl,  st0900_data_tbl,
                                 st0A00_data_tbl,  st0B00_data_tbl,  st0c00_data_tbl, st0100_data_tbl,  st0e00_data_tbl,
                                 st1000_data_tbl,  st1100_data_tbl,  st0500_data_tbl, st1300_data_tbl,  st1400_data_tbl,
                                 stg_dum_data_tbl, stg_dum_data_tbl };

void effect_06_move(WORK_Other* ewk) {
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
            break;
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_06_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 lp_cnt = scr_obj_num6[bg_w.bg_index];
    s16 i;
    const s16* data_ptr;

    if (lp_cnt == 0) {
        return 0;
    }

    data_ptr = scr_obj_data6[bg_w.bg_index];

    for (i = 0; i < lp_cnt; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 6;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.char_table[0] = char_add[bg_w.bg_index];
        ewk->wu.type = i;
        ewk->wu.dead_f = *data_ptr++;
        ewk->wu.my_family = *data_ptr++;
        ewk->wu.my_col_code = *data_ptr++;
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
        ewk->wu.char_index = *data_ptr++;
        ewk->wu.sync_suzi = *data_ptr++;
        suzi_offset_set(ewk);
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
