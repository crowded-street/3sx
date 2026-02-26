/**
 * @file grade.c
 * Grading logic
 */

#include "sf33rd/Source/Game/engine/grade.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"

#include <SDL3/SDL.h>

// sbss
JudgeGals judge_gals[2];
JudgeCom judge_com[2];
s16 last_judge_dada[2][5];

// bss
GradeData judge_item[2][2];
GradeFinalData judge_final[2][2];
u8 ji_sat[2][384];

const s16 ji_grd_init_data[16] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1 };

const s16 grade_t_meichuuritsu2[24][2] = {
    { 0, 35 },   { 6, 50 },   { 11, 65 },  { 16, 80 },  { 21, 95 },  { 26, 110 }, { 31, 125 }, { 36, 140 },
    { 41, 155 }, { 46, 170 }, { 51, 185 }, { 56, 200 }, { 61, 210 }, { 66, 220 }, { 71, 230 }, { 76, 240 },
    { 81, 250 }, { 86, 260 }, { 91, 270 }, { 93, 280 }, { 95, 300 }, { 97, 320 }, { 99, 340 }, { 100, 360 }
};

const s16 grade_t_meichuuritsu3[21][2] = { { 0, 2 },   { 5, 10 },  { 10, 18 }, { 15, 22 }, { 20, 26 }, { 25, 28 },
                                           { 30, 30 }, { 35, 31 }, { 40, 32 }, { 45, 33 }, { 50, 34 }, { 55, 36 },
                                           { 60, 38 }, { 65, 40 }, { 70, 42 }, { 75, 44 }, { 80, 48 }, { 85, 52 },
                                           { 90, 56 }, { 95, 60 }, { 100, 64 } };

const s16 grade_t_em_stun[5][2] = { { 0, 0 }, { 1, 100 }, { 2, 130 }, { 3, 145 }, { 4, 150 } };

const s16 grade_t_max_combo[19][2] = { { 0, 0 },    { 2, 10 },   { 3, 30 },   { 4, 60 },   { 5, 80 },
                                       { 6, 100 },  { 7, 110 },  { 8, 120 },  { 9, 130 },  { 10, 140 },
                                       { 12, 150 }, { 14, 160 }, { 16, 170 }, { 18, 175 }, { 19, 180 },
                                       { 20, 185 }, { 24, 190 }, { 28, 195 }, { 32, 200 } };

const s16 grade_t_bougyoritsu2[14][2] = { { 0, 10 },   { 10, 20 },  { 20, 40 },  { 30, 60 },  { 40, 80 },
                                          { 50, 100 }, { 60, 120 }, { 70, 140 }, { 80, 160 }, { 85, 180 },
                                          { 90, 200 }, { 95, 220 }, { 98, 230 }, { 100, 250 } };

const s16 grade_t_bougyoritsu3[13][2] = { { 0, 10 },   { 10, 20 },  { 20, 30 },  { 30, 40 }, { 40, 50 },
                                          { 50, 60 },  { 60, 70 },  { 70, 80 },  { 80, 90 }, { 85, 100 },
                                          { 90, 110 }, { 95, 130 }, { 100, 150 } };

const s16 grade_t_nokori_vital[13][2] = { { 0, 60 },   { 1, 0 },     { 16, 10 },  { 32, 20 },  { 48, 30 },
                                          { 64, 40 },  { 80, 50 },   { 96, 60 },  { 112, 70 }, { 128, 80 },
                                          { 144, 90 }, { 152, 120 }, { 160, 160 } };

const s16 grade_t_def_nmlblock[11][2] = { { 0, 0 },   { 1, 80 },  { 2, 90 },  { 3, 100 }, { 4, 110 }, { 5, 120 },
                                          { 6, 130 }, { 7, 135 }, { 8, 140 }, { 9, 145 }, { 10, 150 } };

const s16 grade_t_def_rpdblock[11][2] = { { 0, 0 },   { 1, 60 },  { 2, 70 },  { 3, 80 },  { 4, 90 },  { 5, 100 },
                                          { 6, 110 }, { 7, 120 }, { 8, 130 }, { 9, 140 }, { 10, 150 } };

const s16 grade_t_def_grdblock[9][2] = { { 0, 0 },   { 1, 100 }, { 2, 110 }, { 3, 120 }, { 4, 130 },
                                         { 5, 135 }, { 6, 140 }, { 7, 145 }, { 8, 150 } };

const s16 grade_t_first_attack[2] = { 0, 100 };

const s16 grade_t_leap_attack[10][2] = { { 0, 0 },  { 1, 50 }, { 2, 60 },  { 3, 65 },  { 4, 70 },
                                         { 8, 60 }, { 9, 40 }, { 10, 20 }, { 11, 10 }, { 12, 0 } };

const s16 grade_t_target_combo[8][2] = { { 0, 0 },  { 1, 40 },   { 2, 50 },   { 4, 60 },
                                         { 8, 80 }, { 12, 100 }, { 16, 120 }, { 24, 140 } };

const s16 grade_t_nml_nage[10][2] = { { 0, 0 },  { 1, 50 }, { 2, 60 }, { 3, 70 },  { 4, 75 },
                                      { 5, 80 }, { 6, 60 }, { 8, 40 }, { 10, 20 }, { 12, 0 } };

const s16 grade_t_grap_def[6][2] = { { 0, 0 }, { 1, 60 }, { 2, 80 }, { 4, 100 }, { 8, 120 }, { 16, 140 } };

const s16 grade_t_quick_stand[4][2] = { { 0, 0 }, { 1, 40 }, { 3, 50 }, { 6, 60 } };

const s16 grade_t_personal_act[4][2] = { { 0, 0 }, { 1, 50 }, { 2, 53 }, { 3, 55 } };

const s16 grade_t_reversal[8][2] = { { 0, 0 },  { 1, 50 }, { 2, 60 }, { 3, 70 },
                                     { 4, 80 }, { 6, 90 }, { 8, 95 }, { 10, 100 } };

const s16 grade_t_command_waza[9][2] = { { 0, 0 },   { 1, 30 },  { 3, 40 },  { 6, 50 },  { 10, 60 },
                                         { 15, 70 }, { 21, 80 }, { 28, 90 }, { 36, 100 } };

const s16 grade_t_sa_stock_3[6][2] = { { 0, 0 }, { 1, 50 }, { 2, 60 }, { 3, 70 }, { 4, 80 }, { 5, 90 } };

const s16 grade_t_sa_stock_2[6][2] = { { 0, 0 }, { 1, 55 }, { 2, 65 }, { 3, 75 }, { 4, 85 }, { 5, 95 } };

const s16 grade_t_sa_stock_1[6][2] = { { 0, 0 }, { 1, 60 }, { 2, 70 }, { 3, 80 }, { 4, 90 }, { 5, 100 } };

const s16 grade_t_tairyokusa[21][2] = { { 0, 0 },     { 9, 10 },    { 17, 20 },   { 25, 30 },   { 33, 40 },
                                        { 41, 50 },   { 49, 60 },   { 57, 80 },   { 65, 100 },  { 73, 120 },
                                        { 81, 140 },  { 89, 160 },  { 97, 180 },  { 105, 200 }, { 113, 220 },
                                        { 121, 240 }, { 129, 260 }, { 137, 270 }, { 145, 280 }, { 153, 290 },
                                        { 160, 300 } };

const s16 grade_t_app_nmlblock[7][2] = { { 0, 120 }, { 1, 110 }, { 9, 100 }, { 17, 80 },
                                         { 33, 50 }, { 49, 30 }, { 81, 15 } };

const s16 grade_t_app_rpdblock[7][2] = {
    { 0, 90 }, { 1, 80 }, { 9, 70 }, { 17, 50 }, { 33, 30 }, { 49, 20 }, { 81, 10 }
};

const s16 grade_t_app_grdblock[7][2] = { { 0, 150 }, { 1, 140 }, { 9, 120 }, { 17, 100 },
                                         { 33, 70 }, { 49, 40 }, { 81, 20 } };

const s16 grade_t_renshou[8][2] = { { 0, 0 },    { 2, 20 },   { 5, 50 },   { 8, 80 },
                                    { 11, 100 }, { 16, 120 }, { 21, 160 }, { 31, 200 } };

const s16 grade_t_em_renshou[8][2] = { { 0, 0 },   { 2, 20 },   { 4, 50 },   { 6, 80 },
                                       { 9, 100 }, { 12, 120 }, { 16, 160 }, { 20, 200 } };

const s16 grade_t_straight[11][2] = { { 0, 0 },   { 2, 40 },  { 3, 60 },  { 4, 80 },   { 5, 100 }, { 6, 120 },
                                      { 7, 140 }, { 8, 160 }, { 9, 180 }, { 10, 190 }, { 11, 200 } };

const s16 grade_t_round_result[4] = { 0, 50, 100, 160 };

const s16 grade_t_onaji_waza[6][2] = { { 0, 100 }, { 6, 70 }, { 12, 50 }, { 18, 30 }, { 24, 10 }, { 30, 0 } };

const s16 grade_t_f_stage[4][2] = { { 0, -40 }, { 4, -20 }, { 8, 0 }, { 10, 60 } };

const s16 grade_t_f_all[2] = { 0, 100 };

const s16 grade_t_f_continue[11][2] = { { 0, 100 },  { 1, -100 }, { 2, -150 }, { 3, -180 }, { 4, -200 }, { 5, -210 },
                                        { 6, -220 }, { 7, -230 }, { 8, -240 }, { 9, -250 }, { 10, -260 } };

const s16 grade_t_f_gradeup[11][2] = { { 0, 0 },   { 1, 40 },  { 2, 55 },  { 3, 70 },  { 4, 85 },  { 5, 100 },
                                       { 6, 120 }, { 7, 140 }, { 8, 160 }, { 9, 180 }, { 10, 200 } };

const s16 grade_t_f_bss_car[4][2] = { { 0, 0 }, { 1, 80 }, { 2, 40 }, { 3, 100 } };

const s16 grade_t_f_bss_ball[4][2] = { { 0, 0 }, { 1, 100 }, { 2, 80 }, { 3, 160 } };

const s16 grade_t_table[32][2] = { { 0, 20 },       { 75, 20 },     { 150, 20 },    { 225, 40 },    { 300, 40 },
                                   { 375, 40 },     { 450, 60 },    { 525, 60 },    { 600, 60 },    { 675, 80 },
                                   { 750, 80 },     { 825, 80 },    { 900, 100 },   { 975, 100 },   { 1050, 100 },
                                   { 1125, 150 },   { 1200, 200 },  { 1275, 300 },  { 1350, 400 },  { 1425, 600 },
                                   { 1500, 800 },   { 1550, 1000 }, { 1600, 1300 }, { 1650, 1600 }, { 1700, 2000 },
                                   { 1750, 3000 },  { 1800, 4000 }, { 1850, 5000 }, { 1900, 6000 }, { 1940, 8000 },
                                   { 1980, 10000 }, { 2020, 20000 } };

void grade_check_work_1st_init(s16 ix, s16 ix2) {
    s16 i;

    SDL_zero(judge_item[ix][ix2]);
    SDL_zero(judge_final[ix][ix2]);

    for (i = 0; i < 16; i++) {
        judge_final[ix][ix2].vs_cpu_result[i] = -1;
        judge_final[ix][ix2].vs_cpu_player[i] = -1;
        judge_final[ix][ix2].vs_cpu_grade[i] = -1;
    }
}

void grade_check_work_stage_init(s16 ix) {
    judge_item[ix][Play_Type].offence_total = 0;
    judge_item[ix][Play_Type].defence_total = 0;
    judge_item[ix][Play_Type].tech_pts_total = 0;
    judge_item[ix][Play_Type].ex_point_total = 0;
    judge_item[ix][Play_Type].round = 0;
    judge_item[ix][Play_Type].win_round = 0;

    if (Play_Type == 1) {
        judge_item[ix][Play_Type].renshou = Win_Record[ix];
        judge_item[ix][Play_Type].em_renshou = Win_Record[(ix + 1) & 1];
        return;
    }

    judge_item[ix][Play_Type].renshou = 0;
    judge_item[ix][Play_Type].em_renshou = 0;
}

void grade_check_work_round_init(s16 ix) {
    s16 i;

    judge_item[ix][Play_Type].em_stun = 0;
    judge_item[ix][Play_Type].max_combo = 0;
    judge_item[ix][Play_Type].clean_hits = 0;
    judge_item[ix][Play_Type].att_renew = 0;
    judge_item[ix][Play_Type].guard_succ = 0;
    judge_item[ix][Play_Type].vitality = 0;
    judge_item[ix][Play_Type].nml_blocking = 0;
    judge_item[ix][Play_Type].rpd_blocking = 0;
    judge_item[ix][Play_Type].grd_blocking = 0;
    judge_item[ix][Play_Type].first_attack = 0;
    judge_item[ix][Play_Type].leap_attack = 0;
    judge_item[ix][Play_Type].target_combo = 0;
    judge_item[ix][Play_Type].nml_nage = 0;
    judge_item[ix][Play_Type].grap_def = 0;
    judge_item[ix][Play_Type].quick_stand = 0;
    judge_item[ix][Play_Type].personal_act = 0;
    judge_item[ix][Play_Type].reversal = 0;
    judge_item[ix][Play_Type].comwaza = 0;
    judge_item[ix][Play_Type].sa_exec = 0;
    judge_item[ix][Play_Type].tairyokusa = 0;
    judge_item[ix][Play_Type].kimarite = 0;
    judge_item[ix][Play_Type].app_nml_block = -1;
    judge_item[ix][Play_Type].app_rpd_block = -1;
    judge_item[ix][Play_Type].app_grd_block = -1;
    judge_item[ix][Play_Type].onaji_waza = 0;

    if (Round_Operator[ix] == 0) {
        judge_item[ix][Play_Type].grd_miss = ji_grd_init_data[Setup_Lv10(0)];
        judge_item[ix][Play_Type].grd_mcnt = ji_grd_init_data[Setup_Lv10(0)];
    } else {
        judge_item[ix][Play_Type].grd_miss = 0;
        judge_item[ix][Play_Type].grd_mcnt = 0;
    }

    for (i = 0; i < 384; i++) {
        ji_sat[ix][i] = 0;
    }

    judge_gals[ix].grade = 0;
    judge_gals[ix].offence_total = 0;
    judge_gals[ix].defence_total = 0;
    judge_gals[ix].tech_pts_total = 0;
    judge_gals[ix].ex_point_total = 0;

    for (i = 0; i < 5; i++) {
        last_judge_dada[0][i] = last_judge_dada[1][i] = 0;
    }
}

void grade_makeup_final_parameter(s16 ix, s16 pt) {
    renew_judge_final_work(ix, pt);
    judge_final[ix][pt].all_clear = (VS_Index[WINNER] < 10) ^ 1;
    judge_final[ix][pt].keizoku = Continue_Coin[ix];
    makeup_spp_frdat(ix, pt);
    makeup_final_grade(ix, pt);
}

void renew_judge_final_work(s16 ix, s16 pt) {
    u32* frsd;
    s16 i;

    judge_final[ix][pt].all_clear = 0;
    judge_final[ix][pt].keizoku = 0;
    judge_final[ix][pt].sp_point = 0;
    judge_final[ix][pt].fr_ix = 0;
    frsd = (u32*)judge_final[ix][pt].fr_sort_data;

    for (i = 0; i < 16; i++) {
        *frsd++ = 0;
    }
}

void makeup_final_grade(s16 ix, s16 pt) {
    s16 i;
    s16 tt = 0;
    s16 dt;

    if ((dt = judge_final[ix][pt].vcr_ix) == 0) {
        dt = 1;
    }

    for (i = 0; i < judge_final[ix][pt].vcr_ix; i++) {
        tt += judge_final[ix][pt].vs_cpu_result[i];
    }

    if (judge_final[ix][pt].vs_cpu_result[15] != -1) {
        tt += judge_final[ix][pt].vs_cpu_result[15];
        dt += 1;
    }

    if (dt) {
        judge_final[ix][pt].vs_cpu_result[11] = tt / dt;
    } else {
        judge_final[ix][pt].vs_cpu_result[11] = 0;
    }

    judge_final[ix][pt].vs_cpu_grade[11] = get_grade_ix(judge_final[ix][pt].vs_cpu_result[11]);

    if (judge_final[ix][pt].vs_cpu_result[15] != -1) {
        tt /= 11;
    } else {
        tt /= 10;
    }

    for (i = 0; i < 3; i++) {
        if (judge_final[ix][pt].vcr_ix < grade_t_f_stage[i + 1][0]) {
            break;
        }
    }

    tt += grade_t_f_stage[i][1];

    if (judge_final[ix][pt].all_clear) {
        tt += grade_t_f_all[judge_final[ix][pt].all_clear];

        for (i = 0; i < 10; i++) {
            if (judge_final[ix][pt].keizoku < grade_t_f_continue[i + 1][0]) {
                break;
            }
        }

        tt += grade_t_f_continue[i][1];

        for (i = 0; i < 10; i++) {
            if (judge_final[ix][pt].sp_point < grade_t_f_gradeup[i + 1][0]) {
                break;
            }
        }

        tt += grade_t_f_gradeup[i][1];

        if (judge_final[ix][pt].vs_cpu_grade[13] != -1) {
            for (i = 0; i < 3; i++) {
                if (judge_final[ix][pt].vs_cpu_grade[13] < grade_t_f_bss_ball[i + 1][0]) {
                    break;
                }
            }

            tt += grade_t_f_bss_ball[i][1];
        }

        if (judge_final[ix][pt].vs_cpu_grade[14] != -1) {
            for (i = 0; i < 3; i++) {
                if (judge_final[ix][pt].vs_cpu_grade[14] < grade_t_f_bss_car[i + 1][0]) {
                    break;
                }
            }

            tt += grade_t_f_bss_car[i][1];
        }
    }

    if (tt < 0) {
        tt = 0;
    }

    judge_final[ix][pt].vs_cpu_result[12] = tt;
    judge_final[ix][pt].vs_cpu_grade[12] = judge_final[ix][pt].grade = get_grade_ix(tt);
}

void grade_final_grade_bonus() {
    u32 bonus_point = grade_t_table[judge_final[WGJ_Target][Final_Play_Type[WGJ_Target]].grade][1];
    bonus_point *= 0x64;
    Score[WGJ_Target][Final_Play_Type[WGJ_Target]] += bonus_point;
}

void makeup_spp_frdat(s16 ix, s16 pt) {
    s16 i;
    s16 j;
    s16 k;
    u8* dmw;

    dmw = *judge_final[ix][pt].fr_sort_data;

    for (j = 0, i = 0; i < judge_final[ix][pt].vcr_ix; j++, i++) {
        if (i == judge_final[ix][pt].vs_cpu_player[0xF]) {
            *(dmw + (j * 4)) = 9;
            *(dmw + ((((j)) * 4) + 1)) = judge_final[ix][pt].vs_cpu_grade[0xF];
            *(dmw + ((((j)) * 4) + 2)) = 0x12;
            j++;
        }
        *(dmw + (((j)) * 4)) = i;
        *(dmw + ((((j)) * 4) + 1)) = judge_final[ix][pt].vs_cpu_grade[(i)];
        *(dmw + ((((j)) * 4) + 2)) = judge_final[ix][pt].vs_cpu_player[(i)];
    }

    judge_final[ix][pt].fr_ix = j;

    for (k = i; k < 0xA; j++, k++) {
        *(dmw + (((j)) * 4)) = k;
    }

    judge_final[ix][pt].sp_point = 0;

    for (i = 1; i < judge_final[ix][pt].fr_ix; i++) {
        if ((*(dmw + (((((i))) * 4) + 1))) > (*(dmw + ((((i)-1) * 4) + 1)))) {
            judge_final[ix][pt].sp_point += 1;
            *(dmw + ((((i)) * 4) + 3)) = 1;
        }
    }
}

void grade_makeup_round_parameter(s16 ix) {
    s16 ix2 = (ix + 1) & 1;

    judge_item[ix][Play_Type].offence_total += get_offence_total(ix);
    judge_item[ix2][Play_Type].offence_total += get_offence_total(ix2);
    judge_item[ix][Play_Type].defence_total += get_defence_total(ix, 1);
    judge_item[ix2][Play_Type].defence_total += get_defence_total(ix2, 0);
    judge_item[ix][Play_Type].tech_pts_total += get_tech_pts_total(ix);
    judge_item[ix2][Play_Type].tech_pts_total += get_tech_pts_total(ix2);
    judge_item[ix][Play_Type].ex_point_total += get_ex_point_total(ix, 1);
    judge_item[ix2][Play_Type].ex_point_total += get_ex_point_total(ix2, 0);
    judge_item[ix][Play_Type].round++;
    judge_item[ix2][Play_Type].round++;
    judge_item[ix][Play_Type].win_round++;

    backup_RO_PT();
}

void backup_RO_PT() {
    RO_backup[0] = Round_Operator[0];
    RO_backup[1] = Round_Operator[1];
    PT_backup = Play_Type;
}

void grade_makeup_round_para_dko() {
    s16 i;

    for (i = 0; i < 2; i++) {
        judge_item[i][Play_Type].offence_total += get_offence_total(i);
        judge_item[i][Play_Type].defence_total += get_defence_total(i, 0);
        judge_item[i][Play_Type].tech_pts_total += get_tech_pts_total(i);
        judge_item[i][Play_Type].ex_point_total += get_ex_point_total(i, 0);
        judge_item[i][Play_Type].round += 1;
    }

    backup_RO_PT();
}

void grade_makeup_judgement_gals() {
    s16 i;

    for (i = 0; i < 2; i++) {
        judge_gals[i].offence_total = get_offence_total(i);
        judge_gals[i].defence_total = get_defence_total(i, 0);
        judge_gals[i].tech_pts_total = get_tech_pts_total(i);
        judge_gals[i].ex_point_total = get_ex_point_total(i, 0);
        judge_gals[i].grade = get_grade_ix(judge_gals[i].offence_total + judge_gals[i].defence_total +
                                           judge_gals[i].tech_pts_total + judge_gals[i].ex_point_total);
    }
}

void grade_makeup_stage_parameter(s16 ix) {
    s16 i;
    s16 grade;
    s16 plnum;
    s16 point = 0;
    s16 bs;
    s16 qc;

    if ((Mode_Type != MODE_REPLAY) && (Mode_Type != MODE_VERSUS) && (Round_Operator[ix] == 0)) {
        grade_makeup_stage_para_com(ix);
        return;
    }

    qc = bs = 0;

    if (judge_item[ix][Play_Type].round == 0) {
        judge_item[ix][Play_Type].round = 1;
    }

    judge_item[ix][Play_Type].offence_total /= judge_item[ix][Play_Type].round;
    judge_item[ix][Play_Type].defence_total /= judge_item[ix][Play_Type].round;
    judge_item[ix][Play_Type].tech_pts_total /= judge_item[ix][Play_Type].round;
    judge_item[ix][Play_Type].ex_point_total /= judge_item[ix][Play_Type].round;
    judge_item[ix][Play_Type].no_lose = 0;

    if (judge_item[ix][Play_Type].round == judge_item[ix][Play_Type].win_round) {
        judge_item[ix][Play_Type].no_lose = Straight_Counter[ix];
    }

    if (ix == WINNER) {
        if (Play_Type == 0) {
            for (i = 0; i < 10; i++) {
                if (judge_item[ix][Play_Type].no_lose < grade_t_straight[i + 1][0]) {
                    break;
                }
            }

            judge_item[ix][Play_Type].ex_point_total += grade_t_straight[i][1];
        } else if (judge_item[ix][Play_Type].renshou) {
            for (i = 0; i < 7; i++) {
                if (judge_item[ix][Play_Type].renshou < grade_t_renshou[i + 1][0]) {
                    break;
                }
            }

            point += grade_t_renshou[i][1];
        } else {
            for (i = 0; i < 7; i++) {
                if (judge_item[ix][Play_Type].em_renshou < grade_t_em_renshou[i + 1][0]) {
                    break;
                }
            }

            point += grade_t_em_renshou[i][1];
        }
    }

    point = judge_item[ix][Play_Type].offence_total + judge_item[ix][Play_Type].defence_total +
            judge_item[ix][Play_Type].tech_pts_total + judge_item[ix][Play_Type].ex_point_total;
    grade = get_grade_ix(point);

    if (Play_Type == 0) {
        switch (bg_w.stage) {
        case 21:
        case 20:
            bs = 1;
            break;

        default:
            if ((qc = rannyuu_Q_check((ix + 1) & 1))) {
                judge_final[ix][Play_Type].vs_cpu_result[15] = point;
                judge_final[ix][Play_Type].vs_cpu_grade[15] = grade;
                judge_final[ix][Play_Type].vs_cpu_player[15] = judge_final[ix][Play_Type].vcr_ix;
            } else {
                plnum = old_my_char_check(My_char[(ix + 1) & 1], 0);
                judge_final[ix][Play_Type].vs_cpu_result[judge_final[ix][Play_Type].vcr_ix] = point;
                judge_final[ix][Play_Type].vs_cpu_grade[judge_final[ix][Play_Type].vcr_ix] = grade;
                judge_final[ix][Play_Type].vs_cpu_player[judge_final[ix][Play_Type].vcr_ix] = plnum;
                judge_final[ix][Play_Type].vcr_ix += 1;
            }

            judge_item[ix][Play_Type].grade = grade;
            break;
        }

        grade_makeup_final_parameter(ix, Play_Type + 0);

        if (ix == WINNER) {
            return;
        }

        if (bs != 0) {
            return;
        }

        if (qc) {
            judge_final[ix][Play_Type].vs_cpu_result[15] = -1;
            judge_final[ix][Play_Type].vs_cpu_grade[15] = -1;
            judge_final[ix][Play_Type].vs_cpu_player[15] = -1;
            return;
        } else {
            judge_final[ix][Play_Type].vcr_ix--;
        }
    } else {
        judge_item[ix][Play_Type].grade = grade;
    }
}

s32 rannyuu_Q_check(s16 ix) {
    if ((Round_Operator[ix] == 0) && (My_char[ix] == 17)) {
        return 1;
    }

    return 0;
}

void grade_makeup_stage_para_com(s16 ix) {
    judge_com[ix].round = judge_item[ix][Play_Type].round;

    if (judge_com[ix].round == 0) {
        judge_com[ix].round = 1;
    }

    judge_com[ix].offence_total = judge_item[ix][Play_Type].offence_total / judge_com[ix].round;
    judge_com[ix].defence_total = judge_item[ix][Play_Type].defence_total / judge_com[ix].round;
    judge_com[ix].tech_pts_total = judge_item[ix][Play_Type].tech_pts_total / judge_com[ix].round;
    judge_com[ix].ex_point_total = judge_item[ix][Play_Type].ex_point_total / judge_com[ix].round;
    judge_com[ix].grade = get_grade_ix(judge_com[ix].offence_total + judge_com[ix].defence_total +
                                       judge_com[ix].tech_pts_total + judge_com[ix].ex_point_total);
}

void grade_makeup_bonus_parameter(s16 ix) {
    if (Round_Operator[ix] != 0) {
        switch (bg_w.stage) {
        case 21:
            judge_final[ix][Play_Type].vs_cpu_grade[13] = (Bonus_Game_result == 20) + (Bonus_Game_ex_result == 20) * 2;
            break;

        case 20:
            judge_final[ix][Play_Type].vs_cpu_grade[14] = Bonus_Game_result;
            break;
        }
    }
}

s16 get_offence_total(s16 ix) {
    s32 num;
    s32 num2;
    s32 point;
    s32 point2;
    s16 i;
    s16 ix2;

    ix2 = (ix + 1) & 1;
    point = point2 = 0;
    num2 = judge_item[ix2][Play_Type].guard_succ + judge_item[ix2][Play_Type].nml_blocking +
           judge_item[ix2][Play_Type].rpd_blocking + judge_item[ix2][Play_Type].grd_blocking +
           judge_item[ix][Play_Type].clean_hits;

    if (num2 != 0) {
        num = (judge_item[ix][Play_Type].clean_hits * 100) / num2;
    } else {
        num = 0;
    }

    last_judge_dada[ix][0] = remake_2_10(num, 3);

    for (i = 0; i < 23; i++) {
        if (num < grade_t_meichuuritsu2[i + 1][0]) {
            break;
        }
    }

    point2 = grade_t_meichuuritsu2[i][1];

    if (judge_item[ix][Play_Type].att_renew) {
        num = (num2 * 100) / judge_item[ix][Play_Type].att_renew;
    } else {
        num = 0;
    }

    last_judge_dada[ix][1] = remake_2_10(num, 3);
    point2 = (point2 * num) / 100;

    if (judge_item[ix2][Play_Type].grd_mcnt) {
        num = (judge_item[ix2][Play_Type].grd_miss * 100) / judge_item[ix2][Play_Type].grd_mcnt;
    } else {
        num = 0;
    }

    last_judge_dada[ix][2] = remake_2_10(num, 3);

    for (i = 0; i < 20; i++) {
        if (num < grade_t_meichuuritsu3[i + 1][0]) {
            break;
        }
    }

    point2 *= grade_t_meichuuritsu3[i][1];
    point2 /= 32;

    point = point2;

    for (i = 0; i < 4; i++) {
        if (judge_item[ix][Play_Type].em_stun < grade_t_em_stun[i + 1][0]) {
            break;
        }
    }

    point += grade_t_em_stun[i][1];

    for (i = 0; i < 18; i++) {
        if (judge_item[ix][Play_Type].max_combo < grade_t_max_combo[i + 1][0]) {
            break;
        }
    }

    point += grade_t_max_combo[i][1];

    return point;
}

s16 get_defence_total(s16 ix, s16 wf) {
    s32 num = 0;
    s16 i;
    s16 ix2;
    s32 point;
    s32 point2;

    ix2 = (ix + 1) & 1;

    if (judge_item[ix2][Play_Type].att_renew) {
        point2 = ((judge_item[ix2][Play_Type].att_renew - judge_item[ix2][Play_Type].clean_hits) * 100) /
                 judge_item[ix2][Play_Type].att_renew;
    } else {
        point2 = 0;
    }

    last_judge_dada[ix][3] = remake_2_10(point2, 3);

    for (i = 0; i < 13; i++) {
        if (point2 < grade_t_bougyoritsu2[i + 1][0]) {
            break;
        }
    }

    num += grade_t_bougyoritsu2[i][1];
    point2 = judge_item[ix][Play_Type].clean_hits + judge_item[ix2][Play_Type].guard_succ;

    if (judge_item[ix][Play_Type].att_renew) {
        point2 = (point2 * 100) / judge_item[ix][Play_Type].att_renew;
    } else {
        point2 = 0;
    }

    last_judge_dada[ix][4] = remake_2_10(point2, 3);

    for (i = 0; i < 12; i++) {
        if (point2 < grade_t_bougyoritsu3[i + 1][0]) {
            break;
        }
    }

    point = grade_t_bougyoritsu3[i][1];

    if (judge_item[ix2][Play_Type].att_renew == 0) {
        point = (point * 200) / 100;
    }

    num += point;

    if (wf) {
        for (i = 0; i < 12; i++) {
            if (judge_item[ix][Play_Type].vitality < grade_t_nokori_vital[i + 1][0]) {
                break;
            }
        }

        num += grade_t_nokori_vital[i][1];
    }

    for (i = 0; i < 10; i++) {
        if (judge_item[ix][Play_Type].nml_blocking < grade_t_def_nmlblock[i + 1][0]) {
            break;
        }
    }

    num += grade_t_def_nmlblock[i][1];

    for (i = 0; i < 10; i++) {
        if (judge_item[ix][Play_Type].rpd_blocking < grade_t_def_rpdblock[i + 1][0]) {
            break;
        }
    }

    num += grade_t_def_rpdblock[i][1];

    for (i = 0; i < 8; i++) {
        if (judge_item[ix][Play_Type].grd_blocking < grade_t_def_grdblock[i + 1][0]) {
            break;
        }
    }

    num += grade_t_def_grdblock[i][1];

    return num;
}

s16 get_tech_pts_total(s16 ix) {
    s16 i;
    s16 point = 0;

    point += grade_t_first_attack[judge_item[ix][Play_Type].first_attack];

    for (i = 0; i < 9; i++) {
        if (judge_item[ix][Play_Type].leap_attack < grade_t_leap_attack[i + 1][0]) {
            break;
        }
    }

    point += grade_t_leap_attack[i][1];

    for (i = 0; i < 7; i++) {
        if (judge_item[ix][Play_Type].target_combo < grade_t_target_combo[i + 1][0]) {
            break;
        }
    }

    point += grade_t_target_combo[i][1];

    for (i = 0; i < 9; i++) {
        if (judge_item[ix][Play_Type].nml_nage < grade_t_nml_nage[i + 1][0]) {
            break;
        }
    }

    point += grade_t_nml_nage[i][1];

    for (i = 0; i < 5; i++) {
        if (judge_item[ix][Play_Type].grap_def < grade_t_grap_def[i + 1][0]) {
            break;
        }
    }

    point += grade_t_grap_def[i][1];

    for (i = 0; i < 3; i++) {
        if (judge_item[ix][Play_Type].quick_stand < grade_t_quick_stand[i + 1][0]) {
            break;
        }
    }

    point += grade_t_quick_stand[i][1];

    for (i = 0; i < 3; i++) {
        if (judge_item[ix][Play_Type].personal_act < grade_t_personal_act[i + 1][0]) {
            break;
        }
    }

    point += grade_t_personal_act[i][1];

    for (i = 0; i < 7; i++) {
        if (judge_item[ix][Play_Type].reversal < grade_t_reversal[i + 1][0]) {
            break;
        }
    }

    point += grade_t_reversal[i][1];

    for (i = 0; i < 8; i++) {
        if (judge_item[ix][Play_Type].comwaza < grade_t_command_waza[i + 1][0]) {
            break;
        }
    }

    point += grade_t_command_waza[i][1];

    switch (plw[ix].sa->store_max) {
    case 1:
        for (i = 0; i < 5; i++) {
            if (judge_item[ix][Play_Type].sa_exec < grade_t_sa_stock_1[i + 1][0]) {
                break;
            }
        }

        point += grade_t_sa_stock_1[i][1];
        break;

    case 2:
        for (i = 0; i < 5; i++) {
            if (judge_item[ix][Play_Type].sa_exec < grade_t_sa_stock_2[i + 1][0]) {
                break;
            }
        }

        point += grade_t_sa_stock_2[i][1];
        break;

    default:
        for (i = 0; i < 5; i++) {
            if (judge_item[ix][Play_Type].sa_exec < grade_t_sa_stock_3[i + 1][0]) {
                break;
            }
        }

        point += grade_t_sa_stock_3[i][1];
        break;
    }

    return point;
}

s16 get_ex_point_total(s16 ix, s16 wf) {
    s16 i;
    s16 point;

    point = 0;

    if (wf) {
        for (i = 0; i < 20; i++) {
            if (judge_item[ix][Play_Type].tairyokusa < grade_t_tairyokusa[i + 1][0]) {
                break;
            }
        }

        point += grade_t_tairyokusa[i][1];
        point += grade_t_round_result[judge_item[ix][Play_Type].kimarite];
    }

    for (i = 0; i < 5; i++) {
        if (judge_item[ix][Play_Type].onaji_waza < grade_t_onaji_waza[i + 1][0]) {
            break;
        }
    }

    point += grade_t_onaji_waza[i][1];

    if (judge_item[ix][Play_Type].app_nml_block != -1) {
        for (i = 0; i < 6; i++) {
            if (judge_item[ix][Play_Type].app_nml_block < grade_t_app_nmlblock[i + 1][0]) {
                break;
            }
        }

        point += grade_t_app_nmlblock[i][1];
    }

    if (judge_item[ix][Play_Type].app_rpd_block != -1) {
        for (i = 0; i < 6; i++) {
            if (judge_item[ix][Play_Type].app_rpd_block < grade_t_app_rpdblock[i + 1][0]) {
                break;
            }
        }

        point += grade_t_app_rpdblock[i][1];
    }

    if (judge_item[ix][Play_Type].app_grd_block != -1) {
        for (i = 0; i < 6; i++) {
            if (judge_item[ix][Play_Type].app_grd_block < grade_t_app_grdblock[i + 1][0]) {
                break;
            }
        }

        point += grade_t_app_grdblock[i][1];
    }

    return point;
}

void grade_add_clean_hits(WORK_Other* wk) {
    WORK* mwk;
    s16 ix;

    if (pcon_rno[0] != 0) {
        ix = wk->wu.id;

        if (wk->wu.work_id != 1) {
            mwk = (WORK*)(wk->my_master);

            if (mwk->work_id != 1) {
                return;
            }
            ix = mwk->id;
        }
        judge_item[ix][Play_Type].clean_hits += 1;
    }
}

void grade_add_att_renew(WORK_Other* wk) {
    WORK* mwk;
    s16 ix;

    if (pcon_rno[0] != 0) {
        ix = wk->wu.id;

        if (wk->wu.work_id != 1) {
            mwk = (WORK*)(wk->my_master);

            if (mwk->work_id != 1) {
                return;
            }
            ix = mwk->id;
        }
        judge_item[ix][Play_Type].att_renew += 1;
    }
}

void grade_add_guard_success(s16 ix) {
    if ((u16)ix < 2) {
        judge_item[ix][Play_Type].guard_succ++;
    }
}

void grade_add_em_stun(s16 ix) {
    judge_item[ix][Play_Type].em_stun++;

    if (judge_item[ix][Play_Type].em_stun > 4) {
        judge_item[ix][Play_Type].em_stun = 4;
    }
}

void grade_max_combo_check(s16 ix, s16 num) {
    if (judge_item[ix][Play_Type].max_combo < num) {
        judge_item[ix][Play_Type].max_combo = num;
    }
}

void grade_add_leap_attack(s16 ix) {
    judge_item[ix][Play_Type].leap_attack += 1;

    if (judge_item[ix][Play_Type].leap_attack > 12) {
        judge_item[ix][Play_Type].leap_attack = 12;
    }
}

void grade_add_grap_def(s16 ix) {
    judge_item[ix][Play_Type].grap_def += 1;

    if (judge_item[ix][Play_Type].grap_def > 16) {
        judge_item[ix][Play_Type].grap_def = 16;
    }
}

void grade_add_quick_stand(s16 ix) {
    judge_item[ix][Play_Type].quick_stand++;

    if (judge_item[ix][Play_Type].quick_stand > 6) {
        judge_item[ix][Play_Type].quick_stand = 6;
    }
}

void grade_add_nml_nage(WORK* wk) {
    s16 ix;

    if (check_normal_attack(wk->kind_of_waza)) {
        ix = wk->id;
        judge_item[ix][Play_Type].nml_nage += 1;
        if (judge_item[ix][Play_Type].nml_nage > 0xC) {
            judge_item[ix][Play_Type].nml_nage = 0xC;
        }
    }
}

void grade_add_reversal(s16 ix) {
    judge_item[ix][Play_Type].reversal += 1;

    if (judge_item[ix][Play_Type].reversal > 10) {
        judge_item[ix][Play_Type].reversal = 10;
    }
}

void grade_add_target_combo(s16 ix) {
    judge_item[ix][Play_Type].target_combo++;

    if (judge_item[ix][Play_Type].target_combo > 24) {
        judge_item[ix][Play_Type].target_combo = 24;
    }
}

void grade_add_command_waza(s16 ix) {
    judge_item[ix][Play_Type].comwaza += 1;

    if (judge_item[ix][Play_Type].comwaza > 36) {
        judge_item[ix][Play_Type].comwaza = 36;
    }
}

void grade_add_super_arts(s16 ix, s16 num) {
    judge_item[ix][Play_Type].sa_exec += num;

    if (judge_item[ix][Play_Type].sa_exec > 5) {
        judge_item[ix][Play_Type].sa_exec = 5;
    }
}

void grade_store_vitality(s16 ix) {
    judge_item[ix][Play_Type].vitality = plw[ix].wu.vital_new;
}

void grade_add_blocking(PLW* wk) {
    s16 ix = wk->wu.id;

    switch (wk->kind_of_blocking) {
    case 0:
        judge_item[ix][Play_Type].app_nml_block = wk->wu.vital_new;

        if ((judge_item[ix][Play_Type].nml_blocking += 1) > 15) {
            judge_item[ix][Play_Type].nml_blocking = 15;
            break;
        }

        break;

    case 1:
        judge_item[ix][Play_Type].app_rpd_block = wk->wu.vital_new;

        if ((judge_item[ix][Play_Type].rpd_blocking += 1) > 15) {
            judge_item[ix][Play_Type].rpd_blocking = 15;
            return;
        }

        break;

    case 2:
        judge_item[ix][Play_Type].app_grd_block = wk->wu.vital_new;

        if ((judge_item[ix][Play_Type].grd_blocking += 1) > 15) {
            judge_item[ix][Play_Type].grd_blocking = 15;
        }

        break;
    }
}

void grade_get_first_attack(s16 ix) {
    judge_item[ix][Play_Type].first_attack = 1;
}

void grade_set_round_result(s16 ix) {
    if (Round_Result & 0x8201) {
        judge_item[ix][Play_Type].kimarite = 0;
        return;
    }

    if (Round_Result & 0x2C) {
        judge_item[ix][Play_Type].kimarite = 1;
        return;
    }

    if (Round_Result & 0x50) {
        judge_item[ix][Play_Type].kimarite = 2;
        return;
    }

    if (Round_Result & 0x980) {
        judge_item[ix][Play_Type].kimarite = 3;
        return;
    }

    judge_item[ix][Play_Type].kimarite = 0;
}

void grade_add_personal_action(s16 ix) {
    if (!pcon_dp_flag) {
        judge_item[ix][Play_Type].personal_act++;

        if (judge_item[ix][Play_Type].personal_act > 3) {
            judge_item[ix][Play_Type].personal_act = 3;
        }
    }
}

void grade_check_tairyokusa() {
    s16 vwork = plw[1].wu.vital_new - plw[0].wu.vital_new;

    if (vwork > 0 && judge_item[0][Play_Type].tairyokusa < vwork) {
        judge_item[0][Play_Type].tairyokusa = vwork;
    }

    vwork = plw[0].wu.vital_new - plw[1].wu.vital_new;

    if (vwork > 0 && judge_item[1][Play_Type].tairyokusa < vwork) {
        judge_item[1][Play_Type].tairyokusa = vwork;
    }
}

void grade_add_onaji_waza(s16 ix) {
    s16 num = plw[ix].wu.char_index + ((plw[ix].wu.now_koc == 5) * 0xF0);

    if (num < sizeof(ji_sat[0])) {
        if (ji_sat[ix][num] != 0xFF) {
            ji_sat[ix][num]++;
        }

        if (judge_item[ix][Play_Type].onaji_waza < ji_sat[ix][num]) {
            judge_item[ix][Play_Type].onaji_waza = ji_sat[ix][num];
        }
    }
}

s16 grade_get_my_grade(s16 ix) {
    return judge_item[ix][Play_Type].grade;
}

s16 grade_get_my_point_percentage(s16 ix, s16 flag) {
    s32 rnum;

    switch (flag) {
    case 0:
        rnum = judge_item[ix][Play_Type].offence_total * 100;
        rnum /= 500;
        break;

    case 1:
        rnum = judge_item[ix][Play_Type].defence_total * 100;
        rnum /= 500;
        break;

    case 2:
        rnum = judge_item[ix][Play_Type].tech_pts_total * 100;
        rnum /= 500;
        break;

    case 3:
        rnum = judge_item[ix][Play_Type].ex_point_total * 100;
        rnum /= 500;
        break;
    }

    if (rnum > 120) {
        rnum = 120;
    }

    return rnum;
}

s16 grade_get_cm_point_percentage(s16 ix, s16 flag) {
    s32 rnum;

    switch (flag) {
    case 0:
        rnum = judge_com[ix].offence_total * 100;
        rnum /= 500;
        break;

    case 1:
        rnum = judge_com[ix].defence_total * 100;
        rnum /= 500;
        break;

    case 2:
        rnum = judge_com[ix].tech_pts_total * 100;
        rnum /= 500;
        break;

    case 3:
        rnum = judge_com[ix].ex_point_total * 100;
        rnum /= 500;
        break;
    }

    if (rnum > 120) {
        rnum = 120;
    }

    return rnum;
}

s16 get_grade_ix(s16 pts) {
    s16 i;

    for (i = 0; i < 31; i++) {
        if (pts < grade_t_table[i + 1][0]) {
            break;
        }
    }

    return i;
}

void check_guard_miss(WORK* as, PLW* ds, s8 gddir) {
    if (ds->remake_power.total) {
        return;
    }

    judge_item[ds->wu.id][Play_Type].grd_mcnt++;

    if ((ds->guard_flag != 3) && (as->att.guard & 0x3F) && (ds->wu.xyz[1].disp.pos < 2) &&
        (as->work_id != 1 || !as->jump_att_flag || !(ds->cp->sw_new & 0xF)) && (!(ds->cp->sw_new & 1)) &&
        (!(ds->saishin_lvdir & gddir)) && ((ds->cp->waza_flag[3] + ds->cp->waza_flag[4]) == 0)) {
        return;
    }

    judge_item[ds->wu.id][Play_Type].grd_miss++;
}
