#ifndef EFF20_NG_H
#define EFF20_NG_H

#include "structs.h"
#include "types.h"

#define EFFECT_NG20_ID 234

void effect_ng20_move(WORK_Other* ewk);
s32 effect_ng20_init();
s16* effect_ng20_leaf_count(WORK_Other* spawner, s16 group);

#endif
