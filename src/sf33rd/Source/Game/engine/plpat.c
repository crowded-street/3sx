/**
 * @file plpat.c
 * Player Attack Controller
 */

#include "sf33rd/Source/Game/engine/plpat.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effg6.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpat00.h"
#include "sf33rd/Source/Game/engine/plpat01.h"
#include "sf33rd/Source/Game/engine/plpat02.h"
#include "sf33rd/Source/Game/engine/plpat03.h"
#include "sf33rd/Source/Game/engine/plpat04.h"
#include "sf33rd/Source/Game/engine/plpat05.h"
#include "sf33rd/Source/Game/engine/plpat06.h"
#include "sf33rd/Source/Game/engine/plpat07.h"
#include "sf33rd/Source/Game/engine/plpat08.h"
#include "sf33rd/Source/Game/engine/plpat09.h"
#include "sf33rd/Source/Game/engine/plpat10.h"
#include "sf33rd/Source/Game/engine/plpat11.h"
#include "sf33rd/Source/Game/engine/plpat12.h"
#include "sf33rd/Source/Game/engine/plpat13.h"
#include "sf33rd/Source/Game/engine/plpat14.h"
#include "sf33rd/Source/Game/engine/plpat16.h"
#include "sf33rd/Source/Game/engine/plpat17.h"
#include "sf33rd/Source/Game/engine/plpat18.h"
#include "sf33rd/Source/Game/engine/plpat19.h"
#include "sf33rd/Source/Game/engine/plpat20.h"
#include "sf33rd/Source/Game/engine/plpnm.h"
#include "sf33rd/Source/Game/engine/pls00.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"

s16 ja_nmj_rno_change(WORK* wk);
void Attack_07000(PLW* wk);
void get_cancel_timer(PLW* wk);
void check_ja_nmj_dummy_RTNM(PLW* wk);
u8 get_cjdR(PLW*);

void (*const plpat_lv_00[16])(PLW* wk);
void (*const plxx_extra_attack_table[])();

const u8* cjdr_karaburi_table[20];
const u8* cjdr_hits_table[20];
const u8* cjdr_blocking_table[20];
const u8* cjdr_defense_table[20];

/* The first frame of an attack clears its per-attack bookkeeping; every later frame
 * only refreshes the pad feedback. */
static void reset_attack_entry_state(PLW* wk) {
    if (wk->wu.routine_no[3] != 0) {
        // Port-only controller feedback; it does not affect attack behavior.
        pp_pulpara_remake_at(wk);
        return;
    }

    wk->caution_flag = 1;
    wk->dm_vital_backup = 0;
    wk->dm_vital_use = 0;
    wk->total_att_hit_ok = 0;
    wk->hsjp_ok = 0;

    if (!ArcadeBalance_IsEnabled() && wk->wu.routine_no[2] < 16) {
        // Chain EX bookkeeping is port-only and absent from CPS3.
        clear_chainex_check(wk->wu.id);
    }
}

/* A cg_prio of 1 pushes the player in front of the opponent, anything else behind. */
static void bias_next_z_by_cg_prio(PLW* wk) {
    if (!wk->wu.cg_prio) {
        return;
    }

    if (wk->wu.cg_prio == 1) {
        wk->wu.next_z += 4;
    } else {
        wk->wu.next_z -= 4;
    }
}

void Player_attack(PLW* wk) { // 🟡
    wk->wu.next_z = wk->wu.my_priority;
    wk->running_f = 0;
    wk->py->flag = 0;
    wk->guard_flag = 3;
    wk->guard_chuu = 0;
    wk->tsukami_f = false;
    wk->tsukamare_f = false;
    wk->scr_pos_set_flag = 1;
    wk->dm_hos_flag = 0;
    wk->ukemi_success = 0;
    wk->zuru_timer = 0;
    wk->zuru_ix_counter = 0;
    wk->sa_stop_flag = 0;
    wk->ukemi_success = 0;
    wk->ukemi_ok_timer = 0;
    wk->uot_cd_ok_flag = 0;
    wk->hazusenai_flag = 0;
    wk->cat_break_reserve = 0;
    wk->wu.swallow_no_effect = 0; // Port-only visual suppression; CPS3 does not reset this effect flag here.
    check_em_tk_power_off(wk, (PLW*)wk->wu.target_adrs);

    reset_attack_entry_state(wk);

    jumping_guard_type_check(wk);

    if (wk->wu.routine_no[2] > 15) {
        plxx_extra_attack_table[wk->player_number](wk);
    } else {
        wk->sa->saeff_ok = 0;
        wk->sa->saeff_mp = 0;
        plpat_lv_00[wk->wu.routine_no[2]](wk);
    }

    wk->wu.next_z = ((PLW*)wk->wu.target_adrs)->wu.my_priority - 3;

    bias_next_z_by_cg_prio(wk);
}

/* The opening frame shared by the level-4 ground attacks: land, face the way the
 * move was buffered, take the cancel timer, start the animation. Attack_01000
 * and Attack_02000 follow it with their own setup_mvxy_data call, which stays at
 * the call site. */
static void begin_ground_attack(PLW* wk) {
    wk->wu.routine_no[3]++;
    hoken_muriyari_chakuchi(wk);
    wk->wu.rl_flag = wk->wu.rl_waza;
    setup_lvdir_after_autodir(wk);
    get_cancel_timer(wk);
    set_char_move_init(&wk->wu, 4, wk->as->char_ix);
}

void Attack_00000(PLW* wk) { // 🟢
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_ground_attack(wk);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[2] = 4;
            wk->wu.cg_type = 0;
            wk->scr_pos_set_flag = 1;
        }

        break;
    }
}

void Attack_01000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_ground_attack(wk);
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            add_mvxy_speed(&wk->wu);
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            break;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Attack_02000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_ground_attack(wk);
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            add_mvxy_speed(&wk->wu);
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            break;
        }

        break;

    case 2:
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        /* fallthrough */

    case 3:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
        }

        break;
    }
}

/* On the ground - or standing on the bonus-stage car - the attack starts from a
 * landed pose, so the unit is re-initialised before the animation is set. */
static s32 is_standing_for_ja_attack(const PLW* wk) {
    return (Bonus_Game_Flag == 20 && wk->bs2_on_car) || (wk->wu.xyz[1].disp.pos <= 0);
}

static void init_ja_attack_from_ground(PLW* wk) {
    if (is_standing_for_ja_attack(wk)) {
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        setup_lvdir_after_autodir(wk);
        Normal_18000_init_unit(wk, wk->wu.pat_status);
    }
}

/* The dummy-RTNM window, which only runs while the jump is still airborne. The
 * arm's inner break left the switch with nothing after it, so returning here is
 * the same exit. */
static void step_ja_nmj_dummy(PLW* wk) {
    if (wk->wu.routine_no[3] == 3) {
        return;
    }

    check_ja_nmj_dummy_RTNM(wk);

    if (wk->wu.cg_type != 0x40) {
        return;
    }

    if (!(wk->spmv_ng_flag & 0x100000) && ja_nmj_rno_change(&wk->wu)) {
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[3] = 1;
    }

    wk->wu.cg_type = 0;
}

void Attack_03000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        get_cancel_timer(wk);
        init_ja_attack_from_ground(wk);
        set_char_move_init(&wk->wu, 4, wk->as->char_ix);
        break;

    case 1:
        if ((wk->wu.mvxy.a[1].sp > 0) && (wk->wu.xyz[1].disp.pos < 16)) {
            add_mvxy_speed(&wk->wu);
            cal_mvxy_speed(&wk->wu);
            break;
        }

        wk->wu.routine_no[3]++;

    case 2:
        jumping_union_process(&wk->wu, 3);
        step_ja_nmj_dummy(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

/* The later half of the jump-attack rno mapping. The labels are the original
 * pattern-status numbers and the helper returns the same 0 the caller started
 * with when none of them matches, so an unlisted status still changes nothing. */
static s16 ja_nmj_rno_change_rest(WORK* wk) {
    s16 rnum = 0;

    switch (wk->pat_status) {
    case 28:
        wk->routine_no[2] = 25;
        rnum = 1;
        break;

    case 24:
        wk->routine_no[2] = 23;
        rnum = 1;
        break;

    case 18:
        wk->routine_no[2] = 20;
        rnum = 1;
        break;

    case 30:
        wk->routine_no[2] = 26;
        rnum = 1;
        break;
    }

    return rnum;
}

s16 ja_nmj_rno_change(WORK* wk) { // 🟢
    s16 rnum = 0;

    switch (wk->pat_status) {
    case 20:
        wk->routine_no[2] = 21;
        rnum = 1;
        break;

    case 14:
        wk->routine_no[2] = 18;
        rnum = 1;
        break;

    case 26:
        wk->routine_no[2] = 24;
        rnum = 1;
        break;

    case 22:
        wk->routine_no[2] = 22;
        rnum = 1;
        break;

    case 16:
        wk->routine_no[2] = 19;
        rnum = 1;
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
        goto case0;
    }

    if (wk->wu.hf.hit.player & 3) {
        goto case1;
    }

    if (wk->wu.hf.hit.player & 0xC0) {
        goto case2;
    }

    if (wk->wu.hf.hit.player & 0x30) {
        goto case3;
    }

case0:
    return cjdr_karaburi_table[wk->player_number][w_ix];

case1:
    return cjdr_hits_table[wk->player_number][w_ix];

case2:
    return cjdr_blocking_table[wk->player_number][w_ix];

case3:
    return cjdr_defense_table[wk->player_number][w_ix];
}

void Attack_04000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_ground_attack(wk);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[2] = 1;
            wk->wu.cg_type = 0;
            wk->scr_pos_set_flag = 0;
        }

        break;
    }
}

void Attack_05000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);

        if (wk->wu.xyz[1].disp.pos > 0) {
            wk->wu.routine_no[3] = 2;
            char_move_wca(&wk->wu);
        } else {
            hoken_muriyari_chakuchi(wk);
            wk->wu.routine_no[3] = 1;
        }

        wk->cancel_timer = 0;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            add_mvxy_speed(&wk->wu);
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            effect_G6_init(&wk->wu, wk->wu.weight_level);
            break;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Attack_06000(PLW* wk) { // 🟢
    wk->scr_pos_set_flag = 0;
    Attack_07000(wk);
}

void Attack_07000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        get_cancel_timer(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;
            wk->wu.rl_flag = wk->wu.rl_waza;
            break;
        }

        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;
            wk->wu.rl_flag = wk->wu.rl_waza;
        }

        break;
    }
}

/* The opening frame of the multi-step air attack. Started from the ground it
 * picks its movement row from the pattern status, capped at row 10. */
static void begin_stepped_air_attack(PLW* wk) {
    s16 ixx;

    wk->wu.routine_no[3]++;

    if (wk->wu.xyz[1].disp.pos <= 0) {
        wk->wu.rl_flag = wk->wu.rl_waza;
        wk->wu.xyz[1].disp.pos = 0;

        ixx = ((wk->wu.pat_status - 20) / 2 & 3) + 9;

        if (ixx > 11) {
            ixx = 10;
        }

        setup_mvxy_data(&wk->wu, ixx);
    }

    get_cancel_timer(wk);
    set_char_move_init(&wk->wu, 4, (s16)((wk->as->char_ix)));
    wk->wu.mvxy.index = wk->as->data_ix;
}

/* Each 20 marker in the airborne arm folds the next movement row in and steps
 * the index on. The arm's `break` left the switch with nothing after it. */
static void advance_stepped_air_attack(PLW* wk) {
    if ((wk->wu.routine_no[3] != 3) && (wk->wu.cg_type == 20)) {
        add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.cg_type = 0;
        wk->wu.mvxy.index++;
    }
}

void Attack_08000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_stepped_air_attack(wk);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            wk->wu.routine_no[3]++;
        }

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
            break;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        advance_stepped_air_attack(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Attack_09000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        get_cancel_timer(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->data_ix);

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;
            wk->wu.rl_flag = wk->wu.rl_waza;
            break;
        }

        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.cg_type = 0;
            wk->wu.rl_flag = wk->wu.rl_waza;
        }

        if (wk->wu.cg_type == 1) {
            add_mvxy_speed(&wk->wu);
            wk->wu.routine_no[3] = 2;
            wk->wu.cg_type = 0;
            break;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

/* Whatever the leap attack ran into decides how its horizontal speed is cut and
 * which of the two landing states it drops to. Each arm's `break` left the
 * switch immediately, so a return is the same exit. */
static void deflect_leap_attack_on_contact(PLW* wk) {
    if (!((wk->wu.routine_no[3] != 4) && wk->wu.hf.hit.player)) {
        return;
    }

    if ((wk->wu.hf.hit.player & 3) != 0) {
        wk->wu.mvxy.a[0].sp /= 4;
        wk->wu.routine_no[3] = 4;
        return;
    }

    if ((wk->wu.hf.hit.player & 0x30) != 0) {
        wk->wu.mvxy.a[0].sp /= 4;
        wk->wu.mvxy.a[1].sp = 0;
        wk->wu.routine_no[3] = 3;
        return;
    }

    if ((wk->wu.hf.hit.player & 0xC0) != 0) {
        wk->wu.mvxy.a[0].sp /= 2;
        wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
        wk->wu.mvxy.a[1].sp = 0;
        wk->wu.routine_no[3] = 4;
    }
}

/* The recovery half of the leap attack, from the direction hold onwards. The case
 * labels are the original ones and the arms keep their order, so states 3, 4 and
 * 5 still read as those numbers; a value outside them did nothing before and
 * still does nothing. */
static void leap_attack_recover(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 3:
        if (--wk->wu.dir_timer > 0) {
            break;
        }

        wk->wu.routine_no[3] = 4;
        /* fallthrough */

    case 4:
        jumping_union_process(&wk->wu, 5);
        break;

    case 5:
        char_move(&wk->wu);
        break;
    }
}

void Attack_10000(PLW* wk) { // 🟢
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        wk->cancel_timer = 0;
        wk->wu.dir_timer = 3;

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[3] = 2;
        }

        grade_add_leap_attack(wk->wu.id);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type != 20) {
            break;
        }

        wk->wu.routine_no[3]++;
        /* fallthrough */

    case 2:
        jumping_union_process(&wk->wu, 4);
        deflect_leap_attack_on_contact(wk);
        break;

    default:
        leap_attack_recover(wk);
        break;
    }
}

/* While the cat-break timer is still open the break stays reserved. Three arms
 * across Attack_14000 and Attack_15000 said this the same way. */
static void reserve_cat_break_if_open(PLW* wk) {
    if (wk->cat_break_ok_timer) {
        wk->cat_break_reserve = 1;
    }
}

void Attack_14000(PLW* wk) { // 🟡
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        hoken_muriyari_chakuchi(wk);
        wk->wu.rl_flag = wk->wu.rl_waza;
        wk->cat_break_ok_timer = 6;
        setup_lvdir_after_autodir(wk);
        set_char_move_init(&wk->wu, 4, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (ArcadeBalance_IsEnabled()) {
            reserve_cat_break_if_open(wk);
        }

        break;
    }

    if (!ArcadeBalance_IsEnabled()) {
        reserve_cat_break_if_open(wk);
    }
}

/* The opening frame of the cat-break attack: land if it started on the ground,
 * open the break window, start the animation. */
static void begin_cat_break_attack(PLW* wk) {
    wk->wu.routine_no[3]++;

    if (wk->wu.xyz[1].disp.pos <= 0) {
        wk->wu.rl_flag = wk->wu.rl_waza;
        setup_lvdir_after_autodir(wk);
        wk->wu.xyz[1].disp.pos = 0;
        Normal_18000_init_unit(wk, wk->wu.pat_status);
    }

    wk->cat_break_ok_timer = 6;
    set_char_move_init(&wk->wu, 4, wk->as->char_ix);

    if (!ArcadeBalance_IsEnabled() && wk->cat_break_ok_timer) {
        // The port reserves cat-break on the initialization frame; CPS3 starts checking next frame.
        wk->cat_break_reserve = 1;
    }
}

/* The airborne arm's own reserve, which also requires the jump not to have
 * finished. That second term is why it cannot share reserve_cat_break_if_open.
 * Its `break` left the switch with nothing after it, so a return is the same
 * exit. */
static void reserve_cat_break_while_airborne(PLW* wk) {
    if ((wk->wu.routine_no[3] != 3) && wk->cat_break_ok_timer) {
        wk->cat_break_reserve = 1;
    }
}

void Attack_15000(PLW* wk) { // 🟡
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_cat_break_attack(wk);
        break;

    case 1:
        reserve_cat_break_if_open(wk);

        if ((wk->wu.mvxy.a[1].sp > 0) && (wk->wu.xyz[1].disp.pos < 0)) {
            add_mvxy_speed(&wk->wu);
            cal_mvxy_speed(&wk->wu);
            break;
        }

        wk->wu.routine_no[3]++;

    case 2:
        jumping_union_process(&wk->wu, 3);
        reserve_cat_break_while_airborne(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void get_cancel_timer(PLW* wk) { // 🟢
    if (wk->tc_1st_flag) {
        wk->cancel_timer = 0;
    } else if (wk->wu.xyz[1].disp.pos > 0) {
        wk->cancel_timer = 2;
    } else {
        wk->cancel_timer = 2;
    }
}

void hoken_muriyari_chakuchi(PLW* wk) { // 🟢
    if ((Bonus_Game_Flag == 20) && wk->bs2_on_car) {
        wk->wu.xyz[1].disp.pos = bs2_floor[2];
    } else {
        wk->wu.xyz[1].disp.pos = 0;
    }
}

void (*const plpat_lv_00[16])(PLW* wk) = { Attack_00000, Attack_01000, Attack_02000, Attack_03000,
                                           Attack_04000, Attack_05000, Attack_06000, Attack_07000,
                                           Attack_08000, Attack_09000, Attack_10000, Attack_00000,
                                           Attack_00000, Attack_00000, Attack_14000, Attack_15000 };

void (*const plxx_extra_attack_table[])() = {
    pl00_extra_attack, pl01_extra_attack, pl02_extra_attack, pl03_extra_attack, pl04_extra_attack,
    pl05_extra_attack, pl06_extra_attack, pl07_extra_attack, pl08_extra_attack, pl09_extra_attack,
    pl10_extra_attack, pl11_extra_attack, pl12_extra_attack, pl13_extra_attack, pl14_extra_attack,
    pl16_extra_attack, pl17_extra_attack, pl18_extra_attack, pl19_extra_attack, pl20_extra_attack
};

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
