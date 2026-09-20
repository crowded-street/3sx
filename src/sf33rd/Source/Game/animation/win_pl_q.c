/**
 * @file win_pl_q.c
 * Q's Winning Behaviour
 *
 * Split out of win_pl.c: the keep-distance and leave-the-screen actions Q
 * runs after his winning pose, reached from Win_10000.
 */

#include "sf33rd/Source/Game/animation/win_pl_q.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

const s16 q_em_distance_tbl[20][2] = { { -96, -16 }, { -104, 0 },  { -90, -16 }, { -100, -8 }, { -100, 0 },
                                       { -106, 0 },  { 12, -117 }, { -84, -21 }, { -112, 0 },  { -106, 4 },
                                       { -100, 0 },  { -90, -16 }, { -90, -16 }, { -96, -16 }, { -90, -16 },
                                       { -90, -16 }, { 0, -96 },   { -2, -112 }, { -112, 4 },  { -96, -6 } };

/* The distance test both facings share, character for character: the gap this
 * opponent and this facing pair ask for. */
static s16 q_em_far_enough(s16 work, s16 id_w, s16 rl_w) {
    if (work >= q_em_distance_tbl[plw[id_w].player_number][rl_w]) {
        return 1;
    }

    return 0;
}

/* The facing-right side of the distance check. Only this arm is lifted; either
 * one alone measures 10.00. */
static s16 q_em_far_enough_facing_right(PLW* wk, s16 id_w, s16 rl_w) {
    s16 work;

    work = wk->wu.xyz[0].disp.pos - plw[id_w].wu.xyz[0].disp.pos;

    if (q_em_far_enough(work, id_w, rl_w)) {
        return 1;
    }

    return 0;
}

s16 q_em_distance_chk(PLW* wk) {
    s16 work;
    s16 id_w = wk->wu.id ^ 1;
    s16 rl_w = wk->wu.rl_flag ^ plw[id_w].wu.rl_flag;

    if (wk->wu.rl_flag) {
        return q_em_far_enough_facing_right(wk, id_w, rl_w);
    } else {
        work = plw[id_w].wu.xyz[0].disp.pos - wk->wu.xyz[0].disp.pos;

        if (q_em_far_enough(work, id_w, rl_w)) {
            return 1;
        }
    }

    return 0;
}

s32 q_em_dir(PLW* wk) {
    s16 work;
    s16 pos_w;
    s16 id_w = wk->wu.id ^ 1;

    work = wk->wu.xyz[0].disp.pos - plw[id_w].wu.xyz[0].disp.pos;

    if (work < 0) {
        wk->wu.direction = 1;
    } else {
        wk->wu.direction = 0;
    }

    pos_w = wk->wu.xyz[0].disp.pos;

    if (q_em_distance_chk(wk)) {
        if (win_rno[0] == 3) {
            win_rno[0] = 1;
            set_char_move_init(&wk->wu, 9, 36);
        } else if (win_rno[0] == 4) {
            win_rno[0] = 2;
            set_char_move_init(&wk->wu, 9, 36);
        }

        wk->wu.direction = wk->wu.rl_flag;
        win_rno[1] = 4;
        wk->wu.xyz[0].disp.pos = pos_w;
        return 0;
    }

    wk->wu.xyz[0].disp.pos = pos_w;
    return 1;
}

/* The second half of Q's keep-distance action: everything from the dash
 * onwards. The case labels are the original ones, so the states still read as
 * the same numbers. */
static void q_keep_dash_back(PLW* wk) {
    switch (win_rno[1]) {
    case 2:
        win_rno[1]++;
        set_char_move_init(&wk->wu, 9, 41);
        wk->wu.mvxy.d[0].sp = 0;

        if (wk->wu.rl_flag) {
            wk->wu.mvxy.a[0].sp = 0x1C000;
            break;
        }

        wk->wu.mvxy.a[0].sp = -0x1C000;
        break;

    case 3:
        char_move(&wk->wu);
        add_x_sub((WORK_Other*)wk);

        if (!q_em_distance_chk(wk)) {
            break;
        }

        win_rno[1]++;

        if (win_rno[0] == 1) {
            set_char_move_init(&wk->wu, 9, 36);
            break;
        }

        set_char_move_init(&wk->wu, 9, 37);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void q_keeping_action(PLW* wk) {
    switch (win_rno[1]) {
    case 0:
        if (!q_em_dir(wk)) {
            break;
        }

        if (wk->wu.direction == wk->wu.rl_flag) {
            win_rno[1] = 2;
            break;
        }

        win_rno[1] = 1;
        set_char_move_init(&wk->wu, 9, 40);
        wk->wu.rl_flag ^= 1;
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            win_rno[1]++;
            break;
        }

        break;

    default:
        q_keep_dash_back(wk);
        break;
    }
}

static void q_start_dash_away(PLW* wk) {
    set_char_move_init(&wk->wu, 9, 41);
    wk->wu.mvxy.d[0].sp = 0;

    if (wk->wu.rl_flag) {
        wk->wu.mvxy.a[0].sp = 0x1C000;
    } else {
        wk->wu.mvxy.a[0].sp = -0x1C000;
    }
}

static void q_leave_turn_to_face(PLW* wk) {
    if (q_em_dir(wk) == 0) {
        return;
    }

    if (wk->wu.direction == wk->wu.rl_flag) {
        win_rno[1] = 2;
    } else {
        win_rno[1] = 1;
        set_char_move_init(&wk->wu, 9, 40);
        wk->wu.rl_flag ^= 1;
    }
}

static void q_leave_await_turn(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 0xFF) {
        win_rno[1]++;
    }
}

static void q_leave_dash_to_range(PLW* wk) {
    char_move(&wk->wu);
    add_x_sub((WORK_Other*)wk);

    if (q_em_distance_chk(wk)) {
        win_rno[1]++;

        if (win_rno[0] == 2) {
            set_char_move_init(&wk->wu, 9, 36);
        } else {
            set_char_move_init(&wk->wu, 9, 39);
        }
    }
}

static void q_leave_await_pose(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type == 0xFF) {
        win_rno[1]++;
        q_start_dash_away(wk);
    }
}

static void q_leave_walk_off_screen(PLW* wk) {
    s16 work;

    char_move(&wk->wu);
    add_x_sub((WORK_Other*)wk);

    if (wk->wu.rl_flag) {
        work = bg_w.bgw[1].wxy[0].disp.pos + bg_w.pos_offset;
        work += 64;

        if (work < wk->wu.xyz[0].disp.pos) {
            win_rno[1]++;
        }

        return;
    }

    work = bg_w.bgw[1].wxy[0].disp.pos - bg_w.pos_offset;
    work -= 64;

    if (work > wk->wu.xyz[0].disp.pos) {
        win_rno[1]++;
    }
}

void q_leave_after_action(PLW* wk) {
    switch (win_rno[1]) {
    case 0:
        q_leave_turn_to_face(wk);
        break;

    case 1:
        q_leave_await_turn(wk);
        break;

    case 2:
        win_rno[1]++;
        q_start_dash_away(wk);

        break;

    case 3:
        q_leave_dash_to_range(wk);
        break;

    case 4:
        q_leave_await_pose(wk);
        break;

    case 5:
        q_leave_walk_off_screen(wk);
        break;
    }
}
