#ifndef EFF56_H
#define EFF56_H

#include "structs.h"
#include "types.h"

// MARK: - Serialized

extern const u8* ci_pointer;
extern u8 ci_col;
extern u8 ci_timer;

void effect_56_move(WORK_Other* ewk);
s32 effect_56_init(u8 type, u8 kill);

#endif
