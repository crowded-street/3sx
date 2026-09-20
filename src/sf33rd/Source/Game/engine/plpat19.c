/**
 * @file plpat19.c
 * Twelve Attacks
 */

#include "sf33rd/Source/Game/engine/plpat19.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effi3.h"
#include "sf33rd/Source/Game/effect/effk7.h"
#include "sf33rd/Source/Game/effect/effl0.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"

s32 kabe_check3(PLW* wk);
u8 get_lever_dir(PLW* wk);

void (*const pl19_exatt_table[18])(PLW*);

void pl19_extra_attack(PLW* wk) {
    pl19_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

/* The transformation starts only if its effect could be spawned; outside the
 * bonus stage that is what decides between the full animation and the two-frame
 * fallback at state 9. */
static void begin_metamorphose(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = wk->wu.rl_waza;
    hoken_muriyari_chakuchi(wk);
    reset_mvxy_data(&wk->wu);
    wk->metamorphose = 0;
    wk->metamor_over = 0;

    if ((Bonus_Game_Flag != 20) && (effect_K7_init(wk) != -1)) {
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        return;
    }

    set_char_move_init(&wk->wu, 5, wk->as->char_ix + 2);
    wk->wu.routine_no[3] = 9;
}

void Att_METAMORPHOSE(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_metamorphose(wk);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[2] = 32;
            wk->wu.routine_no[3] = 1;
        }

        break;

    case 9:
        char_move(&wk->wu);
        break;
    }
}

const s16 dra_em_tall[20][2] = { { 24, 16 }, { 28, 16 }, { 16, 16 }, { 16, 16 }, { 20, 16 }, { 20, 16 }, { 18, 16 },
                                 { 18, 16 }, { 28, 16 }, { 25, 16 }, { 16, 16 }, { 16, 16 }, { 16, 16 }, { 24, 16 },
                                 { 16, 16 }, { 16, 16 }, { 16, 16 }, { 24, 16 }, { 20, 16 }, { 20, 16 } };

/* The animation's 20 marker hands over the next movement row and steps the index
 * on. Three travelling arms wrote this identically. */
static void take_next_mvxy_row(PLW* wk) {
    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }
}

/* Either end marker stops the horizontal travel and steps the state on. Two arms
 * wrote this identically; Att_SA__D_R_A's version is left alone because it
 * assigns state 5 outright instead of stepping, a second difference. */
static void finish_on_end_marker(PLW* wk) {
    if (wk->wu.cg_type == 64 || wk->wu.cg_type == 0xFF) {
        wk->wu.routine_no[3]++;
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.a[0].sp = 0;
    }
}

/* The airborne leg of a flying attack. While jumping_union_process has not yet
 * handed over to the landed state it keeps feeding the next movement row on the
 * animation's 20 marker; once it has, the vertical speed is flattened and a
 * kop of 2 is stepped back to 1.
 *
 * Four arms across Att_SA__D_R_A, Att_EX__D_R_A and Att_KUUCHUUHISSATU wrote
 * this out, differing only in which state counts as landed - one value, which
 * is what Recipe D allows. */
static void step_union_flight(PLW* wk, s16 landed_rno) {
    if ((wk->wu.routine_no[3] != landed_rno) && (wk->wu.cg_type == 20)) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }

    if (wk->wu.routine_no[3] == landed_rno) {
        if (wk->wu.mvxy.kop[0] == 2) {
            wk->wu.mvxy.kop[0] = 1;
        }

        wk->wu.mvxy.d[1].sp = 0;
        wk->wu.mvxy.a[1].sp = 0;
    }
}

/* Marker 30 teleports the player onto the opponent, 224 above them, clamped to
 * the floor. */
static void snap_above_target(PLW* wk) {
    PLW* emwk;

    if (wk->wu.cg_type == 30) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3]++;
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        emwk = (PLW*)wk->wu.target_adrs;
        wk->wu.xyz[0].disp.pos = emwk->wu.xyz[0].disp.pos;
        wk->wu.xyz[1].disp.pos = emwk->wu.xyz[1].disp.pos + -224;

        if (wk->wu.xyz[1].disp.pos < 0) {
            wk->wu.xyz[1].disp.pos = 0;
        }
    }
}

/* The travelling frames of the super art. Three more markers can arrive here on
 * top of the shared row advance: a reset that ends the travel, the snap that
 * places the player 224 above the opponent, and either end marker. */
static void sa_dra_travel(PLW* wk) {
    char_move(&wk->wu);
    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);

    take_next_mvxy_row(wk);

    if (wk->wu.cg_type == 21) {
        reset_mvxy_data(&wk->wu);
        wk->wu.routine_no[3] = 5;
        wk->wu.cg_type = 0;
    }

    snap_above_target(wk);

    if ((wk->wu.cg_type == 64) || (wk->wu.cg_type == 0xFF)) {
        wk->wu.routine_no[3] = 5;
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.a[0].sp = 0;
    }
}

/* The lift-off frame: the 20 marker loads the first movement row and hands over
 * to the union. Unlike take_next_mvxy_row this one steps the state rather than
 * the row index. */
static void sa_dra_lift_off(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3]++;
    }
}

void Att_SA__D_R_A(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        sa_dra_lift_off(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        step_union_flight(wk, 3);

        break;

    case 3:
        sa_dra_travel(wk);
        break;

    case 4:
        jumping_union_process(&wk->wu, 5);

        step_union_flight(wk, 5);

        break;

    case 5:
        char_move(&wk->wu);
        break;

    default:
        char_move(&wk->wu);
    }
}

/* The opening frame aims the EX version at the opponent: the target offset comes
 * from dra_em_tall, and a left-facing player has both components negated after
 * the delta speed is solved. */
static void aim_ex_dra_at_target(PLW* wk) {
    PLW* twk;
    s16 ex;
    s16 ey;

    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = wk->wu.rl_waza;
    set_char_move_init(&wk->wu, 5, wk->as->char_ix);
    setup_mvxy_data(&wk->wu, wk->as->r_no);
    twk = (PLW*)wk->wu.target_adrs;

    if (wk->wu.rl_flag) {
        ex = twk->wu.position_x - dra_em_tall[twk->player_number][0];
    } else {
        ex = twk->wu.position_x + dra_em_tall[twk->player_number][0];
    }

    ey = dra_em_tall[twk->player_number][1];
    wk->wu.mvxy.a[0].sp = 0;
    cal_delta_speed(&wk->wu, &(Motion_Target) { 8, ex, ey, 2, 2 });

    if (wk->wu.rl_flag == 0) {
        wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
        wk->wu.mvxy.d[0].sp = -wk->wu.mvxy.d[0].sp;
    }
}

void Att_EX__D_R_A(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        aim_ex_dra_at_target(wk);
        break;
    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            add_mvxy_speed(&wk->wu);
            wk->wu.mvxy.kop[1] = 2;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);

        step_union_flight(wk, 3);

        break;

    case 3:
        char_move(&wk->wu);
        add_mvxy_speed(&wk->wu);
        cal_mvxy_speed(&wk->wu);

        take_next_mvxy_row(wk);

        finish_on_end_marker(wk);

        break;

    default:
        char_move(&wk->wu);
        break;
    }
}

/* The travelling frames of the aerial finisher: the shared row advance, the
 * reset marker that steps the state on, and the shared end markers. */
static void kuuchuu_travel(PLW* wk) {
    char_move(&wk->wu);
    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);

    take_next_mvxy_row(wk);

    if (wk->wu.cg_type == 21) {
        reset_mvxy_data(&wk->wu);
        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
    }

    finish_on_end_marker(wk);
}

void Att_KUUCHUUHISSATU(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        setup_mvxy_data(&wk->wu, wk->as->r_no);
        /* fallthrough */

    case 1:
        jumping_union_process(&wk->wu, 2);

        step_union_flight(wk, 2);

        break;

    case 2:
        kuuchuu_travel(wk);
        break;
    default:
        char_move(&wk->wu);
        break;
    }
}

/* The wall bounce itself: reposition against the limit the player now faces,
 * start the bounce animation, drop into state 5 and fire the effect. Both arms
 * of Att_AIRDASH wrote this run identically; what differs - the kabe_check3
 * test and the way each one flips rl_flag - stays at the call sites. */
static void bounce_off_wall(PLW* wk) {
    wk->wu.xyz[0].disp.pos = wk->wu.rl_flag ? bg_w.bgw[1].l_limit2 - 192 : bg_w.bgw[1].r_limit2 + 192;
    set_char_move_init(&wk->wu, 5, 65);
    wk->wu.routine_no[3] = 5;
    wk->wu.cg_type = 0;
    effect_I3_init(&wk->wu, 4);
}

/* The dashing frames. A wall ends the dash outright; otherwise the movement
 * markers feed the next row, add to it, or hand over to the union at state 3.
 * The wall arm's `break` left the switch with nothing after it. */
static void airdash_travel(PLW* wk) {
    char_move(&wk->wu);

    if (kabe_check3(wk) != 0) {
        wk->wu.rl_flag = (wk->wu.rl_flag + 1) & 1;
        bounce_off_wall(wk);
        return;
    }

    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);

    switch (wk->wu.cg_type) {
    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 25:
        add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 30:
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        wk->wu.routine_no[3] = 3;
        wk->wu.cg_type = 0;
        break;
    }
}

/* The union leg checks the wall too, with its own unparenthesised flip. */
static void airdash_bounce_if_walled(PLW* wk) {
    if (kabe_check3(wk)) {
        wk->wu.rl_flag = wk->wu.rl_flag + 1 & 1;
        bounce_off_wall(wk);
    }
}

void Att_AIRDASH(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        airdash_travel(wk);
        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        airdash_bounce_if_walled(wk);
        break;

    case 4:
        char_move(&wk->wu);
        break;

    case 5:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
        }

        break;

    case 6:
        wk->wu.routine_no[3] = 1;
        char_move_cmj4(&wk->wu);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        break;
    }
}

s32 kabe_check3(PLW* wk) {
    if (get_lever_dir(wk) != 1) {
        return 0;
    }

    if (wk->wu.xyz[1].disp.pos < 33) {
        return 0;
    }

    return (wk->wu.rl_flag + wk->micchaku_flag == 2);
}

/* Which personal action starts depends on what is available: no super art picks
 * animation 62, a hidden or oddly-coloured player picks 64, and everything else
 * gets the full 63. Each arm's `break` left the switch with nothing after it. */
static void begin_personal_action(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = wk->wu.rl_waza;
    hoken_muriyari_chakuchi(wk);

    if (wk->sa->ok == -1) {
        wk->wu.routine_no[3] = 2;
        set_char_move_init(&wk->wu, 5, 62);
        return;
    }

    if (wk->wu.disp_flag != 1 || wk->wu.my_col_mode != 0x4200) {
        wk->wu.routine_no[3] = 2;
        set_char_move_init(&wk->wu, 5, 64);
        return;
    }

    set_char_move_init(&wk->wu, 5, 63);
}

void Att_pl19_TOKUSHUKOUDOU(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_personal_action(wk);
        break;

    case 1:
        char_move(&wk->wu);

        switch (wk->wu.cg_type) {
        case 40:
            wk->wu.cg_type = 0;
            add_sp_arts_gauge_tokushu(wk);
            break;

        case 0xFF:
            grade_add_personal_action(wk->wu.id);
            effect_L0_init(&wk->wu, 180);
            break;
        }

        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

/* The opening frame: a non-negative vertical speed is replaced by the table's
 * row 64, shifted into place. */
static void begin_air_axe(PLW* wk) {
    wk->wu.routine_no[3]++;
    set_char_move_init(&wk->wu, 5, wk->as->char_ix);
    wk->wu.mvxy.index = wk->as->r_no;

    if (wk->wu.mvxy.d[1].sp >= 0) {
        wk->wu.mvxy.d[1].sp = wk->wu.move_xy_table[64];
        wk->wu.mvxy.d[1].sp <<= 8;
    }

    wk->wu.mvxy.kop[1] = 0;
}

/* The launch frames: marker 1 steps the state on, marker 20 takes the next row.
 * The row advance is not the shared take_next_mvxy_row - this one clears cg_type
 * before stepping the index, a different order of side effects. */
static void air_axe_launch(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 1) {
        wk->wu.routine_no[3]++;
    }

    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.cg_type = 0;
        wk->wu.mvxy.index++;
    }
}

/* The airborne markers, which only run while the union has not yet landed. */
static void air_axe_markers(PLW* wk) {
    if (wk->wu.routine_no[3] != 3) {
        if (wk->wu.cg_type == 20) {
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
        }

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->as->data_ix);
            wk->wu.cg_type = 0;
        }
    }
}

void Att_AIR_A_X_E(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_air_axe(wk);
        break;

    case 1:
        air_axe_launch(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        air_axe_markers(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

u8 get_lever_dir(PLW* wk) {
    u8 num;

    if (wk->wu.work_id == 1) {
        if (wk->py->flag == 0) {
            num = wcp[wk->wu.id].lever_dir;
        } else {
            num = 0;
        }
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].lever_dir;
    }

    return num;
}

void (*const pl19_exatt_table[18])(PLW*) = {
    Att_HADOUKEN,     Att_AIRDASH,   Att_KUUCHUUHISSATU,     Att_HADOUKEN,       Att_HADOUKEN,       Att_EX__D_R_A,
    Att_METAMORPHOSE, Att_AIR_A_X_E, Att_HADOUKEN,           Att_JINNCHUUWATARI, Att_SLIDE_and_JUMP, Att_SA__D_R_A,
    Att_DUMMY,        Att_DUMMY,     Att_pl19_TOKUSHUKOUDOU, Att_DUMMY,          Att_METAMOR_WAIT,   Att_METAMOR_REBIRTH
};
