/**
 * @file eff70.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff70.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void Setup_Eff70(WORK_Other* ewk);

void effect_70_move(WORK_Other* ewk) {
    if (Suicide[0] == 1) {
        ewk->wu.routine_no[0] = 99;
        ewk->wu.disp_flag = 0;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 1;
            ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
            ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
            set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
            sort_push_request4(&ewk->wu);
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            Complete_Face--;
            ewk->wu.routine_no[0]++;
            ewk->wu.char_index = 0;
            set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        }

        sort_push_request4(&ewk->wu);
        /* fallthrough */

    case 2:
        if (Play_Type == 1 && Sel_PL_Complete[0] & 0x8000 && Sel_PL_Complete[1] & 0x8000) {
            ewk->wu.routine_no[0]++;
            ewk->wu.dir_timer = 30;
        }

        sort_push_request4(&ewk->wu);
        break;

    case 3:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.my_mr_flag = 1;
            ewk->wu.my_mr.size.x = 63;
            ewk->wu.my_mr.size.y = 63;
            ewk->wu.mvxy.a[0].sp = 0x80000;
        }

        sort_push_request4(&ewk->wu);
        break;

    case 4:
        if ((ewk->wu.my_mr.size.x -= ewk->wu.mvxy.a[0].real.h) <= 0) {
            ewk->wu.my_mr.size.x = 0;
        }

        if ((ewk->wu.my_mr.size.y -= ewk->wu.mvxy.a[0].real.h) <= 0) {
            ewk->wu.my_mr.size.y = 0;
        }

        if (ewk->wu.my_mr.size.x <= 0 && ewk->wu.my_mr.size.y <= 0) {
            ewk->wu.routine_no[1]++;
            ewk->wu.my_mr_flag = 0;
            ewk->wu.disp_flag = 0;
            break;
        }

        sort_push_request4(&ewk->wu);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_70_init(s16 id) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    Setup_Eff70(ewk);
    ewk->wu.xyz[0].disp.pos = Face_Pos_Data[id][0] + 512;
    ewk->wu.xyz[1].disp.pos = Face_Pos_Data[id][1] + 0;
    ewk->wu.position_z = 62;
    ewk->wu.dir_step = id;
    ewk->wu.dir_timer = 10;

    if (permission_player[Present_Mode].ok[id] == 0) {
        ewk->wu.my_bright_type = 1;
        ewk->wu.my_bright_level = 7;
        ewk->wu.my_clear_level = 80;
    }

    return 0;
}

void Setup_Eff70(WORK_Other* ewk) {
    ewk->wu.be_flag = 1;
    ewk->wu.id = 70;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = 2;
    ewk->wu.char_index = 13;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
}
