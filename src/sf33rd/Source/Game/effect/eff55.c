/**
 * @file eff55.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff55.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void effect_55_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 3);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.xyz[1].cal += 0x3000;

            if (ewk->wu.xyz[1].disp.pos >= 128) {
                ewk->wu.routine_no[0]++;
                ewk->wu.old_rno[0] = 300;
            }
        }

        disp_pos_trans_entry(ewk);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 0) {
                ewk->wu.routine_no[0]++;
            }
        }

        disp_pos_trans_entry(ewk);
        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.xyz[1].cal -= 0x4000;

            if (ewk->wu.xyz[1].disp.pos < 97) {
                ewk->wu.routine_no[0]++;
                ewk->wu.old_rno[0] = 480;
            }
        }

        disp_pos_trans_entry(ewk);
        break;

    case 4:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] < 0) {
                ewk->wu.routine_no[0] = 1;
            }
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_55_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 55;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = 8492;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.xyz[0].disp.pos = 511;
    ewk->wu.xyz[1].disp.pos = 96;
    ewk->wu.my_priority = 86;
    ewk->wu.position_z = 86;
    ewk->wu.hit_stop = 0;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _brz_char_table;
    suzi_offset_set(ewk);
    return 0;
}
