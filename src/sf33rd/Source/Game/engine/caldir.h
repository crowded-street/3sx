#ifndef CALDIR_H
#define CALDIR_H

#include "structs.h"
#include "types.h"

extern const s32 rate_256_table[256][2];

s16 caldir_pos_256(s16 x1, s16 x2, s16 y1, s16 y2);
s16 caldir_pos_032(s16 x1, s16 x2, s16 y1, s16 y2);
void add_pos_dir_064(WORK* wk, s16 sp);
s16 cal_move_quantity2(s16 x1, s16 x2, s16 y1, s16 y2);
s16 cal_move_quantity3(WORK* wk, s16 tm);
void cmsd_all_x_speed_data(MotionState* cc);
void cmsd_all_y_speed_data(MotionState* cc);
void cmsd_swx_0(MotionState* cc);
void cmsd_swy_0(MotionState* cc);
void cmsd_swx_1(MotionState* cc);
void cmsd_swy_1(MotionState* cc);
void cmsd_swx_2(MotionState* cc);
void cmsd_swy_2(MotionState* cc);
void cmsd_x_initial_speed(MotionState* cc);
void cmsd_y_initial_speed(MotionState* cc);
void cmsd_x_delta_speed(MotionState* cc);
void cmsd_y_delta_speed(MotionState* cc);
void cal_all_speed_data(WORK* wk, s16 tm, s16 x1, s16 y1, s8 xsw, s8 ysw);
void cal_initial_speed(WORK* wk, s16 tm, s16 x1, s16 y1);
void cal_initial_speed_y(WORK* wk, s16 tm, s16 y1);
void cal_delta_speed(WORK* wk, s16 tm, s16 x1, s16 y1, s8 xsw, s8 ysw);
s16 cal_top_of_position_y(WORK* wk);
s16 cal_time_of_sign_change(WORK* wk);
s16 cal_move_dir_forecast(WORK* wk, s16 tm);
s16 remake_2_10(s16 num, s16 keta);

#endif
