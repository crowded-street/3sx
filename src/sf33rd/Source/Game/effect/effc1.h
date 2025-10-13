#ifndef EFFC1_H
#define EFFC1_H

#include "structs.h"
#include "types.h"

void effect_C1_move(WORK_Other* ewk);
#if defined(TARGET_PS2)
s32 effect_C1_init(WORK* wk, s32 /* unused */);
#else
s32 effect_C1_init(WORK* wk);
#endif

#endif
