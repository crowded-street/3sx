/**
 * @file effk8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effk8.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void effect_K8_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        seraph_flag = 1;
        Bg_Disp_Switch(1);
        ewk->wu.my_priority = ewk->wu.position_z = 67;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = 127;
        ewk->wu.my_mr.size.y = 127;
        set_char_move_init(&ewk->wu, 0, 0x8F);
        break;

    case 1:
        if (ewk->wu.dead_f != 0) {
            Bg_Y_Sitei(0U, 0);
            Bg_Disp_Switch(0);
            seraph_flag = 0;
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 3;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            char_move(&ewk->wu);

            if (ewk->wu.dir_old != mwk->now_koc || ewk->wu.dir_step != mwk->char_index) {
                ewk->wu.routine_no[0] = 2;
                Bg_Y_Sitei(0, 0);
                seraph_flag = 0;
                Bg_Disp_Switch(0);
            }
        }

        push_color_trans_req(ewk->wu.cg_type + 0x62, ewk->wu.my_col_code);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_K8_init(WORK* wk, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(5)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->my_master = (u32*)wk;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 208;
    ewk->wu.type = data;
    ewk->wu.my_mts = 6;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = 8;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = 2;
    ewk->wu.position_x = -8;
    ewk->wu.position_y = 240 - base_y_pos;
    ewk->wu.dir_old = wk->now_koc;
    ewk->wu.dir_step = wk->char_index;
    *ewk->wu.char_table = _plef_char_table;
    return 0;
}
