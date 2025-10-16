/**
 * @file effb2.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effb2.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/effb3.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effi6.h"
#include "sf33rd/Source/Game/effect/effl5.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

// rodata

const s16 effb2_sound_tbl[4] = { 0x80, 0x82, 0x84, 0x86 };
const s16 fight_col_move_tbl[18] = { 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0 };

// sbss

s16 b2_curr_no = 0;
s16 rf_b2_flag = 0;

// Forward decls

s32 b3_Break_Into_check(WORK_Other* ewk);
s32 fight_col_chg_sub(WORK_Other* ewk);

// Funcs

void effect_B2_move(WORK_Other* ewk) {
    s16 work;

    b3_Break_Into_check(ewk);

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.old_rno[0] = ewk->wu.old_rno[1] = ewk->wu.old_rno[2] = 0;
        rf_b2_flag = 0;
        b2_curr_no = 0;
        ewk->wu.hit_stop = 2;
        work = random_16();
        work &= 3;
        ewk->wu.dir_old = effb2_sound_tbl[work];
        ewk->wu.my_mr.size.x = 63;
        ewk->wu.my_mr.size.y = 63;
        break;

    case 1:
        ewk->wu.hit_stop -= 1;

        if (ewk->wu.hit_stop < 0) {
            ewk->wu.routine_no[0] += 1;
            ewk->wu.my_mr.size.y = 0;
            effect_I6_init(ewk);
            return;
        }

        break;

    case 2:
        ewk->wu.my_mr.size.y += 10;

        if (ewk->wu.my_mr.size.y >= 63) {
            ewk->wu.routine_no[0] += 1;
            ewk->wu.my_mr.size.y = 63;
            ewk->wu.hit_stop = 64;
            rf_b2_flag = 0;
            return;
        }

        break;

    case 3:
        ewk->wu.hit_stop -= 1;

        if (ewk->wu.hit_stop < 0) {
            ewk->wu.routine_no[0] += 1;
            b2_curr_no = 0;
            rf_b2_flag = 0;
            ewk->wu.hit_stop = 10;
            return;
        }

        break;

    case 4:
        ewk->wu.my_mr.size.x = 63;
        ewk->wu.my_mr.size.y = 0;
        ewk->wu.hit_stop -= 1;

        if (ewk->wu.hit_stop < 0) {
            ewk->wu.routine_no[0] += 1;
            rf_b2_flag = 0;
            effect_L5_init(ewk);
            return;
        }

        break;

    case 5:
        ewk->wu.my_mr.size.y += 6;

        if (ewk->wu.my_mr.size.y >= 63) {
            ewk->wu.routine_no[0] += 1;
            ewk->wu.my_mr.size.y = 63;
            return;
        }

        break;

    case 6:
        if (rf_b2_flag) {
            ewk->wu.routine_no[0] += 1;
            rf_b2_flag = 0;
            return;
        }

        break;

    case 7:
        if (fight_col_chg_sub(ewk) != 0) {
            ewk->wu.routine_no[0] += 1;
            rf_b2_flag = 0;
            ewk->wu.routine_no[1] = 0;
            return;
        }

        break;

    case 8:
        if (rf_b2_flag) {
            ewk->wu.routine_no[0] += 1;
            return;
        }

        break;

    case 9:
        Next_Step = 1;
        ewk->wu.routine_no[0] += 1;
        break;

    case 10:
        ewk->wu.routine_no[0] += 1;
        break;

    case 99:
        ewk->wu.routine_no[0] += 1;
        break;

    case 100:
        ewk->wu.routine_no[0] += 1;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 b3_Break_Into_check(WORK_Other* ewk) {
    if (Break_Into) {
        ewk->wu.routine_no[0] = 99;
        return 1;
    } else {
        return 0;
    }
}

s32 fight_col_chg_sub(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1] += 1;
        ewk->wu.vital_new = 0;
        ewk->wu.hit_stop = 1;
        break;

    case 1:
        ewk->wu.hit_stop -= 1;

        if (ewk->wu.hit_stop <= 0) {
            ewk->wu.hit_stop = 1;
            ewk->wu.vital_new += 1;

            if (ewk->wu.vital_new > 17) {
                ewk->wu.routine_no[1] += 1;
                ewk->wu.hit_stop = 6;
            } else {
                ewk->wu.extra_col = fight_col_move_tbl[ewk->wu.vital_new];
                ewk->wu.extra_col += 0x52;
                ewk->wu.extra_col |= 0x2000;
            }
        }

        break;

    case 2:
        ewk->wu.hit_stop -= 1;

        if (ewk->wu.hit_stop <= 0) {
            ewk->wu.routine_no[1] += 1;
            return 1;
        }

        // fallthrough

    case 3:
        disp_pos_trans_entry5(ewk);
        break;
    }

    return 0;
}

s32 effect_B2_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0x70;
    ewk->wu.work_id = 0x10;
    ewk->wu.my_family = 4;
    bg_w.bgw[3].xy[0].cal = bg_w.bgw[3].wxy[0].cal = 0x100000;
    bg_w.bgw[3].wxy[1].cal = 0;
    bg_w.bgw[3].xy[1].cal = 0;
    bg_w.bgw[3].position_x = 256 - bg_w.pos_offset;
    bg_w.bgw[3].position_y = 0;
    Bg_Family_Set_appoint(3);

    switch (save_w[Present_Mode].Battle_Number[Play_Type]) {
    case 0:
        ewk->wu.type = 0;
        break;

    case 1:
        if (Round_num == 2) {
            ewk->wu.type = 1;
        } else {
            ewk->wu.type = 0;
        }

        break;

    case 2:
        if (Round_num == 4) {
            ewk->wu.type = 1;
        } else {
            ewk->wu.type = 0;
        }

        break;

    case 3:
        if (Round_num == 6) {
            ewk->wu.type = 1;
        } else {
            ewk->wu.type = 0;
        }

        break;
    }

    effect_B3_init(ewk);
    return 0;
}
