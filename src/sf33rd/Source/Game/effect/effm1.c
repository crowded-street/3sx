/**
 * @file effm1.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effm1.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

void effm1_move(WORK_Other* ewk);

void effect_M1_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = 0;
        ewk->wu.kage_hy = 0;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 16;
        set_char_move_init(&ewk->wu, 0, 12);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            effm1_move(ewk);
        }

        pl_eff_trans_entry(ewk);
        break;

    case 99:
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effm1_move(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (oya_ptr->cmwk[0]) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, 13);
        } else {
            char_move(&ewk->wu);
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0xFF) {
            break;
        }

        oya_ptr->cmwk[0] = 2;
        ewk->wu.routine_no[1]++;
        set_char_move_init2(&ewk->wu, 0, 0, 3, 0);
        ewk->wu.rl_flag ^= 1;
        ewk->wu.old_rno[0] = 60;
        cal_initial_speed(&ewk->wu, ewk->wu.old_rno[0], ewk->wu.old_rno[1], ewk->wu.xyz[1].disp.pos);
        break;

    case 2:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[1]++;
            ewk->wu.routine_no[0] = 99;
            ewk->wu.disp_flag = 0;
            break;
        }

        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;
    }
}

s32 effect_M1_init(WORK* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 221;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code + 1;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = wk;
    ewk->wu.rl_flag = wk->rl_flag;
    ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos;
    ewk->wu.xyz[0].disp.pos += wk->rl_flag ? -64 : 64;
    ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos - 2;
    ewk->wu.position_z = wk->my_priority - 12;
    ewk->wu.my_priority = wk->my_priority - 12;
    *ewk->wu.char_table = _etc3_char_table;
    ewk->wu.sync_suzi = 0;

    if (wk->rl_flag) {
        ewk->wu.old_rno[1] = bg_w.bgw[1].wxy[0].disp.pos - (bg_w.pos_offset + 32);
    } else {
        ewk->wu.old_rno[1] = bg_w.bgw[1].wxy[0].disp.pos + (bg_w.pos_offset + 32);
    }

    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
