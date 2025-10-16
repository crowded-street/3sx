/**
 * @file eff57.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff57.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff62.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/stage/bg.h"

void (*const EFF57_Jmp_Tbl[6])();

void effect_57_move(WORK_Other* ewk) {
    if (ewk->wu.routine_no[0] != Order[ewk->wu.dir_old]) {
        ewk->wu.routine_no[0] = Order[ewk->wu.dir_old];
        ewk->wu.routine_no[1] = 0;
    }

    EFF57_Jmp_Tbl[ewk->wu.routine_no[0]](ewk);

    if (ewk->wu.be_flag == 0) {
        return;
    }

    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

    if (ewk->wu.cg_number == 0x6F86) {
        dispSaveLoadTitle(ewk);
        return;
    }

    sort_push_request4(&ewk->wu);
}

void EFF57_WAIT(WORK_Other* ewk) {
    if ((ewk->wu.routine_no[0] = Order[ewk->wu.dir_old])) {
        ewk->wu.routine_no[1] = 0;
    }
}

void EFF57_SLIDE_IN(WORK_Other* ewk) {
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
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos - 384;
        ewk->wu.xyz[1].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 236;
        ewk->wu.position_z = 70;
        ewk->wu.hit_quake = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 0;
        ewk->wu.mvxy.a[0].sp = 0x100000;
        ewk->wu.mvxy.d[0].sp = 0x8000;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.xyz[0].cal += ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.a[0].sp += ewk->wu.mvxy.d[0].sp;

        if (0 < ewk->wu.mvxy.a[0].sp) {
            if (ewk->wu.hit_quake <= ewk->wu.xyz[0].disp.pos) {
                if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
                    Order[ewk->wu.dir_old] = 0;
                }

                ewk->wu.routine_no[0] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
            }
        } else {
            if (ewk->wu.hit_quake >= ewk->wu.xyz[0].disp.pos) {
                if (Order[ewk->wu.dir_old] == ewk->wu.routine_no[0]) {
                    Order[ewk->wu.dir_old] = 0;
                }

                ewk->wu.routine_no[0] = 0;
                ewk->wu.xyz[0].disp.pos = ewk->wu.hit_quake;
            }
        }

        break;
    }
}

void EFF57_CHAR_CHANGE(WORK_Other* ewk) {
    if (--Order_Timer[ewk->wu.dir_old] == 0) {
        ewk->wu.routine_no[0] = 0;
        Order[ewk->wu.dir_old] = 0;
        ewk->wu.dir_step = Order_Dir[ewk->wu.dir_old];
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
    }
}

void EFF57_WALL(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (--Order_Timer[ewk->wu.dir_old]) {
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 0;
        ewk->wu.xyz[1].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 16;
        ewk->wu.position_z = 78;
        ewk->wu.dir_step = Order_Dir[ewk->wu.dir_old];
        set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
        break;

    default:
        ewk->wu.routine_no[0] = 0;
        Order[ewk->wu.dir_old] = 0;
        break;
    }
}

void EFF57_SUDDENLY(WORK_Other* ewk) {
    if (--Order_Timer[ewk->wu.dir_old] != 0) {
        return;
    }

    ewk->wu.routine_no[0] = 0;
    Order[ewk->wu.dir_old] = 0;
    ewk->wu.disp_flag = 1;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 0;
    ewk->wu.xyz[1].disp.pos = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 236;

    if (ewk->master_priority != 999) {
        ewk->wu.position_z = 70;
    } else {
        ewk->wu.position_z = 30;
    }

    set_char_move_init2(&ewk->wu, 0, ewk->wu.char_index, ewk->wu.dir_step + 1, 0);
}

void EFF57_KILL(WORK_Other* ewk) {
    if (--Order_Timer[ewk->wu.dir_old] == 0) {
        push_effect_work(&ewk->wu);
    }
}

s32 effect_57_init(s16 dir_old, s16 ID, s16 Target_BG, s16 char_ix, s16 option) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 57;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.my_family = Target_BG + 1;
    *ewk->wu.char_table = _sel_pl_char_table;
    ewk->wu.dir_step = ID;
    ewk->wu.type = ID;
    ewk->wu.dir_old = dir_old;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.char_index = char_ix;
    ewk->master_priority = option;

    switch (option) {
    case 1:
        effect_58_init(4, 1, 78);
        break;

    case 2:
        effect_62_init(ewk, 0);
        break;
    }

    return 0;
}

void (*const EFF57_Jmp_Tbl[6])() = { EFF57_WAIT,     EFF57_SLIDE_IN, EFF57_CHAR_CHANGE,
                                     EFF57_SUDDENLY, EFF57_KILL,     EFF57_WALL };
