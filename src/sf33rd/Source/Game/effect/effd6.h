#ifndef EFFD6_H
#define EFFD6_H

#include "structs.h"
#include "types.h"

void effect_D6_move(WORK_Other* ewk);
s32 effect_D6_init(WORK_Other* wk, s16 dr, s16 sp, s16 dl, s16 acc);
void setup_hana_extra(WORK* wk, s16 num, s16 acc);

#endif
