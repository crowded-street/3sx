/**
 * @file effc8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc8.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s32 effc8_data_tbl[4] = { 0x30000, 0x200, 0, -0x1800 };

void effect_C8_move(WORK_Other* ewk) {
    PLW* oya_pl = (PLW*)ewk->my_master;
    s16 work;
    const s32* ptr;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 12);
        break;

    case 1:
        switch (oya_pl->wu.routine_no[3]) {
        case 1:
            break;

        case 0:
        case 2:
            if (oya_pl->wu.cg_ix != ewk->wu.cg_ix) {
                work = oya_pl->wu.cg_ix / oya_pl->wu.cgd_type + 1;
                set_char_move_init2(&ewk->wu, 0, 12, work + 1, 0);
                ewk->wu.cg_ix = oya_pl->wu.cg_ix;
            }

            break;

        case 3:
            ewk->wu.routine_no[0]++;
            break;
        }

        pl_eff_trans_entry(ewk);
        break;

    case 2:
        ewk->wu.routine_no[0]++;
        set_char_move_init(&ewk->wu, 0, 13);
        ptr = effc8_data_tbl;

        if (oya_pl->wu.id) {
            ewk->wu.xyz[0].disp.pos += 61;
            ewk->wu.mvxy.a[0].sp = *ptr++;
            ewk->wu.mvxy.d[0].sp = *ptr++;
            ewk->wu.mvxy.a[1].sp = *ptr++;
            ewk->wu.mvxy.d[1].sp = *ptr++;
        } else {
            ewk->wu.xyz[0].disp.pos -= 61;
            ewk->wu.mvxy.a[0].sp = -*ptr;
            ptr++;
            ewk->wu.mvxy.d[0].sp = -*ptr;
            ptr++;
            ewk->wu.mvxy.a[1].sp = *ptr++;
            ewk->wu.mvxy.d[1].sp = *ptr++;
        }

        ewk->wu.xyz[1].disp.pos = 137;
        pl_eff_trans_entry(ewk);
        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            add_x_sub(ewk);
            add_y_sub(ewk);
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[0]++;
                ewk->wu.disp_flag = 0;
            }
        }

        pl_eff_trans_entry(ewk);
        break;

    case 4:
        ewk->wu.routine_no[0]++;
        break;

    case 5:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_C8_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(2)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 128;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_family = 2;
    ewk->wu.rl_flag = wk->wu.rl_flag;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_priority = ewk->wu.position_z = 20;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.my_col_code = wk->wu.my_col_code;
    ewk->my_master = (u32*)wk;
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos = wk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = wk->wu.xyz[1].disp.pos;
    ewk->wu.xyz[0].disp.low = ewk->wu.xyz[1].disp.low = 0;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
