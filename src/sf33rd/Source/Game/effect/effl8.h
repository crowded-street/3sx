#ifndef EFFL8_H
#define EFFL8_H

#include "structs.h"
#include "types.h"

void effect_L8_move(WORK_Other* ewk);
s32 effect_L8_init(PLW* wk);
void check_new_color_data_L8(WORK* wk);
void get_new_color_data_L8(WORK* /* unused */, s16* trom, s16* tram);
void save_old_color_data(s16* wram, s16* tram);
void load_old_color_data(s16* wram, s16* tram);

#endif
