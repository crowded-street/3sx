/**
 * @file eff23.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff23.h"
#include "common.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/texcash.h"

void EFF23_WAIT(WORK_Other_CONN* ewk);
void EFF23_SLIDE_IN(WORK_Other_CONN* ewk);
void EFF23_CHAR_CHANGE(WORK_Other_CONN* /* unused */);
void EFF23_SUDDENLY(WORK_Other_CONN* /* unused */);
void Setup_23_Sub(WORK_Other_CONN* ewk);
void Setup_Letter_23(WORK_Other_CONN* ewk, s16 disp_index);

const s8* Letter_Data_23[4][12] = { { "L.PUNCH",
                                      "M.PUNCH",
                                      "H.PUNCH",
                                      "L.KICK",
                                      "M.KICK",
                                      "H.KICK",
                                      "L.P+L.K",
                                      "M.P+M.K",
                                      "H.P+H.K",
                                      "PUNCH*3",
                                      "KICK*3",
                                      "NONE" },
                                    { "  OFF", "  ON", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
                                    { "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "VIBRATION",
                                      "DEFAULT SETTING",
                                      "EXIT",
                                      "PLAYER 1" },
                                    { "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "  ^^^ ",
                                      "VIBRATION",
                                      "DEFAULT SETTING",
                                      "EXIT",
                                      "PLAYER 2" } };

void (*const EFF23_Jmp_Tbl[4])() = { EFF23_WAIT, EFF23_SLIDE_IN, EFF23_CHAR_CHANGE, EFF23_SUDDENLY };

void effect_23_move(WORK_Other_CONN* ewk) {
    if (Check_Die_61((WORK_Other*)ewk)) {
        push_effect_work(&ewk->wu);
        return;
    }

    EFF23_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

    if (ewk->wu.dir_old == 91 || ewk->wu.dir_old == 103) {
        ewk->wu.my_clear_level = 0;
        sort_push_request3(&ewk->wu);
        return;
    }

    if (Menu_Cursor_Y[ewk->master_id] == ewk->wu.type) {
        ewk->wu.my_clear_level = 0;
    } else {
        ewk->wu.my_clear_level = 128;
    }

    sort_push_request3(&ewk->wu);

    if (ewk->wu.char_index != 2 && ewk->wu.char_index != 3) {
        return;
    }

    if (ewk->wu.type < 8 && ewk->wu.disp_flag) {
        dispButtonImage(-44, 12, ewk->wu.position_z, 23, 19, ewk->wu.my_clear_level, ewk->wu.type);
    }
}

void EFF23_WAIT(WORK_Other_CONN* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }

    Setup_23_Sub(ewk);
}

void EFF23_SLIDE_IN(WORK_Other_CONN* ewk) {
    s16 offset_x;

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
        offset_x = ewk->master_id * 192;
        ewk->wu.xyz[0].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_23[ewk->wu.dir_step][0] + 384 + offset_x;
        ewk->wu.xyz[1].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Slide_Pos_Data_23[ewk->wu.dir_step][1];
        ewk->wu.position_z = 68;
        ewk->wu.hit_quake =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_23[ewk->wu.dir_step][0] + offset_x;
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
        Menu_Cursor_Move--;
        break;
    }
}

void EFF23_CHAR_CHANGE(WORK_Other_CONN* /* unused */) {}

void EFF23_SUDDENLY(WORK_Other_CONN* /* unused */) {}

s32 effect_23_init(s16 id, u8 dir_old, s16 sync_bg, s16 master_player, s16 letter_type, s16 cursor_index,
                   u16 char_offset, s16 pos_index, s16 type) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 23;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x1AC;
    ewk->master_id = id;
    ewk->wu.dir_old = dir_old;
    ewk->wu.my_family = sync_bg + 1;
    ewk->master_player = master_player;
    ewk->wu.char_index = letter_type;
    ewk->wu.type = cursor_index;
    ewk->wu.old_cgnum = char_offset;
    ewk->wu.dir_step = pos_index;
    ewk->master_priority = type;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    Setup_23_Sub(ewk);
    return 0;
}

void Setup_23_Sub(WORK_Other_CONN* ewk) {
    switch (ewk->master_priority) {
    case 0:
        Setup_Letter_23(ewk, Convert_Buff[1][ewk->master_id][ewk->wu.type]);
        break;

    default:
        ewk->wu.char_index = ewk->master_id + 2;
        Setup_Letter_23(ewk, ewk->wu.type);
        break;
    }
}

void Setup_Letter_23(WORK_Other_CONN* ewk, s16 disp_index) {
    s16 x;
    s16 ix;
    s16 offset_x;
    const u8* ptr;

    if (ewk->wu.old_cgnum == 0x70A7) {
        offset_x = 8;
    } else {
        offset_x = 14;
    }

    ptr = (u8*)Letter_Data_23[ewk->wu.char_index][disp_index];
    ix = 0;
    x = 0;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            x += offset_x;
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
