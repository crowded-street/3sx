/**
 * @file eff21.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff21.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 eff21_num[1] = { 3 };

const s32 eff21_sp_tbl[1][2] = { { 0x2000, 0x2000 } };

const s16 eff21_data_tbl_00[27] = { 1,  2,  8492, 640, 0, 10, 17,   0,   0, 0,  2,  8492, 192, 0,
                                    10, 18, 0,    0,   1, 2,  8492, 736, 0, 10, 19, 0,    0 };

const s16* eff21_data_adrs[1] = { eff21_data_tbl_00 };

void effect_21_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[0];
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.xyz[0].cal -= (ewk->wu.mvxy.a[0].sp * bg_w.bg2_sp_x2);
        ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[2];
        ewk->wu.xyz[1].disp.low = 0;
        ewk->wu.xyz[1].cal += (ewk->wu.mvxy.a[1].sp * bg_w.bg2_sp_y);

        if (ewk->wu.old_rno[3]) {
            disp_pos_trans_entry_rs(ewk);
            break;
        }

        disp_pos_trans_entry_s(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_21_init(s16 sync_index) {
    WORK_Other* ewk;
    s16 ix;
    s16 lp_cnt = eff21_num[sync_index];
    s16 i;
    const s16* data_ptr;

    if (lp_cnt == 0) {
        return 0;
    }

    data_ptr = eff21_data_adrs[sync_index];

    for (i = 0; i < lp_cnt; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 21;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.type = i;
        ewk->wu.old_rno[3] = *data_ptr++;
        ewk->wu.my_family = *data_ptr++;
        ewk->wu.my_col_code = *data_ptr++;
        ewk->wu.old_rno[0] = *data_ptr++;
        ewk->wu.old_rno[2] = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
        ewk->wu.char_index = *data_ptr++;
        ewk->wu.sync_suzi = *data_ptr++;
        ewk->wu.mvxy.a[0].sp = eff21_sp_tbl[*data_ptr][0];
        ewk->wu.mvxy.a[1].sp = eff21_sp_tbl[*data_ptr][1];
        data_ptr++;
        ewk->wu.char_table[0] = char_add[bg_w.bg_index];
        ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[0];
        ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[2];
        suzi_offset_set(ewk);
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
