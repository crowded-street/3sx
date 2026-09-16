/**
 * @file eff14.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff14.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff08.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/manage.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sys_sub.h"

/* Wait for the score display to be armed. If the panel is showing, start the
 * colour cycle; if not, the effect is finished before it starts. */
static void e14_await_start(WORK_Other* ewk) {
    if (Suicide[5] & 1) {
        if (PB_Status & 3) {
            ewk->wu.routine_no[0]++;
            ewk->wu.dir_timer = 1;
            ewk->wu.dir_step = 0;
        } else {
            ewk->wu.routine_no[0] = 99;
        }
    }
}

/* Step to the next colour when its timer runs out; the 0xFF entry ends the
 * table and the effect with it. */
static void e14_step_colour(WORK_Other* ewk) {
    if (--ewk->wu.dir_timer == 0) {
        ewk->wu.my_col_code = Rewrite_Color_Data_08[ewk->wu.type][ewk->wu.dir_step][0] + 0;
        ewk->wu.dir_timer = Rewrite_Color_Data_08[ewk->wu.type][ewk->wu.dir_step][1];

        if (ewk->wu.dir_timer == 0xFF) {
            ewk->wu.routine_no[0] = 99;
        } else {
            ewk->wu.dir_step++;
        }
    }
}

void effect_14_move(WORK_Other* ewk) {
    if (Suicide[5] & 0x80) {
        push_effect_work(&ewk->wu);
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        e14_await_start(ewk);
        break;

    case 1:
        e14_step_colour(ewk);
        break;
    }

    Disp_Digit16x24(Disp_Score_Buff[ewk->wu.type], ewk->wu.position_x, ewk->wu.position_y, ewk->wu.my_col_code);
}

s32 effect_14_init(s16 id, s16 x, s16 y, s16 atr) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 14;
    ewk->wu.work_id = 16;
    ewk->wu.type = id;
    ewk->wu.position_x = x;
    ewk->wu.position_y = y;
    ewk->wu.my_col_code = atr;
    Disp_Bonus_Contents = 0;
    effect_14_move((WORK_Other*)ewk);
    return 0;
}
