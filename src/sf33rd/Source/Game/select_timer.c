#include "sf33rd/Source/Game/select_timer.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "types.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

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
    if (gs.Time_Stop == 2) {
        gs.select_timer_state.step = 0;
    }
}

void SelectTimer_Init() {
    gs.select_timer_state.is_running = true;
    gs.select_timer_state.step = 0;
}

void SelectTimer_Finish() {
    SDL_zero(gs.select_timer_state);
}

void SelectTimer_Run() {
    if (Present_Mode == 4 || Present_Mode == 5) {
        return;
    }

    if (Debug_w[24]) {
        return;
    }

    if (gs.Break_Into) {
        return;
    }

    switch (gs.select_timer_state.step) {
    case 0:
        if (gs.Time_Stop == 0) {
            gs.select_timer_state.step = 1;
        }

        break;

    case 1:
        check_sleep();

        if (--gs.Unit_Of_Timer) {
            break;
        }

        gs.Unit_Of_Timer = 60;
        bcdext = 0;
        gs.Select_Timer = sbcd(1, gs.Select_Timer);

        if (gs.Select_Timer == 0) {
            gs.select_timer_state.step = 2;
            gs.select_timer_state.timer = 30;
        }

        break;

    case 2:
        check_sleep();

        if (gs.Select_Timer) {
            gs.select_timer_state.step = 1;
            gs.Unit_Of_Timer = 60;
        } else {
            gs.select_timer_state.timer -= 1;

            if (gs.select_timer_state.timer == 0) {
                gs.Time_Over = true;
                gs.select_timer_state.step = 3;
            }
        }

        break;

    case 3:
        check_sleep();
        gs.Time_Over = true;

        if (gs.Select_Timer) {
            gs.select_timer_state.step = 1;
            gs.Unit_Of_Timer = 60;
        }

        break;

    default:
        gs.select_timer_state.is_running = false;
        break;
    }
}
