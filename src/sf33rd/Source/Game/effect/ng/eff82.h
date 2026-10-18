#ifndef EFF82_NG_H
#define EFF82_NG_H

#include "structs.h"
#include "types.h"

#define EFFECT_NG77_ID 253
#define EFFECT_NG78_ID 254
#define EFFECT_NG79_ID 255
#define EFFECT_NG80_ID 256
#define EFFECT_NG82_ID 257

void effect_ng77_move(WORK_Other* ewk);
void effect_ng78_move(WORK_Other* ewk);
void effect_ng79_move(WORK_Other* ewk);
void effect_ng80_move(WORK_Other* ewk);
void effect_ng82_move(WORK_Other* ewk);
s32 effect_ng82_init();

#endif
