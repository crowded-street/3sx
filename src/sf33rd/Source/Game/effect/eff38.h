#ifndef EFF38_H
#define EFF38_H

#include "structs.h"
#include "types.h"

typedef struct {
    s16 player_id;
    s16 direction;
    s16 character;
    s16 play_status;
    s16 target_bg;
} Effect38Init;

void effect_38_move(WORK_Other* ewk);
s32 effect_38_init_params(Effect38Init params);

#define effect_38_init(PL_id, dir_old, Your_Char, Play_Status, Target_BG)                                               \
    effect_38_init_params((Effect38Init) { PL_id, dir_old, Your_Char, Play_Status, Target_BG })

#endif
