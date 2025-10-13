/**
 * @file eff39.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff39.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/effect/eff59.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void EFF39_WAIT(WORK_Other* ewk);
void EFF39_SUDDENLY(WORK_Other* ewk);
void EFF39_SLIDE_IN(WORK_Other* ewk);
void EFF39_SLIDE_OUT(WORK_Other* ewk);
void EFF39_KILL(WORK_Other* ewk);
void EFF39_MOVE(WORK_Other* ewk);
s32 Get_Pos39(WORK_Other* ewk, s16 Who, s16 Get_Type);

void (*const EFF39_Jmp_Tbl[6])() = {
    EFF39_WAIT, EFF39_SLIDE_IN, EFF39_SLIDE_OUT, EFF39_SUDDENLY, EFF39_MOVE, EFF39_KILL
};

void effect_39_move(WORK_Other* ewk) {
    EFF39_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag != 0) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        sort_push_request4(&ewk->wu);
    }
}

void EFF39_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[6] = 0;
    }
}

void EFF39_SUDDENLY(WORK_Other* ewk) {
    if (--Order_Timer[ewk->wu.dir_old] != 0) {
        return;
    }

    ewk->wu.disp_flag = 1;
    Order[ewk->wu.dir_old] = 0;
    ewk->wu.routine_no[0] = 0;
    ewk->wu.routine_no[6] = 0;
    ewk->wu.xyz[0].disp.pos =
        bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Name_Pos_Data[ewk->master_id][1][ewk->wu.dir_step][0];
    ewk->wu.xyz[1].disp.pos =
        bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Name_Pos_Data[ewk->master_id][1][ewk->wu.dir_step][1];
    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
}

void EFF39_SLIDE_IN(WORK_Other* ewk) {
    if (Order[ewk->wu.dir_old] == 5) {
        ewk->wu.routine_no[0] = 5;
        ewk->wu.routine_no[1] = 0;
        return;
    }

    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old] != 0) {
            break;
        }

        ewk->wu.routine_no[6]++;
        ewk->wu.disp_flag = 1;

        if (ewk->master_id) {
            ewk->wu.mvxy.a[0].sp = -0xF0000;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.hit_quake = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Get_Pos39(ewk, ewk->wu.dir_step, 0);
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake + 256;
            ewk->wu.xyz[1].disp.pos =
                bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Get_Pos39(ewk, ewk->wu.dir_step, 1);
        } else {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 272;
            ewk->wu.mvxy.a[0].sp = 0xF0000;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.hit_quake = Get_Pos39(ewk, ewk->wu.dir_step, 0) + 512;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake - 256;
            ewk->wu.xyz[1].disp.pos =
                bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Get_Pos39(ewk, ewk->wu.dir_step, 1);
        }

        set_char_move_init2(&ewk->wu, 0, (s16)(ewk->wu.char_index), (ewk->wu.dir_step) + 1, 0);
        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (0 < ewk->wu.mvxy.a[0].sp) {
            if (ewk->wu.hit_quake <= ewk->wu.xyz[0].disp.pos) {
                ewk->wu.routine_no[0] = 4;
                Order[ewk->wu.dir_old] = 4;
                ewk->wu.routine_no[6] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;

                if (--Select_Start[ewk->master_id] < 0) {
                    Select_Start[ewk->master_id] = 0;
                }
            }

            break;
        }

        if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
            ewk->wu.routine_no[0] = 4;
            Order[ewk->wu.dir_old] = 4;
            ewk->wu.routine_no[6] = 0;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;

            if (--Select_Start[ewk->master_id] < 0) {
                Select_Start[ewk->master_id] = 0;
            }
        }

        break;
    }
}

void EFF39_SLIDE_OUT(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[6]) {
    case 0:
        if (ewk->wu.disp_flag == 0) {
            ewk->wu.routine_no[1] = 99;
        } else {
            if (--Order_Timer[ewk->wu.dir_old]) {
                break;
            }

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

        if (Ck_Range_Out_S(ewk, ewk->wu.my_family - 1, 48)) {
            ewk->wu.routine_no[6]++;
            ewk->wu.disp_flag = 0;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void EFF39_KILL(WORK_Other* ewk) {
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

void EFF39_MOVE(WORK_Other* ewk) {
    if (Order[ewk->wu.dir_old] != 4) {
        ewk->wu.routine_no[0] = Order[ewk->wu.dir_old];
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[6] = 0;
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (Sel_PL_Complete[ewk->master_id] || plw[ewk->master_id].wu.operator == 0) {
            ewk->wu.routine_no[1] = 2;
        } else {
            ewk->wu.routine_no[1]++;
        }

        /* fallthrough */

    case 1:
        if (ewk->wu.dir_step != ID_of_Face[Cursor_Y[ewk->master_id]][Cursor_X[ewk->master_id]]) {
            ewk->wu.dir_step = ID_of_Face[Cursor_Y[ewk->master_id]][Cursor_X[ewk->master_id]];
            ewk->wu.dir_step += chkNameAkuma(ewk->wu.dir_step, 9);
            ewk->wu.xyz[0].disp.pos =
                bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Get_Pos39(ewk, ewk->wu.dir_step, 0);
            ewk->wu.xyz[1].disp.pos =
                bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Get_Pos39(ewk, ewk->wu.dir_step, 1);
            set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        }

        if (Sel_PL_Complete[ewk->master_id]) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        break;
    }
}

s32 effect_39_init(s16 PL_id, s16 dir_old, s16 Your_Char, s16 Target_BG, s16 Option) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 39;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x2090;
    ewk->wu.my_family = Target_BG + 1;
    ewk->wu.position_z = 71;
    ewk->wu.char_index = 1;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->master_id = PL_id;
    ewk->wu.dir_old = dir_old;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    if (Your_Char == 0x7F) {
        ewk->wu.dir_step = ID_of_Face[Cursor_Y[ewk->master_id]][Cursor_X[ewk->master_id]];
    } else {
        ewk->wu.dir_step = Your_Char;
    }

    ewk->wu.dir_step += chkNameAkuma(ewk->wu.dir_step, 9);
    ewk->wu.dir_step += chkNameSuv(ewk->wu.dir_step, 2);

    if (Option == 1) {
        effect_59_init(ewk, ewk->wu.my_family, 5, 1);
    }

    return 0;
}

s32 Get_Pos39(WORK_Other* ewk, s16 Who, s16 Get_Type) {
    return Name_Pos_Data[ewk->master_id][Play_Type][Who][Get_Type];
}
