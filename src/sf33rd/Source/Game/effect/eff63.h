#ifndef EFF63_H
#define EFF63_H

#include "structs.h"
#include "types.h"

void effect_63_move(WORK_Other_CONN* ewk);
/* The 5 values effect_63_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    u8 dir_old;
    s16 sync_bg;
    s16 master_player;
    s16 letter_type;
    s16 cursor_index;
} Effect63Init;

s32 effect_63_init(const Effect63Init* p);

#endif
