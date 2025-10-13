/**
 * @file effb4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effb4.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 s_mark_tbl[32] = { 0, 1, 0, 1, 0, 3, 0, 0, 0, 2, 4, 0, 0, 1, 0, 2,
                             6, 3, 0, 2, 0, 0, 6, 3, 0, 4, 0, 0, 6, 0, 0, 4 };

void effect_B4_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_rno[0] = random_16();
        ewk->wu.old_rno[0] &= 0x1F;
        ewk->wu.char_index += s_mark_tbl[ewk->wu.old_rno[0]];
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[0]++;
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 0;
        break;

    case 3:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }
}

s32 effect_B4_init(WORK_Other* oya) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 114;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->my_master = (u32*)oya;
    ewk->master_id = 0;
    ewk->wu.my_col_mode = 0;
    ewk->wu.my_col_code = 0x20;
    ewk->wu.my_family = 2;
    ewk->wu.position_z = oya->wu.position_z - 1;
    ewk->wu.char_table[0] = _etc_char_table;
    ewk->wu.char_index = 34;
    ewk->wu.sync_suzi = 0;
    ewk->wu.rl_flag = 0;
    ewk->wu.xyz[0].disp.pos = oya->wu.xyz[0].disp.pos;
    ewk->wu.xyz[1].disp.pos = oya->wu.xyz[1].disp.pos;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
