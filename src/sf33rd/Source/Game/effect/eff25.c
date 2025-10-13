/**
 * @file eff25.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff25.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff26.h"
#include "sf33rd/Source/Game/effect/eff27.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 eff25_data_0000[16] = { 0, 2, 300, 160, 32, 71, 6, 0, 0, 0, 6, 6, 66, 9, 0, -1 };

const s16* scr_obj_data25[1] = { eff25_data_0000 };

void (*eff25_jp_tbl[10])(WORK_Other* ewk) = { eff25_00, eff25_00, eff25_02, eff25_02, eff25_04,
                                              eff25_04, eff25_06, eff25_06, eff25_08, eff25_08 };

void effect_25_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.routine_no[0] = 99;
        ewk->wu.disp_flag = 0;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (!EXE_flag && !Game_pause) {
            eff25_jp_tbl[ewk->wu.old_rno[2]](ewk);
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    case 1:
    case 2:
    case 3:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0]++;
        break;

    case 4:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff25_00(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (eff_hit_flag[ewk->wu.type]) {
            ewk->wu.routine_no[0] = 4;
            break;
        }

        eff25_char_set(ewk);
        break;

    case 1:
        if (eff_hit_check(ewk, ewk->wu.old_rno[4])) {
            piece_set(ewk);
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[1]);
            ewk->wu.routine_no[1]++;
            break;
        }

        if (ewk->wu.hit_stop && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        break;

    case 2:
        if (!EXE_obroll) {
            char_move(&ewk->wu);
        }

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1]++;
        }

        /* fallthrough */

    case 3:
        ewk->wu.routine_no[0]++;
        break;
    }
}

void eff25_02(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (eff_hit_flag[ewk->wu.type]) {
            ewk->wu.disp_flag = 1;
            ewk->wu.routine_no[1] = 4;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[7]);
            break;
        }

        eff25_char_set(ewk);
        break;

    case 1:
        if (eff_hit_check(ewk, ewk->wu.old_rno[4])) {
            piece_set(ewk);
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[1]);
            break;
        }

        if (ewk->wu.hit_stop && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        break;

    case 2:
        if (!EXE_obroll) {
            char_move(&ewk->wu);
        }

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 3:
        break;

    case 4:
        if (!EXE_obroll) {
            char_move(&ewk->wu);
        }

        break;
    }
}

void eff25_04(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (eff_hit_flag[ewk->wu.type]) {
            ewk->wu.routine_no[0] = 4;
            break;
        }

        eff25_char_set(ewk);
        break;

    case 1:
        if (eff_hit_check(ewk, ewk->wu.old_rno[4])) {
            piece_set(ewk);
            ewk->wu.routine_no[1]++;
            break;
        }

        if (ewk->wu.hit_stop && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        break;

    case 2:
        ewk->wu.routine_no[0]++;
        break;
    }
}

void eff25_06(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (eff_hit_flag[ewk->wu.type]) {
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[7]);
            ewk->wu.routine_no[1] = 4;
            break;
        }

        eff25_char_set(ewk);
        break;

    case 1:
        if (eff_hit_check(ewk, ewk->wu.old_rno[4])) {
            piece_set(ewk);
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[1]);
            break;
        }

        if (ewk->wu.hit_stop && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        break;

    case 2:
        if (!EXE_obroll) {
            char_move(&ewk->wu);
        }

        if (ewk->wu.cg_type) {
            set_char_move_init(&ewk->wu, 0, ewk->wu.old_rno[3]);
            ewk->wu.routine_no[1]++;
            break;
        }

        break;

    case 3:
        break;

    case 4:
        if (!EXE_obroll) {
            char_move(&ewk->wu);
        }

        break;
    }
}

void eff25_08(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (eff_hit_flag[ewk->wu.type]) {
            ewk->wu.routine_no[0] = 4;
            break;
        }

        eff25_char_set(ewk);
        break;

    case 1:
        if (eff_hit_check(ewk, ewk->wu.old_rno[4])) {
            piece_set(ewk);
            ewk->wu.routine_no[1]++;
            break;
        }

        if (ewk->wu.hit_stop && !EXE_obroll) {
            char_move(&ewk->wu);
        }

        break;

    case 2:
        ewk->wu.routine_no[0]++;
        break;
    }
}

void eff25_char_set(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

void piece_set(WORK_Other* ewk) {
    if (!(ewk->wu.old_rno[2] & 1)) {
        return;
    }

    if (ewk->wu.old_rno[0] < 0) {
        return;
    }

    effect_27_init(ewk, ewk->wu.old_rno[0]);
}

s32 effect_25_init(s8 num) {
    WORK_Other* ewk;
    s16 ix;
    const s16* data_ptr = scr_obj_data25[num];

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 25;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.type = num;
    ewk->wu.dead_f = *data_ptr++;
    ewk->wu.my_family = *data_ptr++;
    ewk->wu.my_col_code = *data_ptr++;
    ewk->wu.xyz[0].disp.pos = *data_ptr++;
    ewk->wu.xyz[1].disp.pos = *data_ptr++;
    ewk->wu.my_priority = *data_ptr++;
    ewk->wu.char_index = *data_ptr++;
    ewk->wu.hit_stop = *data_ptr++;
    ewk->wu.sync_suzi = *data_ptr++;
    ewk->wu.old_rno[0] = *data_ptr++;
    ewk->wu.old_rno[1] = *data_ptr++;
    ewk->wu.old_rno[7] = *data_ptr++;
    ewk->wu.old_rno[3] = *data_ptr++;
    ewk->wu.old_rno[2] = *data_ptr++;
    ewk->wu.old_rno[4] = *data_ptr++;
    ewk->wu.position_z = ewk->wu.my_priority;

    if (*data_ptr >= 0) {
        effect_26_init(ewk, *data_ptr);
    }

    data_ptr++;
    ewk->wu.char_table[0] = char_add[bg_w.bg_index];
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
