#ifndef EFFE2_H
#define EFFE2_H

#include "structs.h"
#include "types.h"

void effect_E2_move(WORK_Other* ewk);
s32 effect_E2_init(PLW* wk, const s16* data, s16 color_code, u8 ff);
s32 setup_accessories(PLW* wk, u8 data);

#endif
