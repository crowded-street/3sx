#ifndef EFF24_H
#define EFF24_H

#include "structs.h"
#include "types.h"

extern const s16 eff24_data_tbl[56];
extern const s16 eff24_quake_index_tbl[111];
extern const s32 eff24_quake_speed_y_tbl[4][8];
extern const s32 eff24_quake_speed_x_tbl[4][8];
extern const s16 dog24_x_data[8];

void effect_24_move(WORK_Other* ewk);
void eff24_quake_sub(WORK_Other* ewk);
void dog24_data_set(WORK_Other* ewk);
void eff24_sp_data_set(WORK_Other* ewk);
s32 effect_24_init();

#endif
