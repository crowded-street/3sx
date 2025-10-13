/**
 * @file effm8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effm8.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void effm8_move_app(WORK_Other* ewk);
void don_run_sub_m8(WORK_Other* ewk);
void effm8_move_win(WORK_Other* ewk);

const s8 effm8_random_tbl[16] = { 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1 };

const s16 effm8_timer_tbl[4] = { 24, 56, 72, 112 };

void effect_M8_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (!EXE_flag && !Game_pause) {
            if (ewk->wu.type) {
                effm8_move_win(ewk);
            } else {
                effm8_move_app(ewk);
            }
        }

        pl_eff_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void effm8_move_app(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.old_rno[0] = 60;
        break;

    case 1:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, 0x34);
        }

        break;

    case 3:
        don_run_sub_m8(ewk);
        break;
    }
}

void don_run_sub_m8(WORK_Other* ewk) {
    char_move(&ewk->wu);
    add_x_sub(ewk);

    if (!range_x_check3(ewk, 56)) {
        ewk->wu.routine_no[1]++;
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 0;
    }
}

void effm8_move_win(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        }

        break;

    case 1:
        don_run_sub_m8(ewk);
        break;
    }
}

s32 effect_M8_init(WORK* oya, u8 data) {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    s16 work;

    if (data) {
        work = random_16();
        work = effm8_random_tbl[work];

        if (!work) {
            return 0;
        }

        if ((ix = pull_effect_work(3)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[(ix)];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 228;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.disp_flag = 0;
        ewk->my_master = (u32*)oya;
        ewk->master_id = oya->id;
        ewk->wu.type = 1;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = oya->my_col_code;
        ewk->wu.rl_flag = oya->rl_flag;

        if (oya->rl_flag) {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].position_x - 48;
            ewk->wu.mvxy.a[0].sp = 0x48000;
            ewk->wu.mvxy.d[0].sp = 0;
        } else {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + bg_w.pos_offset + 48;
            ewk->wu.mvxy.a[0].sp = -0x48000;
            ewk->wu.mvxy.d[0].sp = 0;
        }

        ewk->wu.xyz[1].cal = oya->xyz[1].cal;
        ewk->wu.xyz[1].disp.pos -= 5;
        ewk->wu.position_z = oya->my_priority;
        ewk->wu.position_z -= 2;
        ewk->wu.my_priority = ewk->wu.position_z;
        *ewk->wu.char_table = _etc2_char_table;
        ewk->wu.char_index = 52;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = -2;
        ewk->wu.kage_hy = 0;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 8;
        ewk->wu.old_rno[0] = 0;
        ewk->wu.my_mts = 14;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

        for (i = 0; i < 4; i++) {
            if ((ix = pull_effect_work(3)) == -1) {
                return -1;
            }

            ewk = (WORK_Other*)frw[(ix)];
            ewk->wu.be_flag = 1;
            ewk->wu.id = 228;
            ewk->wu.work_id = 16;
            ewk->wu.cgromtype = 1;
            ewk->wu.disp_flag = 0;
            ewk->my_master = (u32*)oya;
            ewk->master_id = oya->id;
            ewk->wu.type = 1;
            ewk->wu.my_family = 2;
            ewk->wu.my_col_mode = 0x4200;
            ewk->wu.my_col_code = oya->my_col_code;
            ewk->wu.rl_flag = oya->rl_flag;

            if (oya->rl_flag) {
                ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].position_x - 48;
                ewk->wu.mvxy.a[0].sp = 0x48000;
                ewk->wu.mvxy.d[0].sp = 0;
            } else {
                ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + bg_w.pos_offset + 48;
                ewk->wu.mvxy.a[0].sp = -0x48000;
                ewk->wu.mvxy.d[0].sp = 0;
            }

            ewk->wu.xyz[1].cal = oya->xyz[1].cal;
            ewk->wu.xyz[1].disp.pos -= 5;
            ewk->wu.position_z = oya->my_priority;
            ewk->wu.position_z -= 2;
            ewk->wu.my_priority = ewk->wu.position_z;
            *ewk->wu.char_table = _etc2_char_table;
            ewk->wu.char_index = 55;
            ewk->wu.kage_flag = 1;
            ewk->wu.kage_hx = -2;
            ewk->wu.kage_hy = 0;
            ewk->wu.kage_prio = 71;
            ewk->wu.kage_char = 8;
            ewk->wu.old_rno[0] = effm8_timer_tbl[i];
            ewk->wu.my_mts = 14;
            ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
        }
    } else {
        if ((ix = pull_effect_work(3)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[(ix)];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 228;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.disp_flag = 0;
        ewk->my_master = (u32*)oya;
        ewk->master_id = oya->id;
        ewk->wu.type = 0;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = oya->my_col_code;

        if (oya->id) {
            ewk->wu.xyz[0].disp.pos = 648;
            ewk->wu.mvxy.a[0].sp = 0x48000;
            ewk->wu.mvxy.d[0].sp = 0;
        } else {
            ewk->wu.xyz[0].disp.pos = 376;
            ewk->wu.mvxy.a[0].sp = -0x48000;
            ewk->wu.mvxy.d[0].sp = 0;
        }

        ewk->wu.xyz[1].cal = oya->xyz[1].cal;
        ewk->wu.position_z = oya->my_priority;
        ewk->wu.position_z++;
        ewk->wu.my_priority = ewk->wu.position_z;
        ewk->wu.rl_flag = oya->rl_flag ^ 1;
        *ewk->wu.char_table = _etc2_char_table;
        ewk->wu.char_index = 54;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = -2;
        ewk->wu.kage_hy = 4;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 8;
        ewk->wu.my_mts = 14;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
