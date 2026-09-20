/**
 * @file plpatuni.c
 * Various Attacks
 */

#include "sf33rd/Source/Game/engine/plpatuni.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plpat.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"

#include <SDL3/SDL.h>

void att_ahj_table_reader(PLW* wk);

/* Ten attack openings in this file start the same way: land, face the way the
 * move was buffered, and start the level-5 animation. What each one does next -
 * a movement row, an initial speed, nothing at all - stays at its call site. */
static void begin_uni_attack(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = wk->wu.rl_waza;
    hoken_muriyari_chakuchi(wk);
    set_char_move_init(&wk->wu, 5, wk->as->char_ix);
}

/* Four openings reset the movement data and start from a row of their own. Only
 * the row differs, which is the one value Recipe D allows as a parameter. */
static void begin_uni_attack_at_row(PLW* wk, s16 row) {
    begin_uni_attack(wk);
    reset_mvxy_data(&wk->wu);
    wk->wu.mvxy.index = row;
}

/* The two homing openings take their row without resetting first. */
static void begin_ahj_attack(PLW* wk) {
    begin_uni_attack(wk);
    wk->wu.mvxy.index = wk->as->data_ix;
}

/* Att_CHOUCHUURENGEKI and Att_SLIDE_and_JUMP open with the same six lines, in an
 * order of their own: they land before setting the facing, where the other ten
 * openings do it the other way round. That ordering is why they are not
 * begin_uni_attack call sites, and it is preserved here. */
static void begin_slide_attack(PLW* wk) {
    wk->wu.routine_no[3]++;
    hoken_muriyari_chakuchi(wk);
    wk->wu.rl_flag = wk->wu.rl_waza;
    reset_mvxy_data(&wk->wu);
    wk->wu.mvxy.index = wk->as->r_no;
    set_char_move_init(&wk->wu, 5, wk->as->char_ix);
}

void Att_DUMMY(PLW* /* unused */) {}

/* The wait ends the frame the 30 marker stops arriving, and the launch speed is
 * set up for the union. */
static void metamor_wait_launch(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type != 30) {
        wk->wu.routine_no[3] = 3;
        wk->wu.mvxy.a[0].sp = 0;
        wk->wu.mvxy.a[1].sp = 0;
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.d[1].sp = -0x8000;
        wk->wu.mvxy.kop[0] = wk->wu.mvxy.kop[1] = 0;
    }
}

void Att_METAMOR_WAIT(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 30) {
            wk->wu.routine_no[3] = 2;
        }

        break;

    case 2:
        metamor_wait_launch(wk);
        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

const s16 metareb_pos[20][2] = { { 1, 9 },  { 14, 19 }, { 5, 31 }, { 8, 24 },  { 9, 28 },  { 4, 22 }, { 6, -10 },
                                 { 6, 35 }, { 15, 24 }, { 6, 26 }, { 8, 24 },  { 5, 31 },  { 4, 32 }, { 1, 9 },
                                 { 5, 30 }, { 1, 23 },  { 6, 22 }, { 13, 25 }, { -4, 22 }, { 0, 17 } };

/* The rebirth places the player back on the ground: near the floor it drops to a
 * fixed -8, otherwise it steps back by the per-character offset in metareb_pos,
 * horizontally in whichever direction the move was buffered. */
static void begin_metamor_rebirth(PLW* wk) {
    wk->wu.routine_no[3] = 1;
    wk->wu.rl_flag = wk->wu.rl_waza;
    reset_mvxy_data(&wk->wu);

    if (wk->wu.xyz[1].disp.pos < 3) {
        wk->wu.xyz[1].disp.pos = -8;
    } else {
        wk->wu.xyz[1].disp.pos -= metareb_pos[wk->player_number][1];

        if (wk->wu.rl_flag) {
            wk->wu.xyz[0].disp.pos += metareb_pos[wk->player_number][0];
        } else {
            wk->wu.xyz[0].disp.pos -= metareb_pos[wk->player_number][0];
        }
    }

    set_char_move_init(&wk->wu, 5, 1);
    wk->metamor_over = 0;
}

/* Marker 31 arms the hitboxes, marker 40 launches. The 31 arm's `break` left the
 * switch with nothing after it. */
static void metamor_rebirth_markers(PLW* wk) {
    char_move(&wk->wu);
    if (wk->wu.cg_type == 31) {
        wk->wu.cg_type = 0;
        wk->caution_flag = 0;
        wk->wu.cg_ja = wk->wu.hit_ix_table[wk->wu.cg_hit_ix];
        set_jugde_area(&wk->wu);
        return;
    }

    if (wk->wu.cg_type == 40) {
        wk->wu.routine_no[3] = 2;
        wk->wu.mvxy.a[0].sp = 0;
        wk->wu.mvxy.a[1].sp = 0;
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.d[1].sp = -0x8000;
        wk->wu.mvxy.kop[0] = wk->wu.mvxy.kop[1] = 0;
        wk->scr_pos_set_flag = 1;
    }
}

void Att_METAMOR_REBIRTH(PLW* wk) {
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_metamor_rebirth(wk);
        break;

    case 1:
        metamor_rebirth_markers(wk);
        break;

    case 2:
        wk->scr_pos_set_flag = 1;
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        wk->scr_pos_set_flag = 1;
        char_move(&wk->wu);
        break;
    }
}

void Att_HADOUKEN(PLW* wk) {
    wk->scr_pos_set_flag = 0;
    Att_HADOUKEN2(wk);
}

void Att_HADOUKEN2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack(wk);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Att_NM_OKIAGARI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Att_SHOURYUUKEN(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack_at_row(wk, wk->as->r_no);
        break;

    case 1:
        char_move(&wk->wu);
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
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void Att_SENPUUKYAKU(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack_at_row(wk, wk->as->data_ix);
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

        if (wk->wu.routine_no[3] != 3 && wk->wu.cg_type == 20) {
            add_to_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
        }

        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Att_SENPUUKYAKU2(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack(wk);
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        cal_initial_speed_y(&wk->wu, wk->as->r_no, 0);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
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

/* Once the union hands over to state 3 the vertical speed is flattened and a kop
 * of 2 is stepped back to 1. */
static void abisegeri_settle_on_land(PLW* wk) {
    if (wk->wu.routine_no[3] == 3) {
        if (wk->wu.mvxy.kop[0] == 2) {
            wk->wu.mvxy.kop[0] = 1;
        }

        wk->wu.mvxy.d[1].sp = 0;
        wk->wu.mvxy.a[1].sp = 0;
    }
}

/* The descent, which either end marker stops. */
static void abisegeri_descend(PLW* wk) {
    cal_mvxy_speed(&wk->wu);
    add_mvxy_speed(&wk->wu);
    char_move(&wk->wu);

    if (wk->wu.cg_type == 64 || wk->wu.cg_type == 0xFF) {
        wk->wu.routine_no[3]++;
        wk->wu.mvxy.d[0].sp = 0;
        wk->wu.mvxy.a[0].sp = 0;
    }
}

void Att_ABISEGERI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack(wk);
        setup_mvxy_data(&wk->wu, wk->as->r_no);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 20) {
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            add_mvxy_speed(&wk->wu);
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        abisegeri_settle_on_land(wk);
        break;

    case 3:
        abisegeri_descend(wk);
        break;

    default:
        char_move(&wk->wu);
        break;
    }
}

/* Marker 20 takes the next movement row and hands the move to state 2. Four arms
 * across Att_SHOURYUUREPPA and Att_SLIDE_and_JUMP wrote this identically. */
static void take_row_and_enter_state_2(PLW* wk) {
    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
    }
}

/* Marker 30 drops the move into its falling state. Both the rising arm and the
 * union arm of Att_SHOURYUUREPPA wrote this identically. */
static void shouryuureppa_enter_fall(PLW* wk) {
    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->as->data_ix);
        wk->wu.routine_no[3] = 3;
        wk->wu.cg_type = 0;
    }
}

/* The rising frames. Marker 20 feeds the next row, marker 30 starts the fall, and
 * marker 40 with the right buttons still held cancels into routine 6. */
static void shouryuureppa_rise(PLW* wk) {
    char_move(&wk->wu);

    take_row_and_enter_state_2(wk);

    shouryuureppa_enter_fall(wk);

    if (wk->wu.cg_type == 40 && (wk->cp->sw_new & 0x770) == 0x70) {
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 6;
        wk->wu.routine_no[3] = 0;
        wk->wu.cg_type = 0;
    }
}

void Att_SHOURYUUREPPA(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack_at_row(wk, wk->as->r_no);
        break;

    case 1:
        shouryuureppa_rise(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 1);

        shouryuureppa_enter_fall(wk);

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void Att_SHINSHOURYUUKEN(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack(wk);
        reset_mvxy_data(&wk->wu);
        break;

    case 1:
        char_move(&wk->wu);

        take_row_and_enter_state_2(wk);

        break;

    case 2:
        jumping_union_process(&wk->wu, 1);

        if (wk->wu.cg_type == 30) {
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.cg_type = 0;
        }

        break;
    }
}

/* The airborne markers. The 30 arm returned out of Att_KUUCHUUNICHIRINSHOU
 * outright; here it returns out of the helper and the caller's `break` follows,
 * which reaches the same place because nothing ran after that switch. */
static void nichirinshou_markers(PLW* wk) {
    if (wk->wu.routine_no[3] != 2) {
        if (wk->wu.cg_type == 20) {
            add_to_mvxy_data(&wk->wu, wk->as->data_ix);
            wk->wu.cg_type = 0;
        }

        if (wk->wu.cg_type == 30) {
            add_to_mvxy_data(&wk->wu, wk->as->r_no);
            wk->wu.cg_type = 0;
            return;
        }
    }
}

void Att_KUUCHUUNICHIRINSHOU(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        /* fallthrough */

    case 1:
        jumping_union_process(&wk->wu, 2);

        nichirinshou_markers(wk);
        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

/* The launch frames: marker 1 steps the state on, marker 20 takes the next row. */
static void kuuchuu_jinnchuu_launch(PLW* wk) {
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

/* The airborne markers, which stop once the union has landed. That early `break`
 * left the switch with nothing after it, so a return is the same exit. */
static void kuuchuu_jinnchuu_markers(PLW* wk) {
    if (wk->wu.routine_no[3] == 3) {
        return;
    }

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

void Att_KUUCHUUJINNCHUUWATARI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 5, wk->as->char_ix);
        wk->wu.mvxy.index = wk->as->r_no;
        break;

    case 1:
        kuuchuu_jinnchuu_launch(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        kuuchuu_jinnchuu_markers(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

/* The spin itself. Four markers can arrive - the next movement row, a reset, the
 * jump into recovery, and the hand-over to the union at state 2 - and a grab
 * flag on either side cuts the spin short into state 4. */
static void tenshin_spin(PLW* wk) {
    char_move(&wk->wu);

    switch (wk->wu.cg_type) {
    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 40:
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        break;

    case 50:
        wk->wu.routine_no[3] = 4;
        wk->wu.cg_type = 0;
        break;

    case 30:
        wk->wu.mvxy.index = wk->as->data_ix;
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
        break;
    }

    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);

    if (wk->wu.routine_no[3] != 4 && wk->hos_fi_flag | wk->hos_em_flag) {
        char_move_cmj4(&wk->wu);
        wk->wu.routine_no[3] = 4;
    }
}

/* While the union has not landed, marker 30 still feeds the next row. */
static void tenshin_union_step(PLW* wk) {
    if (wk->wu.routine_no[3] != 3 && wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }
}

/* Recovery reads the same markers as the spin minus the 50 that brought it here,
 * so the two switches are near misses rather than duplicates and stay apart. */
static void tenshin_recover(PLW* wk) {
    char_move(&wk->wu);

    switch (wk->wu.cg_type) {
    case 20:
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
        break;

    case 40:
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        break;

    case 30:
        wk->wu.mvxy.index = wk->as->data_ix;
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 2;
        wk->wu.cg_type = 0;
        break;
    }

    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);
}

void Att_TENSHINSENKYUUTAI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_uni_attack_at_row(wk, wk->as->r_no);
        break;

    case 1:
        tenshin_spin(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        tenshin_union_step(wk);
        break;

    case 3:
        char_move(&wk->wu);
        break;

    case 4:
        tenshin_recover(wk);
        break;
    }
}

void Att_CHOUCHUURENGEKI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_slide_attack(wk);
        break;

    default:
        char_move(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);

        switch (wk->wu.cg_type) {
        case 20:
            setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
            wk->wu.mvxy.index++;
            wk->wu.cg_type = 0;
            break;

        case 21:
            reset_mvxy_data(&wk->wu);
            wk->wu.cg_type = 0;
            break;
        }
    }
}

/* The slide itself: marker 20 continues it, marker 30 turns it into the jump. */
static void slide_run(PLW* wk) {
    char_move(&wk->wu);

    take_row_and_enter_state_2(wk);

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3] = 3;
        wk->wu.cg_type = 0;
    }

    if (wk->wu.routine_no[3] != 1) {
        add_mvxy_speed(&wk->wu);
    }
}

/* While the union has not returned to state 1, marker 20 still feeds rows. */
static void slide_union_step(PLW* wk) {
    if ((wk->wu.routine_no[3] != 1) && (wk->wu.cg_type == 20)) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }
}

/* The jump half. Its marker-30 block is a near miss of the slide's - that one
 * also assigns state 3 - so the two stay apart. */
static void slide_jump(PLW* wk) {
    char_move(&wk->wu);
    cal_mvxy_speed(&wk->wu);
    add_mvxy_speed(&wk->wu);

    take_row_and_enter_state_2(wk);

    if (wk->wu.cg_type == 21) {
        reset_mvxy_data(&wk->wu);
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 1;
    }

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.a[1].sp = wk->wu.mvxy.d[1].sp = wk->wu.mvxy.kop[1] = 0;
        wk->wu.mvxy.index++;
        wk->wu.cg_type = 0;
    }
}

void Att_SLIDE_and_JUMP(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_slide_attack(wk);
        break;

    case 1:
        slide_run(wk);
        break;

    case 2:
        jumping_union_process(&wk->wu, 1);
        slide_union_step(wk);
        break;

    case 3:
        slide_jump(wk);
        break;
    }
}

/* Airborne: marker 20 feeds the next row, marker 1 drops back to the ground state
 * instead of stepping the union. That arm's `break` left the switch with nothing
 * after it. */
static void jinnchuu_airborne(PLW* wk) {
    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.cg_type = 0;
        wk->wu.mvxy.index++;
    }

    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 3;
        return;
    }

    jumping_union_process(&wk->wu, 3);
}

/* Grounded: marker 1 sends it back into the air. */
static void jinnchuu_grounded(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3] = 2;
    }
}

void Att_JINNCHUUWATARI(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_ahj_attack(wk);
        break;

    case 1:
        char_move(&wk->wu);
        att_ahj_table_reader(wk);
        break;

    case 2:
        jinnchuu_airborne(wk);
        break;

    case 3:
        jinnchuu_grounded(wk);
        break;
    }
}

void Att_HOMING_JUMP(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_ahj_attack(wk);
        break;

    case 1:
        char_move(&wk->wu);
        att_ahj_table_reader(wk);
        break;

    case 3:
        wk->wu.routine_no[3] = 2;
        /* fallthrough */

    case 2:
        jumping_union_process(&wk->wu, 1);

        if (wk->wu.routine_no[3] != 1) {
            att_ahj_table_reader(wk);
        }

        break;
    }
}

const s16 ahj_empos_hos[3][20][2] = {
    { { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 },
      { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 } },
    { { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 },
      { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 },
      { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 } },
    { { 56, 56 }, { 48, 48 }, { 40, 40 }, { 40, 28 }, { 64, 40 }, { 52, 40 }, { 64, 56 },
      { 64, 32 }, { 56, 40 }, { 56, 32 }, { 40, 28 }, { 40, 40 }, { 40, 40 }, { 56, 56 },
      { 40, 40 }, { 56, 32 }, { 48, 32 }, { 52, 44 }, { 48, 36 }, { 48, 36 } }
};

const s16 ahj_kop[3][4] = { { 0, 24, 0, 2 }, { 0, 22, 0, 2 }, { 0, 23, 0, 2 } };

const s16 ahj_empos_hos_cps3[6][24][2] = {
    { { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 },
      { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 },
      { 16, 192 }, { 16, 192 }, { 16, 192 }, { 16, 192 }, { 0, 0 },    { 0, 0 },    { 0, 0 },    { 0, 0 } },
    { { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 },
      { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 },
      { 27, -8 }, { 27, -8 }, { 27, -8 }, { 27, -8 }, { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
    { { 56, 56 }, { 48, 48 }, { 40, 40 }, { 40, 28 }, { 64, 40 }, { 52, 40 }, { 64, 56 }, { 64, 32 },
      { 56, 40 }, { 56, 32 }, { 40, 28 }, { 40, 40 }, { 40, 40 }, { 56, 56 }, { 40, 40 }, { 56, 32 },
      { 48, 32 }, { 52, 44 }, { 48, 36 }, { 48, 36 }, { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
    { { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 },
      { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 },
      { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 }, { 48, 0 } },
    { { 24, 86 }, { 28, 68 }, { 16, 62 }, { 16, 52 }, { 20, 72 }, { 20, 58 }, { 18, 82 }, { 18, 52 },
      { 28, 45 }, { 25, 42 }, { 16, 52 }, { 16, 62 }, { 16, 62 }, { 24, 86 }, { 16, 62 }, { 16, 62 },
      { 16, 62 }, { 16, 62 }, { 24, 86 }, { 20, 58 }, { 20, 72 }, { 0, 0 },   { 0, 0 },   { 0, 0 } },
    { { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 },
      { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 },
      { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 16 }, { 48, 0 },  { 48, 0 },  { 48, 0 } },
};

const s16 ahj_kop_cps3[6][4] = {
    { 0, 28, 2, 2 }, { 0, 22, 0, 2 }, { 0, 23, 0, 2 }, { 0, 24, 0, 2 }, { 0, 27, 0, 2 }, { 0, 21, 0, 2 },
};

/* Marker 30 aims the move at the opponent. A kop[0] of 0 solves the delta speed
 * against the row's offsets first and falls through; every other kop skips
 * straight to the facing correction, which is what the original switch said.
 * The derived table rows are passed in rather than recomputed, so
 * ArcadeBalance_IsEnabled is still called exactly once per frame. The row type is
 * written out as the caller already writes it, so the guard sees one `s16(` and
 * one `2` added rather than a substitution. */
static void ahj_aim_on_marker_30(PLW* wk, PLW* twk, const s16 (*curr_empos_hos)[2], const s16* curr_kop) {
    s16 ex;
    s16 ey;

    if (wk->wu.cg_type == 30) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;

        switch (curr_kop[0]) {
        case 0:
            if (wk->wu.rl_flag) {
                ex = twk->wu.position_x - curr_empos_hos[twk->player_number][0];
            } else {
                ex = twk->wu.position_x + curr_empos_hos[twk->player_number][0];
            }

            ey = curr_empos_hos[twk->player_number][1];
            wk->wu.mvxy.a[0].sp = 0;
            cal_delta_speed(&wk->wu, &(Motion_Target) { curr_kop[1], ex, ey, curr_kop[2], curr_kop[3] });
            /* fallthrough */

        default:
            if (wk->wu.rl_flag == 0) {
                wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
                wk->wu.mvxy.d[0].sp = -wk->wu.mvxy.d[0].sp;
            }

            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            add_mvxy_speed(&wk->wu);
            break;
        }
    }
}

void att_ahj_table_reader(PLW* wk) { // 🟡
    PLW* twk = (PLW*)wk->wu.target_adrs;
    // CPS3 uses its original 24-character offsets and control parameters.
    const bool use_cps3_table = ArcadeBalance_IsEnabled() || wk->as->r_no >= SDL_arraysize(ahj_empos_hos);
    const s16(*curr_empos_hos)[2] = use_cps3_table ? ahj_empos_hos_cps3[wk->as->r_no] : ahj_empos_hos[wk->as->r_no];
    const s16* curr_kop = use_cps3_table ? ahj_kop_cps3[wk->as->r_no] : ahj_kop[wk->as->r_no];
    ahj_aim_on_marker_30(wk, twk, curr_empos_hos, curr_kop);

    if (wk->wu.cg_type == 20) {
        setup_mvxy_data(&wk->wu, wk->wu.mvxy.index);
        wk->wu.mvxy.index++;
        wk->wu.routine_no[3]++;
        wk->wu.cg_type = 0;
        add_mvxy_speed(&wk->wu);
    }
}
