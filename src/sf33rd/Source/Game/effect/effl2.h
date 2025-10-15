#ifndef EFFL2_H
#define EFFL2_H

#include "structs.h"
#include "types.h"

extern const s8 effl2_dir_tbl[2][16];

void effect_L2_move(WORK_Other* ewk);
void effl2_dir_check(WORK_Other* ewk);
s32 effect_L2_init();

#endif
