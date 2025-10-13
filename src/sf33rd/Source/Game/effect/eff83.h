#ifndef EFF83_H
#define EFF83_H

#include "structs.h"
#include "types.h"

void effect_83_move(WORK_Other* ewk);
#if defined(TARGET_PS2)
s32 effect_83_init(WORK* wk, s32 /* unused */);
#else
s32 effect_83_init(WORK* wk);
#endif

#endif
