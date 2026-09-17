/**
 * @file plmain_arts.c
 * The three gauges a player carries - EX, super art and max - and the CPS3
 * state machines that spend them.
 *
 * Split out of plmain.c. Everything about_gauge_process reaches moved here
 * whole: eag_union, mpg_union, sag_union and its three CPS3 variants, and the
 * helpers each of them owns. The port's variant already lives in
 * plmain_ps2_arts.c, and sag_union still chooses between the two.
 *
 * No static crossed the boundary in either direction, and nothing changed its
 * linkage.
 */

#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/plmain_internal.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/spgauge.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sysdir.h"

void mpg_union(PLW* wk);
void eag_union(PLW* wk);
void sag_union(PLW* wk);

void about_gauge_process(PLW* wk) { // 🟡
    eag_union(wk);
    sag_union(wk);
    mpg_union(wk);

    // CPS3 has no equivalent max-gauge bit update.
    if (!ArcadeBalance_IsEnabled()) {
        add_sp_arts_gauge_maxbit(wk);
    }
}

/* Both ways out of a spent max gauge clear the same three meter fields. */
static void clear_super_art_meter(PLW* wk) {
    wk->sa->saeff_mp = 0;
    wk->sa->mp_rno = 0;
    wk->sa->mp = 0;
}

/* Two things the port does when a gauge is spent and CPS3 does not: clear the
 * super-art meter bug, and hold the next gain off for twenty frames. Both the
 * EX gauge and the max gauge did each of them, in the same place. */
static void clear_meter_bug_on_port(PLW* wk) {
    if (!ArcadeBalance_IsEnabled()) {
        sag_bug_fix(wk->wu.id);
    }
}

static void hold_next_art_gain_on_port(PLW* wk) {
    if (!ArcadeBalance_IsEnabled()) {
        sag_inc_timer[wk->wu.id] = 20;
    }
}

/* mpg_union's case 2: the max-gauge art being spent, or the state being unwound
 * because it was not. Moved out whole, so the fallthrough from case 1 into
 * default survives and every `break` still leaves the switch it always left. */
static void spend_max_gauge(PLW* wk) {
    switch (wk->sa->saeff_mp) {
    case -1:
        if (!pcon_dp_flag) {
            wk->sa->store = 0;
            wk->sa->gauge.i = 0;
        }

        clear_meter_bug_on_port(wk);

        clear_super_art_meter(wk);

        hold_next_art_gain_on_port(wk);

        break;

    case 1:
        if (wk->wu.routine_no[1] == 4) {
            break;
        }

        /* fallthrough */

    default:
        clear_super_art_meter(wk);
        break;
    }
}

void mpg_union(PLW* wk) { // 🟡
    switch (wk->sa->mp_rno) {
    case 0:
        if (wk->sa->store == wk->sa->store_max) {
            wk->sa->mp_rno = 1;
            wk->sa->mp = 1;
        }

        wk->sa->saeff_mp = 0;
        break;

    case 1:
        if (wk->sa->store < wk->sa->store_max) {
            wk->sa->mp_rno = 0;
            wk->sa->mp = 0;
        } else if (wk->sa->mp == -1) {
            wk->sa->mp_rno = 2;
            wk->sa->saeff_mp = 1;
        }

        break;

    case 2:
        spend_max_gauge(wk);

        break;

    default:
        wk->sa->mp_rno = 0;
        wk->sa->mp = 0;
        wk->sa->store = 0;
        wk->sa->gauge.i = 0;
        wk->sa->saeff_mp = 0;
        break;
    }
}

/* Paying for an EX move: out of the current bar if it covers the cost, or by
 * spending a stock and taking the shortfall out of the next bar. Skipped while
 * pcon_dp_flag is set, as in the original. */
static void spend_ex_gauge(PLW* wk) {
    if (!pcon_dp_flag) {
        if (wk->sa->gauge_type == 1 && wk->sa->store == wk->sa->store_max) {
            wk->sa->gauge.i = 0;
        }

        if (wk->sa->gauge.s.h >= use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]]) {
            wk->sa->gauge.s.h -= use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]];
        } else {
            wk->sa->store--;
            wk->sa->gauge.s.h += wk->sa->gauge_len - use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]];
        }
    }
}

/* EX state 0: the move becomes available once there is a stock or enough gauge
 * for one. */
static void arm_ex_when_affordable(PLW* wk) {
    // CPS3 uses Akuma and Shin Akuma here; the port uses Akuma and Gill.
    if (wk->player_number == CHAR_AKUMA || wk->player_number == CHAR_GILL) {
        if (wk->sa->store != 0) {
            wk->sa->ex_rno = 1;
            wk->sa->ex = 1;
        }
    } else if ((wk->sa->store != 0) || (wk->sa->gauge.s.h >= use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]])) {
        wk->sa->ex_rno = 1;
        wk->sa->ex = 1;
    }
}

/* EX state 1: the move becomes unavailable again, or is fired. Not shared with
 * arm_ex_when_affordable above: the comparisons are inverted, `||` becomes
 * `&&`, and this one has a third arm. */
static void disarm_or_fire_ex(PLW* wk) {
    // CPS3 uses Akuma and Shin Akuma here; the port uses Akuma and Gill.
    if (wk->player_number == CHAR_AKUMA || wk->player_number == CHAR_GILL) {
        if (wk->sa->store == 0) {
            wk->sa->ex_rno = 0;
            wk->sa->ex = 0;
        }
    } else if ((wk->sa->store == 0) && (wk->sa->gauge.s.h < use_ex_gauge[omop_use_ex_gauge_ix[wk->wu.id]])) {
        wk->sa->ex_rno = 0;
        wk->sa->ex = 0;
    } else if (wk->sa->ex == -1) {
        wk->sa->ex_rno = 2;
        sa_gauge_flash[wk->wu.id] |= 2;
    }
}

/* Spending the EX gauge disarms it, and on the port also clears the meter bug
 * and holds the next super-art gain off. */
static void spend_and_disarm_ex(PLW* wk) {
    spend_ex_gauge(wk);

    clear_meter_bug_on_port(wk);

    wk->sa->ex_rno = 0;
    wk->sa->ex = 0;

    hold_next_art_gain_on_port(wk);
}

void eag_union(PLW* wk) { // 🟡
    switch (wk->sa->ex_rno) {
    case 0:
        arm_ex_when_affordable(wk);
        break;

    case 1:
        disarm_or_fire_ex(wk);
        break;

    case 2:
        spend_and_disarm_ex(wk);
        break;

    default:
        wk->sa->ex_rno = 0;
        wk->sa->ex = 0;
        wk->sa->store = 0;
        wk->sa->gauge.i = 0;
        break;
    }
}

/* An art that is not going to run: the request, the state machine and the
 * ready flag all go back to nothing. */
void abandon_super_art(PLW* wk) {
    wk->sa->saeff_ok = 0;
    wk->sa->sa_rno = 0;
    wk->sa->ok = 0;
}

/* The unreachable-state reset the super-art machines fall back on, shared
 * verbatim by sag_union_0, sag_union_3 and sag_union_ps2. sag_union_1's reset
 * also clears dtm_mul and is left where it is. */
void clear_super_art_state(PLW* wk) {
    wk->sa->sa_rno = 0;
    wk->sa->ok = 0;
    wk->sa->store = 0;
    wk->sa->saeff_ok = 0;
}

/* State 0, shared verbatim by sag_union_0 and sag_union_1: take a stock and
 * become ready. sag_union_3's version does not count the art and is left where
 * it is. */
static void arm_super_art_on_stock(PLW* wk) {
    if (wk->sa->store != 0) {
        wk->sa->sa_rno = 1;
        wk->sa->ok = 1;
        wk->sa->id_arts += 1;
    }

    wk->sa->saeff_ok = 0;
}

/* State 1 of the super-art machine, shared verbatim by sag_union_0,
 * sag_union_1 and sag_union_3: drop back to state 0 if the stock went away,
 * otherwise advance when the art has been asked for. sag_union_ps2 has its own
 * version with a gt2 test in it and is not included. */
static void update_super_art_ready(PLW* wk) {
    if (wk->sa->store == 0) {
        wk->sa->sa_rno = 0;
        wk->sa->ok = 0;
    } else if (wk->sa->ok == -1) {
        wk->sa->sa_rno = 2;
        wk->sa->saeff_ok = 1;
    }
}

/* State 2 of the plainest super-art machine: spend the stock, or give the state
 * up because the art never started. */
static void spend_or_abandon_super_art(PLW* wk) {
    if (wk->sa->saeff_ok == -1) {
        if (!pcon_dp_flag) {
            wk->sa->store -= 1;
        }

        abandon_super_art(wk);
    } else if ((wk->sa->saeff_ok != 1) || (wk->wu.routine_no[1] != 4)) {
        abandon_super_art(wk);
    }
}

void sag_union_0(PLW* wk) { // 🟢
    switch (wk->sa->sa_rno) {
    case 0:
        arm_super_art_on_stock(wk);
        break;

    case 1:
        update_super_art_ready(wk);
        break;

    case 2:
        spend_or_abandon_super_art(wk);
        break;

    default:
        clear_super_art_state(wk);
        break;
    }
}

/* Four of sag_union_1's character tests had byte-identical bodies and differed
 * only in which character they named, so the character is the parameter and each
 * call site keeps its own constant.
 *
 * Deliberately not shared with sag_union_ps2, which runs the same four tests
 * written with 32 and 128 rather than 0x20 and 0x80, and routes CHAR_YUN through
 * addSAAttribute instead. Those are different literals and a different call. */
static void mark_art_attack_for(PLW* wk, u8 character) {
    if (My_char[wk->wu.id] == character) {
        wk->wu.kind_of_waza |= 0x20;
        wk->wu.at_koa = 0x80;
    }
}

/* State 4: the gauge draining while the art runs, and the per-character attack
 * attributes that go with it. The drain is skipped while either player is in a
 * super-art freeze.
 *
 * State 2 is deliberately left inline. Lifting it as well measured 6.69 against
 * 6.94 - the file already has two spend_or_abandon helpers, and a third makes
 * the duplication among them cost more than the complexity it removes. */
/* Five characters' arts change what their attacks count as while the art runs;
 * Oro's second art sets a dipsw bit instead. */
static void mark_running_art_attack(PLW* wk) {
    mark_art_attack_for(wk, CHAR_YUN);
    mark_art_attack_for(wk, CHAR_YANG);
    mark_art_attack_for(wk, CHAR_MAKOTO);
    mark_art_attack_for(wk, CHAR_TWELVE);

    if ((My_char[wk->wu.id] == CHAR_ORO) && (wk->sa->kind_of_arts == 2)) {
        wk->wu.att.dipsw |= 0x10;
    }
}

static void drain_gauge_while_art_runs(PLW* wk) {
    if ((wk->sa_stop_flag != 1) && (((PLW*)wk->wu.target_adrs)->sa_stop_flag != 1)) {
        wk->sa->gauge.i -= wk->sa->dtm * wk->sa->dtm_mul;
    }

    if (wk->sa->gauge.s.h < 1) {
        wk->sa->gauge.i = 0;
        wk->sa->ok = 0;
        wk->sa->sa_rno = 0;
        wk->sa->dtm_mul = 1;
    } else {
        mark_running_art_attack(wk);
    }
}

/* The frame the art's effect fires on: the stock is spent and the bar filled.
 * Anything else here means the art did not start, and the state is cleared. */
static void spend_stock_or_abandon_art(PLW* wk) {
    if (wk->sa->saeff_ok == -1) {
        if (!pcon_dp_flag) {
            wk->sa->store -= 1;
        }

        wk->sa->gauge.s.h = wk->sa->gauge_len;
        wk->sa->gauge.s.l = -1;
        wk->sa->sa_rno = 3;
        wk->sa->saeff_ok = 0;
        return;
    }

    if ((wk->sa->saeff_ok != 1) || (wk->wu.routine_no[1] != 4)) {
        abandon_super_art(wk);
        wk->sa->dtm_mul = 1;
    }
}

void sag_union_1(PLW* wk) { // 🟢
    switch (wk->sa->sa_rno) {
    case 0:
        arm_super_art_on_stock(wk);
        break;

    case 1:
        update_super_art_ready(wk);
        break;

    case 2:
        spend_stock_or_abandon_art(wk);
        break;

    case 3:
        if (Timer_Freeze) {
            break;
        }

        wk->sa->sa_rno = 4;
        /* fallthrough */

    case 4:
        drain_gauge_while_art_runs(wk);
        break;

    default:
        wk->sa->sa_rno = 0;
        wk->sa->ok = 0;
        wk->sa->store = 0;
        wk->sa->saeff_ok = 0;
        wk->sa->dtm_mul = 1;
        break;
    }
}

/* State 2 of the gauge-emptying machine. Not shared with
 * spend_or_abandon_super_art: this one also clears the gauge and moves to
 * state 3 rather than 0, and its abandon test is one term shorter. */
static void spend_or_abandon_gauge_art(PLW* wk) {
    if (wk->sa->saeff_ok == -1) {
        wk->sa->store = wk->sa->store + -1;
        wk->sa->gauge.i = 0;
        wk->sa->saeff_ok = 0;
        wk->sa->sa_rno = 3;
    } else if (wk->sa->saeff_ok != 1) {
        abandon_super_art(wk);
    }
}

void sag_union_3(PLW* wk) { // 🟢
    switch (wk->sa->sa_rno) {
    case 0:
        if (wk->sa->store != 0) {
            wk->sa->sa_rno = 1;
            wk->sa->ok = 1;
        }

        wk->sa->saeff_ok = 0;
        break;

    case 1:
        update_super_art_ready(wk);
        break;

    case 2:
        spend_or_abandon_gauge_art(wk);
        break;

    case 3:
        // Do nothing
        break;

    default:
        clear_super_art_state(wk);
        break;
    }
}
void sag_union(PLW* wk) { // 🟡
    // Arcade Balance selects the CPS3 super-art state machine; the port state machine remains available otherwise.
    if (ArcadeBalance_IsEnabled()) {
        void (*const sag_union_cps3_jump_table[4])(PLW* wk) = { sag_union_0, sag_union_1, sag_union_0, sag_union_3 };
        sag_union_cps3_jump_table[wk->sa->gauge_type](wk);
    } else {
        sag_union_ps2(wk);
    }
}

void addSAAttribute(u8* kow, u16* koa) { // 🔴
    switch (*kow & 0x78) {
    case 0:
    case 8:
        *kow = 0x20;
        *koa = 0x80;
        break;

    case 16:
    case 24:
        *kow = 0x28;
        *koa = 0x100;
        break;
    }
}

void demo_set_sa_full(SA_WORK* sa) { // 🟡
    sa->sa_rno = 1;
    sa->ok = 1;
    sa->store = sa->store_max;
    sa->id_arts++;

    if (ArcadeBalance_IsEnabled()) {
        if (sa->gauge_type == 1) {
            sa->gauge.s.h = sa->gauge_len;
            sa->dtm_mul = 1;
        }
    } else {
        // The port initializes a zero gauge regardless of gauge type.
        sa->gauge.s.h = 0;
        sa->gauge.s.l = 0;
        sa->dtm_mul = 1;
    }
}
