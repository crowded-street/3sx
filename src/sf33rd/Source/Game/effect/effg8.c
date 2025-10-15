/**
 * @file effg8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effg8.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

const s32 effg8_sp_tbl[28][4];

void effect_G8_move(WORK_Other* ewk) {
    if (!akebono_flag) {
        ewk->wu.routine_no[0] = 99;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 0;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = 0;
        ewk->wu.my_mr.size.y = 0;
        set_char_move_init(&ewk->wu, 0, 17);
        ewk->wu.old_rno[3] = random_16();
        ewk->wu.old_rno[0] = 192;
        ewk->wu.old_rno[1] = 128;
        ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[0];
        ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[1];
        ewk->wu.mvxy.a[0].sp = effg8_sp_tbl[ewk->wu.old_rno[2]][0];
        ewk->wu.mvxy.d[0].sp = effg8_sp_tbl[ewk->wu.old_rno[2]][1];
        ewk->wu.mvxy.a[1].sp = effg8_sp_tbl[ewk->wu.old_rno[2]][2];
        ewk->wu.mvxy.d[1].sp = effg8_sp_tbl[ewk->wu.old_rno[2]][3];
        break;

    case 1:
        ewk->wu.old_rno[3] -= 1;

        if (ewk->wu.old_rno[3] > 0) {
            break;
        }

        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = 1;
        /* fallthrough */

    case 2:
        ewk->wu.my_mr.size.x += 2;
        ewk->wu.my_mr.size.y += 2;

        if (ewk->wu.my_mr.size.x >= 63) {
            ewk->wu.routine_no[0] += 1;
        }

        /* fallthrough */

    case 3:
        add_x_sub(ewk);
        add_y_sub(ewk);

        if (effg8_range_check(ewk)) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        ewk->wu.position_x = (ewk->wu.xyz[0].disp.pos & 0xFFFF);
        ewk->wu.position_y = (ewk->wu.xyz[1].disp.pos & 0xFFFF);
        sort_push_request4(&ewk->wu);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s16 effg8_range_check(WORK_Other* ewk) {
    if (ewk->wu.xyz[0].disp.pos < 88) {
        return 1;
    }

    if (ewk->wu.xyz[0].disp.pos > 296) {
        return 1;
    }

    if (ewk->wu.xyz[1].disp.pos < 64) {
        return 1;
    }

    if (ewk->wu.xyz[1].disp.pos > 208) {
        return 1;
    }

    return 0;
}

s32 effect_G8_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < 16; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.id = 168;
        ewk->wu.be_flag = 1;
        ewk->wu.type = i;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 30;
        ewk->wu.char_table[0] = _etc2_char_table;
        ewk->wu.my_family = 4;
        ewk->wu.my_priority = ewk->wu.position_z = 10;
        ewk->wu.old_rno[2] = i;
        ewk->wu.my_mts = 14;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}

const s32 effg8_sp_tbl[28][4] = { { 0, 0, 8192, 16384 },
                                  { -65536, -32768, 4096, 32768 },
                                  { -32768, 32768, 0, 16384 },
                                  { -131072, -32768, 0, 0 },
                                  { -65536, -32768, 0, -8192 },
                                  { 0, 0, -8192, -16384 },
                                  { -32768, -32768, -4096, -32768 },
                                  { -65536, -32768, 0, -16384 },
                                  { 0, 0, 8192, 16384 },
                                  { 65536, 32768, 4096, 32768 },
                                  { 32768, 32768, 0, 16384 },
                                  { 131072, 32768, 0, 0 },
                                  { 65536, 32768, 0, -8192 },
                                  { 0, 0, -8192, -16384 },
                                  { 65536, 32768, -4096, -32768 },
                                  { 32768, 32768, 0, -16384 },
                                  { 16384, 131072, 8192, 0 },
                                  { -16384, -131072, -8192, 0 },
                                  { 16384, 131072, -8192, -32768 },
                                  { -12288, -131072, 8192, 32768 },
                                  { -4096, -65536, -8192, -32768 },
                                  { 0, 0, 4096, 65536 },
                                  { 0, 32768, -8192, -32768 },
                                  { 16384, 131072, 0, 8192 },
                                  { 0, 0, 0, 16384 },
                                  { 0, -16384, -8192, -32768 },
                                  { 0, -131072, -8192, -32768 },
                                  { 4096, 131072, -4096, -16384 } };
