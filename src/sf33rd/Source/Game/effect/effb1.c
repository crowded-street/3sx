/**
 * @file effb1.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effb1.h"
#include "common.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/sound/se_data.h"

const s16 effB1_wait_table[4] = { 2, 2, 2, 2 };

const s16 effB1_wait_tbl_2[4] = { 2, 2, 2, 2 };

const CONN bbbs_clear = { 0, 0, 2, 36 };

const CONN bbbs_blocking[20] = { { -172, -12, 0, 32484 }, { -154, -12, 0, 32484 }, { -136, -12, 0, 32484 },
                                 { -118, -12, 0, 32484 }, { -100, -12, 0, 32484 }, { -82, -12, 0, 32484 },
                                 { -64, -12, 0, 32484 },  { -46, -12, 0, 32484 },  { -28, -12, 0, 32484 },
                                 { -10, -12, 0, 32484 },  { -172, 4, 0, 32484 },   { -154, 4, 0, 32484 },
                                 { -136, 4, 0, 32484 },   { -118, 4, 0, 32484 },   { -100, 4, 0, 32484 },
                                 { -82, 4, 0, 32484 },    { -172, 20, 0, 32484 },  { -154, 20, 0, 32484 },
                                 { -136, 20, 0, 32484 },  { -172, 36, 0, 32484 } };

void effB1_trans(WORK* ewk);
void effB1_mark_change(WORK_Other_CONN* ewk);
void effB1_mark_exchange(WORK_Other_CONN* ewk);

void effect_B1_move(WORK_Other_CONN* ewk) {
    s16 i;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            ewk->wu.my_priority = 67;
            ewk->wu.direction = ewk->num_of_conn;
            ewk->num_of_conn = 0;
            ewk->wu.dir_timer = 0;
            ewk->wu.dir_step = Bonus_Game_result;

            if (ewk->wu.kage_prio) {
                ewk->wu.position_z = 8;
                ewk->wu.my_clear_level = 128;
            } else {
                ewk->wu.position_z = 67;
            }
            break;

        case 1:
            if (Game_pause) {
                break;
            }

            if (EXE_flag) {
                break;
            }

            if (--ewk->wu.dir_timer > 0) {
                break;
            }

            if (ewk->wu.kage_prio) {
                sound_effect_request[167](ewk, 167);
            }

            ewk->wu.dir_timer = 3;
            ewk->num_of_conn++;

            if (ewk->num_of_conn >= ewk->wu.direction) {
                ewk->wu.routine_no[0] = 1;
                ewk->wu.routine_no[1] = 0;
            }

            break;
        }

        effB1_trans(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (ewk->wu.dir_step > Bonus_Game_result) {
            ewk->wu.routine_no[1] = 1;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (Bonus_Game_result) {
                for (i = 0; i < Bonus_Game_result; i++) {
                    if (!ewk->conn[i + 20].nx) {
                        ewk->conn[i + 20].nx = 1;
                    }
                }

                effB1_mark_change(ewk);
            }

            break;

        default:
            if (ewk->wu.dir_step != Bonus_Game_result) {
                for (i = Bonus_Game_result; i < ewk->wu.dir_step; i++) {
                    if (ewk->conn[i + 20].nx != 2) {
                        ewk->conn[i + 20].nx = 2;
                        ewk->conn[i + 20].ny = 0;
                    }
                }
            }

            effB1_mark_exchange(ewk);
            break;
        }

        ewk->wu.dir_step = Bonus_Game_result;
        effB1_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effB1_trans(WORK* ewk) {
    ewk->position_x = bg_w.bgw[2].wxy[0].disp.pos;
    ewk->position_y = bg_w.bgw[2].wxy[1].disp.pos;
    sort_push_request3(ewk);
}

void effB1_mark_change(WORK_Other_CONN* ewk) {
    s16 i;

    for (i = 0; i < Bonus_Game_result; i++) {
        switch (ewk->conn[i + 20].ny) {
        default:
            if (--ewk->conn[i + 20].col > 0) {
                break;
            }

            /* fallthrough */

        case 0:
            ewk->conn[i + 20].col = effB1_wait_table[ewk->conn[i + 20].ny];
            ewk->conn[i + 20].ny++;
            ewk->conn[i].chr++;
            break;

        case 4:
            break;
        }
    }
}

void effB1_mark_exchange(WORK_Other_CONN* ewk) {
    s16 i;

    for (i = 0; i < ewk->wu.direction; i++) {
        if (ewk->conn[i + 20].nx == 0) {
            continue;
        }

        if (ewk->conn[i + 20].nx == 1) {
            continue;
        }

        switch (ewk->conn[i + 20].ny) {
        default:
            if (--ewk->conn[i + 20].col > 0) {
                break;
            }

            /* fallthrough */

        case 0:
            ewk->conn[i + 20].col = effB1_wait_tbl_2[ewk->conn[i + 20].ny];
            ewk->conn[i + 20].ny++;
            ewk->conn[i].chr--;
            break;

        case 4:
            break;
        }
    }
}

s32 effect_B1_init(PLW* wk, s32 flag) {
    WORK_Other_CONN* ewk;
    s16 ix;
    s16 i;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 111;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = 3;
    ewk->wu.cgromtype = 1;
    ewk->wu.kage_prio = flag;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 73;
    ewk->num_of_conn = 20;

    for (i = 0; i < 20; i++) {
        ewk->conn[i] = bbbs_blocking[i];
    }

    for (i = 0; i < 20; i++) {
        ewk->conn[i + 20] = bbbs_clear;
    }

    if (wk->wu.rl_flag) {
        for (i = 0; i < 20; i++) {
            ewk->conn[i].nx = -ewk->conn[i].nx;
        }
    }

    return 0;
}
