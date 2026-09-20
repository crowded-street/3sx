/**
 * @file pls00_normal_states.c
 * The nm_* normal-state handlers.
 *
 * Split out of pls00.c, which was 1535 lines. Reached through plpnm_xxxxx in
 * pls00.c, which still owns the dispatch tables.
 */

#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/pls00.h"
#include "sf33rd/Source/Game/engine/pls00_internal.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/system/sysdir.h"

/* The end-of-animation marker sends the state back to standing. Three states
 * wrote this out; the 0/1 protocol is Recipe C's for a run that ends in a
 * return. */
static s32 animation_ended_to_nm_01000(PLW* wk) {
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_01000(&wk->wu);
        return 1;
    }

    return 0;
}

/* The same marker, for the three states that fall back to crouching instead. */
static s32 animation_ended_to_nm_09000(PLW* wk) {
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_09000(&wk->wu);
        return 1;
    }

    return 0;
}

/* Nothing runs on the frame a state is entered. Five states open with this. */
static s32 state_not_started(const PLW* wk) {
    return wk->wu.routine_no[3] == 0;
}

void nm_27_cg_type_check(PLW* wk);
static bool run_common_nm_attack_checks(PLW* wk);

static s32 is_elena_special_36(const PLW* wk) {
    return wk->player_number == CHAR_ELENA && wk->wu.now_koc == 0 && wk->wu.char_index == 36;
}

void nm_00000(PLW* /* unused */) { // 🟢
    // Do nothing
}

/* Defined below, next to the states that share them. */
static bool run_common_nm_attack_checks(PLW* wk);
static bool run_common_nm_attack_checks_no_turn(PLW* wk);
static bool run_attack_checks_before_leap(PLW* wk);

/* A check that may take over the normal state, as the check lists see it. Every
 * one of these functions was already being called in a boolean context; the
 * adapters below carry the narrower types - and the one guarded call - into this
 * one, so nothing is converted that was not already tested. */
typedef s32 (*NmStateCheck)(PLW* wk);

/* Run a list of checks in order and stop at the first that takes. Reports
 * whether one did, for the lists whose callers ask. */
static s32 run_nm_state_checks(PLW* wk, const NmStateCheck* checks) {
    s32 i;

    for (i = 0; checks[i] != NULL; i++) {
        if (checks[i](wk)) {
            return 1;
        }
    }

    return 0;
}

/* The adapters. Each returns the value its own `if` tested. */
static s32 nm_check_common_attacks(PLW* wk) {
    return run_common_nm_attack_checks(wk);
}

static s32 nm_check_common_attacks_no_turn(PLW* wk) {
    return run_common_nm_attack_checks_no_turn(wk);
}

static s32 nm_check_f_r_walk(PLW* wk) {
    return check_F_R_walk(wk);
}

/* nm_09000 guards its walk start on the arcade balance being on, and does
 * nothing at all when it is off. */
static s32 nm_check_arcade_walk_start(PLW* wk) {
    if (ArcadeBalance_IsEnabled()) {
        return check_arcade_walk_start(wk);
    }

    return 0;
}

/* The gauge checks take a second argument, always zero at these call sites; the
 * super-arts and before-leap checks report narrower types. */
static s32 nm_check_full_gauge_attack(PLW* wk) {
    return check_full_gauge_attack(wk, 0);
}

static s32 nm_check_full_gauge_attack2(PLW* wk) {
    return check_full_gauge_attack2(wk, 0);
}

static s32 nm_check_super_arts_attack(PLW* wk) {
    return check_super_arts_attack(wk);
}

static s32 nm_check_before_leap(PLW* wk) {
    return run_attack_checks_before_leap(wk);
}

/* The remaining check-list entries that are not a plain call.
 *
 * nm_02000 and nm_08000 each divert to a pattern state when the animation
 * reports 64; nm_27000 and nm_29000 each run a pair of checks only while the
 * lever is not held down. Each returns what its own arm returned from. */
static s32 nm_divert_to_36000_on_cg_64(PLW* wk) {
    if (wk->wu.cg_type == 64) {
        TO_nm_36000(&wk->wu);
        return 1;
    }

    return 0;
}

static s32 nm_divert_to_37000_on_cg_64(PLW* wk) {
    if (wk->wu.cg_type == 64) {
        TO_nm_37000(&wk->wu);
        return 1;
    }

    return 0;
}

static s32 nm_check_arcade_walk_start_and(PLW* wk) {
    return ArcadeBalance_IsEnabled() && check_arcade_walk_start(wk);
}

/* Both turn states run a pair of checks only while the lever is not held down. */
static s32 nm_check_unless_lever_down(PLW* wk, const NmStateCheck* checks) {
    if (wk->cp->lever_dir != 2) {
        return run_nm_state_checks(wk, checks);
    }

    return 0;
}

static s32 nm_check_bend_or_walk_unless_down(PLW* wk) {
    static const NmStateCheck checks[] = { check_bend_myself, nm_check_f_r_walk, NULL };

    return nm_check_unless_lever_down(wk, checks);
}

static s32 nm_check_stand_or_walk_unless_down(PLW* wk) {
    static const NmStateCheck checks[] = { check_stand_up, nm_check_arcade_walk_start_and, NULL };

    return nm_check_unless_lever_down(wk, checks);
}

static s32 nm_cg_type_check_27(PLW* wk) {
    nm_27_cg_type_check(wk);

    return 0;
}

void nm_01000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = { setup_kuzureochi,    nm_check_common_attacks, check_bend_myself,
                                           check_defense_lever, nm_check_f_r_walk,       NULL };

    run_nm_state_checks(wk, checks);
}

static bool run_gauge_attack_checks(PLW* wk) {
    static const NmStateCheck checks[] = {
        nm_check_full_gauge_attack, nm_check_full_gauge_attack2, nm_check_super_arts_attack, NULL
    };

    return run_nm_state_checks(wk, checks);
}

/* The six attacks every path tries first, in this order: both full-gauge
 * attacks, the super art, the special, the taunt and the throw. */
static bool run_attack_checks_before_leap(PLW* wk) {
    if (run_gauge_attack_checks(wk)) {
        return true;
    }

    if (check_special_attack(wk)) {
        return true;
    }

    if (check_chouhatsu(wk)) {
        return true;
    }

    if (check_catch_attack(wk)) {
        return true;
    }

    return false;
}

/* The ten checks both normal-attack paths run first, in this order. */
static bool run_nm_attack_checks_before_turn(PLW* wk) {
    if (check_ashimoto(wk)) {
        return true;
    }

    if (run_attack_checks_before_leap(wk)) {
        return true;
    }

    if (check_leap_attack(wk)) {
        return true;
    }

    if (check_nm_attack(wk)) {
        return true;
    }

    if (check_cg_cancel_data(wk)) {
        return true;
    }

    return false;
}

/* And the three they both run last. */
static bool run_nm_attack_checks_after_turn(PLW* wk) {
    if (check_F_R_dash(wk)) {
        return true;
    }

    if (ArcadeBalance_IsEnabled() && check_360_jump(wk)) {
        return true;
    }

    if (check_jump_ready(wk)) {
        return true;
    }

    return false;
}

static bool run_common_nm_attack_checks_no_turn(PLW* wk) {
    if (run_nm_attack_checks_before_turn(wk)) {
        return true;
    }

    return run_nm_attack_checks_after_turn(wk);
}

void nm_02000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = { animation_ended_to_nm_01000,
                                           nm_divert_to_36000_on_cg_64,
                                           nm_check_common_attacks_no_turn,
                                           check_bend_myself,
                                           check_defense_lever,
                                           nm_check_f_r_walk,
                                           NULL };

    run_nm_state_checks(wk, checks);
}

void nm_03000(PLW* wk) { // 🟡
    if (run_common_nm_attack_checks(wk)) {
        return;
    }

    if (check_bend_myself(wk)) {
        return;
    }

    if (check_walking_lv_dir(wk)) {
        wk->wu.routine_no[2] = 39;
        wk->wu.routine_no[3] = 0;
        wk->wu.cg_type = 0;
    }

    check_defense_lever(wk);
}

void nm_05000(PLW* wk) { // 🟢
    if (check_ashimoto_ex(wk) == 0) {
        jumping_cg_type_check(wk);
    }
}

void nm_07000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = { animation_ended_to_nm_01000, nm_check_common_attacks, check_defense_lever,
                                           nm_check_f_r_walk,           check_bend_myself,       NULL };

    run_nm_state_checks(wk, checks);
}

void nm_08000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = { animation_ended_to_nm_09000,
                                           nm_divert_to_37000_on_cg_64,
                                           nm_check_common_attacks,
                                           check_defense_lever,
                                           nm_check_arcade_walk_start_and,
                                           check_stand_up,
                                           NULL };

    run_nm_state_checks(wk, checks);
}

void nm_09000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = { setup_kuzureochi,    nm_check_common_attacks,    check_stand_up,
                                           check_defense_lever, nm_check_arcade_walk_start, NULL };

    run_nm_state_checks(wk, checks);
}

void nm_10000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = {
        animation_ended_to_nm_09000, nm_check_common_attacks_no_turn, check_defense_lever, check_stand_up, NULL
    };

    run_nm_state_checks(wk, checks);
}

void nm_11000(PLW* wk) { // 🔵
    // Do nothing
}

void nm_13000(PLW* wk) { // 🔵
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_01000(wk);
    }
}

/* Which of the three landing states this jump goes to, taken from the
 * direction the lever settled on. */
/* Both jump entries pick a routine from the direction check the same way; only
 * the three routine numbers the arms name differ. */
static void enter_jump_routine(PLW* wk, s16 forward, s16 backward, s16 neutral) {
    check_jump_rl_dir(wk);

    switch (wk->jpdir) {
    case JUMP_DIR_FORWARD:
        wk->wu.routine_no[2] = forward;
        break;

    case JUMP_DIR_BACKWARD:
        wk->wu.routine_no[2] = backward;
        break;

    default:
        wk->wu.routine_no[2] = neutral;
        break;
    }

    wk->wu.routine_no[3] = 0;
}

static void enter_jump_from_16000(PLW* wk) {
    enter_jump_routine(wk, 21, 23, 22);
}

void nm_16000(PLW* wk) { // 🟢
    set_new_jpdir(wk);

    if (state_not_started(wk)) {
        return;
    }

    switch (wk->wu.cg_type) {
    case 0xFF:
        enter_jump_from_16000(wk);
        break;

    case 1:
        break;
    }

    if (run_gauge_attack_checks(wk)) {
        return;
    }

    if (check_special_attack(wk)) {
        return;
    }

    if (check_chouhatsu(wk)) {
        return;
    }

    check_leap_attack(wk);
}

/* The same choice from 17000, which lands in its own three states. */
static void enter_jump_from_17000(PLW* wk) {
    enter_jump_routine(wk, 24, 26, 25);
}

void nm_17000(PLW* wk) { // 🟢 The only difference is DIP switch handling
    set_new_jpdir(wk);

    if (state_not_started(wk)) {
        return;
    }

    if (wk->wu.cg_type == 0xFF) {
        enter_jump_from_17000(wk);
        return;
    }

    if (run_gauge_attack_checks(wk)) {
        return;
    }

    if (!(wk->spmv_ng_flag & DIP_HIGH_JUMP_2ND_IMPACT_STYLE_ENABLED) && wk->high_jump_flag) {
        return;
    }

    if (check_special_attack(wk)) {
        return;
    }

    check_chouhatsu(wk);
}

void check_jump_rl_dir(PLW* wk) { // 🟢
    if (check_rl_flag(&wk->wu) == 0) {
        wk->wu.rl_flag = wk->wu.rl_waza;
        wk->cp->lever_dir = lvdir_conv[wk->cp->lever_dir];
        wk->jpdir = lvdir_conv[wk->jpdir];
    }
}

void set_new_jpdir(PLW* wk) { // 🟢
    if ((wk->cp->sw_lvbt & 1) && wk->cp->lever_dir) {
        wk->jpdir = wk->cp->lever_dir;
    }
}

static bool run_jump_attack_checks(PLW* wk) {
    static const NmStateCheck checks[] = { nm_check_before_leap, check_nm_attack, check_cg_cancel_data, NULL };

    return run_nm_state_checks(wk, checks);
}

void nm_18000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] < 2 && wk->wu.xyz[1].disp.pos > 0) {
        if (run_jump_attack_checks(wk)) {
            return;
        }

        if (check_sankaku_tobi(wk)) {
            return;
        }

        if (check_air_jump(wk)) {
            return;
        }
    }

    jumping_cg_type_check(wk);
}

static void reset_guard_for_new_state(PLW* wk) {
    wk->guard_flag = 0;

    if (!ArcadeBalance_IsEnabled()) {
        clear_chainex_check(wk->wu.id);
    }
}

static bool run_early_attack_checks(PLW* wk) {
    static const NmStateCheck checks[] = { nm_check_before_leap, check_leap_attack, check_nm_attack, NULL };

    return run_nm_state_checks(wk, checks);
}

static void handle_jump_attack_state(PLW* wk) {
    if (run_early_attack_checks(wk)) {
        return;
    }

    if (check_cg_cancel_data(wk)) {
        return;
    }

    if (ArcadeBalance_IsEnabled() && check_360_jump(wk)) {
        return;
    }

    check_jump_ready(wk);
}

static void handle_jump_defense_state(PLW* wk) {
    if (run_early_attack_checks(wk)) {
        return;
    }

    check_cg_cancel_data(wk);
}

/* Clearing the guard and handing over to the jump attack, which two arms do
 * together. */
static void enter_jump_attack_state(PLW* wk) {
    reset_guard_for_new_state(wk);
    handle_jump_attack_state(wk);
}

/* The same for the jump defense. */
static void enter_jump_defense_state(PLW* wk) {
    reset_guard_for_new_state(wk);
    handle_jump_defense_state(wk);
}

static bool run_forward_jump_checks(PLW* wk) {
    if (run_early_attack_checks(wk)) {
        return true;
    }

    if (check_cg_cancel_data(wk)) {
        return true;
    }

    if (check_turn_to_back(wk)) {
        return true;
    }

    if (check_F_R_dash(wk)) {
        return true;
    }

    if (ArcadeBalance_IsEnabled() && check_360_jump(wk)) {
        return true;
    }

    if (check_jump_ready(wk)) {
        return true;
    }

    return false;
}

/* What a grounded jump-cancel does with a forward lever: try the jump, then a
 * crouch, then a walk. Each `break` in the original left the switch with
 * nothing after it, so returning here reaches the same place. */
static void run_low_pat_ground_step(PLW* wk) {
    if (run_forward_jump_checks(wk)) {
        return;
    }

    if (check_bend_myself(wk)) {
        return;
    }

    check_F_R_walk(wk);
}

/* The airborne equivalent: the jump, then standing up, then the arcade-only
 * walk start. Not shared with the grounded one - the middle and last steps are
 * different calls. */
static void run_high_pat_ground_step(PLW* wk) {
    if (run_forward_jump_checks(wk)) {
        return;
    }

    if (check_stand_up(wk)) {
        return;
    }

    if (ArcadeBalance_IsEnabled()) {
        check_arcade_walk_start(wk);
    }
}

/* The grounded half of the jump-cancel dispatch. Split from the airborne half
 * below rather than left as one function: the two switches carry the same five
 * case labels but call different state entries, so they cannot be merged, and
 * at cc 18 the parent was well over the threshold with both inline. */
static void jumping_cg_type_low_pat(PLW* wk) {
    switch (wk->wu.cg_type) {
    case 0xFF:
        reset_guard_for_new_state(wk);

        TO_nm_01000(&wk->wu);
        break;

    case 2:
        enter_jump_attack_state(wk);
        break;

    case 7:
        enter_jump_defense_state(wk);
        break;

    case 3:
        reset_guard_for_new_state(wk);

        run_low_pat_ground_step(wk);
        break;

    case 64:
        reset_guard_for_new_state(wk);

        if (wk->wu.pat_status < 14) {
            TO_nm_36000(&wk->wu);
        } else {
            TO_nm_38000(&wk->wu);
        }

        break;
    }
}

/* The airborne half. Same five labels, different destinations. */
static void jumping_cg_type_high_pat(PLW* wk) {
    switch (wk->wu.cg_type) {
    case 0xFF:
        reset_guard_for_new_state(wk);

        TO_nm_09000(&wk->wu);
        break;

    case 2:
        enter_jump_attack_state(wk);
        break;

    case 7:
        enter_jump_defense_state(wk);
        break;

    case 3:
        reset_guard_for_new_state(wk);

        run_high_pat_ground_step(wk);
        break;

    case 64:
        reset_guard_for_new_state(wk);

        TO_nm_37000(&wk->wu);
        break;
    }
}

void jumping_cg_type_check(PLW* wk) { // 🟡
    if (wk->wu.pat_status < 32) {
        jumping_cg_type_low_pat(wk);
    } else {
        jumping_cg_type_high_pat(wk);
    }
}

void jumping_guard_type_check(PLW* wk) { // 🟢
    switch (wk->wu.cg_type) {
    case 0xFF:
    case 64:
    case 2:
    case 3:
    case 7:
        wk->guard_flag = 0;
        break;
    }
}

static bool run_common_nm_attack_checks(PLW* wk) {
    if (run_nm_attack_checks_before_turn(wk)) {
        return true;
    }

    if (check_turn_to_back(wk)) {
        return true;
    }

    return run_nm_attack_checks_after_turn(wk);
}

void nm_27000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = { animation_ended_to_nm_01000,
                                           nm_check_common_attacks,
                                           nm_check_bend_or_walk_unless_down,
                                           nm_cg_type_check_27,
                                           NULL };

    run_nm_state_checks(wk, checks);
}

/* cg_type 2 of the nm_27 state: if the opponent is not attacking and the player
 * is not going into a defensive state, step the script back one command and run
 * it again. Each `break` left the switch with nothing after it. */
static void rewind_script_if_not_defending(PLW* wk) {
    if (check_em_catt(wk) == 0) {
        return;
    }

    if (check_defense_kind(wk) != 0) {
        return;
    }

    wk->wu.cg_ix -= wk->wu.cgd_type;
    char_move_z(&wk->wu);
}

void nm_27_cg_type_check(PLW* wk) { // 🟢
    if (state_not_started(wk)) {
        return;
    }

    if (wk->sa_stop_flag == 1) {
        return;
    }

    switch (wk->wu.cg_type) {
    case 1:
        check_defense_kind(wk);
        break;

    case 2:
        rewind_script_if_not_defending(wk);
        break;

    case 64:
        if (wk->wu.routine_no[2] == 29) {
            wk->wu.routine_no[2] = 37;
        } else {
            wk->wu.routine_no[2] = 36;
        }

        wk->wu.routine_no[3] = 0;
        wk->wu.cg_type = 0;
        break;
    }
}

void nm_29000(PLW* wk) { // 🟡
    static const NmStateCheck checks[] = { animation_ended_to_nm_09000,
                                           nm_check_common_attacks,
                                           nm_check_stand_or_walk_unless_down,
                                           nm_cg_type_check_27,
                                           NULL };

    run_nm_state_checks(wk, checks);
}

static void dispatch_by_pat_status(PLW* wk, void (*on_low_pat_status)(WORK*), void (*on_high_pat_status)(WORK*)) {
    if (wk->wu.pat_status < 32) {
        on_low_pat_status(&wk->wu);
    } else {
        on_high_pat_status(&wk->wu);
    }
}

/* Everything a player can start from this state, in the order nm_31000 tried
 * them: the two EX gauges, the super art, a special, a taunt, a throw, the
 * universal overhead, and finally a normal. Each `break` in the original left
 * the switch with nothing after it, so returning here reaches the same place. */
static void try_any_attack(PLW* wk) {
    if (run_attack_checks_before_leap(wk)) {
        return;
    }

    if (check_leap_attack(wk)) {
        return;
    }

    check_nm_attack(wk);
}

void nm_31000(PLW* wk) { // 🟢
    if (state_not_started(wk)) {
        return;
    }

    switch (wk->wu.cg_type) {
    case 0:
        try_any_attack(wk);
        break;

    case 64:
        dispatch_by_pat_status(wk, TO_nm_36000, TO_nm_37000);
        break;

    case 0xFF:
        dispatch_by_pat_status(wk, TO_nm_01000, TO_nm_09000);
        break;
    }
}

void nm_34000(PLW* wk) { // 🟢
    if (state_not_started(wk)) {
        return;
    }

    switch (wk->wu.cg_type) {
    case 0xFF:
    case 64:
        TO_nm_18000_01(&wk->wu);
        break;

    default:
        if (wk->wu.routine_no[3] < 3) {
            break;
        }

        dispatch_by_pat_status(wk, TO_nm_36000, TO_nm_37000);
        break;
    }
}

/* Both the 36000 and 39000 cancels go back to routine 1; the sub-routine they
 * resume at depends on whether this is the first character's first koc. */
static void return_to_routine_1(PLW* wk) {
    if (wk->wu.now_koc == 0 && wk->wu.char_index == 0) {
        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 1;
    } else {
        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 0;
    }
}

void nm_36000(PLW* wk) { // 🟢
    if (wk->wu.cg_type == 0xFF) {
        return_to_routine_1(wk);
    } else if (is_elena_special_36(wk)) {
        exset_char_move_init(&wk->wu, 0, 0);
        wk->wu.routine_no[2] = 1;
        wk->wu.routine_no[3] = 1;
    }

    nm_01000(wk);
}

void nm_37000(PLW* wk) { // 🟢
    if (wk->wu.cg_type == 0xFF) {
        wk->wu.routine_no[2] = 9;
        wk->wu.routine_no[3] = 0;
    }

    nm_09000(wk);
}

/* The triangle jump and the double jump are console-only additions; arcade
 * balance has neither. */
static bool try_console_air_moves(PLW* wk) {
    if (ArcadeBalance_IsEnabled()) {
        return false;
    }

    if (check_sankaku_tobi(wk)) {
        return true;
    }

    if (check_air_jump(wk)) {
        return true;
    }

    return false;
}

void nm_38000(PLW* wk) { // 🟡
    bool in_air = true;

    if (!ArcadeBalance_IsEnabled()) {
        in_air = (wk->wu.xyz[1].disp.pos > 0);
    }

    if (wk->wu.routine_no[3] < 2 && in_air) {
        if (run_jump_attack_checks(wk)) {
            return;
        }

        if (try_console_air_moves(wk)) {
            return;
        }
    }

    jumping_cg_type_check(wk);
}

void nm_39000(PLW* wk) { // 🟢
    if (wk->wu.cg_type == 0xFF) {
        return_to_routine_1(wk);
    }

    nm_01000(wk);
}

void nm_40000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] && wk->wu.cg_type == 0xFF) {
        wk->wu.routine_no[3] = 9;
    }
}

void nm_42000(PLW* wk) { // 🟡
    if (ArcadeBalance_IsEnabled()) {
        if (wk->wu.routine_no[3] == 2 || wk->wu.routine_no[3] == 3) {
            if (FUN_06120790(wk)) {
                return;
            }
        }
    }

    if (wk->wu.routine_no[3] < 4) {
        return;
    }

    jumping_cg_type_check(wk);
}

void nm_45000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] == 3 && run_jump_attack_checks(wk)) {
        return;
    }

    switch (wk->wu.cg_type) {
    case 64:
        dispatch_by_pat_status(wk, TO_nm_36000, TO_nm_37000);
        break;

    case 0xFF:
        dispatch_by_pat_status(wk, TO_nm_01000, TO_nm_09000);
        break;

    default:
        jumping_cg_type_check(wk);
        break;
    }
}

void nm_47000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] > 3) {
        jumping_cg_type_check(wk);
    }
}

void nm_48000(PLW* wk) { // 🟢
    jumping_cg_type_check(wk);
}

void nm_49000(PLW* wk) { // 🟢
    jumping_cg_type_check(wk);
}

void nm_51000(PLW* /* unused */) { // 🟢
    // Do nothing
}

void nm_52000(PLW* wk) { // 🟢
    if (run_gauge_attack_checks(wk)) {
        return;
    }

    check_special_attack(wk);
}

void nm_55000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] > 1) {
        jumping_cg_type_check(wk);
    }
}

void nm_57000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] > 2) {
        jumping_cg_type_check(wk);
    }
}
