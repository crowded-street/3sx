/**
 * @file pls00_normal_states.c
 * The nm_* normal-state handlers.
 *
 * Split out of pls00.c, which was 1535 lines. Reached through plpnm_xxxxx in
 * pls00.c, which still owns the dispatch tables.
 */

#include "sf33rd/Source/Game/engine/pls00.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/engine/pls00_internal.h"

void nm_27_cg_type_check(PLW* wk);
static bool run_common_nm_attack_checks(PLW* wk);

static s32 is_elena_special_36(const PLW* wk) {
    return wk->player_number == CHAR_ELENA && wk->wu.now_koc == 0 && wk->wu.char_index == 36;
}

void nm_00000(PLW* /* unused */) { // 🟢
    // Do nothing
}

void nm_01000(PLW* wk) { // 🟡
    if (setup_kuzureochi(wk)) {
        return;
    }

    if (run_common_nm_attack_checks(wk)) {
        return;
    }

    if (check_bend_myself(wk)) {
        return;
    }

    if (check_defense_lever(wk)) {
        return;
    }

    check_F_R_walk(wk);
}

/* The six attacks every path tries first, in this order: both full-gauge
 * attacks, the super art, the special, the taunt and the throw. */
static bool run_attack_checks_before_leap(PLW* wk) {
    if (check_full_gauge_attack(wk, 0)) {
        return true;
    }

    if (check_full_gauge_attack2(wk, 0)) {
        return true;
    }

    if (check_super_arts_attack(wk)) {
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
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_01000(&wk->wu);
        return;
    }

    if (wk->wu.cg_type == 64) {
        TO_nm_36000(&wk->wu);
        return;
    }

    if (run_common_nm_attack_checks_no_turn(wk)) {
        return;
    }

    if (check_bend_myself(wk)) {
        return;
    }

    if (check_defense_lever(wk)) {
        return;
    }

    check_F_R_walk(wk);
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
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_01000(&wk->wu);
        return;
    }

    if (run_common_nm_attack_checks(wk)) {
        return;
    }

    if (check_defense_lever(wk)) {
        return;
    }

    if (check_F_R_walk(wk)) {
        return;
    }

    check_bend_myself(wk);
}

void nm_08000(PLW* wk) { // 🟡
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_09000(&wk->wu);
        return;
    }

    if (wk->wu.cg_type == 64) {
        TO_nm_37000(&wk->wu);
        return;
    }

    if (run_common_nm_attack_checks(wk)) {
        return;
    }

    if (check_defense_lever(wk)) {
        return;
    }

    if (ArcadeBalance_IsEnabled() && check_arcade_walk_start(wk)) {
        return;
    }

    check_stand_up(wk);
}

void nm_09000(PLW* wk) { // 🟡
    if (setup_kuzureochi(wk)) {
        return;
    }

    if (run_common_nm_attack_checks(wk)) {
        return;
    }

    if (check_stand_up(wk)) {
        return;
    }

    if (check_defense_lever(wk)) {
        return;
    }

    if (ArcadeBalance_IsEnabled()) {
        check_arcade_walk_start(wk);
    }
}

void nm_10000(PLW* wk) { // 🟡
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_09000(&wk->wu);
        return;
    }

    if (run_common_nm_attack_checks_no_turn(wk)) {
        return;
    }

    if (check_defense_lever(wk)) {
        return;
    }

    check_stand_up(wk);
}

void nm_11000(PLW* wk) { // 🔵
    // Do nothing
}

void nm_13000(PLW* wk) { // 🔵
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_01000(wk);
    }
}

void nm_16000(PLW* wk) { // 🟢
    set_new_jpdir(wk);

    if (wk->wu.routine_no[3] == 0) {
        return;
    }

    switch (wk->wu.cg_type) {
    case 0xFF:
        check_jump_rl_dir(wk);

        switch (wk->jpdir) {
        case JUMP_DIR_FORWARD:
            wk->wu.routine_no[2] = 21;
            break;

        case JUMP_DIR_BACKWARD:
            wk->wu.routine_no[2] = 23;
            break;

        default:
            wk->wu.routine_no[2] = 22;
            break;
        }

        wk->wu.routine_no[3] = 0;
        break;

    case 1:
        break;
    }

    if (check_full_gauge_attack(wk, 0)) {
        return;
    }

    if (check_full_gauge_attack2(wk, 0)) {
        return;
    }

    if (check_super_arts_attack(wk)) {
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

void nm_17000(PLW* wk) { // 🟢 The only difference is DIP switch handling
    set_new_jpdir(wk);

    if (wk->wu.routine_no[3] == 0) {
        return;
    }

    if (wk->wu.cg_type == 0xFF) {
        check_jump_rl_dir(wk);

        switch (wk->jpdir) {
        case JUMP_DIR_FORWARD:
            wk->wu.routine_no[2] = 24;
            break;

        case JUMP_DIR_BACKWARD:
            wk->wu.routine_no[2] = 26;
            break;

        default:
            wk->wu.routine_no[2] = 25;
            break;
        }

        wk->wu.routine_no[3] = 0;
        return;
    }

    if (check_full_gauge_attack(wk, 0)) {
        return;
    }

    if (check_full_gauge_attack2(wk, 0)) {
        return;
    }

    if (check_super_arts_attack(wk)) {
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
    if (run_attack_checks_before_leap(wk)) {
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
    if (run_attack_checks_before_leap(wk)) {
        return true;
    }

    if (check_leap_attack(wk)) {
        return true;
    }

    if (check_nm_attack(wk)) {
        return true;
    }

    return false;
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
        reset_guard_for_new_state(wk);
        handle_jump_attack_state(wk);
        break;

    case 7:
        reset_guard_for_new_state(wk);
        handle_jump_defense_state(wk);
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
        reset_guard_for_new_state(wk);
        handle_jump_attack_state(wk);
        break;

    case 7:
        reset_guard_for_new_state(wk);
        handle_jump_defense_state(wk);
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
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_01000(&wk->wu);
        return;
    }

    if (run_common_nm_attack_checks(wk)) {
        return;
    }

    if (wk->cp->lever_dir != 2) {
        if (check_bend_myself(wk)) {
            return;
        }

        if (check_F_R_walk(wk)) {
            return;
        }
    }

    nm_27_cg_type_check(wk);
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
    if (wk->wu.routine_no[3] == 0) {
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
    if (wk->wu.cg_type == 0xFF) {
        TO_nm_09000(&wk->wu);
        return;
    }

    if (run_common_nm_attack_checks(wk)) {
        return;
    }

    if (wk->cp->lever_dir != 2) {
        if (check_stand_up(wk)) {
            return;
        }

        if (ArcadeBalance_IsEnabled() && check_arcade_walk_start(wk)) {
            return;
        }
    }

    nm_27_cg_type_check(wk);
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
    if (wk->wu.routine_no[3] == 0) {
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
    if (wk->wu.routine_no[3] == 0) {
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

void nm_36000(PLW* wk) { // 🟢
    if (wk->wu.cg_type == 0xFF) {
        if (wk->wu.now_koc == 0 && wk->wu.char_index == 0) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 1;
        } else {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }
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
        if (wk->wu.now_koc == 0 && wk->wu.char_index == 0) {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 1;
        } else {
            wk->wu.routine_no[2] = 1;
            wk->wu.routine_no[3] = 0;
        }
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
    if (check_full_gauge_attack(wk, 0)) {
        return;
    }

    if (check_full_gauge_attack2(wk, 0)) {
        return;
    }

    if (check_super_arts_attack(wk)) {
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

