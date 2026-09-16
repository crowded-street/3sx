#ifndef EFF08_H
#define EFF08_H

#include "structs.h"
#include "types.h"

extern const u8 Rewrite_Color_Data_08[2][13][2];

void effect_08_move(WORK_Other* ewk);
/* The 5 values effect_08_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    s8 sc_num;
    s8 x;
    s8 y;
    u16 atr;
    s16 color_type;
} Effect08Init;

s32 effect_08_init(const Effect08Init* p);

#endif
