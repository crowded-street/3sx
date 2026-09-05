/**
 * @file hitcheck_damage.c
 * Player damage, parry status, cancel, chip-damage, and scaling processing.
 */

#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "arcade/arcade_balance.h"
#include "bin2obj/exchange.h"
#include "bin2obj/gauge.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff02.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitefef.h"
#include "sf33rd/Source/Game/engine/hitefpl.h"
#include "sf33rd/Source/Game/engine/hitplef.h"
#include "sf33rd/Source/Game/engine/hitplpl.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/system/sysdir.h"

#include <SDL3/SDL.h>

static void apply_head_damage_variant(PLW* as, PLW* ds) {
    if (check_head_damage(ds->wu.routine_no[2])) {
        ds->wu.routine_no[2] = get_kind_of_head_dm(as->wu.dir_atthit, ds->wu.dm_rl);
    }
}

static void apply_trunk_damage_variant(PLW* as, PLW* ds) {
    if (check_trunk_damage(ds->wu.routine_no[2])) {
        ds->wu.routine_no[2] = get_kind_of_trunk_dm(as->wu.dir_atthit, ds->wu.dm_rl);
    }
}

static bool uses_ground_hand_damage(s16 damage_point) {
    if (damage_point == 4) {
        return true;
    }

    if (damage_point == 5) {
        return true;
    }

    if (damage_point == 6) {
        return true;
    }

    return damage_point == 7;
}

static void resolve_ground_damage_variant(PLW* as, PLW* ds) {
    if (ds->wu.pat_status >= 32) {
        ds->wu.routine_no[2] = get_kagami_damage(ds->wu.routine_no[2]);
        return;
    }

    if (ds->dm_point == 0) {
        apply_head_damage_variant(as, ds);
        return;
    }

    if (ds->dm_point == 1) {
        apply_head_damage_variant(as, ds);
        return;
    }

    if (uses_ground_hand_damage(ds->dm_point)) {
        ds->wu.routine_no[2] = get_grd_hand_damage(ds->wu.routine_no[2]);
    }

    apply_trunk_damage_variant(as, ds);
}

static void apply_air_damage_reaction(PLW* as, PLW* ds) {
    as->wu.hf.hit.player = 2;
    ds->wu.kezurare_flag = 0;
    dm_reaction_init_set(as, ds);

    if (as->wu.att.dipsw & 0x10) {
        ds->wu.routine_no[2] = get_sky_sp_damage(ds->wu.routine_no[2]);
    } else {
        ds->wu.routine_no[2] = get_sky_nm_damage(ds->wu.routine_no[2]);
    }
}

static void apply_ground_damage_reaction(PLW* as, PLW* ds) {
    as->wu.hf.hit.player = 1;
    ds->wu.kezurare_flag = 0;
    dm_reaction_init_set(as, ds);

    if (as->wu.zu_flag == 0) {
        resolve_ground_damage_variant(as, ds);
    }
}

static void finish_player_damage(PLW* as, PLW* ds, s8 gddir) {
    ds->wu.routine_no[1] = 1;
    ds->wu.routine_no[3] = 0;
    grade_add_clean_hits((WORK_Other*)as);
    check_guard_miss(&as->wu, ds, gddir);
    effect_02_init(&as->wu, ds->dm_point, 1, ds->wu.dm_rl);
    dm_status_copy(&as->wu, &ds->wu);
    same_dm_stop(&as->wu, &ds->wu);
    as->wu.cmwk[8]++;
    as->wu.cmwk[15]++;
    ds->wu.dm_count_up++;

    if (ds->wu.xyz[1].disp.pos < 0) {
        ds->wu.xyz[1].cal = 0;
    }

    add_combo_work(as, ds);
    hit_pattern_extdat_check(&as->wu);

    if (ds->atemi_flag && ds->atemi_point != ds->dm_point) {
        ds->atemi_flag = 0;
    }

    paring_ctr_vs[Play_Type][ds->wu.id] = 0;
    paring_counter[ds->wu.id] = 0;
    paring_bonus_r[ds->wu.id] = 0;
}

typedef enum {
    PLAYER_DAMAGE_AIR_DEFENSE,
    PLAYER_DAMAGE_GROUND_DEFENSE,
    PLAYER_DAMAGE_AIR_REACTION,
    PLAYER_DAMAGE_GROUND_REACTION,
    PLAYER_DAMAGE_GUARD,
    PLAYER_DAMAGE_PARRY,
} PlayerDamagePath;

static bool uses_forced_damage_path(PLW* as, PLW* ds) {
    if (ds->guard_flag == 3) {
        return true;
    }

    if (as->wu.att.guard == 0) {
        return true;
    }

    return ds->py->flag != 0;
}

static bool uses_air_damage_path(PLW* ds) {
    if (ds->wu.xyz[1].disp.pos > 0) {
        return true;
    }

    return check_pat_status(&ds->wu);
}

static PlayerDamagePath select_air_or_ground_damage_path(PLW* ds, PlayerDamagePath air_path) {
    if (uses_air_damage_path(ds)) {
        return air_path;
    }

    return PLAYER_DAMAGE_GROUND_DEFENSE;
}

static PlayerDamagePath select_player_damage_path(PLW* as, PLW* ds) {
    if (!uses_forced_damage_path(as, ds)) {
        return select_air_or_ground_damage_path(ds, PLAYER_DAMAGE_AIR_DEFENSE);
    }

    if (ds->wu.pat_status == 10) {
        ds->wu.xyz[1].cal = 0;
        return PLAYER_DAMAGE_GROUND_DEFENSE;
    }

    if (ds->wu.pat_status == 12 && ds->wu.xyz[1].disp.pos < 6) {
        ds->wu.xyz[1].cal = 0;
        return PLAYER_DAMAGE_GROUND_DEFENSE;
    }

    if (ds->wu.routine_no[1] != 1) {
        return select_air_or_ground_damage_path(ds, PLAYER_DAMAGE_AIR_DEFENSE);
    }

    if (uses_air_damage_path(ds)) {
        return PLAYER_DAMAGE_AIR_REACTION;
    }

    return PLAYER_DAMAGE_GROUND_REACTION;
}

static PlayerDamagePath resolve_player_defense(PLW* as, PLW* ds, s8 gddir, PlayerDamagePath path) {
    s32 defense_result;

    if (path == PLAYER_DAMAGE_AIR_DEFENSE) {
        defense_result = defense_sky(as, ds, gddir);
    } else if (path == PLAYER_DAMAGE_GROUND_DEFENSE) {
        defense_result = defense_ground(as, ds, gddir);
    } else {
        return path;
    }

    switch (defense_result) {
    case 0:
        return PLAYER_DAMAGE_PARRY;

    case 1:
        return PLAYER_DAMAGE_GUARD;

    default:
        return path == PLAYER_DAMAGE_AIR_DEFENSE ? PLAYER_DAMAGE_AIR_REACTION : PLAYER_DAMAGE_GROUND_REACTION;
    }
}

void plef_at_vs_player_damage_union(PLW* as, PLW* ds, s8 gddir) { // 🟡
    // CPS3 lacks the port-side training chip-damage metadata resets kept below.
    PlayerDamagePath path;

    ds->wu.dm_guard_success = -1;
    path = resolve_player_defense(as, ds, gddir, select_player_damage_path(as, ds));

    switch (path) {
    case PLAYER_DAMAGE_AIR_REACTION:
        apply_air_damage_reaction(as, ds);
        break;

    case PLAYER_DAMAGE_GROUND_REACTION:
        apply_ground_damage_reaction(as, ds);
        break;

    case PLAYER_DAMAGE_GUARD:
        set_guard_status(as, ds);
        return;

    case PLAYER_DAMAGE_PARRY:
        set_paring_status(as, ds);
        return;

    default:
        break;
    }

    finish_player_damage(as, ds, gddir);
}

static bool is_running_damage_reaction(PLW* ds) {
    if (ds->wu.routine_no[2] == 89) {
        return true;
    }

    return ds->wu.routine_no[2] == 90;
}

static bool should_apply_running_damage_reaction(PLW* as, PLW* ds) {
    if (!is_running_damage_reaction(ds)) {
        return false;
    }

    if (ds->running_f != 1) {
        return false;
    }

    if (!Dsas_dir_table[as->wu.att.dir]) {
        return false;
    }

    if (check_work_position(&as->wu, &ds->wu)) {
        return ds->move_distance > 0;
    }

    return ds->move_distance < 0;
}

void dm_reaction_init_set(PLW* as, PLW* ds) { // 🟢
    ds->wu.routine_no[2] = as->wu.att.reaction;

    if (should_apply_running_damage_reaction(as, ds)) {
        ds->wu.routine_no[2] = 99;
    }

    ds->wu.routine_no[2] = change_damage_attribute(as, as->wu.at_attribute, ds->wu.routine_no[2]);
}

void set_guard_status(PLW* as, PLW* ds) { // 🟡
    // CPS3 always starts the guard effect; local keeps the semi-auto parry option's presentation behavior.
    if (as->wu.att.hs_you == 0 && as->wu.att.hs_me == 0) {
        ds->wu.routine_no[2] = ds->wu.old_rno[2];
    } else {
        ds->wu.routine_no[1] = 1;
        ds->wu.routine_no[3] = 0;

        if (ds->spmv_ng_flag & DIP_SEMI_AUTO_PARRY_DISABLED) {
            effect_02_init(&as->wu, ds->dm_point, 2, ds->wu.dm_rl);
        }

        dm_status_copy(&as->wu, &ds->wu);
        same_dm_stop(&as->wu, &ds->wu);

        if (ds->wu.xyz[1].disp.pos < 0) {
            ds->wu.xyz[1].cal = 0;
        }

        ds->wu.dm_piyo = 0;
        as->wu.cmwk[8]++;
        add_sp_arts_gauge_guard(as);
        ds->wu.dm_arts_point = 0;
        grade_add_guard_success(ds->wu.id);
    }

    hit_pattern_extdat_check(&as->wu);
}

const s8 sel_sp_ch_tbl[12] = { 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0 };

const s16 sel_hs_add_tbl[6] = { 4, 3, 2, 1, 0, 0 };

static s16 get_parry_hit_stop_index(PLW* as) {
    s16 hsadix = 4;

    if ((as->wu.kind_of_waza & 0xF8) == 0) {
        hsadix = (as->wu.kind_of_waza / 2) & 3;
    }

    return hsadix;
}

static bool uses_enhanced_parry_hit_stop(s16 stop_type) {
    if (stop_type == 0) {
        return true;
    }

    if (stop_type == 2) {
        return true;
    }

    return stop_type == 4;
}

static bool uses_standard_parry_hit_stop(s16 stop_type) {
    if (stop_type == 1) {
        return true;
    }

    if (stop_type == 3) {
        return true;
    }

    if (stop_type == 5) {
        return true;
    }

    if (stop_type == 6) {
        return true;
    }

    if (stop_type == 7) {
        return true;
    }

    if (stop_type == 8) {
        return true;
    }

    return stop_type == 9;
}

static void apply_parry_hit_stop(PLW* as, PLW* ds, s16 hsadix) {
    s16 stop_type = (as->wu.xyz[1].disp.pos > 0) + (ds->wu.routine_no[2] - 31) * 2;

    if (uses_enhanced_parry_hit_stop(stop_type)) {
        ds->wu.dm_stop = -15;
        as->wu.hit_stop = sel_hs_add_tbl[hsadix] + 16;
        as->wu.hit_quake = sel_hs_add_tbl[hsadix] + 16;
    } else if (uses_standard_parry_hit_stop(stop_type)) {
        ds->wu.dm_stop = -15;
        as->wu.hit_stop = 16;
        as->wu.hit_quake = 16;
    } else {
        ds->wu.dm_stop = 0;
        as->wu.hit_stop = 0;
        as->wu.hit_quake = 0;
    }
}

static bool should_remake_parry_movement(PLW* as) {
    if (as->wu.pat_status < 0xE) {
        return false;
    }

    if (as->wu.pat_status >= 31) {
        return false;
    }

    if (as->wu.work_id != 1) {
        return false;
    }

    return sel_sp_ch_tbl[as->wu.kind_of_waza >> 3] == 0;
}

static void award_parry_bonus(PLW* ds) {
    if (Bonus_Game_Flag != 0) {
        return;
    }

    if (!(ds->spmv_ng_flag & DIP_AUTO_PARRY_DISABLED)) {
        return;
    }

    paring_bonus_r[ds->wu.id] = 1;
    paring_ctr_vs[Play_Type][ds->wu.id]++;

    if (paring_ctr_vs[Play_Type][ds->wu.id] > 39) {
        paring_ctr_vs[Play_Type][ds->wu.id] = 39;
    }

    paring_counter[ds->wu.id] = parisucc_pts[Play_Type][paring_ctr_vs[Play_Type][ds->wu.id] - 1];
}

static void apply_parry_status(PLW* as, PLW* ds) {
    s16 hsadix = get_parry_hit_stop_index(as);

    ds->wu.routine_no[1] = 0;
    ds->wu.routine_no[3] = 0;
    waza_compel_all_init2(ds);
    dm_status_copy(&as->wu, &ds->wu);
    ds->wu.dm_piyo = 0;
    ds->wu.cg_type = 0;
    apply_parry_hit_stop(as, ds, hsadix);
    ds->wu.dm_quake = 0;

    if (ds->wu.xyz[1].disp.pos < 0) {
        ds->wu.xyz[1].cal = 0;
    }

    ds->wu.dm_arts_point = 0;

    if (should_remake_parry_movement(as)) {
        remake_mvxy_PoGR(&as->wu);
    }

    award_parry_bonus(ds);
    as->wu.cmwk[8]++;
}

void set_paring_status(PLW* as, PLW* ds) { // 🟡
    // CPS3 always awards parry bonuses;
    // local's default DIP disables auto-parry, so this only differs when auto-parry is enabled.
    if ((as->wu.att.hs_you == 0) && (as->wu.att.hs_me == 0)) {
        ds->wu.routine_no[2] = ds->wu.old_rno[2];
    } else {
        apply_parry_status(as, ds);
    }

    hit_pattern_extdat_check(&as->wu);
}

s32 check_normal_attack(u8 waza) { // 🟢
    return sel_sp_ch_tbl[waza >> 3] == 0;
}

static void apply_ground_chain_cancel(WORK* as) {
    s16 i;

    if (!(((PLW*)as)->spmv_ng_flag2 & DIP2_ALL_MOVES_CANCELLABLE_BY_HIGH_JUMP_DISABLED)) {
        as->cg_cancel |= 1;
    }

    if (!(((PLW*)as)->spmv_ng_flag2 & DIP2_ALL_MOVES_CANCELLABLE_BY_DASH_DISABLED)) {
        as->cg_cancel |= 2;
    }

    if (((PLW*)as)->spmv_ng_flag2 & DIP2_GROUND_CHAIN_COMBO_DISABLED) {
        return;
    }

    i = 0;

    if (((PLW*)as)->player_number == 4) {
        as->cg_meoshi = chain_hidou_nm_ground_table[as->kow & 7];
        as->cg_cancel |= 8;
    } else {
        as->cg_meoshi = i | chain_normal_ground_table[as->kow & 7];
        as->cg_cancel |= 8;
    }
}

static void apply_air_chain_cancel(WORK* as) {
    s16 i;

    if (((PLW*)as)->spmv_ng_flag2 & DIP2_AIR_CHAIN_COMBO_DISABLED) {
        return;
    }

    if (hikusugi_check(as)) {
        return;
    }

    i = 0;

    if (((PLW*)as)->player_number == 7) {
        as->cg_meoshi = chain_hidou_nm_air_table[as->kow & 7];
        as->cg_cancel |= 8;
    } else {
        as->cg_meoshi = i | chain_normal_air_table[as->kow & 7];
        as->cg_cancel |= 8;
    }
}

static void apply_normal_move_chain_cancel(WORK* as) {
    switch (plpat_rno_filter[as->routine_no[2]]) {
    case 9:
        if (as->routine_no[3] != 1) {
            break;
        }

        /* fallthrough */

    case 1:
        apply_ground_chain_cancel(as);
        break;

    case 2:
        apply_air_chain_cancel(as);
        break;
    }
}

static void apply_hit_pattern_extension(WORK* as) {
    switch ((as->cg_extdat & 0xC0) + ((as->cg_extdat & 0x3F) != 0)) {
    case 0x80:
        char_move_z(as);
        break;

    case 0x40:
        as->cg_ctr = 1;

        if (!ArcadeBalance_IsEnabled()) {
            as->cg_extdat = 0;
        }

        break;

    case 0x81:
        setup_comm_abbak(as);
        as->cg_ix = ((as->cg_extdat & 0x3F) - 1) * as->cgd_type - as->cgd_type;
        as->cg_next_ix = 0;
        char_move_z(as);
        break;

    case 0x41:
        as->cg_ctr = 1;
        /* fallthrough */

    case 0x1:
        setup_comm_abbak(as);
        as->cg_ix = ((as->cg_extdat & 0x3F) - 1) * as->cgd_type - as->cgd_type;
        as->cg_next_ix = 0;

        if (!ArcadeBalance_IsEnabled()) {
            as->cg_extdat = 0;
        }

        break;
    }
}

static void restrict_target_combo(WORK* as) {
    if (!(((PLW*)as)->spmv_ng_flag2 & DIP2_TARGET_COMBO_DISABLED)) {
        return;
    }

    if (!(as->cg_cancel & 8)) {
        return;
    }

    if (as->kow & 0xF8) {
        return;
    }

    if (as->kow & 6) {
        as->cg_cancel &= 0xF7;
        as->cg_meoshi = 0;
    } else if (as->cg_meoshi & 0x110) {
        as->cg_meoshi &= 0xF99F;
    } else {
        as->cg_cancel &= 0xF7;
        as->cg_meoshi = 0;
    }
}

static void enable_super_art_cancel(WORK* as) {
    if (((PLW*)as)->spmv_ng_flag2 & DIP2_SA_TO_SA_CANCEL_DISABLED) {
        return;
    }

    if (!(as->kow & 0x60)) {
        return;
    }

    as->cg_cancel |= 0x40;
}

static void enable_special_move_cancel(WORK* as) {
    if (((PLW*)as)->spmv_ng_flag2 & DIP2_SPECIAL_TO_SPECIAL_CANCEL_DISABLED) {
        return;
    }

    if (as->kow & 0x60) {
        return;
    }

    if (!(as->kow & 0xF8)) {
        return;
    }

    as->cg_cancel |= 0x60;
}

static void enable_normal_move_cancel(WORK* as) {
    if (((PLW*)as)->spmv_ng_flag2 & DIP2_ALL_NORMALS_CANCELLABLE_DISABLED) {
        return;
    }

    if (as->kow & 0xF8) {
        return;
    }

    switch (plpat_rno_filter[as->routine_no[2]]) {
    case 9:
        if (as->routine_no[3] != 1) {
            break;
        }

        /* fallthrough */

    case 1:
    case 2:
        as->cg_cancel |= 0x60;
        break;
    }
}

static bool can_apply_normal_move_chain(WORK* as) {
    if (as->kow & 0xF8) {
        return false;
    }

    if (as->routine_no[1] != 4) {
        return false;
    }

    return as->routine_no[2] < 0x10;
}

void hit_pattern_extdat_check(WORK* as) { // 🟡
    // CPS3 leaves cg_extdat latched; local's extra cancel rewrites are DIP-gated and default off.
    apply_hit_pattern_extension(as);

    if (as->work_id != 1) {
        return;
    }

    restrict_target_combo(as);
    enable_super_art_cancel(as);
    enable_special_move_cancel(as);
    enable_normal_move_cancel(as);

    if (can_apply_normal_move_chain(as)) {
        apply_normal_move_chain_cancel(as);
    }
}

static bool prevents_chip_damage_ko(WORK* as, s16 player_id) {
    return as->no_death_attack || (plw[player_id].spmv_ng_flag2 & DIP2_CHIP_DAMAGE_KO_DISABLED);
}

static s16 damage_attacker_player_id(WORK* attacker) {
    if (attacker->work_id == 1) {
        return attacker->id;
    }

    return ((WORK_Other*)attacker)->master_id;
}

s16 check_dm_att_guard(WORK* as, WORK* ds, s16 kom) { // 🟡
    // CPS3 uses raw kezuri_pow; local defaults match CPS3 for chip DIPs, while non-arcade keeps the kom scaling.
    s16 curr_id;
    s16 divisor;
    s16 rnum;

    rnum = 0;
    ds->kezurare_flag = 0;
    curr_id = damage_attacker_player_id(as);

    if (!(plw[curr_id].spmv_ng_flag & DIP_CHIP_DAMAGE_ENABLED)) {
        as->kezuri_pow = 0;
    }

    if (!as->kezuri_pow) {
        ds->dm_vital = 0;
        return rnum;
    }

    if (ds->dm_vital == 0) {
        return rnum;
    }

    ds->kezurare_flag = 1;
    divisor = as->kezuri_pow;

    if (!ArcadeBalance_IsEnabled()) {
        divisor /= kom;
    }

    ds->dm_vital = ds->dm_vital / divisor;

    if (ds->dm_vital == 0) {
        ds->dm_vital = 1;
    }

    if (ds->dm_vital <= ds->vital_new) {
        return rnum;
    }

    if (prevents_chip_damage_ko(as, curr_id)) {
        ds->dm_vital = ds->vital_new;
    } else {
        ds->dm_guard_success = ds->routine_no[2];
        rnum = 1;
    }

    return rnum;
}

static bool supports_blocking_chip_damage(WORK* as, TAMA* tama) {
    if (as->work_id != 4) {
        return false;
    }

    if (as->id != 13) {
        return false;
    }

    if (tama->kz_blocking == 0) {
        return false;
    }

    return as->kezuri_pow;
}

s16 check_dm_att_blocking(WORK* as, WORK* ds, s16 dnum) { // 🟡
    // CPS3 lacks the port-side training chip-damage metadata kept below.
    s16 rnum = 0;
    TAMA* tama = (TAMA*)as->my_effadrs;

    ds->kezurare_flag = 0;

    if (!supports_blocking_chip_damage(as, tama)) {
        ds->dm_vital = 0;
        return rnum;
    }

    if (ds->dm_vital == 0) {
        return rnum;
    }

    ds->kezurare_flag = 1;

    if (as->kezuri_pow) {
        ds->dm_vital = ds->dm_vital / as->kezuri_pow;
    } else {
        ds->dm_vital = 0;
    }

    if (ds->dm_vital == 0) {
        ds->dm_vital = 1;
    }

    if (ds->dm_vital <= ds->vital_new) {
        return rnum;
    }

    if (as->no_death_attack) {
        ds->dm_vital = ds->vital_new;
    } else {
        ds->dm_guard_success = dnum;
        rnum = 1;
    }

    return rnum;
}

static bool uses_medium_damage_modifier(s16 status) {
    if (status == 32) {
        return true;
    }

    if (status == 3) {
        return true;
    }

    return status == 25;
}

static bool uses_high_damage_modifier(s16 status) {
    if (status == 7) {
        return true;
    }

    if (status == 23) {
        return true;
    }

    return status == 35;
}

static bool uses_double_damage_modifier(s16 status) {
    if (status == 1) {
        return true;
    }

    if (status == 21) {
        return true;
    }

    return status == 37;
}

static void apply_status_damage_modifier(PLW* ds) {
    if (uses_medium_damage_modifier(ds->wu.pat_status)) {
        ds->wu.dm_vital = (ds->wu.dm_vital * 125) / 100;
    } else if (uses_high_damage_modifier(ds->wu.pat_status)) {
        ds->wu.dm_vital = (ds->wu.dm_vital * 150) / 100;
    } else if (uses_double_damage_modifier(ds->wu.pat_status)) {
        ds->wu.dm_vital *= 2;
    }
}

static void apply_throw_damage_scaling(PLW* as, PLW* ds) {
    ds->wu.dm_vital = (ds->wu.dm_vital) * (as->tk_nage + 32) / 32;

    if ((as->tk_nage -= 2) < 0) {
        as->tk_nage = 0;
    }
}

static void apply_strike_damage_scaling(PLW* as, PLW* ds) {
    ds->wu.dm_vital = (ds->wu.dm_vital) * (as->tk_dageki + 32) / 32;

    if ((as->tk_dageki -= 2) < 0) {
        as->tk_dageki = 0;
    }
}

static void apply_attack_damage_scaling(PLW* as, PLW* ds) {
    if (!ds->wu.dm_vital) {
        return;
    }

    if (as->wu.routine_no[1] == 2) {
        apply_throw_damage_scaling(as, ds);
    }

    if (as->wu.routine_no[1] == 4) {
        apply_strike_damage_scaling(as, ds);
    }

    ds->utk_nage = as->tk_nage;
    ds->utk_dageki = as->tk_dageki;
}

static void apply_stun_scaling(PLW* as, PLW* ds) {
    if (!ds->wu.dm_piyo) {
        return;
    }

    ds->wu.dm_piyo = ds->wu.dm_piyo * (as->tk_kizetsu + 32) / 32;

    if ((as->tk_kizetsu -= 2) < 0) {
        as->tk_kizetsu = 0;
    }

    ds->utk_kizetsu = as->tk_kizetsu;
}

static void apply_damage_vital_backup(PLW* as, PLW* ds) {
    switch (as->dm_vital_use) {
    case 1:
        ds->wu.dm_vital += as->dm_vital_backup;
        as->dm_vital_backup = 0;
        break;

    case 2:
        as->dm_vital_backup /= 2;
        ds->wu.dm_vital += as->dm_vital_backup;
        break;
    }
}

void set_damage_and_piyo(PLW* as, PLW* ds) { // 🟡
    // CPS3 has fixed stun gain; local's extra-option stun multiplier is neutral by default.
    cal_damage_vitality(as, ds);
    ds->wu.dm_piyo = _add_piyo_gauge[as->player_number][as->wu.att.piyo];
    ds->wu.dm_piyo = ds->wu.dm_piyo * stun_gauge_omake[omop_stun_gauge_add[(ds->wu.id + 1) & 1]] / 32;
    apply_status_damage_modifier(ds);
    apply_attack_damage_scaling(as, ds);
    apply_stun_scaling(as, ds);
    as->wu.at_ten_ix = remake_score_index(ds->wu.dm_vital);
    cal_combo_waribiki(as, ds);
    cal_dm_vital_gauge_hosei(ds);
    cal_combo_waribiki2(ds);

    if (as->wu.work_id != 1) {
        return;
    }

    apply_damage_vital_backup(as, ds);
}

s16 remake_score_index(s16 dmv) { // 🟢
    s16 i;

    for (i = 0; i < 16; i++) {
        if (dmv < rsix_r_table[i][0]) {
            break;
        }
    }

    return rsix_r_table[i][1];
}

void same_dm_stop(WORK* as, WORK* ds) { // 🟢
    if (as->work_id != 1) {
        return;
    }

    if (!(as->att.dipsw & 1)) {
        return;
    }

    if (ds->xyz[1].disp.pos <= 0 && (ds->vital_new - ds->dm_vital) >= -2) {
        return;
    }

    switch ((ds->dm_stop < 0) + ((as->att.hs_me < 0) * 2)) {
    case 1:
        ds->dm_stop = -as->att.hs_me;
        /* fallthrough */

    case 2:
        ds->dm_stop = -as->att.hs_me;
        break;

    default:
        ds->dm_stop = as->att.hs_me;
        break;
    }
}
