#ifndef EFF86_H
#define EFF86_H

#include "structs.h"
#include "types.h"

extern const s16 eff86_data_tbl00[7];

void effect_86_move(WORK_Other* ewk);
void eff86_0000(WORK_Other* ewk);
s32 effect_86_init(s16 type86);

#endif
