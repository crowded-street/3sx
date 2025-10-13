#ifndef EFFF6_H
#define EFFF6_H

#include "structs.h"
#include "types.h"

typedef struct {
    // total size: 0x18
    s16 dir;         // offset 0x0, size 0x2
    s16 limit_x_pos; // offset 0x2, size 0x2
    s16 limit_y_pos; // offset 0x4, size 0x2
    s16 zoom_v;      // offset 0x6, size 0x2
    s32 sp_x_a;      // offset 0x8, size 0x4
    s32 sp_x_d;      // offset 0xC, size 0x4
    s32 sp_y_a;      // offset 0x10, size 0x4
    s32 sp_y_d;      // offset 0x14, size 0x4
} EFFF6_ETC;

extern const s16 efff6_move01_tbl[6];      // size: 0xC, address: 0x515180
extern const s16 efff6_data_tbl00[61][8];  // size: 0x3D0, address: 0x515190
extern const EFFF6_ETC efff6_etc_data[61]; // size: 0x5B8, address: 0x515560

void effect_F6_move(WORK_Other* ewk);
void efff6_move(WORK_Other* ewk);
void efff6_move_common(WORK_Other* ewk);
void efff6_move01(WORK_Other* ewk);
s32 effect_F6_init(u8 typenum);

#endif
