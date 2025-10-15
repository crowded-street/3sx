/**
 * @file eff04.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff04.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 EFF04_Cursor_Data[7][7][2] = {
    { { 16, 156 }, { 32, 135 }, { 48, 114 }, { 64, 93 }, { 80, 72 }, { 96, 51 }, { 112, 30 } },
    { { 16, 164 }, { 32, 142 }, { 48, 120 }, { 64, 98 }, { 80, 76 }, { 96, 54 }, { 112, 32 } },
    { { 32, 140 }, { 48, 108 }, { 64, 76 }, { 112, 44 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 48, 156 }, { 64, 124 }, { 80, 92 }, { 96, 60 }, { 112, 28 }, { 0, 0 }, { 0, 0 } },
    { { 32, 156 }, { 48, 132 }, { 64, 108 }, { 80, 84 }, { 96, 60 }, { 112, 36 }, { 0, 0 } },
    { { 24, 132 }, { 48, 92 }, { 112, 52 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { -16, 163 }, { 0, 145 }, { 16, 127 }, { 32, 109 }, { 48, 91 }, { 68, 68 }, { 120, 28 } },
};

void effect_04_move(WORK_Other* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (--ewk->wu.dir_timer) {
            break;
        }

        ewk->wu.routine_no[0]++;
        /* fallthrough */

    case 1:
        ewk->wu.position_x = EFF04_Cursor_Data[ewk->wu.type][Menu_Cursor_Y[0]][0] + ewk->wu.dmcal_m;
        ewk->wu.position_y = EFF04_Cursor_Data[ewk->wu.type][Menu_Cursor_Y[0]][1] + 0;
        char_move(&ewk->wu);
        sort_push_request4(&ewk->wu);
        break;
    }
}

s32 effect_04_init(s16 Death_Type, s16 cg_type, s16 sync_bg, s16 priority) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = 1;
    ewk->wu.id = 4;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = sync_bg + 1;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.dir_timer = 20;
    ewk->wu.type = cg_type;
    ewk->master_player = Death_Type;
    ewk->wu.my_priority = ewk->wu.position_z = priority;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.char_table[0] = _sel_pl_char_table;
    ewk->wu.dmcal_m = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos;
    set_char_move_init(&ewk->wu, 0, 62);
    return 0;
}
