#ifndef EFF23_H
#define EFF23_H

#include "structs.h"
#include "types.h"

void effect_23_move(WORK_Other_CONN* ewk);
s32 effect_23_init(s16 id, u8 dir_old, s16 sync_bg, s16 master_player, s16 letter_type, s16 cursor_index,
                   u16 char_offset, s16 pos_index, s16 type);

#endif
