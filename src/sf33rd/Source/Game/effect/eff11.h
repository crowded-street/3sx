#ifndef EFF11_H
#define EFF11_H

#include "structs.h"
#include "types.h"

extern const s16 eff11_data_tbl[24];
extern const s16 eff11_quake_index_tbl[111];
extern const s32 eff11_quake_speed_y_tbl[2][8];
extern const s32 eff11_quake_speed_y_tbl2[2][4];
extern const s32 eff11_quake_speed_x_tbl[2][8];

void effect_11_move(WORK_Other* ewk);
void eff11_quake_sub(WORK_Other* ewk);
void quake_level_middle(WORK_Other* ewk);
void quake_level_large(WORK_Other* ewk);
s32 effect_11_init();

#endif
