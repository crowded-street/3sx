#ifndef EFFH6_H
#define EFFH6_H

#include "structs.h"
#include "types.h"

typedef struct H6InitArgs {
    s16 timer;
    const char* str;
    s16 x;
    s16 y;
    s16 original_color;
} H6InitArgs;

void effect_H6_move(WORK_Other* ewk);
s32 effect_H6_init(const H6InitArgs* args);

#endif
