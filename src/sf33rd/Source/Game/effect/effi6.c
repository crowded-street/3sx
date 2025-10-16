/**
 * @file effi6.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effi6.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

// Forward decls

void effi6_line_move(WORK_Other* ewk);

// Funcs

void effect_I6_move(WORK_Other* ewk) {
    WORK_Other* oya_ptr = (WORK_Other*)ewk->my_master;

    switch (oya_ptr->wu.routine_no[0]) {
    case 2:
        effi6_line_move(ewk);
        /* fallthrough */

    case 3:
        ewk->wu.my_mr.size.x = oya_ptr->wu.my_mr.size.x;
        ewk->wu.my_mr.size.y = oya_ptr->wu.my_mr.size.y;
        disp_pos_trans_entry5(ewk);
        break;

    case 4:
        ewk->wu.disp_flag = 0;
        disp_pos_trans_entry5(ewk);
        break;

    case 5:
    case 99:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effi6_line_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = 0;
        ewk->wu.my_mr.size.y = 0;
        set_char_move_init2(&ewk->wu, 0, 2, 3, 0);
        /* fallthrough */

    case 1:
        break;
    }
}

s32 effect_I6_init(WORK_Other* oya) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0xBA;
    ewk->wu.work_id = 0x10;
    ewk->wu.cgromtype = 1;
    ewk->my_master = (u32*)oya;
    ewk->wu.my_family = 4;
    ewk->wu.my_col_code = 0x52;
    ewk->wu.my_priority = ewk->wu.position_z = 10;
    ewk->wu.my_mts = 0xE;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    *ewk->wu.char_table = (u32*)_etc_char_table;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.xyz[1].disp.pos = 0x90;
    return 0;
}
