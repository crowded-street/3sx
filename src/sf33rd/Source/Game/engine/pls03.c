/**
 * @file pls03.c
 * Player Special Attack and Super Art Execution
 */

#include "sf33rd/Source/Game/engine/pls03.h"
#include "arcade/arcade_balance.h"
#include "arcade/arcade_stubs.h"
#include "bin2obj/asstbl.h"
#include "common.h"
#include "constants.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03_internal.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/system/sysdir.h"

// Forward decls

u16 decode_wst_data(PLW* wk, u16 cmd, s16 cmd_ex);

s32 player_is_grounded_or_on_car(const PLW* wk) {
    return ((Bonus_Game_Flag == 0x14) && wk->bs2_on_car) || (wk->wu.xyz[1].disp.pos <= 0);
}

/* Enter the attack's routine and clear the per-attack hit state. Two checks
 * write this out identically; the third puts a cancel_timer reset in the middle
 * of it, which is a difference, so it stays inline. */
static void begin_attack_routine(PLW* wk) {
    set_attack_routine_number(wk);
    wk->wu.paring_attack_flag = 0;
    wk->wu.meoshi_hit_flag = 0;
    wk->wu.att_hit_ok = 0;
    wk->wu.hf.hit_flag = 0;
}

static s32 player_is_airborne_off_car(const PLW* wk) {
    return ((Bonus_Game_Flag != 0x14) || !wk->bs2_on_car) && (wk->wu.xyz[1].disp.pos > 0);
}

static s32 slot_blocked_by_super(const PLW* wk, s16 i) {
    return (wk->cp->btix[i] & 0x1000) && (wk->metamorphose || (wk->sa->ok != -1));
}

void hissatsu_setup_union(PLW* wk, s16 rno) { // 🟢
    wk->wu.routine_no[1] = 4;
    wk->wu.routine_no[2] = rno;
    wk->wu.routine_no[3] = 0;
    wk->cancel_timer = 0;
    wk->wu.cg_type = 0;
    wk->wu.att_hit_ok = 0;
    wk->wu.hf.hit_flag = 0;
    wk->wu.meoshi_hit_flag = 0;
    wk->wu.paring_attack_flag = 0;
}

const s16 cmdixconv_table[36] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                  2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

s16 cmdixconv(s16 ix) { // 🔴
    return cmdixconv_table[ix - 20];
}

bool is_blocked_by_arcade_switch(PLW* wk, s16 ix) {
    if (ArcadeBalance_IsEnabled()) {
        if (wk->cp->btix[ix] & 0x4000) {
            if (DAT_020156b2 == 3 || DAT_020156b2 == 2) {
                return true;
            }
        }
    }

    return false;
}

static void commit_special_attack(PLW* wk, s16 i, s16 j) {
    wk->wu.cg_cancel &= 0x40;
    hissatsu_setup_union(wk, wk->cp->waza_r[i][j]);
    waza_flag_clear_only_1(wk->wu.id, i);
    grade_add_command_waza(wk->wu.id);

    if (!ArcadeBalance_IsEnabled()) {
        chainex_check[wk->wu.id][i - 20] = 1;
        chainex_spat_cancel_kidou(&wk->wu);
    }
}

/* The meter side of the EX gate, for a character that is not metamorphosed.
 * Returns 0 wherever ex_slot_is_allowed refused the slot. The `wk->sa->ex = -1`
 * that spends the EX stock stays inside the test it belongs to, in the position
 * the original had it. */
static s32 ex_meter_allows_slot(PLW* wk, s16 i) {
    if ((wk->sa->mp == -1) || (wk->sa->ok == -1)) {
        return 0;
    }

    if (wk->cp->btix[i] & 0x400) {
        if ((wk->spmv_ng_flag2 & DIP2_EX_MOVE_DISABLED) || (wk->sa->ex != 1)) {
            return 0;
        }

        wk->sa->ex = -1;
    }

    return 1;
}

/* The j == 3 slot of a special is the EX gate. Returns 0 wherever the original
 * moved on to the next j, and 1 where it fell through to the commit. The
 * `wk->sa->ex = -1` consumption stays inside the test it belongs to, in the same
 * position as the original.
 *
 * The grounded and airborne callers differ in exactly one value - which DIP
 * switch disables their specials - so it is passed in and each call site keeps
 * its own flag verbatim. */
static s32 ex_slot_is_allowed(PLW* wk, s16 i, u32 specials_disabled) {
    if (!(wk->cp->btix[i] & 0x600)) {
        return 0;
    }

    if ((wk->cp->btix[i] & 0x200) && (wk->spmv_ng_flag & specials_disabled)) {
        return 0;
    }

    if (wk->metamorphose) {
        if (wk->cp->btix[i] & 0x400) {
            return 0;
        }
    } else if (!ex_meter_allows_slot(wk, i)) {
        return 0;
    }

    return 1;
}

/* The four reasons a grounded slot is passed over, in the order the original
 * tested them. They are not the airborne four: this one checks a live shell
 * where the airborne one checks downward momentum. */
static s32 grounded_slot_is_skipped(PLW* wk, s16 i) {
    return ((wk->spmv_ng_flag2 & DIP2_UNKNOWN_22) && chainex_check[wk->wu.id][i - 20]) ||
           (wk->cp->waza_flag[i] == -1) || ((wk->cp->btix[i] & 0x800) && shell_live_check(wk, i)) ||
           slot_blocked_by_super(wk, i);
}

/* The four button strengths of one grounded special slot, strongest first. The
 * mirror of try_airborne_special_strengths; the two differ in which DIP switch
 * they read and in both table offsets, so they are not merged. */
/* Whether this button strength must be passed over. The fourth strength is the
 * EX slot and has its own gate; the other three only check the DIP switch. The
 * sense stays negative, as the original's two `continue`s had it. */
static s32 ground_strength_is_blocked(PLW* wk, s16 i, s16 j) {
    if (j == 3) {
        return !ex_slot_is_allowed(wk, i, DIP_GROUND_SPECIALS_DISABLED);
    }

    return wk->spmv_ng_flag & DIP_GROUND_SPECIALS_DISABLED;
}

static s32 try_grounded_special_strengths(PLW* wk, s16 i, u16 cusw) {
    s16 j;
    u16 exsw;

    for (j = 3; j >= 0; j--) {
        exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[i][j]];

        if (exsw != cmdshot_conv_tbl[wk->cp->exdt[i][j] & 0xF]) {
            continue;
        }

        if (ground_strength_is_blocked(wk, i, j)) {
            continue;
        }

        setup_comm_back(&wk->wu);

        if (ArcadeBalance_IsEnabled()) {
            wk->as = &asstbl_lv_9900_g_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][j + (i - 20) * 4];
        } else {
            wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(i)][j + (i - 28) * 4];
        }

        commit_special_attack(wk, i, j);

        return 1;
    }

    return 0;
}

/* The slot fires on its button index alone, or its command has not completed. */
static s32 grounded_slot_has_no_command(const PLW* wk, s16 i) {
    return ((wk->cp->btix[i] & 0xFF) == 0x80) || !wk->cp->waza_flag[i];
}

static s32 check_special_attack_grounded(PLW* wk) {
    s16 i;
    u16 cusw;
    u16* conpane;

    conpane = &wk->cp->sw_lvbt;

    for (i = 28; i < 38; i++) {
        if (grounded_slot_is_skipped(wk, i)) {
            continue;
        }

        if (is_blocked_by_arcade_switch(wk, i)) {
            return 0;
        }

        if (grounded_slot_has_no_command(wk, i)) {
            continue;
        }

        cusw = conpane[wk->cp->btix[i] & 0xFF];

        if (try_grounded_special_strengths(wk, i, cusw)) {
            return 1;
        }
    }

    return 0;
}

/* The four reasons an airborne slot is passed over, in the order the original
 * tested them. `||` short-circuits left to right, so each test is still reached
 * only when every earlier one was false. */
static s32 airborne_slot_is_skipped(const PLW* wk, s16 i) {
    return ((wk->spmv_ng_flag2 & DIP2_UNKNOWN_22) && chainex_check[wk->wu.id][i - 20]) ||
           (wk->cp->waza_flag[i] == -1) || slot_blocked_by_super(wk, i) ||
           ((wk->cp->btix[i] & 0x2000) && (wk->wu.mvxy.a[0].sp < 0));
}

/* The four button strengths of one airborne special slot, strongest first.
 * Returns 1 when one of them fired, which is where check_special_attack_airborne
 * returned 1; returning 0 is where it carried on to the next slot.
 *
 * check_special_attack_grounded keeps its own copy of this loop. The two differ
 * in which DIP switch they read and in both table offsets, so sharing would mean
 * changing literals, and extracting from both would only create a twin pair. */
/* Whether this button strength must be passed over. The fourth strength is the
 * EX slot and has its own gate; the other three only check the DIP switch. The
 * sense stays negative, as the original's two `continue`s had it. */
static s32 air_strength_is_blocked(PLW* wk, s16 i, s16 j) {
    if (j == 3) {
        return !ex_slot_is_allowed(wk, i, DIP_AIR_SPECIALS_DISABLED);
    }

    return wk->spmv_ng_flag & DIP_AIR_SPECIALS_DISABLED;
}

static s32 try_airborne_special_strengths(PLW* wk, s16 i, u16 cusw) {
    s16 j;
    u16 exsw;

    for (j = 3; j >= 0; j--) {
        exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[i][j]];

        if (exsw != cmdshot_conv_tbl[wk->cp->exdt[i][j] & 0xF]) {
            continue;
        }

        if (air_strength_is_blocked(wk, i, j)) {
            continue;
        }

        setup_comm_back(&wk->wu);

        if (ArcadeBalance_IsEnabled()) {
            wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][j + (i - 38) * 4];
        } else {
            wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(i)][j + (i - 46) * 4];
        }

        commit_special_attack(wk, i, j);

        return 1;
    }

    return 0;
}

/* Rising and still low: a special cannot be cancelled into another one here,
 * unless arcade balance or the DIP switch says otherwise. Returns 1 wherever
 * check_special_attack_airborne returned 0 at its top. */
static s32 air_special_cancel_is_blocked(const PLW* wk) {
    if ((wk->wu.mvxy.a[1].sp > 0) && (wk->wu.xyz[1].disp.pos < 32)) {
        if (ArcadeBalance_IsEnabled()) {
            return 1;
        } else if (wk->spmv_ng_flag2 & DIP2_SPECIAL_TO_SPECIAL_CANCEL_DISABLED) {
            return 1;
        }
    }

    return 0;
}

/* A slot whose button index is 0x80 fires on the slot alone, with no strength
 * to choose, so it commits at strength 0. */
static void commit_airborne_button_special(PLW* wk, s16 i) {
    setup_comm_back(&wk->wu);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][(i - 38) * 4];
    } else {
        wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(i)][(i - 46) * 4];
    }

    commit_special_attack(wk, i, 0);
}

/* One airborne slot: a slot with a button index picks a strength from the
 * command, and a slot without one fires on its own. Returns 1 when a special
 * started, which is where check_special_attack_airborne returned 1; returning 0
 * is where it carried on to the next slot. */
static s32 try_airborne_slot(PLW* wk, s16 i, u16* conpane) {
    u16 cusw;

    if ((wk->cp->btix[i] & 0xFF) != 0x80) {
        if (!wk->cp->waza_flag[i]) {
            return 0;
        }

        cusw = conpane[wk->cp->btix[i] & 0xFF];

        return try_airborne_special_strengths(wk, i, cusw);
    }

    if (wk->cp->waza_flag[i]) {
        commit_airborne_button_special(wk, i);
        return 1;
    }

    return 0;
}

static s32 check_special_attack_airborne(PLW* wk) {
    s16 i;
    u16* conpane;

    if (air_special_cancel_is_blocked(wk)) {
        return 0;
    }

    conpane = &wk->cp->sw_lvbt;

    for (i = 46; i < 56; i++) {
        if (airborne_slot_is_skipped(wk, i)) {
            continue;
        }

        if (is_blocked_by_arcade_switch(wk, i)) {
            return 0;
        }

        if (try_airborne_slot(wk, i, conpane)) {
            return 1;
        }
    }

    return 0;
}

s32 check_special_attack(PLW* wk) { // 🟡
    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 2;
    }

    if (pcon_dp_flag) {
        return 0;
    }

    if (player_is_grounded_or_on_car(wk)) {
        return check_special_attack_grounded(wk);
    }

    return check_special_attack_airborne(wk);
}

void chainex_spat_cancel_kidou(WORK* wk) { // 🔴
    // This movement adjustment has no CPS3 counterpart; every caller excludes it in Arcade Balance mode.
    MVXY curr;

    if (wk->old_rno[1] == 4 && wk->old_rno[2] > 15) {
        curr = wk->mvxy;
        setup_mvxy_data(wk, 10);
        wk->mvxy.a[0].sp = curr.a[0].sp / 2;
        wk->mvxy.d[0].sp = 0;
        wk->mvxy.a[1].sp = curr.a[1].sp / 2;
    }
}

/// Universal overhead check
/* The universal overhead's input requirement, which the DIP switch swaps
 * between a dedicated command and the default two-button input. Returns 1
 * wherever check_leap_attack returned 0. */
/* With the universal-overhead DIP on, the leap reads its dedicated button and
 * refuses while any lever direction is held. */
static s32 leap_overhead_input_is_missing(const PLW* wk) {
    if (wk->cp->ca25 == 0) {
        return 1;
    }

    if (wk->cp->sw_lvbt & 0xF) {
        return 1;
    }

    return 0;
}

/* Without it, the leap is a command and needs a button actually held. */
static s32 leap_command_input_is_missing(const PLW* wk) {
    if (wk->cp->waza_flag[14] == 0) {
        return 1;
    }

    if (!(wk->cp->sw_now & 0x770)) {
        return 1;
    }

    return 0;
}

static s32 leap_input_is_missing(const PLW* wk) {
    if (wk->spmv_ng_flag2 & DIP2_UNIVERSAL_OVERHEAD_DEFAULT_INPUT_ENABLED) {
        return leap_overhead_input_is_missing(wk);
    }

    return leap_command_input_is_missing(wk);
}

s32 check_leap_attack(PLW* wk) { // 🟡
    if (wk->spmv_ng_flag2 & DIP2_UNIVERSAL_OVERHEAD_DISABLED) {
        return 0;
    }

    if (pcon_dp_flag) {
        return 0;
    }

    wk->permited_koa |= 0x200;

    if (leap_input_is_missing(wk)) {
        return 0;
    }

    if (player_is_airborne_off_car(wk)) {
        return 0;
    }

    setup_comm_back(&wk->wu);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_D010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][0];
    } else {
        wk->as = &_asstbl_lv_D010[wk->player_number];
    }

    hissatsu_setup_union(wk, wk->cp->waza_r[14][0]);
    return 1;
}

static bool is_blocked_by_hikusugi(PLW* wk) {
    if (!ArcadeBalance_IsEnabled()) {
        if (hikusugi_check(&wk->wu)) {
            return true;
        }
    }

    return false;
}

static void select_nm_attack_level_3010(PLW* wk, s16 kos, s16 level) {
    s16 koa = waza_select(wk, kos, level);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_3010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos][koa];
    } else {
        wk->as = &_asstbl_lv_3010[wk->player_number][kos][koa];
    }
}

static void select_nm_attack_level_2010(PLW* wk, s16 kos, s16 level) {
    s16 koa = waza_select(wk, kos, level);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_2010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos][koa];
    } else {
        wk->as = &_asstbl_lv_2010[wk->player_number][kos][koa];
    }
}

static void select_nm_attack_level_4010(PLW* wk, s16 kos, s16 level) {
    s16 koa = waza_select(wk, kos, level);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_4010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos][koa];
    } else {
        wk->as = &_asstbl_lv_4010[wk->player_number][kos][koa];
    }
}

/* The standing and crouching attacks, which are chosen from a different pair
 * of tables depending on whether the lever is down. Returns 0 when the player
 * is in the air and no attack is chosen at all. */
static s32 select_standing_nm_attack(PLW* wk, s16 kos) {
    s16 koa;

    if (player_is_airborne_off_car(wk)) {
        return 0;
    }

    if (wk->cp->sw_lvbt & 2) {
        koa = waza_select(wk, kos, 1);

        if (ArcadeBalance_IsEnabled()) {
            wk->as = &asstbl_lv_1010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos][koa];
        } else {
            wk->as = &_asstbl_lv_1010[wk->player_number][kos][koa];
        }

        return 1;
    }

    koa = waza_select(wk, kos, 0);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_0010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos][koa];
    } else {
        wk->as = &_asstbl_lv_0010[wk->player_number][kos][koa];
    }

    return 1;
}

/* Every crouching and jumping stance starts its normal the same way: the
 * too-low test first, then the selector for that stance's family at that
 * stance's level. Only the call differs, and each stance writes its own. */
static s32 start_nm_attack(PLW* wk, s16 kos, s16 level, void (*select)(PLW*, s16, s16)) {
    if (is_blocked_by_hikusugi(wk)) {
        return 0;
    }

    select(wk, kos, level);
    return 1;
}

/* The jumping stances and the standing fallback, split off so neither half of
 * the stance dispatch is long. The case labels are the ones decode_wst_data
 * writes into pat_status; they are not renumbered. */
static s32 begin_jumping_nm_attack(PLW* wk, s16 kos) {
    switch (wk->wu.pat_status) {
    case 24:
        return start_nm_attack(wk, kos, 4, select_nm_attack_level_4010);

    case 18:
        return start_nm_attack(wk, kos, 7, select_nm_attack_level_4010);

    case 30:
        return start_nm_attack(wk, kos, 10, select_nm_attack_level_4010);

    default:
        if (!select_standing_nm_attack(wk, kos)) {
            return 0;
        }

        break;
    }

    return 1;
}

/* Which normal attack the current stance starts, and whether one starts at
 * all: every crouching and jumping stance is blocked by the same too-low
 * test first, and the standing case answers for itself. */
static s32 begin_nm_attack(PLW* wk, s16 kos) {
    switch (wk->wu.pat_status) {
    case 20:
        return start_nm_attack(wk, kos, 3, select_nm_attack_level_3010);

    case 14:
        return start_nm_attack(wk, kos, 6, select_nm_attack_level_3010);

    case 26:
        return start_nm_attack(wk, kos, 9, select_nm_attack_level_3010);

    case 22:
        return start_nm_attack(wk, kos, 2, select_nm_attack_level_2010);

    case 16:
        return start_nm_attack(wk, kos, 5, select_nm_attack_level_2010);

    case 28:
        return start_nm_attack(wk, kos, 8, select_nm_attack_level_2010);

    default:
        return begin_jumping_nm_attack(wk, kos);
    }
}

s32 check_nm_attack(PLW* wk) { // 🟡
    s16 kos;

    wk->permited_koa |= 4;

    if ((kos = shot_data_convert(wk->cp->sw_now)) < 0) {
        return 0;
    }

    if (!begin_nm_attack(wk, kos)) {
        return 0;
    }

    setup_comm_back(&wk->wu);
    wk->current_attack = shot_data_refresh(kos);
    begin_attack_routine(wk);
    wk->wu.cg_cancel &= 0xF8;
    return 1;
}

s16 hikusugi_check(WORK* wk) { // 🔴
    s16 rnum = 0;

    if ((wk->mvxy.a[1].real.h < 0) && (wk->xyz[1].disp.pos < 16)) {
        rnum = 1;
    }

    return rnum;
}

s32 FUN_06120790(PLW* wk) { // 🔵
    fatal_error("Not implemented");
}

/// Taunt check
/* A taunt is refused while the lever is held. Hugo is checked against a
 * narrower mask than everyone else - that is the existing FIXME, reported and
 * left exactly as it was. */
static s32 taunt_lever_is_held(const PLW* wk) {
    if (wk->player_number == CHAR_HUGO) { // FIXME: Make Hugo's taunt work with Start
        return wk->cp->sw_lvbt & 0xE;
    }

    return wk->cp->sw_lvbt & 0xF;
}

s32 check_chouhatsu(PLW* wk) { // 🟢 Same overall but differs because of Start and DIP switches
    if (wk->spmv_ng_flag & DIP_TAUNT_DISABLED) {
        return 0;
    }

    if ((wk->spmv_ng_flag & DIP_TAUNT_AFTER_KO_DISABLED) && pcon_dp_flag) {
        return 0;
    }

    wk->permited_koa |= 0x80;

    if (wk->wu.xyz[1].disp.pos > 0) {
        return 0;
    }

    if (taunt_lever_is_held(wk)) {
        return 0;
    }

    if (wk->cp->ca36 == 0) {
        return 0;
    }

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_E010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][0];
    } else {
        wk->as = &_asstbl_lv_E010[wk->player_number];
    }

    setup_comm_back(&wk->wu);
    begin_attack_routine(wk);
    return 1;
}

/// Check for throw tech input
/// @param wk Player object whos inputs should be checked
/// @return 1 if throw tech input was registered, 0 otherwise
s32 check_nagenuke_cmd(PLW* wk) { // 🟢 Same overall but differs because of DIP switches
    if (wk->spmv_ng_flag2 & DIP2_THROW_BREAK_DISABLED) {
        return 0;
    }

    if (wk->cat_break_reserve) {
        return 1;
    }

    if ((wk->spmv_ng_flag2 & DIP2_THROW_BREAK_LOCKOUT_ENABLED) && (wk->cp->sw_lvbt & 3)) {
        return 0;
    }

    if (wk->cp->sw_now & 0x660) {
        return 0;
    }

    if (wk->cp->ca14) {
        return 1;
    }

    return 0;
}

const u8 nml_catch_h2_ok[2][21] = { { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                      0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
                                    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x11, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00 } };

/* Arcade balance has a separate crouching throw, and a character may be
 * allowed one and not the other. */
/* Bit 0x10 of the character's catch table says whether that catch exists at all;
 * row 0 is the grounded table and row 1 the airborne one. */
static s32 catch_is_disabled_for_character(const PLW* wk, s16 row) {
    return !(nml_catch_h2_ok[row][CHAR_3SX_TO_ARCADE(wk->player_number)] & 0x10);
}

static s32 resolve_arcade_ground_catch(PLW* wk, s16 kos) {
    if (wk->cp->sw_lvbt & 1) {
        return 0;
    }

    if (wk->cp->sw_lvbt & 2) {
        if (!(nml_catch_h2_ok[0][CHAR_3SX_TO_ARCADE(wk->player_number)] & 1)) {
            return 0;
        }

        kos += 3;
    }

    wk->as = &asstbl_lv_A010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos];
    return 1;
}

static s32 resolve_ground_catch_target(PLW* wk, s16 kos) {
    if (catch_is_disabled_for_character(wk, 0)) {
        return 0;
    }

    if (ArcadeBalance_IsEnabled()) {
        return resolve_arcade_ground_catch(wk, kos);
    }

    if (wk->cp->sw_lvbt & 3) {
        return 0;
    }

    wk->as = &_asstbl_lv_A010[kos];
    return 1;
}

static s32 resolve_air_catch_target(PLW* wk, s16 kos) {
    if (catch_is_disabled_for_character(wk, 1)) {
        return 0;
    }

    if (wk->wu.xyz[1].disp.pos < 24) {
        return 0;
    }

    if (ArcadeBalance_IsEnabled()) {
        if (wk->cp->sw_lvbt & 2) {
            if (!(nml_catch_h2_ok[1][CHAR_3SX_TO_ARCADE(wk->player_number)] & 1)) {
                return 0;
            }

            kos += 3;
        }

        wk->as = &asstbl_lv_B010_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos];
    } else {
        wk->as = &_asstbl_lv_B010[kos];
    }

    return 1;
}

/* Which catch the pattern status calls for. The two resolvers are asked the same
 * question and answer the same way, so the caller only needs the one test. */
static s32 resolve_catch_target(PLW* wk, s16 kos) {
    if ((wk->wu.pat_status < 0xE) || (wk->wu.pat_status > 0x1E)) {
        return resolve_ground_catch_target(wk, kos);
    }

    return resolve_air_catch_target(wk, kos);
}

s32 check_catch_attack(PLW* wk) { // 🟡
    s16 kos;

    if (pcon_dp_flag) {
        return 0;
    }

    if (wk->spmv_ng_flag2 & DIP2_THROW_DISABLED) {
        return 0;
    }

    wk->permited_koa |= 0x100;

    if (wk->cp->ca14 == 0) {
        return 0;
    }

    kos = ((wk->cp->sw_new & 4) != 0) + (((wk->cp->sw_new & 8) != 0) * 2);

    if (!resolve_catch_target(wk, kos)) {
        return 0;
    }

    setup_comm_back(&wk->wu);
    set_attack_routine_number(wk);
    wk->cancel_timer = 0;
    wk->wu.paring_attack_flag = 0;
    wk->wu.meoshi_hit_flag = 0;
    wk->wu.att_hit_ok = 0;
    wk->wu.hf.hit_flag = 0;
    wk->wu.cg_cancel = 0;
    return 1;
}

void set_attack_routine_number(PLW* wk) { // 🟢
    wk->wu.routine_no[1] = 4;
    wk->wu.routine_no[2] = wk->as->r_no;
    wk->wu.routine_no[3] = 0;
    wk->wu.cg_type = 0;
}

u16 get_nearing_range(s16 pnum, s16 kos) {
    const u16* asstbl;
    u16 nrange = 0;
    u16 lwork;

    if ((kos = shot_data_convert(kos)) < 0) {
        return nrange;
    }

    asstbl = _asstbl_lv_0000[pnum][kos];

    if ((lwork = asstbl[0] & 0x7FF)) {
        nrange = lwork;
    } else {
        if ((lwork = asstbl[1] & 0x7FF)) {
            nrange = lwork;
        }
    }

    return nrange;
}

/* Every arm of waza_select picks between an arcade table and the PS2 one the
 * same way, indexing the first with the converted character number and the
 * second with the raw one. Only the pair of tables differs, and each arm passes
 * its own two by name. */
static const u16* select_waza_table(const PLW* wk, s16 kos, AsstblCharRows* arcade, AsstblCharRows* ps2) {
    if (ArcadeBalance_IsEnabled()) {
        return arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][kos];
    }

    return ps2[wk->player_number][kos];
}

/* Which waza table a step level reads. The switch used to sit inside
 * waza_select; its default returned 0 from that function, which here is a null
 * table the caller turns back into the same 0. */
/* The upper half of the step-level table dispatch. Split off because one switch
 * over eleven levels is one Complex Method; the lower half hands everything it
 * does not name to this one, so an unknown level still reaches the same null. */
static const u16* select_waza_table_for_high_step(PLW* wk, s16 kos, s16 sf) {
    switch (sf) {
    case 3:
    case 6:
    case 9:
        return select_waza_table(wk, kos, asstbl_lv_3000_arcade, _asstbl_lv_3000);

    case 4:
    case 7:
    case 10:
        return select_waza_table(wk, kos, asstbl_lv_4000_arcade, _asstbl_lv_4000);

    default:
        return NULL;
    }
}

/* Which waza table a step level reads. The switch used to sit inside
 * waza_select; its default returned 0 from that function, which here is a null
 * table the caller turns back into the same 0. */
static const u16* select_waza_table_for_step(PLW* wk, s16 kos, s16 sf) {
    switch (sf) {
    case 0:
        return select_waza_table(wk, kos, asstbl_lv_0000_arcade, _asstbl_lv_0000);

    case 1:
        return select_waza_table(wk, kos, asstbl_lv_1000_arcade, _asstbl_lv_1000);

    case 2:
    case 5:
    case 8:
        return select_waza_table(wk, kos, asstbl_lv_2000_arcade, _asstbl_lv_2000);

    default:
        return select_waza_table_for_high_step(wk, kos, sf);
    }
}

s32 waza_select(PLW* wk, s16 kos, s16 sf) { // 🟢
    const u16* wst = select_waza_table_for_step(wk, kos, sf);

    if (wst == NULL) {
        return 0;
    }

    if (decode_wst_data(wk, wst[0], wst[1])) {
        return 2;
    }

    if (decode_wst_data(wk, wst[1], wst[1])) {
        return 1;
    }

    return 0;
}

static s32 rising_within_height(const PLW* wk, s16 cmd_ex) {
    return (wk->wu.mvxy.a[1].sp > 0) && cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex);
}

static s32 falling_within_height(const PLW* wk, s16 cmd_ex) {
    return (wk->wu.mvxy.a[1].sp <= 0) && cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex);
}

/* Rising, holding exactly this lever direction, and within the step's height.
 * The falling twin below is a separate predicate: the two differ in `>` against
 * `<=`, which Recipe D may not parameterise. */
static s32 rising_with_lever_at_height(PLW* wk, u16 lever, s16 cmd_ex) {
    return (wk->wu.mvxy.a[1].sp > 0) && (lever == (wk->cp->sw_new & 0xF)) &&
           cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex);
}

/* Falling, holding exactly this lever direction, and within the step's height. */
static s32 falling_with_lever_at_height(PLW* wk, u16 lever, s16 cmd_ex) {
    return (wk->wu.mvxy.a[1].sp <= 0) && (lever == (wk->cp->sw_new & 0xF)) &&
           cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex);
}

/* The wst dispatch, split four ways. One switch over eleven lever-and-height
 * combinations plus a range default is one Complex Method; each part here keeps
 * its own arms and hands everything it does not name to the next, so a cmd that
 * matches none of them still arrives at the range test exactly as before. */
/* The last arm of the wst dispatch and the body-range default it falls back to. */
static u16 decode_wst_body_range_data(PLW* wk, u16 cmd, u16 lever, s16 cmd_ex) {
    u16 rnum = 0;

    switch (cmd & 0xF000) {
    case 0x5000:
        if (falling_within_height(wk, cmd_ex)) {
            rnum = wk->cp->sw_new & lever;
        }

        break;

    default:
        if (get_em_body_range(&wk->wu) >= cmd) {
            rnum = 1;
        }

        break;
    }

    return rnum;
}

static u16 decode_wst_range_data(PLW* wk, u16 cmd, u16 lever, s16 cmd_ex) {
    u16 rnum = 0;

    switch (cmd & 0xF000) {
    case 0x9000:
        if (falling_with_lever_at_height(wk, lever, cmd_ex)) {
            rnum = 1;
        }

        break;

    case 0x6000:
        if (rising_within_height(wk, cmd_ex)) {
            rnum = wk->cp->sw_new & lever;
        }

        break;

    default:
        rnum = decode_wst_body_range_data(wk, cmd, lever, cmd_ex);
        break;
    }

    return rnum;
}

static u16 decode_wst_height_data(PLW* wk, u16 cmd, u16 lever, s16 cmd_ex) {
    u16 rnum = 0;

    switch (cmd & 0xF000) {
    case 0x2000:
        if (rising_within_height(wk, cmd_ex)) {
            rnum = 1;
        }

        break;

    case 0x1000:
        if (falling_within_height(wk, cmd_ex)) {
            rnum = 1;
        }

        break;

    case 0xA000:
        if (rising_with_lever_at_height(wk, lever, cmd_ex)) {
            rnum = 1;
        }

        break;

    default:
        rnum = decode_wst_range_data(wk, cmd, lever, cmd_ex);
        break;
    }

    return rnum;
}

static u16 decode_wst_lever_and_height_data(PLW* wk, u16 cmd, u16 lever, s16 cmd_ex) {
    u16 rnum = 0;

    switch (cmd & 0xF000) {
    case 0x7000:
        if ((wk->cp->sw_new & lever) && cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex)) {
            rnum = 1;
        }

        break;

    case 0xB000:
        if ((lever == (wk->cp->sw_new & 0xF)) && cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex)) {
            rnum = 1;
        }

        break;

    default:
        rnum = decode_wst_height_data(wk, cmd, lever, cmd_ex);
        break;
    }

    return rnum;
}

u16 decode_wst_data(PLW* wk, u16 cmd, s16 cmd_ex) { // 🟢
    u16 lever;
    u16 rnum;

    if (cmd == 0) {
        return 0;
    }

    rnum = 0;
    lever = cmd & 0xF;

    switch (cmd & 0xF000) {
    case 0x4000:
        rnum = wk->cp->sw_new & lever;
        break;

    case 0x8000:
        rnum = (lever == (wk->cp->sw_new & 0xF));
        break;

    case 0x3000:
        rnum = cmd_ex_check(wk->wu.xyz[1].disp.pos, cmd_ex);
        break;

    default:
        rnum = decode_wst_lever_and_height_data(wk, cmd, lever, cmd_ex);
        break;
    }

    return rnum;
}

/* On the car bonus stage the range is measured to the car's own effect work,
 * from the middle of its correction box. */
static s16 body_range_to_bonus_car(WORK* wk) {
    WORK* em = (WORK*)((WORK*)wk->target_adrs)->my_effadrs;
    s16* dad = (s16*)(em->hosei_adrs + (get_sel_hosei_tbl_ix(((WORK_Other*)em)->master_player) + 1));
    s16 res_hs = wk->xyz[0].disp.pos - (em->xyz[0].disp.pos + dad[0] + (dad[1] / 2));

    if (res_hs < 0) {
        res_hs = -res_hs;
    }

    res_hs -= (dad[1] / 2);
    return res_hs;
}

/* Otherwise it is measured to the opponent, from the near edge of their box. */
static s16 body_range_to_opponent(WORK* wk) {
    WORK* em = (WORK*)wk->target_adrs;
    s16 res_hs = wk->xyz[0].disp.pos - em->xyz[0].disp.pos;

    if (res_hs < 0) {
        res_hs = -res_hs;
    }

    res_hs += em->hosei_adrs[1].hos_box[0];
    return res_hs;
}

s16 get_em_body_range(WORK* wk) { // 🟢
    if (Bonus_Game_Flag == 20 && wk->operator != 0) {
        return body_range_to_bonus_car(wk);
    }

    return body_range_to_opponent(wk);
}

s32 cmd_ex_check(s16 px, s16 cx) { // 🟢
    if (cx) {
        if (cx < 0) {
            if (px + cx <= 0) {
                return 1;
            }
        } else if (px - cx >= 0) {
            return 1;
        }
    } else {
        return 1;
    }

    return 0;
}

const s16 shot_prio[6][2] = { { 256, 3 }, { 16, 0 }, { 512, 4 }, { 32, 1 }, { 1024, 5 }, { 64, 2 } };

s16 shot_data_convert(u16 sw) { // 🟢
    s16 i;
    s16 rnum = -1;

    for (i = 0; i < 6; i++) {
        if (sw & shot_prio[i][0]) {
            rnum = shot_prio[i][1];
        }
    }

    return rnum;
}

const s16 shot_refresh[6] = { 16, 32, 64, 256, 512, 1024 };

s16 shot_data_refresh(s16 sw) { // 🟢
    return shot_refresh[sw];
}

const s16 rc_shot_conv[16] = { 0x10,  0x20,  0x40,  0x70,  0x100, 0x200, 0x400, 0x700,
                               0x110, 0x220, 0x440, 0x770, 0x0,   0x0,   0x0,   0x0 };

const s16 rc_shot_conv_arcade[16] = { 0x10, 0x20,  0x40,  0x70,  0x80, 0x100, 0x200, 0x380,
                                      0x90, 0x120, 0x240, 0x3F0, 0x0,  0x0,   0x0,   0x0 };

s16 renbanshot_conpaneshot(const s16* dadr, s16 pow) { // 🟡
    const int ix = dadr[pow] & 0xF;

    if (ArcadeBalance_IsEnabled()) {
        return rc_shot_conv_arcade[ix];
    } else {
        return rc_shot_conv[ix];
    }
}

s16 datacmd_conpanecmd(s16 dat) { // 🟢
    dat = (dat & 0x700) >> 1 | (dat & 0x7F);
    return dat;
}

const u8 renda_status_table[4] = { 0, 20, 32, 0 };

s32 check_renda_cancel(PLW* wk) { // 🟢
    if (wk->wu.rl_flag != wk->wu.rl_waza) {
        return 0;
    }

    wk->permited_koa |= 32;

    if (wk->wu.pat_status == renda_status_table[wk->cp->sw_new & 3] && wk->current_attack == (wk->cp->sw_now & 0x770)) {
        setup_comm_back(&wk->wu);
        wk->wu.cg_ix = wk->wu.cg_eftype * wk->wu.cgd_type - (wk->wu.cgd_type * 2);
        wk->wu.cg_next_ix = 0;
        wk->wu.cg_ctr = 1;
        wk->wu.meoshi_hit_flag = 0;
        wk->wu.att_hit_ok = 0;
        wk->wu.hf.hit_flag = 0;
        wk->caution_flag = 1;
        wk->cancel_timer = 0;
        wk->wu.cg_cancel &= 0xE0;
        pp_pulpara_remake_at_init2(&wk->wu);
        return 1;
    }

    return 0;
}

const s16 cnmc_conv_data[16] = { 5, 8, 2, 0, 4, 7, 1, 0, 6, 9, 3, 0, 0, 0, 0, 0 };

const s16 cnmc_Z_lever_data[16][6] = { { 0, -1, -1, -1, -1, -1 }, { 1, -1, -1, -1, -1, -1 }, { 2, -1, -1, -1, -1, -1 },
                                       { 3, -1, -1, -1, -1, -1 }, { 4, -1, -1, -1, -1, -1 }, { 5, -1, -1, -1, -1, -1 },
                                       { 6, -1, -1, -1, -1, -1 }, { 7, -1, -1, -1, -1, -1 }, { 8, -1, -1, -1, -1, -1 },
                                       { 9, -1, -1, -1, -1, -1 }, { 4, 5, 6, 7, 8, 9 },      { 1, 2, 3, 4, 5, 6 },
                                       { 4, 5, -1, -1, -1, -1 },  { 5, 6, -1, -1, -1, -1 },  { 4, 6, -1, -1, -1, -1 },
                                       { 4, 5, 6, -1, -1, -1 } };

const s16 cnmc_z_lever_data[16][8] = { { -1, -1, -1, -1, -1, -1, -1, -1 }, { 4, 1, 2, -1, -1, -1, -1, -1 },
                                       { 1, 2, 3, -1, -1, -1, -1, -1 },    { 2, 3, 6, -1, -1, -1, -1, -1 },
                                       { 1, 4, 7, -1, -1, -1, -1, -1 },    { 1, 2, 3, 4, 6, 7, 8, 9 },
                                       { 3, 6, 9, -1, -1, -1, -1, -1 },    { 4, 7, 8, -1, -1, -1, -1, -1 },
                                       { 7, 8, 9, -1, -1, -1, -1, -1 },    { 8, 9, 6, -1, -1, -1, -1, -1 },
                                       { 1, 3, 4, 5, 6, 7, 8, 9 },         { 1, 2, 3, 4, 5, 6, 7, 9 },
                                       { 1, 4, 5, 7, -1, -1, -1, -1 },     { 3, 5, 6, 9, -1, -1, -1, -1 },
                                       { 1, 4, 7, 3, 6, 9, -1, -1 },       { 1, 4, 7, 5, 3, 6, 9, -1 } };

/* One row of lever data, scanned to its -1 terminator. The two meoshi tables
 * differ in the table and in the row length; both are written out at the call
 * site, and the row is subscripted there too, so the scan itself takes an
 * ordinary `const s16*` and no array extent enters this file as a literal. */
static s32 lever_row_matches(const s16* row, s16 count, s16 wdat) {
    s16 i;

    for (i = 0; i < count; i++) {
        if (row[i] == -1) {
            return 0;
        }

        if (wdat == row[i]) {
            return 1;
        }
    }

    return 0;
}

static s32 meoshi_lever_matches(const PLW* wk, s16 tdat, s16 wdat) {
    if (wk->wu.cg_meoshi & 0x80) {
        return lever_row_matches(cnmc_Z_lever_data[tdat], 6, wdat);
    }

    return lever_row_matches(cnmc_z_lever_data[tdat], 8, wdat);
}

/* Outcome of the meoshi cancel gates:
 *   0 - no cancel
 *   1 - cancel, and set up the next attack
 *   2 - cancel as a target-combo continuation
 */
/* The release half of the meoshi cancel: letting the buttons go can cancel too,
 * but only when the cancel window allows it and the released buttons are a
 * subset of the ones the move wants. */
static s32 meoshi_release_cancels(PLW* wk, s16 tdat) {
    s16 wdat;

    if (!(wk->wu.cg_cancel & 0x80)) {
        return 0;
    }

    wdat = wk->cp->sw_off & 0x770;

    if (wdat & ~tdat) {
        return 0;
    }

    if (shot_data_convert(wk->cp->sw_off) < 0) {
        return 0;
    }

    if (!(wk->wu.cg_meoshi & 0x800)) {
        return 0;
    }

    return 1;
}

/* A button went down this frame: the 0x800 flag decides which kind of cancel
 * the caller reports. */
static s32 meoshi_cancel_on_press(const PLW* wk) {
    if ((wk->wu.cg_meoshi & 0x800)) {
        return 1;
    }

    return 2;
}

/* The cancel names no buttons: only the 0x800 flag lets it through. */
static s32 meoshi_cancel_without_buttons(const PLW* wk) {
    if (!(wk->wu.cg_meoshi & 0x800)) {
        return 0;
    }

    return 1;
}

static s32 meoshi_cancel_gate(PLW* wk) {
    s16 tdat;
    s16 wdat;

    if ((tdat = wk->wu.cg_meoshi & 0x770) == 0) {
        return meoshi_cancel_without_buttons(wk);
    }

    wdat = wk->cp->sw_new & 0x770;

    if (wdat & ~tdat) {
        return 0;
    }

    if (shot_data_convert(wk->cp->sw_now) >= 0) {
        return meoshi_cancel_on_press(wk);
    }

    return meoshi_release_cancels(wk, tdat);
}

static s32 commit_meoshi_cancel(PLW* wk) {
    if (wk->wu.cg_meoshi & 0x1000) {
        if (char_move_cmms3(wk) == 0) {
            return 0;
        }
    } else {
        char_move_cmms2(&wk->wu);
    }

    wk->wu.hf.hit_flag = 0;
    wk->wu.att_hit_ok = 0;
    wk->wu.meoshi_hit_flag = 0;
    wk->wu.cg_cancel &= 0x60;

    if ((wk->tc_1st_flag == 0) && (wk->wu.now_koc == 4)) {
        grade_add_target_combo(wk->wu.id);
    }

    wk->tc_1st_flag = 1;
    pp_pulpara_remake_at_init2(wk);
    return 1;
}

static s32 continue_target_combo(PLW* wk) {
    if ((wk->tc_1st_flag == 0) && wk->wu.now_koc == 4) {
        grade_add_target_combo(wk->wu.id);
    }

    check_nm_attack(wk);
    wk->tc_1st_flag = 1;
    return 1;
}

s32 check_meoshi_cancel(PLW* wk) { // 🟢
    s16 tdat;
    s16 wdat;

    wk->permited_koa |= 0x10;

    if (wk->wu.meoshi_hit_flag == 0) {
        return 0;
    }

    tdat = wk->wu.cg_meoshi & 0x8F;

    if (tdat != 0) {
        wdat = cnmc_conv_data[wk->cp->sw_new & 0xF];
        tdat &= 0xF;

        if (!meoshi_lever_matches(wk, tdat, wdat)) {
            return 0;
        }
    }

    switch (meoshi_cancel_gate(wk)) {
    case 1:
        return commit_meoshi_cancel(wk);

    case 2:
        return continue_target_combo(wk);

    default:
        return 0;
    }
}

const s16 gml_real_lever_data[16] = { 0, 6, 2, 10, 4, 0, 8, 5, 1, 9, 0, 0, 4, 8, 4, 8 };

s16 get_meoshi_lever(s16 data) { // 🟢
    return gml_real_lever_data[data & 0xF];
}

s16 get_meoshi_shot(s16 data) { // 🟢
    return ((data & 0x700) >> 1) + (data & 0x70);
}

const s16 cmdshot_conv_tbl[32] = { 16,   32,   64,   128,  256,  512,  1024, 2048, 272,  544,  1088, 2176, 0, 0, 0, 0,
                                   1904, 1904, 1904, 1920, 1904, 1904, 1904, 2160, 1904, 1904, 1904, 2176, 0, 0, 0, 0 };
