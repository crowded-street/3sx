/**
 * @file plpat17.c
 * Makoto Attacks
 */

#include "sf33rd/Source/Game/engine/plpat17.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effi3.h"
#include "sf33rd/Source/Game/effect/effl8.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

void set_kabe_move_spd(WORK* wk, s16 tm);
s32 kabe_check(WORK* wk);

void (*const pl17_exatt_table[18])(PLW*);

void pl17_extra_attack(PLW* wk) {
    pl17_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

/* Marker 21 resets the movement data and restarts the attack at state 2. Three
 * arms of Att_PL17_AT1 wrote this identically. */
static void restart_at_state_2_on_marker_21(PLW* wk) {
    if (wk->wu.cg_type == 21) {
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 2;
    }
}

/* Marker 20 takes the next movement row and hands the attack to state 3. Two
 * arms wrote this identically; the marker-30 blocks beside them are near misses
 * - one assigns state 4 and the other does not - and stay inline. */
static void take_row_and_enter_state_3(PLW* wk) {
    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 3;
        wk->wu.cg_type = 0;
    }
}

/* Outside the bonus stage, or in it but not on the car, the attack starts from
 * the floor. */
static s32 not_riding_the_bonus_car(const PLW* wk) {
    return Bonus_Game_Flag != 20 || (Bonus_Game_Flag == 20 && wk->bs2_on_car == 0);
}

/* The opening frame. Outside the bonus-stage car the attack always starts from
 * the floor. */
static void begin_pl17_at1(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = wk->wu.rl_waza;
    wk->scr_pos_set_flag = 0;
    reset_mvxy_data(&wk->wu);
    setup_mvxy_data(&wk->wu, wk->as->r_no);
    wk->wu.mvxy.index = wk->as->data_ix;

    if (not_riding_the_bonus_car(wk)) {
        wk->wu.xyz[1].disp.pos = 0;
    }

    set_char_move_init(&wk->wu, 5, wk->as->char_ix);
    set_kabe_move_spd(&wk->wu, 28);
    wk->rl_save = 0;
}

/* The wind-up: marker 10 launches into state 3 with its effect. */
static void pl17_at1_wind_up(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 10) {
        wk->wu.routine_no[3] = 3;
        effect_I3_init(&wk->wu, 2);
    }

    if (wk->wu.routine_no[3] != 1) {
        add_mvxy_speed(&wk->wu);
    }
}

/* The grounded markers: 20 hands over to state 3, 30 kills the vertical speed
 * and goes to state 4. */
static void pl17_at1_ground_markers(PLW* wk) {
    char_move(&wk->wu);

    take_row_and_enter_state_3(wk);

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 4;
        wk->wu.cg_type = 0;
    }

    if (wk->wu.routine_no[3] != 2) {
        add_mvxy_speed(&wk->wu);
    }
}

/* The union leg's markers, which only run while it has not returned to state 2. */
static void pl17_at1_union_markers(PLW* wk) {
    jumping_union_process(&wk->wu, 2);

    if (wk->wu.routine_no[3] != 2) {
        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        restart_at_state_2_on_marker_21(wk);

        if (wk->wu.cg_type == 25) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = 5;
        }
    }
}

/* The airborne markers. Its marker-30 block is a near miss of the grounded one -
 * that one assigns state 4 as well - so the two stay apart. */
static void pl17_at1_air_markers(PLW* wk) {
    char_move(&wk->wu);
    cal_mvxy_speed(&wk->wu);
    add_mvxy_speed(&wk->wu);

    take_row_and_enter_state_3(wk);

    restart_at_state_2_on_marker_21(wk);

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }
}

/* A wall bounce saved on an earlier frame re-centres the player 142 either side
 * of the stage centre, on the side they face, and restarts the attack. */
static void recentre_after_wall_bounce(PLW* wk) {
    if (wk->rl_save) {
        wk->rl_save = 0;
        wk->wu.routine_no[3] = 2;
        wk->wu.xyz[0].disp.pos = get_center_position();

        if (wk->wu.rl_flag) {
            wk->wu.xyz[0].disp.pos -= 142;
        } else {
            wk->wu.xyz[0].disp.pos += 142;
        }
    }
}

/* The wall phase: the union's markers, the saved wall bounce that re-centres the
 * player 142 either side of the stage centre, and the wall test that sets it. */
static void pl17_at1_wall_phase(PLW* wk) {
    jumping_union_process(&wk->wu, 2);

    if (wk->wu.routine_no[3] != 2) {
        restart_at_state_2_on_marker_21(wk);

        if (wk->wu.cg_type == 26) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = 3;
        }

        if (wk->wu.cg_type == 30) {
            wk->wu.cg_type = 0;
            wk->wu.mvxy.d[0].sp = 0;
        }

        recentre_after_wall_bounce(wk);
    }

    if ((wk->wu.routine_no[3] == 5) && (kabe_check(&wk->wu))) {
        char_move_cmj4(&wk->wu);
        reset_mvxy_data(&wk->wu);
        wk->rl_save = 1;
    }
}

void Att_PL17_AT1(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_pl17_at1(wk);
        break;

    case 1:
        pl17_at1_wind_up(wk);
        break;

    case 2:
        pl17_at1_ground_markers(wk);
        break;

    case 3:
        pl17_at1_union_markers(wk);
        break;

    case 4:
        pl17_at1_air_markers(wk);
        break;

    case 5:
        pl17_at1_wall_phase(wk);
        break;
    }
}

void set_kabe_move_spd(WORK* wk, s16 tm) {
    s16 tar_pos;

    tar_pos = get_center_position();

    if (wk->rl_flag) {
        tar_pos -= 192;
    } else {
        tar_pos += 192;
    }

    cal_all_speed_data(wk, &(Motion_Target) { tm, tar_pos, wk->xyz[1].disp.pos + 120, 2, 2 });

    if (!wk->rl_flag) {
        wk->mvxy.a[0].sp = -wk->mvxy.a[0].sp;
        wk->mvxy.d[0].sp = -wk->mvxy.d[0].sp;
    }

    wk->mvxy.kop[0] = 1;
}

s32 kabe_check(WORK* wk) {
    s16 tar_pos;

    if (wk->xyz[1].disp.pos < 85) {
        return 0;
    }

    tar_pos = get_center_position();

    if (wk->rl_flag) {
        tar_pos -= 142;

        if (!(wk->xyz[0].disp.pos > tar_pos)) {
            wk->xyz[0].disp.pos = tar_pos;
            return 1;
        }

        return 0;
    }

    tar_pos += 142;

    if (wk->xyz[0].disp.pos >= tar_pos) {
        wk->xyz[0].disp.pos = tar_pos;
        return 1;
    }

    return 0;
}

void Att_PL17_AT2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 10) {
            wk->wu.cg_type = 0;
            effect_L8_init(wk);
        }

        break;
    }
}

/* The taunt's markers: 40 pays the super-art gauge, 10 and 20 each add ten to the
 * damage bonus against their own ceiling, and 30 ends the taunt and slows the
 * stun recovery, up to three times. The 10 and 20 arms differ in their ceiling
 * and in whether the personal action is graded - two differences, so they stay
 * as they are. */
static void pl17_taunt_markers(PLW* wk) {
    char_move(&wk->wu);

    switch (wk->wu.cg_type) {
    case 40:
        wk->wu.cg_type = 0;
        add_sp_arts_gauge_tokushu(wk);
        break;

    case 10:
        wk->wu.cg_type = 0;
        wk->tk_dageki += 10;

        if (wk->tk_dageki > 10) {
            wk->tk_dageki = 10;
        }

        grade_add_personal_action(wk->wu.id);
        break;

    case 20:
        wk->wu.cg_type = 0;
        wk->tk_dageki += 10;

        if (wk->tk_dageki > 20) {
            wk->tk_dageki = 20;
        }

        break;

    case 30:
        wk->wu.routine_no[3]++;

        if (wk->tk_success < 3) {
            wk->tk_success++;
            wk->py->recover = (wk->py->recover * 110) / 100;
        }

        break;
    }
}

void Att_PL17_TOKUSHUKOUDOU(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        pl17_taunt_markers(wk);
        break;

    default:
        char_move(&wk->wu);
        break;
    }
}

void (*const pl17_exatt_table[18])(PLW*) = {
    Att_CHOUCHUURENGEKI, Att_PL17_AT1,     Att_HADOUKEN2,      Att_PL17_AT2, Att_KUUCHUUJINNCHUUWATARI,
    Att_DUMMY,           Att_DUMMY,        Att_DUMMY,          Att_DUMMY,    Att_DUMMY,
    Att_DUMMY,           Att_DUMMY,        Att_DUMMY,          Att_DUMMY,    Att_PL17_TOKUSHUKOUDOU,
    Att_DUMMY,           Att_METAMOR_WAIT, Att_METAMOR_REBIRTH
};
