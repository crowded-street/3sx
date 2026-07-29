#ifndef EFF79_H
#define EFF79_H

#include "structs.h"
#include "types.h"

// MARK: - Serialized

extern u8 OK_Appear79[2];
extern u8 Extra_Counter[2];

void effect_79_move(WORK_Other* ewk);
s32 effect_79_init(s16 pl_id, s16 plate_id, s16 pos_id, s16 time, s16 Target_BG);

#endif
