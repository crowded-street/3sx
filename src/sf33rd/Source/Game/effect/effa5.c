/**
 * @file effa5.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effa5.h"
#include "common.h"
#include "sf33rd/Source/Game/BCD.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"

s32 Check_Sleep_A5(WORK_Other* ewk);

void effect_A5_move(WORK_Other* ewk) {
    if (Present_Mode == 4 || Present_Mode == 5) {
        return;
    }

    if (Debug_w[24]) {
        return;
    }

    if (Break_Into) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (Time_Stop == 0) {
            ewk->wu.routine_no[0]++;
        }

        break;

    case 1:
        if (!Check_Sleep_A5(ewk)) {
            break;
        }

        if (--Unit_Of_Timer) {
            break;
        }

        Unit_Of_Timer = 50;
        bcdext = 0;

        if ((Select_Timer = sbcd(1, Select_Timer)) == 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.dir_timer = 30;
        }

        break;

    case 2:
        if (!Check_Sleep_A5(ewk)) {
            break;
        }

        if (Select_Timer) {
            ewk->wu.routine_no[0] = 1;
            Unit_Of_Timer = 50;
        } else if (--ewk->wu.dir_timer == 0) {
            Time_Over = 1;
            ewk->wu.routine_no[0]++;
        }

        break;

    case 3:
        if (!Check_Sleep_A5(ewk)) {
            break;
        }

        Time_Over = 1;

        if (Select_Timer) {
            ewk->wu.routine_no[0] = 1;
            Unit_Of_Timer = 50;
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 Check_Sleep_A5(WORK_Other* ewk) {
    if (Time_Stop == 2) {
        ewk->wu.routine_no[0] = 0;
    }

    return 1;
}

s32 effect_A5_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 105;
    return 0;
}
