#ifndef EFF39_H
#define EFF39_H

#include "structs.h"
#include "types.h"

void effect_39_move(WORK_Other* ewk);

typedef struct {
    s16 PL_id;
    s16 dir_old;
    s16 Your_Char;
    s16 Target_BG;
    s16 Option;
} Effect39InitParams;

s32 effect_39_init_with_params(Effect39InitParams params);

#define effect_39_init(...) effect_39_init_with_params((Effect39InitParams){ __VA_ARGS__ })

#endif
