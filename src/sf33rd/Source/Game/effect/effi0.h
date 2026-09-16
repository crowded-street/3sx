#ifndef EFFI0_H
#define EFFI0_H

#include "structs.h"
#include "types.h"

typedef struct {
    WORK* master;
    s16 x_offset;
    s16 y_offset;
    s16 x_speed;
    s16 y_speed;
    s16 target_y;
} EffectI0Init;

void effect_I0_move(WORK_Other* ewk);
s32 effect_I0_init(EffectI0Init params);
s32 setup_koishi_extra(WORK* wk, u8 num);

#endif
