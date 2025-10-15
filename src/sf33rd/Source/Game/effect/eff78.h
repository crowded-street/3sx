#ifndef EFF78_H
#define EFF78_H

#include "structs.h"
#include "types.h"

extern const s16 eff78_data_tbl[4];
extern const s16 crow_char_tbl[3][3];

void effect_78_move(WORK_Other* ewk);
s32 crow_fuss_check(WORK_Other* ewk);
void crow_fuss_move(WORK_Other* ewk);
s32 effect_78_init();

#endif
