/**
 * @file effm0.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effm0.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

void animal_init(WORK_Other* ewk);
void animal_control(WORK_Other* ewk);
void animal_0000(WORK_Other* ewk);
void animal_0001(WORK_Other* ewk);
void animal_0002(WORK_Other* ewk);
void animal_0004(WORK_Other* ewk);
void animal_0005(WORK_Other* ewk);
void mouse_run_set(WORK_Other* ewk);
void mouse_stand_set(WORK_Other* ewk);
void cat_run_set(WORK_Other* ewk);
void don_run_set(WORK_Other* ewk);

const s16 animal_0005_tbl[16] = { 40, 50, 160, 70, 80, 100, 30, 200, 340, 10, 110, 18, 40, 60, 30, 150 };

void effect_M0_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        animal_init(ewk);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            animal_control(ewk);
        }

        disp_pos_trans_entry(ewk);
        break;

    case 99:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void animal_init(WORK_Other* ewk) {
    s16 work_l = bg_w.bgw[1].wxy[0].disp.pos - bg_w.pos_offset;
    s16 work_r = bg_w.bgw[1].wxy[0].disp.pos + bg_w.pos_offset;

    switch (ewk->wu.type) {
    case 0:
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = -4;
        ewk->wu.kage_hy = 33;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 3;

        if (ewk->wu.rl_flag) {
            ewk->wu.xyz[0].disp.pos = work_l;
            break;
        }

        ewk->wu.xyz[0].disp.pos = work_r;
        break;

    case 2:
        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = -4;
        ewk->wu.kage_hy = 33;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 3;
        ewk->wu.rl_flag ^= 1;
        break;

    case 6:
        if (plw->player_number == 7) {
            ewk->wu.my_col_code = plw->wu.my_col_code;
        } else {
            ewk->wu.my_col_code = plw[1].wu.my_col_code;
        }

        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = -2;
        ewk->wu.kage_hy = 24;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 8;

        if (ewk->wu.rl_flag) {
            ewk->wu.xyz[0].disp.pos = work_l;
            break;
        }

        ewk->wu.xyz[0].disp.pos = work_r;
        break;

    default:
        if (ewk->wu.rl_flag) {
            ewk->wu.xyz[0].disp.pos = work_r;
        } else {
            ewk->wu.xyz[0].disp.pos = work_l;
        }

        ewk->wu.kage_flag = 1;
        ewk->wu.kage_hx = 0;
        ewk->wu.kage_hy = 33;
        ewk->wu.kage_prio = 71;
        ewk->wu.kage_char = 0;
        ewk->wu.rl_flag ^= 1;
        break;
    }
}

void animal_control(WORK_Other* ewk) {
    void (*animal_jp[7])(WORK_Other* ewk) = { animal_0000, animal_0001, animal_0002, animal_0001,
                                              animal_0004, animal_0005, animal_0000 };
    animal_jp[ewk->wu.type](ewk);
}

void animal_0000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;

        if (ewk->wu.type == 6) {
            ewk->wu.old_rno[0] = 10;
            don_run_set(ewk);
            ewk->wu.xyz[1].disp.pos = 24;
        } else {
            ewk->wu.old_rno[0] = 60;
            cat_run_set(ewk);
            ewk->wu.xyz[1].disp.pos = 32;
        }

        ewk->wu.my_priority = ewk->wu.position_z = 27;
        break;

    case 1:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        add_x_sub(&ewk->wu);
        char_move(&ewk->wu);

        if (!range_x_check3(ewk, 48)) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 3:
        add_x_sub(&ewk->wu);
        char_move(&ewk->wu);
        ewk->wu.routine_no[0] = 99;
        ewk->wu.routine_no[1]++;
        break;
    }
}

void animal_0001(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.old_rno[0] = 30;
        ewk->wu.disp_flag = 1;
        mouse_run_set(ewk);
        ewk->wu.xyz[1].disp.pos = 32;
        ewk->wu.my_priority = ewk->wu.position_z = 27;
        break;

    case 1:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        add_x_sub(&ewk->wu);
        char_move(&ewk->wu);

        if (!range_x_check3(ewk, 32)) {
            ewk->wu.routine_no[0] = 99;
            ewk->wu.routine_no[1]++;
        }

        break;
    }
}

void animal_0002(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, 47);
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos;
        ewk->wu.xyz[1].disp.pos = 32;
        ewk->wu.my_priority = ewk->wu.position_z = 27;
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1]++;
            ewk->wu.rl_flag ^= 1;
            cat_run_set(ewk);
        }

        break;

    case 2:
        add_x_sub(&ewk->wu);
        char_move(&ewk->wu);

        if (!range_x_check3(ewk, 32)) {
            ewk->wu.routine_no[0] = 99;
            ewk->wu.routine_no[1]++;
        }

        break;
    }
}

void animal_0004(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.old_rno[0] = 30;
        ewk->wu.disp_flag = 1;
        mouse_stand_set(ewk);
        ewk->wu.xyz[1].disp.pos = 32;
        ewk->wu.my_priority = ewk->wu.position_z = 27;

        if (ewk->wu.rl_flag) {
            ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos + 48;
            break;
        }

        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos - 48;
        break;

    case 1:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1]++;
            mouse_run_set(ewk);
        }

        break;

    case 3:
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);

        if (!range_x_check3(ewk, 32)) {
            ewk->wu.routine_no[0] = 99;
            ewk->wu.routine_no[1]++;
        }

        break;
    }
}

void animal_0005(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.old_rno[0] = 30;
        ewk->wu.disp_flag = 1;
        mouse_run_set(ewk);
        ewk->wu.xyz[1].disp.pos = 32;
        ewk->wu.my_priority = ewk->wu.position_z = 27;
        ewk->wu.old_rno[2] = random_16();
        ewk->wu.old_rno[3] = animal_0005_tbl[ewk->wu.old_rno[2]];
        break;

    case 1:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
        }

        break;

    case 2:
        char_move(&ewk->wu);
        ewk->wu.old_rno[3]--;

        if (ewk->wu.old_rno[3] <= 0) {
            ewk->wu.routine_no[1]++;
            mouse_stand_set(ewk);
            break;
        }

        add_x_sub(&ewk->wu);
        break;

    case 3:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1]++;
            mouse_run_set(ewk);
        }

        break;

    case 4:
        add_x_sub(&ewk->wu);
        char_move(&ewk->wu);

        if (!range_x_check3(ewk, 32)) {
            ewk->wu.routine_no[0] = 99;
            ewk->wu.routine_no[1]++;
        }

        break;
    }
}

void mouse_run_set(WORK_Other* ewk) {
    set_char_move_init(&ewk->wu, 0, 25);

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = -0x30000;
    } else {
        ewk->wu.mvxy.a[0].sp = 0x30000;
    }

    ewk->wu.mvxy.d[0].sp = 0;
}

void mouse_stand_set(WORK_Other* ewk) {
    set_char_move_init(&ewk->wu, 0, 18);
    ewk->wu.mvxy.a[0].sp = 0;
    ewk->wu.mvxy.d[0].sp = 0;
}

void cat_run_set(WORK_Other* ewk) {
    set_char_move_init(&ewk->wu, 0, 26);

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = 0x38000;
    } else {
        ewk->wu.mvxy.a[0].sp = -0x38000;
    }

    ewk->wu.mvxy.d[0].sp = 0;
}

void cat_run_set2(WORK_Other* ewk) {
    set_char_move_init(&ewk->wu, 0, 26);

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = 0x2C000;
    } else {
        ewk->wu.mvxy.a[0].sp = -0x2C000;
    }

    ewk->wu.mvxy.d[0].sp = 0;
}

void cat_walk_set(WORK_Other* ewk) {
    set_char_move_init(&ewk->wu, 0, 27);

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = 0x18000;
    } else {
        ewk->wu.mvxy.a[0].sp = -0x18000;
    }

    ewk->wu.mvxy.d[0].sp = 0;
}

void don_run_set(WORK_Other* ewk) {
    set_char_move_init(&ewk->wu, 0, 52);

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = 0x48000;
    } else {
        ewk->wu.mvxy.a[0].sp = -0x48000;
    }

    ewk->wu.mvxy.d[0].sp = 0;
}

s32 effect_M0_init(u8 pl_rl, u8 animal_type) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 220;
    ewk->wu.work_id = 16;
    ewk->wu.type = animal_type;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.char_table[0] = _etc2_char_table;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = 58;
    ewk->wu.rl_flag = pl_rl;
    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
