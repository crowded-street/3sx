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

/* The knock-back arc, which one state reaches without the animation start. */
static void setup_buttobi_arc(PLW* wk) {
    buttobi_add_y_check(wk);
    setup_butt_own_data(&wk->wu);
}

/* Start the level-6 damage animation, carry any pattern-to-pattern state over,
 * and set that arc up. Two states open this way. */
static void begin_buttobi_animation(PLW* wk) {
    set_char_move_init(&wk->wu, 6, wk->as->char_ix);
    check_dmpat_to_dmpat(wk);
    setup_buttobi_arc(wk);
}

/* Solve the rise from the character's own buttobi time and start the sky timer.
 * Two states launch from the ground this way. */
static void launch_buttobi_from_ground(PLW* wk) {
    cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], 0);
    get_sky_dm_timer(wk);
}

/* Three runs the damage states write out identically; the same three the late
 * states share, kept file-local on each side because a `static` may not be
 * widened to bridge the two.
 *
 * The sky flight's hos flag and its union step. */
static void enter_sky_flight(PLW* wk) {
    set_dm_hos_flag_sky(wk);
    first_flight_union(wk, 3, 3);
}

/* Take the attacker's facing and turn away from it. */
static void face_away_from_attacker(PLW* wk) {
    wk->wu.dm_rl = ((WORK*)wk->wu.dmg_adrs)->rl_flag;
    wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
}

/* Step the state on and start the with-cancel animation. */
static void begin_wca_state(PLW* wk) {
    wk->wu.routine_no[3]++;
    char_move_wca_init(&wk->wu);
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
        face_away_from_attacker(wk);
        wk->dm_ix = wk->as->char_ix + wk->wu.dm_attlv;
        set_char_move_init(&wk->wu, 1, wk->dm_ix);
        setup_butt_own_data(&wk->wu);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->zuru_timer = 0;
        wk->zuru_ix_counter = 0;
        break;

    case 1:
        begin_wca_state(wk);
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
        setup_buttobi_arc(wk);
        launch_buttobi_from_ground(wk);
        break;

    case 1:
        begin_wca_state(wk);
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
        begin_buttobi_animation(wk);
        cal_initial_speed_y(&wk->wu, _buttobi_time_table[wk->as->char_ix][wk->wu.dm_attlv], wk->wu.xyz[1].disp.pos);
        get_sky_dm_timer(wk);
        break;

    case 1:
        begin_wca_state(wk);
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

/* The launch into state 18: the arc from the attack level, and the attribute
 * effects that go with the hit. */
static void begin_damage_18000(PLW* wk) {
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
}

void Damage_18000(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_damage_18000(wk);
        break;

    case 1:
        if (setup_kuuchuu_nmdm(wk)) {
            break;
        }

        begin_wca_state(wk);
        /* fallthrough */

    case 2:
        enter_sky_flight(wk);
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
        face_away_from_attacker(wk);
        begin_buttobi_animation(wk);
        launch_buttobi_from_ground(wk);
        break;

    case 1:
        if (setup_kuuchuu_nmdm(wk)) {
            break;
        }

        begin_wca_state(wk);
        /* fallthrough */

    case 2:
        enter_sky_flight(wk);
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
        face_away_from_attacker(wk);
        setup_butt_own_data(&wk->wu);
        buttobi_add_y_check(wk);
        set_char_move_init(&wk->wu, 6, wk->as->char_ix);
        check_dmpat_to_dmpat(wk);
        get_sky_dm_timer(wk);
        break;

    case 1:
        begin_wca_state(wk);
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
