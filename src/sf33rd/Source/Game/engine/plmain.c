/**
 * @file plmain.c
 * Player Character's Core Gameplay Logic
 */

#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/plmain_internal.h"
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
void addSAAttribute(u8* kow, u16* koa);
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

/* The slide timer counts down while it is running, and the flag follows it. */
static void run_zuru_timer(PLW* wk) {
    if (zuru_timer_is_running(wk)) {
        wk->zuru_timer -= 2;
    }

    if (wk->zuru_timer < 0) {
        wk->zuru_flag = true;
    } else {
        wk->zuru_flag = false;
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

        run_zuru_timer(wk);

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

/* A frame of hit stop freezes this player, unless it is the only one moving -
 * and the frame it ends on may start a cancelled move. */
static s16 run_hit_stop(PLW* wk, WORK* emwk) {
    s16 num = 1;

    tick_hit_stop(wk);

    if (only_this_player_is_moving(wk, emwk)) {
        num = 0;
    }

    if ((wk->wu.hit_stop == 0) && (wk->hsjp_ok != 0)) {
        char_move_cmhs(wk);
    }

    return num;
}

s16 check_hit_stop(PLW* wk) { // 🟢
    s16 num;
    WORK* emwk = (WORK*)wk->wu.target_adrs;

    num = 0;

    if (damage_stop_wins(wk)) {
        return 0;
    }

    if (wk->wu.hit_stop) {
        num = run_hit_stop(wk, emwk);
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
#if DEBUG
/* Each of the six super-art slots is armed the same way, and a slot the
 * character does not have is left alone. */
static void arm_super_art_slot(PLW* wk, u8 slot_ix) {
    if (slot_ix != 0) {
        wk->cp->waza_flag[slot_ix] = 9;
    }
}
#endif

static void arm_one_button_super_arts(PLW* wk) {
#if DEBUG
    if (debug_config.one_button_sa) {
        arm_super_art_slot(wk, wk->sa->nmsa_g_ix);
        arm_super_art_slot(wk, wk->sa->exsa_g_ix);
        arm_super_art_slot(wk, wk->sa->exs2_g_ix);
        arm_super_art_slot(wk, wk->sa->nmsa_a_ix);
        arm_super_art_slot(wk, wk->sa->exsa_a_ix);
        arm_super_art_slot(wk, wk->sa->exs2_a_ix);
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
