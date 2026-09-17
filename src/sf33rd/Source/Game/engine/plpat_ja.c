/**
 * @file plpat_ja.c
 * The jump-attack dummy-RTNM window and its cjdR duration tables.
 *
 * Split out of plpat.c, which was 917 lines and 42 functions. This group is
 * self-contained: nothing outside plpat.c ever named any of it, and the three
 * functions reached from plpat.c already had external linkage, so the split
 * widened nothing.
 */

#include "sf33rd/Source/Game/engine/plpat_internal.h"

const u8* cjdr_karaburi_table[20];
const u8* cjdr_hits_table[20];
const u8* cjdr_blocking_table[20];
const u8* cjdr_defense_table[20];

/* Every arm of both mappings says the same two things: point routine_no[2] at
 * the successor state and report that a change happened. Only the state number
 * differs, so it is the one parameter. */
static s16 set_ja_nmj_routine(WORK* wk, s16 rno) {
    wk->routine_no[2] = rno;
    return 1;
}

/* The later half of the jump-attack rno mapping. The labels are the original
 * pattern-status numbers and the helper returns the same 0 the caller started
 * with when none of them matches, so an unlisted status still changes nothing. */
static s16 ja_nmj_rno_change_rest(WORK* wk) {
    s16 rnum = 0;

    switch (wk->pat_status) {
    case 28:
        rnum = set_ja_nmj_routine(wk, 25);
        break;

    case 24:
        rnum = set_ja_nmj_routine(wk, 23);
        break;

    case 18:
        rnum = set_ja_nmj_routine(wk, 20);
        break;

    case 30:
        rnum = set_ja_nmj_routine(wk, 26);
        break;
    }

    return rnum;
}

s16 ja_nmj_rno_change(WORK* wk) { // 🟢
    s16 rnum = 0;

    switch (wk->pat_status) {
    case 20:
        rnum = set_ja_nmj_routine(wk, 21);
        break;

    case 14:
        rnum = set_ja_nmj_routine(wk, 18);
        break;

    case 26:
        rnum = set_ja_nmj_routine(wk, 24);
        break;

    case 22:
        rnum = set_ja_nmj_routine(wk, 22);
        break;

    case 16:
        rnum = set_ja_nmj_routine(wk, 19);
        break;

    default:
        rnum = ja_nmj_rno_change_rest(wk);
        break;
    }

    return rnum;
}

/* The jump-attack hitboxes: either the attack box or the catch box is up. */
static s32 ja_hitbox_active(const PLW* wk) {
    return (wk->wu.cg_ja.atix != 0) || (wk->wu.cg_ja.caix != 0);
}

/* Both boxes are down, or the attack can no longer connect - either way the
 * dummy-RTNM countdown may start. */
static s32 ja_hitbox_down_or_spent(const PLW* wk) {
    return ((wk->wu.cg_ja.atix == 0) && (wk->wu.cg_ja.caix == 0)) || !wk->wu.att_hit_ok;
}

/* A box is up again and it can still connect, so the countdown restarts. */
static s32 ja_hitbox_active_and_live(const PLW* wk) {
    return ((wk->wu.cg_ja.atix != 0) || (wk->wu.cg_ja.caix != 0)) && wk->wu.att_hit_ok;
}

/* State 2 counts the window down, unless a box came back up while the attack can
 * still connect - then the window restarts. */
static void tick_ja_dummy_countdown(PLW* wk) {
    if (ja_hitbox_active_and_live(wk)) {
        wk->ja_nmj_rno = 1;
        return;
    }

    if (!--wk->ja_nmj_cnt) {
        wk->ja_nmj_rno = 3;
    }
}

/* The expired window: a live box restarts it, and with no box at all the unit is
 * pushed into cg_type 64 so the caller's rno change can fire. */
static void hold_ja_dummy_state(PLW* wk) {
    if (ja_hitbox_active(wk)) {
        if (wk->wu.att_hit_ok) {
            wk->ja_nmj_rno = 1;
            return;
        }
    } else if (wk->wu.cg_type == 0) {
        wk->wu.cg_type = 64;
    }
}

void check_ja_nmj_dummy_RTNM(PLW* wk) { // 🟢
    if (wk->wu.xyz[1].disp.pos <= 0) {
        wk->ja_nmj_rno = 0;
        return;
    }

    switch (wk->ja_nmj_rno) {
    case 0:
        if (ja_hitbox_active(wk)) {
            wk->ja_nmj_rno = 1;
        }

        break;

    case 1:
        if (ja_hitbox_down_or_spent(wk)) {
            wk->ja_nmj_cnt = get_cjdR(wk);
            wk->ja_nmj_rno = 2;
        }

        break;

    case 2:
        tick_ja_dummy_countdown(wk);
        break;

    default:
        hold_ja_dummy_state(wk);
        break;
    }
}

/* Nothing was hit this frame: either the attack can still connect or it touched
 * nothing at all. */
static s32 ja_attack_never_landed(const PLW* wk) {
    return wk->wu.att_hit_ok || (wk->wu.hf.hit.player == 0);
}

u8 get_cjdR(PLW* wk) { // 🟢
    s16 w_ix = (wk->wu.kind_of_waza & 6);
    w_ix += ((wk->wu.hf.hit.player & 0xA2) != 0);

    if (ja_attack_never_landed(wk)) {
        return cjdr_karaburi_table[wk->player_number][w_ix];
    }

    if (wk->wu.hf.hit.player & 3) {
        return cjdr_hits_table[wk->player_number][w_ix];
    }

    if (wk->wu.hf.hit.player & 0xC0) {
        return cjdr_blocking_table[wk->player_number][w_ix];
    }

    if (wk->wu.hf.hit.player & 0x30) {
        return cjdr_defense_table[wk->player_number][w_ix];
    }

    /* The last test fell into the case0 label, so the karaburi row is what an
     * unmatched hit mask returned. */
    return cjdr_karaburi_table[wk->player_number][w_ix];
}

const u8 cjdr_karaburi_type3[8] = { 255, 255, 255, 255, 255, 255, 255, 255 };

const u8* cjdr_karaburi_table[20] = {
    cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3,
    cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3,
    cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3,
    cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3, cjdr_karaburi_type3
};

const u8 cjdr_hits_type3[8] = { 255, 255, 255, 255, 255, 255, 255, 255 };

const u8* cjdr_hits_table[20] = {
    cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3,
    cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3,
    cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3,
    cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3, cjdr_hits_type3,
};

const u8 cjdr_blocking_type0[8] = { 16, 7, 18, 9, 20, 11, 20, 11 };
const u8 cjdr_blocking_type1[8] = { 17, 8, 19, 10, 21, 12, 21, 12 };
const u8 cjdr_blocking_type2[8] = { 18, 9, 20, 11, 22, 13, 22, 13 };

const u8* cjdr_blocking_table[20] = {
    cjdr_blocking_type0, cjdr_blocking_type1, cjdr_blocking_type1, cjdr_blocking_type1, cjdr_blocking_type0,
    cjdr_blocking_type2, cjdr_blocking_type1, cjdr_blocking_type0, cjdr_blocking_type1, cjdr_blocking_type1,
    cjdr_blocking_type1, cjdr_blocking_type0, cjdr_blocking_type1, cjdr_blocking_type2, cjdr_blocking_type0,
    cjdr_blocking_type1, cjdr_blocking_type1, cjdr_blocking_type1, cjdr_blocking_type2, cjdr_blocking_type1
};

const u8 cjdr_defense_type3[8] = { 255, 255, 255, 255, 255, 255, 255, 255 };

const u8* cjdr_defense_table[20] = { cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3,
                                     cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3,
                                     cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3,
                                     cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3,
                                     cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3, cjdr_defense_type3 };
