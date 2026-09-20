/**
 * @file plpat09.c
 * Oro Attacks
 */

#include "sf33rd/Source/Game/engine/plpat09.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff13.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

/* Land, face the way the move was buffered, and start the level-5 animation.
 * Four of this character's attacks open with exactly these four lines. */
static void begin_pl09_attack(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = wk->wu.rl_waza;
    hoken_muriyari_chakuchi(wk);
    set_char_move_init(&wk->wu, 5, wk->as->char_ix);
}

void mvxy_table_reader(PLW* wk);

const u8 tenguiwa_stand_by[2][8] = { { 24, 25, 26, 27, 28, 29, 30, 30 }, { 31, 32, 33, 34, 35, 34, 33, 31 } };

const s16 tenguiwa_pos_hosei[4][6] = {
    { 8, 112, 2, 4, 64, 48 }, { 48, 104, -2, 40, 52, 96 }, { -48, 100, 2, -40, 56, 144 }, { -8, 96, -2, 0, 24, 192 }
};

const s16 tenguiwa_pos_hosei2[8][6] = { { 72, 100, 2, 68, 80, 48 },   { 32, 132, -2, 32, 56, 96 },
                                        { 8, 112, 2, 4, 76, 144 },    { -32, 130, -2, -32, 52, 192 },
                                        { -64, 100, 2, -64, 78, 48 }, { 16, 144, -2, 16, 96, 96 },
                                        { -8, 144, 2, -8, 96, 144 },  { 0, 160, -2, 0, 104, 192 } };

const s16 pl09_tk_table[14] = { 0, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700 };

const s16 homing_hos[2][20][2] = { { { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 },
                                     { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 },
                                     { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 }, { 0, 128 } },
                                   { { 24, 86 }, { 28, 68 }, { 16, 62 }, { 16, 52 }, { 20, 72 }, { 20, 58 }, { 18, 82 },
                                     { 18, 52 }, { 28, 45 }, { 25, 42 }, { 16, 52 }, { 16, 62 }, { 16, 62 }, { 24, 86 },
                                     { 16, 62 }, { 16, 62 }, { 16, 62 }, { 24, 86 }, { 20, 58 }, { 20, 72 } } };

const s16 homing_kop[2][4] = { { 1, 14, 0, 2 }, { 0, 14, 0, 2 } };

void (*const pl09_exatt_table[18])(PLW*);

void pl09_extra_attack(PLW* wk) {
    pl09_exatt_table[wk->wu.routine_no[2] - 16](wk);
}

/* Marker 20 loads the next movement row. Att_SP_YAGYOUDAMA's airborne and
 * grounded arms wrote this identically; its case 1 is a near miss - it steps the
 * state as well - and stays inline. */
static void take_next_row_on_marker_20(PLW* wk) {
    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.cg_type = 0;
        wk->wu.mvxy.index++;
    }
}

/* Airborne: marker 1 drops back to the ground state instead of stepping the
 * union. Its `break` left the switch with nothing after it. */
static void yagyoudama_airborne(PLW* wk) {
    take_next_row_on_marker_20(wk);

    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 3;
        return;
    }

    jumping_union_process(&wk->wu, 3);
}

/* Grounded: marker 1 sends it back into the air. */
static void yagyoudama_grounded(PLW* wk) {
    char_move(&wk->wu);

    take_next_row_on_marker_20(wk);

    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 2;
    }
}

void Att_SP_YAGYOUDAMA(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_pl09_attack(wk);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3]++;
        }

        break;

    case 2:
        yagyoudama_airborne(wk);
        break;

    case 3:
        yagyoudama_grounded(wk);
        break;
    }
}

/* Spawning one tenguiwa set: the rocks themselves, then the shells the player
 * owns are walked and the first few given their positions from the set's table.
 *
 * The stand-by set and the EX set differed in four things - which stand-by row
 * the rocks come from, how many rocks, which position table, and which slot is
 * the last - and every one of them is written out at its own call site and only
 * counted or indexed here. That is Recipe T's case.
 *
 * The two shell guards were written differently, `continue` on one side and a
 * nested `if` on the other. They are the same loop; this is the `continue`
 * form. */
static void place_tenguiwa_set(PLW* wk, const TenguiwaSet* set) {
    s16 i;
    s16 j;
    u16 num;
    WORK* tmw;

    for (i = 0; i < set->rock_count; i++) {
        effect_13_init(&wk->wu, set->tengu[random_16() & 7]);
    }

    for (j = 0, i = 0; i < 8; i++) {
        if (!get_my_shell_ix(&wk->wu, i, &tmw)) {
            continue;
        }

        num = tmw->type - 24;

        if (!(num < 36)) {
            continue;
        }

        tmw->old_pos[0] = set->pos[j][0];
        tmw->old_pos[1] = set->pos[j][1];
        tmw->old_pos[2] = set->pos[j][2];
        tmw->scr_mv_x = set->pos[j][3];
        tmw->scr_mv_y = set->pos[j][4];
        tmw->direction = set->pos[j][5];

        j++;

        if (j > set->last_slot) {
            break;
        }
    }
}

s32 set_tenguiwa(PLW* wk, u8 data) {
    if (!data) {
        place_tenguiwa_set(wk, &(TenguiwaSet){ tenguiwa_stand_by[0], 3, tenguiwa_pos_hosei, 2 });
        return 0;
    }

    place_tenguiwa_set(wk, &(TenguiwaSet){ tenguiwa_stand_by[1], 5, tenguiwa_pos_hosei2, 4 });
    return 0;
}

/* The taunt itself. Marker 40 pays the super-art gauge, marker 20 counts another
 * success up to 13, marker 30 ends it. While it runs, the stun timer recovers at
 * the rate that count selects. The 30 arm's `break` left the switch with nothing
 * after it, so a return is the same exit. */
static void tokushu_taunt_frames(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 40) {
        wk->wu.cg_type = 0;
        add_sp_arts_gauge_tokushu(wk);
    }

    if (wk->wu.cg_type == 20) {
        wk->wu.cg_type = 0;

        if (++wk->tk_success > 13) {
            wk->tk_success = 13;
        }
    }

    if (wk->wu.cg_type == 30) {
        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
        return;
    }

    wk->py->now.timer -= wk->py->recover * pl09_tk_table[wk->tk_success] / 100;

    if (wk->py->now.quantity.h <= 0) {
        wk->py->now.timer = 0;
    }
}

void Att_PL09_TOKUSHUKOUDOU(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->tk_success = 0;
        wk->wu.rl_flag = wk->wu.rl_waza;
        hoken_muriyari_chakuchi(wk);
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        tokushu_taunt_frames(wk);
        break;

    default:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 64) {
            grade_add_personal_action(wk->wu.id);
        }

        break;
    }
}

/* The grounded frames: marker 1 sends the move back into the air, to state 2 on
 * the first pass and state 3 afterwards. Case 3 is the mirror of this and is
 * deliberately left inline - extracting it too would make a twin pair, and one
 * extraction already takes the caller under the threshold. */
static void jinnchuu_ex_grounded(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = (wk->wu.routine_no[1] == 0) ? 2 : 3;
    }
}

void Att_JINNCHUUWATARI_EX(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_pl09_attack(wk);
        wk->pl09_dat_index = wk->as->r_no;
        wk->wu.mvxy.index = wk->as->data_ix;
        break;

    case 1:
        char_move(&wk->wu);
        mvxy_table_reader(wk);
        break;

    case 2:
        mvxy_table_reader(wk);
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3] = (wk->wu.routine_no[1] == 0) ? 2 : 4;
            break;
        }

        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        jinnchuu_ex_grounded(wk);
        break;
    }
}

/* Both kop arms finish the aim the same way: take the height from the row, clear
 * the horizontal speed, solve the arc and step to the next row. Only the x they
 * solved for differs, and it is passed in. */
static void aim_at_homing_height(PLW* wk, const PLW* twk, const s16* curr_kop, s16 ex) {
    s16 ey = homing_hos[wk->pl09_dat_index][twk->player_number][1];

    wk->wu.mvxy.a[0].sp = 0;
    cal_initial_speed(&wk->wu, curr_kop[1], ex, ey);
    wk->pl09_dat_index++;
}

/* kop 0 aims at the opponent, offset by the row for that character, and mirrors
 * the result back across the player when the facing does not match the side the
 * opponent is on. kop 1's midpoint form is the near twin of this and stays
 * inline: one extraction already takes the caller under the threshold. */
/* The opponent is to the right: aim short of them by the row's offset, and mirror
 * that back across the player when the facing does not match.
 *
 * Only this arm is lifted. Its mirror stays inline: the two differ in the sign of
 * both operations and in which way rl_flag is tested, so as two functions they
 * would be a duplication pair, and one arm is enough to clear the bump. */
static s16 homing_target_x_from_left(const PLW* wk, const PLW* twk) {
    s16 ex = twk->wu.xyz[0].disp.pos - homing_hos[wk->pl09_dat_index][twk->player_number][0];

    if (!wk->wu.rl_flag) {
        ex = wk->wu.xyz[0].disp.pos - (ex - wk->wu.xyz[0].disp.pos);
    }

    return ex;
}

static s16 homing_target_x(const PLW* wk, const PLW* twk) {
    s16 ex;

    if (wk->wu.xyz[0].disp.pos < twk->wu.xyz[0].disp.pos) {
        return homing_target_x_from_left(wk, twk);
    }

    ex = twk->wu.xyz[0].disp.pos + homing_hos[wk->pl09_dat_index][twk->player_number][0];

    if (wk->wu.rl_flag) {
        ex = wk->wu.xyz[0].disp.pos + (wk->wu.xyz[0].disp.pos - ex);
    }

    return ex;
}

/* Marker 30 aims the homing jump. kop 0 targets the opponent at the row's offset
 * and mirrors the result when the player faces the other way; kop 1 aims at the
 * midpoint between the two. Either way the facing correction and the state step
 * follow. The target work and the kop row are passed in so they are read once,
 * as the original read them. */
static void homing_aim_on_marker_30(PLW* wk, PLW* twk, const s16* curr_kop) {
    s16 ex;
    s16 ey;

    if (wk->wu.cg_type != 30) {
        return;
    }

    setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
    wk->wu.mvxy.index++;

    switch (curr_kop[0]) {
    case 0:
        ex = homing_target_x(wk, twk);
        aim_at_homing_height(wk, twk, curr_kop, ex);
        break;

    case 1:
        ex = wk->wu.xyz[0].disp.pos;

        if (wk->wu.xyz[0].disp.pos < twk->wu.xyz[0].disp.pos) {
            ex += (twk->wu.xyz[0].disp.pos - wk->wu.xyz[0].disp.pos) / 2;
        } else {
            ex -= (wk->wu.xyz[0].disp.pos - twk->wu.xyz[0].disp.pos) / 2;
        }

        aim_at_homing_height(wk, twk, curr_kop, ex);
        break;
    }

    if (wk->wu.rl_flag == 0) {
        wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
        wk->wu.mvxy.d[0].sp = -wk->wu.mvxy.d[0].sp;
    }

    wk->wu.routine_no[3]++;
    wk->wu.cg_type = 0;
    add_mvxy_speed(&wk->wu);
}

void mvxy_table_reader(PLW* wk) {
    PLW* twk = (PLW*)wk->wu.target_adrs;
    const s16* curr_kop = &homing_kop[wk->pl09_dat_index][0];

    homing_aim_on_marker_30(wk, twk, curr_kop);

    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
        add_mvxy_speed(&wk->wu);
    }
}

void Att_PL09_EX_TENGUIWA(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_pl09_attack(wk);
        wk->sa->dtm_mul = 2;
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Att_PL09_EX_KISHINRIKI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_pl09_attack(wk);
        reset_mvxy_data(&wk->wu);
        wk->wu.mvxy.index = wk->as->r_no;
        wk->sa->dtm_mul = 16;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            add_mvxy_speed(&wk->wu);
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

void (*const pl09_exatt_table[18])(PLW*) = {
    Att_HADOUKEN,      Att_SHOURYUUKEN,      Att_KUUCHUUNICHIRINSHOU,   Att_HADOUKEN,       Att_HADOUKEN,
    Att_HADOUKEN,      Att_HADOUKEN,         Att_KUUCHUUJINNCHUUWATARI, Att_JINNCHUUWATARI, Att_JINNCHUUWATARI_EX,
    Att_SP_YAGYOUDAMA, Att_PL09_EX_TENGUIWA, Att_PL09_EX_KISHINRIKI,    Att_DUMMY,          Att_PL09_TOKUSHUKOUDOU,
    Att_DUMMY,         Att_METAMOR_WAIT,     Att_METAMOR_REBIRTH
};
