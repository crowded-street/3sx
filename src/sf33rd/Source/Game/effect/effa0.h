#ifndef EFFA0_H
#define EFFA0_H

#include "structs.h"
#include "types.h"

void effect_A0_move(WORK_Other_CONN* ewk);
/* The 7 values effect_A0_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    s16 type;
    u16 disp_target;
    s16 pos_index;
    s16 old_rno;
    s16 zero;
    s16 target_bg;
    s16 master_player;
} EffectA0Init;

s32 effect_A0_init(const EffectA0Init* p);

#endif
