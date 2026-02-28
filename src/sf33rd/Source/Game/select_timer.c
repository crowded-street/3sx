#include "sf33rd/Source/Game/select_timer.h"
#include "constants.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "types.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

SelectTimerState select_timer_state = { 0 };
static s16 bcdext = 0;

static u8 sbcd(u8 a, u8 b) {
    s16 c;
    s16 d;

    if ((d = (b & 0xF) - (a & 0xF) - (bcdext & 1)) < 0) {
        d += 10;
        d |= 16;
    }

    c = (b & 0xF0) - (a & 0xF0) - (d & 0xF0);
    d &= 0xF;

    if ((d |= c) < 0) {
        d += 160;
        bcdext = 1;
    } else {
        bcdext = 0;
    }

    return d;
}

static void check_sleep() {
    if (Time_Stop == 2) {
        select_timer_state.step = 0;
    }
}

void SelectTimer_Init() {
    select_timer_state.is_running = true;
    select_timer_state.step = 0;
}

void SelectTimer_Finish() {
    SDL_zero(select_timer_state);
}

void SelectTimer_Run() {
    if (Present_Mode == 4 || Present_Mode == 5) {
        return;
    }

    if (Debug_w[24]) {
        return;
    }

    if (Break_Into) {
        return;
    }

    switch (select_timer_state.step) {
    case 0:
        if (Time_Stop == 0) {
            select_timer_state.step = 1;
        }

        break;

    case 1:
        check_sleep();

        if (--Unit_Of_Timer) {
            break;
        }

        Unit_Of_Timer = UNIT_OF_TIMER_MAX;
        bcdext = 0;
        Select_Timer = sbcd(1, Select_Timer);

        if (Select_Timer == 0) {
            select_timer_state.step = 2;
            select_timer_state.timer = 30;
        }

        break;

    case 2:
        check_sleep();

        if (Select_Timer) {
            select_timer_state.step = 1;
            Unit_Of_Timer = UNIT_OF_TIMER_MAX;
        } else {
            select_timer_state.timer -= 1;

            if (select_timer_state.timer == 0) {
                Time_Over = true;
                select_timer_state.step = 3;
            }
        }

        break;

    case 3:
        check_sleep();
        Time_Over = true;

        if (Select_Timer) {
            select_timer_state.step = 1;
            Unit_Of_Timer = UNIT_OF_TIMER_MAX;
        }

        break;

    default:
        select_timer_state.is_running = false;
        break;
    }
}
