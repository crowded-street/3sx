#ifndef EFFK6_H
#define EFFK6_H

#include "structs.h"
#include "types.h"

void effect_K6_move(WORK_Other* ewk);
s32 effect_K6_init(s16 PL_id, s16 dir_old, s16 dm_vital, s16 Target_BG);
s32 chkNameSuv(s32 plnum, s32 rnum);

#endif
