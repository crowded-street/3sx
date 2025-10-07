#include "sf33rd/Source/Game/EFF63.h"
#include "common.h"
#include "sf33rd/Source/Game/EFF61.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void EFF63_WAIT(WORK_Other_CONN* ewk);
void EFF63_SLIDE_IN(WORK_Other_CONN* ewk);
void EFF63_CHAR_CHANGE(WORK_Other_CONN* /* unused */);
void EFF63_SUDDENLY(WORK_Other_CONN* /* unused */);
void Disp_63_Sub(WORK_Other_CONN* ewk);
void Setup_Letter_63(WORK_Other_CONN* ewk, s16 disp_index);

const s8* Letter_Data_63[3][21] = { { "-10", "-9", "-8", "-7", "-6", "-5", "-4", "-3", "-2", "-1", "0",
                                      "1",   "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10" },
                                    { "94%", "96%", "98%", "100%", NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                      NULL,  NULL,  NULL,  NULL,   NULL, NULL, NULL, NULL, NULL, NULL },
                                    { "OFF", "ON", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                      NULL,  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } };

void (*const EFF63_Jmp_Tbl[4])() = { EFF63_WAIT, EFF63_SLIDE_IN, EFF63_CHAR_CHANGE, EFF63_SUDDENLY };

void effect_63_move(WORK_Other_CONN* ewk) {
    if (Check_Die_61((WORK_Other*)ewk) != 0) {
        push_effect_work(&ewk->wu);
        return;
    }

    EFF63_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

    if (Menu_Cursor_Y[0] == ewk->wu.type) {
        ewk->wu.my_clear_level = 0;
    } else {
        ewk->wu.my_clear_level = 128;
    }

    sort_push_request3(&ewk->wu);
}

void EFF63_WAIT(WORK_Other_CONN* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }

    Disp_63_Sub(ewk);
}

void EFF63_SLIDE_IN(WORK_Other_CONN* ewk) {
    if (Order[ewk->wu.dir_old] != 1) {
        ewk->wu.routine_no[0] = Order[ewk->wu.dir_old];
        ewk->wu.routine_no[1] = 0;
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[0].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_63[ewk->wu.type][0] + 384;
        ewk->wu.xyz[1].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Slide_Pos_Data_63[ewk->wu.type][1];
        ewk->wu.position_z = 68;
        ewk->wu.hit_quake = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_63[ewk->wu.type][0];
        ewk->wu.mvxy.a[0].sp = -0x400000;
        ewk->wu.mvxy.d[0].sp = 0x50000;
        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (0 < ewk->wu.mvxy.a[0].sp) {
            if (ewk->wu.hit_quake <= ewk->wu.xyz[0].disp.pos) {
                if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
                    Order[ewk->wu.dir_old] = 0;
                }

                ewk->wu.routine_no[0] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
            }

            break;
        }

        if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
            if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
                Order[ewk->wu.dir_old] = 0;
            }

            ewk->wu.routine_no[0] = 0;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
        }

        break;
    }
}

void EFF63_CHAR_CHANGE(WORK_Other_CONN* /* unused */) {}

void EFF63_SUDDENLY(WORK_Other_CONN* /* unused */) {}

s32 effect_63_init(u8 dir_old, s16 sync_bg, s16 master_player, s16 letter_type, s16 cursor_index) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 63;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = (sync_bg + 1);
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.type = cursor_index;

    switch (letter_type) {
    case 0:
    case 1:
        ewk->wu.dir_step = 0;
        break;

    case 2:
    case 3:
        ewk->wu.dir_step = 1;
        break;

    default:
        ewk->wu.dir_step = 2;
        break;
    }

    ewk->wu.dir_old = dir_old;
    ewk->master_player = master_player;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    Disp_63_Sub(ewk);
    return 0;
}

void Disp_63_Sub(WORK_Other_CONN* ewk) {
    s16 disp_index;

    switch (ewk->wu.type) {
    case 0:
        disp_index = (s16)X_Adjust + 10;
        break;

    case 1:
        disp_index = (s16)Y_Adjust + 10;
        break;

    case 2:
        disp_index = (s16)Disp_Size_H - 94;
        disp_index >>= 1;
        break;

    case 3:
        disp_index = (s16)Disp_Size_V - 94;
        disp_index >>= 1;
        break;

    default:
        disp_index = sys_w.screen_mode;
        break;
    }

    Setup_Letter_63(ewk, disp_index);
}

void Setup_Letter_63(WORK_Other_CONN* ewk, s16 disp_index) {
    const u8* ptr = (u8*)Letter_Data_63[ewk->wu.dir_step][disp_index];
    s16 ix = 0;
    s16 x = 0;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            x += 12;
            ptr++;
            continue;
        }

        ewk->conn[ix].nx = x;
        ewk->conn[ix].ny = 0;
        ewk->conn[ix].col = 0;
        ewk->conn[ix].chr = *ptr + 0x7047;
        x += 16;
        ptr++;
        ix++;
    }

    ewk->num_of_conn = ix;
}
