/**
 * @file effd0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effd0.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void d0_speed_set(WORK* ewk, s16 num);

const s32 effd0_data_tbl[9][4] = { { 0x4000, -0x800, -0x6000, -0x400 }, { -0x4000, 0x600, -0x4000, -0x600 },
                                   { 0x4000, -0xC00, -0x3000, -0x400 }, { 0x800, -0x300, -0x6000, -0x600 },
                                   { 0x0, -0x100, -0x5000, -0x500 },    { 0x6000, -0xA00, -0x3000, -0x600 },
                                   { 0x6000, 0xA00, -0x2000, -0x500 },  { -0x6000, 0xA00, -0x1000, -0x500 },
                                   { -0x4000, 0x600, -0x4000, -0x600 } };

const s16 effd0_conter[9] = { 32, 40, 30, 48, 64, 16, 32, 36, 72 };

void effect_D0_move(WORK_Other* ewk) {
    if (Exec_Wipe) {
        ewk->wu.no_death_attack = 1;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_rno[1] = 0;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (ewk->wu.no_death_attack && !Exec_Wipe) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[0]++;
            set_char_move_init(&ewk->wu, 0, 15);
            d0_speed_set(&ewk->wu, ewk->wu.old_rno[1]);
            ewk->wu.xyz[1].disp.pos += 96;

            if (ewk->wu.rl_flag) {
                ewk->wu.xyz[0].disp.pos += 22;
            } else {
                ewk->wu.xyz[0].disp.pos -= 22;
            }
        }

        pl_eff_trans_entry(ewk);
        break;

    case 2:
        if (ewk->wu.no_death_attack && !Exec_Wipe) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            d0_speed_set(&ewk->wu, ewk->wu.old_rno[1]);
        } else {
            char_move(&ewk->wu);
            add_x_sub(ewk);
            add_y_sub(ewk);
        }

        if (ewk->wu.xyz[1].disp.pos < 0) {
            ewk->wu.routine_no[0]++;
            set_char_move_init(&ewk->wu, 0, 16);
        }

        pl_eff_trans_entry(ewk);
        break;

    case 3:
        if (ewk->wu.no_death_attack && !Exec_Wipe) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[0]++;
        }

        pl_eff_trans_entry(ewk);
        break;

    case 4:
        if (ewk->wu.no_death_attack && !Exec_Wipe) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        pl_eff_trans_entry(ewk);
        break;

    case 99:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void d0_speed_set(WORK* ewk, s16 num) {
    ewk->old_rno[0] = effd0_conter[num];
    ewk->old_rno[1]++;

    if (ewk->rl_flag) {
        ewk->mvxy.a[0].sp = -effd0_data_tbl[num][0];
        ewk->mvxy.d[0].sp = -effd0_data_tbl[num][1];
    } else {
        ewk->mvxy.a[0].sp = effd0_data_tbl[num][0];
        ewk->mvxy.d[0].sp = effd0_data_tbl[num][1];
    }

    ewk->mvxy.a[1].sp = effd0_data_tbl[num][2];
    ewk->mvxy.d[1].sp = effd0_data_tbl[num][3];
}

s32 effect_D0_init(PLW* oya, s32 /* unused */) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 130;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.disp_flag = 0;
    ewk->my_master = (u32*)oya;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = 14;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_priority = ewk->wu.position_z = oya->wu.my_priority + 1;
    ewk->wu.sync_suzi = 0;
    ewk->master_id = oya->wu.id;
    ewk->wu.xyz[0].cal = oya->wu.xyz[0].cal;
    ewk->wu.xyz[1].cal = oya->wu.xyz[1].cal;
    ewk->wu.rl_flag = oya->wu.rl_flag;
    *ewk->wu.char_table = _etc_char_table;

    if (oya->wu.id) {
        ewk->wu.my_col_code = 22;
    } else {
        ewk->wu.my_col_code = 6;
    }

    ewk->wu.no_death_attack = 0;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
