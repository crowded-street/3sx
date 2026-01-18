#ifndef EFFB2_H
#define EFFB2_H

#include "structs.h"
#include "types.h"

// MARK: - Serialized

extern s16 rf_b2_flag;
extern s16 b2_curr_no;

void effect_B2_move(WORK_Other* ewk);
s32 effect_B2_init();

#endif
