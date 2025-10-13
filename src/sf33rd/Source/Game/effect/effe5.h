#ifndef EFFE5_H
#define EFFE5_H

#include "structs.h"
#include "types.h"

extern const u16 after_image_color[][2];

void effect_E5_move(WORK_Other* ewk);
s32 effect_E5_init(PLW* wk);
void effect_e7_e8_init_union(WORK_Other* nwk, WORK_Other* ek, PLW* mk);
void get_attdata_of_illusion(WORK_Other* ewk);
s32 setup_after_images(PLW* wk, u8 ix);
s32 erase_after_images(PLW* wk, u8 who);

#endif
