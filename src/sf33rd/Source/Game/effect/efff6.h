#ifndef EFFF6_H
#define EFFF6_H

#include "structs.h"
#include "types.h"

typedef struct {
    s16 dir;
    s16 limit_x_pos;
    s16 limit_y_pos;
    s16 zoom_v;
    s32 sp_x_a;
    s32 sp_x_d;
    s32 sp_y_a;
    s32 sp_y_d;
} EFFF6_ETC;

extern const s16 efff6_move01_tbl[6];
extern const s16 efff6_data_tbl00[61][8];
extern const EFFF6_ETC efff6_etc_data[61];

void effect_F6_move(WORK_Other* ewk);
void efff6_move(WORK_Other* ewk);
void efff6_move_common(WORK_Other* ewk);
void efff6_move01(WORK_Other* ewk);
s32 effect_F6_init(u8 typenum);

#endif
