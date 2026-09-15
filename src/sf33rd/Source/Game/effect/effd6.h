#ifndef EFFD6_H
#define EFFD6_H

#include "structs.h"
#include "types.h"

void effect_D6_move(WORK_Other* ewk);

typedef struct {
    s16 direction;
    s16 speed;
    s16 delta;
    s16 acceleration;
} EffectD6Init;

s32 effect_D6_init_params(WORK_Other* wk, const EffectD6Init* init);

static inline s32 effect_D6_init(WORK_Other* wk, s16 dr, s16 sp, s16 dl, s16 acc) {
    const EffectD6Init init = { dr, sp, dl, acc };
    return effect_D6_init_params(wk, &init);
}

void setup_hana_extra(WORK* wk, s16 num, s16 acc);

#endif
