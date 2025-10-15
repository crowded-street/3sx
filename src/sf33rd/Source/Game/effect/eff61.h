#ifndef EFF61_H
#define EFF61_H

#include "structs.h"
#include "types.h"

void effect_61_move(WORK_Other_CONN* ewk);
s32 Check_Die_61(WORK_Other* ewk);
s32 effect_61_init(s16 master, u8 dir_old, s16 sync_bg, s16 master_player, s16 char_ix, s16 cursor_index,
                   u16 letter_type);

#endif
