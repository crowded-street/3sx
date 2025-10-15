#ifndef EFF57_H
#define EFF57_H

#include "structs.h"
#include "types.h"

void effect_57_move(WORK_Other* ewk);
void EFF57_KILL(WORK_Other* ewk);
s32 effect_57_init(s16 dir_old, s16 ID, s16 Target_BG, s16 char_ix, s16 option);

#endif
