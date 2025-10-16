/**
 * @file eff47.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff47.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s32 eff47_sp_tbl[4][4] = { { 0x40000, -0x1000, 0x30000, -0x4000 },
                                 { -0x20000, 0x800, 0x30000, -0x4000 },
                                 { 0x10000, 0x0, 0x20000, -0x4000 },
                                 { -0x50000, 0x1000, 0x40000, -0x4000 } };

const s16 eff47_data_tbl[12] = { 16, 144, 50, -68, 122, 50, 24, 48, 30, -37, 70, 30 };

void effect_47_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.kage_flag = 0;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 0) {
                ewk->wu.routine_no[0]++;
                ewk->wu.disp_flag = 0;
            } else {
                char_move(&ewk->wu);
                add_x_sub(&ewk->wu);
                add_y_sub(&ewk->wu);
            }
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_47_init(WORK* wk, s32 /* unused */) {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    const s16* data_ptr = eff47_data_tbl;

    for (i = 0; i < 4; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 47;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        *ewk->wu.char_table = _etc_char_table;
        ewk->wu.type = i;
        ewk->wu.my_family = 2;
        ewk->wu.char_index = i + 0x43;
        ewk->wu.my_col_mode = wk->my_col_mode;
        ewk->wu.my_col_code = wk->my_col_code;
        ewk->wu.my_family = wk->my_family;
        ewk->wu.position_z = ewk->wu.my_priority = wk->my_priority - 2;
        ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos;
        ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos;
        ewk->wu.xyz[0].disp.pos += *(s16*)data_ptr++;
        ewk->wu.xyz[1].disp.pos += *(s16*)data_ptr++;
        ewk->wu.old_rno[0] = *data_ptr++;
        ewk->wu.mvxy.a[0].sp = eff47_sp_tbl[i][0];
        ewk->wu.mvxy.d[0].sp = eff47_sp_tbl[i][1];
        ewk->wu.mvxy.a[1].sp = eff47_sp_tbl[i][2];
        ewk->wu.mvxy.d[1].sp = eff47_sp_tbl[i][3];
        ewk->wu.my_mts = 14;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
