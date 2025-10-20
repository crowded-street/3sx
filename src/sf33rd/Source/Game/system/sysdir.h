#ifndef SYSDIR_H
#define SYSDIR_H

#include "structs.h"
#include "types.h"

typedef enum Dipswitch {
    DIP_TAUNT_DISABLED = 1 << 0,
    DIP_TAUNT_AFTER_KO_DISABLED = 1 << 1,
    DIP_FORWARD_DASH_DISABLED = 1 << 2,
    DIP_BACK_DASH_DISABLED = 1 << 3,
    DIP_GUARD_DISABLED = 1 << 4,
    DIP_AIR_GUARD_DISABLED = 1 << 5,
    DIP_AUTO_GUARD_DISABLED = 1 << 6,
    DIP_AUTO_PARRY_DISABLED = 1 << 7,
    DIP_UNKNOWN_8 = 1 << 8,
    DIP_UNKNOWN_9 = 1 << 9,
    DIP_AIR_PARRY_DISABLED = 1 << 10,
    DIP_ANTI_AIR_PARRY_DISABLED = 1 << 11,
    DIP_RED_PARRY_DISABLED = 1 << 12,
    DIP_ABSOLUTE_GUARD_DISABLED = 1 << 13,
    DIP_EXTREME_CHIP_DAMAGE_DISABLED = 1 << 14,
    DIP_CHIP_DAMAGE_ENABLED = 1 << 15,
    DIP_JUMP_DISABLED = 1 << 16,
    DIP_HIGH_JUMP_DISABLED = 1 << 17,
    DIP_UNKNOWN_18 = 1 << 18,
    DIP_UNKNOWN_19 = 1 << 19,
    DIP_AUTO_AIR_RECOVERY_DISABLED = 1 << 21,
    DIP_HIGH_JUMP_CANCEL_DISABLED = 1 << 22,
    DIP_HIGH_JUMP_2ND_IMPACT_STYLE_ENABLED = 1 << 23,
    DIP_SEMI_AUTO_PARRY_DISABLED = 1 << 24,
    DIP_AIR_KNOCKDOWNS_DISABLED = 1 << 25,
    DIP_UNKNOWN_28 = 1 << 28,
    DIP_UNKNOWN_29 = 1 << 29,
    DIP_UNKNOWN_30 = 1 << 30,
    DIP_UNKNOWN_31 = 1 << 31,
} Dipswitch;

typedef enum Dipswitch2 {
    DIP2_TARGET_COMBO_DISABLED = 1 << 0,
    DIP2_SPECIAL_TO_SPECIAL_CANCEL_DISABLED = 1 << 1,
    DIP2_ALL_NORMALS_CANCELLABLE_DISABLED = 1 << 2,
    DIP2_SA_TO_SA_CANCEL_DISABLED = 1 << 3,
    DIP2_UNIVERSAL_OVERHEAD_DISABLED = 1 << 4,
    DIP2_UNIVERSAL_OVERHEAD_DEFAULT_INPUT_ENABLED = 1 << 5,
    DIP2_SPECIAL_MOVE_SUPER_ART_CANCEL_DISABLED = 1 << 6,
    DIP2_SUPER_ART_CANCEL_DISABLED = 1 << 7,
    DIP2_THROW_DISABLED = 1 << 8,
    DIP2_QUICK_STAND_DISABLED = 1 << 9,
    DIP2_THROW_BREAK_DISABLED = 1 << 10,
    DIP2_THROW_BREAK_LOCKOUT_ENABLED = 1 << 11,
    DIP2_EX_MOVE_DISABLED = 1 << 12,
    DIP2_SA_GAUGE_ROUND_RESET_DISABLED = 1 << 17,
    DIP2_SA_GAUGE_MAX_START_DISABLED = 1 << 18,
    DIP2_GROUND_CHAIN_COMBO_DISABLED = 1 << 20,
    DIP2_AIR_CHAIN_COMBO_DISABLED = 1 << 21,
    DIP2_UNKNOWN_22 = 1 << 22,
    DIP2_UNKNOWN_23 = 1 << 23,
    DIP2_ALL_MOVES_CANCELLABLE_BY_HIGH_JUMP_DISABLED = 1 << 24,
    DIP2_ALL_MOVES_CANCELLABLE_BY_DASH_DISABLED = 1 << 25,
    DIP2_ALL_SUPER_ARTS_AVAILABLE_DISABLED = 1 << 27,
    DIP2_CHIP_DAMAGE_KO_DISABLED = 1 << 28,
    DIP2_WHIFFED_NORMALS_BUILD_SA_GAUGE_DISABLED = 1 << 29
} Dipswitch2;

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

/// Red parry input window level
extern s16 omop_r_block_ix[2];

/// Parry input window level
extern s16 omop_b_block_ix[2];

extern s16 omop_otedama_ix[2];
extern s16 omop_stun_gauge_len[2];
extern s16 omop_stun_gauge_rcv[2];
extern s16 omop_stun_gauge_add[2];
extern s16 omop_sa_gauge_ix[2];

/// Proximity block distance level
extern s16 omop_guard_distance_ix[2];

/// EX move gauge consumption level
extern s16 omop_use_ex_gauge_ix[2];

extern u8 chainex_check[2][36];

void init_omop();
u32 sag_ikinari_max();
void get_extra_option_parameter(_EXTRA_OPTION* omop_extra);
void get_system_direction_parameter(SystemDir* sysdir_data);
u32 check_use_all_SA();
u32 check_without_SA();

#endif
