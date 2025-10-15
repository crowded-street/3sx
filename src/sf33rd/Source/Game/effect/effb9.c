/**
 * @file effb9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effb9.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

// sbss

WORK_Other* oya_p = NULL;

// Funcs

void effect_B9_move(WORK_Other* ewk) {
    oya_p = (WORK_Other*)ewk->my_master;

    switch (oya_p->wu.routine_no[0]) {
    case 2:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1] += 1;
            ewk->wu.disp_flag = 1;
            set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);
            ewk->wu.my_mr_flag = 1;
            /* fallthrough */

        case 1:
            ewk->wu.my_mr.size.x = oya_p->wu.my_mr.size.x;
            ewk->wu.my_mr.size.y = oya_p->wu.my_mr.size.y;
            disp_pos_trans_entry5(ewk);
            break;
        }

        break;

    case 3:
        ewk->wu.my_mr.size.x = oya_p->wu.my_mr.size.x;
        ewk->wu.my_mr.size.y = oya_p->wu.my_mr.size.y;
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

s32 effect_B9_init(WORK_Other* oya) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0x77;
    ewk->wu.work_id = 0x10;
    ewk->my_master = (u32*)oya;
    ewk->wu.my_family = 4;
    ewk->wu.my_col_code = 0x52;
    ewk->wu.my_priority = ewk->wu.position_z = 10;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.my_mts = 0xE;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    if (oya->wu.type) {
        ewk->wu.old_rno[0] = 2;
        ewk->wu.char_index = 2;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
        ewk->wu.xyz[0].disp.pos += 0x50;
        ewk->wu.xyz[1].disp.pos = 0x90;
    } else {
        ewk->wu.old_rno[0] = 3;
        ewk->wu.char_index = Round_num + 1;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
        ewk->wu.xyz[0].disp.pos += 0x70;
        ewk->wu.xyz[1].disp.pos = 0x90;
    }

    return 0;
}
