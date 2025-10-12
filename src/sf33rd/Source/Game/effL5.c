#include "sf33rd/Source/Game/effL5.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effb2.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "structs.h"

#include "sf33rd/Source/Game/sound/se.h"

// Forward decls

void hukuromoji_move(WORK_Other* ewk);

// Funcs

void effect_L5_move(WORK_Other* ewk) {
    WORK_Other* oya_ptr = (WORK_Other*)ewk->my_master;

    switch (oya_ptr->wu.routine_no[0]) {
    case 5:
    case 6:
        hukuromoji_move(ewk);
        disp_pos_trans_entry5(ewk);
        break;

    case 7:
        ewk->wu.extra_col = oya_ptr->wu.extra_col;
        disp_pos_trans_entry5(ewk);
        break;

    case 8:
        ewk->wu.disp_flag = 0;
        disp_pos_trans_entry5(ewk);
        break;

    case 9:
    case 99:
        push_effect_work(&ewk->wu);
        break;
    }
}

void hukuromoji_move(WORK_Other* ewk) {
    WORK_Other* oya_ptr = (WORK_Other*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = 127;
        ewk->wu.my_mr.size.y = 127;
        set_char_move_init2(&ewk->wu, 0, 2, 6, 0);
        ewk->wu.hit_stop = 2;
        break;

    case 1:
        ewk->wu.hit_stop -= 1;

        if (ewk->wu.hit_stop < 0) {
            ewk->wu.routine_no[1] += 1;
            Sound_SE(oya_ptr->wu.dir_old + 1);
            return;
        }

        break;

    case 2:
        ewk->wu.my_mr.size.x -= 6;
        ewk->wu.my_mr.size.y -= 6;

        if (ewk->wu.my_mr.size.x < 64) {
            ewk->wu.routine_no[1] += 1;
            ewk->wu.my_mr.size.x = 63;
            ewk->wu.my_mr.size.y = 63;
            ewk->wu.hit_stop = 4;
            set_char_move_init2(&ewk->wu, 0, 2, 7, 0);
            return;
        }

        break;

    case 3:
        ewk->wu.hit_stop -= 1;

        if (ewk->wu.hit_stop <= 0) {
            ewk->wu.routine_no[1] += 1;
            rf_b2_flag = 1;
        }

        break;

    case 4:
        break;
    }
}

s32 effect_L5_init(WORK_Other* oya) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0xD7;
    ewk->wu.work_id = 0x10;
    ewk->my_master = (u32*)oya;
    ewk->wu.my_family = 4;
    ewk->wu.my_mts = 0xE;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.my_col_code = 0x52;
    ewk->wu.my_priority = ewk->wu.position_z = 9;
    *ewk->wu.char_table = _etc_char_table;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].position_x + bg_w.pos_offset;
    ewk->wu.xyz[1].disp.pos = 0x90;
    return 0;
}
