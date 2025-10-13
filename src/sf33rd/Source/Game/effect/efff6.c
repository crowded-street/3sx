/**
 * @file efff6.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/efff6.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

#include "sf33rd/Source/Game/opening/opening.h"

// forward decls
void efff6_move_common(WORK_Other* ewk);
void efff6_move01(WORK_Other* ewk);

void effect_F6_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (ewk->wu.old_rno[1] <= op_w.index) {
            ewk->wu.routine_no[0] += 1;
        } else if (ewk->wu.old_rno[2] <= op_w.index) {
            if (ewk->wu.type == 0x1B) {
                efff6_move01(ewk);
            } else {
                efff6_move(ewk);
            }
        }
        break;
    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void efff6_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);
        if (ewk->wu.my_mr.size.x == 0x3F) {
            ewk->wu.my_mr_flag = 0;
        } else {
            ewk->wu.my_mr_flag = 1;
        }
        if (ewk->wu.type == 0x36) {
            push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 0x3C, 8);
            push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 0x3D, 9);
            push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 0x3E, 10);
            ewk->wu.my_col_code = 8;
        } else {
            push_color_trans_req(ewk->wu.my_col_code & 0x1FF, 0);
            ewk->wu.my_col_code = 0;
        }
        /* fallthrough */
    case 1:
        efff6_move_common(ewk);
        disp_pos_trans_entry5(ewk);
        break;
    }
}

void efff6_move_common(WORK_Other* ewk) {
    s16 work;

    switch (ewk->wu.routine_no[3]) {
    case 0:
        switch (ewk->wu.direction) {
        case 0x20:
            add_x_sub(ewk);
            if (ewk->wu.xyz[0].disp.pos <= ewk->wu.old_rno[3]) {
                ewk->wu.routine_no[3] += 1;
                ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[3];
            }
            break;
        default:
            break;
        case 0x10:
            add_x_sub(ewk);
            if (ewk->wu.xyz[0].disp.pos >= ewk->wu.old_rno[3]) {
                ewk->wu.routine_no[3] += 1;
                ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[3];
            }
            break;
        case 0x200:
            add_y_sub(ewk);
            if (ewk->wu.xyz[1].disp.pos <= ewk->wu.old_rno[4]) {
                ewk->wu.routine_no[3] += 1;
                ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[4];
            }
            break;
        case 0x100:
            add_y_sub(ewk);
            if (ewk->wu.xyz[1].disp.pos >= ewk->wu.old_rno[4]) {
                ewk->wu.routine_no[3] += 1;
                ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[4];
            }
            break;
        case 0x120:
            work = 0;
            if (ewk->wu.xyz[0].disp.pos > ewk->wu.old_rno[3]) {
                add_x_sub(ewk);
            } else {
                ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[3];
                work |= 1;
            }
            if (ewk->wu.xyz[1].disp.pos < ewk->wu.old_rno[4]) {
                add_y_sub(ewk);
            } else {
                ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[4];
                work |= 0x10;
            }
            if (work == 0x11) {
                ewk->wu.routine_no[3] += 1;
            }
            break;
        case 0x220:
            work = 0;
            if (ewk->wu.xyz[0].disp.pos > ewk->wu.old_rno[3]) {
                add_x_sub(ewk);
            } else {
                ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[3];
                work |= 1;
            }
            if (ewk->wu.xyz[1].disp.pos > ewk->wu.old_rno[4]) {
                add_y_sub(ewk);
            } else {
                ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[4];
                work |= 0x10;
            }
            if (work == 0x11) {
                ewk->wu.routine_no[3] += 1;
            }
            break;
        case 0x110:
            work = 0;
            if (ewk->wu.xyz[0].disp.pos < ewk->wu.old_rno[3]) {
                add_x_sub(ewk);
            } else {
                ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[3];
                work |= 1;
            }
            if (ewk->wu.xyz[1].disp.pos < ewk->wu.old_rno[4]) {
                add_y_sub(ewk);
            } else {
                ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[4];
                work |= 0x10;
            }
            if (work == 0x11) {
                ewk->wu.routine_no[3] += 1;
            }
            break;
        case 0x210:
            work = 0;
            if (ewk->wu.xyz[0].disp.pos < ewk->wu.old_rno[3]) {
                add_x_sub(ewk);
            } else {
                ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[3];
                work |= 1;
            }
            if (ewk->wu.xyz[1].disp.pos > ewk->wu.old_rno[4]) {
                add_y_sub(ewk);
            } else {
                ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[4];
                work |= 0x10;
            }
            if (work == 0x11) {
                ewk->wu.routine_no[3] += 1;
            }
            break;
        }
        break;
    case 1:
        break;
    }
}

void efff6_move01(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);
        if (ewk->wu.my_mr.size.x == 0x3F) {
            ewk->wu.my_mr_flag = 0;
        } else {
            ewk->wu.my_mr_flag = 1;
        }
        ewk->wu.old_rno[5] = 1;
        ewk->wu.old_rno[6] = 0;
        push_color_trans_req(ewk->wu.my_col_code & 0x1FF, 0);
        ewk->wu.my_col_code = 0;
        /* fallthrough */
    case 1:
        ewk->wu.old_rno[5] -= 1;
        if (ewk->wu.old_rno[5] <= 0) {
            ewk->wu.old_rno[5] = 2;
            ewk->wu.old_rno[6] += 1;
            if (ewk->wu.old_rno[6] >= 6) {
                ewk->wu.routine_no[2] += 1;
            } else {
                push_color_trans_req(efff6_move01_tbl[ewk->wu.old_rno[6]] + 0x12C, 0);
            }
        }
        /* fallthrough */
    case 2:
        efff6_move_common(ewk);
        disp_pos_trans_entry5(ewk);
        break;
    }
}

s32 effect_F6_init(u8 typenum) {
    WORK_Other* ewk;
    s16 ix;
    const s16* data_ptr;

    if (Debug_w[0x30] & 4) {
        return 0;
    }

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    data_ptr = efff6_data_tbl00[typenum];
    ewk->wu.id = 0x9C;
    ewk->wu.be_flag = 1;
    ewk->wu.work_id = 0x10;
    ewk->wu.type = typenum;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.char_table[0] = _op_char_table;
    ewk->wu.my_family = 2;
    ewk->wu.my_mts = 9;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.my_col_code = 0x12C;
    ewk->wu.my_col_code += *data_ptr++;
    ewk->wu.xyz[0].disp.pos = *data_ptr++;
    ewk->wu.xyz[1].disp.pos = *data_ptr++;
    ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
    ewk->wu.old_rno[0] = *data_ptr++;
    ewk->wu.char_index = *data_ptr++;
    ewk->wu.old_rno[2] = *data_ptr++;
    ewk->wu.old_rno[1] = *data_ptr++;
    ewk->wu.direction = efff6_etc_data[typenum].dir;
    ewk->wu.old_rno[3] = efff6_etc_data[typenum].limit_x_pos;
    ewk->wu.old_rno[4] = efff6_etc_data[typenum].limit_y_pos;
    ewk->wu.my_mr.size.x = efff6_etc_data[typenum].zoom_v;
    ewk->wu.my_mr.size.y = efff6_etc_data[typenum].zoom_v;
    ewk->wu.mvxy.a[0].sp = efff6_etc_data[typenum].sp_x_a;
    ewk->wu.mvxy.d[0].sp = efff6_etc_data[typenum].sp_x_d;
    ewk->wu.mvxy.a[1].sp = efff6_etc_data[typenum].sp_y_a;
    ewk->wu.mvxy.d[1].sp = efff6_etc_data[typenum].sp_y_d;
    effect_F6_move(ewk);
    return 0;
}

const s16 efff6_move01_tbl[6] = { 20, 28, 20, 28, 20, 28 }; // size: 0xC, address: 0x515180

const s16 efff6_data_tbl00[61][8] = {
    { 10, 512, 16, 70, 3, 1, 0, 1 },      { 24, 512, -224, 80, 3, 1, 1, 2 },    { 12, 448, -32, 70, 3, 2, 2, 3 },
    { 14, 440, -10, 80, 3, 2, 3, 4 },     { 28, 616, -91, 80, 3, 2, 4, 5 },     { 38, 488, -169, 80, 3, 3, 5, 6 },
    { 27, 666, -8, 78, 3, 3, 6, 7 },      { 19, 487, -63, 80, 3, 3, 7, 8 },     { 13, 487, -15, 80, 3, 3, 8, 9 },
    { 11, 571, -48, 80, 3, 3, 9, 10 },    { 8, 452, -32, 78, 3, 3, 10, 11 },    { 17, 429, 4, 80, 3, 4, 11, 12 },
    { 14, 502, -249, 80, 3, 4, 12, 13 },  { 15, 561, -49, 78, 3, 4, 13, 14 },   { 30, 456, -239, 78, 3, 4, 14, 15 },
    { 25, 556, 0, 78, 3, 4, 15, 16 },     { 14, 556, -234, 80, 3, 4, 16, 17 },  { 39, 451, -46, 80, 3, 5, 17, 18 },
    { 24, 569, -163, 80, 3, 5, 18, 19 },  { 11, 512, 0, 80, 3, 5, 19, 20 },     { 13, 567, 0, 80, 3, 6, 20, 21 },
    { 36, 580, -24, 80, 3, 6, 21, 22 },   { 29, 452, 71, 80, 3, 6, 22, 23 },    { 39, 477, -32, 80, 3, 6, 23, 24 },
    { 15, 568, -52, 80, 3, 7, 24, 25 },   { 38, 522, 0, 80, 3, 7, 25, 26 },     { 38, 512, 0, 80, 3, 8, 26, 27 },
    { 20, 452, -122, 80, 3, 8, 27, 28 },  { 12, 476, -53, 80, 3, 7, 28, 29 },   { 11, 498, -48, 80, 3, 9, 29, 30 },
    { 25, 400, 37, 80, 3, 9, 30, 31 },    { 36, 402, -127, 80, 3, 9, 31, 32 },  { 39, 535, 8, 80, 3, 9, 32, 33 },
    { 30, 466, -256, 80, 3, 9, 33, 34 },  { 26, 617, -139, 80, 3, 9, 34, 35 },  { 27, 481, 60, 80, 3, 11, 35, 36 },
    { 28, 480, 2, 80, 3, 11, 36, 37 },    { 14, 365, -165, 80, 3, 11, 37, 38 }, { 10, 498, -50, 80, 3, 11, 38, 39 },
    { 11, 361, 36, 80, 3, 11, 39, 40 },   { 38, 539, -6, 80, 3, 11, 40, 41 },   { 35, 455, -54, 80, 3, 15, 65, 66 },
    { 13, 512, 0, 80, 3, 15, 66, 67 },    { 27, 512, -52, 80, 3, 16, 67, 68 },  { 15, 453, -49, 80, 3, 17, 71, 72 },
    { 27, 454, -237, 80, 3, 17, 72, 73 }, { 28, 486, -121, 80, 3, 17, 73, 74 }, { 39, 512, -67, 80, 3, 18, 74, 75 },
    { 29, 441, 0, 80, 3, 18, 75, 76 },    { 27, 355, 0, 80, 3, 18, 76, 77 },    { 38, 438, -237, 80, 3, 17, 77, 78 },
    { 38, 476, -241, 80, 3, 18, 78, 79 }, { 14, 448, -48, 80, 3, 19, 80, 81 },  { 25, 616, -102, 80, 3, 20, 82, 83 },
    { 0, 500, 32, 80, 3, 10, 70, 71 },    { 0, 392, 178, 67, 3, 27, 91, 93 },   { 0, 392, 50, 67, 3, 27, 91, 93 },
    { 0, 520, 178, 67, 3, 27, 91, 93 },   { 0, 520, 50, 67, 3, 27, 91, 93 },    { 0, 648, 178, 67, 3, 27, 91, 93 },
    { 0, 648, 50, 67, 3, 27, 91, 93 }
}; // size: 0x3D0, address: 0x515190

const EFFF6_ETC efff6_etc_data[61] = {
    { .dir = 512,
      .limit_x_pos = 0,
      .limit_y_pos = -32,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = -32768,
      .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = -136,
      .zoom_v = 127,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 131072,
      .sp_y_d = 0 },
    { .dir = 16,
      .limit_x_pos = 538,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 98304,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = 32,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 131072,
      .sp_y_d = 0 },
    { .dir = 32,
      .limit_x_pos = 456,
      .limit_y_pos = 0,
      .zoom_v = 127,
      .sp_x_a = -262144,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = 0,
      .zoom_v = 127,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 393216,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 79, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 16,
      .limit_x_pos = 544,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 393216,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 32,
      .limit_x_pos = 456,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = -262144,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 79, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = -116,
      .zoom_v = 127,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 262144,
      .sp_y_d = 0 },
    { .dir = 16,
      .limit_x_pos = 569,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 180224,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 63, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 32,
      .limit_x_pos = 454,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = -262144,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 512,
      .limit_x_pos = 0,
      .limit_y_pos = -179,
      .zoom_v = 127,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = -196608,
      .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 24576,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 63, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 63, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 32,
      .limit_x_pos = 384,
      .limit_y_pos = 0,
      .zoom_v = 127,
      .sp_x_a = -131072,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 16,
      .limit_x_pos = 573,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 65536,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = 25,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 262144,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 108, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 63, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 32,
      .limit_x_pos = 462,
      .limit_y_pos = 0,
      .zoom_v = 127,
      .sp_x_a = -393216,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 89, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 63, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 16,
      .limit_x_pos = 509,
      .limit_y_pos = 0,
      .zoom_v = 127,
      .sp_x_a = 524288,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 79, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = 30,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 65536,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 83, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 63, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 131072,
      .sp_y_d = 0 },
    { .dir = 256,
      .limit_x_pos = 0,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = 524288,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 79, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 16,
      .limit_x_pos = 553,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 393216,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 63, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 16,
      .limit_x_pos = 560,
      .limit_y_pos = 0,
      .zoom_v = 79,
      .sp_x_a = 262144,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 32,
      .limit_x_pos = 407,
      .limit_y_pos = 0,
      .zoom_v = 95,
      .sp_x_a = -229376,
      .sp_x_d = 0,
      .sp_y_a = 0,
      .sp_y_d = 0 },
    { .dir = 512,
      .limit_x_pos = 0,
      .limit_y_pos = -50,
      .zoom_v = 79,
      .sp_x_a = 0,
      .sp_x_d = 0,
      .sp_y_a = -98304,
      .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 },
    { .dir = 0, .limit_x_pos = 0, .limit_y_pos = 0, .zoom_v = 127, .sp_x_a = 0, .sp_x_d = 0, .sp_y_a = 0, .sp_y_d = 0 }
};
