#ifndef EFFJ8_H
#define EFFJ8_H

#include "structs.h"
#include "types.h"

extern const s16 effj8_timer_tbl[8];
extern const s16 effj8_y_tbl[8];
extern const s32 effj8_sp_tbl[8][4];

void effect_J8_move(WORK_Other* ewk);
void dragonfly_move(WORK_Other* ewk);
void dragonfly_l_move_0(WORK_Other* ewk);
void dragonfly_l_move_1(WORK_Other* ewk);
s16 dragonfly_l_move_2(WORK_Other* ewk);
s16 dragonfly_l_move_3(WORK_Other* ewk);
s16 dragonfly_l_move_4(WORK_Other* ewk);
void dragonfly_l_move(WORK_Other* ewk);
void dragonfly_r_move_0(WORK_Other* ewk);
void dragonfly_r_move_1(WORK_Other* ewk);
s16 dragonfly_r_move_2(WORK_Other* ewk);
s16 dragonfly_r_move_3(WORK_Other* ewk);
s16 dragonfly_r_move_4(WORK_Other* ewk);
void dragonfly_r_move(WORK_Other* ewk);
void dragonfly_move_0000(WORK_Other* ewk);
void dragonfly_move_0001(WORK_Other* ewk);
void dragonfly_move_0004(WORK_Other* ewk);
void dragonfly_move_0005(WORK_Other* ewk);
void dragonfly_stop_timer(WORK_Other* ewk);
void dragonfly_line_set(WORK_Other* ewk, s16 dir_type);
void dragonfly_move_next(WORK_Other* ewk);
s32 effect_J8_init();

#endif
