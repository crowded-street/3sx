#ifndef EFF40_H
#define EFF40_H

#include "structs.h"
#include "types.h"

void effect_40_move(WORK_Other* ewk);
/* The 6 values effect_40_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    s16 id;
    s16 type;
    s16 char_ix;
    s16 sync_bg;
    s16 master_player;
    s16 master_priority;
} Effect40Init;

s32 effect_40_init(const Effect40Init* p);

#endif
