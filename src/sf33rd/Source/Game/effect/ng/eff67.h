#ifndef EFF67_NG_H
#define EFF67_NG_H

#include "structs.h"
#include "types.h"

#define EFFECT_NG67_ID 253

void effect_ng67_move(WORK_Other* ewk);
s32 effect_ng67_init(WORK_Other* spawner, s16 group);
bool effect_ng67_is_hidden(WORK_Other* ewk);

#endif
