#ifndef EFFA3_H
#define EFFA3_H

#include "structs.h"
#include "types.h"

void effect_A3_move(WORK_Other* ewk);

typedef struct {
    s16 id;
    u8 type;
    u8 type_in_type;
    u8 dir_step;
    u8 death_type;
    s16 pos_x;
    s16 pos_y;
    s16 buff;
} EffectA3Init;

s32 effect_A3_init_params(EffectA3Init init);

#define effect_A3_init(id, type, type_in_type, dir_step, death_type, pos_x, pos_y, buff)                            \
    effect_A3_init_params((EffectA3Init) { (id), (type), (type_in_type), (dir_step), (death_type), (pos_x), (pos_y), \
                                           (buff) })

#endif
