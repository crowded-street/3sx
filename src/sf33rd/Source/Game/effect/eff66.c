/**
 * @file eff66.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff66.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

void EFF66_WAIT(WORK_Other* ewk);
void EFF66_SUSPEND(WORK_Other* ewk);
void EFF66_SLIDE_IN(WORK_Other* ewk);
void EFF66_BOWAN(WORK_Other* ewk);
void EFF66_FLASH(WORK_Other* ewk);
void EFF66_SUDDENLY(WORK_Other* ewk);
void Setup_Pos_66(WORK_Other* ewk);

const u8 Flash_Data_66[14] = { 0, 128, 255, 128, 0, 128, 255, 128, 0, 128, 255, 128, 0, 128 };

const s16 EFF66_Half_OBJ_Data[15][7] = {
    { -192, 384, 16, 48, 0, 128, 1 },  { -144, 144, 12, 156, 1, 42, 1 },  { -64, 128, 0, 96, 0, 128, 1 },
    { -184, 368, -40, 208, 1, 77, 1 }, { 120, 144, 40, 80, 1, 42, 2 },    { 128, 144, -26, 180, 0, 128, 1 },
    { -168, 336, -16, 80, 0, 128, 1 }, { 0, 160, 0, 80, 0, 128, 1 },      { -200, 400, 0, 256, 0, 179, 1 },
    { -72, 144, 0, 96, 1, 93, 1 },     { -72, 144, 0, 96, 0, 128, 1 },    { 200, 368, -40, 156, 1, 77, 1 },
    { -184, 368, -40, 156, 1, 77, 1 }, { -184, 368, -54, 160, 1, 64, 1 }, { -176, 352, -32, 192, 1, 77, 1 }
};

void (*const EFF66_Jmp_Tbl[7])();

void effect_66_move(WORK_Other* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->wu.routine_no[0] != Order[ewk->wu.dir_old]) {
        ewk->wu.routine_no[0] = Order[ewk->wu.dir_old];
        ewk->wu.routine_no[1] = 0;
    }

    Check_Pos_OBJ(ewk);
    EFF66_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag == 0) {
        return;
    }

    if (ewk->master_priority != -0x3FFC && ewk->master_priority != -0x3FFB && ewk->master_priority != -0x3FF6) {
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
    }

    if (ewk->master_priority & 0x4000) {
        if (Pause_Down) {
            sort_push_requestB(&ewk->wu);
        }

        return;
    } else if (ewk->master_priority & 0x8000) {
        sort_push_requestA(&ewk->wu);
    } else {
        sort_push_request4(&ewk->wu);
    }
}

void EFF66_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF66_SUSPEND(WORK_Other* ewk) {
    ewk->wu.disp_flag = 0;

    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF66_SLIDE_IN(WORK_Other* ewk) {
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
        ewk->wu.disp_flag = ewk->wu.rl_waza;
        ewk->wu.xyz[0].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_66[ewk->wu.type - 7][0] + 384;
        ewk->wu.xyz[1].disp.pos =
            bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Slide_Pos_Data_66[ewk->wu.type - 7][1];
        ewk->wu.position_z = Slide_Pos_Data_66[ewk->wu.type - 7][2];
        ewk->wu.hit_quake = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Slide_Pos_Data_66[ewk->wu.type - 7][0];
        ewk->wu.mvxy.a[0].sp = -0x400000;
        ewk->wu.mvxy.d[0].sp = 0x50000;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
            if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
                Order[ewk->wu.dir_old] = 0;
            }

            ewk->wu.routine_no[0] = 0;
            ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
        }

        break;
    }
}

void EFF66_BOWAN(WORK_Other* ewk) {
    if (ewk->wu.routine_no[0] != Order[ewk->wu.dir_old]) {
        ewk->wu.routine_no[0] = Order[ewk->wu.dir_old];
        ewk->wu.routine_no[1] = 0;
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        Setup_Pos_66(ewk);
        break;

    case 1:
        ewk->wu.my_clear_level -= 8;

        if (ewk->wu.my_clear_level < 0) {
            ewk->wu.routine_no[1]++;
            ewk->wu.my_clear_level = 0;
        }

        break;

    case 2:
        ewk->wu.my_clear_level += 8;

        if (ewk->wu.my_clear_level > 127) {
            ewk->wu.routine_no[1]--;
            ewk->wu.my_clear_level = 127;
        }

        break;
    }
}

void EFF66_FLASH(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        Setup_Pos_66(ewk);
        ewk->wu.dir_timer = 1;
        ewk->wu.dmcal_m = 0;
        /* fallthrough */

    case 1:
        if (--ewk->wu.dir_timer != 0) {
            break;
        }

        ewk->wu.my_clear_level = Flash_Data_66[ewk->wu.dmcal_m];

        if (++ewk->wu.dmcal_m > 13) {
            Order[ewk->wu.dir_old] = 0;
            ewk->wu.routine_no[0] = 0;
        } else {
            ewk->wu.dir_timer = 1;
        }

        break;
    }
}

void EFF66_SUDDENLY(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        Setup_Pos_66(ewk);
        break;

    default:
        ewk->wu.routine_no[0] = 0;
        Order[ewk->wu.dir_old] = 0;
        break;
    }
}

void Setup_Pos_66(WORK_Other* ewk) {
    if (--Order_Timer[ewk->wu.dir_old]) {
        return;
    }

    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = ewk->wu.rl_waza;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Suddenly_Pos_Data_66[ewk->wu.type][0];
    ewk->wu.xyz[1].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Suddenly_Pos_Data_66[ewk->wu.type][1];
    ewk->wu.position_z = Suddenly_Pos_Data_66[ewk->wu.type][2];

    if (ewk->master_priority & 0x4000) {
        ewk->wu.position_x = 0;
        ewk->wu.position_y = 0x28;

        if (ewk->master_priority == -0x3FFB) {
            if (ewk->wu.type == 10) {
                ewk->wu.position_x = -0x60;
            } else {
                ewk->wu.position_x = 0x50;
            }
        }

        if (ewk->master_priority == -0x3FF6) {
            ewk->wu.position_x = 192;
            ewk->wu.position_y = 76;
            ewk->wu.position_z = 3;
        }
    }

    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
}

s32 effect_66_init(s16 order_index, s16 id, s16 master_player, s16 target_bg, s16 char_ix, s16 char_ix2, s16 option) {
    WORK_Other* ewk;
    s16 ix;
    s16 cg_type;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 66;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.my_family = target_bg + 1;
    ewk->wu.rl_waza = 1;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.dir_old = order_index;
    ewk->wu.dir_step = char_ix2;
    ewk->wu.type = id;
    ewk->master_player = master_player;
    ewk->wu.char_index = char_ix;
    ewk->master_priority = option;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    switch (option) {
    case 1:
        ewk->wu.my_clear_level = 0x80;
        return 0;
    }

    if (ewk->master_priority & 0x8000) {
        cg_type = ewk->master_priority & 0x3FFF;

        switch (cg_type) {
        case 9:
        case 10:
        case 13:
            ewk->wu.my_col_code = 3;
            break;

        default:
            ewk->wu.my_col_code = 1;
            break;
        }

        ewk->wu.shell_ix[0] = EFF66_Half_OBJ_Data[cg_type][0];
        ewk->wu.shell_ix[1] = EFF66_Half_OBJ_Data[cg_type][1];
        ewk->wu.shell_ix[2] = EFF66_Half_OBJ_Data[cg_type][2];
        ewk->wu.shell_ix[3] = EFF66_Half_OBJ_Data[cg_type][3];
        ewk->wu.charset_id = EFF66_Half_OBJ_Data[cg_type][4];
        ewk->wu.my_clear_level = EFF66_Half_OBJ_Data[cg_type][5];
        ewk->wu.rl_waza = EFF66_Half_OBJ_Data[cg_type][6];
    }

    return 0;
}

void (*const EFF66_Jmp_Tbl[7])() = { EFF66_WAIT, EFF66_SLIDE_IN, EFF66_BOWAN, EFF66_SUDDENLY,
                                     EFF57_KILL, EFF66_SUSPEND,  EFF66_FLASH };
