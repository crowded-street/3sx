/**
 * @file plpdm_states_late.c
 * The later Damage_* hit-reaction states, from the stun on.
 *
 * Split out of plpdm_states.c, which was 1036 lines with 39 functions - itself
 * a split of plpdm.c. Damage_25000 onwards moved here whole, with the helpers
 * only they reach. No static crossed the boundary in either direction and
 * nothing changed its linkage; plpdm.c's table still indexes every one of them
 * by reaction number.
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
