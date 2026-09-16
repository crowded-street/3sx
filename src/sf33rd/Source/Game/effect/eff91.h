#ifndef EFF91_H
#define EFF91_H

#include "structs.h"
#include "types.h"

void effect_91_move(WORK_Other* ewk);
/* The 6 values effect_91_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    s16 master_id;
    s16 type;
    s16 target_bg;
    s16 char_ix;
    s16 char_ix2;
    s16 master_player;
} Effect91Init;

s32 effect_91_init(const Effect91Init* p);

#endif
