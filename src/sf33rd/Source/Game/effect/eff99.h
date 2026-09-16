#ifndef EFF99_H
#define EFF99_H

#include "structs.h"
#include "types.h"

void effect_99_move(WORK_Other_CONN* ewk);
/* The 6 values effect_99_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    s16 id;
    s16 type;
    s16 char_offset;
    s16 letter_index;
    s16 pos_index;
    s16 master_player;
} Effect99Init;

s32 effect_99_init(const Effect99Init* p);

#endif
