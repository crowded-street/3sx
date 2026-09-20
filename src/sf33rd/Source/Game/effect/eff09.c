/**
 * @file eff09.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff09.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/eff09_animation.h"
#include "sf33rd/Source/Game/effect/eff09_endgame.h"
#include "sf33rd/Source/Game/effect/eff09_late.h"
#include "sf33rd/Source/Game/effect/effb4.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

const s16 eff09_data[2][9] = { { 0, 2, 8492, 832, 192, 10, 0, 1, 0 }, { 19, 2, 8492, 528, 108, 78, 1, 0, 0 } };

const s16 eff09_data2[43][8] = {
    { 1, 0, 0, 4, 1, 25, 1, 0 },       { 2, 0, -34, 153, -10, 26, 1, 0 }, { 3, 6, 48, 240, -1, 99, 1, 0 },
    { 4, 0, 48, -4, -10, 28, 1, 0 },   { 5, 0, 10, 96, -20, 46, 1, 0 },   { 6, 0, 0, 0, -20, 47, 1, 0 },
    { 7, 0, 0, 80, -10, 43, 1, 0 },    { 0, 0, -38, 84, -2, 41, 1, 0 },   { 8, 0, 0, 0, -2, 55, 1, 0 },
    { 8, 0, 0, 0, -2, 56, 1, 0 },      { 9, 0, 0, 0, -12, 71, 1, 0 },     { 9, 0, 0, 0, -12, 72, 1, 0 },
    { 10, 0, 0, 0, -12, 73, 1, 0 },    { 11, 0, 48, -10, -10, 28, 1, 0 }, { 12, 0, 0, 74, 2, 75, 1, 0 },
    { 13, 0, -4, 154, -2, 80, 1, 0 },  { 13, 0, -32, 112, -2, 81, 1, 0 }, { 13, 0, 16, 122, -2, 82, 1, 0 },
    { 14, 26, 45, 0, -16, 84, 1, 0 },  { 14, 26, 12, 0, -16, 84, 1, 0 },  { 15, 0, -18, 62, -2, 85, 1, 0 },
    { 17, 0, -32, 72, -2, 85, 1, 0 },  { 16, 0, 0, 0, -12, 87, 1, 0 },    { 18, 0, 0, 0, 1, 96, 1, 0 },
    { 20, 0, 160, 45, 10, 20, 1, 0 },  { 20, 0, 192, 45, 10, 21, 1, 0 },  { 20, 0, 208, 45, 10, 22, 1, 0 },
    { 20, 0, 176, 45, 10, 23, 1, 0 },  { 5, 0, 30, 3, 2, 48, 1, 0 },      { 21, 0, -24, 136, -2, 51, 1, 0 },
    { 21, 0, -24, 136, -2, 51, 1, 0 }, { 0, 26, 34, 42, -2, 60, 1, 0 },   { 22, 0, 0, 0, -2, 61, 1, 0 },
    { 23, 0, 72, -5, 2, 63, 1, 0 },    { 24, 0, -96, 8, 4, 97, 1, 0 },    { 24, 0, 80, 4, 3, 98, 1, 0 },
    { 25, 0, 68, 40, 1, 26, 1, 0 },    { 25, 0, 42, 51, 1, 26, 1, 0 },    { 24, 0, -432, 4, 3, 98, 1, 0 },
    { 24, 0, -192, 8, 4, 88, 1, 0 },   { 24, 0, -304, 8, 4, 97, 1, 0 },   { 26, 6, 0, 0, 1, 101, 1, 0 },
    { 27, 0, 0, 16, -10, 107, 1, 0 }
};

const s32 eff09_2000_data[2][4] = { { -0x10000, 0x50000, -0x1000, -0x6000 }, { -0x50000, 0x60000, -0xC00, -0x6000 } };

const s32 eff09_add_tbl0[2][2] = { { 0x800, -0x800 }, { 0x800, -0x1000 } };

const s32 eff09_add_tbl1[2][2] = { { -0x800, 0x800 }, { -0x1200, 0x1000 } };

const s32 eff09_add_tbl2[2][2] = { { -0x800, 0x800 }, { -0x1000, 0x1200 } };

const s16 sean_ball_hit[4] = { -16, 32, -11, 22 };

const s32 eff09_13_tbl[3][4] = { { 0x0, 0x1000, 0x30000, 0x0 },
                                 { 0x48000, 0x1000, 0x28000, 0x0 },
                                 { -0x48000, -0x1000, 0x28000, 0x0 } };

const s32 eff09_19000_tbl[3][4] = { { 0x30000, 0x0, 0x10000, -0x6000 },
                                    { 0x20000, 0x0, 0x48000, -0x6000 },
                                    { 0x18000, 0x0, 0x30000, -0x6000 } };

void sean_ball_move(WORK_Other* ewk, u16 sw_work);
void ball_bound_set(WORK_Other* ewk);
void jijii_win_tama_sub(WORK_Other* ewk);

void (*eff09_tbl[28])(WORK_Other*);

void effect_09_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[1] = 99;
    }

    eff09_tbl[ewk->wu.routine_no[0]](ewk);
}

static s32 eff09_0000_animation_updates_enabled(const WORK_Other* ewk) {
    return !EXE_flag && !Game_pause && ewk->wu.hit_stop;
}

/* eff09_8000 asks only for the hit stop, where eff09_0000 also refuses while the
 * game is paused. */
static s32 eff09_8000_animation_updates_enabled(const WORK_Other* ewk) {
    return ewk->wu.hit_stop;
}

/* The two balls disagree on what counts as the animation having ended:
 * eff09_0000 takes any non-zero cg_type, eff09_8000 only 0xFF. */
static s32 eff09_0000_animation_ended(const WORK_Other* ewk) {
    return ewk->wu.cg_type;
}

static s32 eff09_8000_animation_ended(const WORK_Other* ewk) {
    return ewk->wu.cg_type == 0xFF;
}

/* What eff09_0000 and eff09_8000 do not share: the two tests in case 1 and the
 * transfer entry it posts. Three more parameters would put the helper over the
 * argument-count threshold, so they travel together. */
typedef struct {
    s32 (*updates_enabled)(const WORK_Other* ewk);
    s32 (*animation_ended)(const WORK_Other* ewk);
    void (*disp_pos_trans_entry)(WORK_Other* ewk);
} Eff09_Ball_Ops;

/* The ball state machine both eff09_0000 and eff09_8000 run. */
static void run_eff09_ball_state(WORK_Other* ewk, const Eff09_Ball_Ops* ops) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.dead_f = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (ops->updates_enabled(ewk)) {
            char_move(&ewk->wu);

            if (ops->animation_ended(ewk)) {
                ewk->wu.routine_no[1]++;
                ewk->wu.disp_flag = 0;
            }
        }

        ops->disp_pos_trans_entry(ewk);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff09_0000(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    run_eff09_ball_state(
        ewk,
        &(Eff09_Ball_Ops) { eff09_0000_animation_updates_enabled, eff09_0000_animation_ended, disp_pos_trans_entry_rs }
    );
}

static s32 eff09_1000_updates_enabled() {
    return !EXE_flag && !Game_pause && !EXE_obroll;
}

static s32 eff09_1000_can_register_hit(WORK_Other* ewk) {
    return ewk->wu.old_rno[0] && !EXE_flag && !Game_pause && !EXE_obroll && eff_hit_check(ewk, 0);
}

static void initialize_eff09_1000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.my_priority = ewk->wu.position_z = 67;
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos = 4;
    ewk->wu.old_rno[0] = random_16();
    ewk->wu.old_rno[0] &= 1;

    if (ewk->master_id) {
        ewk->wu.type = 10;
    } else {
        ewk->wu.type = 9;
    }

    if (EXE_obroll) {
        set_char_move_init(&ewk->wu, 0, 25);
        ewk->wu.old_rno[0] = 1;
    } else if (ewk->wu.old_rno[0]) {
        set_char_move_init(&ewk->wu, 0, 25);
    } else {
        set_char_move_init(&ewk->wu, 0, 65);
    }

    sort_push_request(&ewk->wu);
}

static void advance_eff09_1000_animation(WORK_Other* ewk) {
    if (eff09_1000_updates_enabled()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[1]++;
        }
    }
}

static void display_eff09_1000(WORK_Other* ewk) {
    if (!obr_no_disp_check()) {
        sort_push_request(&ewk->wu);
    }
}

static void advance_eff09_1000_hit(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    if (eff09_1000_can_register_hit(ewk)) {
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, 65);
    }

    sort_push_request(&ewk->wu);
}

static void display_eff09_1000_final(WORK_Other* ewk) {
    if (obr_no_disp_check() == 0) {
        sort_push_request(&ewk->wu);
    }
}

void eff09_1000(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_1000(ewk);
        break;

    case 1:
    case 3:
        advance_eff09_1000_animation(ewk);
        display_eff09_1000(ewk);
        break;

    case 2:
        advance_eff09_1000_hit(ewk);
        break;

    case 4:
        display_eff09_1000_final(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static s32 eff09_2000_updates_enabled() {
    return !EXE_flag && !Game_pause;
}

static s32 eff09_2000_is_outside_horizontal_bounds(const WORK_Other* ewk) {
    return ewk->wu.xyz[0].disp.pos >= 785 || ewk->wu.xyz[0].disp.pos < 240;
}

static s32 eff09_2000_is_outside_bounds(const WORK_Other* ewk) {
    return (ewk->wu.xyz[0].disp.pos >= 785 || ewk->wu.xyz[0].disp.pos < 240) || ewk->wu.xyz[1].disp.pos < -56;
}

static void handle_eff09_2000_horizontal_exit(WORK_Other* ewk) {
    if (eff09_2000_is_outside_horizontal_bounds(ewk)) {
        ewk->wu.routine_no[1] = 99;
        Appear_free[ewk->master_id] = 1;
        ewk->wu.disp_flag = 0;
    }
}

static void select_eff09_2000_opponent(WORK_Other* ewk, u16* sw_work, PLW** hit_pl, const s16** pl_hit_ptr) {
    if (ewk->master_id) {
        *sw_work = p2sw_0;
        *hit_pl = &plw[0];
        *pl_hit_ptr = &plw[0].wu.h_bod->body_dm[0][0];
    } else {
        *sw_work = p1sw_0;
        *hit_pl = &plw[1];
        *pl_hit_ptr = &plw[1].wu.h_bod->body_dm[0][0];
    }
}

static void handle_eff09_2000_floor_contact(WORK_Other* ewk) {
    if (ewk->wu.xyz[1].disp.pos < 0) {
        Sound_SE((ewk->master_id * 0x300) + 0x157);
        ewk->wu.routine_no[1]++;
        ball_bound_set(ewk);
        Appear_free[ewk->master_id] = 1;
    }
}

static void initialize_eff09_2000(WORK_Other* ewk, s16* work) {
    const s32* ptr;
    u16 sw_work;

    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    Appear_free[ewk->master_id] = 0;

    if (ewk->master_id) {
        sw_work = p2sw_0;
    } else {
        sw_work = p1sw_0;
    }

    if (sw_work & 0x4000) {
        *work = 0;
    } else {
        *work = random_16();
        *work &= 1;
    }

    ewk->wu.dir_step = *work;
    ptr = eff09_2000_data[ewk->wu.dir_step];
    ewk->wu.mvxy.a[0].sp = *ptr++;
    ewk->wu.mvxy.a[1].sp = *ptr++;
    ewk->wu.mvxy.d[0].sp = *ptr++;
    ewk->wu.mvxy.d[1].sp = *ptr++;

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.d[0].sp = -ewk->wu.mvxy.d[0].sp;
    }

    ewk->wu.old_rno[0] = ewk->wu.old_rno[1] = ewk->wu.old_rno[2] = ewk->wu.old_rno[3] = 0;
    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);
}

static void advance_eff09_2000_bounce(WORK_Other* ewk) {
    if (eff09_2000_updates_enabled()) {
        if (eff09_2000_is_outside_bounds(ewk)) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        } else {
            add_x_sub(&ewk->wu);
            add_y_sub(&ewk->wu);
        }
    }
}

void eff09_2000(WORK_Other* ewk) {
    s16 work;
    u16 sw_work;
    const s16* pl_hit_ptr;
    PLW* hit_pl;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_2000(ewk, &work);
        return;

    case 1:
        if (eff09_2000_updates_enabled()) {
            char_move(&ewk->wu);
            add_x_sub(&ewk->wu);
            add_y_sub(&ewk->wu);
            handle_eff09_2000_horizontal_exit(ewk);
            select_eff09_2000_opponent(ewk, &sw_work, &hit_pl, &pl_hit_ptr);

            sean_ball_move(ewk, sw_work);

            if (hit_check_subroutine(&hit_pl->wu, &ewk->wu, pl_hit_ptr, sean_ball_hit)) {
                ewk->wu.routine_no[1]++;
                ball_bound_set(ewk);
                set_char_move_init2(&hit_pl->wu, 9, 63, 1, 0);
                Appear_free[ewk->master_id] = 2;
                effect_B4_init(ewk);
                work = random_16();
                work &= 7;
                add_super_arts_gauge(
                    plw[ewk->master_id].sa, plw[ewk->master_id].wu.id, 1, plw[ewk->master_id].metamorphose
                );
                break;
            }

            handle_eff09_2000_floor_contact(ewk);
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        advance_eff09_2000_bounce(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void adjust_sean_ball_up(WORK_Other* ewk, u16 sw_work) {
    if (sw_work & 1) {
        ewk->wu.old_rno[0]++;

        if (ewk->wu.old_rno[0] < 8) {
            ewk->wu.mvxy.d[1].sp += eff09_add_tbl0[ewk->wu.dir_step][0];
        }
    }
}

static void adjust_sean_ball_down(WORK_Other* ewk, u16 sw_work) {
    if (sw_work & 2) {
        ewk->wu.old_rno[1]++;

        if (ewk->wu.old_rno[1] < 8) {
            ewk->wu.mvxy.d[1].sp += eff09_add_tbl0[ewk->wu.dir_step][1];
        }
    }
}

/* Which side of Sean's ball this is: the button that pushes it, the counter it
 * counts on, how far that counter may run while facing and while turned away,
 * and the column of the two add tables it reads. */
typedef struct {
    u16 button;
    s16 counter;
    s16 limit_facing;
    s16 limit_away;
    s16 column;
} SeanBallSide;

static void adjust_sean_ball(WORK_Other* ewk, u16 sw_work, const SeanBallSide* side) {
    if (sw_work & side->button) {
        ewk->wu.old_rno[side->counter]++;

        if (ewk->wu.rl_flag) {
            if (ewk->wu.old_rno[side->counter] < side->limit_facing) {
                ewk->wu.mvxy.d[0].sp += eff09_add_tbl1[ewk->wu.dir_step][side->column];
            }
        } else if (ewk->wu.old_rno[side->counter] < side->limit_away) {
            ewk->wu.mvxy.d[0].sp += eff09_add_tbl2[ewk->wu.dir_step][side->column];
        }
    }
}

static void adjust_sean_ball_left(WORK_Other* ewk, u16 sw_work) {
    adjust_sean_ball(ewk, sw_work, &(SeanBallSide) { 4, 2, 4, 8, 0 });
}

static void adjust_sean_ball_right(WORK_Other* ewk, u16 sw_work) {
    adjust_sean_ball(ewk, sw_work, &(SeanBallSide) { 8, 3, 8, 4, 1 });
}

void sean_ball_move(WORK_Other* ewk, u16 sw_work) {
    if (!plw[ewk->master_id].wu.operator) {
        return;
    }

    adjust_sean_ball_up(ewk, sw_work);
    adjust_sean_ball_down(ewk, sw_work);
    adjust_sean_ball_left(ewk, sw_work);
    adjust_sean_ball_right(ewk, sw_work);
}

void ball_bound_set(WORK_Other* ewk) {
    ewk->wu.mvxy.a[0].sp = -0x40000;
    ewk->wu.mvxy.a[1].sp = 0x60000;
    ewk->wu.mvxy.d[0].sp = -0x1000;
    ewk->wu.mvxy.d[1].sp = -0x8000;
    ewk->wu.dir_timer = 12;

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
        ewk->wu.mvxy.d[0].sp = -ewk->wu.mvxy.d[0].sp;
    }
}

static void advance_eff09_3000_arrival(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
        } else {
            add_y_sub(&ewk->wu);
        }

        char_move(&ewk->wu);
    }
}

static void advance_eff09_3000_animation(WORK_Other* ewk) {
    if (Exec_Wipe) {
        Sound_SE((Winner_id * 0x300) + 0x130);
    }

    if (!EXE_flag && !Game_pause) {
        char_move(&ewk->wu);
    }
}

void eff09_3000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (Exec_Wipe) {
            ewk->wu.routine_no[1] = 99;
            break;
        }

        Sound_SE((Winner_id * 0x300) + 0x136);
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_rno[0] = 96;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], ewk->wu.xyz[0].disp.pos, 40, 1, 2 });
        break;

    case 1:
        advance_eff09_3000_arrival(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        advance_eff09_3000_animation(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_4000(WORK_Other* ewk) {
    s16 work;
    s16 work2;
    WORK* oya_ptr = (WORK*)ewk->my_master;

    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 2;
    ewk->wu.kage_hy = -3;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 0;
    ewk->wu.my_col_code = 0x2000;
    ewk->wu.position_z = ewk->wu.my_priority = 20;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    ewk->wu.rl_flag = oya_ptr->rl_flag;

    if (oya_ptr->rl_flag) {
        work = bg_w.bgw[1].xy[0].disp.pos - 272;
        work2 = oya_ptr->xyz[0].disp.pos - 88;
    } else {
        work = bg_w.bgw[1].xy[0].disp.pos + 272;
        work2 = oya_ptr->xyz[0].disp.pos + 88;
    }

    ewk->wu.xyz[0].disp.pos = work;
    ewk->wu.old_rno[0] = 202;
    cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], work2, 0, 0, 2 });
}

/* The timed approach both effects run: while updates are enabled, move the
 * effect along and count its timer down. Non-zero on the frame the timer
 * reaches zero and the state steps on - which is where the two callers each do
 * their own thing. */
static s32 eff09_approach_complete(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
            return 1;
        }
    }

    return 0;
}

static void advance_eff09_4000(WORK_Other* ewk) {
    if (eff09_approach_complete(ewk)) {
        set_char_move_init(&ewk->wu, 0, 29);
    }
}

void eff09_4000(WORK_Other* ewk) {

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_4000(ewk);
        break;

    case 1:
        advance_eff09_4000(ewk);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff09_5000(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff09_6000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 9) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 2;
            ewk->wu.blink_timing = 1;
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
            break;
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff09_7000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.old_rno[0] = 32;

        if (ewk->master_id) {
            ewk->wu.mvxy.a[0].sp = 0x30000;
            ewk->wu.mvxy.a[1].sp = 0x90000;
            ewk->wu.mvxy.d[0].sp = 0x1000;
            ewk->wu.mvxy.d[1].sp = -0x6000;
            break;
        }

        ewk->wu.mvxy.a[0].sp = -0x30000;
        ewk->wu.mvxy.a[1].sp = 0x90000;
        ewk->wu.mvxy.d[0].sp = -0x1000;
        ewk->wu.mvxy.d[1].sp = -0x6000;
        break;

    case 1:
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] < 0) {
            ewk->wu.routine_no[1]++;
            break;
        }

        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff09_8000(WORK_Other* ewk) {
    run_eff09_ball_state(
        ewk,
        &(Eff09_Ball_Ops) { eff09_8000_animation_updates_enabled, eff09_8000_animation_ended, disp_pos_trans_entry_s }
    );
}

static void initialize_eff09_11000(WORK_Other* ewk, const WORK* oya_ptr) {
    s16 work;
    s16 work2;

    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 2;
    ewk->wu.kage_hy = -10;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 0;
    ewk->wu.my_col_code = 0x2000;
    ewk->wu.position_z = ewk->wu.my_priority = 20;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    ewk->wu.rl_flag = oya_ptr->rl_flag ^ 1;

    if (oya_ptr->rl_flag) {
        work = bg_w.bgw[1].xy[0].disp.pos + 272;
        work2 = oya_ptr->xyz[0].disp.pos + 56;
    } else {
        work = bg_w.bgw[1].xy[0].disp.pos - 272;
        work2 = oya_ptr->xyz[0].disp.pos - 56;
    }

    ewk->wu.xyz[0].disp.pos = work;
    ewk->wu.old_rno[0] = 220;
    cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], work2, 0, 0, 2 });
}

static void advance_eff09_11000(WORK_Other* ewk, WORK* oya_ptr) {
    if (eff09_approach_complete(ewk)) {
        oya_ptr->cmwk[0] = 9;
    }
}

static void finish_eff09_11000_approach(WORK_Other* ewk, const WORK* oya_ptr) {
    add_x_sub(&ewk->wu);

    if (oya_ptr->rl_flag) {
        if (oya_ptr->xyz[0].disp.pos > ewk->wu.xyz[0].disp.pos) {
            ewk->wu.routine_no[1]++;
        }
    } else if (oya_ptr->xyz[0].disp.pos < ewk->wu.xyz[0].disp.pos) {
        ewk->wu.routine_no[1]++;
    }
}

void eff09_11000(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_11000(ewk, oya_ptr);
        break;

    case 1:
        advance_eff09_11000(ewk, oya_ptr);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        finish_eff09_11000_approach(ewk, oya_ptr);
        /* fallthrough */

    case 3:
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_12000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    ewk->wu.mvxy.a[1].sp = -0x10000;
    ewk->wu.mvxy.d[1].sp = 0;
    ewk->wu.mvxy.d[0].sp = 0;

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = -0x20000;
    } else {
        ewk->wu.mvxy.a[0].sp = 0x20000;
    }

    suzi_sync_pos_set(ewk);
    sort_push_request(&ewk->wu);
}

void eff09_12000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_12000(ewk);
        break;

    case 1:
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos < 56) {
            ewk->wu.routine_no[1]++;
            ewk->wu.blink_timing = 1;
            ewk->wu.disp_flag = 2;
            ewk->wu.old_rno[1] = 20;
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        ewk->wu.old_rno[1]--;

        if (ewk->wu.old_rno[1] < 0) {
            ewk->wu.routine_no[1]++;
            break;
        }

        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);
        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static s32 eff09_13000_has_left_play_area(const WORK_Other* ewk) {
    return ewk->wu.xyz[0].disp.pos < bg_w.bgw[1].pos_x_work - 240 ||
           ewk->wu.xyz[0].disp.pos > bg_w.bgw[1].pos_x_work + 240 || ewk->wu.xyz[1].disp.pos > 512;
}

static void advance_eff09_13000_intro(WORK_Other* ewk, const WORK* oya_ptr) {
    if (eff09_2000_updates_enabled()) {
        char_move(&ewk->wu);

        if (oya_ptr->cg_type == 99) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, 83);
            ewk->wu.mvxy.a[0].sp = eff09_13_tbl[ewk->wu.type - 0xF][0];
            ewk->wu.mvxy.d[0].sp = eff09_13_tbl[ewk->wu.type - 0xF][1];
            ewk->wu.mvxy.a[1].sp = eff09_13_tbl[ewk->wu.type - 0xF][2];
            ewk->wu.mvxy.d[1].sp = eff09_13_tbl[ewk->wu.type - 0xF][3];
        }
    }
}

void eff09_13000(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.dead_f = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        advance_eff09_13000_intro(ewk, oya_ptr);

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 2:
        if (eff09_2000_updates_enabled()) {
            char_move(&ewk->wu);
            add_x_sub(&ewk->wu);
            add_y_sub(&ewk->wu);

            if (eff09_13000_has_left_play_area(ewk)) {
                ewk->wu.routine_no[1]++;
                ewk->wu.disp_flag = 0;
            }
        }

        suzi_sync_pos_set(ewk);
        sort_push_request(&ewk->wu);
        break;

    case 3:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_21000_type_29(WORK_Other* ewk, s16* arrive_x, s16* arrive_y) {
    ewk->wu.old_rno[0] = 70;

    if (ewk->wu.rl_flag) {
        *arrive_x = ewk->wu.xyz[0].disp.pos + 42;
    } else {
        *arrive_x = ewk->wu.xyz[0].disp.pos - 42;
    }

    *arrive_y = (ewk->wu.xyz[1].disp.pos + 6);
}

static void initialize_eff09_21000_winner(WORK_Other* ewk, s16* arrive_x, s16* arrive_y) {
    ewk->wu.old_rno[0] = 64;

    if (plw[Winner_id].wu.rl_flag) {
        *arrive_x = plw[Winner_id].wu.xyz[0].disp.pos + 28;
    } else {
        *arrive_x = plw[Winner_id].wu.xyz[0].disp.pos - 28;
    }

    *arrive_y = plw[Winner_id].wu.xyz[1].disp.pos + 162;
}

static void initialize_eff09_21000(WORK_Other* ewk) {
    s16 arrive_x;
    s16 arrive_y;

    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
    ewk->wu.mvxy.d[0].sp = 0;
    ewk->wu.mvxy.d[1].sp = -0x8000;

    if (ewk->wu.type == 29) {
        initialize_eff09_21000_type_29(ewk, &arrive_x, &arrive_y);
    } else {
        initialize_eff09_21000_winner(ewk, &arrive_x, &arrive_y);
    }

    cal_all_speed_data(&ewk->wu, &(Motion_Target) { ewk->wu.old_rno[0], arrive_x, arrive_y, 2, 2 });
}

static void advance_eff09_21000_arrival(WORK_Other* ewk) {
    if (eff09_2000_updates_enabled()) {
        char_move(&ewk->wu);
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, 58);
        } else {
            add_x_sub(&ewk->wu);
            add_y_sub(&ewk->wu);
        }
    }
}

/* What each of the two late animations does when it ends. */
static void end_eff09_21000_animation(WORK_Other* ewk) {
    ewk->wu.disp_flag = 0;
    plw[Winner_id].wu.cmwk[0] = 1;
}

static void end_eff09_26000_transition(WORK_Other* ewk) {
    set_char_move_init(&ewk->wu, 0, 102);
}

/* The late animation step 21000 and 26000 share: while updates are enabled,
 * move, and when the animation has ended step the routine and do the state's own
 * ending. They disagree on what counts as ended - any cg_type against 0xFF - and
 * on the ending itself. */
static void advance_eff09_late_animation(
    WORK_Other* ewk, s32 (*animation_ended)(const WORK_Other* ewk), void (*on_end)(WORK_Other* ewk)
) {
    if (eff09_2000_updates_enabled()) {
        char_move(&ewk->wu);

        if (animation_ended(ewk)) {
            ewk->wu.routine_no[1]++;
            on_end(ewk);
        }
    }
}

static void advance_eff09_21000_animation(WORK_Other* ewk) {
    advance_eff09_late_animation(ewk, eff09_0000_animation_ended, end_eff09_21000_animation);
}

void eff09_21000(WORK_Other* ewk) {
    if (Suicide[0]) {
        ewk->wu.routine_no[1] = 99;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_21000(ewk);
        break;

    case 1:
        advance_eff09_21000_arrival(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 2:
        advance_eff09_21000_animation(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 3:
        ewk->wu.routine_no[1]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_26000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    set_char_move_init(&ewk->wu, 0, 101);
    ewk->wu.old_rno[0] = 0;
    ewk->wu.xyz[1].disp.pos += base_y_pos;
}

static void advance_eff09_26000_start_animation(WORK_Other* ewk, WORK* oya_ptr) {
    if (eff09_2000_updates_enabled()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 9) {
            ewk->wu.routine_no[1]++;
            oya_ptr->cmwk[0] = 1;
        }
    }
}

static void advance_eff09_26000_transition(WORK_Other* ewk) {
    advance_eff09_late_animation(ewk, eff09_8000_animation_ended, end_eff09_26000_transition);
}

static void advance_eff09_26000_parent_wait(WORK_Other* ewk, const WORK* oya_ptr) {
    if (eff09_2000_updates_enabled()) {
        char_move(&ewk->wu);

        if (oya_ptr->cmwk[0] == 2) {
            ewk->wu.routine_no[1]++;
        }
    }
}

static void advance_eff09_26000_tracking(WORK_Other* ewk, const WORK* oya_ptr) {
    if (eff09_2000_updates_enabled()) {
        char_move(&ewk->wu);
    }

    ewk->wu.xyz[1].disp.pos = oya_ptr->position_y + base_y_pos;
    ewk->wu.xyz[1].disp.pos -= 42;
    jijii_win_tama_sub(ewk);
    disp_pos_trans_entry(ewk);
}

void eff09_26000(WORK_Other* ewk) {
    WORK* oya_ptr;

    if (test_flag) {
        ewk->wu.routine_no[1] = 99;
    }

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_26000(ewk);
        break;

    case 1:
        advance_eff09_26000_start_animation(ewk, oya_ptr);
        jijii_win_tama_sub(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 2:
        advance_eff09_26000_transition(ewk);
        jijii_win_tama_sub(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 3:
        advance_eff09_26000_parent_wait(ewk, oya_ptr);
        jijii_win_tama_sub(ewk);
        disp_pos_trans_entry(ewk);
        break;

    case 4:
        advance_eff09_26000_tracking(ewk, oya_ptr);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void jijii_win_tama_sub(WORK_Other* ewk) {
    WORK* oya_ptr = (WORK*)ewk->my_master;

    ewk->wu.old_rno[0] ^= 1;

    if (ewk->wu.old_rno[0]) {
        ewk->wu.position_z = oya_ptr->position_z - 2;
        return;
    }

    ewk->wu.position_z = oya_ptr->position_z + 2;
}

static void advance_eff09_27000_landing(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos < -8) {
            ewk->wu.routine_no[1]++;
            ewk->wu.xyz[1].cal = -0x80000;
            ewk->wu.mvxy.a[0].sp = -0x38000;
            ewk->wu.mvxy.d[0].sp = 0;

            if (ewk->wu.rl_flag) {
                ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
            }

            set_char_move_init2(&ewk->wu, 0, 107, 2, 0);
        }
    }
}

static void initialize_eff09_27000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = -8;
    ewk->wu.kage_hy = -11;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 8;
    ewk->wu.mvxy.a[0].sp = -0x80000;
    ewk->wu.mvxy.d[0].sp = 0;
    ewk->wu.mvxy.a[1].sp = 0;
    ewk->wu.mvxy.d[1].sp = -0x6000;

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
    }

    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void advance_eff09_27000_exit(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);

        if (range_x_check3(ewk, 36) == 0) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        }
    }
}

void eff09_27000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_27000(ewk);
        break;

    case 1:
        advance_eff09_27000_landing(ewk);
        pl_eff_trans_entry(ewk);
        break;

    case 2:
        advance_eff09_27000_exit(ewk);
        pl_eff_trans_entry(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void (*eff09_tbl[28])(WORK_Other*) = { eff09_0000,  eff09_1000,  eff09_2000,  eff09_3000,  eff09_4000,  eff09_5000,
                                       eff09_6000,  eff09_7000,  eff09_8000,  eff09_9000,  eff09_10000, eff09_11000,
                                       eff09_12000, eff09_13000, eff09_14000, eff09_15000, eff09_16000, eff09_17000,
                                       eff09_18000, eff09_19000, eff09_20000, eff09_21000, eff09_22000, eff09_23000,
                                       eff09_24000, eff09_25000, eff09_26000, eff09_27000 };
