#include "sf33rd/Source/Game/EFFF9.h"
#include "common.h"
#include "sf33rd/Source/Game/EFFB6.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/workuser.h"

const u16 plXXtxt0[2] = { 0, 0xFFFF };
const u16 pl00txt1[8] = { 1, 212, 2, 212, 3, 212, 0, 0xFFFF };
const u16 pl00txt2[12] = { 4, 212, 5, 212, 6, 212, 7, 212, 8, 212, 0, 0xFFFF };
const u16 pl00txt3[8] = { 9, 212, 10, 212, 11, 212, 0, 0xFFFF };
const u16 pl00txt4[8] = { 12, 212, 13, 212, 14, 212, 0, 0xFFFF };
const u16 pl00txt5[8] = { 15, 212, 16, 212, 17, 212, 0, 0xFFFF };
const u16 pl01txt1[6] = { 1, 212, 2, 196, 0, 0xFFFF };
const u16 pl01txt2[10] = { 3, 228, 4, 228, 5, 228, 6, 212, 0, 0xFFFF };
const u16 pl01txt3[14] = { 7, 212, 8, 212, 9, 212, 10, 212, 11, 228, 12, 196, 0, 0xFFFF };
const u16 pl01txt4[4] = { 13, 212, 0, 0xFFFF };
const u16 pl01txt5[6] = { 14, 212, 15, 212, 0, 0xFFFF };
const u16 pl02txt1[6] = { 1, 232, 2, 240, 0, 0xFFFF };
const u16 pl02txt2[8] = { 3, 228, 4, 212, 5, 212, 0, 0xFFFF };
const u16 pl02txt3[4] = { 6, 222, 0, 0xFFFF };
const u16 pl02txt4[2] = { 0, 0xFFFF };
const u16 pl02txt5[10] = { 7, 192, 8, 212, 9, 248, 10, 212, 0, 0xFFFF };
const u16 pl03txt1[8] = { 1, 180, 2, 240, 3, 248, 0, 0xFFFF };
const u16 pl03txt2[6] = { 4, 255, 5, 240, 0, 0xFFFF };
const u16 pl03txt3[8] = { 6, 228, 7, 228, 8, 244, 0, 0xFFFF };
const u16 pl03txt4[10] = { 9, 208, 10, 255, 11, 144, 12, 256, 0, 0xFFFF };
const u16 pl03txt5[8] = { 13, 212, 14, 212, 15, 212, 0, 0xFFFF };
const u16 pl03txt6[12] = { 16, 180, 17, 196, 18, 212, 19, 228, 20, 240, 0, 0xFFFF };
const u16 pl04txt1[10] = { 1, 232, 2, 232, 3, 232, 4, 256, 0, 0xFFFF };
const u16 pl04txt2[14] = { 5, 196, 6, 212, 7, 144, 8, 144, 9, 212, 10, 240, 0, 0xFFFF };
const u16 pl05txt1[8] = { 1, 176, 2, 176, 3, 212, 0, 0xFFFF };
const u16 pl05txt2[10] = { 4, 96, 5, 176, 6, 212, 7, 212, 0, 0xFFFF };
const u16 pl05txt3[6] = { 8, 212, 9, 240, 0, 0xFFFF };
const u16 pl05txt4[4] = { 10, 304, 0, 0xFFFF };
const u16 pl05txt5[6] = { 11, 240, 12, 240, 0, 0xFFFF };
const u16 pl05txt6[6] = { 13, 212, 14, 240, 0, 0xFFFF };
const u16 pl06txt1[6] = { 1, 212, 2, 240, 0, 0xFFFF };
const u16 pl06txt2[6] = { 3, 112, 4, 240, 0, 0xFFFF };
const u16 pl06txt3[12] = { 5, 160, 6, 224, 7, 212, 8, 212, 9, 240, 0, 0xFFFF };
const u16 pl06txt4[8] = { 10, 212, 11, 224, 12, 256, 0, 0xFFFF };
const u16 pl06txt5[6] = { 13, 240, 14, 288, 0, 0xFFFF };
const u16 pl07txt1[6] = { 1, 240, 2, 240, 0, 0xFFFF };
const u16 pl07txt2[10] = { 3, 240, 4, 128, 5, 212, 6, 256, 0, 0xFFFF };
const u16 pl07txt3[8] = { 7, 212, 8, 272, 9, 272, 0, 0xFFFF };
const u16 pl07txt4[12] = { 10, 212, 11, 212, 12, 192, 13, 160, 14, 240, 0, 0xFFFF };
const u16 pl08txt1[10] = { 1, 144, 2, 192, 3, 240, 4, 272, 0, 0xFFFF };
const u16 pl08txt2[6] = { 5, 240, 6, 240, 0, 0xFFFF };
const u16 pl08txt3[10] = { 7, 224, 8, 224, 9, 224, 10, 240, 0, 0xFFFF };
const u16 pl08txt4[4] = { 11, 360, 0, 0xFFFF };
const u16 pl09txt1[4] = { 1, 304, 0, 0xFFFF };
const u16 pl09txt2[6] = { 2, 288, 3, 288, 0, 0xFFFF };
const u16 pl09txt3[4] = { 4, 240, 0, 0xFFFF };
const u16 pl09txt4[6] = { 5, 256, 6, 288, 0, 0xFFFF };
const u16 pl09txt5[8] = { 7, 256, 8, 256, 9, 272, 0, 0xFFFF };
const u16 pl09txt6[6] = { 10, 288, 11, 304, 0, 0xFFFF };
const u16 pl10txt1[10] = { 1, 176, 2, 212, 3, 212, 4, 192, 0, 0xFFFF };
const u16 pl10txt2[10] = { 5, 160, 6, 192, 7, 212, 8, 212, 0, 0xFFFF };
const u16 pl10txt3[6] = { 9, 212, 10, 212, 0, 0xFFFF };
const u16 pl10txt4[10] = { 11, 224, 12, 212, 13, 212, 14, 144, 0, 0xFFFF };
const u16 pl10txt5[6] = { 15, 212, 16, 212, 0, 0xFFFF };
const u16 pl10txt6[8] = { 17, 224, 18, 224, 19, 288, 0, 0xFFFF };
const u16 pl11txt1[4] = { 1, 240, 0, 0xFFFF };
const u16 pl11txt2[6] = { 2, 212, 3, 240, 0, 0xFFFF };
const u16 pl11txt3[8] = { 4, 212, 5, 212, 6, 212, 0, 0xFFFF };
const u16 pl11txt4[12] = { 7, 212, 8, 212, 9, 212, 10, 192, 11, 240, 0, 0xFFFF };
const u16 pl11txt5[8] = { 12, 212, 13, 212, 14, 256, 0, 0xFFFF };
const u16 pl12txt1[6] = { 1, 212, 2, 240, 0, 0xFFFF };
const u16 pl12txt2[8] = { 3, 176, 4, 212, 5, 212, 0, 0xFFFF };
const u16 pl12txt3[6] = { 6, 212, 7, 212, 0, 0xFFFF };
const u16 pl12txt4[6] = { 8, 160, 9, 212, 0, 0xFFFF };
const u16 pl12txt5[6] = { 10, 240, 11, 304, 0, 0xFFFF };
const u16 pl12txt6[4] = { 12, 304, 0, 0xFFFF };
const u16 pl13txt1[8] = { 1, 240, 2, 240, 3, 240, 0, 0xFFFF };
const u16 pl13txt2[8] = { 4, 192, 5, 212, 6, 256, 0, 0xFFFF };
const u16 pl13txt3[6] = { 7, 212, 8, 256, 0, 0xFFFF };
const u16 pl13txt4[8] = { 9, 212, 10, 212, 11, 256, 0, 0xFFFF };
const u16 pl14txt1[6] = { 1, 240, 2, 256, 0, 0xFFFF };
const u16 pl14txt2[10] = { 3, 192, 4, 128, 5, 144, 6, 240, 0, 0xFFFF };
const u16 pl14txt3[6] = { 7, 212, 8, 256, 0, 0xFFFF };
const u16 pl14txt4[2] = { 0, 0xFFFF };
const u16 pl14txt5[6] = { 9, 212, 10, 240, 0, 0xFFFF };
const u16 pl15txt1[10] = { 1, 128, 2, 128, 3, 144, 4, 240, 0, 0xFFFF };
const u16 pl15txt2[4] = { 5, 352, 0, 0xFFFF };
const u16 pl15txt3[10] = { 6, 212, 7, 128, 8, 192, 9, 128, 0, 0xFFFF };
const u16 pl15txt4[8] = { 10, 224, 11, 176, 12, 240, 0, 0xFFFF };
const u16 pl15txt5[4] = { 13, 212, 0, 0xFFFF };
const u16 pl16txt1[8] = { 1, 160, 2, 192, 3, 288, 0, 0xFFFF };
const u16 pl16txt2[6] = { 4, 240, 5, 240, 0, 0xFFFF };
const u16 pl16txt3[6] = { 6, 120, 7, 212, 0, 0xFFFF };
const u16 pl16txt4[2] = { 0, 0xFFFF };
const u16 pl16txt5[6] = { 8, 224, 9, 240, 0, 0xFFFF };
const u16 pl16txt6[14] = { 10, 192, 11, 212, 12, 212, 13, 212, 14, 192, 15, 212, 0, 0xFFFF };
const u16 pl16txt7[6] = { 16, 176, 17, 240, 0, 0xFFFF };
const u16 pl17txt1[10] = { 1, 216, 2, 216, 3, 216, 4, 240, 0, 0xFFFF };
const u16 pl17txt2[10] = { 5, 224, 6, 224, 7, 212, 8, 384, 0, 0xFFFF };
const u16 pl17txt3[6] = { 9, 192, 10, 336, 0, 0xFFFF };
const u16 pl17txt6[10] = { 11, 160, 12, 144, 13, 240, 14, 304, 0, 0xFFFF };
const u16 pl17txt4[4] = { 15, 384, 0, 0xFFFF };
const u16 pl17txt5[6] = { 16, 240, 17, 288, 0, 0xFFFF };
const u16 pl18txt1[6] = { 1, 212, 2, 212, 0, 0xFFFF };
const u16 pl18txt2[10] = { 3, 212, 4, 212, 5, 212, 6, 212, 0, 0xFFFF };
const u16 pl18txt3[8] = { 7, 212, 8, 212, 9, 212, 0, 0xFFFF };
const u16 pl18txt4[10] = { 10, 212, 11, 212, 12, 212, 13, 212, 0, 0xFFFF };
const u16 pl18txt5[8] = { 14, 212, 15, 212, 16, 212, 0, 0xFFFF };
const u16 pl19txt1[6] = { 1, 212, 2, 212, 0, 0xFFFF };
const u16 pl19txt2[6] = { 3, 212, 4, 240, 0, 0xFFFF };
const u16 pl19txt3[10] = { 5, 176, 6, 192, 7, 176, 8, 256, 0, 0xFFFF };
const u16 pl19txt4[10] = { 9, 192, 10, 176, 11, 176, 12, 288, 0, 0xFFFF };
const u16 pl19txt5[4] = { 13, 240, 0, 0xFFFF };
const u16 pl19txt6[8] = { 14, 212, 15, 192, 16, 336, 0, 0xFFFF };
const u16 pl19txt7[2] = { 0, 0xFFFF };

const u16* gill[6] = { plXXtxt0, pl00txt1, pl00txt2, pl00txt3, pl00txt4, pl00txt5 };
const u16* alex[6] = { plXXtxt0, pl01txt1, pl01txt2, pl01txt3, pl01txt4, pl01txt5 };
const u16* ryu[6] = { plXXtxt0, pl02txt1, pl02txt2, pl02txt3, pl02txt4, pl02txt5 };
const u16* yun[7] = { plXXtxt0, pl03txt1, pl03txt2, pl03txt3, pl03txt4, pl03txt5, pl03txt6 };
const u16* dudley[3] = { plXXtxt0, pl04txt1, pl04txt2 };
const u16* necro[7] = { plXXtxt0, pl05txt1, pl05txt2, pl05txt3, pl05txt4, pl05txt5, pl05txt6 };
const u16* hugo[6] = { plXXtxt0, pl06txt1, pl06txt2, pl06txt3, pl06txt4, pl06txt5 };
const u16* ibuki[5] = { plXXtxt0, pl07txt1, pl07txt2, pl07txt3, pl07txt4 };
const u16* elena[5] = { plXXtxt0, pl08txt1, pl08txt2, pl08txt3, pl08txt4 };
const u16* oro[7] = { plXXtxt0, pl09txt1, pl09txt2, pl09txt3, pl09txt4, pl09txt5, pl09txt6 };
const u16* yang[7] = { plXXtxt0, pl10txt1, pl10txt2, pl10txt3, pl10txt4, pl10txt5, pl10txt6 };
const u16* ken[6] = { plXXtxt0, pl11txt1, pl11txt2, pl11txt3, pl11txt4, pl11txt5 };
const u16* sean[7] = { plXXtxt0, pl12txt1, pl12txt2, pl12txt3, pl12txt4, pl12txt5, pl12txt6 };
const u16* urien[5] = { plXXtxt0, pl13txt1, pl13txt2, pl13txt3, pl13txt4 };
const u16* gouki[6] = { plXXtxt0, pl14txt1, pl14txt2, pl14txt3, pl14txt4, pl14txt5 };
const u16* chun[6] = { plXXtxt0, pl15txt1, pl15txt2, pl15txt3, pl15txt4, pl15txt5 };
const u16* makoto[8] = { plXXtxt0, pl16txt1, pl16txt2, pl16txt3, pl16txt4, pl16txt5, pl16txt6, pl16txt7 };
const u16* q[7] = { plXXtxt0, pl17txt1, pl17txt2, pl17txt3, pl17txt4, pl17txt5, pl17txt6 };
const u16* twelve[6] = { plXXtxt0, pl18txt1, pl18txt2, pl18txt3, pl18txt4, pl18txt5 };
const u16* remy[8] = { plXXtxt0, pl19txt1, pl19txt2, pl19txt3, pl19txt4, pl19txt5, pl19txt6, pl19txt7 };

void* txt_no_tbl[20] = { gill, alex, ryu,  yun,   dudley, necro, hugo,   ibuki, elena,  oro,
                         yang, ken,  sean, urien, gouki,  chun,  makoto, q,     twelve, remy };

u16** efff9_txt_no_adrs;
u16* efff9_txt_scene_adrs;
s16 efff9_suicide;
s16 efff9_PL_NO;
s16 efff9_txt_point;
s16 efff9_message;
s16 keep_mes_no;

s32 Rewrite();

void effect_F9_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void get_message_conn_data(WORK_Other_CONN * ewk, s32 kind, s32 pl, s32 msg);
#endif

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.my_mts = 12;
        ewk->free = 0;

        if (mes_already) {
            get_message_conn_data((WORK_Other_CONN*)ewk, 2, ewk->master_player, mes_already);
        }

        ewk->wu.old_rno[4] = ewk->free;

        if (ewk->wu.old_rno[4] == 0) {
            ewk->wu.old_rno[5] = 0;
        } else {
            ewk->wu.old_rno[5] = 1;
        }

        ewk->wu.old_rno[6] = 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.vitality = 240;
        ewk->free = ewk->wu.old_rno[5];
        efff9_suicide = 0;
        break;

    case 1:
    case 2:
    case 3:
    case 4:
        ewk->wu.routine_no[0]++;
        break;

    case 5:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.type = 0;
            ewk->wu.routine_no[0] = 6;
            break;
        }

        ewk->wu.old_rno[6]--;

        if (ewk->wu.old_rno[6] == 0) {
            ewk->wu.old_rno[6] = 3;

            if (ewk->wu.old_rno[4] == ewk->wu.old_rno[5]) {
                ewk->wu.old_rno[6] = 3;
            } else {
                ewk->wu.old_rno[5]++;

                if (Country != 1 && Country != 8 && (ewk->wu.old_rno[5]++, ewk->wu.old_rno[4] < ewk->wu.old_rno[5])) {
                    ewk->wu.old_rno[5] = ewk->wu.old_rno[4];
                }

                ewk->free = ewk->wu.old_rno[5];
            }
        }

        if (ewk->wu.old_rno[3] == 0) {
            Rewrite();
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 6;
        } else {
            ewk->wu.old_rno[3]--;

            if (efff9_suicide == 1) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 6;
                ewk->wu.disp_flag = 0;
            } else {
                sort_push_request3(&ewk->wu);
            }
        }

        break;

    case 6:
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
    }
}

void effect_F9_init(s16 END_PL_NO) {
    mes_already = 0;
    efff9_suicide = 0;
    efff9_txt_point = 0;
    keep_mes_no = 0;
    efff9_PL_NO = END_PL_NO;
}

void efff9_wk_set(WORK_Other_CONN* ewk) {
    ewk->wu.be_flag = 1;
    ewk->wu.id = 159;
    ewk->wu.work_id = 16;
    ewk->wu.rl_flag = 0;
    ewk->wu.cgromtype = 1;
    ewk->wu.sync_suzi = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 55;
    ewk->wu.my_family = 4;
    ewk->wu.my_priority = 5;
    ewk->wu.position_x = 312;
    ewk->wu.position_y = 24;
    ewk->wu.position_z = ewk->wu.my_priority = 5;
}

s32 Rewrite_End_Message(u16 mes_no) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    keep_mes_no = mes_no;
    efff9_wk_set((WORK_Other_CONN*)ewk);
    ewk->master_player = efff9_PL_NO;
    efff9_suicide = 1;
    efff9_txt_point = 2;
    efff9_txt_no_adrs = txt_no_tbl[efff9_PL_NO];
    efff9_txt_scene_adrs = efff9_txt_no_adrs[mes_no];
    efff9_message = efff9_txt_scene_adrs[0];
    ewk->wu.old_rno[3] = efff9_txt_scene_adrs[1];
    mes_already = efff9_message;
    return 0;
}

s32 Rewrite() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    efff9_wk_set((WORK_Other_CONN*)ewk);
    ewk->master_player = efff9_PL_NO;
    efff9_suicide = 1;
    efff9_txt_no_adrs = txt_no_tbl[efff9_PL_NO];
    efff9_txt_scene_adrs = efff9_txt_no_adrs[keep_mes_no];
    efff9_message = efff9_txt_scene_adrs[efff9_txt_point];
    ewk->wu.old_rno[3] = efff9_txt_scene_adrs[efff9_txt_point + 1];
    efff9_txt_point += 2;
    mes_already = efff9_message;
    return 0;
}
