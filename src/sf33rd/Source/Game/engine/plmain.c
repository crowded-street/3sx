/**
 * @file plmain.c
 * Player Character's Core Gameplay Logic
 */

#include "sf33rd/Source/Game/engine/plmain.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpca.h"
#include "sf33rd/Source/Game/engine/plpcu.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/plpnm.h"
#include "sf33rd/Source/Game/engine/pls00.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/spgauge.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/meta_col.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/ui/input_history.h"

void plmv_1010(PLW* wk);
void plmv_1020(PLW* wk, s16 step);
void mpg_union(PLW* wk);
void eag_union(PLW* wk);
void sag_union(PLW* wk);
void addSAAttribute(u8* kow, u16* koa);
void check_omop_vital(PLW* wk);
s16 select_hit_stop(s16 ms, s16 sb);

static s32 zuru_timer_is_running(const PLW* wk) {
    return Timer_Freeze == 0 && wk->wu.hit_stop == 0 && wk->zuru_timer > 0;
}

static s32 only_this_player_is_moving(const PLW* wk, const WORK* emwk) {
    return (wk->wu.routine_no[3] == 0) && ((wk->wu.routine_no[1] == 1) || (wk->wu.routine_no[1] == 3)) &&
           (emwk->routine_no[1] != 1) && (emwk->routine_no[1] != 3);
}

/* Read this frame's lever data - from the pad for a human player, from the CPU
 * algorithm otherwise - normalise it, and record it in the input history. */
static void read_lever_input(PLW* wk, u16 lv_data) {
    if (wk->wu.operator) {
        wk->cp->sw_lvbt = lv_data;
    } else {
        wk->cp->sw_lvbt = processed_lvbt(cpu_algorithm(wk));
    }

    if (!ArcadeBalance_IsEnabled()) {
        // The PS2 port normalizes impossible direction combinations; CPS3 uses the raw processed input.
        wk->cp->sw_lvbt = check_illegal_lever_data(wk->cp->sw_lvbt);
    }

    InputHistory_Append(wk->cp->sw_lvbt, wk->wu.id);
}

/* The states in which the player's input is thrown away for the frame. The
 * resurrection case is arcade-balance-gated where the others are not, as in the
 * original. */
static void silence_lever_when_inactive(PLW* wk) {
    if (wk->metamor_over) {
        wk->cp->sw_lvbt = 0;
    }

    if (!ArcadeBalance_IsEnabled()) {
        if (wk->resurrection_resv) {
            wk->cp->sw_lvbt = 0;
        }
    }

    if (wk->dead_flag) {
        wk->cp->sw_lvbt = 0;
    }

    if (wk->wkey_flag) {
        wk->cp->sw_lvbt = 0;
    }

    if ((wk->dead_flag + wk->wkey_flag) == 0) {
        wk->hurimukenai_flag = 0;
    }
}

void Player_move(PLW* wk, u16 lv_data) { // 🟡
    // CPS3 has a pre-recorded replay path here, but it seems to be unreachable from normal gameplay, so it's omitted.
    s16 i;

    read_lever_input(wk, lv_data);

    silence_lever_when_inactive(wk);

    for (i = 0; i < 8; i++) {
        wk->wu.old_rno[i] = wk->wu.routine_no[i];
    }

    for (i = 0; i < 3; i++) {
        wk->wu.old_pos[i] = wk->wu.xyz[i].disp.pos;
    }

    get_saikinnno_idouryou(wk);
    wk->old_gdflag = wk->guard_flag;
    wk->wu.renew_attack = 0;
    wk->wu.vital_old = wk->wu.vital_new;

    if (wk->sa_stop_flag != 1) {
        waza_check(wk);
    } else {
        key_thru(wk);
    }

    wk->wu.cmwk[10] = wk->cp->lgp;
    wk->wu.cmwk[11] += wk->cp->lgp;
    wk->wu.cmwk[11] &= 0x7FFF;
    wk->wu.cmwk[12] = wk->cp->sw_new;
    wk->wu.cmwk[13] = wk->cp->sw_now;
    plmain_lv_00[wk->wu.routine_no[0]](wk);
}

u16 check_illegal_lever_data(u16 data) { // 🔴
    u16 lever = data & 0xF;

    data = (data & ~0xF) | Correct_Lv_Data[lever];
    return data;
}

/* How the super-art gauge starts a round, which the two DIP switches between
 * them decide: empty, or full for a demo. Skipped entirely under arcade
 * balance, as in the original, and the fallthrough from the round-reset case
 * into the demo case is preserved. */
static void reset_super_art_gauge_for_round(PLW* wk) {
    if (!ArcadeBalance_IsEnabled()) {
        wk->omop_vital_timer = 40;

        switch (wk->spmv_ng_flag2 & (DIP2_SA_GAUGE_ROUND_RESET_DISABLED | DIP2_SA_GAUGE_MAX_START_DISABLED)) {
        case DIP2_SA_GAUGE_MAX_START_DISABLED:
            clear_super_arts_point(wk);
            spgauge_cont_init();
            break;

        case DIP2_SA_GAUGE_ROUND_RESET_DISABLED:
            if (Round_num != 0) {
                break;
            }

            /* fallthrough */

        case 0:
            demo_set_sa_full(wk->sa);
            spgauge_cont_demo_init();
            break;
        }
    }
}

void player_mv_0000(PLW* wk) { // 🟡
    s16 i;

    for (i = 0; i < 8; i++) {
        wk->old_pos_data[i] = 0;
    }

    setup_vitality(&wk->wu, wk->player_number);
    set_player_shadow(wk);
    wk->bullet_hcnt = wk->bhcnt_timer = 0;
    wk->auto_guard = 0;
    wk->wu.hit_stop = wk->wu.dm_stop = 0;
    wk->wu.hit_quake = wk->wu.dm_quake = 0;
    wk->tsukamarenai_flag = 0;
    wk->zuru_timer = 0;
    wk->zuru_flag = false;
    wk->tsukami_f = wk->tsukamare_f = false;
    clear_kizetsu_point(wk);

    // CPS3-only assignment
    // TODO: figure out what this does
    // DAT_20281a8[wk->wu.id] = 0;

    wk->ukemi_ok_timer = 0;
    wk->uot_cd_ok_flag = 0;
    wk->ukemi_success = 0;

    clear_my_shell_ix(&wk->wu);

    wk->sa->mp = 0;
    wk->sa->ok = 0;
    wk->sa->ex = 0;
    wk->sa->mp_rno = 0;
    wk->sa->mp_rno2 = 0;
    wk->sa->sa_rno = 0;
    wk->sa->sa_rno2 = 0;
    wk->sa->ex_rno = 0;

    wk->metamorphose = 0;
    wk->metamor_over = 0;
    wk->sa_healing = 0;

    if (!ArcadeBalance_IsEnabled()) {
        wk->resurrection_resv = 0;
    }

    wk->dm_hos_flag = 0;
    wk->kezurijini_flag = 0;
    wk->wu.floor = 0;
    wk->bs2_area_car = 0;
    wk->bs2_over_car = 0;
    wk->bs2_on_car = 0;
    wk->wu.extra_col = wk->wu.extra_col_2 = 0;
    wk->sa_stop_flag = 0;
    clear_tk_flags(wk);
    wk->wu.routine_no[0] = 1;

    // CPS3-only block
    // if (wk->player_number == CHAR_ELENA) {
    //     FUN_06107d24(wk); // No-op function
    // }

    wk->wu.routine_no[6] = 0;
    wk->wu.cmwk[0] = 0;

    // Force Twelve to swap to his real palette to override the X.C.O.P.Y. palette
    if (wk->player_number == CHAR_TWELVE) {
        metamor_color_restore(wk->wu.id);
    }

    reset_super_art_gauge_for_round(wk);

    about_gauge_process(wk);
}

void player_mv_1000(PLW* wk) { // 🟡
    switch (appear_type) {
    case APPEAR_TYPE_NON_ANIMATED:
        plmv_1010(wk);

        if (Combo_Demo_Flag == 0) {
            plmv_1020(wk, 88);
        } else {
            set_super_arts_status(wk->wu.id);
            demo_set_sa_full(wk->sa);
        }

        Appear_end++;
        break;

    case APPEAR_TYPE_UNKNOWN_3:
        plmv_1010(wk);
        plmv_1020(wk, 128);
        break;

    case APPEAR_TYPE_ANIMATED:
    case APPEAR_TYPE_UNKNOWN_2:
        wk->wu.routine_no[0] = 2;
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 0;
        wk->wu.routine_no[3] = 0;

        if (Combo_Demo_Flag == 0) {
            wk->wu.disp_flag = 1;
        }

        appear_data_init_set(wk);
        break;
    }

    Player_normal(wk);

    // CPS3 does not run the port's extra gauge update after player-normal processing.
    if (!ArcadeBalance_IsEnabled()) {
        about_gauge_process(wk);
    }
}

void plmv_1010(PLW* wk) { // 🟢
    wk->wu.routine_no[0] = 3;
    wk->wu.routine_no[1] = 0;
    wk->wu.routine_no[2] = 1;
    wk->wu.routine_no[3] = 0;

    if (Combo_Demo_Flag == 0) {
        wk->wu.disp_flag = 1;
    }
}

void plmv_1020(PLW* wk, s16 step) { // 🟡
    if (wk->wu.id) {
        wk->wu.rl_flag = 0;
        wk->wu.xyz[0].disp.pos = step + get_center_position();
        wk->wu.xyz[1].disp.pos = 0;
    } else {
        wk->wu.rl_flag = 1;
        wk->wu.xyz[0].disp.pos = get_center_position() - step;
        wk->wu.xyz[1].disp.pos = 0;
    }

    // CPS3 only sets the initial position here; the port also advances the gauge.
    if (!ArcadeBalance_IsEnabled()) {
        about_gauge_process(wk);
    }
}

void player_mv_2000(PLW* wk) { // 🟡
    if (wk->wu.routine_no[2] == 1) {
        wk->wu.routine_no[0] = 3;

        if (Combo_Demo_Flag == 0) {
            wk->wu.disp_flag = 1;
        }

        wk->wu.cg_type = 0;
    }

    Player_normal(wk);

    if (!ArcadeBalance_IsEnabled()) {
        about_gauge_process(wk);
    }
}

void player_mv_3000(PLW* wk) { // 🟡
    if (gouki_app) {
        gouki_appear(wk);
    } else {
        Player_normal(wk);
    }

    if (!ArcadeBalance_IsEnabled()) {
        about_gauge_process(wk);
    }
}

void player_mv_4000(PLW* wk) { // 🟡
    wk->permited_koa = 0;
    check_extra_jump_timer(wk);

    if (wk->sa_stop_flag != 1) {
        check_lever_data(wk);
    }

    if (wk->tsukamare_f) {
        wk->wu.hit_stop = wk->wu.dm_stop = 0;
    }

    if (!check_hit_stop(wk)) {
        plmain_lv_02[wk->wu.routine_no[1]](wk);

        if (zuru_timer_is_running(wk)) {
            wk->zuru_timer -= 2;
        }

        if (wk->zuru_timer < 0) {
            wk->zuru_flag = true;
        } else {
            wk->zuru_flag = false;
        }

        if (!ArcadeBalance_IsEnabled()) {
            check_omop_vital(wk);
        }
    }

    if (Timer_Freeze == 0) {
        look_after_timers(wk);
    }

    about_gauge_process(wk);
}

/* The two stops competing for the same frame. Returns 1 in the case where the
 * damage stop won, which ended check_hit_stop at once - before the
 * sa_stop_flag test at its foot, so the caller returns straight away too. */
static s32 damage_stop_wins(PLW* wk) {
    if ((wk->wu.dm_stop != 0) && (wk->wu.hit_stop != 0)) {
        if (wk->wu.routine_no[3]) {
            wk->wu.hit_stop = select_hit_stop(wk->wu.hit_stop, wk->wu.dm_stop);
            wk->wu.dm_stop = 0;
        } else {
            wk->wu.dm_stop = select_hit_stop(wk->wu.dm_stop, wk->wu.hit_stop);
            wk->wu.hit_stop = 0;
            return 1;
        }
    }

    return 0;
}

/* One frame of hit stop counting off, including the super-art freeze handing
 * back control part way through. A negative hit_stop counts the other way and
 * keeps animating, as in the original. */
static void tick_hit_stop(PLW* wk) {
    if (wk->wu.hit_stop > 0) {
        wk->wu.hit_stop--;

        if (wk->sa_stop_flag == 2) {
            if (wk->just_sa_stop_timer == Game_timer) {
                wk->wu.hit_stop++;
            }

            if (wk->wu.hit_stop <= wk->sa_stop_sai) {
                wk->sa_stop_lvdir = wk->cp->sw_lvbt;
                wk->sa_stop_flag = 1;
            }
        }
    } else {
        wk->wu.hit_stop++;
        char_move(&wk->wu);
    }
}

s16 check_hit_stop(PLW* wk) { // 🟢
    s16 num;
    WORK* emwk = (WORK*)wk->wu.target_adrs;

    num = 0;

    if (damage_stop_wins(wk)) {
        return 0;
    }

    if (wk->wu.hit_stop) {
        num = 1;

        tick_hit_stop(wk);

        if (only_this_player_is_moving(wk, emwk)) {
            num = 0;
        }

        if ((wk->wu.hit_stop == 0) && (wk->hsjp_ok != 0)) {
            char_move_cmhs(wk);
        }
    }

    if (wk->sa_stop_flag) {
        Timer_Freeze = 1;
    }

    return num;
}

s16 select_hit_stop(s16 ms, s16 sb) {
    s16 maf = 1;

    if (sb < 0) {
        sb = -sb;
    }

    if (ms < 0) {
        ms = -ms;
        maf = -1;
    }

    if (ms < sb) {
        ms = sb;
    }

    return ms * maf;
}

/* The recovery-roll window closing, either because it ran out or because the
 * player took it. Both outcomes clear the same two fields and differ only in
 * what ukemi_success is left at, as in the original. */
static void tick_ukemi_window(PLW* wk) {
    if (wk->uot_cd_ok_flag) {
        wk->ukemi_ok_timer--;

        if (wk->ukemi_ok_timer <= 0) {
            wk->ukemi_ok_timer = 0;
            wk->uot_cd_ok_flag = 0;
            wk->ukemi_success = 0;
        } else if (check_ukemi_flag(wk)) {
            wk->ukemi_ok_timer = 0;
            wk->uot_cd_ok_flag = 0;
            wk->ukemi_success = 1;
        }
    }
}

/* The stun gauge draining back down between hits. */
static void tick_stun_recovery(PLW* wk) {
    if (wk->py->now.quantity.h && (wk->wu.hit_stop == 0)) {
        // CPS3 uses the raw rate. The default port option is neutral (32 / 32); non-default recovery options differ.
        wk->py->now.timer -= (wk->py->recover * stun_gauge_r_omake[omop_stun_gauge_rcv[wk->wu.id]]) / 32;

        if (wk->py->now.quantity.h <= 0) {
            wk->py->now.timer = 0;
        }
    }
}

/* The one-button super-art debug option. Compiled away entirely in a release
 * build, as the block it came from was. */
static void arm_one_button_super_arts(PLW* wk) {
#if DEBUG
    if (debug_config.one_button_sa) {
        if (wk->sa->nmsa_g_ix != 0) {
            wk->cp->waza_flag[wk->sa->nmsa_g_ix] = 9;
        }

        if (wk->sa->exsa_g_ix != 0) {
            wk->cp->waza_flag[wk->sa->exsa_g_ix] = 9;
        }

        if (wk->sa->exs2_g_ix != 0) {
            wk->cp->waza_flag[wk->sa->exs2_g_ix] = 9;
        }

        if (wk->sa->nmsa_a_ix != 0) {
            wk->cp->waza_flag[wk->sa->nmsa_a_ix] = 9;
        }

        if (wk->sa->exsa_a_ix != 0) {
            wk->cp->waza_flag[wk->sa->exsa_a_ix] = 9;
        }

        if (wk->sa->exs2_a_ix != 0) {
            wk->cp->waza_flag[wk->sa->exs2_a_ix] = 9;
        }
    }
#else
    (void)wk;
#endif
}

void look_after_timers(PLW* wk) { // 🟡
    if (wk->tsukamarenai_flag) {
        wk->tsukamarenai_flag--;
    }

    if (wk->cat_break_ok_timer) {
        wk->cat_break_ok_timer--;
    }

    tick_ukemi_window(wk);

    if (wk->bullet_hcnt) {
        if (--wk->bhcnt_timer <= 0) {
            wk->bullet_hcnt = 0;
        }
    }

    tick_stun_recovery(wk);

    arm_one_button_super_arts(wk);
}

void about_gauge_process(PLW* wk) { // 🟡
    eag_union(wk);
    sag_union(wk);
    mpg_union(wk);

    // CPS3 has no equivalent max-gauge bit update.
    if (!ArcadeBalance_IsEnabled()) {
        add_sp_arts_gauge_maxbit(wk);
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

        if (!ArcadeBalance_IsEnabled()) {
            // CPS3 clears this meter state without the port's super-art bug workaround.
            sag_bug_fix(wk->wu.id);
        }

        wk->sa->saeff_mp = 0;
        wk->sa->mp_rno = 0;
        wk->sa->mp = 0;

        if (!ArcadeBalance_IsEnabled()) {
            // The port delays the next super-art gain for 20 frames; CPS3 does not.
            sag_inc_timer[wk->wu.id] = 20;
        }

        break;

    case 1:
        if (wk->wu.routine_no[1] == 4) {
            break;
        }

        /* fallthrough */

    default:
        wk->sa->saeff_mp = 0;
        wk->sa->mp_rno = 0;
        wk->sa->mp = 0;
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

void eag_union(PLW* wk) { // 🟡
    switch (wk->sa->ex_rno) {
    case 0:
        arm_ex_when_affordable(wk);
        break;

    case 1:
        disarm_or_fire_ex(wk);
        break;

    case 2:
        spend_ex_gauge(wk);

        if (!ArcadeBalance_IsEnabled()) {
            // CPS3 clears this meter state without the port's super-art bug workaround.
            sag_bug_fix(wk->wu.id);
        }

        wk->sa->ex_rno = 0;
        wk->sa->ex = 0;

        if (!ArcadeBalance_IsEnabled()) {
            // The port delays the next super-art gain for 20 frames; CPS3 does not.
            sag_inc_timer[wk->wu.id] = 20;
        }

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
static void abandon_super_art(PLW* wk) {
    wk->sa->saeff_ok = 0;
    wk->sa->sa_rno = 0;
    wk->sa->ok = 0;
}

/* The unreachable-state reset the super-art machines fall back on, shared
 * verbatim by sag_union_0, sag_union_3 and sag_union_ps2. sag_union_1's reset
 * also clears dtm_mul and is left where it is. */
static void clear_super_art_state(PLW* wk) {
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
        mark_art_attack_for(wk, CHAR_YUN);
        mark_art_attack_for(wk, CHAR_YANG);
        mark_art_attack_for(wk, CHAR_MAKOTO);
        mark_art_attack_for(wk, CHAR_TWELVE);

        if ((My_char[wk->wu.id] == CHAR_ORO) && (wk->sa->kind_of_arts == 2)) {
            wk->wu.att.dipsw |= 0x10;
        }
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

void get_saikinnno_idouryou(PLW* wk) { // 🟢
    s16 i;

    for (i = 0; i < 7; i++) {
        wk->old_pos_data[i] = wk->old_pos_data[i + 1];
    }

    wk->old_pos_data[i] = wk->wu.xyz[0].disp.pos;
    wk->move_distance = wk->old_pos_data[7] - wk->old_pos_data[0];
    wk->move_power = cal_move_quantity2(wk->old_pos_data[0], 0, wk->old_pos_data[7], 0);
    wk->move_power >>= 3;
}

void clear_attack_num(WORK* wk) { // 🟢
    s16 i;

    for (i = 0; i < 4; i++) {
        wk->uketa_att[i] = 0;
    }

    wk->attack_num = 0;
}

void clear_tk_flags(PLW* wk) { // 🟢
    wk->tk_success = 0;
    wk->tk_dageki = 0;
    wk->tk_nage = 0;
    wk->tk_kizetsu = 0;
    wk->tk_konjyou = 0;
    wk->att_plus = 8;
    wk->def_plus = 8;
}

void (*const plmain_lv_00[5])(PLW* wk) = {
    player_mv_0000, player_mv_1000, player_mv_2000, player_mv_3000, player_mv_4000
};

void (*const plmain_lv_02[5])(PLW* wk) = { Player_normal, Player_damage, Player_catch, Player_caught, Player_attack };

const u8 plpnm_mvkind[59] = { 0, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 0, 0, 0, 0, 0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3,
                              3, 2, 2, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const u8 plpdm_mvkind[32] = { 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };

const u8 plpxx_kind[5] = { 0, 1, 0, 1, 0 };

/* The four states that hold the draining vitality still, in the order the
 * original tested them. Each was its own `break` out of case 0; `||`
 * short-circuits the same way, and none of the four has a side effect. */
static s32 vital_drain_is_paused(const PLW* wk) {
    return vital_dec_timer || ((wk->wu.routine_no[1] == 0) && !(plpnm_mvkind[wk->wu.routine_no[2]] & 1)) ||
           ((wk->wu.routine_no[1] == 1) && !(plpdm_mvkind[wk->wu.routine_no[2]] & 1)) || (wk->wu.routine_no[1] == 3);
}

/* The second of the two player-one states that hold the vitality drain: a
 * specific pattern of a specific attack routine. The three numbers are the
 * original's. */
static s32 in_second_credit_pose(const PLW* wk) {
    return (wk->wu.routine_no[1] == 4) && (wk->wu.routine_no[2] == 22) && (wk->wu.pat_status == 23);
}

/* The vitality drain itself, and the death it ends in. Every `break` in the
 * original left the switch with nothing after it, so each is a `return` here.
 *
 * The player-one credit-check states are the two that hold the drain without
 * being a pause: the game is waiting on a coin, not on the player. */
static void drain_vitality(PLW* wk) {
    if (vital_drain_is_paused(wk)) {
        return;
    }

    if (wk->player_number == 0) {
        if ((wk->wu.routine_no[1] == 4) && (wk->wu.routine_no[2] == 21)) {
            if (ca_check_flag == 0) {
                ca_check_flag = 1;
            }

            return;
        }

        if (in_second_credit_pose(wk)) {
            return;
        }
    }

    wk->wu.vital_new--;

    if (wk->wu.vital_new < 0) {
        wk->wu.vital_new = -1;
        wk->wu.dm_koa = 4;
        wk->dead_flag = 1;
        wk->guard_flag = 3;
        ca_check_flag = 0;
        return;
    }
}

/* One point of health back, up to the bar's length. */
static void gain_one_vitality(PLW* wk) {
    wk->wu.vital_new++;

    if (wk->wu.vital_new > 160) {
        wk->wu.vital_new = 160;
    }
}

/* Mode 2 only regains while the player is idle, and only on the frames the
 * regain timer allows. */
static s32 idle_regain_is_blocked(PLW* wk) {
    if (vital_inc_timer) {
        return 1;
    }

    if (wk->wu.routine_no[1] != 0) {
        return 1;
    }

    if (!(plpnm_mvkind[wk->wu.routine_no[2]] & 2)) {
        return 1;
    }

    return 0;
}

/* Mode 3 stops regaining while the player is in one of the plpxx states, and
 * for a while after leaving one. */
static s32 timed_regain_is_blocked(PLW* wk) {
    if (plpxx_kind[wk->wu.routine_no[1]]) {
        return 1;
    }

    if (plpxx_kind[wk->wu.old_rno[1]]) {
        wk->omop_vital_timer = 40;
    }

    if (wk->omop_vital_timer) {
        wk->omop_vital_timer--;
        return 1;
    }

    return 0;
}

void check_omop_vital(PLW* wk) { // 🔴
    if (pcon_dp_flag) {
        return;
    }

    if (wk->dead_flag) {
        return;
    }

    if (sa_stop_check()) {
        return;
    }

    if (wk->resurrection_resv) {
        wk->wu.vital_new = -1;
        return;
    }

    switch (omop_vital_ix[wk->wu.id]) {
    case 0:
        drain_vitality(wk);
        break;

    case 2:
        if (idle_regain_is_blocked(wk)) {
            break;
        }

        gain_one_vitality(wk);
        break;

    case 3:
        if (timed_regain_is_blocked(wk)) {
            break;
        }

        /* fallthrough */

    case 4:
        gain_one_vitality(wk);
        break;
    }
}
