#ifndef PLS02_H
#define PLS02_H

#include "structs.h"
#include "types.h"

extern const s16 satse[20];

s32 random_16();
s32 random_16_com();
s32 random_16_ex_com();
s32 random_16_bg();
s32 random_32();
s32 random_32_com();
s32 random_32_ex_com();
s8 get_guard_direction(WORK* as, WORK* ds);
s16 cal_attdir(WORK* wk);
s16 cal_attdir_flip(s16 dir);
s16 get_kind_of_head_dm(s16 dir, s8 drl);
s16 get_kind_of_trunk_dm(s16 dir, s8 drl);
void setup_vitality(WORK* wk, s16 pno);
void cal_dm_vital_gauge_hosei(PLW* wk);
void add_sp_arts_gauge_init(PLW* wk);
void add_sp_arts_gauge_maxbit(PLW* wk);
void add_super_arts_gauge(SA_WORK* wk, s16 ix, s16 asag, u8 mf);
void dead_voice_request();
void add_to_mvxy_data(WORK* wk, u16 ix);
void setup_move_data_easy(WORK* wk, const s16* adrs, s16 prx, s16 pry);
void setup_mvxy_data(WORK* wk, u16 ix);
void cal_mvxy_speed(WORK* wk);
void add_mvxy_speed(WORK* wk);
void add_mvxy_speed_exp(WORK* wk, s16 dvp);
void add_mvxy_speed_no_use_rl(WORK* wk);
void setup_saishin_lvdir(PLW* ds, s8 gddir);
void add_sp_arts_gauge_guard(PLW* wk);
s16 check_work_position(WORK* p1, WORK* p2);
s32 set_field_hosei_flag(PLW* pl, s16 pos, s16 ix);
s16 check_work_position(WORK* p1, WORK* p2);
void setup_lvdir_after_autodir(PLW* wk);
void reset_mvxy_data(WORK* wk);
void remake_mvxy_PoSB(WORK* wk);
void remake_mvxy_PoGR(WORK* wk);
void check_body_touch();
void check_body_touch2();
s32 check_be_car_object();
s16 get_sel_hosei_tbl_ix(s16 plnum);
s16 check_work_position_bonus(WORK* hm, s16 tx);
void set_hit_stop_hit_quake(WORK* wk);
void add_sp_arts_gauge_paring(PLW* wk);
void add_sp_arts_gauge_nagenuke(PLW* wk);
void add_sp_arts_gauge_ukemi(PLW* wk);
s8 get_weight_point(WORK* wk);
void setup_butt_own_data(WORK* wk);
void add_mvxy_speed_direct(WORK* wk, s16 sx, s16 sy);
s16 check_buttobi_type(PLW* wk);
s16 check_buttobi_type2(PLW* wk);
void add_sp_arts_gauge_hit_dm(PLW* wk);
void add_sp_arts_gauge_tokushu(PLW* wk);

#endif
