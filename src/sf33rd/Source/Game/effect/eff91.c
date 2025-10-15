/**
 * @file eff91.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff91.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 EFF91_Pos_Data[2][3][2] = { { { -88, 95 }, { -94, 71 }, { -88, 47 } },
                                      { { 104, 95 }, { 98, 71 }, { 104, 47 } } };

void effect_91_move(WORK_Other* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->wu.be_flag == 0) {
        return;
    }

    if ((ewk->wu.type == 1 && !Debug_w[49]) && (Round_Operator[0] == 0 || Round_Operator[1] == 0)) {
        ewk->wu.my_clear_level = 205;
        sort_push_request4(&ewk->wu);
        return;
    }

    if (Menu_Cursor_Y[ewk->master_id] == ewk->wu.type) {
        if (Menu_Cursor_X[ewk->master_id] != 0) {
            ewk->wu.my_clear_level = 0;
        } else {
            ewk->wu.my_clear_level = Flash_Synchro;
        }
    } else {
        ewk->wu.routine_no[1] = 0;
        ewk->wu.my_clear_level = 128;
    }

    sort_push_request4(&ewk->wu);
}

s32 effect_91_init(s16 master_id, s16 type, s16 target_bg, s16 char_ix, s16 char_ix2, s16 master_player) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = 1;
    ewk->wu.id = 91;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 428;
    ewk->master_id = master_id;
    ewk->wu.my_family = target_bg + 1;
    ewk->wu.char_table[0] = _sel_pl_char_table;
    ewk->wu.type = type;
    ewk->master_player = master_player;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + EFF91_Pos_Data[master_id][ewk->wu.type][0];
    ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + EFF91_Pos_Data[master_id][ewk->wu.type][1];
    ewk->wu.position_z = 68;
    set_char_move_init2(&ewk->wu, 0, char_ix, char_ix2 + 1, 0);
    return 0;
}
