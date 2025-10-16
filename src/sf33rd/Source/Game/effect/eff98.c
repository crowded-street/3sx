/**
 * @file eff98.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff98.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/effect/eff80.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"

void (*const EFF98_Jmp_Tbl[5])();

void effect_98_move(WORK_Other* ewk) {
    EFF98_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    sort_push_request4(&ewk->wu);
}

void EFF98_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF98_SLIDE_IN(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.vital_new =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Plate_Pos_Data_79[1][ewk->master_id][0][0];
        ewk->wu.xyz[1].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Plate_Pos_Data_79[1][ewk->master_id][0][1];
        Stop_Cursor[ewk->master_id] = 1;
        Disp_Command_Name[ewk->master_id][ewk->master_player] = 1;
        effect_80_init(ewk, ewk->master_id, ewk->master_player, ewk->wu.my_family - 1);

        if (ewk->master_id == 0) {
            ewk->wu.xyz[0].disp.pos = 0xF0;
            ewk->wu.mvxy.a[0].sp = 0xF0000;
            ewk->wu.mvxy.d[0].sp = 0x8000;
        } else {
            ewk->wu.xyz[0].disp.pos = 0x310;
            ewk->wu.mvxy.a[0].sp = -0xF0000;
            ewk->wu.mvxy.d[0].sp = -0x8000;
        }

        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (0 < ewk->wu.mvxy.a[0].sp) {
            if (ewk->wu.vital_new <= ewk->wu.xyz[0].disp.pos) {
                Order[ewk->wu.dir_old] = 0;
                ewk->wu.routine_no[0] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.vital_new;
            }
        } else if (ewk->wu.vital_new >= ewk->wu.xyz[0].disp.pos) {
            Order[ewk->wu.dir_old] = 0;
            ewk->wu.routine_no[0] = 0;
            ewk->wu.xyz[0].disp.pos = ewk->wu.vital_new;
        }

        break;
    }
}

void EFF98_SLIDE_OUT(WORK_Other* /* unused */) {}

void EFF98_SUDDENLY(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[0].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Plate_Pos_Data_79[1][ewk->master_id][0][0];
        ewk->wu.xyz[1].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Plate_Pos_Data_79[1][ewk->master_id][0][1];
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.routine_no[0] = 0;
        Order[ewk->wu.dir_old] = 0;
        break;
    }
}

void EFF98_KILL(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] == 0) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_98_init(s16 PL_id, s16 dir_old, s16 master_player, s16 Target_BG) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->master_player = master_player;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 98;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = Target_BG + 1;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->master_id = PL_id;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.dir_old = dir_old;
    ewk->wu.char_index = 14;
    ewk->wu.dir_step = 30;
    ewk->wu.position_z = 35;
    return 0;
}

void (*const EFF98_Jmp_Tbl[5])() = { EFF98_WAIT, EFF98_SLIDE_IN, EFF98_SLIDE_OUT, EFF98_SUDDENLY, EFF98_KILL };
