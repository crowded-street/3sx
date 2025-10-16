/**
 * @file effk4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effk4.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void get_init_position_effK4(WORK* wk);
void get_init_speed_and_timer_effK4(WORK* wk);

const s16 effK4_isp_table[4][4][2] = { { { -512, 768 }, { -768, 768 }, { 512, 768 }, { 768, 768 } },
                                       { { -1024, 1024 }, { -1280, 1024 }, { 1024, 1024 }, { 1280, 1024 } },
                                       { { -1280, 1280 }, { -1792, 1280 }, { 1280, 1280 }, { 1792, 1280 } },
                                       { { -1536, 1536 }, { -2048, 1536 }, { 1536, 1536 }, { 2048, 1536 } } };

const s16 effK4_isp_x_hosei[4][8] = { { 0, 128, 256, 384, -128, -256, -384, -512 },
                                      { 0, 128, 256, 384, -128, -256, -512, -768 },
                                      { 0, 256, 384, 512, -256, -512, -768, -1024 },
                                      { 0, 256, 512, -256, -512, -768, -1024, -1280 } };

const s16 effK4_isp_y_hosei[4][8] = { { 0, 0, 128, 128, 256, 256, 384, -128 },
                                      { 0, 128, 128, 256, 256, 384, 384, -256 },
                                      { 0, 128, 256, 384, 512, -128, -256, -512 },
                                      { 0, 256, 384, 512, 640, -256, -512, -768 } };

const s16 effK4_life_time[4] = { 24, 24, 24, 24 };

const s16 effK4_char_sel_table[4][16] = {
    { 132, 132, 132, 132, 133, 133, 133, 133, 134, 134, 134, 134, 135, 135, 135, 135 },
    { 136, 136, 136, 137, 137, 137, 138, 138, 138, 139, 139, 139, 132, 132, 133, 134 },
    { 140, 140, 132, 132, 136, 136, 141, 141, 141, 142, 142, 142, 143, 143, 143, 137 },
    { 140, 140, 140, 136, 136, 136, 132, 132, 132, 140, 136, 132, 134, 142, 143, 140 }
};

void effect_K4_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.blink_timing = ewk->master_id;
        get_init_position_effK4(&ewk->wu);
        get_init_speed_and_timer_effK4(&ewk->wu);
        ewk->wu.position_z = 24;
        set_char_move_init(&ewk->wu, 0, effK4_char_sel_table[ewk->wu.dm_attlv][random_16()]);
        /* fallthrough */

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            char_move(&ewk->wu);
            add_mvxy_speed(&ewk->wu);
            cal_mvxy_speed(&ewk->wu);

            if (ewk->wu.kage_hy) {
                ewk->wu.kage_hy--;
            } else {
                ewk->wu.disp_flag = 2;
            }

            if (--ewk->wu.kage_prio < 0) {
                ewk->wu.disp_flag = 0;
                ewk->wu.routine_no[0] = 2;
            }
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
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

void get_init_position_effK4(WORK* wk) {
    s16 xhs;

    wk->xyz[0].disp.pos = wk->hit_mark_x;
    wk->xyz[1].disp.pos = wk->hit_mark_y;
    xhs = 32 - (random_32() * 2);
    wk->xyz[0].disp.pos += xhs;
    wk->xyz[1].disp.pos += 8 - random_16();
    wk->position_z = 24;
    wk->type = (xhs < 0) * 2;
}

void get_init_speed_and_timer_effK4(WORK* wk) {
    s16 data[4];
    s16 ix;

    ix = wk->type + (random_16() & 1);
    data[0] = effK4_isp_table[wk->dm_attlv][ix][0];
    data[2] = effK4_isp_table[wk->dm_attlv][ix][1];
    data[1] = 0;
    data[3] = -96;
    ix = random_16() & 7;
    data[0] += effK4_isp_x_hosei[wk->dm_attlv][ix];
    ix = random_16() & 7;
    data[2] += effK4_isp_y_hosei[wk->dm_attlv][ix];
    setup_move_data_easy(wk, &data[0], 1, 0);
    wk->kage_prio = (random_16() & 7) + effK4_life_time[wk->dm_attlv];
    wk->kage_hy = wk->kage_prio / 2;
}

void setup_effK4(WORK* wk) {
    WORK* twk;
    s16 i;
    s16 j;

    (void)wk;
    (void)twk;
    (void)i;
    (void)j;
}
