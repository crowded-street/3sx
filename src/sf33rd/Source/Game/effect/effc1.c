/**
 * @file effc1.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc1.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void effect_C1_move(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;
    s16 work;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        setup_shadow_of_the_Effy(&ewk->wu);
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.old_rno[0] = 64;

        if (oya_ptr->char_index == 67) {
            work = oya_ptr->xyz[0].disp.pos;
        } else if (oya_ptr->rl_flag) {
            work = oya_ptr->xyz[0].disp.pos + 74;
        } else {
            work = oya_ptr->xyz[0].disp.pos - 74;
        }

        cal_all_speed_data(&ewk->wu, ewk->wu.old_rno[0], work, ewk->wu.xyz[1].disp.pos, 2, 2);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.old_rno[0]--;
            add_x_sub(ewk);
            add_y_sub(ewk);
            work = ewk->wu.xyz[0].disp.pos - oya_ptr->xyz[0].disp.pos;

            if (work < 0) {
                work = -work;
            }

            if (work < 0x91) {
                ewk->wu.routine_no[0]++;
                Sound_SE((ewk->master_id * 0x300) + 0x15E);
                char_move_z(&ewk->wu);
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
            add_x_sub(ewk);
            add_y_sub(ewk);
            ewk->wu.old_rno[0]--;

            if (ewk->wu.old_rno[0] <= 0) {
                ewk->wu.routine_no[0]++;

                if (oya_ptr->char_index == 67) {
                    set_char_move_init(&ewk->wu, 0, 37);
                } else {
                    set_char_move_init(&ewk->wu, 0, 38);
                }
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;
    }
}

#if defined(TARGET_PS2)
s32 effect_C1_init(WORK* wk, s32 /* unused */) {
    s16 get_my_trans_mode(s32 curr);
#else
s32 effect_C1_init(WORK* wk) {
#endif

    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 121;
    ewk->wu.work_id = 16;
    ewk->master_id = wk->id;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code + 6;
    ewk->wu.my_family = wk->my_family;
    ewk->my_master = (u32*)wk;
    ewk->wu.rl_flag = wk->rl_flag;

    if (wk->rl_flag) {
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + (bg_w.pos_offset + 16);
    } else {
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos - (bg_w.pos_offset + 16);
    }

    ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos - 16;
    ewk->wu.my_priority = wk->my_priority - 12;
    ewk->wu.position_z = ewk->wu.my_priority - 12;
    *ewk->wu.char_table = _etc2_char_table;
    ewk->wu.char_index = 36;
    ewk->wu.sync_suzi = 0;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
