/**
 * @file eff56.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff56.h"
#include "common.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

const u8* ci_pointer;
u8 ci_col;
u8 ci_timer;

const u8 ci_color_tbl[26] = { 21, 2,  22, 2,  21, 2,  20, 2,  21, 2,  22, 2,  21,
                              2,  20, 2,  21, 2,  22, 2,  21, 2,  20, 2,  20, 255 };

const u8 bonus_ci_color_tbl[6] = { 20, 12, 20, 12, 20, 255 };

void effect_56_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        if (Bonus_Game_Flag && bg_w.stage == 20) {
            ci_pointer = bonus_ci_color_tbl;
            ci_col = *ci_pointer++;
            ci_timer = *ci_pointer++;
        } else {
            ci_pointer = ci_color_tbl;
            ci_col = *ci_pointer++;
            ci_timer = *ci_pointer++;
        }

        /* fallthrough */

    case 1:
        if (ewk->wu.type < 7) {
            ci_set(ewk->wu.type, ci_col);
        } else {
            nw_set(ewk->wu.type - 7, ci_col);
        }

        break;

    default:
    case 2:
        if (Message_Suicide[ewk->wu.charset_id]) {
            push_effect_work(&ewk->wu);
            return;
        }

        if (ewk->wu.type < 7) {
            ci_set(ewk->wu.type, 20);
        } else {
            nw_set(ewk->wu.type - 7, 20);
        }

        return;
    }

    if (ci_timer > 1) {
        ci_timer--;
        return;
    }

    ci_col = *ci_pointer++;
    ci_timer = *ci_pointer++;

    if (ci_timer == 0xFF) {
        ewk->wu.routine_no[0]++;
    }
}

s32 effect_56_init(u8 type, u8 kill) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 56;
    ewk->wu.work_id = 16;
    ewk->wu.type = type;
    ewk->wu.charset_id = kill;
    return 0;
}
