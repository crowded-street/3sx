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

        if (sw_lever == sw_work) {
            finish_or_advance_command();
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (sw_lever == 0) {
            finish_or_advance_command();
        }
    } else if (chk_pl->now_lvbt & 0xF && sw_lever & waza_ptr->w_lvr) {
        finish_or_advance_command();
    }
}

static void resolve_tame_flag_or_reset_timer() {
    if (waza_ptr->uni0.tame.flag) {
        waza_ptr->uni0.tame.flag = 0;

        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
        } else {
            check_next();
        }
    } else {
        waza_ptr->free2 = waza_ptr->free1;
        waza_ptr->w_int--;

        if (waza_ptr->w_int < 0) {
            waza_ptr->w_type = 0;
        }
    }
}

void check_1() { // 🟢
    if (dead_lvr_check()) {
        return;
    }

    sw_work = waza_ptr->w_lvr & 0xF;

    if (waza_ptr->w_lvr & 0x8000) {
        if (sw_work == chk_pl->sw_lever) {
            waza_ptr->free2--;

            if (!waza_ptr->uni0.tame.flag && waza_ptr->free2 < 0) {
                waza_ptr->uni0.tame.flag = 1;
            }
        } else {
            resolve_tame_flag_or_reset_timer();
        }
    } else {
        if (sw_work & chk_pl->sw_lever) {
            if (!waza_ptr->uni0.tame.flag) {
                waza_ptr->free1--;

                if (waza_ptr->free1 < 0) {
                    waza_ptr->uni0.tame.flag = 1;
                }
            }
        } else {
            resolve_tame_flag_or_reset_timer();
        }
    }
}

void check_2() { // 🟢
    sw_work = chk_pl->sw_new & waza_ptr->w_lvr;

    if (waza_ptr->w_lvr == sw_work) {
        if (!waza_ptr->uni0.tame.flag) {
            waza_ptr->free2--;

            if (waza_ptr->free2 < 0) {
                waza_ptr->uni0.tame.flag = 1;
            }
        }
    } else {
        if (waza_ptr->uni0.tame.flag && sw_work == 0) {
            waza_ptr->uni0.tame.flag = 0;

            finish_or_advance_command();

            return;
        }

        waza_ptr->free2 = waza_ptr->free1;
        waza_ptr->w_int--;

        if (waza_ptr->w_int < 0) {
            waza_ptr->w_type = 0;
        }
    }
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

    if (waza_ptr->shot_ok) {
        waza_ptr->free2--;

        if (waza_ptr->free2 < 0) {
            waza_ptr->shot_ok = 0;
            waza_ptr->free2 = waza_ptr->free1;
        }
    }

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;
        waza_ptr->free2 = waza_ptr->free1;

        finish_or_advance_command();
    }
}

static bool try_reset_on_tame_field(s16* field, s32 w_int_value) {
    if (wcp[cmd_id].waza_flag[waza_type[cmd_id]]) {
        if (waza_ptr->w_int > 0 && *field) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            *field = 0;
            waza_ptr->w_int = w_int_value;
            return true;
        }
    } else if (*field >= 5) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
        *field = 0;
        waza_ptr->w_int = w_int_value;
        chk_pl->waza_no = waza_type[cmd_id];
        return true;
    }

    return false;
}

void check_4() { // 🟢
    if (waza_ptr->w_lvr == 0x10) {
        if (chk_pl->sw_now & 0x10) {
            waza_ptr->uni0.tame.flag++;
        }

        if (chk_pl->sw_now & 0x20) {
            waza_ptr->uni0.tame.shot_flag++;
        }

        if (chk_pl->sw_now & 0x40) {
            waza_ptr->uni0.tame.shot_flag2++;
        }
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

void check_6() { // 🟢
    s16 i;
    u16 lvr_work;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
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
    } else {
        waza_ptr->free1--;

        if (waza_ptr->free1 <= 0) {
            waza_ptr->free1 = 14;
            waza_ptr->shot_ok = 0;
        }
    }

    lvr_work = 1 & 0xFFFF;

    for (i = 0; i < 4; i++) {
        if (chk_pl->sw_lever == lvr_work) {
            waza_ptr->shot_ok |= (lvr_work);
            waza_ptr->free1 = 14;
        }

        lvr_work *= 2;
    }

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

    if (waza_ptr->shot_ok) {
        waza_ptr->free2--;

        if (waza_ptr->free2 < 0) {
            waza_ptr->shot_ok = 0;
            waza_ptr->free2 = waza_ptr->free1;
            waza_ptr->uni0.tame.shot_flag = 0;
        }
    }

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;
        waza_ptr->free2 = waza_ptr->free1;

        finish_or_advance_command();
    }
}

static void resolve_extended_lever_command() {
    sw_work = waza_ptr->w_lvr & 0xF;

    if (waza_ptr->w_lvr == 0) {
        if (chk_pl->new_lvbt == 0) {
            finish_or_advance_command();
        }
    } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
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
}

void check_9() { // 🟢
    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
    }

    if (waza_ptr->w_lvr & 0x8000) {
        resolve_extended_lever_command();
    } else if (waza_ptr->w_lvr == 0) {
        if (chk_pl->new_lvbt == 0) {
            finish_or_advance_command();
            return;
        }

        if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
            waza_ptr->w_type = 0;
        }
    } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
        if (chk_pl->sw_lever & waza_ptr->w_lvr) {
            finish_or_advance_command();
            return;
        }

        waza_ptr->w_type = 0;
    }
}

void paring_miss_init() { // 🟢
    waza_ptr->free3 = 0;
    waza_ptr->w_type = 0;
    waza_ptr->uni0.tame.flag = 0;
    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
}

static void clear_flags_below(s16 self_ix, s16 other1, s16 other2, s16 other3, s16 other4) {
    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[other1]) {
        wcp[cmd_id].waza_flag[other1] = 0;
    }

    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[other2]) {
        wcp[cmd_id].waza_flag[other2] = 0;
    }

    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[other3]) {
        wcp[cmd_id].waza_flag[other3] = 0;
    }

    if (wcp[cmd_id].waza_flag[self_ix] > wcp[cmd_id].waza_flag[other4]) {
        wcp[cmd_id].waza_flag[other4] = 0;
    }
}

static void clear_lower_priority_waza_flags() {
    switch (waza_type[cmd_id]) {
    case 3:
        clear_flags_below(3, 4, 5, 6, 12);
        break;

    case 4:
        clear_flags_below(4, 3, 5, 6, 12);
        break;

    case 5:
        clear_flags_below(5, 3, 4, 6, 12);

        if (waza_work[cmd_id][6].free3 > 0) {
            wcp[cmd_id].waza_flag[5] = 0;
        }

        break;

    case 6:
        clear_flags_below(6, 3, 4, 5, 12);

        if (waza_work[cmd_id][5].free3 > 0) {
            wcp[cmd_id].waza_flag[6] = 0;
        }

        break;

    case 12:
        clear_flags_below(12, 3, 4, 5, 6);
        break;
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
        if (lever_held_and_move_allowed()) {
            if (chk_pl->sw_lever == waza_ptr->w_lvr) {
                waza_ptr->shot_ok++;
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                waza_ptr->free3 = wcp[cmd_id].reset[waza_type[cmd_id]] + 10;
                waza_ptr->w_int = 6;

                clear_lower_priority_waza_flags();
            } else {
                waza_ptr->shot_ok = 0;
                break;
            }
        }

        break;

    case 2:
        waza_ptr->w_int--;
        waza_ptr->free3--;

        if (waza_ptr->w_int > 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever & 8) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever != waza_ptr->w_lvr) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }
        } else {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            waza_ptr->shot_ok++;
        }

        break;

    case 3:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
            break;
        }

        if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            break;
        }

        if (chk_pl->sw_now & 0xF) {
            waza_ptr->shot_ok++;
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
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
        clear_flags_below(3, 4, 5, 6, 12);
        break;

    case 4:
        clear_flags_below(4, 3, 5, 6, 12);
        break;

    case 5:
        clear_flags_below(5, 3, 4, 6, 12);
        break;

    case 6:
        clear_flags_below(6, 3, 4, 5, 12);
        break;

    case 12:
        clear_flags_below(12, 3, 4, 5, 6);
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
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                waza_ptr->free3 = wcp[cmd_id].reset[waza_type[cmd_id]] + 10;
                waza_ptr->w_int = 6;

                clear_lower_priority_waza_flags_no_free3_check();
            } else {
                waza_ptr->shot_ok = 0;
                break;
            }
        }

        break;

    case 2:
        waza_ptr->w_int--;
        waza_ptr->free3--;

        if (waza_ptr->w_int > 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever & 8) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever != waza_ptr->w_lvr) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
                waza_ptr->shot_ok++;
                break;
            }
        } else {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            waza_ptr->shot_ok++;
        }

        break;

    case 3:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
            break;
        }

        if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            break;
        }

        if (chk_pl->sw_now & 0xF) {
            waza_ptr->shot_ok++;
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
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
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
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
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
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

        if (chk_pl->sw_lever == sw_work) {
            waza_ptr->shot_ok++;

            advance_when_shot_count_reached();
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (chk_pl->sw_lever == 0) {
            waza_ptr->shot_ok += 1;

            advance_when_shot_count_reached();
        }
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

void check_16() { // 🟢
    s16 i;
    u16 w_flag;

    waza_ptr->w_int--;

    if (waza_ptr->w_int < 0) {
        waza_ptr->w_type = 0;
        waza_ptr->shot_ok = 0;
        return;
    }

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

    if (waza_ptr->shot_ok >= waza_ptr->w_lvr) {
        waza_ptr->shot_ok = 0;

        if (*waza_ptr->w_ptr == 0x1C) {
            command_ok();
            return;
        }

        check_next();
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

    if (waza_ptr->w_lvr & 0x8000) {
        if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF)) {
            sw_work = waza_ptr->w_lvr & 0xF;

            if (sw_lever == sw_work) {
                waza_ptr->w_int = waza_ptr->free1;
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            }
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (chk_pl->sw_lever == 0) {
            waza_ptr->w_int = waza_ptr->free1;
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
        }
    } else if ((chk_pl->old_lvbt & 0xF) != (chk_pl->new_lvbt & 0xF) && (sw_lever & waza_ptr->w_lvr)) {
        waza_ptr->w_int = waza_ptr->free1;
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
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

    if (waza_ptr->w_lvr & 0x8000) {
        if (chk_pl->now_lvbt & 0xF) {
            sw_work = waza_ptr->w_lvr & 0xF;
            if (sw_lever == sw_work) {
                wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
                check_next();
            }
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (chk_pl->sw_lever == 0) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
            check_next();
        }
    } else if ((chk_pl->now_lvbt & 0xF) != 0 && (sw_lever & waza_ptr->w_lvr)) {
        wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];
        check_next();
    }
}

void check_20() { // 🟢
    // Do nothing
}

/* check_21's charged-lever arm. The one `command_ok(); return;` inside it left
 * check_21 with nothing after the if/else chain, so returning from the helper
 * reaches the same place. */
static void resolve_charged_lever_command(u16 sw_lever) {
    sw_work = waza_ptr->w_lvr & 0xF;
    if (sw_work == 0) {
        if (sw_lever == 0) {
            if (((*waza_ptr->w_ptr)) == 0x1C) {
                command_ok();
                return;
            }
            check_next();
        }
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

void check_22() { // 🟢
    s16 i;

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

    for (i = 0; i < 8; i++) {
        if (chk_pl->sw_lever == chk22_tbl[i]) {
            waza_ptr->free3 |= 1 << i;
        }
    }

    if (waza_ptr->free3 == 0xFF) {
        if (((*waza_ptr->w_ptr)) == 0x1C) {
            command_ok();
            return;
        }

        waza_ptr->free3 = 0;
        check_next();
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
        waza_ptr->w_int -= 1;
        waza_ptr->free3 -= 1;

        if (((waza_ptr->w_int)) > 0) {
            if (chk_pl->sw_lever == 0) {
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever & 8) {
                wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
                waza_ptr->shot_ok++;
                break;
            }

            if (chk_pl->sw_lever != ((waza_ptr->w_lvr))) {
                wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
                waza_ptr->w_type = 0;
                break;
            }
        } else {
            wcp[cmd_id].waza_flag[(waza_type[cmd_id])] = 0;
            waza_ptr->shot_ok++;
        }

        break;

    case 3:
        waza_ptr->free3--;

        if (waza_ptr->free3 < 0) {
            waza_ptr->w_type = 0;
            break;
        }

        if ((chk_pl->sw_now & 8) || !(chk_pl->sw_now != waza_ptr->w_lvr)) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            break;
        }

        if (chk_pl->sw_now & 0xF) {
            wcp[cmd_id].waza_flag[waza_type[cmd_id]] = 0;
            waza_ptr->w_type = 0;
        }

        break;
    }
}

static void run_lever_command_check(u16 sw_lever) {
    if (dead_lvr_check()) {
        return;
    }

    if (waza_ptr->w_lvr & 0x8000) {
        sw_work = waza_ptr->w_lvr & 0xF;

        if (sw_lever == sw_work) {
            finish_or_advance_command();
        }
    } else if (waza_ptr->w_lvr == 0) {
        if (sw_lever == 0) {
            finish_or_advance_command();
        }
    } else {
        if (sw_lever & waza_ptr->w_lvr) {
            finish_or_advance_command();
        }
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

