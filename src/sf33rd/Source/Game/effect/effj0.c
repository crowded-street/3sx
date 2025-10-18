/**
 * @file effj0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effj0.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

void effect_J0_move(WORK_Other* ewk) {
    WORK_Other* mwk = (WORK_Other*)ewk->my_master;
    WORK_Other* cwk = (WORK_Other*)ewk->wu.target_adrs;
    WORK* sub_w = (WORK*)cwk->wu.target_adrs;
    ImageBuff* image_buff = (ImageBuff*)sub_w + 9;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.cg_att_ix = 0;
        ewk->wu.cg_hit_ix = 0;
        sort_push_request(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (mwk->wu.routine_no[0] >= 2 || mwk->wu.routine_no[1] >= 2) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (!EXE_flag && !Game_pause && mwk->wu.hit_stop <= 0) {
            if (--ewk->wu.dir_timer == 0) {
                ewk->wu.routine_no[0] = 2;
                break;
            }

            ewk->wu.position_x = image_buff[ewk->wu.dir_step].pos_x;
            ewk->wu.position_y = image_buff[ewk->wu.dir_step].pos_y;
        }

        ewk->wu.old_cgnum = ewk->wu.cg_number = mwk->wu.cg_number;
        ewk->wu.rl_flag = mwk->wu.rl_flag;
        ewk->wu.cg_flip = mwk->wu.cg_flip;
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_J0_init(WORK_Other* ek, WORK_Other* mk, s16 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = 2;
    ewk->wu.id = 190;
    ewk->wu.old_cgnum = ek->wu.cg_number = mk->wu.cg_number;
    ewk->wu.dir_step = data;
    ewk->wu.position_x = ewk->wu.old_pos[0] = mk->wu.position_x;
    ewk->wu.position_y = ewk->wu.old_pos[1] = mk->wu.position_y;
    ewk->wu.rl_flag = mk->wu.rl_flag;
    ewk->wu.cg_flip = mk->wu.cg_flip;
    ewk->wu.blink_timing = mk->wu.blink_timing;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = mk->wu.my_family;
    ewk->wu.cgromtype = mk->wu.cgromtype;
    ewk->wu.my_col_mode = mk->wu.my_col_mode;
    ewk->wu.my_col_code = mk->wu.my_col_code;
    ewk->wu.extra_col = mk->wu.current_colcd;
    ewk->my_master = mk;
    ewk->wu.target_adrs = ek;
    ewk->master_id = mk->wu.id;
    ewk->master_player = mk->master_player;
    ewk->wu.dir_timer = ek->wu.dir_timer;
    return 0;
}
