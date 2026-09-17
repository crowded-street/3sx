/**
 * @file plmain_ps2_arts.c
 * The port's super-art state machine - the one Arcade Balance turns off.
 *
 * Split out of plmain.c, which was 1430 lines with 65 functions. sag_union_ps2
 * and the eight functions only it reaches moved here whole. The CPS3 state
 * machines (sag_union_0, _1 and _3) stay in plmain.c, where sag_union still
 * chooses between the two.
 *
 * Two of plmain.c's statics widened to make this possible - see
 * plmain_internal.h, which says which and why.
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

/* The sag_union_ps2 side of the same three tests. A separate helper from
 * mark_art_attack_for on purpose: that one is written with 0x20 and 0x80, this
 * one with 32 and 128. The values agree, the literals do not, and rewriting a
 * literal is not a refactor. */
static void mark_ps2_art_attack_for(PLW* wk, u8 character) {
    if (My_char[wk->wu.id] == character) {
        wk->wu.kind_of_waza |= 32;
        wk->wu.at_koa = 128;
    }
}

/* The gt2 dispatch that was case 2 of sag_union_ps2's switch: what the super
 * art does once it is stored and running. Moved out whole, so every `break`
 * still belongs to the switch it belonged to before. */
/* gauge type 0: the art is paid for once and then simply stops. */
static void sag_ps2_instant_art(PLW* wk) {
    switch (wk->sa->saeff_ok) {
    case -1:
        if (!pcon_dp_flag) {
            if (wk->sa->ex4th_exec) {
                wk->sa->store = 0;
            } else {
                wk->sa->store--;
            }
        }

        sag_bug_fix(wk->wu.id);
        abandon_super_art(wk);
        sag_inc_timer[wk->wu.id] = 20;
        break;

    case 1:
        if (wk->wu.routine_no[1] == 4) {
            break;
        }

        /* fallthrough */

    default:
        abandon_super_art(wk);
        break;
    }
}

/* gauge type 1: the bar is filled, then drained frame by frame while the art
 * runs, and the characters whose arts mark their attacks are marked here. */
/* Starting a timed art: the stock is paid for, the bar is filled, and the
 * height it had is remembered so it can come back afterwards. */
static void sag_ps2_timed_begin(PLW* wk) {
    switch (wk->sa->saeff_ok) {
    case -1:
        if (!pcon_dp_flag) {
            if (wk->sa->ex4th_exec) {
                wk->sa->store = 0;
            } else {
                wk->sa->store--;
            }
        }

        sag_bug_fix(wk->wu.id);

        if (wk->sa->mp == 1) {
            wk->sa->bacckup_g_h = 0;
        } else {
            wk->sa->bacckup_g_h = wk->sa->gauge.s.h;
        }

        wk->sa->gauge.s.h = wk->sa->gauge_len;
        wk->sa->gauge.s.l = -1;
        wk->sa->sa_rno2 = 1;
        wk->sa->saeff_ok = 0;
        break;

    case 1:
        if (wk->wu.routine_no[1] == 4) {
            break;
        }

        /* fallthrough */

    default:
        abandon_super_art(wk);
        wk->sa->dtm_mul = 1;
        break;
    }
}

/* Running a timed art: the bar drains every frame that neither player is in a
 * super stop, and the characters whose arts mark their attacks are marked
 * here. Emptying the bar ends the art and restores what was left. */
static void sag_ps2_timed_drain(PLW* wk) {
    if ((wk->sa_stop_flag != 1) && (((PLW*)wk->wu.target_adrs)->sa_stop_flag != 1)) {
        wk->sa->gauge.i -= wk->sa->dtm * wk->sa->dtm_mul;
    }

    if (wk->sa->gauge.s.h <= 0 || Suicide[6] != 0) {
        wk->sa->gauge.i = 0;
        wk->sa->ok = 0;
        wk->sa->sa_rno = 0;
        wk->sa->dtm_mul = 1;
        wk->sa->gauge.s.h = wk->sa->bacckup_g_h;
        sag_inc_timer[wk->wu.id] = 20;
        return;
    }

    if (My_char[wk->wu.id] == CHAR_YUN) {
        addSAAttribute(&wk->wu.kind_of_waza, &wk->wu.at_koa);
    }

    mark_ps2_art_attack_for(wk, CHAR_YANG);
    mark_ps2_art_attack_for(wk, CHAR_MAKOTO);
    mark_ps2_art_attack_for(wk, CHAR_TWELVE);

    if ((My_char[wk->wu.id] == CHAR_ORO) && (wk->sa->kind_of_arts == 2)) {
        wk->wu.att.dipsw |= 0x10;
    }
}

static void sag_ps2_timed_art(PLW* wk) {
    switch (wk->sa->sa_rno2) {
    case 0:
        sag_ps2_timed_begin(wk);
        break;

    case 1:
        if (Timer_Freeze != 0) {
            break;
        }

        wk->sa->sa_rno2 = 2;
        /* fallthrough */

    case 2:
        sag_ps2_timed_drain(wk);
        break;
    }
}

/* gauge type 3: paid for, with no drain of its own. */
static void sag_ps2_stored_art(PLW* wk) {
    switch (wk->sa->sa_rno2) {
    case 0:
        switch (wk->sa->saeff_ok) {
        case -1:
            sag_bug_fix(wk->wu.id);
            wk->sa->store--;
            wk->sa->saeff_ok = 0;
            wk->sa->sa_rno2 = 1;
            break;

        case 1:
            break;

        default:
            abandon_super_art(wk);
        }

        break;

    default:
        break;
    }
}

static void sag_union_ps2_active(PLW* wk) {
    switch (wk->sa->gt2) {
    case 0:
        sag_ps2_instant_art(wk);
        break;

    case 1:
        sag_ps2_timed_art(wk);
        break;

    case 3:
        sag_ps2_stored_art(wk);
        break;

    default:
        clear_super_art_state(wk);
        break;
    }
}

/* Armed and waiting: losing the last stock disarms the art, and the request
 * flag fires it. A gauge type 0 art starts from an empty remembered bar. */
static void fire_ps2_art_when_ready(PLW* wk) {
    if (wk->sa->store == 0) {
        wk->sa->sa_rno = 0;
        wk->sa->ok = 0;
        return;
    }

    if (wk->sa->ok == -1) {
        wk->sa->sa_rno = 2;
        wk->sa->sa_rno2 = 0;
        wk->sa->saeff_ok = 1;

        if (wk->sa->gt2 == 0) {
            wk->sa->bacckup_g_h = 0;
        }
    }
}

void sag_union_ps2(PLW* wk) { // 🔴
    switch (wk->sa->sa_rno) {
    case 0:
        if (wk->sa->store) {
            wk->sa->sa_rno = 1;
            wk->sa->ok = 1;
            wk->sa->id_arts++;
        }

        wk->sa->saeff_ok = 0;
        break;

    case 1:
        fire_ps2_art_when_ready(wk);
        break;

    case 2:
        sag_union_ps2_active(wk);

        break;
    }
}
