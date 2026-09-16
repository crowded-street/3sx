/**
 * @file plpdm_states.c
 * The Damage_* hit-reaction states.
 *
 * Split out of plpdm.c, which was 1666 lines. Each function is one reaction
 * routine; the table in plpdm.c indexes them by reaction number, and the
 * machinery they are built from stayed there.
 */

#include "sf33rd/Source/Game/engine/plpdm.h"
#include "arcade/arcade_balance.h"
#include "bin2obj/buttobi.h"
#include "bin2obj/etc.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effa7.h"
#include "sf33rd/Source/Game/effect/effd9.h"
#include "sf33rd/Source/Game/effect/effe2.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effg6.h"
#include "sf33rd/Source/Game/effect/effi3.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpca.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pow_pow.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#include <SDL3/SDL.h>
#include "sf33rd/Source/Game/engine/plpdm_internal.h"

static s32 is_mid_body_hit_on_0x44(const PLW* wk) {
    return wk->as->char_ix == 0x44 && (wk->dm_point == 2 || wk->dm_point == 3);
}

void Damage_00000(PLW* wk) {
    wk->wu.next_z = 30;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        set_char_move_init(&wk->wu, 1, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[3]++;
        }

        break;

    case 2:
        // Do nothing
        break;
    }
}

/* While the animation runs, a cg_type of 1 is the frame that moves the work. */
static void move_damage_01000(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 1) {
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);
    }
}

/* The same frame, later in the state, ends the movement instead of continuing
 * it. */
static void land_damage_01000(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 1) {
        wk->wu.routine_no[3] = 3;
        wk->wu.cg_type = 0;
        add_mvxy_speed(&wk->wu);
    }
}

void Damage_01000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] = 1;
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        reset_mvxy_data(&wk->wu);
        set_char_move_init(&wk->wu, 1, wk->as->char_ix);
        break;

    case 1:
        move_damage_01000(wk);
        break;

    case 2:
        land_damage_01000(wk);
        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }

    if (wk->wu.cg_type == 0xFF || wk->wu.cg_type == 64) {
        wk->guard_flag = 0;
    }
}

void Damage_04000(PLW* wk) {
    wk->guard_flag = 0;
    wk->guard_chuu = guard_kind[wk->wu.routine_no[2] - 4];
    set_dm_hos_flag_grd(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;

        if ((wk->wu.dm_quake /= 2) < 4) {
            wk->wu.dm_quake = 4;
        }

        set_char_move_init(&wk->wu, 1, wk->as->char_ix);
        wk->dm_step_tbl = _dm_step_data[_select_grd_dsd[wk->wu.dm_impact][get_weight_point(&wk->wu)]];
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        pp_pulpara_guard(&wk->wu);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        setup_smoke_type(wk);
        wk->wu.cmwk[14] = _guard_pause_table[0][wk->wu.dm_attlv];
        char_move_wca(&wk->wu);
        add_dm_step_tbl(wk, 1);
        break;

    case 2:
        add_dm_step_tbl(wk, 1);

        if (--wk->wu.cmwk[14] <= 0) {
            wk->wu.routine_no[3]++;
            char_move_wca(&wk->wu);
            break;
        }

        /* fallthrough */

    default:
        char_move(&wk->wu);
        break;
    }
}
/* Holding the guard: the block animation runs down a counter. Landing hands
 * the work to state 5, and the counter running out to state 38. */
static void hold_guard_07000(PLW* wk) {
    jumping_union_process(&wk->wu, 3);
    set_dm_hos_flag_grd(wk);
    add_dm_step_tbl(wk, 0);
    wk->wu.cmwk[14]--;

    if (wk->wu.routine_no[3] == 3) {
        if (wk->wu.cmwk[14] <= 0) {
            wk->wu.cmwk[14] = 1;
        }

        wk->wu.routine_no[2] = 5;
        wk->wu.routine_no[3] = 2;
        setup_smoke_type(wk);
        return;
    }

    if (wk->wu.cmwk[14] <= 0) {
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 38;
        wk->wu.routine_no[3] = 1;
        wk->wu.cg_type = 0;
        wk->wu.cg_next_ix = 0;
        char_move_wca(&wk->wu);
    }
}

void Damage_07000(PLW* wk) {
    wk->guard_flag = 0;
    wk->guard_chuu = guard_kind[wk->wu.routine_no[2] - 4];

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;

        if (remake_initial_speeds(&wk->wu)) {
            wk->wu.routine_no[2] = 5;
            wk->wu.routine_no[3] = 0;
            wk->wu.xyz[1].disp.pos = 0;
            wk->as = &dm_reaction_table[5];
            Damage_04000(wk);
            break;
        }

        if ((wk->wu.dm_quake /= 2) < 4) {
            wk->wu.dm_quake = 4;
        }

        set_char_move_init(&wk->wu, 1, (s16)(wk->as->char_ix));
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        pp_pulpara_guard(&wk->wu);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        wk->wu.cmwk[14] = _guard_pause_table[1][wk->wu.dm_attlv];
        wk->dm_step_tbl = _dm_step_data[_select_grd_dsd[wk->wu.dm_impact][get_weight_point(&wk->wu)]];
        char_move_wca(&wk->wu);
        /* fallthrough */

    case 2:
        hold_guard_07000(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

/* The knockback an attack level gives, by the direction the work was already
 * moving. Each case keeps its own percentages and its own table row. */
static void remake_speeds_by_direction(WORK* wk, s16 ix, s32 ay) {
    switch (ix) {
    case 0:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 80) / 100;
        wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * 120) / 100;
        cal_initial_speed_y(wk, ris_data_table[0][wk->dm_attlv], wk->xyz[1].disp.pos);
        wk->mvxy.a[1].sp += (ay * 60) / 100;
        break;

    case 1:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 75) / 100;
        wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * 100) / 100;
        cal_initial_speed_y(wk, ris_data_table[1][wk->dm_attlv], wk->xyz[1].disp.pos);
        wk->mvxy.a[1].sp += (ay * 35) / 100;
        break;

    case 2:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 70) / 100;
        wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * 80) / 100;
        cal_initial_speed_y(wk, ris_data_table[2][wk->dm_attlv], wk->xyz[1].disp.pos);
        wk->mvxy.a[1].sp += (ay * 20) / 100;
        break;

    case 3:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 80) / 100;
        wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp - 0x8000) - 0x8000;
        wk->mvxy.a[1].sp += (ay * 10) / 100;
        break;

    default:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 90) / 100;
        wk->mvxy.a[1].sp = wk->mvxy.a[1].sp + 0xFFFE0000;
        break;
    }
}

/* With no attack level the work keeps its own arc, damped horizontally and -
 * when it was already falling - vertically. */
static void remake_speeds_unattacked(WORK* wk, s32 ay, s32 dy) {
    wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * 120) / 100;

    if (ay >= 0) {
        wk->mvxy.a[1].sp = ay;
    } else {
        wk->mvxy.a[1].sp = (ay * 60) / 100;
    }

    wk->mvxy.d[1].sp = dy;
}

s32 remake_initial_speeds(WORK* wk) {
    s16 ix;
    s32 ay = wk->mvxy.a[1].sp;
    s32 dy = wk->mvxy.d[1].sp;

    if ((wk->xyz[1].disp.pos < 8) && (ay <= 0)) {
        return 1;
    }

    setup_butt_own_data(wk);
    ix = dir32_guard_air[cal_move_dir_forecast(wk, 5)];

    if (wk->dm_attlv) {
        remake_speeds_by_direction(wk, ix, ay);
    } else {
        remake_speeds_unattacked(wk, ay, dy);
    }

    if ((wk->xyz[1].disp.pos < 12) && (cal_move_quantity3(wk, 3) <= 0)) {
        return 1;
    }

    return 0;
}

/* The hit itself: the damage pattern for this attack level, the step table for
 * the impact and the defender's weight, and the attribute effects. */
static void begin_damage_12000(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
    wk->dm_ix = wk->as->char_ix + wk->wu.dm_attlv;
    set_char_move_init(&wk->wu, 1, wk->dm_ix);
    wk->dm_step_tbl = _dm_step_data[_select_hit_dsd[wk->wu.dm_impact][get_weight_point(&wk->wu)]];
    wk->zuru_timer = 0;
    wk->zuru_ix_counter = 0;

    if (wk->wu.dm_attribute) {
        setup_accessories(wk, wk->wu.pat_status);

        if (wk->wu.dm_attribute != 2) {
            effect_D9_init(wk, (u8)wk->wu.dm_attribute);
        }
    }
}

/* How long the hit stops the defender: one row of the pause table for a
 * crouching hit, another standing, and a third when the attack was a jump-in.
 * The jump-in row wins, as it did. */
static void set_damage_12000_pause(PLW* wk) {
    if (wk->wu.pat_status == 32) {
        wk->wu.cmwk[14] = _damage_pause_table[1][wk->wu.dm_attlv];
    } else {
        wk->wu.cmwk[14] = _damage_pause_table[0][wk->wu.dm_attlv];
    }

    if (wk->wu.dm_jump_att_flag) {
        wk->wu.cmwk[14] = _damage_pause_table[2][wk->wu.dm_attlv];
    }
}

void Damage_12000(PLW* wk) {
    set_dm_hos_flag_grd(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_damage_12000(wk);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        setup_smoke_type(wk);
        set_damage_12000_pause(wk);
        char_move_wca(&wk->wu);
        add_dm_step_tbl(wk, 1);
        break;

    case 2:
        add_dm_step_tbl(wk, 1);

        if (--wk->wu.cmwk[14] <= 0) {
            wk->wu.routine_no[3]++;
            char_move_wca(&wk->wu);
            break;
        }

        /* fallthrough */

    default:
        char_move(&wk->wu);
        break;
    }

    if (wk->wu.cg_type == 0xFF || wk->wu.cg_type == 0x40) {
        wk->guard_flag = 0;
    }
}

static void run_zuru_damage_sequence(PLW* wk, s32 ttktv_arg) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.dm_rl = ((WORK*)wk->wu.dmg_adrs)->rl_flag;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        wk->dm_ix = wk->as->char_ix + wk->wu.dm_attlv;
        set_char_move_init(&wk->wu, 1, wk->dm_ix);
        setup_butt_own_data(&wk->wu);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        wk->dm_hos_flag = 1;
        first_TtktV_union(wk, 3, ttktv_arg);
        break;

    case 3:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

void Damage_14000(PLW* wk) {
    run_zuru_damage_sequence(wk, 4);
}

void Damage_16000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        buttobi_add_y_check(wk);
        setup_butt_own_data(&wk->wu);
        cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], 0);
        get_sky_dm_timer(wk);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        wk->dm_hos_flag = 1;
        first_flight_union(wk, 3, 3);
        break;

    case 3:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }

    if (wk->wu.cg_type == 0xFF || wk->wu.cg_type == 0x40) {
        wk->guard_flag = 0;
    }
}

/* The airborne part of state 17: the flight itself, the landing, the pause
 * counter, and the auto air recovery that hands the work to state 23. */
static void fly_damage_17000(PLW* wk) {
    jumping_union_process(&wk->wu, 3);
    set_dm_hos_flag_sky(wk);

    if (wk->wu.cg_ja.boix == 0) {
        wk->guard_flag = 0;
    }

    if (wk->wu.routine_no[3] == 3) {
        wk->guard_flag = 0;
        wk->tsukamarenai_flag = 7;
        combo_rp_clear_check(wk->wu.id);
        return;
    }

    if (wk->wu.cmwk[14] > 0 && --wk->wu.cmwk[14] == 0) {
        char_move_wca(&wk->wu);
    }

    if (!(wk->spmv_ng_flag & DIP_AUTO_AIR_RECOVERY_DISABLED) && wk->wu.mvxy.a[1].real.h < -2) {
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 23;
        wk->wu.routine_no[3] = 1;
        exset_char_move_init(&wk->wu, wk->wu.now_koc, dm17_to_nm23_change[wk->player_number]);
    }

    wk->tsukamarenai_flag = 7;
}

void Damage_17000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        check_dmpat_to_dmpat(wk);
        buttobi_add_y_check(wk);
        setup_butt_own_data(&wk->wu);
        cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
        get_sky_dm_timer(wk);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        wk->wu.cmwk[14] = _damage_pause_table[3][wk->wu.dm_attlv];
        /* fallthrough */

    case 2:
        fly_damage_17000(wk);
        break;

    case 3:
        char_move(&wk->wu);
        wk->guard_flag = 0;
        break;
    }
}

void Damage_18000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        check_dmpat_to_dmpat(wk);
        buttobi_add_y_check(wk);
        setup_butt_own_data(&wk->wu);
        cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
        get_sky_dm_timer(wk);

        if (wk->wu.dm_attribute) {
            setup_accessories(wk, wk->wu.pat_status);

            if (wk->wu.dm_attribute != 2) {
                effect_D9_init(wk, (u8)wk->wu.dm_attribute);
            }
        }

        break;

    case 1:
        if (setup_kuuchuu_nmdm(wk)) {
            break;
        }

        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        set_dm_hos_flag_sky(wk);
        first_flight_union(wk, 3, 3);
        break;

    case 3:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

void Damage_19000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.dm_rl = ((WORK*)wk->wu.dmg_adrs)->rl_flag;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        check_dmpat_to_dmpat(wk);
        buttobi_add_y_check(wk);
        setup_butt_own_data(&wk->wu);
        cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], 0);
        get_sky_dm_timer(wk);
        break;

    case 1:
        if (setup_kuuchuu_nmdm(wk)) {
            break;
        }

        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        set_dm_hos_flag_sky(wk);
        first_flight_union(wk, 3, 3);
        break;

    case 3:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

static void run_flight_damage_sequence(PLW* wk, s32 flight_arg) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.dm_rl = ((WORK*)wk->wu.dmg_adrs)->rl_flag;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        setup_butt_own_data(&wk->wu);
        buttobi_add_y_check(wk);
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        check_dmpat_to_dmpat(wk);
        get_sky_dm_timer(wk);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        set_dm_hos_flag_sky(wk);
        first_flight_union(wk, 3, flight_arg);
        break;

    case 3:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

void Damage_20000(PLW* wk) {
    run_flight_damage_sequence(wk, 4);
}

void Damage_21000(PLW* wk) {
    run_zuru_damage_sequence(wk, 2);
}

void Damage_23000(PLW* wk) {
    run_flight_damage_sequence(wk, 2);
}

void Damage_24000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        wk->dm_step_tbl = _dm_step_data[_select_hit_dsd[wk->wu.dm_impact][get_weight_point(&wk->wu)]];

        if (is_mid_body_hit_on_0x44(wk)) {
            set_char_move_init(&wk->wu, 1, 0x45);
        } else {
            wk->zuru_timer = 0;
            wk->zuru_ix_counter = 0;
            set_char_move_init(&wk->wu, 1, wk->as->char_ix);
        }

        break;

    case 1:
        wk->wu.routine_no[3]++;
        wk->wu.cmwk[14] = _damage_pause_table[0][wk->wu.dm_attlv];
        char_move_wca(&wk->wu);
        add_dm_step_tbl(wk, 1);
        break;

    case 2:
        add_dm_step_tbl(wk, 1);

        if (--wk->wu.cmwk[14] <= 0) {
            wk->wu.routine_no[3]++;
            char_move_wca(&wk->wu);
            break;
        }

        /* fallthrough */

    default:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            wk->wu.routine_no[2] = 0;
            wk->wu.routine_no[3] = 1;
        }

        break;
    }
}

/* Mashing shortens the stun: the lever-and-button count both takes time off
 * the timer and advances the animation by several frames at once. */
static void shake_off_kizetsu(PLW* wk) {
    s16 i;
    s16 hok;

    if ((pcon_dp_flag != 0) && (wk->py->time > 48)) {
        wk->py->time = 48;
    }

    wk->py->time -= wk->cp->lgp / 2;

    if (wk->cp->lgp > 13) {
        hok = 5;
    } else {
        hok = hok_table[wk->cp->lgp / 2];
    }

    for (i = 0; i < hok; i++) {
        char_move(&wk->wu);
    }

    setup_kuzureochi(wk);
}

void Damage_25000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 1, wk->as->char_ix);
        wk->py->flag = 0;
        wk->py->time = kizetsu_timer_table[(wk->kizetsu_kow & 0xF8) / 8][(wk->kizetsu_kow & 7) / 2][random_16()];
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        SDL_zerop(wk->rp);
        check_em_tk_power_off(wk, (PLW*)wk->wu.target_adrs);
        grade_add_em_stun((wk->wu.id + 1) & 1);
        break;

    case 1:
        shake_off_kizetsu(wk);
        break;
    }

    if (wk->wu.cg_se) {
        pulpul_request(wk->wu.id, 48);
        wk->wu.cg_se = 0;
    }
}

/* The launch: the damage pattern, the reduced fall acceleration and a
 * horizontal speed taken from the attack's move_power, clamped at both ends. */
static void begin_damage_26000(PLW* wk) {
    wk->wu.routine_no[3]++;
    set_char_move_init(&wk->wu, 6, wk->as->char_ix);
    check_dmpat_to_dmpat(wk);
    buttobi_add_y_check(wk);
    setup_butt_own_data(&wk->wu);
    wk->wu.mvxy.d[1].sp = (wk->wu.mvxy.d[1].sp * 80) / 100;
    cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], 0);
    wk->wu.mvxy.a[0].real.h = wk->move_power;
    wk->wu.mvxy.a[0].real.l = 0;
    wk->wu.mvxy.a[0].sp *= 3;
    wk->wu.mvxy.a[0].sp /= 4;
    wk->wu.mvxy.d[0].sp = 0;

    if (wk->wu.mvxy.a[0].real.h > 4) {
        wk->wu.mvxy.a[0].real.h = 4;
    }

    if (wk->wu.mvxy.a[0].real.h <= 0) {
        wk->wu.mvxy.a[0].real.h = 1;
    }

    get_sky_dm_timer(wk);
}

void Damage_26000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_damage_26000(wk);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        set_dm_hos_flag_sky(wk);
        first_flight_union(wk, 3, 3);

        if (wk->wu.routine_no[3] == 3 && wk->player_number == 8) {
            wk->wu.rl_flag = (wk->wu.rl_flag + 1) & 1;
        }

        break;

    case 3:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

void Damage_27000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->dm_ix = wk->as->char_ix + wk->wu.dm_attlv;
        set_char_move_init(&wk->wu, 1, wk->dm_ix);
        setup_butt_own_data(&wk->wu);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    default:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

void Damage_28000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        buttobi_add_y_check(wk);
        setup_butt_own_data(&wk->wu);
        cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
        get_sky_dm_timer(wk);
        break;

    case 1:
        set_dm_hos_flag_sky(wk);
        first_flight_union(wk, 2, 3);
        break;

    case 2:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

void Damage_29000(PLW* wk) {
    PLW* twk = (PLW*)wk->wu.target_adrs;
    const u16* datadrs;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.dm_rl = twk->wu.rl_flag;

        if (wk->dm_point > 2) {
            wk->wu.routine_no[2] = wk->as->data_ix;
            plpdm_lv_00[wk->wu.routine_no[2]](wk);
            break;
        }

        wk->wu.routine_no[3]++;
        datadrs = exdm_ix_data[wk->wu.dm_exdm_ix][wk->player_number];

        if (twk->wu.rl_flag) {
            wk->wu.xyz[0].disp.pos = twk->wu.xyz[0].disp.pos - datadrs[0];
        } else {
            wk->wu.xyz[0].disp.pos = twk->wu.xyz[0].disp.pos + datadrs[0];
        }

        wk->wu.xyz[1].disp.pos = twk->wu.xyz[1].disp.pos + datadrs[1];
        wk->wu.rl_flag = (wk->wu.dm_rl + datadrs[2]) & 1;
        wk->wu.cg_olc_ix = datadrs[3];
        wk->wu.cg_olc = wk->wu.olc_ix_table[wk->wu.cg_olc_ix];
        wk->wu.cg_number = datadrs[4];
        wk->wu.cg_ctr = 0xFA;
        wk->wu.cg_flip = 0;
        wk->wu.cg_type = 0;
        wk->wu.cg_hit_ix = 0;
        wk->wu.cg_ja = wk->wu.hit_ix_table[wk->wu.cg_hit_ix];
        set_jugde_area(&wk->wu);
        break;

    case 1:
        wk->wu.routine_no[2] = wk->as->data_ix;
        wk->wu.routine_no[3]++;

        if (wk->wu.routine_no[2] == 18) {
            set_char_move_init(&wk->wu, 6, wk->as->char_ix);
            char_move_wca_init(&wk->wu);
            buttobi_add_y_check(wk);
            setup_butt_own_data(&wk->wu);
            cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
        } else {
            setup_butt_own_data(&wk->wu);
            set_char_move_init(&wk->wu, 6, wk->as->char_ix);
            char_move_wca_init(&wk->wu);
            buttobi_add_y_check(wk);
        }

        get_sky_dm_timer(wk);
        plpdm_lv_00[wk->wu.routine_no[2]](wk);
        break;
    }
}

/* Hitting the wall: the work is handed to state 18 with a fresh arc, the
 * attribute effects are replayed, and the screen shakes. */
static void bounce_damage_30000(PLW* wk) {
    wk->wu.routine_no[2] = 18;
    wk->wu.routine_no[3] = 1;
    set_char_move_init(&wk->wu, 6, wk->as->data_ix);
    wk->wu.dm_butt_type++;
    setup_butt_own_data(&wk->wu);
    cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->data_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
    get_sky_dm_timer(wk);

    if (wk->wu.dm_attribute) {
        setup_accessories(wk, wk->wu.pat_status);

        if (wk->wu.dm_attribute != 2) {
            effect_D9_init(wk, (u8)wk->wu.dm_attribute);
        }
    }

    wk->wu.hit_stop = 3;
    wk->wu.hit_quake = 0;
    bg_w.quake_x_index = 6;
    pp_screen_quake(bg_w.quake_x_index);
    effect_I3_init(&wk->wu, 1);
    subtract_cu_vital(wk);
}

void Damage_30000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.dm_rl = ((WORK*)wk->wu.dmg_adrs)->rl_flag;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        check_dmpat_to_dmpat(wk);
        buttobi_add_y_check(wk);
        setup_butt_own_data(&wk->wu);
        cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], 0);
        break;

    case 1:
        if (setup_kuuchuu_nmdm(wk)) {
            break;
        }

        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        set_dm_hos_flag_sky(wk);
        first_flight_union(wk, 3, 3);

        if (wk->wu.routine_no[3] == 3 || !wk->hos_fi_flag) {
            break;
        }

        bounce_damage_30000(wk);
        break;

    case 3:
        char_move(&wk->wu);
        buttobi_chakuchi_cg_type_check(wk);
        break;
    }
}

/* The spin: every other frame advances the animation, and when the timer runs
 * out the work is handed to state 18 as a fresh fall. */
static void spin_damage_31000(PLW* wk) {
    if (wk->wu.dir_timer & 1) {
        char_move(&wk->wu);
    }

    wk->wu.cg_hit_ix = 1;
    wk->wu.cg_ja = wk->wu.hit_ix_table[1];
    set_jugde_area(&wk->wu);

    if (--wk->wu.dir_timer >= 0) {
        return;
    }

    set_char_move_init(&wk->wu, 6, 17);
    wk->wu.cg_wca_ix++;
    char_move_wca(&wk->wu);
    wk->wu.routine_no[2] = 18;
    wk->wu.routine_no[3] = 2;
    setup_butt_own_data(&wk->wu);
    cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
    get_sky_dm_timer(wk);
}

void Damage_31000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.dm_rl = ((WORK*)wk->wu.dmg_adrs)->rl_flag;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;

        if (wk->wu.xyz[1].disp.pos <= 0) {
            wk->wu.xyz[1].disp.pos = 1;
        }

        set_char_move_init(&wk->wu, 6, 10);
        setup_butt_own_data(&wk->wu);
        get_sky_dm_timer(wk);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        set_dm_hos_flag_sky(wk);
        first_flight_union(wk, 3, 3);

        if (wk->wu.routine_no[3] != 3) {
            break;
        }

        wk->wu.dir_timer = 10;
        wk->wu.cg_hit_ix = 1;
        wk->wu.cg_ja = wk->wu.hit_ix_table[1];
        set_jugde_area(&wk->wu);
        break;

    case 3:
        spin_damage_31000(wk);
        break;
    }
}

