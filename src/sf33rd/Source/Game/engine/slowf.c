/**
 * @file slowf.c
 * Slow-Motion Controller
 */

#include "sf33rd/Source/Game/engine/slowf.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/workuser.h"

s16 EXE_flag;
s16 SLOW_flag;
s16 SLOW_timer;

const s8 slow_timer_to_flag[32] = { 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                                    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

void init_slow_flag() {
    EXE_flag = 0;
    SLOW_flag = 0;
    SLOW_timer = 0;
}

void set_conclusion_slow() {
    SLOW_timer = 95;
}

void set_EXE_flag() {
    s16 tmw;

    if (!Game_pause) {
        if (SLOW_timer) {
            if (--SLOW_timer) {
                tmw = SLOW_timer / 8;

                if (tmw > 31) {
                    tmw = 31;
                }

                SLOW_flag = slow_timer_to_flag[tmw];
            } else {
                SLOW_flag = 0;
            }
        }

        EXE_flag = Game_timer % (SLOW_flag + 1);
    }
}
