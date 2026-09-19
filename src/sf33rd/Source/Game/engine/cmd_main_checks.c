/**
 * @file cmd_main_checks.c
 * The check_* command-step checkers.
 *
 * Split out of cmd_main.c, which was 1628 lines. Each function is one step
 * type in a character's command table; chk_move_jp in cmd_main.c indexes them
 * by step type. The shared parser state - waza_ptr, chk_pl, cmd_pl, cmd_id,
 * sw_work - lives in cmd_data.h, as it already did.
 */

#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "arcade/arcade_balance.h"
#include "arcade/arcade_cmd_data.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main_internal.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sysdir.h"

#include <SDL3/SDL.h>

/* Every call site sits at the end of an if/else arm with nothing after the
 * chain, so the early return here is the same exit as the original's. */
static void finish_or_advance_command(void) {
    if (*waza_ptr->w_ptr == 28) {
        command_ok();
        return;
    }

    check_next();
}

static void advance_when_shot_count_reached(void) {
    if (waza_ptr->shot_ok >= waza_ptr->free1) {
        finish_or_advance_command();
    }
}

static s32 lever_held_and_move_allowed(void) {
    return (cmd_pl->wu.xyz[1].disp.pos > 0 || (waza_type[cmd_id] != 5 && waza_type[cmd_id] != 6)) &&
           chk_pl->now_lvbt & 0xF;
}

/* Counts shot_ok as part of the test, exactly where the original did. */
static s32 lever_changed_and_shot_counted(void) {
    return ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) && (chk_pl->sw_lever & waza_ptr->w_lvr) &&
           (waza_ptr->shot_ok += 1, waza_ptr->shot_ok < waza_ptr->free1 == 0);
}

void check_init() { // 🟢
    cmd_tbl_ptr += 12;
    waza_ptr->w_type = *cmd_tbl_ptr++;
    waza_ptr->w_int = *cmd_tbl_ptr++;
    waza_ptr->free1 = *cmd_tbl_ptr;
    waza_ptr->free2 = *cmd_tbl_ptr++;
    waza_ptr->w_lvr = *cmd_tbl_ptr++;
    waza_ptr->w_ptr = cmd_tbl_ptr;
    waza_ptr->uni0.tame.flag = 0;
    waza_ptr->uni0.tame.shot_flag = 0;
    waza_ptr->uni0.tame.shot_flag2 = 0;
    waza_ptr->shot_ok = 0;
    waza_ptr->free3 = 0;
    chk_move_jp[waza_ptr->w_type]();
}

void check_next() { // 🟢
    s16* next_ptr = waza_ptr->w_ptr;

    waza_ptr->w_type = *next_ptr++;
    waza_ptr->w_int = *next_ptr++;
    waza_ptr->free1 = *next_ptr;
    waza_ptr->free2 = *next_ptr++;
    waza_ptr->w_lvr = *next_ptr++;
    waza_ptr->w_ptr = next_ptr;

    if (waza_ptr->w_type != 10) {
        chk_move_jp[waza_ptr->w_type]();
    }
}

/* The lever is exactly where the command wants it. check_0's first two arms
 * asked this identically and differ in one value, the direction wanted. */
static void advance_when_lever_is(u16 sw_lever, u16 want) {
    if (sw_lever == want) {
        finish_or_advance_command();
    }
}

/* The command window this waza is allowed: opened to the length the command
 * table gives it, and closed. Twenty places in this file wrote one or the other
 * out in full. */
static void open_waza_window() {
    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
}

static void close_waza_window() {
    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
}

void check_0() { // 🟢
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (dead_lvr_check()) {
        return;
    }

    if (waza_ptr->w_lvr & 0x8000) {
        sw_work = waza_ptr->w_lvr & 0xF;
        advance_when_lever_is(sw_lever, sw_work);
    } else if (waza_ptr->w_lvr == 0) {
        advance_when_lever_is(sw_lever, 0);
    } else if (chk_pl->now_lvbt & 0xF && sw_lever & waza_ptr->w_lvr) {
        finish_or_advance_command();
    }
}

/* The charge is abandoned: the charge timer goes back to its full length and
 * the entry's own timer runs down. Written out identically wherever a charge
 * is given up. */
static void reset_charge_timer() {
    waza_ptr->free2 = waza_ptr->free1;
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }
}

/* The charge was complete when the lever left: the command fires. */
static void fire_charged_command() {
    waza_ptr->uni0.tame.flag = 0;

    if (*waza_ptr->w_ptr == 0x1C) {
        command_ok();
    } else {
        check_next();
    }
}

static void resolve_tame_flag_or_reset_timer() {
    if (waza_ptr->uni0.tame.flag) {
        fire_charged_command();
    } else {
        reset_charge_timer();
    }
}

/* check_1's charge step for a masked lever. Unlike the 0x8000 arm above it,
 * the countdown itself is skipped once the flag is set - that is the original's
 * shape, not a simplification of it. */
static void charge_until_tame_flag_set(void) {
    if (!waza_ptr->uni0.tame.flag) {
        waza_ptr->free1--;

        if (waza_ptr->free1 < 0) {
            waza_ptr->uni0.tame.flag = 1;
        }
    }
}

/* The charged form: the lever must match exactly, and the charge only counts
 * once the timer has run past zero. */
static void charge_on_exact_lever() {
    if (sw_work == chk_pl->sw_lever) {
        waza_ptr->free2--;

        if (!waza_ptr->uni0.tame.flag && waza_ptr->free2 < 0) {
            waza_ptr->uni0.tame.flag = 1;
        }
    } else {
        resolve_tame_flag_or_reset_timer();
    }
}

void check_1() { // 🟢
    if (dead_lvr_check()) {
        return;
    }

    sw_work = waza_ptr->w_lvr & 0xF;

    if (waza_ptr->w_lvr & 0x8000) {
        charge_on_exact_lever();
    } else {
        if (sw_work & chk_pl->sw_lever) {
            charge_until_tame_flag_set();
        } else {
            resolve_tame_flag_or_reset_timer();
        }
    }
}

/* The buttons are still held, so the charge timer runs down; reaching zero
 * sets the charge flag once. */
static void charge_while_buttons_held() {
    if (waza_ptr->uni0.tame.flag) {
        return;
    }

    waza_ptr->free2--;

    if (waza_ptr->free2 < 0) {
        waza_ptr->uni0.tame.flag = 1;
    }
}

/* They are not: releasing them after a full charge fires the command, and
 * anything else restarts the timer and runs the entry's own timer down. */
static void release_or_restart_charge() {
    if (waza_ptr->uni0.tame.flag && sw_work == 0) {
        waza_ptr->uni0.tame.flag = 0;

        finish_or_advance_command();

        return;
    }

    reset_charge_timer();
}

void check_2() { // 🟢
    sw_work = chk_pl->sw_new & waza_ptr->w_lvr;

    if (waza_ptr->w_lvr == sw_work) {
        charge_while_buttons_held();
    } else {
        release_or_restart_charge();
    }
}

/* The window for a repeated button press closing. Runs the countdown only while
 * a press has been counted, exactly as the original did, and returns 1 on the
 * frame the window actually closed.
 *
 * check_7 and check_3 shared everything here except one trailing statement, so
 * that statement stays at check_7's call site rather than being parameterised -
 * and it still runs after the two assignments above it, as it did before. */
static s32 expire_shot_window(void) {
    if (waza_ptr->shot_ok) {
        waza_ptr->free2--;

        if (waza_ptr->free2 < 0) {
            waza_ptr->shot_ok = 0;
            waza_ptr->free2 = waza_ptr->free1;
            return 1;
        }
    }

    return 0;
}

void check_3() { // 🟢
    s16 i;
    s16 w_flag;
    s16* shot_cnt_adrs;

    sw_work = chk_pl->sw_new & 0x770;
    waza_ptr->uni0.tame.shot_flag2 = waza_ptr->uni0.tame.shot_flag;
    waza_ptr->uni0.tame.shot_flag = 0;
    shot_cnt_adrs = &chk_pl->s1_cnt;
    w_flag = 0x10;

    for (i = 0; i < 6; i++) {
        if (*shot_cnt_adrs >= waza_ptr->w_int) {
            waza_ptr->uni0.tame.shot_flag |= w_flag;
        }

        shot_cnt_adrs++;

        if ((chk_pl->shot_down & w_flag) && (waza_ptr->uni0.tame.shot_flag2 & w_flag)) {
            waza_ptr->shot_ok++;
        }

        w_flag <<= 1;
    }

    expire_shot_window();

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;
        waza_ptr->free2 = waza_ptr->free1;

        finish_or_advance_command();
    }
}

static bool try_reset_on_tame_field(s16* field, s32 w_int_value) {
    if (wcp[cmd_id].waza_flag[waza_type[cmd_id]]) {
        if (waza_ptr->w_int > 0 && *field) {
            open_waza_window();
            *field = 0;
            waza_ptr->w_int = w_int_value;
            return true;
        }
    } else if (*field >= 5) {
        open_waza_window();
        *field = 0;
        waza_ptr->w_int = w_int_value;
        chk_pl->waza_no = waza_type[cmd_id];
        return true;
    }

    return false;
}

/* Count how long each punch button has been held.
 *
 * The kick arm below keeps its copy inline. The two differ in three mask
 * literals, so Recipe D cannot merge them, and extracting both was measured at
 * 5.90 against 6.08 for extracting this one - the twin pair costs more than the
 * second extraction saves, even though the parent clears either way. */
static void count_held_punches(void) {
    if (chk_pl->sw_now & 0x10) {
        waza_ptr->uni0.tame.flag++;
    }

    if (chk_pl->sw_now & 0x20) {
        waza_ptr->uni0.tame.shot_flag++;
    }

    if (chk_pl->sw_now & 0x40) {
        waza_ptr->uni0.tame.shot_flag2++;
    }
}

void check_4() { // 🟢
    if (waza_ptr->w_lvr == 0x10) {
        count_held_punches();
    } else {
        if (chk_pl->sw_now & 0x100) {
            waza_ptr->uni0.tame.flag++;
        }

        if (chk_pl->sw_now & 0x200) {
            waza_ptr->uni0.tame.shot_flag++;
        }

        if (chk_pl->sw_now & 0x400) {
            waza_ptr->uni0.tame.shot_flag2++;
        }
    }

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->uni0.tame.shot_flag = 0;
        waza_ptr->uni0.tame.shot_flag2 = 0;
        waza_ptr->w_int = waza_ptr->free1;
    }

    if (try_reset_on_tame_field(&waza_ptr->uni0.tame.shot_flag2, 9)) {
        return;
    }

    if (try_reset_on_tame_field(&waza_ptr->uni0.tame.shot_flag, 12)) {
        return;
    }

    try_reset_on_tame_field(&waza_ptr->uni0.tame.flag, 15);
}

void check_5() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    if (dead_lvr_check() == 0 && waza_ptr->w_lvr == chk_pl->sw_now) {
        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
            return;
        }

        check_next();
    }
}

/* The timer ran out: the next command entry is read and the four-direction
 * state starts again. */
static void restart_four_direction_command() {
    cmd_tbl_ptr += 12;
    waza_ptr->w_type = *cmd_tbl_ptr++;
    waza_ptr->w_int = *cmd_tbl_ptr++;
    waza_ptr->free2 = *cmd_tbl_ptr++;
    waza_ptr->w_lvr = *cmd_tbl_ptr++;
    waza_ptr->w_ptr = cmd_tbl_ptr;
    waza_ptr->uni0.tame.flag = 0;
    waza_ptr->uni0.tame.shot_flag = 0;
    waza_ptr->uni0.tame.shot_flag2 = 0;
    waza_ptr->free1 = 14;
    waza_ptr->shot_ok = 0;
}

/* It did not: the window between directions runs down instead, and losing it
 * clears what has been collected. */
static void tick_four_direction_window() {
    waza_ptr->free1--;

    if (waza_ptr->free1 <= 0) {
        waza_ptr->free1 = 14;
        waza_ptr->shot_ok = 0;
    }
}

/* Each of the four directions the lever is on this frame is collected, and
 * holding one refreshes the window. */
static void collect_held_lever_direction() {
    s16 i;
    u16 lvr_work = 1 & 0xFFFF;

    for (i = 0; i < 4; i++) {
        if (chk_pl->sw_lever == lvr_work) {
            waza_ptr->shot_ok |= (lvr_work);
            waza_ptr->free1 = 14;
        }

        lvr_work *= 2;
    }
}

void check_6() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        restart_four_direction_command();
    } else {
        tick_four_direction_window();
    }

    collect_held_lever_direction();

    if (waza_ptr->shot_ok == 15) {
        if (*waza_ptr->w_ptr == 28) {
            command_ok();
            return;
        }

        waza_ptr->shot_ok = 0;
        check_next();
    }
}

void check_7() { // 🟢
    s16 i;
    s16 w_flag;
    s16* shot_cnt_adrs;

    waza_ptr->w_int--;

    if (waza_ptr->w_type == 8) {
        sw_work = chk_pl->sw_new & 0x70;
        shot_cnt_adrs = &chk_pl->s1_cnt;
        w_flag = 0x10;
    } else {
        sw_work = chk_pl->sw_new & 0x780;
        shot_cnt_adrs = &chk_pl->s4_cnt;
        w_flag = 0x100;
    }

    waza_ptr->uni0.tame.shot_flag2 = waza_ptr->uni0.tame.shot_flag;
    waza_ptr->uni0.tame.shot_flag = 0;

    for (i = 0; i < 3; i++) {
        if (*shot_cnt_adrs & waza_ptr->w_lvr) {
            waza_ptr->uni0.tame.shot_flag |= w_flag;
        }

        shot_cnt_adrs++;

        if (chk_pl->shot_down & w_flag && waza_ptr->uni0.tame.shot_flag2 & w_flag) {
            waza_ptr->shot_ok += 1;
        }

        w_flag *= 2;
    }

    if (expire_shot_window()) {
        waza_ptr->uni0.tame.shot_flag = 0;
    }

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;
        waza_ptr->free2 = waza_ptr->free1;

        finish_or_advance_command();
    }
}

/* The lever moved this frame: landing on the direction the command wants
 * advances it, anything else ends it. */
static void advance_on_extended_lever_move() {
    if (chk_pl->sw_lever == sw_work) {
        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
            return;
        }

        check_next();
        return;
    }

    waza_ptr->w_type = 0;
}

static void resolve_extended_lever_command() {
    sw_work = waza_ptr->w_lvr & 0xF;

    if (waza_ptr->w_lvr == 0) {
        if (chk_pl->new_lvbt == 0) {
            finish_or_advance_command();
        }
    } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
        advance_on_extended_lever_move();
    }
}

/* The command wants the lever at rest: it advances there, and any other
 * movement ends it. */
static void resolve_lever_back_to_neutral() {
    if (chk_pl->new_lvbt == 0) {
        finish_or_advance_command();
        return;
    }

    if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
        waza_ptr->w_type = 0;
    }
}

/* The command wants a direction, and the lever just moved: landing on it
 * advances, anything else ends the command. */
static void resolve_lever_onto_direction() {
    if (chk_pl->sw_lever & waza_ptr->w_lvr) {
        finish_or_advance_command();
        return;
    }

    waza_ptr->w_type = 0;
}

void check_9() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    if (waza_ptr->w_lvr & 0x8000) {
        resolve_extended_lever_command();
    } else if (waza_ptr->w_lvr == 0) {
        resolve_lever_back_to_neutral();
    } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
        resolve_lever_onto_direction();
    }
}

void paring_miss_init() { // 🟢
    waza_ptr->free3 = 0;
    waza_ptr->w_type = 0;
    waza_ptr->uni0.tame.flag = 0;
    close_waza_window();
}

/* The four peer slots a winning slot clears. The field order is the parameter
 * order clear_flags_below took, and the types are the parameter types, so every
 * call site's argument list converts through the compound literal unchanged. */
typedef struct {
    s16 other1;
    s16 other2;
    s16 other3;
    s16 other4;
} WazaFlagPeers;

static void clear_flags_below(s16 self_ix, WazaFlagPeers peers) {
    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[peers.other1]) {
        wcp[cmd_id].waza_flag[peers.other1] = 0;
    }

    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[peers.other2]) {
        wcp[cmd_id].waza_flag[peers.other2] = 0;
    }

    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[peers.other3]) {
        wcp[cmd_id].waza_flag[peers.other3] = 0;
    }

    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[peers.other4]) {
        wcp[cmd_id].waza_flag[peers.other4] = 0;
    }
}

static void clear_lower_priority_waza_flags() {
    switch (waza_type[cmd_id]) {
    case 3:
        clear_flags_below(3, (WazaFlagPeers){ 4, 5, 6, 12 });
        break;

    case 4:
        clear_flags_below(4, (WazaFlagPeers){ 3, 5, 6, 12 });
        break;

    case 5:
        clear_flags_below(5, (WazaFlagPeers){ 3, 4, 6, 12 });

        if (waza_work[cmd_id][6].free3 > 0) {
            wcp[cmd_id].waza_flag[5] = 0;
        }

        break;

    case 6:
        clear_flags_below(6, (WazaFlagPeers){ 3, 4, 5, 12 });

        if (waza_work[cmd_id][5].free3 > 0) {
            wcp[cmd_id].waza_flag[6] = 0;
        }

        break;

    case 12:
        clear_flags_below(12, (WazaFlagPeers){ 3, 4, 5, 6 });
        break;
    }
}

/* The half of a dash command that watches the lever come back: the window it
 * has to happen in, the release itself, and the timeout. check_10 and check_12
 * had these three states written out byte for byte identically. Case labels
 * are the originals. */
/* The dash-release states from 3 on, reached from state 2's default. The case
 * labels are the original ones and the switch is on the same expression, so a
 * shot_ok that used to match here still matches here and one that matches
 * nothing still does nothing. */
static void run_dash_release_states_from_3() {
    switch (waza_ptr->shot_ok) {
    case 3:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
            break;
        }

        if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
            close_waza_window();
            break;
        }

        if (chk_pl->sw_now & 0xF) {
            waza_ptr->shot_ok++;
            close_waza_window();
        }

        break;

    case 4:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
        }

        break;
    }
}

static void run_dash_release_states() {
    switch (waza_ptr->shot_ok) {
    case 2:
        waza_ptr->w_int--;
        waza_ptr->free3--;

        if (waza_ptr->w_int > 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever & 8) {
                close_waza_window();
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever != waza_ptr->w_lvr) {
                close_waza_window();
                waza_ptr->shot_ok++;
                break;
            }
        } else {
            close_waza_window();
            waza_ptr->shot_ok++;
        }

        break;

    default:
        run_dash_release_states_from_3();
        break;
    }
}

/* check_10's arming step. The one `break` inside it ended the arm, and nothing
 * runs after the switch, so it is a `return` here. */
static void arm_check_10_window() {
    if (lever_held_and_move_allowed()) {
        if (chk_pl->sw_lever == waza_ptr->w_lvr) {
            waza_ptr->shot_ok++;
            open_waza_window();
            waza_ptr->free3 = wcp[cmd_id].reset[waza_type[cmd_id]] + 10;
            waza_ptr->w_int = 6;

            clear_lower_priority_waza_flags();
        } else {
            waza_ptr->shot_ok = 0;
            return;
        }
    }
}

void check_10() { // 🟢
    switch (waza_ptr->shot_ok) {
    case 0:
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok++;
        }
        break;

    case 1:
        arm_check_10_window();

        break;

    default:
        run_dash_release_states();
        break;
    }
}

void check_11() { // 🟢
    if (dead_lvr_check()) {
        paring_miss_init();
        return;
    }

    switch (waza_ptr->uni0.tame.flag) {
    case 0:
        if (chk_pl->sw_lever & 8) {
            waza_ptr->uni0.tame.flag = 1;
            break;
        }

        waza_ptr->uni0.tame.flag = 0;
        break;

    case 1:
        if (chk_pl->sw_lever == 2) {
            check_next();
            break;
        }

        if (!(chk_pl->sw_lever & 8)) {
            waza_ptr->uni0.tame.flag = 0;
        }

        break;
    }
}

static void clear_lower_priority_waza_flags_no_free3_check() {
    switch (waza_type[cmd_id]) {
    case 3:
        clear_flags_below(3, (WazaFlagPeers){ 4, 5, 6, 12 });
        break;

    case 4:
        clear_flags_below(4, (WazaFlagPeers){ 3, 5, 6, 12 });
        break;

    case 5:
        clear_flags_below(5, (WazaFlagPeers){ 3, 4, 6, 12 });
        break;

    case 6:
        clear_flags_below(6, (WazaFlagPeers){ 3, 4, 5, 12 });
        break;

    case 12:
        clear_flags_below(12, (WazaFlagPeers){ 3, 4, 5, 6 });
        break;
    }
}

void check_12() { // 🟢
    switch (waza_ptr->shot_ok) {
    case 0:
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok++;
        }
        break;

    case 1:
        if (cmd_pl->wu.xyz[1].disp.pos > 0 && (chk_pl->now_lvbt & 0xF) != 0) {
            if (chk_pl->sw_lever == waza_ptr->w_lvr) {
                waza_ptr->shot_ok++;
                open_waza_window();
                waza_ptr->free3 = wcp[cmd_id].reset[waza_type[cmd_id]] + 10;
                waza_ptr->w_int = 6;

                clear_lower_priority_waza_flags_no_free3_check();
            } else {
                waza_ptr->shot_ok = 0;
                break;
            }
        }

        break;

    default:
        run_dash_release_states();
        break;
    }
}

void check_13() { // 🟢
    u16 sw_w;

    if (waza_ptr->free3 > 0) {
        waza_ptr->free3--;

        if (waza_ptr->free3 <= 0) {
            waza_ptr->w_type = 0;
        }
    }

    if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF) && (chk_pl->sw_lever) == 2) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0x10 - ukemi_time_tbl[wcp[cmd_id].waza_flag[waza_type[cmd_id]]];
        waza_ptr->free3 = 0x10;
        chk_pl->waza_no = waza_type[cmd_id];
    }

    sw_w = (chk_pl->sw_now | chk_pl->old_now) & 0x70;

    if (sw_w == 0x70) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0x10 - ukemi_time_tbl[wcp[cmd_id].waza_flag[waza_type[cmd_id]]];
        waza_ptr->free3 = 0x10;
        chk_pl->waza_no = waza_type[cmd_id];
    }
}

static void reset_tame_flag_or_charge(s32 w_int_on_release) {
    if (waza_ptr->uni0.tame.flag >= 3) {
        open_waza_window();
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->w_int = w_int_on_release;
        chk_pl->waza_no = waza_type[cmd_id];
        return;
    }

    if (waza_ptr->w_int < 0) {
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->w_int = waza_ptr->free1;
    }
}

/* check_14's charge window running out while the move is still armed. The
 * original's `return` left check_14 with nothing after the if/else chain, so
 * returning from the helper reaches the same place. */
static void close_tame_window_if_elapsed(void) {
    if (waza_ptr->w_int <= 0) {
        if (waza_ptr->uni0.tame.flag) {
            open_waza_window();
            waza_ptr->uni0.tame.flag = 0;

            if (waza_type[cmd_id] & 1) {
                waza_ptr->w_int = 10;
            } else {
                waza_ptr->w_int = 6;
            }
            return;
        }

        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->w_int = waza_ptr->free1;
    }
}

void check_14() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_lvr == 0x10) {
        if (chk_pl->sw_now & 0x70) {
            waza_ptr->uni0.tame.flag++;
        }
    } else if (chk_pl->sw_now & 0x700) {
        waza_ptr->uni0.tame.flag += 1;
    }

    if (wcp[cmd_id].waza_flag[waza_type[cmd_id]]) {
        close_tame_window_if_elapsed();
    } else if (waza_type[cmd_id] & 1) {
        reset_tame_flag_or_charge(0xA);
    } else {
        reset_tame_flag_or_charge(6);
    }
}

/* The lever is where the command wants it, so this repetition counts. The two
 * arms of check_15 that do this differ in one value, the direction wanted;
 * one wrote its increment as ++ and the other as += 1. */
static void count_shot_when_lever_matches(u16 want) {
    if (chk_pl->sw_lever == want) {
        waza_ptr->shot_ok++;

        advance_when_shot_count_reached();
    }
}

void check_15() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
        return;
    }

    if (dead_lvr_check()) {
        return;
    }

    if (waza_ptr->w_lvr & 0x8000) {
        sw_work = waza_ptr->w_lvr & 0xF;
        count_shot_when_lever_matches(sw_work);
    } else if (waza_ptr->w_lvr == 0) {
        count_shot_when_lever_matches(0);
    } else if (
        lever_changed_and_shot_counted()
    ) {
        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
            return;
        }

        check_next();
    }
}

/* Count how many of the three buttons in this group were pressed this frame.
 * Command type 17 watches the punches, anything else the kicks. */
static void count_pressed_buttons_in_group() {
    s16 i;
    u16 w_flag;

    if (waza_ptr->w_type == 17) {
        sw_work = chk_pl->sw_now & 0x70;
        w_flag = 0x10;
    } else {
        sw_work = chk_pl->sw_now & 0x700;
        w_flag = 0x100;
    }

    waza_ptr->uni0.tame.shot_flag2 = waza_ptr->uni0.tame.shot_flag;
    waza_ptr->uni0.tame.shot_flag = 0;

    for (i = 0; i < 3; i++) {
        if (sw_work & w_flag) {
            waza_ptr->shot_ok++;
        }

        w_flag *= 2;
    }
}

void check_16() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
        waza_ptr->shot_ok = 0;
        return;
    }

    count_pressed_buttons_in_group();

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;

        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
            return;
        }

        check_next();
    }
}

/* The three lever forms check_18 accepts - a charged lever matching its
 * direction on a change, a neutral lever, or a lever sharing a bit with the
 * wanted one - each of which restarts the window. */
static void open_charged_lever_window(u16 sw_lever) {
    if (waza_ptr->w_lvr & 0x8000) {
        if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
            sw_work = waza_ptr->w_lvr & 0xF;

            if (sw_lever == sw_work) {
                waza_ptr->w_int = waza_ptr->free1;
                open_waza_window();
            }
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (chk_pl->sw_lever == 0) {
            waza_ptr->w_int = waza_ptr->free1;
            open_waza_window();
        }
    } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF) && (sw_lever & waza_ptr->w_lvr)) {
        waza_ptr->w_int = waza_ptr->free1;
        open_waza_window();
    }
}

void check_18() { // 🟢
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
        return;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (dead_lvr_check()) {
        return;
    }

    open_charged_lever_window(sw_lever);
}

/* The same three lever forms for check_19, which opens the window and moves
 * to the next command rather than restarting the interval. */
static void advance_on_lever_match(u16 sw_lever) {
    if (waza_ptr->w_lvr & 0x8000) {
        if (chk_pl->now_lvbt & 0xF) {
            sw_work = waza_ptr->w_lvr & 0xF;
            if (sw_lever == sw_work) {
                open_waza_window();
                check_next();
            }
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (chk_pl->sw_lever == 0) {
            open_waza_window();
            check_next();
        }
    } else if ((chk_pl->now_lvbt & 0xF) != 0 && (sw_lever & waza_ptr->w_lvr)) {
        open_waza_window();
        check_next();
    }
}

void check_19() { // 🟢
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (dead_lvr_check()) {
        return;
    }

    advance_on_lever_match(sw_lever);
}

void check_20() { // 🟢
    // Do nothing
}

/* check_21's charged-lever arm. The one `command_ok(); return;` inside it left
 * check_21 with nothing after the if/else chain, so returning from the helper
 * reaches the same place. */
/* The neutral form of the charged lever command: it advances when the lever
 * is at rest. */
static void advance_on_neutral_lever(u16 sw_lever) {
    if (sw_lever != 0) {
        return;
    }

    if (((*waza_ptr->w_ptr)) == 0x1C) {
        command_ok();
        return;
    }

    check_next();
}

static void resolve_charged_lever_command(u16 sw_lever) {
    sw_work = waza_ptr->w_lvr & 0xF;

    if (sw_work == 0) {
        advance_on_neutral_lever(sw_lever);
    } else if (chk_pl->now_lvbt & 0xF) {
        if (sw_lever == sw_work) {
            finish_or_advance_command();
        }
    }
}

void check_21() { // 🟢
    u16 sw_lever;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    sw_lever = chk_pl->sw_lever & 0xF;

    if (dead_lvr_check()) {
        return;
    }

    if (waza_ptr->w_lvr & 0x8000) {
        resolve_charged_lever_command(sw_lever);
    } else if (waza_ptr->w_lvr == 0) {
        if (sw_lever == 0) {
            finish_or_advance_command();
        }
    } else if ((chk_pl->now_lvbt & 0xF) && (sw_lever & waza_ptr->w_lvr)) {
        finish_or_advance_command();
    }
}

/* Mark whichever of the eight lever positions the lever is on this frame. */
static void collect_circle_lever_position() {
    s16 i;

    for (i = 0; i < 8; i++) {
        if (chk_pl->sw_lever == chk22_tbl[i]) {
            waza_ptr->free3 |= 1 << i;
        }
    }
}

void check_22() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_int = waza_ptr->free2;
        cmd_tbl_ptr += 12;
        waza_ptr->w_type = *cmd_tbl_ptr++;
        waza_ptr->w_int = *cmd_tbl_ptr++;
        waza_ptr->free1 = *cmd_tbl_ptr;
        waza_ptr->free2 = *cmd_tbl_ptr++;
        waza_ptr->w_lvr = *cmd_tbl_ptr++;
        waza_ptr->w_ptr = cmd_tbl_ptr;
        waza_ptr->uni0.tame.flag = 0;
        waza_ptr->uni0.tame.shot_flag = 0;
        waza_ptr->uni0.tame.shot_flag2 = 0;
        waza_ptr->shot_ok = 0;
        waza_ptr->free3 = 0;
    }

    collect_circle_lever_position();

    if (waza_ptr->free3 == 0xFF) {
        if (((*waza_ptr->w_ptr)) == 0x1C) {
            command_ok();
            return;
        }

        waza_ptr->free3 = 0;
        check_next();
    }
}

/* While the window is still open, the lever ends the step three ways: back
 * to neutral advances, the down bit clears the flag and advances, and any
 * other direction clears the flag and drops the command. Running out of
 * window advances too. */
static void watch_lever_during_window(void) {
    waza_ptr->w_int -= 1;
    waza_ptr->free3 -= 1;

    if (((waza_ptr->w_int)) > 0) {
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok++;
            return;
        }

        if (chk_pl->sw_lever & 8) {
            wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
            waza_ptr->shot_ok++;
            return;
        }

        if (chk_pl->sw_lever != ((waza_ptr->w_lvr))) {
            wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
            waza_ptr->w_type = 0;
            return;
        }
    } else {
        wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
        waza_ptr->shot_ok++;
    }
}

/* Once the window has closed, the same lever readings are taken from
 * sw_now instead, and the spare counter running out drops the command. */
static void watch_lever_after_window(void) {
    waza_ptr->free3--;

    if (waza_ptr->free3 < 0) {
        waza_ptr->w_type = 0;
        return;
    }

    if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
        close_waza_window();
        return;
    }

    if (chk_pl->sw_now & 0xF) {
        close_waza_window();
        waza_ptr->w_type = 0;
    }
}

void check_23() { // 🟢
    switch (waza_ptr->shot_ok) {
    case 0:
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok++;
            break;
        }

        break;

    case 1:
        if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF) && chk_pl->sw_lever == waza_ptr->w_lvr) {
            waza_ptr->shot_ok++;
            wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = wcp[cmd_id].reset[(waza_type[cmd_id])];
            waza_ptr->free3 = (s16)(((((wcp[cmd_id].reset[(waza_type[cmd_id])])) + 3)));
            waza_ptr->w_int = 6;
        }

        break;

    case 2:
        watch_lever_during_window();
        break;

    case 3:
        watch_lever_after_window();
        break;
    }
}

/* Does the lever satisfy this step, under whichever of the three matching modes
 * the step asks for? sw_work is set only in the exact-match mode, as in the
 * original - the other two modes leave it alone. */
static s32 lever_satisfies_step(u16 sw_lever) {
    if (waza_ptr->w_lvr & 0x8000) {
        sw_work = waza_ptr->w_lvr & 0xF;
        return sw_lever == sw_work;
    }

    if (waza_ptr->w_lvr == 0) {
        return sw_lever == 0;
    }

    return sw_lever & waza_ptr->w_lvr;
}

static void run_lever_command_check(u16 sw_lever) {
    if (dead_lvr_check()) {
        return;
    }

    if (lever_satisfies_step(sw_lever)) {
        finish_or_advance_command();
    }
}

void check_24() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    run_lever_command_check(chk_pl->now_lvbt & 0xF);
}

void check_25() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    run_lever_command_check(chk_pl->sw_lever & 0xF);
}

void check_26() { // 🟢
    u16 sw_lever = chk_pl->sw_now & 0xF;
    u16 sw_now_lvr = chk_pl->sw_lever & 0xF;

    if (!dead_lvr_check()) {
        sw_work = waza_ptr->w_lvr & 0xF;

        if (sw_lever != sw_work) {
            if (sw_now_lvr != sw_work && waza_ptr->uni0.tame.flag) {
                finish_or_advance_command();
            }
        } else {
            waza_ptr->uni0.tame.flag = 1;
        }
    }
}

