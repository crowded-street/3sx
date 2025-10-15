#ifndef EFF85_H
#define EFF85_H

#include "structs.h"
#include "types.h"

extern const s16 eff85_char_index_tbl[9];

void effect_85_move(WORK_Other* ewk);
void eff85_0000(WORK_Other* ewk);
void eff85_0100(WORK_Other* ewk);
void eff85_1000(WORK_Other* ewk);
void eff85_common();
void eff85_3000(WORK_Other* ewk);
void eff85_5000(WORK_Other* ewk);
void eff85_7000(WORK_Other* ewk);
void eff85_8000(WORK_Other* ewk);
void eff85_9000(WORK_Other* ewk);
s32 swallow_sprize_check();
void eff85_0200(WORK_Other* ewk);
s32 effect_85_init();

#endif
