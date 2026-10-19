#ifndef EFF82_NG_H
#define EFF82_NG_H

#include "structs.h"
#include "types.h"

#define EFFECT_NG77_ID 261
#define EFFECT_NG78_ID 262
#define EFFECT_NG79_ID 263
#define EFFECT_NG80_ID 264
#define EFFECT_NG82_ID 265

void effect_ng77_move(WORK_Other* ewk);
void effect_ng78_move(WORK_Other* ewk);
void effect_ng79_move(WORK_Other* ewk);
void effect_ng80_move(WORK_Other* ewk);
void effect_ng82_move(WORK_Other* ewk);
s32 effect_ng82_init();

#endif
