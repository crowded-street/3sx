#ifndef EFF44_H
#define EFF44_H

#include "structs.h"
#include "types.h"

extern const s16 scr_obj_num44[10];
extern const s16 eff44_data_tbl0[16];
extern const s16 eff44_data_tbl1[8];
extern const s16 eff44_data_tbl2[16];
extern const s16 eff44_data_tbl3[56];
extern const s16 eff44_data_tbl4[8];
extern const s16 eff44_data_tbl5[24];
extern const s16 eff44_data_tbl6[24];
extern const s16 eff44_data_tbl7[40];
extern const s16 eff44_data_tbl8[32];
extern const s16 eff44_data_tbl9[16];
extern const s16* scr_obj_data44[10];

void effect_44_move(WORK_Other* ewk);
s32 effect_44_init(s16 type);

#endif
