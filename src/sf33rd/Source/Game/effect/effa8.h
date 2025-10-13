#ifndef EFFA8_H
#define EFFA8_H

#include "structs.h"
#include "types.h"

void effect_A8_move(WORK_Other_CONN* ewk);
s32 effect_A8_init(s16 id, u8 dir_old, s16 sync_bg, s16 master_player, s16 cursor_index, s16 char_ix, s16 pos_index);

#endif
