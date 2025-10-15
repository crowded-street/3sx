#ifndef EFFD7_H
#define EFFD7_H

#include "structs.h"
#include "types.h"

void effect_D7_move(WORK_Other* ewk);
void cal_speeds_effD7(WORK_Other* ewk, s16 tm, s16 tx, s16 ty, s16 ysp);
void ball_init_position_effD7(WORK_Other* ewk, PLW* mwk);
u8 screen_range_check_effD7(WORK* wk);
s32 effect_D7_init(PLW* wk);

#endif
