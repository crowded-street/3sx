/**
 * @file eff64.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff64.h"
#include "common.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void EFF64_WAIT(WORK_Other_CONN* ewk);
void EFF64_SLIDE_IN(WORK_Other_CONN* ewk);
void EFF64_CHAR_CHANGE(WORK_Other_CONN* /* unused */);
void EFF64_SUDDENLY(WORK_Other_CONN* /* unused */);
void Disp_64_Sub(WORK_Other_CONN* ewk);
void Setup_Letter_64(WORK_Other_CONN* ewk, s16 disp_index);

const s8* Letter_Data_64[16][16] = {
    { "\"OFF\"", "\"/ON/\"", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "#;;;;;;;",
      "##;;;;;;",
      "###;;;;;",
      "####;;;;",
      "#####;;;",
      "######;;",
      "#######;",
      "########",
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL },
    { "30", "60", "99", "{", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "1", "3", "5", "7", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "#;;;", "##;;", "###;", "####", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "OLD", "NEW", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "DISABLE", "ENABLE", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "OFF", "ON", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "HUMAN", "CPU", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "ENGLISH", "JAPANESE", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "SYSTEM  NORMAL",
      "SYSTEM  ORIGINAL",
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL },
    { "Stereo", "Monaural", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "Silence", "-14", "-13", "-12", "-11", "-10", "-9", "-8", "-7", "-6", "-5", "-4", "-3", "-2", "-1", "Standard" },
    { "Arrange", "Original", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { "-PLAY(A)", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
    { " STOP(B) BUTTON-", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

void (*const EFF64_Jmp_Tbl[4])() = { EFF64_WAIT, EFF64_SLIDE_IN, EFF64_CHAR_CHANGE, EFF64_SUDDENLY };

void effect_64_move(WORK_Other_CONN* ewk) {
    if (Check_Die_61((WORK_Other*)ewk)) {
        push_effect_work(&ewk->wu);
        return;
    }

    EFF64_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag == 0) {
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

    if (Menu_Cursor_Y[0] == ewk->wu.type) {
        ewk->wu.my_clear_level = 0;
    } else {
        ewk->wu.my_clear_level = 128;
    }

    sort_push_request3(&ewk->wu);
}

void EFF64_WAIT(WORK_Other_CONN* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }

    Disp_64_Sub(ewk);
}

void EFF64_SLIDE_IN(WORK_Other_CONN* ewk) {
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
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_64[ewk->wu.dir_step][0] + 384;
        ewk->wu.xyz[1].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Slide_Pos_Data_64[ewk->wu.dir_step][1];
        ewk->wu.position_z = 70;
        ewk->wu.hit_quake = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_64[ewk->wu.dir_step][0];
        ewk->wu.mvxy.a[0].sp = -0x400000;
        ewk->wu.mvxy.d[0].sp = 0x50000;
        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (ewk->wu.hit_quake < ewk->wu.xyz[0].disp.pos) {
            break;
        }

        if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
            Order[ewk->wu.dir_old] = 0;
        }

        ewk->wu.routine_no[0] = 0;
        ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
        break;
    }
}

void EFF64_CHAR_CHANGE(WORK_Other_CONN* /* unused */) {}

void EFF64_SUDDENLY(WORK_Other_CONN* /* unused */) {}

s32 effect_64_init(u8 dir_old, s16 sync_bg, s16 master_player, s16 letter_type, s16 cursor_index, u16 char_offset,
                   s16 pos_index, s16 convert_id, s16 convert_id2) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 64;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = sync_bg + 1;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.dir_step = pos_index;
    ewk->wu.type = cursor_index;
    ewk->wu.char_index = letter_type;
    ewk->wu.dir_old = dir_old;
    ewk->master_player = master_player;
    ewk->wu.old_cgnum = char_offset;
    ewk->master_priority = convert_id;
    ewk->wu.cg_type = convert_id2;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    Disp_64_Sub(ewk);
    return 0;
}

void Disp_64_Sub(WORK_Other_CONN* ewk) {
    Setup_Letter_64(ewk, Convert_Buff[ewk->master_priority][ewk->wu.cg_type][ewk->wu.type]);
}

void Setup_Letter_64(WORK_Other_CONN* ewk, s16 disp_index) {
    s16 x;
    s16 ix;
    s16 offset_x;
    const u8* ptr;

    if (ewk->wu.old_cgnum == 0x70A7) {
        offset_x = 8;
    } else {
        offset_x = 14;
    }

    ptr = (u8*)Letter_Data_64[ewk->wu.char_index][disp_index];
    ix = 0;
    x = 0;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            x += offset_x;
            ptr++;
            continue;
        }

        if (*ptr == '/') {
            x += offset_x / 2;
            ptr++;
            continue;
        }

        ewk->conn[ix].nx = x;
        ewk->conn[ix].ny = 0;
        ewk->conn[ix].col = 0;
        ewk->conn[ix].chr = ewk->wu.old_cgnum + *ptr;
        x += offset_x;
        ptr++;
        ix++;
    }

    ewk->num_of_conn = ix;
}
