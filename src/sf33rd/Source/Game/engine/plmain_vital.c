/**
 * @file plmain_vital.c
 * The OMOP vitality drain: the bar that empties while a round runs, and the
 * ways a player earns it back.
 *
 * Split out of plmain.c. check_omop_vital and the eight helpers only it
 * reaches moved here whole; plmain.c's player_mv_4000 still calls it once a
 * frame. Nothing changed its linkage - every one of these was already
 * file-scope, and the three const tables they read stay where they are, in
 * plmain.c next to the state machines that index them.
 */

#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/plmain_internal.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sysdir.h"

/* The four states that hold the draining vitality still, in the order the
 * original tested them. Each was its own `break` out of case 0; `||`
 * short-circuits the same way, and none of the four has a side effect. */
static s32 vital_drain_is_paused(const PLW* wk) {
    return vital_dec_timer || ((wk->wu.routine_no[1] == 0) && !(plpnm_mvkind[wk->wu.routine_no[2]] & 1)) ||
           ((wk->wu.routine_no[1] == 1) && !(plpdm_mvkind[wk->wu.routine_no[2]] & 1)) || (wk->wu.routine_no[1] == 3);
}

/* The second of the two player-one states that hold the vitality drain: a
 * specific pattern of a specific attack routine. The three numbers are the
 * original's. */
static s32 in_second_credit_pose(const PLW* wk) {
    return (wk->wu.routine_no[1] == 4) && (wk->wu.routine_no[2] == 22) && (wk->wu.pat_status == 23);
}

/* The vitality drain itself, and the death it ends in. Every `break` in the
 * original left the switch with nothing after it, so each is a `return` here.
 *
 * The player-one credit-check states are the two that hold the drain without
 * being a pause: the game is waiting on a coin, not on the player. */
/* The first player's drain stops in two more places than anyone else's: the
 * pose that latches the check flag, and the second-credit pose. */
static s32 first_player_drain_is_held(PLW* wk) {
    if (wk->player_number != 0) {
        return 0;
    }

    if ((wk->wu.routine_no[1] == 4) && (wk->wu.routine_no[2] == 21)) {
        if (ca_check_flag == 0) {
            ca_check_flag = 1;
        }

        return 1;
    }

    return in_second_credit_pose(wk);
}

/* An empty bar is a death, and the damage that reads as having caused it. */
static void kill_on_empty_vitality(PLW* wk) {
    wk->wu.vital_new = -1;
    wk->wu.dm_koa = 4;
    wk->dead_flag = 1;
    wk->guard_flag = 3;
    ca_check_flag = 0;
}

static void drain_vitality(PLW* wk) {
    if (vital_drain_is_paused(wk)) {
        return;
    }

    if (first_player_drain_is_held(wk)) {
        return;
    }

    wk->wu.vital_new--;

    if (wk->wu.vital_new < 0) {
        kill_on_empty_vitality(wk);
    }
}

/* One point of health back, up to the bar's length. */
static void gain_one_vitality(PLW* wk) {
    wk->wu.vital_new++;

    if (wk->wu.vital_new > 160) {
        wk->wu.vital_new = 160;
    }
}

/* Mode 2 only regains while the player is idle, and only on the frames the
 * regain timer allows. */
static s32 idle_regain_is_blocked(PLW* wk) {
    if (vital_inc_timer) {
        return 1;
    }

    if (wk->wu.routine_no[1] != 0) {
        return 1;
    }

    if (!(plpnm_mvkind[wk->wu.routine_no[2]] & 2)) {
        return 1;
    }

    return 0;
}

/* Mode 3 stops regaining while the player is in one of the plpxx states, and
 * for a while after leaving one. */
static s32 timed_regain_is_blocked(PLW* wk) {
    if (plpxx_kind[wk->wu.routine_no[1]]) {
        return 1;
    }

    if (plpxx_kind[wk->wu.old_rno[1]]) {
        wk->omop_vital_timer = 40;
    }

    if (wk->omop_vital_timer) {
        wk->omop_vital_timer--;
        return 1;
    }

    return 0;
}

static void regain_while_idle(PLW* wk) {
    if (idle_regain_is_blocked(wk)) {
        return;
    }

    gain_one_vitality(wk);
}

/* Health does not move at all while the game is paused, while the player is
 * dead, or during a super stop. */
static s32 vital_is_frozen(PLW* wk) {
    if (pcon_dp_flag) {
        return 1;
    }

    if (wk->dead_flag) {
        return 1;
    }

    if (sa_stop_check()) {
        return 1;
    }

    return 0;
}

void check_omop_vital(PLW* wk) { // 🔴
    if (vital_is_frozen(wk)) {
        return;
    }

    if (wk->resurrection_resv) {
        wk->wu.vital_new = -1;
        return;
    }

    switch (omop_vital_ix[wk->wu.id]) {
    case 0:
        drain_vitality(wk);
        break;

    case 2:
        regain_while_idle(wk);
        break;

    case 3:
        if (timed_regain_is_blocked(wk)) {
            break;
        }

        /* fallthrough */

    case 4:
        gain_one_vitality(wk);
        break;
    }
}
