/**
 * @file eff08.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff08.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

const u8 Rewrite_Color_Data_08[2][13][2] = { { { 30, 2 },
                                               { 15, 2 },
                                               { 30, 2 },
                                               { 15, 2 },
                                               { 30, 2 },
                                               { 15, 2 },
                                               { 30, 2 },
                                               { 15, 2 },
                                               { 30, 2 },
                                               { 15, 2 },
                                               { 15, 255 },
                                               { 0, 0 },
                                               { 0, 0 } },
                                             { { 26, 2 },
                                               { 30, 2 },
                                               { 26, 2 },
                                               { 30, 2 },
                                               { 26, 2 },
                                               { 30, 2 },
                                               { 26, 2 },
                                               { 30, 2 },
                                               { 26, 2 },
                                               { 30, 2 },
                                               { 26, 255 },
                                               { 0, 0 },
                                               { 0, 0 } } };

void effect_08_move(WORK_Other* ewk) {
    if (Suicide[5] & 0x80) {
        push_effect_work(&ewk->wu);
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (Suicide[5] & 1) {
            if (PB_Status & 3) {
                ewk->wu.routine_no[0]++;
                ewk->wu.dir_timer = 1;
                ewk->wu.dir_step = 0;
            } else {
                ewk->wu.routine_no[0] = 99;
            }
        }

        break;

    case 1:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.my_col_code = Rewrite_Color_Data_08[ewk->master_priority][ewk->wu.dir_step][0] + 0;
            ewk->wu.dir_timer = Rewrite_Color_Data_08[ewk->master_priority][ewk->wu.dir_step][1];

            if (ewk->wu.dir_timer == 0xFF) {
                ewk->wu.routine_no[0] = 99;
            } else {
                ewk->wu.dir_step++;
            }
        }

        break;
    }

    sc_ram_to_vram_opc(ewk->wu.type, ewk->wu.position_x, ewk->wu.position_y, ewk->wu.my_col_code);
}

s32 effect_08_init(s8 sc_num, s8 x, s8 y, u16 atr, s16 color_type) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 8;
    ewk->wu.work_id = 16;
    ewk->wu.type = sc_num + 0;
    ewk->wu.position_x = x;
    ewk->wu.position_y = y;
    ewk->wu.my_col_code = atr;
    ewk->master_priority = color_type;
    effect_08_move((WORK_Other*)ewk);
    return 0;
}
