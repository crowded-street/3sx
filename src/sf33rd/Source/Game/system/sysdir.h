#ifndef SYSDIR_H
#define SYSDIR_H

#include "structs.h"
#include "types.h"

extern const s16 use_ex_gauge[4];
extern const s16 guard_distance[4];
extern const s16 sa_gauge_omake[4];
extern const s16 stun_gauge_omake[4];
extern const s16 stun_gauge_r_omake[4];
extern const s16 stun_gauge_len_omake[5];
extern const s16 blok_b_omake[4];
extern const s16 blok_r_omake[4];
extern const s16 sag_stock_omake[11];
extern const s16 sag_length_omake[17];
extern const s16 base_vital_omake[7];

extern s16 omop_dokidoki;
extern s16 omop_round_timer;
extern s16 omop_cockpit;
extern s16 omop_sa_bar_disp[2];
extern s16 omop_st_bar_disp[2];
extern s16 omop_vt_bar_disp[2];
extern s16 omop_vital_init[2];
extern s16 omop_sag_len_ix[2];
extern s16 omop_sag_max_ix[2];
extern s16 omop_vital_ix[2];
extern s16 omop_r_block_ix[2];
extern s16 omop_b_block_ix[2];
extern s16 omop_otedama_ix[2];
extern s16 omop_stun_gauge_len[2];
extern s16 omop_stun_gauge_rcv[2];
extern s16 omop_stun_gauge_add[2];
extern s16 omop_sa_gauge_ix[2];
extern s16 omop_guard_distance_ix[2];
extern s16 omop_use_ex_gauge_ix[2];
extern u8 chainex_check[2][36];

void init_omop();
u32 sag_ikinari_max();
void get_extra_option_parameter(_EXTRA_OPTION* omop_extra);
void get_system_direction_parameter(DirData* sysdir_data);
u32 check_use_all_SA();
u32 check_without_SA();

#endif
