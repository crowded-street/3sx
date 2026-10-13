/**
 * @file eff29.c
 * Fish swimming in a tank on the second area of Yun's stage from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff29.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

static const s16 char_base[2] = { 82, 86 };
static const s16 fish_positions[2][3] = { { 240, 136, 87 }, { 128, 148, 87 } };

static void set_speed_x(WORK_Other* ewk, s32 speed) {
    ewk->wu.mvxy.a[0].sp = speed;
    ewk->wu.mvxy.d[0].sp = 0;
}

static void set_speed_y(WORK_Other* ewk, s32 speed) {
    ewk->wu.mvxy.a[1].sp = speed;
    ewk->wu.mvxy.d[1].sp = 0;
}

static void set_char(WORK_Other* ewk, s16 index) {
    set_char_move_init(&ewk->wu, 0, char_base[ewk->wu.type] + index);
}

static void red_fish(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char(ewk, 0);
        set_speed_x(ewk, -0x6000);
        break;

    case 1:
        char_move(&ewk->wu);
        add_x_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos < 160) {
            ewk->wu.xyz[0].disp.pos = 160;
            ewk->wu.routine_no[1]++;
            set_char(ewk, 1);
        }

        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1]++;
            set_char(ewk, 2);
            set_speed_x(ewk, 0x6000);
        }

        break;

    case 3:
        char_move(&ewk->wu);
        add_x_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos > 240) {
            ewk->wu.xyz[0].disp.pos = 240;
            ewk->wu.routine_no[1]++;
            set_char(ewk, 3);
        }

        break;

    case 4:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1] = 1;
            set_char(ewk, 0);
            set_speed_x(ewk, -0x6000);
        }

        break;

    default:
        ewk->wu.routine_no[0] = 2;
        break;
    }
}

static void dark_fish(WORK_Other* ewk) {
    if (ewk->wu.routine_no[2] != 0) {
        char_move(&ewk->wu);
    }

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        set_char(ewk, 0);
        set_speed_x(ewk, 0x4000);

        /* fallthrough */

    case 1:
        add_x_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos > 256) {
            ewk->wu.routine_no[2]++;
            ewk->wu.xyz[0].disp.pos = 256;
            set_speed_x(ewk, -0x4000);
        }

        break;

    case 2:
        add_x_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos < 128) {
            ewk->wu.routine_no[2]++;
            ewk->wu.xyz[0].disp.pos = 128;
            set_speed_x(ewk, 0x4000);
        }

        break;

    case 3:
        add_x_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos > 256) {
            ewk->wu.routine_no[2]++;
            ewk->wu.xyz[0].disp.pos = 256;
            set_char(ewk, 1);
        }

        break;

    case 4:
        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[2]++;
            set_speed_x(ewk, -0x4000);
            set_speed_y(ewk, -0x5000);
            set_char(ewk, 2);
        }

        break;

    case 5:
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos < 112) {
            ewk->wu.routine_no[2]++;
            ewk->wu.xyz[1].disp.pos = 112;
            set_char(ewk, 3);
        }

        break;

    case 6:
        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[2]++;
            set_speed_x(ewk, -0x4000);
            set_char(ewk, 4);
        }

        break;

    case 7:
        add_x_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos < 160) {
            ewk->wu.routine_no[2]++;
            ewk->wu.xyz[0].disp.pos = 160;
            set_char(ewk, 5);
        }

        break;

    case 8:
        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[2]++;
            set_speed_x(ewk, -0x4000);
            set_speed_y(ewk, 0x8000);
            set_char(ewk, 6);
        }

        break;

    case 9:
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos > 151) {
            ewk->wu.routine_no[2] = 1;
            ewk->wu.xyz[1].disp.pos = 151;
            set_speed_x(ewk, 0x4000);
            set_char(ewk, 0);
        }

        break;
    }
}

void effect_ng29_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.routine_no[0] = 99;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.disp_flag = 1;
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            if (ewk->wu.type == 0) {
                red_fish(ewk);
            } else {
                dark_fish(ewk);
            }
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    case 2:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng29_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < SDL_arraysize(fish_positions); i++) {
        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG29_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.type = i;
        ewk->wu.dead_f = 1;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_code = 0x13C; // NG 0x50
        ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = fish_positions[i][0];
        ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = fish_positions[i][1];
        ewk->wu.my_priority = ewk->wu.position_z = fish_positions[i][2];
        ewk->wu.char_table[0] = _ng_hkg_b_char_table;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
