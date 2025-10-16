/**
 * @file effj8.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effj8.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

void effect_J8_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
            break;
        }

        if (!EXE_flag && !Game_pause && !EXE_obroll) {
            dragonfly_move(ewk);
        }

        disp_pos_trans_entry(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

void dragonfly_move(WORK_Other* ewk) {
    void (*const dragonfly_move_jp1[8])(WORK_Other*) = { dragonfly_move_0000, dragonfly_move_0001, dragonfly_r_move,
                                                         dragonfly_l_move,    dragonfly_move_0004, dragonfly_move_0005,
                                                         dragonfly_l_move,    dragonfly_r_move };
    dragonfly_move_jp1[ewk->wu.routine_no[1]](ewk);
}

void dragonfly_l_move_0(WORK_Other* ewk) {
    s16 work;

    ewk->wu.rl_flag = 1;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].r_limit2 + bg_w.pos_offset + 32;
    work = random_16();
    work &= 7;
    ewk->wu.xyz[1].disp.pos = effj8_y_tbl[work];
    ewk->wu.xyz[1].disp.low = 0;
}

void dragonfly_l_move_1(WORK_Other* ewk) {
    char_move(&ewk->wu);
    dragonfly_line_set(ewk, 0);
}

s16 dragonfly_l_move_2(WORK_Other* ewk) {
    char_move(&ewk->wu);
    add_x_sub(&ewk->wu);
    add_y_sub(&ewk->wu);

    if (ewk->wu.xyz[0].disp.pos <= bg_w.bgw[1].l_limit2 - bg_w.pos_offset - 24) {
        dragonfly_stop_timer(ewk);
        return 99;
    }

    if (ewk->wu.mvxy.a[0].sp < -0x2FFFF) {
        return 1;
    }

    return 0;
}

s16 dragonfly_l_move_3(WORK_Other* ewk) {
    add_x_sub(&ewk->wu);
    add_y_sub(&ewk->wu);

    if (ewk->wu.xyz[0].disp.pos <= bg_w.bgw[1].l_limit2 - bg_w.pos_offset - 24) {
        dragonfly_stop_timer(ewk);
        set_char_move_init(&ewk->wu, 0, 6);
        return 99;
    }

    if (ewk->wu.mvxy.a[0].sp >= 0) {
        dragonfly_stop_timer(ewk);
        set_char_move_init(&ewk->wu, 0, 6);
        return 1;
    }

    return 0;
}

s16 dragonfly_l_move_4(WORK_Other* ewk) {
    char_move(&ewk->wu);
    ewk->wu.dir_timer--;

    if (ewk->wu.dir_timer <= 0) {
        set_char_move_init(&ewk->wu, 0, 4);
        return 1;
    }

    return 0;
}

void dragonfly_l_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        dragonfly_l_move_0(ewk);
        /* fallthrough */

    case 1:
        ewk->wu.routine_no[2]++;
        dragonfly_l_move_1(ewk);
        break;

    case 2:
        ewk->wu.routine_no[2] += dragonfly_l_move_2(ewk);
        break;

    case 3:
        ewk->wu.routine_no[2] += dragonfly_l_move_3(ewk);
        break;

    case 4:
        char_move(&ewk->wu);

        if (dragonfly_l_move_4(ewk)) {
            ewk->wu.routine_no[2] = 1;
            break;
        }

        break;

    default:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            dragonfly_move_next(ewk);
            set_char_move_init(&ewk->wu, 0, 4);
        }

        break;
    }
}

void dragonfly_r_move_0(WORK_Other* ewk) {
    s16 work;

    ewk->wu.rl_flag = 0;
    ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].l_limit2 - bg_w.pos_offset - 32;
    work = random_16();
    work &= 7;
    ewk->wu.xyz[1].disp.pos = effj8_y_tbl[work];
    ewk->wu.xyz[1].disp.low = 0;
}

void dragonfly_r_move_1(WORK_Other* ewk) {
    char_move(&ewk->wu);
    dragonfly_line_set(ewk, 1);
}

s16 dragonfly_r_move_2(WORK_Other* ewk) {
    char_move(&ewk->wu);
    add_x_sub(&ewk->wu);
    add_y_sub(&ewk->wu);

    if (ewk->wu.xyz[0].disp.pos > bg_w.bgw[1].r_limit2 + bg_w.pos_offset + 24) {
        dragonfly_stop_timer(ewk);
        return 99;
    }

    if (ewk->wu.mvxy.a[0].sp < 0x30001) {
        return 1;
    }

    return 0;
}

s16 dragonfly_r_move_3(WORK_Other* ewk) {
    add_x_sub(&ewk->wu);
    add_y_sub(&ewk->wu);

    if (ewk->wu.xyz[0].disp.pos >= bg_w.bgw[1].r_limit2 + bg_w.pos_offset + 24) {
        dragonfly_stop_timer(ewk);
        set_char_move_init(&ewk->wu, 0, 6);
        return 99;
    }

    if (ewk->wu.mvxy.a[0].sp <= 0) {
        dragonfly_stop_timer(ewk);
        set_char_move_init(&ewk->wu, 0, 6);
        return 1;
    }

    return 0;
}

s16 dragonfly_r_move_4(WORK_Other* ewk) {
    char_move(&ewk->wu);
    ewk->wu.dir_timer--;

    if (ewk->wu.dir_timer <= 0) {
        set_char_move_init(&ewk->wu, 0, 4);
        return 1;
    }

    return 0;
}

void dragonfly_r_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        dragonfly_r_move_0(ewk);
        /* fallthrough */

    case 1:
        ewk->wu.routine_no[2]++;
        dragonfly_r_move_1(ewk);
        break;

    case 2:
        ewk->wu.routine_no[2] += dragonfly_r_move_2(ewk);
        break;

    case 3:
        ewk->wu.routine_no[2] += dragonfly_r_move_3(ewk);
        break;

    case 4:
        char_move(&ewk->wu);

        if (dragonfly_r_move_4(ewk)) {
            ewk->wu.routine_no[2] = 1;
        }

        break;

    default:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            dragonfly_move_next(ewk);
            set_char_move_init(&ewk->wu, 0, 4);
        }

        break;
    }
}

void dragonfly_move_0000(WORK_Other* ewk) {
    char_move(&ewk->wu);

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.rl_flag = 1;
        ewk->wu.xyz[0].disp.pos = bg_w.bgw[1].r_limit2 + bg_w.pos_offset + 32;
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.xyz[1].disp.pos = 107;
        ewk->wu.xyz[1].disp.low = 0;
        dragonfly_stop_timer(ewk);
        break;

    case 1:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            ewk->wu.routine_no[2]++;
            ewk->wu.dir_timer = 150;
            ewk->wu.mvxy.a[0].sp = -0x32C5F;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = -0x1B4E;
            ewk->wu.mvxy.d[1].sp = 0;
        }

        break;

    case 2:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            dragonfly_move_next(ewk);
            ewk->wu.xyz[0].disp.pos = 512;
            ewk->wu.xyz[0].disp.low = 0;
            ewk->wu.xyz[1].disp.pos = 91;
            ewk->wu.xyz[1].disp.low = 0;
            break;
        }

        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        break;
    }
}

const s16 effj8_timer_tbl[8] = { 60, 120, 180, 90, 150, 30, 220, 160 };

const s16 effj8_y_tbl[8] = { 128, 80, 96, 160, 176, 112, 144, 168 };

void dragonfly_move_0001(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        char_move(&ewk->wu);
        ewk->wu.routine_no[2]++;
        ewk->wu.rl_flag = 1;
        dragonfly_stop_timer(ewk);
        break;

    case 1:
        char_move(&ewk->wu);
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            ewk->wu.routine_no[2]++;
            break;
        }

        break;

    case 2:
        ewk->wu.routine_no[2]++;
        dragonfly_l_move_1(ewk);
        /* fallthrough */

    case 3:
        ewk->wu.routine_no[2] += dragonfly_l_move_2(ewk);
        break;

    case 4:
        ewk->wu.routine_no[2] += dragonfly_l_move_3(ewk);
        break;

    case 5:
        char_move(&ewk->wu);

        if (dragonfly_l_move_4(ewk)) {
            ewk->wu.routine_no[2] = 2;
            break;
        }

        break;

    default:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            dragonfly_move_next(ewk);
            set_char_move_init(&ewk->wu, 0, 4);
        }

        break;
    }
}

void dragonfly_move_0004(WORK_Other* ewk) {
    char_move(&ewk->wu);

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.rl_flag = 0;
        dragonfly_stop_timer(ewk);
        break;

    case 1:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            ewk->wu.routine_no[2]++;
            ewk->wu.dir_timer = 180;
            ewk->wu.mvxy.a[0].sp = 0x364FA;
            ewk->wu.mvxy.d[0].sp = 0;
            ewk->wu.mvxy.a[1].sp = -0x27D2;
            ewk->wu.mvxy.d[1].sp = 0;
        }

        break;

    case 2:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            dragonfly_move_next(ewk);
            ewk->wu.xyz[0].disp.pos = 627;
            ewk->wu.xyz[0].disp.low = 0;
            ewk->wu.xyz[1].disp.pos = 92;
            ewk->wu.xyz[1].disp.low = 0;
            break;
        }

        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        break;
    }
}

void dragonfly_move_0005(WORK_Other* ewk) {
    char_move(&ewk->wu);

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.rl_flag = 0;
        dragonfly_stop_timer(ewk);
        break;

    case 1:
        ewk->wu.dir_timer--;
        if (ewk->wu.dir_timer <= 0) {
            ewk->wu.routine_no[2]++;
            break;
        }
        break;

    case 2:
        ewk->wu.routine_no[2]++;
        dragonfly_r_move_1(ewk);
        break;

    case 3:
        ewk->wu.routine_no[2] += dragonfly_r_move_2(ewk);
        break;

    case 4:
        ewk->wu.routine_no[2] += dragonfly_r_move_3(ewk);
        break;

    case 5:
        char_move(&ewk->wu);

        if (dragonfly_r_move_4(ewk)) {
            ewk->wu.routine_no[2] = 2;
            break;
        }

        break;

    default:
        ewk->wu.dir_timer--;

        if (ewk->wu.dir_timer <= 0) {
            dragonfly_move_next(ewk);
            set_char_move_init(&ewk->wu, 0, 4);
        }

        break;
    }
}

void dragonfly_stop_timer(WORK_Other* ewk) {
    s16 work = random_16();

    work &= 7;
    ewk->wu.dir_timer = effj8_timer_tbl[work];
}

const s32 effj8_sp_tbl[8][4] = { { 0x60000, -0x4000, 0x4000, 0 }, { 0x60000, -0x4800, -0x4000, 0 },
                                 { 0x60000, -0x1000, 0x4000, 0 }, { 0x60000, -0x2000, -0x4000, 0 },
                                 { 0x80000, -0xA000, 0x4000, 0 }, { 0x80000, -0x2000, -0x4000, 0 },
                                 { 0x80000, -0x6000, 0x4000, 0 }, { 0x80000, -0x4000, -0x4000, 0 } };

void dragonfly_line_set(WORK_Other* ewk, s16 dir_type) {
    s16 work = random_16();
    const s32* sp_ptr;

    work &= 7;
    sp_ptr = &effj8_sp_tbl[work][0];

    if (dir_type) {
        ewk->wu.mvxy.a[0].sp = *sp_ptr++;
        ewk->wu.mvxy.d[0].sp = *sp_ptr++;
    } else {
        ewk->wu.mvxy.a[0].sp = -*sp_ptr++;
        ewk->wu.mvxy.d[0].sp = -*sp_ptr++;
    }

    ewk->wu.xyz[0].disp.low = 0;
    ewk->wu.mvxy.a[1].sp = *sp_ptr++;
    ewk->wu.mvxy.d[1].sp = *sp_ptr++;
}

void dragonfly_move_next(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.routine_no[1] &= 7;
    ewk->wu.routine_no[2] = 0;
    ewk->wu.routine_no[3] = 0;
}

s32 effect_J8_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < 2; i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = 198;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.type = i;
        ewk->wu.dead_f = 1;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_code = 8492;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

        switch (ewk->wu.type) {
        case 0:
            ewk->wu.routine_no[1] = 4;
            ewk->wu.xyz[0].disp.pos = 16;
            ewk->wu.xyz[1].disp.pos = 120;
            break;
        case 1:
            ewk->wu.routine_no[1] = 0;
            ewk->wu.xyz[0].disp.pos = -1;
            ewk->wu.xyz[1].disp.pos = 104;
            break;
        }

        ewk->wu.my_priority = ewk->wu.position_z = 83;
        ewk->wu.char_index = 4;
        ewk->wu.sync_suzi = 0;
        ewk->wu.char_table[0] = char_add[bg_w.bg_index];
        suzi_offset_set(ewk);
    }

    return 0;
}
