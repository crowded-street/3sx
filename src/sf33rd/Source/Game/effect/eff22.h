#ifndef EFF22_H
#define EFF22_H

#include "structs.h"
#include "types.h"

extern const s16 snow_pos_tbl[12][2];
extern const s32 snow_sp[4][12][4];

void effect_22_move(WORK_Other* ewk);
s32 effect_22_init();

#endif
