#ifndef CMB_WIN_H
#define CMB_WIN_H

#include "types.h"

extern const u8 cmb_pos_tbl[2][21];
extern const u8 cmb_window_move_tbl[7];
extern const u8 cmb_window_time_tbl[7];
extern const u16 combo_score_tbl[12][2];

extern u8 cst_write[2];
extern u8 cst_read[2];
extern s8 cmb_calc_now[];
extern s8 last_hit_time;
extern s8 sarts_finish_flag[2];
extern s8 cmb_all_stock[];
extern s16 score_calc[2][12];
extern s16 calc_hit[2][10];
extern u8 end_flag[2];
extern u8 sa_kind;
extern s16 hit_num;
extern s8 bonus_pts[2];
extern s8 paring_attack[2];
extern s8 rever_attack[2];
extern s8 first_attack;
extern s8 cmb_stock[2];
extern s16 old_cmb_flag[2];

void combo_cont_init();
void combo_cont_main();
void combo_control(s8 PL);
void check_and_set_combo(s8 PL);
void combo_hensuu_clear(s8 PL);
void combo_rp_clear_check(s8 PL);
void super_arts_finish_check(s8 PL);
void super_arts_last_check(s8 PL);
void first_attack_pts_check(s8 PL);
s32 reversal_check(s8 PL);
void reversal_continue_check(s8 PL);
void reversal_attack_pts_check(s8 PL);
s32 paring_check(s8 PL);
void hit_combo_check(s8 PL);
s32 arts_finish_check(s8 PL);
s32 arts_finish_check2(u8 PL);
u32 SCORE_CALCULATION(s8 PL);
void SCORE_PLUS(s8 PL, u32 PTS);
void combo_window_push(s8 PL, s8 KIND);
void combo_window_trans(s8 PL);
void training_disp_data_set(s8 PL, s16 hnum);

#endif
