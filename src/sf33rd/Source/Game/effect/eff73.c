/**
 * @file eff73.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff73.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/workuser.h"

const s32 eff73_sp_tbl[4][3] = {
    { 0x28000, -0x18000, -0x800 }, { 0x10000, 0x8000, 0x20000 }, { -0x4000, 0x0, 0x0 }, { -0x10000, 0x8000, 0x28000 }
};

const s16 eff73_vanish_tbl[8] = { 0x003C, 0x0018, 0x0026, 0x000E, 0x0016, 0x001C, 0x0012, 0x0028 };

const s16 eff73_survive_tbl[8] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0003, 0x0002, 0x0001, 0x0000 };

void effect_73_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 2);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            add_x_sub(ewk);
            add_y_sub(ewk);
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 0) {
                if (ewk->wu.old_rno[1]) {
                    ewk->wu.routine_no[0]++;
                } else {
                    ewk->wu.routine_no[0] = 99;
                    ewk->wu.disp_flag = 0;
                }
            }
        }

        disp_pos_trans_entry_r(ewk);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            add_x_sub(ewk);
            add_y_sub(ewk);

            if (ewk->wu.xyz[1].disp.pos < 64) {
                ewk->wu.routine_no[0]++;
                ewk->wu.mvxy.a[0].sp = eff73_sp_tbl[ewk->wu.type][0];
                ewk->wu.mvxy.d[0].sp = 0;
                ewk->wu.mvxy.a[1].sp = 0x30000;
                ewk->wu.mvxy.d[1].sp = -0x4000;
            }
        }

        disp_pos_trans_entry_r(ewk);
        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            add_x_sub(ewk);
            add_y_sub(ewk);

            if (ewk->wu.xyz[1].disp.pos < 56) {
                ewk->wu.routine_no[0] = 99;
                ewk->wu.disp_flag = 0;
            }
        }

        disp_pos_trans_entry_r(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_73_init(WORK_Other* oya) {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    s16 work;
    s16 work2;

    if (EXE_obroll) {
        return 0;
    }

    work = random_16();
    work &= 7;

    for (i = 0; i < 4; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 73;
        ewk->wu.work_id = 16;
        ewk->my_master = (u32*)oya;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.type = i;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.dead_f = 1;
        ewk->wu.my_family = 2;
        ewk->wu.sync_suzi = 0;
        *ewk->wu.char_table = _hkg_char_table;
        ewk->wu.xyz[0].disp.pos = oya->wu.xyz[0].disp.pos + 0;
        ewk->wu.xyz[1].disp.pos = oya->wu.xyz[1].disp.pos + 64;
        ewk->wu.position_z = ewk->wu.my_priority = 80;
        ewk->wu.mvxy.a[0].sp = eff73_sp_tbl[i][0];
        ewk->wu.mvxy.a[1].sp = eff73_sp_tbl[i][1];
        ewk->wu.mvxy.a[1].sp = eff73_sp_tbl[i][2];
        ewk->wu.mvxy.d[1].sp = -0x4000;

        if (i == eff73_survive_tbl[work]) {
            ewk->wu.old_rno[1] = 1;
        } else {
            ewk->wu.old_rno[1] = 0;
        }

        work2 = random_16();
        work2 &= 7;

        ewk->wu.old_rno[0] = eff73_vanish_tbl[work2];
        suzi_offset_set(ewk);
    }

    return 0;
}
