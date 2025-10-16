#ifndef HITCHECK_H
#define HITCHECK_H

#include "structs.h"
#include "types.h"

extern const s16 Dsas_dir_table[16];
extern const s8 sel_sp_ch_tbl[12];
extern const s16 sel_hs_add_tbl[6];
extern const u16 chain_normal_ground_table[];
extern const u16 chain_hidou_nm_ground_table[];
extern const u16 chain_normal_air_table[];
extern const u16 chain_hidou_nm_air_table[];
extern const u8 plpat_rno_filter[];
extern const s16 rsix_r_table[17][2];
extern const s16 attr_flame_tbl[83];
extern const s16 attr_thunder_tbl[83];
extern const s16 attr_freeze_tbl[83];
extern const s16 sky_nm_damage_tbl[83];
extern const s16 sky_sp_damage_tbl[83];
extern const s16 kagami_damage_tbl[83];
extern const s16 grd_hand_damage_tbl[83];
extern const u8 hddm_damage_tbl[83];
extern const u8 trdm_damage_tbl[83];
extern const u8 ttk_damage_req_tbl[83];
extern const u8 parisucc_pts[2][40];
extern const u8 dm_oiuchi_catch[32];

extern HS hs[32];

extern WORK* q_hit_push[32];
extern s8 ca_check_flag; // size: 0x1, address: 0x5790F4

void make_red_blocking_time(s16 id, s16 ix, s16 num);
void hit_check_main_process();
s16 set_judge_result();
void check_result_extra();
void set_caught_status(s16 ix);
s32 check_pat_status(WORK* wk);
s16 check_blocking_flag(PLW* as, PLW* ds);
void setup_catch_atthit(WORK* as, WORK* ds);
void set_catch_hit_mark_pos(WORK* as, WORK* ds);
void set_struck_status(s16 ix);
void cal_hit_mark_pos(WORK* as, WORK* ds, s16 ix2, s16 ix);
void plef_at_vs_player_damage_union(PLW* as, PLW* ds, s8 gddir);
void dm_reaction_init_set(PLW* as, PLW* ds);
void set_guard_status(PLW* as, PLW* ds);
void set_paring_status(PLW* as, PLW* ds);
s32 check_normal_attack(u8 waza);
void hit_pattern_extdat_check(WORK* as);
s16 check_dm_att_guard(WORK* as, WORK* ds, s16 kom);
s16 check_dm_att_blocking(WORK* as, WORK* ds, s16 dnum);
void set_damage_and_piyo(PLW* as, PLW* ds);
s16 remake_score_index(s16 dmv);
void same_dm_stop(WORK* as, WORK* ds);
s32 defense_sky(PLW* as, PLW* ds, s8 gddir);
void blocking_point_count_up(PLW* wk);
s32 defense_ground(PLW* as, PLW* ds, s8 gddir);
void setup_dm_rl(WORK* as, WORK* ds);
void dm_status_copy(WORK* as, WORK* ds);
void add_combo_work(PLW* as, PLW* ds);
void nise_combo_work(PLW* as, PLW* ds, s16 num);
void cal_combo_waribiki(PLW* as, PLW* ds);
void cal_combo_waribiki2(PLW* ds);
void catch_hit_check();
void attack_hit_check();
s16 hit_check_subroutine(WORK* wk1, WORK* wk2, const s16* hd1, const s16* hd2);
s32 hit_check_x_only(WORK* wk1, WORK* wk2, s16* hd1, s16* hd2);
void cal_hit_mark_position(WORK* wk1, WORK* wk2, s16* hd1, s16* hd2);
void get_target_att_position(WORK* wk, s16* tx, s16* ty);
s16 get_att_head_position(WORK* wk);
void hit_push_request(WORK* hpr_wk);
void clear_hit_queue();
s16 change_damage_attribute(PLW* as, u16 atr, u16 ix);
s16 get_sky_nm_damage(u16 ix);
s16 get_sky_sp_damage(u16 ix);
s16 get_kagami_damage(u16 ix);
s16 get_grd_hand_damage(u16 ix);
u8 check_head_damage(s16 ix);
u8 check_trunk_damage(s16 ix);
u8 check_ttk_damage_request(s16 ix);

#endif
