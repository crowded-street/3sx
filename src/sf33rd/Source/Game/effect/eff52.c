/**
 * @file eff52.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff52.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/texcash.h"

void Setup_Char_52(WORK_Other* ewk);
void Setup_Pos_52(WORK_Other* ewk);

void (*const EFF52_Jmp_Tbl[5])();

void effect_52_move(WORK_Other* ewk) {
    EFF52_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag != 0) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
    }
}

void EFF52_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[6] = 0;
    }
}

void EFF52_SUDDENLY(WORK_Other* ewk) {
    s16 x;

    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] != 0) {
            break;
        }

        ewk->wu.routine_no[6]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.position_z = 72;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    case 1:
        char_move(&ewk->wu);
        x = ewk->wu.cg_ix / ewk->wu.cgd_type;

        if (x < ewk->wu.direction) {
            break;
        }

        ewk->wu.routine_no[6]++;

    case 2:
        if (Select_Start[ewk->master_id] != 0) {
            break;
        }

        Order[ewk->wu.dir_old] = 4;
        ewk->wu.routine_no[0] = 4;
        ewk->wu.routine_no[1] = 0;
        Order_Timer[ewk->wu.dir_old] = 1;
        break;
    }
}

void EFF52_SLIDE_IN(WORK_Other* ewk) {
    if (Order[ewk->wu.dir_old] == 4) {
        ewk->wu.routine_no[0] = 4;
        ewk->wu.routine_no[1] = 0;
        return;
    }

    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] == 0) {
            ewk->wu.routine_no[6]++;
            ewk->wu.disp_flag = 1;
            set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        }

        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (0 < ewk->wu.mvxy.a[0].sp) {
            if (ewk->wu.hit_quake <= ewk->wu.xyz[0].disp.pos) {
                ewk->wu.routine_no[0] = 0;
                Order[ewk->wu.dir_old] = 0;
                ewk->wu.routine_no[6] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
            }
        } else if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
            ewk->wu.routine_no[0] = 0;
            Order[ewk->wu.dir_old] = 0;
            ewk->wu.routine_no[6] = 0;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
        }

        break;
    }
}

void EFF52_SLIDE_OUT(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (ewk->wu.disp_flag == 0) {
            ewk->wu.routine_no[1] = 99;
        } else if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        } else {
            ewk->wu.routine_no[6]++;
        }

        if (Order_Dir[ewk->wu.dir_old] == 4) {
            ewk->wu.mvxy.a[0].sp = -0xF0000;
            ewk->wu.mvxy.d[0].sp = 0;
        } else {
            ewk->wu.mvxy.a[0].sp = 0xF0000;
            ewk->wu.mvxy.d[0].sp = 0;
        }

        break;

    case 1:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (Ck_Range_Out_S(ewk, 2, 128)) {
            ewk->wu.routine_no[6]++;
            ewk->wu.disp_flag = 0;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void EFF52_KILL(WORK_Other* ewk) {
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

s32 effect_52_init(s16 PL_id, s16 dir_old) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 52;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = 3;
    ewk->wu.dir_step = 1;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->master_id = PL_id;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.dir_old = dir_old;
    ewk->wu.dir_timer = Order_Timer[dir_old];
    ewk->wu.rl_flag = PL_id;
    ewk->wu.position_z = 74;
    Setup_Char_52(ewk);
    Setup_Pos_52(ewk);
    return 0;
}

void Setup_Char_52(WORK_Other* ewk) {
    s16 ix;

    if (ewk->wu.dir_old == 37) {
        ewk->wu.char_index = 16;
        return;
    }

    ewk->wu.char_index = 18;
    ewk->wu.dir_step = ID_of_Face[Cursor_Y[ewk->master_id]][Cursor_X[ewk->master_id]];
    ix = chkNameAkuma(ewk->wu.dir_step, 6);
    ewk->wu.dm_vital = Pattern_Data_52[ewk->wu.dir_step + ix][0];
    ewk->wu.direction = Pattern_Data_52[ewk->wu.dir_step + ix][1];
}

void Setup_Pos_52(WORK_Other* ewk) {
    s16 ix;

    if (ewk->wu.dir_old == 37) {
        ewk->wu.xyz[1].disp.pos = 208;

        if (ewk->master_id) {
            ewk->wu.xyz[0].disp.pos = 720;
            ewk->wu.hit_quake = 502;
            ewk->wu.mvxy.a[0].sp = -0x11FFFD;
            ewk->wu.mvxy.d[0].sp = -0x10000;
        } else {
            ewk->wu.xyz[0].disp.pos = 304;
            ewk->wu.hit_quake = 520;
            ewk->wu.mvxy.a[0].sp = 0x11FFFD;
            ewk->wu.mvxy.d[0].sp = 0x10000;
        }
    } else {
        ix = chkNameAkuma(ewk->wu.dir_step, 6);
        ewk->wu.xyz[0].disp.pos = Pos_Data_52[ewk->master_id][ewk->wu.dir_step + ix][0] + 512;
        ewk->wu.xyz[1].disp.pos = Pos_Data_52[ewk->master_id][ewk->wu.dir_step + ix][1] + 0;
    }
}

void (*const EFF52_Jmp_Tbl[5])() = { EFF52_WAIT, EFF52_SLIDE_IN, EFF52_SLIDE_OUT, EFF52_SUDDENLY, EFF52_KILL };
