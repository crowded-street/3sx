/**
 * @file effg9.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effg9.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

const s16 g9_pos_hos[16] = { 0, 1, -1, 2, -2, 0, 3, -3, 1, -1, 2, -2, 4, 3, 0, 3 };

void effect_G9_move(WORK_Other* ewk) {
    s16 rnd_ix;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;
        ewk->wu.disp_flag = ewk->wu.type;
        ewk->wu.xyz[0].disp.pos += g9_pos_hos[random_16()];
        rnd_ix = random_16();
        ewk->wu.xyz[1].disp.pos += g9_pos_hos[rnd_ix];

        if (g9_pos_hos[rnd_ix] > 1) {
            ewk->wu.position_z = 67;
        } else {
            ewk->wu.position_z = 33;
        }

        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] += 1;
            break;
        }

        if (Pause_Hit_Marks) {
            break;
        }

        if ((EXE_flag == 0) && (Game_pause == 0)) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type == 0xFF) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] += 1;
                break;
            }

            add_mvxy_speed(&ewk->wu);
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
        sort_push_request8(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_G9_init(WORK* wk) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.type = wk->disp_flag;
    ewk->wu.id = 0xA9;
    ewk->wu.work_id = 0x10;
    ewk->wu.rl_flag = wk->rl_flag;
    ewk->wu.char_index = wk->direction;
    ewk->wu.my_family = wk->my_family;
    ewk->wu.cgromtype = wk->cgromtype;
    ewk->wu.my_col_mode = wk->my_col_mode;
    ewk->wu.my_col_code = wk->my_col_code;
    ewk->wu.mvxy.a[0].sp = wk->mvxy.a[0].sp;
    ewk->wu.mvxy.a[1].sp = wk->mvxy.a[1].sp;
    ewk->wu.blink_timing = wk->blink_timing;
    ewk->wu.xyz[0].disp.pos = wk->xyz[0].disp.pos;
    ewk->wu.xyz[1].disp.pos = wk->xyz[1].disp.pos;
    *ewk->wu.char_table = _plef_char_table;
    return 0;
}
