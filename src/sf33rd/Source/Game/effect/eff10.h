#ifndef EFF10_H
#define EFF10_H

#include "structs.h"
#include "types.h"

void effect_10_move(WORK_Other* ewk);
/* The 7 values effect_10_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    s16 id;
    u8 Type;
    u8 Type_in_Type;
    u8 dir_step;
    u8 Death_Type;
    s16 pos_x;
    s16 pos_y;
} Effect10Init;

s32 effect_10_init(const Effect10Init* p);

#endif
