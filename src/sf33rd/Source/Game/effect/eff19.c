/**
 * @file eff19.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff19.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 eff19_data_tbl[14] = { 384, 192, 256, 196, 416, 240, 328, 288, 376, 256, 452, 252, 206, 210 };

const s8 eff19_wait_tbl[16] = { 6, 30, 3, 50, 20, 18, 14, 28, 0, 8, 18, 4, 30, 23, 38, 4 };

const s8 effect_19_s_tbl[16] = { 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 };

const s8 effect_19_m_tbl[16] = { 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1 };

const s8 effect_19_l_tbl[16] = { 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1 };

void effect_19_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 6);
        break;

    case 1:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            eff19_quake_sub(ewk);
        }

        disp_pos_trans_entry_r(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff19_quake_sub(WORK_Other* ewk) {
    s16 work;
    s8 fall_go;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (bg_w.quake_y_index <= 2) {
            break;
        }

        work = random_16();

        if (bg_w.quake_y_index < 8) {
            fall_go = effect_19_s_tbl[work];
        } else if (bg_w.quake_y_index > 14) {
            fall_go = effect_19_l_tbl[work];
        } else {
            fall_go = effect_19_m_tbl[work];
        }

        if (fall_go) {
            ewk->wu.routine_no[1]++;
            ewk->wu.routine_no[2] = 0;
            ewk->wu.mvxy.a[1].sp = 0;
            ewk->wu.mvxy.d[1].sp = -0x6000;
            ewk->wu.old_rno[0] = eff19_wait_tbl[work];
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.routine_no[2] = 1;
        ewk->wu.old_rno[0] = 60;
        break;

    case 1:
        if (ewk->wu.routine_no[2]) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] >= 0) {
                break;
            }

            ewk->wu.routine_no[1] = 0;
            set_char_move_init(&ewk->wu, 0, 6);
            break;
        }

        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        add_y_sub(ewk);

        if (ewk->wu.xyz[1].disp.pos < 66) {
            ewk->wu.routine_no[1]++;
            break;
        }

        break;

    case 3:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
            break;
        }

        break;

    case 4:
        if (!range_x_check(ewk)) {
            ewk->wu.routine_no[1] = 0;
            ewk->wu.disp_flag = 1;
            ewk->wu.xyz[1].disp.pos = eff19_data_tbl[(ewk->wu.type * 2) + 1];
            set_char_move_init(&ewk->wu, 0, 6);
        }

        break;
    }
}

s32 effect_19_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    const s16* data_ptr = &eff19_data_tbl[0];

    for (i = 0; i < 7; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 19;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.dead_f = 1;
        ewk->wu.type = i;
        ewk->wu.work_id = 16;
        ewk->wu.my_family = 2;
        ewk->wu.char_index = 6;
        ewk->wu.char_table[0] = _jp3_char_table;
        ewk->wu.sync_suzi = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 8492;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
        ewk->wu.my_priority = ewk->wu.position_z = 80;
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = *data_ptr++;
        suzi_offset_set(ewk);
    }

    return 0;
}
