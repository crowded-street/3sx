#ifndef EFF67_H
#define EFF67_H

#include "structs.h"
#include "types.h"

void effect_67_move(WORK_Other_CONN* ewk);

typedef struct {
    s16 id;
    s16 x;
    s16 y;
    s16 time;
    s16 char_index;
    s16 priority;
    s16 routine;
    s16 color;
} Effect67Init;

s32 effect_67_init_params(Effect67Init init);

#define effect_67_init(id, x, y, time, char_index, priority, routine, color)                                      \
    effect_67_init_params((Effect67Init) { (id), (x), (y), (time), (char_index), (priority), (routine), (color) })

#endif
