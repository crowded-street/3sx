#ifndef SELECT_TIMER_H
#define SELECT_TIMER_H

#include "types.h"

#include <stdbool.h>

typedef struct SelectTimerState {
    bool is_running;
    int step;
    int timer;
} SelectTimerState;

extern SelectTimerState select_timer_state;

void SelectTimer_Init();
void SelectTimer_Finish();
void SelectTimer_Run();

#endif
