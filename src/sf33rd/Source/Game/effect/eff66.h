#ifndef EFF66_H
#define EFF66_H

#include "structs.h"
#include "types.h"

void effect_66_move(WORK_Other* ewk);

typedef struct {
    s16 order_index;
    s16 id;
    s16 master_player;
    s16 target_bg;
    s16 char_ix;
    s16 char_ix2;
    s16 option;
} Effect66InitParams;

s32 effect_66_init_with_params(Effect66InitParams params);

#define effect_66_init(...) effect_66_init_with_params((Effect66InitParams){ __VA_ARGS__ })

#endif
