#ifndef COMMON_PAD_H
#define COMMON_PAD_H

#include "types.h"

typedef struct {
    u8 port;
    u8 slot;
    u8 vib;
    u8 etc;
} PAD_CONN;

typedef struct {
    s16 x;
    s16 y;
    s16 pow;
    s16 ang;
    f32 rad;
} PAD_STICK;

typedef struct {
    u8 pow[16];
} PAD_ANSHOT;

#endif
