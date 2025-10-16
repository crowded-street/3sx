/**
 * @file eff24.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff24.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 eff24_data_tbl[56] = { 8492, 480, 6,  10, 12, 2, 0, 300,  224, 43, 81, 3, 1, 1, 300, 288, 44, 80, 4, 2, 1,
                                 300,  512, 48, 80, 5,  1, 0, 8492, 512, 48, 80, 6, 2, 0, 300, 644, 48, 80, 7, 1, 1,
                                 300,  752, 48, 81, 8,  3, 1, 300,  800, 32, 80, 9, 3, 1 };

const s16 eff24_quake_index_tbl[111] = { 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 7, 7, 7,
                                         7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                         7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                         7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
                                         7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 };

const s32 eff24_quake_speed_y_tbl[4][8] = { { 0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000, 0x70000, 0xC0000 },
                                            { 0xE000, 0x10000, 0x1C000, 0x24000, 0x30000, 0x48000, 0x60000, 0x80000 },
                                            { 0xC000, 0xE000, 0x10000, 0x18000, 0x20000, 0x30000, 0x48000, 0x60000 },
                                            { 0x2000, 0x8000, 0xC000, 0x10000, 0x18000, 0x20000, 0x30000, 0x60000 } };

const s32 eff24_quake_speed_x_tbl[4][8] = { { 0x1200, 0x1200, 0x1400, 0x1400, 0x1000, 0x1200, 0x1200, 0x1400 },
                                            { 0x1800, 0x1800, 0x1400, 0x1400, 0x1000, 0x1800, 0x1400, 0x1400 },
                                            { 0x1000, 0x1000, 0x800, 0x800, 0x800, 0x800, 0x400, 0x400 },
                                            { 0x400, 0x400, 0x800, 0x800, 0x400, 0x400, 0x400, 0x400 } };

const s16 dog24_x_data[8] = { 0, 0, 0, 6, 10, 16, 32, 40 };

void effect_24_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_rno[0] = 0;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            eff24_quake_sub(ewk);
        }

        if (ewk->wu.old_rno[7]) {
            disp_pos_trans_entry_r(ewk);
            break;
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
    }
}

void eff24_quake_sub(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (bg_w.quake_y_index > 0) {
            ewk->wu.routine_no[1]++;
            eff24_sp_data_set(ewk);
        }

        break;

    case 1:
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos < ewk->wu.old_rno[2]) {
            ewk->wu.routine_no[1]++;
            ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[2];
            ewk->wu.xyz[1].disp.low = 0;
            eff24_sp_data_set(ewk);
        }

        break;

    case 2:
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        ewk->wu.old_rno[5]--;

        if (ewk->wu.old_rno[5] > 0) {
            break;
        }

        ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[4];
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[2];
        ewk->wu.xyz[1].disp.low = 0;

        if (ewk->wu.type == 0 && ewk->wu.old_rno[1] > 2) {
            ewk->wu.routine_no[1]++;
            dog24_data_set(ewk);

            if (ewk->wu.old_rno[6]) {
                set_char_move_init(&ewk->wu, 0, 14);
            } else {
                set_char_move_init(&ewk->wu, 0, 13);
            }

            ewk->wu.old_rno[6] ^= 1;
            break;
        }

        ewk->wu.routine_no[1] = 0;
        ewk->wu.old_rno[1] = 0;
        ewk->wu.old_rno[0] = 0;
        break;

    case 3:
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        ewk->wu.old_rno[5]--;

        if (ewk->wu.old_rno[5] > 0) {
            break;
        }

        ewk->wu.routine_no[1]++;
        // fallthrough

    case 4:
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1] = 0;
            ewk->wu.old_rno[1] = 0;
            ewk->wu.old_rno[0] = 0;
            ewk->wu.old_rno[4] = ewk->wu.xyz[0].disp.pos;
            ewk->wu.xyz[0].disp.low = 0;
            ewk->wu.old_rno[2] = ewk->wu.xyz[1].disp.pos;
            ewk->wu.xyz[1].disp.low = 0;
        }

        break;
    }
}

void dog24_data_set(WORK_Other* ewk) {
    s16 work;

    ewk->wu.old_rno[5] = 40;

    if (ewk->wu.old_rno[6]) {
        work = ewk->wu.xyz[0].disp.pos + dog24_x_data[ewk->wu.old_rno[1]];
    } else {
        work = ewk->wu.xyz[0].disp.pos - dog24_x_data[ewk->wu.old_rno[1]];
    }

    cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[5], work, ewk->wu.xyz[1].disp.pos, 2, 0);
}

void eff24_sp_data_set(WORK_Other* ewk) {
    s16 work;

    if (ewk->wu.old_rno[0]) {
        ewk->wu.old_rno[5] = 12;
        ewk->wu.mvxy.d[0].sp = 0;

        switch (ewk->wu.old_rno[1]) {
        case 0:
        case 1:
            ewk->wu.mvxy.d[1].sp = -0x2000;
            break;

        case 2:
        case 3:
            ewk->wu.mvxy.d[1].sp = -0x4000;
            break;

        default:
            ewk->wu.mvxy.d[1].sp = -0x5000;
            break;
        }

        cal_initial_speed(&ewk->wu, ewk->wu.old_rno[5], ewk->wu.old_rno[4], ewk->wu.old_rno[2]);
    } else {
        ewk->wu.old_rno[1] = eff24_quake_index_tbl[bg_w.quake_y_index];
        ewk->wu.mvxy.a[0].sp = eff24_quake_speed_x_tbl[ewk->wu.old_rno[3]][ewk->wu.old_rno[1]];
        ewk->wu.mvxy.d[0].sp = 0;
        work = random_16();

        if (work & 1) {
            ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
        }

        ewk->wu.mvxy.a[1].sp = eff24_quake_speed_y_tbl[ewk->wu.old_rno[3]][ewk->wu.old_rno[1]];
        ewk->wu.mvxy.d[1].sp = -0x6000;
    }

    ewk->wu.old_rno[0]++;
    ewk->wu.old_rno[0] &= 1;
}

s32 effect_24_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    const s16* data_ptr = eff24_data_tbl;

    for (i = 0; i < 8; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 24;
        ewk->wu.type = i;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;

        if (i) {
            ewk->wu.my_family = 2;
        } else {
            ewk->wu.my_family = 3;
        }

        ewk->wu.my_col_mode = 0x4200;
        *ewk->wu.char_table = _grm_char_table;
        ewk->wu.sync_suzi = 0;
        ewk->wu.my_col_code = *data_ptr++;
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].disp.pos = *data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
        ewk->wu.char_index = *data_ptr++;
        ewk->wu.old_rno[3] = *data_ptr++;
        ewk->wu.old_rno[7] = *data_ptr++;
        ewk->wu.old_rno[6] = 0;
        ewk->wu.old_rno[0] = 0;
        ewk->wu.old_rno[1] = 0;
        ewk->wu.old_rno[4] = ewk->wu.xyz[0].disp.pos;
        ewk->wu.old_rno[2] = ewk->wu.xyz[1].disp.pos;
        suzi_offset_set(ewk);
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
