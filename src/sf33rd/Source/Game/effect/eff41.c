/**
 * @file eff41.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff41.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/effect/effd9.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

void eff41_process_00(WORK_Other* ewk, PLW* mwk);
void eff41_process_01(WORK_Other* ewk, PLW* mwk);
void gauge_minus(WORK_Other* ewk, PLW* mwk);

const s16 sa_sign_data[69][5] = {
    { 28, 70, 156, 1, 1 },  { -41, 37, 156, 1, 1 },  { 12, 56, 156, 1, 1 },  { -14, 80, 156, 1, 1 },
    { -14, 48, 156, 1, 1 }, { -28, 56, 156, 1, 1 },  { -12, 56, 156, 1, 1 }, { -20, 84, 156, 1, 1 },
    { -38, 44, 156, 1, 1 }, { 48, 92, 156, 1, 1 },   { -44, 51, 156, 1, 1 }, { -30, 28, 156, 1, 1 },
    { -30, 28, 156, 1, 1 }, { 28, 70, 156, 1, 1 },   { -32, 42, 156, 1, 1 }, { 0, 72, 156, 1, 1 },
    { 14, 65, 156, 1, 1 },  { -30, 28, 156, 1, 1 },  { -58, 60, 156, 1, 1 }, { -20, 64, 156, 1, 1 },
    { -22, 66, 156, 1, 1 }, { 22, 66, 156, 1, 1 },   { 0, 48, 156, 1, 1 },   { -22, 74, 156, 1, 1 },
    { -36, 52, 156, 1, 1 }, { -32, 48, 156, 1, 1 },  { -12, 40, 156, 1, 1 }, { -24, 68, 156, 1, 1 },
    { -3, 72, 156, 1, 1 },  { 2, 72, 156, 1, 1 },    { 20, 130, 159, 2, 0 }, { 0, 72, 156, 1, 1 },
    { 28, 10, 156, 1, 1 },  { -48, 144, 158, 2, 0 }, { -16, 88, 156, 1, 1 }, { -58, 74, 156, 1, 1 },
    { -16, 88, 156, 1, 1 }, { -9, 100, 158, 2, 0 },  { -57, 48, 157, 0, 0 }, { 25, 53, 156, 1, 1 },
    { -28, 44, 156, 1, 1 }, { -22, 72, 156, 1, 1 },  { -22, 72, 156, 1, 1 }, { -21, 123, 156, 1, 1 },
    { -13, 67, 156, 1, 1 }, { -22, 56, 156, 1, 1 },  { 32, 24, 156, 1, 1 },  { -64, 62, 156, 1, 1 },
    { -32, 64, 156, 1, 1 }, { -17, 52, 156, 1, 1 },  { 3, 53, 156, 1, 1 },   { -12, 104, 160, 2, 0 },
    { 14, 56, 158, 2, 0 },  { 31, 90, 156, 1, 1 },   { -16, 80, 156, 1, 1 }, { -2, 58, 156, 1, 1 },
    { 24, 105, 156, 1, 1 }, { -37, 70, 156, 1, 1 },  { -21, 87, 156, 1, 1 }, { -11, 83, 156, 1, 1 },
    { -4, 120, 158, 1, 0 }, { -36, 76, 156, 1, 1 },  { 38, 60, 156, 1, 1 },  { -8, 74, 156, 1, 1 },
    { -5, 52, 158, 1, 0 },  { -7, 73, 156, 1, 1 },   { -8, 90, 156, 1, 1 },  { -18, 16, 156, 1, 1 },
    { 14, 56, 158, 1, 0 }
};

void (*const eff41_main_process[2])() = { eff41_process_00, eff41_process_01 };

void effect_41_move(WORK_Other* ewk) {
    PLW* mwk = (PLW*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.disp_flag = 1;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 0xA;
        ewk->wu.position_z = 24;

        switch (sa_sign_data[ewk->wu.type][3]) {
        case 1:
            sa_gauge_flash[mwk->wu.id] |= 4;
            break;
        case 2:
            sa_gauge_flash[mwk->wu.id] |= 4;
            break;
        }

        set_char_move_init(&ewk->wu, 0, sa_sign_data[ewk->wu.type][2]);
        goto jump;

    case 1:
        if (ewk->wu.dead_f == 1 || mwk->wu.routine_no[1] != 4) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            if (ewk->wu.hit_stop) {
                ewk->wu.hit_stop--;
            } else {
                char_move(&ewk->wu);
            }
        }

        if (ewk->wu.cg_type == 4) {
            ewk->wu.cg_type = 0;
            effect_D9_init(mwk, 7);
        }

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            return;
        }

    jump:
        eff41_main_process[sa_sign_data[ewk->wu.type][4]](ewk, mwk);
        ewk->wu.cg_type = 0;

        if (ewk->wu.extra_col != 0) {
            push_color_trans_req(ewk->wu.extra_col & 0x1FF, 10);
            ewk->wu.extra_col = 0;
        }

        sort_push_request(&ewk->wu);
        break;

    case 2:
        erase_my_shell_ix((WORK*)ewk->my_master, ewk->wu.myself);
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff41_process_00(WORK_Other* ewk, PLW* mwk) {
    if (ewk->wu.cg_type == 1) {
        gauge_minus(ewk, mwk);
    }

    ewk->wu.position_x = mwk->wu.position_x;

    if (mwk->wu.rl_flag) {
        ewk->wu.position_x -= sa_sign_data[ewk->wu.type][0];
    } else {
        ewk->wu.position_x += sa_sign_data[ewk->wu.type][0];
    }

    ewk->wu.position_y = mwk->wu.position_y + sa_sign_data[ewk->wu.type][1];
}

void eff41_process_01(WORK_Other* ewk, PLW* mwk) {
    switch (ewk->wu.cg_type) {
    case 1:
        gauge_minus(ewk, mwk);
        ewk->wu.routine_no[1] = 1;
        break;

    case 2:
        ewk->wu.routine_no[1] = 2;
        break;
    }

    switch (ewk->wu.routine_no[1]) {
    case 1:
        ewk->wu.position_x = mwk->wu.position_x;
        ewk->wu.position_y = 0;
        break;

    case 2:
        ewk->wu.position_x = bg_w.bgw[1].position_x + bg_w.pos_offset;
        ewk->wu.position_y = mwk->wu.position_y + sa_sign_data[ewk->wu.type][1];
        break;

    default:
        ewk->wu.position_x = mwk->wu.position_x;

        if (mwk->wu.rl_flag) {
            ewk->wu.position_x -= sa_sign_data[ewk->wu.type][0];
        } else {
            ewk->wu.position_x += sa_sign_data[ewk->wu.type][0];
        }

        ewk->wu.position_y = mwk->wu.position_y + sa_sign_data[ewk->wu.type][1];
        break;
    }
}

void gauge_minus(WORK_Other* ewk, PLW* mwk) {
    switch (sa_sign_data[ewk->wu.type][3]) {
    case 1:
        mwk->sa->saeff_ok = -1;
        grade_add_super_arts(mwk->wu.id, 1);
        break;

    case 2:
        mwk->sa->saeff_mp = -1;
        grade_add_super_arts(mwk->wu.id, 2);
        break;
    }
}

s32 effect_41_init(PLW* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if (test_flag) {
        return 0;
    }

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    write_my_shell_ix(&wk->wu, ix);
    ewk->wu.be_flag = 1;
    ewk->wu.type = data;
    ewk->wu.id = 41;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 15;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->my_master = (u32*)wk;
    ewk->master_work_id = wk->wu.work_id;
    ewk->master_id = wk->wu.id;
    *ewk->wu.char_table = _plef_char_table;
    return 0;
}
