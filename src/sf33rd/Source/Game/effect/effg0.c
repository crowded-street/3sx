/**
 * @file effg0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effg0.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/screen/sel_data.h"
#include "sf33rd/Source/Game/stage/bg.h"

void Check_Die_G0(WORK_Other_CONN* ewk);
void effG0_trans(WORK* ewk);
void Flash_G0(WORK_Other_CONN* ewk);

const u32 bunkai_table_G0[6] = { 1, 10, 100, 1000, 10000, 100000 };

const u16 bunkai_numobj_G0[10] = { 27159, 27160, 27161, 27162, 27163, 27164, 27165, 27166, 27167, 27168 };

const CONN Result_Score[6] = { { 40, 0, 0, 27159 }, { 32, 0, 0, 27159 }, { 24, 0, 0, 27159 },
                               { 16, 0, 0, 27159 }, { 8, 0, 0, 27159 },  { 0, 0, 0, 27159 } };

void effect_G0_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            if (--ewk->wu.dir_timer) {
                break;
            }

            ewk->wu.routine_no[0] = 1;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_cgnum = 0;
            /* fallthrough */

        default:
            effG0_trans(&ewk->wu);
            break;
        }

        break;

    case 1:
        Check_Die_G0((WORK_Other_CONN*)ewk);

        switch (ewk->wu.routine_no[1]) {
        case 0:
            ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
            ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

            if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
                ewk->wu.routine_no[1]++;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
                Order_Dir[ewk->wu.dir_old] = 0;
            }

            break;

        case 1:
            switch (Order[ewk->wu.dir_old]) {
            case 0:
                break;

            case 1:
                Flash_G0((WORK_Other_CONN*)ewk);
                break;
            }

            break;
        }

        effG0_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void Check_Die_G0(WORK_Other_CONN* ewk) {
    if (Suicide[2]) {
        ewk->wu.disp_flag = 0;
        ewk->wu.type = 0;
        ewk->wu.routine_no[0] = 2;
    }
}

void effG0_trans(WORK* ewk) {
    ewk->cg_number = (ewk->cg_number + 1) & 0x7FFF;

    if (ewk->cg_number == 0) {
        ewk->cg_number = 1;
    }

    ewk->position_x = ewk->xyz[0].disp.pos;
    ewk->position_y = bg_w.bgw[ewk->my_family - 1].xy[1].disp.pos + ewk->xyz[1].disp.pos;
    sort_push_request3(ewk);
}

void Flash_G0(WORK_Other_CONN* ewk) {
    s16 ix;

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.vital_new = 4;
        ewk->wu.dir_timer = 1;
        /* fallthrough */

    case 1:
        if (--ewk->wu.dir_timer != 0) {
            break;
        }

        ewk->wu.routine_no[2] = 2;
        ewk->wu.dir_timer = 3;

        for (ix = 0; ix < ewk->num_of_conn; ix++) {
            ewk->conn[ix].chr += 10;
        }

        break;

    case 2:
        if (--ewk->wu.dir_timer != 0) {
            break;
        }

        for (ix = 0; ix < ewk->num_of_conn; ix++) {
            ewk->conn[ix].chr -= 10;
        }

        if (--ewk->wu.vital_new == 0) {
            Order[ewk->wu.dir_old] = 0;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.routine_no[2] = 0;
            break;
        }

        ewk->wu.routine_no[2] = 1;
        ewk->wu.dir_timer = 3;
        break;
    }
}

s16 score_bunkai_G0(WORK_Other_CONN* ewk, u32 tsc) {
    s16 noobjans = 0;
    s16 i;
    s16 ixs[6];
    s16 ixa[6];

    for (i = 5; i > 0; i--) {
        ixa[i] = tsc / bunkai_table_G0[i];
        ixs[i] = tsc %= bunkai_table_G0[i];
    }

    ixa[i] = tsc;

    for (i = 0; i < 6; i++) {
        if ((ewk->conn[i].chr = bunkai_numobj_G0[ixa[i]]) != 27159) {
            noobjans = i + 1;
        }
    }

    if (noobjans == 0) {
        return 1;
    }

    return noobjans;
}

s32 effect_G0_init(s16 Order, s16 Time, u32 Score, s16 Pos_Index) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 160;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = 2;
    ewk->wu.dir_old = Order;
    ewk->wu.dir_timer = Time;
    ewk->wu.dm_butt_type = Score;
    ewk->wu.my_col_code = 82;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->num_of_conn = 6;

    for (ix = 0; ix < 6; ix++) {
        ewk->conn[ix] = Result_Score[ix];
    }

    if (Perfect_Flag) {
        ix = 1;
    } else {
        ix = 0;
    }

    ewk->wu.hit_quake = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Compute_Score_Pos_Data_G0[ix][Pos_Index][0];
    ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake + 416;
    ewk->wu.xyz[1].disp.pos = Compute_Score_Pos_Data_G0[ix][Pos_Index][1];
    ewk->wu.my_priority = ewk->wu.position_z = 5;
    ewk->wu.mvxy.a[0].sp = 0xFFFE8000;
    ewk->wu.mvxy.d[0].sp = 0xFFFD8000;
    ewk->num_of_conn = score_bunkai_G0((WORK_Other_CONN*)ewk, Score);
    return 0;
}
