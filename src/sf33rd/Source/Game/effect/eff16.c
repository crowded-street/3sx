/**
 * @file eff16.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff16.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

const u32 bunkai_table[8] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

const u16 bunkai_numobj[10] = { 32474, 32475, 32476, 32477, 32478, 32479, 32480, 32481, 32482, 32483 };

const CONN bbbs_score[4][8] = { { { -64, 164, 0, 32474 },
                                  { -80, 164, 0, 32474 },
                                  { -96, 164, 0, 32474 },
                                  { -112, 164, 0, 32474 },
                                  { -128, 164, 0, 32474 },
                                  { -144, 164, 0, 32474 },
                                  { -160, 164, 0, 32474 },
                                  { -176, 164, 0, 32474 } },
                                { { 176, 164, 0, 32474 },
                                  { 160, 164, 0, 32474 },
                                  { 144, 164, 0, 32474 },
                                  { 128, 164, 0, 32474 },
                                  { 112, 164, 0, 32474 },
                                  { 96, 164, 0, 32474 },
                                  { 80, 164, 0, 32474 },
                                  { 64, 164, 0, 32474 } },
                                { { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 } },
                                { { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 },
                                  { 0, 0, 0, 0 } } };

void eff16_trans(WORK* ewk);
static s16 score_bunkai_eff16(WORK_Other_CONN* ewk, u32 tsc);

void effect_16_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_cgnum = 0;
            ewk->free = score_bunkai_eff16((WORK_Other_CONN*)ewk, Continue_Coin[ewk->wu.type] + Score[ewk->wu.type][0]);
            ewk->wu.direction = ewk->free;
            ewk->free = 0;
            ewk->wu.dir_timer = 0;

            if (ewk->wu.kage_prio) {
                ewk->wu.position_z = 29;
                ewk->wu.my_clear_level = 128;
                break;
            }

            ewk->wu.position_z = 67;
            break;

        case 1:
            if (--ewk->wu.dir_timer <= 0) {
                ewk->wu.dir_timer = 3;
                ewk->free++;

                if (ewk->free >= ewk->wu.direction) {
                    ewk->wu.routine_no[0] = 1;
                    ewk->wu.routine_no[1] = 0;
                }
            }

            break;
        }

        eff16_trans(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        ewk->free = score_bunkai_eff16((WORK_Other_CONN*)ewk, Continue_Coin[ewk->wu.type] + Score[ewk->wu.type][0]);
        eff16_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff16_trans(WORK* ewk) {
    ewk->position_x = bg_w.bgw[2].wxy[0].disp.pos;
    ewk->position_y = bg_w.bgw[2].wxy[1].disp.pos;
    sort_push_request3(ewk);
}

s16 score_bunkai_eff16(WORK_Other_CONN* ewk, u32 tsc) {
    s16 noobjans = 0;
    s16 i;
    s16 ixs[8];
    s16 ixa[8];

    for (i = 7; i > 0; i--) {
        ixa[i] = tsc / bunkai_table[i];
        ixs[i] = tsc %= bunkai_table[i];
    }

    ixa[i] = tsc;

    for (i = 0; i < 8; i++) {
        if ((ewk->conn[i].chr = bunkai_numobj[ixa[i]]) != 32474) {
            noobjans = i + 1;
        }
    }

    return noobjans;
}

s32 effect_16_init(PLW* wk, s16 flag) {
    WORK_Other_CONN* ewk;
    s16 ix;
    s16 i;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 16;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.my_family = 3;
    ewk->wu.cgromtype = 1;
    ewk->wu.type = (wk->wu.id + 1) & 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 73;
    ewk->wu.kage_prio = flag;
    ewk->num_of_conn = 8;

    for (i = 0; i < 8; i++) {
        ewk->conn[i] = bbbs_score[ewk->wu.type][i];
    }

    return 0;
}
