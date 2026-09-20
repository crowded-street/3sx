/**
 * @file sys_sub_em.c
 * CPU Opponent Selection
 *
 * Split out of sys_sub.c: building the list of characters the CPU may field
 * against a player, and keeping the two players' ladders apart.
 */

#include "sf33rd/Source/Game/system/sys_sub.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void Setup_Candidate_Buff(s16 PL_id);
s16 Check_EM_Buff(s16 ix, s16 ok_urien);
s32 Check_EM_Sub(s16 ix, s16 ok_urien, s16 Rnd);

u8 Candidate_Buff[16];

/* The candidate ladder both entry points build: the buffer cleared, the
 * eligible characters collected, the eight rungs filled and the mid-boss put
 * on the ninth. The two differ in one value, the rung the fill starts at. */
static void build_em_candidates(s16 PL_id, s16 first, s16 ok_urien) {
    s16 ix;

    for (ix = 0; ix < 16; ix++) {
        Candidate_Buff[ix] = 0xFF;
    }

    Setup_Candidate_Buff(PL_id);

    for (ix = first; ix < 8; ix++) {
        EM_Candidate[PL_id][0][ix] = Check_EM_Buff(ix, ok_urien);
        EM_Candidate[PL_id][1][ix] = Check_EM_Buff(ix, ok_urien);
    }

    EM_Candidate[PL_id][0][8] = Middle_Class_Boss_Data[My_char[PL_id]];
    EM_Candidate[PL_id][1][8] = Middle_Class_Boss_Data[My_char[PL_id]];
}

void Initialize_EM_Candidate(s16 PL_id) {
    s16 ok_urien = random_16();

    build_em_candidates(PL_id, 0, ok_urien);

    if (My_char[PL_id] != 0) {
        EM_Candidate[PL_id][0][9] = 0;
        EM_Candidate[PL_id][1][9] = 0;
    } else {
        EM_Candidate[PL_id][0][9] = 1;
        EM_Candidate[PL_id][1][9] = 1;
    }
}

/* The last two reasons a character is not offered: it is this character's
 * mid-boss, or the player has already broken it. Reached only when none of the
 * earlier tests fired, which is what falling through to them meant. */
static s32 candidate_is_excluded_late(s16 PL_id, s16 ix) {
    if (ix == Middle_Class_Boss_Data[My_char[PL_id]]) {
        return 1;
    }

    if (Break_Com[PL_id][ix]) {
        return 1;
    }

    return 0;
}

static s32 candidate_is_excluded(s16 PL_id, s16 ix) {
    if (My_char[PL_id] == 0 && ix == 1) {
        return 1;
    }

    if (ix == My_char[PL_id]) {
        return 1;
    }

    if (ix == 17) {
        return 1;
    }

    return candidate_is_excluded_late(PL_id, ix);
}

void Setup_Candidate_Buff(s16 PL_id) {
    s16 em;
    s16 ix;
    s16 s2;

    for (em = 0, s2 = ix = 1; ix <= 19; ix++) {
        if (candidate_is_excluded(PL_id, ix)) {
            continue;
        }

        Candidate_Buff[em] = ix;
        em++;

        if (em >= 16) {
            break;
        }
    }
}

/* Taking a slot out of the candidate buffer, the direction the scan walks in,
 * and the step that wraps it round the sixteen slots. */
static s16 claim_candidate(s16 Rnd) {
    s16 em;

    em = Candidate_Buff[Rnd];
    Candidate_Buff[Rnd] = 0xFF;
    return em;
}

static s16 pick_scan_direction() {
    s16 Next;

    Next = random_16() & 1;

    if (Next == 0) {
        Next = -1;
    }

    return Next;
}

static s16 next_candidate_slot(s16 Rnd, s16 Next) {
    Rnd += Next;

    if (Rnd < 0) {
        Rnd = 15;
    }

    if (Rnd > 15) {
        Rnd = 0;
    }

    return Rnd;
}

s16 Check_EM_Buff(s16 ix, s16 ok_urien) {
    s16 Rnd = random_16();
    s16 Next;

    if (Check_EM_Sub(ix, ok_urien, Rnd)) {
        return claim_candidate(Rnd);
    }

    Next = pick_scan_direction();

    while (1) {
        if (Check_EM_Sub(ix, ok_urien, Rnd)) {
            return claim_candidate(Rnd);
        }

        Rnd = next_candidate_slot(Rnd, Next);
    }
}

static s32 em_allowed_in_late_slot(s16 ix, s16 ok_urien, s16 em) {
    switch (em) {
    case 14:
        if (ix < 6) {
            return 0;
        }

        return 1;

    case 13:
        if (ok_urien != 0 && ix < 4) {
            return 0;
        }

        return 1;

    default:
        return 1;
    }
}

s32 Check_EM_Sub(s16 ix, s16 ok_urien, s16 Rnd) {
    s16 em;

    if (Candidate_Buff[Rnd] == 0xFF) {
        return 0;
    }

    em = Candidate_Buff[Rnd];

    switch (em) {
    case 2:
    case 11:
    case 6:
    case 8:
        if (ix < 4) {
            return 0;
        }

        return 1;

    default:
        return em_allowed_in_late_slot(ix, ok_urien, em);
    }
}

void Check_Same_CPU(s16 PL_id) {
    s16 ok_urien;

    if (VS_Index[PL_id] >= 9) {
        return;
    }

    if (Last_My_char[PL_id] == My_char[PL_id]) {
        return;
    }

    ok_urien = random_16();

    build_em_candidates(PL_id, VS_Index[PL_id], ok_urien);
}
