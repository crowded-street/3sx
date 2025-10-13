/**
 * @file effl1.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effl1.h"
#include "common.h"
#include "sf33rd/Source/Game/Grade.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effm3.h"
#include "sf33rd/Source/Game/workuser.h"

void effL1_trans(WORK* ewk);
void grade_data_disp();
void effL1_w_win_init(WORK_Other_CONN* ewk);
void effL1_w_grade_init(WORK_Other_CONN* ewk);
void effL1_k_grade_init(WORK_Other_CONN* ewk);
void effL1_w_score_init(WORK_Other_CONN* ewk);
void effL1_w_graph_init(WORK_Other_CONN* ewk);
void effL1_k_graph_init(WORK_Other_CONN* ewk);
void effL1_f_stage_p_init(WORK_Other_CONN* ewk);
void effL1_f_stage_r_init(WORK_Other_CONN* ewk);
void effL1_f_grade_init(WORK_Other_CONN* ewk);
void effL1_f_mk_spp_init(WORK_Other_CONN* ewk);
void effL1_f_mk_all_init(WORK_Other_CONN* ewk);
void effL1_f_kz_cont_init(WORK_Other_CONN* ewk);
void effL1_f_kz_spp_init(WORK_Other_CONN* ewk);
void effL1_f_score_init(WORK_Other_CONN* ewk);
void effL1_suuchi_bunkai_sub(WORK_Other_CONN* ewk, u32 tsc);

const u32 bunkai_table_l1[8] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

void (*const effL1_item_init[20])() = {
    effL1_w_win_init,    effL1_w_grade_init,  effL1_w_score_init,   effL1_w_graph_init,   effL1_w_graph_init,
    effL1_w_graph_init,  effL1_w_graph_init,  effL1_f_stage_p_init, effL1_f_stage_r_init, effL1_f_grade_init,
    effL1_f_mk_spp_init, effL1_f_mk_all_init, effL1_f_kz_cont_init, effL1_f_kz_spp_init,  effL1_f_score_init,
    effL1_k_grade_init,  effL1_k_graph_init,  effL1_k_graph_init,   effL1_k_graph_init,   effL1_k_graph_init
};

const s16 effL1_base_data[20][5] = { { 64, 1, 1, 1, 0 }, { 60, 3, 0, 0, 92 }, { 64, 3, 1, 1, 0 }, { 68, 3, 0, 1, 0 },
                                     { 68, 3, 0, 1, 0 }, { 68, 3, 0, 1, 0 },  { 68, 3, 0, 1, 0 }, { 18, 1, 0, 1, 0 },
                                     { 17, 1, 0, 1, 0 }, { 15, 1, 0, 1, 0 },  { 18, 1, 0, 1, 8 }, { 19, 1, 0, 1, 0 },
                                     { 19, 1, 0, 1, 0 }, { 19, 1, 0, 1, 0 },  { 19, 1, 0, 1, 0 }, { 60, 3, 0, 1, 0 },
                                     { 68, 3, 0, 1, 0 }, { 68, 3, 0, 1, 0 },  { 68, 3, 0, 1, 0 }, { 68, 3, 0, 1, 0 } };

const CONN gj_score[8] = { { -63, 49, 0, 0x6F5C },  { -71, 49, 0, 0x6F5C }, { -79, 49, 0, 0x6F5C },
                           { -87, 49, 0, 0x6F5C },  { -95, 49, 0, 0x6F5C }, { -103, 49, 0, 0x6F5C },
                           { -111, 49, 0, 0x6F5C }, { -119, 49, 0, 0x6F5C } };

const CONN gj_wins[3] = { { 175, 54, 0, 0x6ED7 }, { 162, 54, 0, 0x6ED7 }, { 149, 54, 0, 0x6ED7 } };

const CONN gj_grade[32][4] = {
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F43 }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F43 }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F43 }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F42 }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F42 }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F42 }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F41 }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F41 }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F38 }, { -29, 75, 0, 0x6F41 }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F37 }, { -29, 75, 0, 0x6F40 }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F37 }, { -29, 75, 0, 0x6F40 }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F37 }, { -29, 75, 0, 0x6F40 }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F37 }, { -29, 75, 0, 0x6F3F }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F37 }, { -29, 75, 0, 0x6F3F }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F37 }, { -29, 75, 0, 0x6F3F }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F36 }, { -29, 75, 0, 0x6F3E }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F36 }, { -29, 75, 0, 0x6F3E }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F36 }, { -29, 75, 0, 0x6F3E }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F35 }, { -29, 75, 0, 0x6F3D }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F35 }, { -29, 75, 0, 0x6F3D }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F35 }, { -29, 75, 0, 0x6F3D }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F35 }, { -29, 75, 0, 0x6F3C }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F35 }, { -29, 75, 0, 0x6F3C }, { -29, 75, 0, 0x6F44 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F35 }, { -29, 75, 0, 0x6F3C }, { -29, 75, 0, 0x6F45 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F34 }, { -29, 75, 0, 0x6F3B }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F34 }, { -29, 75, 0, 0x6F3B }, { -29, 75, 0, 0x6EE2 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F34 }, { -29, 75, 0, 0x6F3B }, { -29, 75, 0, 0x6EE1 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F33 }, { -29, 75, 0, 0x6F3A }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } },
    { { -29, 75, 0, 0x6F33 }, { -29, 75, 0, 0x6F3A }, { -29, 75, 0, 0x6EE3 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F33 }, { -29, 75, 0, 0x6F3A }, { -29, 75, 0, 0x6EE4 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F33 }, { -29, 75, 0, 0x6F3A }, { -29, 75, 0, 0x6EE5 }, { 0, 0, 0, 0x0003 } },
    { { -29, 75, 0, 0x6F32 }, { -29, 75, 0, 0x6F39 }, { 0, 0, 0, 0x0002 }, { 0, 0, 0, 0x0000 } }
};

const CONN gj_bar[6] = { { -123, 86, 0, 0x6F50 }, { -113, 86, 0, 0x6F50 }, { -103, 86, 0, 0x6F50 },
                         { -93, 86, 0, 0x6F50 },  { -83, 86, 0, 0x6F50 },  { -73, 86, 0, 0x6F6F } };

const CONN gj_f_stage_p[11] = { { -160, 72, 0, 0x6F51 }, { -128, 72, 0, 0x6F51 }, { -96, 72, 0, 0x6F51 },
                                { -64, 72, 0, 0x6F51 },  { -32, 72, 0, 0x6F51 },  { 0, 72, 0, 0x6F51 },
                                { 32, 72, 0, 0x6F51 },   { 64, 72, 0, 0x6F51 },   { 96, 72, 0, 0x6F51 },
                                { 128, 72, 0, 0x6F51 },  { 176, 72, 0, 0x6F5B } };

const CONN gj_f_stage_r[22] = { { -168, 72, 0, 0x6F12 }, { -168, 65, 0, 0x6EFA }, { -136, 72, 0, 0x6F12 },
                                { -136, 65, 0, 0x6EFA }, { -104, 72, 0, 0x6F12 }, { -104, 65, 0, 0x6EFA },
                                { -72, 72, 0, 0x6F12 },  { -72, 65, 0, 0x6EFA },  { -40, 72, 0, 0x6F12 },
                                { -40, 65, 0, 0x6EFA },  { -8, 72, 0, 0x6F12 },   { -8, 65, 0, 0x6EFA },
                                { 24, 72, 0, 0x6F12 },   { 24, 65, 0, 0x6EFA },   { 56, 72, 0, 0x6F12 },
                                { 56, 65, 0, 0x6EFA },   { 88, 72, 0, 0x6F12 },   { 88, 65, 0, 0x6EFA },
                                { 120, 72, 0, 0x6F12 },  { 120, 65, 0, 0x6EFA },  { 168, 72, 0, 0x6F12 },
                                { 168, 65, 0, 0x6EFA } };

const CONN gj_f_mk_spp[10] = { { -168, 48, 0, 0x6F0F }, { -136, 48, 0, 0x6F0F }, { -104, 48, 0, 0x6F0F },
                               { -72, 48, 0, 0x6F0F },  { -40, 48, 0, 0x6F0F },  { -8, 48, 0, 0x6F0F },
                               { 24, 48, 0, 0x6F0F },   { 56, 48, 0, 0x6F0F },   { 88, 48, 0, 0x6F0F },
                               { 168, 48, 0, 0x6F0F } };

const CONN gj_f_mk_spp_Q[11] = { { -168, 48, 0, 0x6F0F }, { -136, 48, 0, 0x6F0F }, { -104, 48, 0, 0x6F0F },
                                 { -72, 48, 0, 0x6F0F },  { -40, 48, 0, 0x6F0F },  { -8, 48, 0, 0x6F0F },
                                 { 24, 48, 0, 0x6F0F },   { 56, 48, 0, 0x6F0F },   { 88, 48, 0, 0x6F0F },
                                 { 120, 48, 0, 0x6F0F },  { 168, 48, 0, 0x6F0F } };

const CONN gj_f_mk_all[6] = { { -102, 32, 0, 0x6EF6 }, { -90, 32, 0, 0x6EF5 }, { -78, 32, 0, 0x6EF5 },
                              { -66, 32, 0, 0x6EF5 },  { -54, 32, 0, 0x6EF5 }, { -36, 32, 0, 0x6EF8 } };

const CONN gj_f_kz_cont[7] = { { -102, 23, 0, 0x6EF5 }, { -90, 23, 0, 0x6EF5 }, { -78, 23, 0, 0x6EF5 },
                               { -66, 23, 0, 0x6EF5 },  { -54, 23, 0, 0x6EF5 }, { -42, 23, 0, 0x6EE9 },
                               { -32, 23, 0, 0x6EE9 } };

const CONN gj_f_kz_spp[8] = { { -110, 14, 0, 0x6EF6 }, { -102, 14, 0, 0x6EF5 }, { -90, 14, 0, 0x6EF5 },
                              { -78, 14, 0, 0x6EF5 },  { -66, 14, 0, 0x6EF5 },  { -54, 14, 0, 0x6EF5 },
                              { -42, 14, 0, 0x6EE9 },  { -32, 14, 0, 0x6EE9 } };

const CONN gj_f_score[10] = { { -32, 5, 0, 0x6EE9 }, { -42, 5, 0, 0x6EE9 },  { -52, 5, 0, 0x6EE9 },
                              { -62, 5, 0, 0x6EE9 }, { -72, 5, 0, 0x6EE9 },  { -82, 5, 0, 0x6EE9 },
                              { -92, 5, 0, 0x6EE9 }, { -102, 5, 0, 0x6EE9 }, { -114, 5, 0, 0x6EF5 },
                              { -126, 5, 0, 0x6EF5 } };

const CONN gj_loser[2] = { { -158, 160, 0, 0x6EC3 }, { -135, 144, 0, 0x6F12 } };

const s16 gj_loser_face[20] = { 0x6EC3, 0x6EC4, 0x6EC5, 0x6EC6, 0x6EC7, 0x6EC8, 0x6EC9, 0x6ECA, 0x6ECB, 0x6ECC,
                                0x6ECD, 0x6ECE, 0x6ECF, 0x6ED0, 0x6ED1, 0x6ED2, 0x6ED3, 0x6ED4, 0x6ED5, 0x6ED6 };

const CONN gj_bar2[6] = { { -123, 164, 0, 0x6F50 }, { -113, 164, 0, 0x6F50 }, { -103, 164, 0, 0x6F50 },
                          { -93, 164, 0, 0x6F50 },  { -83, 164, 0, 0x6F50 },  { -73, 164, 0, 0x6F6F } };

void effect_L1_move(WORK_Other_CONN* ewk) {
    s16 i;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = effL1_base_data[ewk->wu.type][3];
        ewk->wu.dir_timer = effL1_base_data[ewk->wu.type][4];
        ewk->wu.old_cgnum = 0;
        ewk->wu.my_col_code = 0x90;

        if (effL1_base_data[ewk->wu.type][2]) {
            Setup_Color_L1((WORK_Other*)ewk);
        }

        ewk->wu.my_family = effL1_base_data[ewk->wu.type][1];
        ewk->wu.position_z = ewk->wu.my_priority = effL1_base_data[ewk->wu.type][0];
        ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos;
        ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos;
        effL1_item_init[ewk->wu.type](ewk);
        effL1_trans(&ewk->wu);
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[2] != 0) {
            ewk->wu.routine_no[0] = 2;
            ewk->wu.type = 0;
            ewk->wu.disp_flag = 0;
            break;
        }

        switch (ewk->wu.type) {
        case 1:
            if (--ewk->wu.dir_timer < 0) {
                ewk->wu.dir_timer = 0;
                ewk->wu.disp_flag = 1;
            }

            grade_data_disp();
            break;

        case 10:
            if (--ewk->wu.dir_timer < 0) {
                ewk->wu.dir_timer = ewk->wu.dir_step;
                ewk->wu.direction = (ewk->wu.direction + 1) & ewk->wu.dir_old;

                for (i = 0; i < ewk->num_of_conn; i++) {
                    ewk->conn[i].chr = ewk->conn[ewk->num_of_conn + ewk->wu.direction].chr;
                }
            }

            break;
        }

        effL1_trans(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void effL1_trans(WORK* ewk) {
    ewk->cg_number = (ewk->cg_number + 1) & 0x7FFF;

    if (ewk->cg_number == 0) {
        ewk->cg_number = 1;
    }

    sort_push_request3(ewk);
}

void grade_data_disp() {
    // Do nothing
};

void effL1_w_win_init(WORK_Other_CONN* ewk) {
    s16 i;

    effL1_suuchi_bunkai_sub(ewk, WGJ_Win);
    ewk->num_of_conn = 3;

    for (i = 0; i < 3; i++) {
        ewk->conn[i] = gj_wins[i];
        ewk->conn[i].chr += ewk->wu.shell_ix[i];
    }
}

void effL1_w_grade_init(WORK_Other_CONN* ewk) {
    s16 i;

    ewk->wu.direction = grade_get_my_grade((s32)Winner_id);

    for (i = 0; i < 4; i++) {
        ewk->conn[i] = gj_grade[ewk->wu.direction][i];

        if (ewk->conn[i].chr < 0x6ED7) {
            break;
        }

        effect_M3_init(ewk, i);
    }

    ewk->num_of_conn = ewk->conn[i].chr;
    ewk->wu.position_x -= 384;
    ewk->prio_reverse = 1;
}

void effL1_k_grade_init(WORK_Other_CONN* ewk) {
    s16 i;

    if (kakushi_op) {
        ewk->wu.direction = judge_item[kakushi_ix][1].grade;
    } else {
        ewk->wu.direction = judge_com[kakushi_ix].grade;
    }

    ewk->num_of_conn = 2;

    for (i = 0; i < 2; i++) {
        ewk->conn[i] = gj_loser[i];
    }

    ewk->conn[0].chr = gj_loser_face[My_char[kakushi_ix]];
    ewk->conn[1].chr += ewk->wu.direction;
    ewk->wu.position_x -= 384;
}

void effL1_w_score_init(WORK_Other_CONN* ewk) {
    s16 i;

    effL1_suuchi_bunkai_sub(ewk, WGJ_Score);
    ewk->num_of_conn = 8;

    for (i = 0; i < 8; i++) {
        ewk->conn[i] = gj_score[i];
        ewk->conn[i].chr += ewk->wu.shell_ix[i];
    }

    ewk->wu.position_x -= 384;
}

void effL1_w_graph_init(WORK_Other_CONN* ewk) {
    s16 i;

    ewk->wu.direction = grade_get_my_point_percentage((s32)Winner_id, (s16)(ewk->wu.type - 3));

    if (ewk->wu.direction) {
        ewk->wu.direction /= 2;

        if (ewk->wu.direction == 0) {
            ewk->wu.direction = 1;
        }
    }

    ewk->wu.dir_step = ewk->wu.direction % 10;
    ewk->wu.direction /= 10;

    for (i = 0; i < 6; i++) {
        ewk->conn[i] = gj_bar[i];
        ewk->conn[i].ny -= (ewk->wu.type - 3) * 4;
    }

    ewk->num_of_conn = ewk->wu.direction;

    if (ewk->wu.dir_step) {
        ewk->conn[ewk->num_of_conn].chr = (ewk->conn[ewk->num_of_conn].chr - 10) + ewk->wu.dir_step;
        ewk->num_of_conn++;
    }

    ewk->wu.position_x -= 384;
}

void effL1_k_graph_init(WORK_Other_CONN* ewk) {
    s16 i;

    if (kakushi_op) {
        ewk->wu.direction = grade_get_my_point_percentage((s32)kakushi_ix, (s16)(ewk->wu.type - 16));
    } else {
        ewk->wu.direction = grade_get_cm_point_percentage((s32)kakushi_ix, (s16)(ewk->wu.type - 16));
    }

    if (ewk->wu.direction) {
        ewk->wu.direction /= 2;

        if (ewk->wu.direction == 0) {
            ewk->wu.direction = 1;
        }
    }

    ewk->wu.dir_step = ewk->wu.direction % 10;
    ewk->wu.direction /= 10;

    for (i = 0; i < 6; i++) {
        ewk->conn[i] = gj_bar2[i];
        ewk->conn[i].ny -= (ewk->wu.type - 16) * 3;
    }

    ewk->num_of_conn = ewk->wu.direction;

    if (ewk->wu.dir_step) {
        ewk->conn[ewk->num_of_conn].chr = (ewk->conn[ewk->num_of_conn].chr - 10) + ewk->wu.dir_step;
        ewk->num_of_conn++;
    }

    ewk->wu.position_x -= 384;
}

void effL1_f_stage_p_init(WORK_Other_CONN* ewk) {
    s16 i;

    for (i = 0; i < 10; i++) {
        ewk->conn[i] = gj_f_stage_p[i];
        ewk->conn[i].chr += judge_final[WGJ_Target][Play_Type].fr_sort_data[i][0];
    }

    ewk->conn[i] = gj_f_stage_p[i];

    if (judge_final[WGJ_Target][Play_Type].vs_cpu_result[15] != -1) {
        ewk->num_of_conn = 11;
    } else {
        ewk->num_of_conn = 10;
        ewk->conn[9] = ewk->conn[10];
    }
}

void effL1_f_stage_r_init(WORK_Other_CONN* ewk) {
    s16 i;

    for (i = 0; i < 22; i++) {
        ewk->conn[i] = gj_f_stage_r[i];
        ewk->conn[i].chr += judge_final[WGJ_Target][Play_Type].fr_sort_data[i / 2][(i & 1) + 1];
    }

    ewk->num_of_conn = judge_final[WGJ_Target][Play_Type].fr_ix * 2;

    if (judge_final[WGJ_Target][Play_Type].vs_cpu_result[15] == -1) {
        ewk->conn[18].nx = ewk->conn[20].nx;
        ewk->conn[18].ny = ewk->conn[20].ny;
        ewk->conn[19].nx = ewk->conn[21].nx;
        ewk->conn[19].ny = ewk->conn[21].ny;
    }
}

void effL1_f_grade_init(WORK_Other_CONN* ewk) {
    s16 i;

    ewk->wu.direction = judge_final[WGJ_Target]->grade;

    for (i = 0; i < 4; i++) {
        ewk->conn[i] = gj_grade[ewk->wu.direction][i];
        ewk->conn[i].nx += 184;
        ewk->conn[i].ny -= 64;

        if (ewk->conn[i].chr < 0x6ED7) {
            break;
        }
    }

    ewk->num_of_conn = ewk->conn[i].chr;
    ewk->prio_reverse = 1;
}

void effL1_f_mk_spp_init(WORK_Other_CONN* ewk) {
    s16 i;
    s16 k = 0;

    if (judge_final[WGJ_Target][Play_Type].vs_cpu_result[15] == -1) {
        for (i = 0; i < 10; i++) {
            if (judge_final[WGJ_Target][Play_Type].fr_sort_data[i][3]) {
                ewk->conn[k] = gj_f_mk_spp[i];
                k++;
            }
        }
    } else {
        for (i = 0; i < 11; i++) {
            if (judge_final[WGJ_Target][Play_Type].fr_sort_data[i][3]) {
                ewk->conn[k] = gj_f_mk_spp_Q[i];
                k++;
            }
        }
    }

    ewk->num_of_conn = k;
    ewk->wu.dir_old = 3;
    ewk->wu.dir_step = 8;
    ewk->wu.direction = 0;
    ewk->conn[k].chr = 0x6F0F;
    ewk->conn[k + 1].chr = 0x6F10;
    ewk->conn[k + 2].chr = 0x6F11;
    ewk->conn[k + 3].chr = 0x6F10;
}

void effL1_f_mk_all_init(WORK_Other_CONN* ewk) {
    s16 i;

    ewk->num_of_conn = 6;

    for (i = 0; i < 6; i++) {
        ewk->conn[i] = gj_f_mk_all[i];
    }

    if (judge_final[WGJ_Target][Play_Type].all_clear) {
        ewk->conn[4].nx -= 4;
        ewk->conn[5].nx -= 6;
        ewk->conn[5].chr++;
    }
}

void effL1_f_kz_cont_init(WORK_Other_CONN* ewk) {
    s16 i;

    effL1_suuchi_bunkai_sub(ewk, judge_final[WGJ_Target][Play_Type].keizoku);
    ewk->num_of_conn = 7;

    for (i = 0; i < 7; i++) {
        ewk->conn[i] = gj_f_kz_cont[i];
    }

    ewk->conn[6].chr += ewk->wu.shell_ix[0];
    ewk->conn[5].chr += ewk->wu.shell_ix[1];
}

void effL1_f_kz_spp_init(WORK_Other_CONN* ewk) {
    s16 i;

    effL1_suuchi_bunkai_sub(ewk, judge_final[WGJ_Target][Play_Type].sp_point);
    ewk->num_of_conn = 8;

    for (i = 0; i < 8; i++) {
        ewk->conn[i] = gj_f_kz_spp[i];
    }

    ewk->conn[7].chr += ewk->wu.shell_ix[0];
    ewk->conn[6].chr += ewk->wu.shell_ix[1];
}

void effL1_f_score_init(WORK_Other_CONN* ewk) {
    s16 i;

    effL1_suuchi_bunkai_sub(ewk, WGJ_Score);
    ewk->num_of_conn = 10;

    for (i = 0; i < 10; i++) {
        ewk->conn[i] = gj_f_score[i];
    }

    for (i = 0; i < 8; i++) {
        ewk->conn[i].chr += ewk->wu.shell_ix[i];
    }
}

void effL1_suuchi_bunkai_sub(WORK_Other_CONN* ewk, u32 tsc) {
    s16 i;

    for (i = 7; i > 0; i--) {
        ewk->wu.shell_ix[i] = tsc / bunkai_table_l1[i];
        tsc %= bunkai_table_l1[i];
    }

    ewk->wu.shell_ix[i] = tsc;
}

s32 effect_L1_init(s16 flag) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 211;
    ewk->wu.type = flag;
    ewk->wu.my_mts = 13;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    return 0;
}
