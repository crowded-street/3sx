#ifndef EFF45_H
#define EFF45_H

#include "structs.h"
#include "types.h"

extern MessageData Message_Data[4];

void effect_45_move(WORK_Other_CONN* ewk);
s32 effect_45_init(u8 id, s16 sync_bg, s16 master_player);
void Convert_16_10_3(WORK_Other_CONN* ewk, u16 target);

#endif
