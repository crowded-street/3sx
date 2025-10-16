/**
 * @file effb0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effb0.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

const s16 effb0_timer_tbl[8] = { 60, 30, 120, 150, 8, 20, 10, 90 };

const s16 effb0_data_tbl[8][2] = { { 656, 112 }, { 328, 96 },  { 352, 224 }, { 688, 144 },
                                   { 344, 128 }, { 640, 224 }, { 336, 176 }, { 672, 72 } };

void effect_B0_move(WORK_Other* ewk) {
    s16 work;

    if (ewk->wu.old_rno[6] < end_w.r_no_2) {
        ewk->wu.routine_no[0] = 99;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.old_rno[5]--;

        if (ewk->wu.old_rno[5] <= 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.disp_flag = 0;
            work = random_16();
            work &= 7;
            ewk->wu.old_rno[5] = effb0_timer_tbl[work];
            work = random_16();
            work &= 7;
            ewk->wu.xyz[0].disp.pos = effb0_data_tbl[work][0];
            work = random_16();
            work &= 7;
            ewk->wu.xyz[1].disp.pos = effb0_data_tbl[work][1];
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_B0_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    s16 work;

    for (i = 0; i < 8; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.id = 110;
        ewk->wu.be_flag = 1;
        ewk->wu.type = i;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.old_rno[0] = end_w.r_no_2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.char_table[0] = (u32*)_end_char_table;
        ewk->wu.my_col_code = 300;
        ewk->wu.my_family = 1;
        ewk->wu.old_rno[6] = 0;
        ewk->wu.old_rno[6] += end_w.r_no_2;
        ewk->wu.xyz[0].disp.pos = effb0_data_tbl[i][0];
        ewk->wu.xyz[1].disp.pos = effb0_data_tbl[i][1];
        ewk->wu.my_priority = ewk->wu.position_z = 80;
        ewk->wu.char_index = 29;
        work = random_16();
        work &= 7;
        ewk->wu.old_rno[5] = effb0_timer_tbl[work];
        ewk->wu.my_mts = 8;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
