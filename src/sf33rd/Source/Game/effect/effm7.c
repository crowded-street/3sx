/**
 * @file effm7.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effm7.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void effm7_move(WORK_Other* ewk);

void effect_M7_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (!EXE_flag && !Game_pause) {
            effm7_move(ewk);
        }

        pl_eff_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void effm7_move(WORK_Other* ewk) {
    s16 id_w;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        id_w = ewk->master_id ^ 1;
        ewk->wu.rl_flag ^= plw[id_w].wu.rl_flag;

        if (plw[id_w].wu.rl_flag) {
            ewk->wu.xyz[0].disp.pos = plw[id_w].wu.xyz[0].disp.pos - ewk->wu.xyz[0].disp.pos;
        } else {
            ewk->wu.xyz[0].disp.pos = plw[id_w].wu.xyz[0].disp.pos + ewk->wu.xyz[0].disp.pos;
        }

        set_char_move_init(&ewk->wu, 0, 0);
        break;

    case 1:
        ewk->wu.old_rno[1]--;
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        char_move(&ewk->wu);

        ewk->wu.old_rno[1]--;

        if (ewk->wu.old_rno[1] < 0) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 1, 0x6C);
        }

        break;

    case 3:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 1) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.mvxy.a[0].sp = 0;
        ewk->wu.mvxy.d[0].sp = 0;
        ewk->wu.mvxy.a[1].sp = 0x78000;
        ewk->wu.mvxy.d[1].sp = -0x6000;
        break;

    case 4:
        add_y_sub(ewk);
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 2) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.mvxy.d[0].sp = 0;

        if (ewk->wu.rl_flag) {
            ewk->wu.mvxy.a[0].sp = 0x80000;
        } else {
            ewk->wu.mvxy.a[0].sp = -0x80000;
        }

        ewk->wu.mvxy.a[1].sp = -0x8000;
        ewk->wu.mvxy.d[1].sp = 0x4000;
        break;

    case 5:
        add_x_sub(ewk);
        add_y_sub(ewk);

        if (!range_x_check3(ewk, 208)) {
            ewk->wu.routine_no[0] = 99;
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[1]++;
        }

        break;
    }
}

const s16 effm7_data_tbl[42] = {
    48,  -8, -1, 0, 18, 30, 90, 64,   2,  1,  0, 17, 16, 98,  -32,  -8, -1, 1, 18, 4,  92,
    -64, 2,  1,  1, 17, 24, 96, -112, -8, -1, 1, 18, 8,  102, -224, 2,  1,  1, 17, 10, 106
};

s32 effect_M7_init(PLW* oya) {
    WORK_Other* ewk;
    s16 ix;
    s16 i;
    const s16* data_ptr = effm7_data_tbl;
    s16 em_id = oya->wu.id ^ 1;

    for (i = 0; i < 6; i++) {
        if ((ix = pull_effect_work(3)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 227;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.disp_flag = 0;
        ewk->my_master = (u32*)oya;
        ewk->master_id = oya->wu.id;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = oya->wu.id ? 8 : 0;
        ewk->wu.xyz[0].disp.pos = *data_ptr++;
        ewk->wu.xyz[1].cal = plw[em_id].wu.xyz[1].cal;
        ewk->wu.xyz[1].disp.pos += *(s16*)data_ptr++;
        ewk->wu.position_z = plw[em_id].wu.my_priority;
        ewk->wu.position_z += *(s16*)data_ptr++;
        ewk->wu.my_priority = ewk->wu.position_z;
        ewk->wu.rl_flag = *data_ptr++;
        ewk->wu.kage_char = *data_ptr++;
        ewk->wu.old_rno[0] = *data_ptr++;
        ewk->wu.old_rno[1] = *data_ptr++;
        ewk->wu.char_table[0] = *oya->wu.char_table;
        ewk->wu.char_table[1] = _etc_char_table;
        ewk->wu.char_index = 0;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = 6;
        ewk->wu.kage_hy = 0;
        ewk->wu.kage_prio = ewk->wu.position_z + 5;
        ewk->wu.my_mts = oya->wu.my_mts;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
