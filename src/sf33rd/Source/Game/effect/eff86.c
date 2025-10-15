/**
 * @file eff86.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff86.h"
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

const s16 eff86_data_tbl00[7] = { 0, 2, 8224, 511, 56, 10, 18 };

const s16* eff86_adrs_tbl[1] = { eff86_data_tbl00 };

void effect_86_move(WORK_Other* ewk) {
    void (*eff86_jp_tbl[1])(WORK_Other*) = { eff86_0000 };

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            eff86_jp_tbl[ewk->wu.routine_no[1]](ewk);
        }

        disp_pos_trans_entry(ewk);
        return;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
    }
}

void eff86_0000(WORK_Other* ewk) {
    s16 work;

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.disp_flag = 1;
        work = plw[0].wu.position_x + plw[1].wu.position_x;
        work >>= 1;
        ewk->wu.xyz[0].disp.pos = work;
        work = plw[1].wu.position_y + plw[1].wu.position_y;
        work >>= 1;
        ewk->wu.xyz[1].disp.pos = work + 92;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
        }

        break;
    }
}

s32 effect_86_init(s16 type86) {
    WORK_Other* ewk;
    s16 ix;
    const s16* data_ptr;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    data_ptr = eff86_adrs_tbl[type86];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 86;
    ewk->wu.work_id = 16;
    ewk->wu.type = type86;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.dead_f = 1;
    ewk->wu.routine_no[1] = *data_ptr++;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = *data_ptr++;
    ewk->wu.my_col_code = *data_ptr++;
    ewk->wu.xyz[0].disp.pos = *data_ptr++;
    ewk->wu.xyz[1].disp.pos = *data_ptr++;
    ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
    ewk->wu.char_index = *data_ptr++;
    ewk->wu.char_table[0] = _etc_char_table;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
