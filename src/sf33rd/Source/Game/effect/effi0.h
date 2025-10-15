#ifndef EFFI0_H
#define EFFI0_H

#include "structs.h"
#include "types.h"

void effect_I0_move(WORK_Other* ewk);
s32 effect_I0_init(WORK* wk, s16 hsx, s16 hsy, s16 spx, s16 spy, s16 nxy);
s32 setup_koishi_extra(WORK* wk, u8 num);

#endif
