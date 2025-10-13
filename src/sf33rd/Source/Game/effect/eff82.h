#ifndef EFF82_H
#define EFF82_H

#include "structs.h"
#include "types.h"

void effect_82_move(WORK_Other* ewk);
#if defined(TARGET_PS2)
s32 effect_82_init(WORK* wk, s32 /* unused */);
#else
s32 effect_82_init(WORK* wk);
#endif

#endif
