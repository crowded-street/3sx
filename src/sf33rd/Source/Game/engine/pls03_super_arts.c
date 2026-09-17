/**
 * @file pls03_super_arts.c
 * Super Art selection and execution.
 *
 * Split out of pls03.c, which was 1326 lines. These six functions are reached
 * only through pls03.h and call nothing else in pls03.c but the two helpers
 * declared in pls03_internal.h.
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


/// Check EX SA attack
/* The caller asked for a slot that must already be armed, and it is not. */
static s32 slot_needs_arming_and_is_not(const PLW* wk, u8 slot_ix, s8 always) {
    return always && !(wk->cp->btix[slot_ix] & 0x100);
}

/* The chain-cancel record says this slot has already been used this chain, and
 * the DIP switch that enforces that is set. */
static s32 chain_cancel_already_used(const PLW* wk, u8 slot_ix) {
    return (wk->spmv_ng_flag2 & DIP2_UNKNOWN_23) && chainex_check[wk->wu.id][slot_ix - 20];
}

/* The gates a grounded EX super-art slot must pass before its command is even
 * looked at. Returns 1 wherever the original returned 0 without firing. */
static s32 grounded_ex_slot_is_blocked(PLW* wk, u8 slot_ix, s8 always) {
    if (wk->spmv_ng_flag & DIP_UNKNOWN_30) {
        return 1;
    }

    if (slot_ix == 0) {
        return 1;
    }

    if (slot_ix > 0x1C) {
        return 1;
    }

    if (slot_needs_arming_and_is_not(wk, slot_ix, always)) {
        return 1;
    }

    if (chain_cancel_already_used(wk, slot_ix)) {
        return 1;
    }

    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 0x40;
    }

    return is_blocked_by_arcade_switch(wk, slot_ix);
}

/* One grounded EX super-art slot: every gate it must pass, then the
 * command match that fires it. Returns 1 when the art started.
 *
 * check_full_gauge_attack and check_full_gauge_attack2 were byte-identical
 * apart from which slot field they read, so the slot index is the parameter -
 * u8, the field's own type - and each caller passes its own field. */
/* The same for the grounded EX super. It stays separate from the airborne
 * one: the table it reaches into and the offset within it are both different. */
/* Once an EX strength matches, both the grounded and the airborne path finish
 * identically: drop the cancel, mark the meter spent, set the union up from that
 * strength's command entry, and - outside arcade balance - record the chain-EX
 * use. The `- 20` is what both wrote, airborne included, even though its table
 * lookup above uses 38; that asymmetry is the original's and is left alone. */
static s32 launch_ex_strength(PLW* wk, u8 slot_ix, s16 j) {
    wk->wu.cg_cancel = 0;
    wk->sa->mp = -1;
    hissatsu_setup_union(wk, wk->cp->waza_r[slot_ix][j]);
    waza_compel_all_init2(wk);

    if (!ArcadeBalance_IsEnabled()) {
        chainex_check[wk->wu.id][slot_ix - 20] = 1;
        chainex_spat_cancel_kidou(&wk->wu);
    }

    return 1;
}

static s32 try_grounded_ex_strengths(PLW* wk, u8 slot_ix, u16 cusw) {
    s16 j;
    u16 exsw;

    for (j = 3; j >= 0; j--) {
        if ((j == 3) && !(wk->cp->btix[slot_ix] & 0x600)) {
            continue;
        }

        exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j]];

        if (exsw == cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j] & 0xF]) {
            setup_comm_back(&wk->wu);

            if (ArcadeBalance_IsEnabled()) {
                wk->as = &asstbl_lv_9900_g_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                                 [j + (slot_ix - 20) * 4];
            } else {
                wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(slot_ix)]
                                         [j + (slot_ix - 20) * 4];
            }

            return launch_ex_strength(wk, slot_ix, j);
        }
    }

    return 0;
}

static s32 try_grounded_ex_super(PLW* wk, u8 slot_ix, s8 always) {
    u16* conpane;
    u16 cusw;

    if (grounded_ex_slot_is_blocked(wk, slot_ix, always)) {
        return 0;
    }

    conpane = &wk->cp->sw_lvbt;

    if (wk->cp->waza_flag[slot_ix] == -1) {
        return 0;
    }

    if (((wk->cp->btix[slot_ix] & 0xFF) != 0x80) && wk->cp->waza_flag[slot_ix]) {
        cusw = conpane[wk->cp->btix[slot_ix] & 0xFF];

        return try_grounded_ex_strengths(wk, slot_ix, cusw);
    }

    return 0;
}

/* One airborne EX super-art slot. Not shared with the grounded version:
 * that one tests `> 0x1C` where this tests `< 0x1C`, and indexes its table
 * from 20 rather than 38. A comparison operator and an offset are not
 * values Recipe D may parameterise.
 *
 * check_full_gauge_attack and check_full_gauge_attack2 were byte-identical
 * apart from which slot field they read, so the slot index is the parameter -
 * u8, the field's own type - and each caller passes its own field. */
/* Try each strength of the airborne EX super in turn, strongest first, and
 * fire the first one whose buttons are all held. */
static s32 try_airborne_ex_strengths(PLW* wk, u8 slot_ix, u16 cusw) {
    s16 j;
    u16 exsw;

    for (j = 3; j >= 0; j--) {
        if ((j == 3) && !(wk->cp->btix[slot_ix] & 0x600)) {
            continue;
        }

        exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j]];

        if (exsw == cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j] & 0xF]) {
            setup_comm_back(&wk->wu);

            if (ArcadeBalance_IsEnabled()) {
                wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                                 [j + (slot_ix - 38) * 4];
            } else {
                wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(slot_ix)]
                                         [j + (slot_ix - 38) * 4];
            }

            return launch_ex_strength(wk, slot_ix, j);
        }
    }

    return 0;
}

static s32 try_airborne_ex_super(PLW* wk, u8 slot_ix, s8 always) {
    u16* conpane;
    u16 cusw;

    if (wk->spmv_ng_flag & DIP_UNKNOWN_31) {
        return 0;
    }

    if (slot_ix == 0) {
        return 0;
    }

    if (slot_ix < 0x1C) {
        return 0;
    }

    if (slot_needs_arming_and_is_not(wk, slot_ix, always)) {
        return 0;
    }

    if (chain_cancel_already_used(wk, slot_ix)) {
        return 0;
    }

    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 0x40;
    }

    if (is_blocked_by_arcade_switch(wk, slot_ix)) {
        return 0;
    }

    conpane = &wk->cp->sw_lvbt;

    if (wk->cp->waza_flag[slot_ix] == -1) {
        return 0;
    }

    if (((wk->cp->btix[slot_ix] & 0xFF) != 0x80) && wk->cp->waza_flag[slot_ix]) {
        cusw = conpane[wk->cp->btix[slot_ix] & 0xFF];

        return try_airborne_ex_strengths(wk, slot_ix, cusw);
    }

    return 0;
}

/* Neither full-gauge attack can start unless the meter is exactly full and the
 * debug pause is off. Both wrote these two guards out identically. */
static s32 full_gauge_attack_is_blocked(const PLW* wk) {
    if (wk->sa->mp != 1) {
        return 1;
    }

    if (pcon_dp_flag) {
        return 1;
    }

    return 0;
}

s32 check_full_gauge_attack(PLW* wk, s8 always) {
    if (full_gauge_attack_is_blocked(wk)) {
        return 0;
    }

    if (player_is_grounded_or_on_car(wk)) {
        return try_grounded_ex_super(wk, wk->sa->exsa_g_ix, always);
    }

    return try_airborne_ex_super(wk, wk->sa->exsa_a_ix, always);
}

s32 check_full_gauge_attack2(PLW* wk, s8 always) {
    if (full_gauge_attack_is_blocked(wk)) {
        return 0;
    }

    if (player_is_grounded_or_on_car(wk)) {
        return try_grounded_ex_super(wk, wk->sa->exs2_g_ix, always);
    }

    return try_airborne_ex_super(wk, wk->sa->exs2_a_ix, always);
}

/* With the arts selectable, try each of the three in turn and keep the first
 * that fires. Super_Arts and the DC status are set for each attempt, and both
 * are left pointing at whichever art was tried last - the original does the
 * same, so a failed sweep leaves the third art selected. */
static s16 try_each_selectable_art(PLW* wk) {
    s16 rnum = 0;
    s16 i;

    for (i = 0; i < 3; i++) {
        Super_Arts[wk->wu.id] = i;
        set_super_arts_status_dc(wk->wu.id);
        rnum = check_super_arts_attack_dc(wk);

        if (rnum) {
            wk->sa->gt2 = wk->sa->gauge_type;
            break;
        }
    }

    return rnum;
}

s16 check_super_arts_attack(PLW* wk) { // 🟡
    s16 rnum = 0;

    if (cmd_sel[wk->wu.id]) {
        if (wk->sa->ok != -1) {
            rnum = try_each_selectable_art(wk);
        }
    } else {
        rnum = check_super_arts_attack_dc(wk);
    }

    return rnum;
}

/* The slot carries neither of the two bits that make it an EX-capable art. */
static s32 slot_has_no_ex_bits(const PLW* wk, s16 ix) {
    return !(wk->cp->btix[ix] & 0x600);
}

/* The fourth EX art is charged but the max-gauge art is not the one armed. */
static s32 fourth_ex_art_is_locked(const PLW* wk) {
    return wk->sa->ex4th_full && (wk->sa->mp != 1);
}

/* Outside arcade balance, the fourth strength is skipped either because the slot
 * has no EX bits at all or because the fourth EX art is charged while the
 * max-gauge art is not the one armed. */
static s32 ex_strength_is_unavailable(const PLW* wk, s16 ix, s16 j) {
    return (j == 3) && (slot_has_no_ex_bits(wk, ix) || fourth_ex_art_is_locked(wk));
}

static bool should_skip_dc_slot(PLW* wk, s16 ix, s16 j) {
    if (ArcadeBalance_IsEnabled()) {
        return (j == 3) && slot_has_no_ex_bits(wk, ix);
    }

    return ex_strength_is_unavailable(wk, ix, j);
}

/* The gates a grounded double-cancel super-art slot must pass. Returns 1
 * wherever check_super_arts_attack_dc returned 0 without firing.
 *
 * The airborne arm keeps its own copy inline. The two differ in `>` against `<`
 * and in which table they index, so Recipe D may not merge them, and extracting
 * from both would only trade one flagged function for a twin pair - see
 * "Between two twin arms, extract from one of them only" in the playbook. */
static s32 grounded_dc_slot_is_blocked(PLW* wk) {
    if (wk->spmv_ng_flag & DIP_UNKNOWN_30) {
        return 1;
    }

    if (wk->sa->nmsa_g_ix == 0) {
        return 1;
    }

    if (wk->sa->nmsa_g_ix > 0x1C) {
        return 1;
    }

    if ((wk->spmv_ng_flag2 & DIP2_UNKNOWN_23) && chainex_check[wk->wu.id][wk->sa->nmsa_g_ix - 20]) {
        return 1;
    }

    if (is_blocked_by_arcade_switch(wk, wk->sa->nmsa_g_ix)) {
        return 1;
    }

    return wk->cp->waza_flag[wk->sa->nmsa_g_ix] == -1;
}

/* The four button strengths of the grounded double-cancel slot, strongest
 * first. Returns 1 when one of them fired, which is where
 * check_super_arts_attack_dc returned 1.
 *
 * The airborne arm keeps its own copy: it indexes its table from 38 rather than
 * 20 and reads a different slot field, so Recipe D cannot merge them and
 * extracting from both would create a twin pair. */
static s32 try_grounded_dc_strengths(PLW* wk, u16 cusw) {
    s16 j;
    u16 exsw;

    for (j = 3; j >= 0; j--) {
        if (should_skip_dc_slot(wk, wk->sa->nmsa_g_ix, j)) {
            continue;
        }

        exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_g_ix][j]];

        if (exsw == cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_g_ix][j] & 0xF]) {
            setup_comm_back(&wk->wu);

            if (ArcadeBalance_IsEnabled()) {
                wk->as = &asstbl_lv_9900_g_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                                 [j + (wk->sa->nmsa_g_ix - 20) * 4];
            } else {
                wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_g_ix)]
                                         [j + (wk->sa->nmsa_g_ix - 20) * 4];
                wk->sa->ex4th_exec = (j == 3) * wk->sa->ex4th_full;
            }

            wk->wu.cg_cancel = 0;
            wk->sa->ok = -1;
            hissatsu_setup_union(wk, wk->cp->waza_r[wk->sa->nmsa_g_ix][j]);
            waza_compel_all_init2(wk);

            if (!ArcadeBalance_IsEnabled()) {
                chainex_check[wk->wu.id][wk->sa->nmsa_g_ix - 20] = 1;
                chainex_spat_cancel_kidou(&wk->wu);
            }

            return 1;
        }
    }

    return 0;
}

/* The airborne mirror of try_grounded_dc_strengths. Kept separate: it reads
 * nmsa_a_ix and indexes its table from 38, which is two differences, so Recipe D
 * cannot merge the pair. Extracting both was measured against extracting one -
 * see the commit - and both won here because it brought the parent under the
 * threshold. */
/* Fire the airborne direct cancel at strength j: the art it names, the
 * cancel state it leaves behind, and the chain-EX bookkeeping the console
 * rules add. */
static void fire_airborne_dc(PLW* wk, s16 j) {
    setup_comm_back(&wk->wu);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                         [j + (wk->sa->nmsa_a_ix - 38) * 4];
    } else {
        wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_a_ix)]
                                 [j + (wk->sa->nmsa_a_ix - 38) * 4];
        wk->sa->ex4th_exec = (j == 3) * wk->sa->ex4th_full;
    }

    wk->wu.cg_cancel = 0;
    wk->sa->ok = -1;
    hissatsu_setup_union(wk, wk->cp->waza_r[wk->sa->nmsa_a_ix][j]);
    waza_compel_all_init2(wk);

    if (!ArcadeBalance_IsEnabled()) {
        chainex_check[wk->wu.id][wk->sa->nmsa_a_ix - 20] = 1;
        chainex_spat_cancel_kidou(&wk->wu);
    }
}

static s32 try_airborne_dc_strengths(PLW* wk, u16 cusw) {
    s16 j;
    u16 exsw;

    for (j = 3; j >= 0; j--) {
        if (should_skip_dc_slot(wk, wk->sa->nmsa_a_ix, j)) {
            continue;
        }

        exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_a_ix][j]];

        if (exsw == cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_a_ix][j] & 0xF]) {
            fire_airborne_dc(wk, j);
            return 1;
        }
    }

    return 0;
}

/* The grounded direct cancel: the slot has to be free, and the command's own
 * button group has to be one this art reads. */
static s32 try_grounded_dc(PLW* wk) {
    u16 cusw;
    u16* conpane;

    if (grounded_dc_slot_is_blocked(wk)) {
        return 0;
    }

    conpane = &wk->cp->sw_lvbt;

    if (((wk->cp->btix[wk->sa->nmsa_g_ix] & 0xFF) != 0x80) && wk->cp->waza_flag[wk->sa->nmsa_g_ix]) {
        cusw = conpane[wk->cp->btix[wk->sa->nmsa_g_ix] & 0xFF];

        if (try_grounded_dc_strengths(wk, cusw)) {
            return 1;
        }
    }

    return 0;
}

/* The airborne one, which has four more gates of its own before the same
 * button-group test. */
/* The five reasons an airborne direct cancel cannot start, in the order the
 * original tested them. */
static s32 airborne_dc_slot_is_blocked(const PLW* wk) {
    if (wk->spmv_ng_flag & DIP_UNKNOWN_31) {
        return 1;
    }

    if (wk->sa->nmsa_a_ix == 0) {
        return 1;
    }

    if (wk->sa->nmsa_a_ix < 0x1C) {
        return 1;
    }

    if (chain_cancel_already_used(wk, wk->sa->nmsa_a_ix)) {
        return 1;
    }

    return is_blocked_by_arcade_switch(wk, wk->sa->nmsa_a_ix);
}

static s32 try_airborne_dc(PLW* wk) {
    u16 cusw;
    u16* conpane;

    if (airborne_dc_slot_is_blocked(wk)) {
        return 0;
    }

    conpane = &wk->cp->sw_lvbt;

    if (wk->cp->waza_flag[wk->sa->nmsa_a_ix] == -1) {
        return 0;
    }

    if (((wk->cp->btix[wk->sa->nmsa_a_ix] & 0xFF) != 0x80) && (wk->cp->waza_flag[wk->sa->nmsa_a_ix])) {
        cusw = conpane[wk->cp->btix[wk->sa->nmsa_a_ix] & 0xFF];

        if (try_airborne_dc_strengths(wk, cusw)) {
            return 1;
        }
    }

    return 0;
}

s32 check_super_arts_attack_dc(PLW* wk) { // 🟡
    s16 j;
    u16 exsw;

    if (wk->sa->ok != 1) {
        return 0;
    }

    if (pcon_dp_flag) {
        return 0;
    }

    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 1;
    }

    if (player_is_grounded_or_on_car(wk)) {
        return try_grounded_dc(wk);
    }

    return try_airborne_dc(wk);
}

/* The gates a grounded super art must pass before it starts. The airborne arm
 * keeps its own copy inline: it tests a different DIP switch and `<` rather
 * than `>`, so the two cannot be merged, and extracting from both would only
 * create a twin pair. */
static s32 grounded_art_is_blocked(PLW* wk) {
    if (wk->spmv_ng_flag & DIP_UNKNOWN_30) {
        return 1;
    }

    if (wk->sa->ok != 1) {
        return 1;
    }

    if (wk->sa->nmsa_g_ix > 0x1C) {
        return 1;
    }

    return is_blocked_by_arcade_switch(wk, wk->sa->nmsa_g_ix);
}

/* Both super arts finish the same way: drop the current cancel, mark the art
 * spent, set the union up from the slot's first command entry, and - outside
 * arcade balance - latch the gauge type. Only the slot index differs, which is
 * the one value Recipe D allows as a parameter. */
static s32 launch_super_art(PLW* wk, s16 slot_ix) {
    wk->wu.cg_cancel = 0;
    wk->sa->ok = -1;
    hissatsu_setup_union(wk, wk->cp->waza_r[slot_ix][0]);
    waza_compel_all_init2(wk);

    if (!ArcadeBalance_IsEnabled()) {
        wk->sa->gt2 = wk->sa->gauge_type;
    }

    return 1;
}

/* Starting a grounded super art once its gates have passed. */
static s32 start_grounded_super_art(PLW* wk) {
    if (grounded_art_is_blocked(wk)) {
        return 0;
    }

    setup_comm_back(&wk->wu);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_9900_g_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][(wk->sa->nmsa_g_ix - 20) * 4];
    } else {
        wk->as = _assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_g_ix)] + (wk->sa->nmsa_g_ix - 20) * 4;
        wk->sa->ex4th_exec = 0;
    }

    return launch_super_art(wk, wk->sa->nmsa_g_ix);
}

/* The airborne equivalent. Its gates are still inline here where the grounded
 * side has them in a helper, because the two gate chains differ in `<` against
 * `>` and in which slot field they read, and naming both was measured worse. */
static s32 start_airborne_super_art(PLW* wk) {
    if (wk->spmv_ng_flag & DIP_UNKNOWN_31) {
        return 0;
    }

    if (wk->sa->ok != 1) {
        return 0;
    }

    if (wk->sa->nmsa_a_ix < 0x1C) {
        return 0;
    }

    if (is_blocked_by_arcade_switch(wk, wk->sa->nmsa_a_ix)) {
        return 0;
    }

    setup_comm_back(&wk->wu);

    if (ArcadeBalance_IsEnabled()) {
        wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][(wk->sa->nmsa_a_ix - 38) * 4];
    } else {
        wk->as = _assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_a_ix)] + (wk->sa->nmsa_a_ix - 38) * 4;
        wk->sa->ex4th_exec = 0;
    }

    return launch_super_art(wk, wk->sa->nmsa_a_ix);
}

s32 execute_super_arts(PLW* wk) { // 🟡
    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 1;
    }

    if ((wk->sa->gauge_type != 3) && pcon_dp_flag) {
        return 0;
    }

    if (player_is_grounded_or_on_car(wk)) {
        return start_grounded_super_art(wk);
    }

    return start_airborne_super_art(wk);
}
