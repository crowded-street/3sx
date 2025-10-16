/**
 * @file effb3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effb3.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effb2.h"
#include "sf33rd/Source/Game/effect/effb9.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "structs.h"

// sbss

WORK_Other* oya_adrs = NULL;

// Forward decls

void round_move_init(WORK_Other* ewk);
void round_move(WORK_Other* ewk);
void fight_move(WORK_Other* ewk);
void fight_col_move(WORK_Other* ewk);
void fight_vanish(WORK_Other* ewk);

// Funcs

void effect_B3_move(WORK_Other* ewk) {
    oya_adrs = (WORK_Other*)ewk->my_master;

    if (ewk->wu.old_rno[1] != oya_adrs->wu.routine_no[0]) {
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
    }

    ewk->wu.old_rno[1] = oya_adrs->wu.routine_no[0];

    switch (oya_adrs->wu.routine_no[0]) {
    case 1:
        round_move_init(ewk);
        break;

    case 2:
        round_move(ewk);
        break;

    case 3:
    case 5:
    case 6:
        ewk->wu.my_mr.size.x = oya_adrs->wu.my_mr.size.x;
        ewk->wu.my_mr.size.y = oya_adrs->wu.my_mr.size.y;
        disp_pos_trans_entry5(ewk);
        break;

    case 4:
        fight_move(ewk);
        break;

    case 7:
        fight_col_move(ewk);
        break;

    case 8:
        fight_vanish(ewk);
        break;

    case 9:
    case 10:
    case 99:
        ewk->wu.disp_flag = 0;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void round_move_init(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.my_mr_flag = 1;
        set_char_move_init2(&ewk->wu, 0, 2, 1, 0);
        /* fallthrough */

    case 1:
        ewk->wu.my_mr.size.x = oya_adrs->wu.my_mr.size.x;
        ewk->wu.my_mr.size.y = oya_adrs->wu.my_mr.size.y;
        disp_pos_trans_entry5(ewk);
        break;
    }
}

void round_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;

        if (ewk->wu.hit_quake) {
            Sound_SE(138);
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
            ewk->wu.xyz[0].disp.pos -= 96;
            ewk->wu.xyz[1].disp.pos = 144;
            set_char_move_init2(&ewk->wu, 0, 2, 4, 0);
        } else {
            Sound_SE(oya_adrs->wu.dir_old);
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
            ewk->wu.xyz[0].disp.pos -= 32;
            set_char_move_init2(&ewk->wu, 0, 2, 2, 0);
        }

        /* fallthrough */

    case 1:
        ewk->wu.my_mr.size.x = oya_adrs->wu.my_mr.size.x;
        ewk->wu.my_mr.size.y = oya_adrs->wu.my_mr.size.y;
        disp_pos_trans_entry5(ewk);
        break;
    }
}

void fight_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;
        set_char_move_init2(&ewk->wu, 0, 2, 5, 0);
        ewk->wu.my_mr.size.x = 63;
        ewk->wu.my_mr.size.y = 0;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
        ewk->wu.xyz[1].disp.pos = 144;
        disp_pos_trans_entry5(ewk);
        break;

    case 1:
        ewk->wu.my_mr.size.x = oya_adrs->wu.my_mr.size.x;
        ewk->wu.my_mr.size.y = oya_adrs->wu.my_mr.size.y;
        disp_pos_trans_entry5(ewk);
        break;
    }
}

void fight_col_move(WORK_Other* ewk) {
    ewk->wu.extra_col = oya_adrs->wu.extra_col;
    disp_pos_trans_entry5(ewk);
}

void fight_vanish(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;
        set_char_move_init2(&ewk->wu, 0, 4, 1, 0);
        disp_pos_trans_entry5(ewk);
        ewk->wu.my_col_code = 0x52;
        ewk->wu.extra_col = 0;
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1] += 1;
        }

        disp_pos_trans_entry5(ewk);
        break;

    case 2:
        rf_b2_flag = 1;
        disp_pos_trans_entry5(ewk);
        break;
    }
}

s32 effect_B3_init(WORK_Other* oya) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0x71;
    ewk->wu.work_id = 0x10;
    ewk->my_master = (u32*)oya;
    ewk->wu.my_family = 4;
    ewk->wu.my_col_code = 0x52;
    ewk->wu.my_priority = ewk->wu.position_z = 10;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.xyz[1].cal = 0x780000;
    ewk->wu.xyz[0].disp.low = 0;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
    ewk->wu.xyz[1].disp.pos = 0x90;
    ewk->wu.old_rno[1] = 0;

    if (oya->wu.type) {
        ewk->wu.hit_quake = 1;
    } else {
        ewk->wu.hit_quake = 0;
    }

    effect_B9_init(oya);
    return 0;
}
