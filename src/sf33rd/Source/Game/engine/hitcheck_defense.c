/**
 * @file hitcheck_defense.c
 * Air and ground defense resolution.
 */

#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/system/sysdir.h"

static bool can_attempt_cps3_air_parry(PLW* as, PLW* ds) {
    if (ds->py->flag != 0) {
        return false;
    }

    if (ds->guard_flag & 2) {
        return false;
    }

    return as->wu.att.guard & 4;
}

static bool can_cps3_air_parry(PLW* ds) {
    if (ds->spmv_ng_flag & DIP_AIR_PARRY_DISABLED) {
        return false;
    }

    return ds->cp->waza_flag[5] != 0;
}

static bool can_cps3_anti_air_parry(PLW* ds) {
    if (ds->spmv_ng_flag & DIP_ANTI_AIR_PARRY_DISABLED) {
        return false;
    }

    return ds->cp->waza_flag[6] != 0;
}

static s32 resolve_cps3_air_parry(PLW* as, PLW* ds, s16 reaction) {
    blocking_point_count_up(ds);
    as->wu.hf.hit.player = 0x80;
    ds->wu.routine_no[2] = reaction;

    if (check_dm_att_blocking(&as->wu, &ds->wu, 7)) {
        return 2;
    }

    return 0;
}

static bool is_cps3_air_guard_rejected(PLW* as, PLW* ds, s8 gddir) {
    if (!(as->wu.att.guard & 0x20)) {
        return true;
    }

    if (ds->guard_flag & 1) {
        return true;
    }

    if (ds->spmv_ng_flag & DIP_AIR_GUARD_DISABLED) {
        return true;
    }

    return !(gddir & ds->saishin_lvdir);
}

static s32 finish_cps3_air_guard(PLW* as, PLW* ds) {
    as->wu.hf.hit.player = 0x20;
    ds->wu.routine_no[2] = 7;

    if (check_dm_att_guard(&as->wu, &ds->wu, 2)) {
        return 2;
    }

    return 1;
}

s32 defense_sky_cps3(PLW* as, PLW* ds, s8 gddir) { // 🟢
    if (can_attempt_cps3_air_parry(as, ds)) {
        if (can_cps3_air_parry(ds)) {
            return resolve_cps3_air_parry(as, ds, 0x22);
        }

        if (can_cps3_anti_air_parry(ds)) {
            return resolve_cps3_air_parry(as, ds, 0x23);
        }
    }

    if (is_cps3_air_guard_rejected(as, ds, gddir)) {
        return 2;
    }

    return finish_cps3_air_guard(as, ds);
}

typedef struct {
    PLW* attacker;
    PLW* defender;
    s8 just_now;
    s8 attack_attribute;
    s8 automatic_parry;
    s8 automatic_guard;
} Ps2AirDefense;

static bool can_ps2_red_air_parry(const Ps2AirDefense* defense) {
    if (defense->defender->spmv_ng_flag & DIP_RED_PARRY_DISABLED) {
        return false;
    }

    if (defense->defender->cp->waza_flag[5] >=
        grdb2[defense->defender->wu.id][defense->attack_attribute]) {
        return true;
    }

    return defense->automatic_parry;
}

static bool can_ps2_air_parry(const Ps2AirDefense* defense) {
    if (defense->defender->py->flag != 0) {
        return false;
    }

    if (defense->defender->guard_flag & 2) {
        return false;
    }

    if (!(defense->attacker->wu.att.guard & 4)) {
        return false;
    }

    if (defense->just_now) {
        return can_ps2_red_air_parry(defense);
    }

    if (defense->defender->spmv_ng_flag & DIP_AIR_PARRY_DISABLED) {
        return false;
    }

    if (defense->defender->cp->waza_flag[5] != 0) {
        return true;
    }

    return defense->automatic_parry;
}

static s32 resolve_ps2_air_parry(const Ps2AirDefense* defense) {
    blocking_point_count_up(defense->defender);
    defense->attacker->wu.hf.hit.player = 0x80;
    defense->defender->wu.routine_no[2] = 0x22;

    if (check_dm_att_blocking(&defense->attacker->wu, &defense->defender->wu, 7)) {
        return 2;
    }

    return 0;
}

static bool bypasses_manual_guard_input(PLW* defender, s8 automatic_guard, s8 just_now) {
    if (defender->auto_guard) {
        return true;
    }

    if (automatic_guard) {
        return true;
    }

    if (defender->spmv_ng_flag & DIP_ABSOLUTE_GUARD_DISABLED) {
        return false;
    }

    return just_now;
}

typedef struct {
    PLW* attacker;
    PLW* defender;
    s8 direction;
    u16 attack_guard_mask;
    u16 disabled_flag;
    u16 input_mask;
    s8 bypasses_manual_input;
} GuardRejectionCheck;

static bool is_guard_rejected(const GuardRejectionCheck* check) {
    if (!(check->attacker->wu.att.guard & check->attack_guard_mask)) {
        return true;
    }

    if (check->defender->guard_flag & 1) {
        return true;
    }

    if (check->defender->spmv_ng_flag & check->disabled_flag) {
        return true;
    }

    if (check->bypasses_manual_input) {
        return false;
    }

    if (!(check->defender->saishin_lvdir & check->direction)) {
        return true;
    }

    return check->defender->cp->sw_lvbt & check->input_mask;
}

static s32 finish_ps2_air_guard(const Ps2AirDefense* defense) {
    defense->attacker->wu.hf.hit.player = 0x20;
    defense->defender->wu.routine_no[2] = 7;

    if (check_dm_att_guard(&defense->attacker->wu, &defense->defender->wu, 2)) {
        return 2;
    }

    return 1;
}

s32 defense_sky_ps2(PLW* as, PLW* ds, s8 gddir) { // 🔴
    Ps2AirDefense defense;
    GuardRejectionCheck guard_check;

    defense.attacker = as;
    defense.defender = ds;
    defense.automatic_parry = (ds->spmv_ng_flag & DIP_AUTO_PARRY_DISABLED) == 0;
    defense.automatic_guard = (ds->spmv_ng_flag & DIP_AUTO_GUARD_DISABLED) == 0;

    if (ds->dead_flag) {
        ds->guard_flag = 3;
    }

    defense.just_now = 0;

    if (ds->guard_chuu != 0 && ds->guard_chuu < 5) {
        defense.just_now = 1;
        defense.attack_attribute = check_normal_attack(as->wu.kind_of_waza);
    }

    if (can_ps2_air_parry(&defense)) {
        return resolve_ps2_air_parry(&defense);
    }

    guard_check.attacker = defense.attacker;
    guard_check.defender = defense.defender;
    guard_check.direction = gddir;
    guard_check.attack_guard_mask = 32;
    guard_check.disabled_flag = DIP_AIR_GUARD_DISABLED;
    guard_check.input_mask = 3;
    guard_check.bypasses_manual_input =
        bypasses_manual_guard_input(defense.defender, defense.automatic_guard, defense.just_now);

    if (is_guard_rejected(&guard_check)) {
        return 2;
    }

    return finish_ps2_air_guard(&defense);
}

s32 defense_sky(PLW* as, PLW* ds, s8 gddir) { // 🟡
    // CPS3 lacks the local red-parry/auto-guard extensions and has a second air-parry branch.
    if (ArcadeBalance_IsEnabled()) {
        return defense_sky_cps3(as, ds, gddir);
    } else {
        return defense_sky_ps2(as, ds, gddir);
    }
}

static bool is_standing_blocking_state(PLW* wk) {
    if (wk->wu.routine_no[1] == 0) {
        if (wk->wu.routine_no[2] > 30) {
            return wk->wu.routine_no[2] < 36;
        }
    }

    return false;
}

static bool is_crouching_blocking_state(PLW* wk) {
    if (wk->wu.routine_no[1] == 1) {
        if (wk->wu.routine_no[2] > 3) {
            return wk->wu.routine_no[2] < 8;
        }
    }

    return false;
}

void blocking_point_count_up(PLW* wk) { // 🟡
    // CPS3 always scores parries; local's default DIP disables auto-parry, so this only differs when auto-parry is
    // enabled.
    wk->kind_of_blocking = 0;

    if (is_standing_blocking_state(wk)) {
        wk->kind_of_blocking = 1;
    }

    if (is_crouching_blocking_state(wk)) {
        wk->kind_of_blocking = 2;
    }

    if (wk->spmv_ng_flag & DIP_AUTO_PARRY_DISABLED) {
        grade_add_blocking(wk);
    }
}

static s32 resolve_standing_guard_block(PLW* as, PLW* ds);
static s32 resolve_crouching_guard_block(PLW* as, PLW* ds);

typedef struct {
    PLW* attacker;
    PLW* defender;
    s8 just_now;
    s8 attack_attribute;
} Cps3GroundDefense;

static bool can_cps3_standing_parry(const Cps3GroundDefense* defense) {
    if (defense->just_now) {
        return defense->defender->cp->waza_flag[3] >=
               grdb[defense->defender->wu.id][defense->attack_attribute][0];
    }

    if (defense->attacker->wu.jump_att_flag == 0) {
        return defense->defender->cp->waza_flag[3] != 0;
    }

    return defense->defender->cp->waza_flag[12] != 0;
}

typedef bool (*Cps3ParryCheck)(const Cps3GroundDefense* defense);
typedef s32 (*Cps3ParryResolver)(PLW* attacker, PLW* defender);

typedef struct {
    u16 guard_mask;
    u16 disabled_flag;
    Cps3ParryCheck can_parry;
    Cps3ParryResolver resolve_parry;
} Cps3ParryConfig;

static bool try_cps3_parry(const Cps3GroundDefense* defense, s32* result, const Cps3ParryConfig* config) {
    if (!(defense->attacker->wu.att.guard & config->guard_mask)) {
        return false;
    }

    if (defense->defender->spmv_ng_flag & config->disabled_flag) {
        return false;
    }

    if (!config->can_parry(defense)) {
        return false;
    }

    *result = config->resolve_parry(defense->attacker, defense->defender);
    return true;
}

static bool can_cps3_crouching_parry(const Cps3GroundDefense* defense) {
    if (defense->just_now) {
        return defense->defender->cp->waza_flag[4] >=
               grdb[defense->defender->wu.id][defense->attack_attribute][1];
    }

    return defense->defender->cp->waza_flag[4] != 0;
}

static bool try_cps3_ground_parry(const Cps3GroundDefense* defense, s32* result) {
    static const Cps3ParryConfig standing = { 2, DIP_UNKNOWN_8, can_cps3_standing_parry,
                                              resolve_standing_guard_block };
    static const Cps3ParryConfig crouching = { 1, DIP_UNKNOWN_9, can_cps3_crouching_parry,
                                               resolve_crouching_guard_block };

    if (defense->defender->py->flag != 0) {
        return false;
    }

    if (defense->defender->guard_flag & 2) {
        return false;
    }

    if (!(defense->attacker->wu.att.guard & 3)) {
        return false;
    }

    if (try_cps3_parry(defense, result, &standing)) {
        return true;
    }

    return try_cps3_parry(defense, result, &crouching);
}

static bool configure_cps3_guard_animation(const Cps3GroundDefense* defense) {
    switch (defense->attacker->wu.att.guard & 0x18) {
    case 8:
        if (!(defense->defender->cp->sw_lvbt & 2)) {
            return false;
        }

        defense->defender->wu.routine_no[2] = 6;
        return true;

    case 16:
        if (defense->defender->cp->sw_lvbt & 2) {
            return false;
        }

        defense->defender->wu.routine_no[2] = 5;
        return true;

    default:
        if (defense->defender->cp->sw_lvbt & 2) {
            defense->defender->wu.routine_no[2] = 6;
        } else {
            defense->defender->wu.routine_no[2] = 5;
        }

        return true;
    }
}

static s32 finish_ground_guard(PLW* attacker, PLW* defender) {
    if (defender->wu.routine_no[2] == 5 && check_attbox_dir(defender) == 0) {
        defender->wu.routine_no[2] = 4;
    }

    if (check_dm_att_guard(&attacker->wu, &defender->wu, 1)) {
        return 2;
    }

    return 1;
}

s32 defense_ground_cps3(PLW* as, PLW* ds, s8 gddir) { // 🟢
    Cps3GroundDefense defense;
    GuardRejectionCheck guard_check;
    s32 parry_result;

    defense.attacker = as;
    defense.defender = ds;
    defense.just_now = 0;
    defense.attack_attribute = 0;

    if (ds->guard_chuu != 0 && ds->guard_chuu < 5) {
        defense.just_now = 1;
        defense.attack_attribute = check_normal_attack(as->wu.kind_of_waza);
    }

    if (try_cps3_ground_parry(&defense, &parry_result)) {
        return parry_result;
    }

    guard_check.attacker = defense.attacker;
    guard_check.defender = defense.defender;
    guard_check.direction = gddir;
    guard_check.attack_guard_mask = 0x18;
    guard_check.disabled_flag = DIP_GUARD_DISABLED;
    guard_check.input_mask = 1;
    guard_check.bypasses_manual_input = defense.defender->auto_guard;

    if (is_guard_rejected(&guard_check)) {
        return 2;
    }

    if (!configure_cps3_guard_animation(&defense)) {
        return 2;
    }

    defense.attacker->wu.hf.hit.player = 0x10;
    return finish_ground_guard(defense.attacker, defense.defender);
}

static s32 resolve_standing_guard_block(PLW* as, PLW* ds) {
    blocking_point_count_up(ds);
    as->wu.hf.hit.player = 64;

    if (check_attbox_dir(ds) == 0) {
        ds->wu.routine_no[2] = 31;
    } else {
        ds->wu.routine_no[2] = 32;
    }

    if (check_dm_att_blocking(&as->wu, &ds->wu, 5)) {
        return 2;
    }

    return 0;
}

static s32 resolve_crouching_guard_block(PLW* as, PLW* ds) {
    blocking_point_count_up(ds);
    as->wu.hf.hit.player = 64;
    ds->wu.routine_no[2] = 33;

    if (check_dm_att_blocking(&as->wu, &ds->wu, 6)) {
        return 2;
    }

    return 0;
}

typedef struct {
    PLW* attacker;
    PLW* defender;
    s8 just_now;
    s8 attack_attribute;
    s8 automatic_parry;
    s8 automatic_guard;
} Ps2GroundDefense;

typedef enum {
    PARRY_STANDING,
    PARRY_CROUCHING,
} ParryStance;

static bool attack_supports_parry_stance(const Ps2GroundDefense* defense, ParryStance stance) {
    if (stance == PARRY_STANDING) {
        return defense->attacker->wu.att.guard & 2;
    }

    return defense->attacker->wu.att.guard & 1;
}

static bool can_red_parry(const Ps2GroundDefense* defense, ParryStance stance) {
    if (defense->defender->spmv_ng_flag & DIP_RED_PARRY_DISABLED) {
        return false;
    }

    if (stance == PARRY_STANDING) {
        return (defense->defender->cp->waza_flag[3] >=
                grdb[defense->defender->wu.id][defense->attack_attribute][0]) ||
               defense->automatic_parry;
    }

    return !(defense->defender->cp->waza_flag[4] <
             grdb[defense->defender->wu.id][defense->attack_attribute][1]) ||
           defense->automatic_parry;
}

static bool is_standard_parry_disabled(const Ps2GroundDefense* defense, ParryStance stance) {
    if (stance == PARRY_STANDING) {
        return defense->defender->spmv_ng_flag & DIP_UNKNOWN_8;
    }

    return defense->defender->spmv_ng_flag & DIP_UNKNOWN_9;
}

static bool can_air_parry(const Ps2GroundDefense* defense, ParryStance stance) {
    if (defense->defender->spmv_ng_flag & DIP_ANTI_AIR_PARRY_DISABLED) {
        return false;
    }

    if (stance == PARRY_STANDING) {
        return defense->defender->cp->waza_flag[12] != 0 || defense->automatic_parry;
    }

    return defense->defender->cp->waza_flag[4] != 0 || defense->automatic_parry;
}

static bool can_ground_parry(const Ps2GroundDefense* defense, ParryStance stance) {
    if (stance == PARRY_STANDING) {
        return defense->defender->cp->waza_flag[3] != 0 || defense->automatic_parry;
    }

    return defense->defender->cp->waza_flag[4] != 0 || defense->automatic_parry;
}

static bool can_standard_parry(const Ps2GroundDefense* defense, ParryStance stance) {
    if (is_standard_parry_disabled(defense, stance)) {
        return false;
    }

    if (defense->attacker->wu.jump_att_flag) {
        return can_air_parry(defense, stance);
    }

    return can_ground_parry(defense, stance);
}

static bool try_ps2_parry(const Ps2GroundDefense* defense, ParryStance stance, s32* result) {
    bool can_parry;

    if (!attack_supports_parry_stance(defense, stance)) {
        return false;
    }

    if (defense->just_now) {
        can_parry = can_red_parry(defense, stance);
    } else {
        can_parry = can_standard_parry(defense, stance);
    }

    if (!can_parry) {
        return false;
    }

    if (stance == PARRY_STANDING) {
        *result = resolve_standing_guard_block(defense->attacker, defense->defender);
    } else {
        *result = resolve_crouching_guard_block(defense->attacker, defense->defender);
    }

    return true;
}

static bool try_ps2_ground_parry(const Ps2GroundDefense* defense, s32* result) {
    if (defense->defender->py->flag != 0) {
        return false;
    }

    if (defense->defender->guard_flag & 2) {
        return false;
    }

    if (!(defense->attacker->wu.att.guard & 3)) {
        return false;
    }

    if (try_ps2_parry(defense, PARRY_STANDING, result)) {
        return true;
    }

    return try_ps2_parry(defense, PARRY_CROUCHING, result);
}

static bool is_manual_guard_input_rejected(const Ps2GroundDefense* defense, s8 gddir) {
    if (bypasses_manual_guard_input(defense->defender, defense->automatic_guard, defense->just_now)) {
        return false;
    }

    if (!(defense->defender->saishin_lvdir & gddir)) {
        return true;
    }

    return defense->defender->cp->sw_lvbt & 1;
}

static bool is_ps2_ground_guard_rejected(const Ps2GroundDefense* defense, s8 gddir) {
    if (!(defense->attacker->wu.att.guard & 0x18)) {
        return true;
    }

    if (defense->defender->guard_flag & 1) {
        return true;
    }

    if (defense->defender->spmv_ng_flag & DIP_GUARD_DISABLED) {
        return true;
    }

    return is_manual_guard_input_rejected(defense, gddir);
}

static bool configure_directional_guard_animation(const Ps2GroundDefense* defense, bool requires_down,
                                                  s16 animation) {
    bool is_down = defense->defender->cp->sw_lvbt & 2;

    if (defense->automatic_guard == 0 && is_down != requires_down) {
        return false;
    }

    defense->defender->wu.routine_no[2] = animation;
    return true;
}

static bool configure_ps2_guard_animation(const Ps2GroundDefense* defense) {
    switch (defense->attacker->wu.att.guard & 0x18) {
    case 8:
        return configure_directional_guard_animation(defense, true, 6);

    case 16:
        return configure_directional_guard_animation(defense, false, 5);

    default:
        if (defense->defender->cp->sw_lvbt & 2) {
            defense->defender->wu.routine_no[2] = 6;
        } else {
            defense->defender->wu.routine_no[2] = 5;
        }

        return true;
    }
}

s32 defense_ground_ps2(PLW* as, PLW* ds, s8 gddir) { // 🔴
    Ps2GroundDefense defense;
    s32 parry_result;

    defense.attacker = as;
    defense.defender = ds;
    defense.automatic_parry = (ds->spmv_ng_flag & DIP_AUTO_PARRY_DISABLED) == 0;
    defense.automatic_guard = (ds->spmv_ng_flag & DIP_AUTO_GUARD_DISABLED) == 0;

    if (ds->dead_flag) {
        ds->guard_flag = 3;
    }

    defense.just_now = 0;

    if (ds->guard_chuu != 0 && ds->guard_chuu < 5) {
        defense.just_now = 1;
        defense.attack_attribute = check_normal_attack(as->wu.kind_of_waza);
    }

    if (try_ps2_ground_parry(&defense, &parry_result)) {
        return parry_result;
    }

    if (is_ps2_ground_guard_rejected(&defense, gddir)) {
        return 2;
    }

    if (!configure_ps2_guard_animation(&defense)) {
        return 2;
    }

    defense.attacker->wu.hf.hit.player = 16;
    return finish_ground_guard(defense.attacker, defense.defender);
}

s32 defense_ground(PLW* as, PLW* ds, s8 gddir) { // 🟡
    // CPS3 lacks the local red-parry/auto-guard extensions in the PS2 ground-defense path.
    if (ArcadeBalance_IsEnabled()) {
        return defense_ground_cps3(as, ds, gddir);
    } else {
        return defense_ground_ps2(as, ds, gddir);
    }
}
