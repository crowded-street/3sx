/**
 * @file efff2.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/efff2.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 efff2_data_tbl1[10][4] = { { 1, 384, 608, 80 }, { 2, 592, 496, 86 }, { 1, 640, 576, 80 }, { 2, 336, 584, 86 },
                                     { 1, 464, 584, 80 }, { 1, 544, 640, 80 }, { 2, 640, 512, 86 }, { 1, 480, 592, 80 },
                                     { 2, 416, 640, 86 }, { 1, 576, 576, 80 } };

const s32 efff2_sp_tbl1[10][2] = { { -0xC000, 0 }, { -0x1800, 0 }, { 0x2000, 0 },  { -0x2000, 0 }, { -0x2000, 0 },
                                   { 0, 0 },       { 0x3000, 0 },  { -0x8000, 0 }, { -0x6000, 0 }, { 0x6000, 0 } };

const s16 efff2_timer_tbl[16] = { 60, 0, 40, 90, 20, 10, 8, 130, 1, 34, 50, 70, 6, 80, 22, 100 };

void effect_F2_move(WORK_Other* ewk) {
    s16 work;

    if (ewk->wu.old_rno[6] < end_w.r_no_2) {
        ewk->wu.routine_no[1] = 99;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[4], ewk->wu.char_index, 0);
        work = random_16();
        work &= 0xF;
        ewk->wu.old_rno[5] = efff2_timer_tbl[work];
        break;

    case 1:
        ewk->wu.old_rno[5]--;

        if (ewk->wu.old_rno[5] <= 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 9) {
            ewk->wu.routine_no[1]++;
            ewk->wu.cg_type = 0;
            ewk->wu.mvxy.a[0].sp = efff2_sp_tbl1[ewk->wu.type][0];
            ewk->wu.mvxy.d[0].sp = efff2_sp_tbl1[ewk->wu.type][1];
            ewk->wu.mvxy.a[1].sp = -0x18000;
            ewk->wu.mvxy.d[1].sp = -0x400;
        }

        disp_pos_trans_entry(ewk);
        break;

    case 3:
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos < 256) {
            ewk->wu.routine_no[1] = 0;
            ewk->wu.xyz[0].disp.pos = efff2_data_tbl1[ewk->wu.type][1];
            ewk->wu.xyz[1].disp.pos = efff2_data_tbl1[ewk->wu.type][2];
            ewk->wu.old_rno[4] = 19;
            ewk->wu.char_index = 1;
            break;
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_F2_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    const s16* data_ptr = &efff2_data_tbl1[0][0];

    for (i = 0; i < 10; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.id = 152;
        ewk->wu.be_flag = 1;
        ewk->wu.work_id = 0x10;
        ewk->wu.cgromtype = 1;
        ewk->wu.type = i;
        ewk->wu.old_rno[0] = end_w.r_no_2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.char_table[0] = (u32*)_end_char_table;
        ewk->wu.my_col_code = 0x12C;
        ewk->wu.my_family = *data_ptr++;
        ewk->wu.old_rno[6] = 0;
        ewk->wu.old_rno[6] += end_w.r_no_2;
        ewk->wu.old_rno[4] = 19;
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
        ewk->wu.char_index = 1;
        ewk->wu.old_rno[1] = 1;
        ewk->wu.my_mts = 8;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
