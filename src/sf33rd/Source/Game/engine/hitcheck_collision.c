/**
 * @file hitcheck_collision.c
 * Catch, attack-box, hit-geometry, and hit-queue processing.
 */

#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "arcade/arcade_balance.h"
#include "bin2obj/exchange.h"
#include "bin2obj/gauge.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff02.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitefef.h"
#include "sf33rd/Source/Game/engine/hitefpl.h"
#include "sf33rd/Source/Game/engine/hitplef.h"
#include "sf33rd/Source/Game/engine/hitplpl.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/system/sysdir.h"

#include <SDL3/SDL.h>

static bool should_skip_normal_catch(WORK* target) {
    return (target->routine_no[1] == 1) && (target->routine_no[3] != 0) && (target->routine_no[2] != 0x19);
}

static bool should_skip_oiuchi_catch(WORK* target) {
    return (target->routine_no[1] == 1) && (target->routine_no[3] != 0) && (target->cg_type != 10) &&
           !dm_oiuchi_catch[target->routine_no[2]];
}

static bool should_skip_catch_target(WORK* mad, WORK* sad) {
    if (mad->att.guard & 0x18) {
        return false;
    }

    if (((PLW*)sad)->tsukamarenai_flag) {
        return true;
    }

    if (!(mad->att.dipsw & 0x60)) {
        return should_skip_normal_catch(sad);
    }

    return should_skip_oiuchi_catch(sad);
}

typedef struct {
    WORK* attacker;
    WORK* target;
    s16* attack_box;
    s16* target_box;
    s16 attacker_index;
    s16 target_index;
} CatchScan;

static bool prepare_catch_attacker(CatchScan* scan, s16 attacker_index) {
    scan->attacker_index = attacker_index;

    if (hs[attacker_index].flag.results & 0x1000) {
        return false;
    }

    scan->attacker = q_hit_push[attacker_index];

    if (scan->attacker->work_id != 1) {
        return false;
    }

    if (scan->attacker->att_hit_ok == 0) {
        return false;
    }

    scan->attack_box = &scan->attacker->h_cat->cat_box[0];
    return scan->attack_box[1] != 0;
}

static bool prepare_catch_target(CatchScan* scan, s16 target_index) {
    scan->target_index = target_index;

    if (target_index == scan->attacker_index) {
        return false;
    }

    if (hs[target_index].flag.results & 0x100) {
        return false;
    }

    scan->target = q_hit_push[target_index];

    if (scan->target->work_id != 1) {
        return false;
    }

    scan->target_box = &scan->target->h_cau->cau_box[0];

    if (scan->target_box[1] == 0) {
        return false;
    }

    return !should_skip_catch_target(scan->attacker, scan->target);
}

static bool register_catch_hit(CatchScan* scan) {
    if (!hit_check_subroutine(scan->attacker, scan->target, scan->attack_box, scan->target_box)) {
        return false;
    }

    hs[scan->attacker_index].flag.results |= 0x1000;
    hs[scan->attacker_index].my_hit = (u16)scan->target_index;
    hs[scan->target_index].flag.results |= 0x100;
    hs[scan->target_index].dm_me = (u16)scan->attacker_index;
    scan->attacker->att_hit_ok = 0;
    hs[scan->attacker_index].ah = scan->attack_box;
    hs[scan->target_index].dh = scan->target_box;
    scan->attacker->att_hit_ok = 0;
    return true;
}

static void scan_catch_targets(CatchScan* scan) {
    s16 si;

    for (si = 0; si < hpq_in; si++) {
        if (!prepare_catch_target(scan, si)) {
            continue;
        }

        if (!register_catch_hit(scan)) {
            continue;
        }

        break;
    }
}

void catch_hit_check() { // 🟢
    CatchScan scan;
    s16 mi;

    for (mi = 0; mi < hpq_in; mi++) {
        if (!prepare_catch_attacker(&scan, mi)) {
            continue;
        }

        scan_catch_targets(&scan);
    }
}

static bool is_same_owner_target(WORK* mad, WORK* sad) {
    if (mad->work_id == 1) {
        if (sad->work_id == 1) {
            return false;
        }

        if (((WORK_Other*)sad)->refrected != 0) {
            return false;
        }

        return mad->id == ((WORK_Other*)sad)->master_id;
    }

    if (sad->work_id == 1) {
        return ((WORK_Other*)mad)->master_id == sad->id;
    }

    return ((WORK_Other*)mad)->master_id == ((WORK_Other*)sad)->master_id;
}

static bool uses_vs_id_filter(WORK* mad, WORK* sad) {
    if (!(mad->att.dipsw & 2)) {
        return true;
    }

    if (sad->att.dipsw & 2) {
        return false;
    }

    if (sad->work_id == 1) {
        return true;
    }

    return !((WORK_Other*)sad)->refrected;
}

static bool can_vs_id_block_target(WORK* mad, WORK* sad) {
    bool target_uses_dipswitch = sad->att.dipsw & 2;

    if (mad->work_id == 1) {
        return !target_uses_dipswitch;
    }

    if (mad->work_id == 8) {
        return !target_uses_dipswitch;
    }

    return true;
}

static bool is_blocked_by_vs_id_filter(WORK* mad, WORK* sad) {
    if (!uses_vs_id_filter(mad, sad)) {
        return false;
    }

    if (!can_vs_id_block_target(mad, sad)) {
        return false;
    }

    return !(mad->vs_id & sad->work_id);
}

typedef struct {
    WORK* attacker;
    WORK* target;
    s16 attacker_index;
    s16 target_index;
} HitScan;

static bool is_excluded_attack_damage_box_pair(s16 attack_box, s16 damage_box) {
    return (attack_box == 2 || attack_box == 3) && (damage_box == 8 || damage_box == 9);
}

static bool is_damage_box_on_wrong_side(const HitScan* scan) {
    if (((scan->attacker->rl_flag) + (scan->target->rl_flag)) & 1) {
        return false;
    }

    if (scan->attacker->rl_flag) {
        return !(scan->attacker->xyz[0].disp.pos <= scan->target->xyz[0].disp.pos);
    }

    return !(scan->attacker->xyz[0].disp.pos >= scan->target->xyz[0].disp.pos);
}

static bool is_auxiliary_damage_box_blocked(const HitScan* scan, s16 damage_box) {
    return scan->attacker->att.dipsw & 4 && (damage_box >= 8 || scan->target->cg_ja.bhix == 0);
}

static bool should_skip_auxiliary_damage_box(const HitScan* scan, s16 damage_box) {
    return ((damage_box > 3) && (damage_box < 0xA)) &&
           (is_damage_box_on_wrong_side(scan) || is_auxiliary_damage_box_blocked(scan, damage_box));
}

static bool is_push_damage_box_blocked(const HitScan* scan, s16 damage_box) {
    if (damage_box != 10) {
        return false;
    }

    return !(scan->attacker->att.dipsw & 64) || scan->target->kind_of_waza & 0x60 || pcon_dp_flag ||
           scan->target->pat_status == 0x26;
}

static bool should_end_damage_box_scan(const HitScan* scan, s16 damage_box) {
    return damage_box > 3 && scan->attacker->att_hit_ok == 0;
}

static bool should_skip_damage_box(const HitScan* scan, s16 attack_box, s16 damage_box) {
    return dmdat_adrs[damage_box][1] == 0 || is_excluded_attack_damage_box_pair(attack_box, damage_box) ||
           should_skip_auxiliary_damage_box(scan, damage_box) || is_push_damage_box_blocked(scan, damage_box);
}

static void record_hit_if_stronger(const HitScan* scan, s16* attack_box_data, s16 attack_box, s16 damage_box) {
    s16 overlap = hit_check_subroutine(scan->attacker, scan->target, attack_box_data, dmdat_adrs[damage_box]);

    if (overlap > mkm_wk[scan->target_index]) {
        hs[scan->attacker_index].flag.results |= 0x10;
        hs[scan->attacker_index].my_hit = scan->target_index;
        hs[scan->attacker_index].my_att = attack_box;
        hs[scan->target_index].flag.results |= 1;
        hs[scan->target_index].dm_me = scan->attacker_index;
        hs[scan->target_index].dm_body = damage_box;
        scan->attacker->att_hit_ok = 0;
        mkm_wk[scan->target_index] = overlap;
        hs[scan->attacker_index].ah = attack_box_data;
        hs[scan->target_index].dh = dmdat_adrs[damage_box];
    }
}

static bool check_attack_box_against_damage_boxes(const HitScan* scan, s16* mh, s16 lp) {
    s16 lp2;

    for (lp2 = 0; lp2 < 11; lp2++) {
        if (should_end_damage_box_scan(scan, lp2)) {
            return false;
        }

        if (should_skip_damage_box(scan, lp, lp2)) {
            continue;
        }

        record_hit_if_stronger(scan, mh, lp, lp2);
    }

    return true;
}

static bool should_skip_attacker(WORK* attacker, WORK* target) {
    return attacker->cg_ja.atix == 0 || attacker->att_hit_ok == 0 || is_blocked_by_vs_id_filter(attacker, target) ||
           is_same_owner_target(attacker, target);
}

static void check_attacker_against_target(WORK* sad, s16 mi, s16 si) {
    WORK* mad;
    HitScan scan;
    s16* mh;
    s16 lp;
    s16* assign2;

    if (mi == si) {
        return;
    }

    if (hs[mi].flag.results & 0x1110) {
        return;
    }

    mad = q_hit_push[mi];

    if (should_skip_attacker(mad, sad)) {
        return;
    }

    scan.attacker = mad;
    scan.target = sad;
    scan.attacker_index = mi;
    scan.target_index = si;
    mh = &mad->h_att->att_box[0][0];

    for (lp = 0; lp < 4; lp++, assign2 = mh += 4) {
        if (mh[1] == 0) {
            continue;
        }

        if (!check_attack_box_against_damage_boxes(&scan, mh, lp)) {
            return;
        }
    }
}

void attack_hit_check() { // 🟢
    WORK* sad;
    s16* mh;
    s16* sh;
    s16 mi;
    s16 si;
    s16 lp;

    s16* assign1;

    for (si = 0; si < hpq_in; si++) {
        if (hs[si].flag.results & 0x1101) {
            continue;
        }

        sad = q_hit_push[si];
        sh = sad->h_bod->body_dm[0];
        mh = sad->h_han->hand_dm[0];

        for (lp = 0; lp < 4; lp++, sh += 4, assign1 = mh += 4) {
            dmdat_adrs[lp] = sh;
            dmdat_adrs[lp + 4] = mh;
        }

        dmdat_adrs[8] = sad->h_att->att_box[2];
        dmdat_adrs[9] = sad->h_att->att_box[3];
        dmdat_adrs[10] = sad->h_hos->hos_box;

        for (mi = 0; mi < hpq_in; mi++) {
            check_attacker_against_target(sad, mi, si);
        }
    }
}

s16 hit_check_subroutine(WORK* wk1, WORK* wk2, const s16* hd1, const s16* hd2) { // 🟢
    s16 d0;
    s16 d1;
    s16 d2;
    s16 d3;

    d0 = *hd1++;
    d1 = *hd1++;

    if (wk1->rl_flag) {
        d0 = -d0;
        d0 -= d1;
    }

    d0 += wk1->xyz[0].disp.pos;
    d2 = *hd2++;
    d3 = *hd2++;

    if (wk2->rl_flag) {
        d2 = -d2;
        d2 -= d3;
    }

    d2 += wk2->xyz[0].disp.pos;
    d2 += d3 - d0;
    d3 += d1;

    if ((u32)d2 >= d3) {
        return 0;
    }

    d0 = (wk1->xyz[1].disp.pos + *hd1++) - (wk2->xyz[1].disp.pos + *hd2++);
    d0 += d1 = *hd1;
    d1 += *hd2;

    if ((u32)d0 >= d1) {
        return 0;
    }

    if (d2 > (d3 - d2)) {
        d2 = d3 - d2;
    }

    return d2;
}

s32 hit_check_x_only(WORK* wk1, WORK* wk2, s16* hd1, s16* hd2) { // 🟢
    s16 d0;
    s16 d1;
    s16 d2;
    s16 d3;

    d0 = *hd1++;
    d1 = *hd1++;

    if (wk1->rl_flag) {
        d0 = -d0;
        d0 -= d1;
    }

    d0 += wk1->xyz[0].disp.pos;
    d2 = *hd2++;
    d3 = *hd2++;

    if (wk2->rl_flag) {
        d2 = -d2;
        d2 -= d3;
    }

    d2 += wk2->xyz[0].disp.pos;
    d2 += d3 - d0;
    d3 += d1;

    if ((u32)d2 >= d3) {
        return 0;
    }

    return 1;
}

void cal_hit_mark_position(WORK* wk1, WORK* wk2, s16* hd1, s16* hd2) { // 🟢
    s16 d0 = *hd1++;
    s16 d1 = *hd1++;
    s16 d2;
    s16 d3;

    if (wk1->rl_flag) {
        d0 = -d0;
        d0 -= d1;
    }

    d0 += wk1->xyz[0].disp.pos;
    d1 += d0;
    d2 = *hd2++;
    d3 = *hd2++;

    if (wk2->rl_flag) {
        d2 = -d2;
        d2 -= d3;
    }

    d2 += wk2->xyz[0].disp.pos;
    d3 += d2;

    if (d0 < d2) {
        d0 = d2;
    }

    if (d1 > d3) {
        d1 = d3;
    }

    wk2->hit_mark_x = (d0 + d1) >> 1;

    d0 = wk1->xyz[1].disp.pos + *hd1++;
    d1 = *hd1 + d0;
    d2 = wk2->xyz[1].disp.pos + *hd2++;
    d3 = *hd2 + d2;

    if (d0 < d2) {
        d0 = d2;
    }

    if (d1 > d3) {
        d1 = d3;
    }

    wk2->hit_mark_y = (d0 + d1) >> 1;
}

void get_target_att_position(WORK* wk, s16* tx, s16* ty) { // 🟢
    s16 i;
    s16(*ta)[4];

    *tx = wk->xyz[0].disp.pos;
    *ty = wk->xyz[1].disp.pos;
    ta = &wk->h_att->att_box[0];

    for (i = 0; i < 3; ta++, i++) {
        if (!ta[0][0]) {
            continue;
        }

        if (wk->rl_flag) {
            *tx -= ta[0][0] + (ta[0][1] / 2);
        } else {
            *tx += ta[0][0] + (ta[0][1] / 2);
        }

        *ty += ta[0][2] + (ta[0][3] / 2);
        break;
    }
}

static s16 adjust_att_head_position(WORK* wk, s16 tx, s16 offset) {
    s16 kx;

    if (wk->rl_flag) {
        kx = tx - offset;

        if (tx < kx) {
            return kx;
        }

        return tx;
    }

    kx = tx + offset;

    if (tx > kx) {
        return kx;
    }

    return tx;
}

s16 get_att_head_position(WORK* wk) { // 🟢
    s16* ta;
    s16 tx;
    s16 i;

    tx = wk->xyz[0].disp.pos;

    if (wk->cg_ja.atix == 0) {
        return tx;
    }

    ta = &wk->h_att->att_box[0][0];

    for (i = 0; i < 3; i++) {
        if (*ta) {
            return adjust_att_head_position(wk, tx, *ta);
        }

        ta += 4;
    }

    return tx;
}

void hit_push_request(WORK* hpr_wk) { // 🟢
    if (hpq_in < 31 && hpr_wk->cg_hit_ix != 0) {
        q_hit_push[hpq_in++] = hpr_wk;
    }
}

void clear_hit_queue() { // 🟢
    s16 i;

    hpq_in = 0;

    for (i = 0; i < 32; i++) {
        mkm_wk[i] = 0;
    }

    for (i = 0; i < 32; i++) {
        q_hit_push[i] = 0;
    }

    SDL_zeroa(hs);
}

static bool is_reversed_gill(PLW* as) {
    if (as->wu.work_id != 1) {
        return false;
    }

    if (as->player_number != CHAR_GILL) {
        return false;
    }

    return as->wu.rl_flag;
}

static u16 apply_flame_damage_attribute(PLW* as, u16 ix) {
    if (is_reversed_gill(as)) {
        ix = attr_freeze_tbl[ix - 32];
        as->wu.at_attribute = 3;
        return ix;
    }

    return attr_flame_tbl[ix - 32];
}

static u16 apply_freeze_damage_attribute(PLW* as, u16 ix) {
    if (is_reversed_gill(as)) {
        ix = attr_flame_tbl[ix - 32];
        as->wu.at_attribute = 1;
        return ix;
    }

    return attr_freeze_tbl[ix - 32];
}

s16 change_damage_attribute(PLW* as, u16 atr, u16 ix) { // 🟢
    switch (atr) {
    case 1:
        ix = apply_flame_damage_attribute(as, ix);
        break;

    case 2:
        ix = attr_thunder_tbl[ix - 32];
        break;

    case 3:
        ix = apply_freeze_damage_attribute(as, ix);
        break;
    }

    return ix;
}

s16 get_sky_nm_damage(u16 ix) { // 🟢
    ix -= 32;
    return sky_nm_damage_tbl[ix];
}

s16 get_sky_sp_damage(u16 ix) { // 🟢
    ix -= 32;
    return sky_sp_damage_tbl[ix];
}

s16 get_kagami_damage(u16 ix) { // 🟢
    ix -= 32;
    return kagami_damage_tbl[ix];
}

s16 get_grd_hand_damage(u16 ix) { // 🟢
    ix -= 32;
    return grd_hand_damage_tbl[ix];
}

u8 check_head_damage(s16 ix) { // 🟢
    ix -= 32;
    return hddm_damage_tbl[ix];
}

u8 check_trunk_damage(s16 ix) { // 🟢
    ix -= 32;
    return trdm_damage_tbl[ix];
}

u8 check_ttk_damage_request(s16 ix) { // 🟢
    ix -= 32;
    return ttk_damage_req_tbl[ix];
}
