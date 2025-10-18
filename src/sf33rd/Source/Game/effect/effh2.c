/**
 * @file effh2.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effh2.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

const s16 panel_guide[8][2] = { { 0, 6 }, { 0, 5 }, { 0, 4 }, { 0, 2 }, { 0, 0 }, { 0, 2 }, { 0, 4 }, { 0, 5 } };

void effect_H2_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 0x22;
        set_char_move_init2(&ewk->wu, 0, 0, ewk->wu.type + 21, 0);
        ewk->wu.disp_flag = 0;
        ewk->wu.direction = 7;
        ewk->wu.dir_old = 7;
        ewk->wu.dir_timer = 0;
        ewk->wu.dir_step = 0;
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (mwk->disp_flag == 1) {
            ewk->wu.disp_flag = 1;
        }

        ewk->wu.dir_timer = ewk->wu.dir_timer + 1 & ewk->wu.dir_old;

        if (ewk->wu.dir_timer == 0) {
            ewk->wu.dir_step = ewk->wu.dir_step + 1 & ewk->wu.direction;
        }

        ewk->wu.position_x = mwk->position_x + panel_guide[ewk->wu.dir_step][0];
        ewk->wu.position_y = mwk->position_y + panel_guide[ewk->wu.dir_step][1] + 17;
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_H2_init(WORK* wk, u8 gal, u8 ohen) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 172;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.charset_id = gal;
    ewk->wu.type = ohen;
    ewk->wu.rl_flag = 0;
    ewk->my_master = wk;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_family = 2;
    ewk->wu.position_z = wk->position_z - 1;
    ewk->wu.char_table[0] = _ag_face_panel_table;
    return 0;
}
