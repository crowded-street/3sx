/**
 * @file effi4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effi4.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effj6.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

void effect_I4_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);

        if (eff_hit_flag[1]) {
            ewk->wu.routine_no[0] = 2;
            break;
        }

        break;

    case 1:
        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            effect_i4_hit_sub(ewk);
        }

        /* fallthrough */

    case 2:
        disp_pos_trans_entry_r(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void effect_i4_hit_sub(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (eff_hit_check2(ewk, 0, 2)) {
            set_char_move_init(&ewk->wu, 0, 25);
            ewk->wu.routine_no[1]++;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.routine_no[3] = 0;
            break;
        }

        if (eff_hit_check2(ewk, 0, 0)) {
            set_char_move_init(&ewk->wu, 0, 24);
            ewk->wu.routine_no[1]++;
            ewk->wu.routine_no[2] = 0;
            ewk->wu.routine_no[3] = 0;
        }

        break;

    case 1:
        if (ewk->wu.routine_no[2]) {
            effi4_up_to_down(ewk);
            break;
        }

        effi4_down_to_up(ewk);
        break;
    }
}

void effi4_down_to_up(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[3]) {
    case 0:
        char_move(&ewk->wu);

        if (!ewk->wu.cg_type) {
            break;
        }

        ewk->wu.type = 2;

        if (eff_hit_check2(ewk, 0, 2)) {
            ewk->wu.routine_no[3]++;
            break;
        }

        if (ewk->wu.cg_type == 0xFF) {
            eff_hit_flag[1] = 0;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.type = 1;
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[3]++;
            set_char_move_init(&ewk->wu, 0, 26);
            break;
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1] = 99;
            ewk->wu.routine_no[0] = 2;
        }

        break;
    }
}

void effi4_up_to_down(WORK_Other* ewk) {
    char_move(&ewk->wu);

    if (ewk->wu.cg_type == 0xFF) {
        ewk->wu.routine_no[1] = 99;
        ewk->wu.routine_no[0] = 2;
    }
}

s32 effect_I4_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 184;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.type = 1;
    ewk->wu.dead_f = 0;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = 8492;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.char_table[0] = _chn_char_table;
    ewk->wu.xyz[0].disp.pos = 832;
    ewk->wu.xyz[1].disp.pos = 192;
    ewk->wu.my_priority = ewk->wu.position_z = 72;
    ewk->wu.char_index = 5;
    ewk->wu.routine_no[1] = 0;
    effect_J6_init(ewk);
    return 0;
}
