/**
 * @file effd6.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effd6.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/PLS01.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/SLOWF.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

const s16 num_of_hana[4] = { 3, 4, 5, 6 };

const s16 hana_dir_hosei[4][6] = {
    { -3, 2, -1, 1, -2, 0 }, { -3, 1, -2, 3, 0, 2 }, { -1, 2, -3, 4, -2, 0 }, { -1, 3, -3, 1, 0, 2 }
};

const s16 hana_speed_hosei[4][6] = { { 0x3C0, 0x4C0, 0x580, 0x5C0, 0x6C0, 0x700 },
                                     { 0x400, 0x480, 0x600, 0x640, 0x720, 0x780 },
                                     { 0x3C0, 0x4C0, 0x540, 0x580, 0x680, 0x740 },
                                     { 0x400, 0x440, 0x640, 0x6C0, 0x780, 0x700 } };

const s16 hana_delta_hosei[4][6] = { { -64, -72, -80, -72, -88, -80 },
                                     { -64, -72, -80, -72, -88, -80 },
                                     { -64, -72, -80, -72, -88, -80 },
                                     { -64, -72, -80, -72, -88, -80 } };

void effect_D6_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        add_pos_dir_064(&ewk->wu, ewk->wu.dir_old * 2);
        set_char_move_init(&ewk->wu, 0, 0x7D);
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (sa_stop_check() == 0) {
            if (EXE_flag == 0 && Game_pause == 0) {
                switch (ewk->wu.routine_no[1]) {
                case 0:
                    ewk->wu.dir_old += ewk->wu.dir_step;

                    if (ewk->wu.dir_old < 0) {
                        ewk->wu.dir_old = 0;
                        ewk->wu.routine_no[1]++;
                        ewk->wu.dir_timer = 24;
                    }

                    add_pos_dir_064(&ewk->wu, ewk->wu.dir_old);
                    char_move(&ewk->wu);
                    break;

                case 1:
                    char_move(&ewk->wu);

                    if (--ewk->wu.dir_timer <= 0) {
                        ewk->wu.routine_no[1]++;
                        ewk->wu.disp_flag = 2;
                        ewk->wu.dir_timer = 12;
                    }

                    break;

                default:
                    char_move(&ewk->wu);

                    if (--ewk->wu.dir_timer <= 0) {
                        ewk->wu.routine_no[0]++;
                        ewk->wu.disp_flag = 0;
                    }

                    break;
                }
            }

            ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
            ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
        }

        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_D6_init(WORK_Other* wk, s16 dr, s16 sp, s16 dl, s16 acc) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 136;
    ewk->wu.work_id = 16;
    ewk->wu.my_mts = 14;
    ewk->wu.rl_flag = wk->wu.rl_flag;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->wu.my_col_mode = wk->wu.my_col_mode;
    ewk->wu.my_col_code = wk->wu.my_col_code;
    ewk->wu.cgromtype = 1;
    ewk->wu.direction = dr;
    ewk->wu.dir_old = (sp * acc) / 16;
    ewk->wu.dir_step = (dl * acc) / 16;
    ewk->master_id = ewk->wu.blink_timing = wk->master_id;
    ewk->wu.xyz[0].disp.pos = wk->wu.position_x;
    ewk->wu.xyz[1].disp.pos = wk->wu.position_y;
    ewk->wu.position_z = wk->wu.position_z + 1;
    *ewk->wu.char_table = _plef_char_table;
    return 0;
}

void setup_hana_extra(WORK* wk, s16 num, s16 acc) {
    s16 i;
    s16 way = wk->direction * 4;
    s16 rnd_00 = random_16() & 3;
    s16 rnd_01;

    for (i = 0; i < num_of_hana[num]; i++) {
        rnd_01 = random_16() & 3;
        effect_D6_init((WORK_Other*)wk,
                       way + hana_dir_hosei[rnd_00][i] & 0x3F,
                       hana_speed_hosei[rnd_01][i],
                       hana_delta_hosei[rnd_01][i],
                       acc);
    }
}
