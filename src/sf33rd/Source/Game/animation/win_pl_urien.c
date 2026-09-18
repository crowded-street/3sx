/**
 * @file win_pl_urien.c
 * Urien's Winning Behaviour
 *
 * Split out of win_pl.c: the dash Urien runs after his winning pose, reached
 * from Win_14000.
 */

#include "sf33rd/Source/Game/animation/win_pl_urien.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"

s32 urien_dash_chk(PLW* wk) {
    s16 id_w = wk->wu.id ^ 1;
    s16 pos_w = wk->wu.xyz[0].disp.pos - plw[id_w].wu.xyz[0].disp.pos;

    if (pos_w < 0) {
        pos_w = -pos_w;

        if (!wk->wu.rl_waza) {
            wk->wu.rl_waza = 1;
        }
    } else if (wk->wu.rl_waza) {
        wk->wu.rl_waza = 0;
    }

    if (pos_w < 88) {
        return 1;
    }

    return 0;
}

static void urien_dash_launch(PLW* wk) {
    win_rno[1]++;

    if (urien_dash_chk(wk)) {
        win_rno[1] = 5;
    } else {
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 0, 4);
        setup_mvxy_data(&wk->wu, 2);
    }
}

static void urien_dash_fall(PLW* wk) {
    add_mvxy_speed(&wk->wu);
    cal_mvxy_speed(&wk->wu);
    char_move(&wk->wu);

    if (wk->wu.xyz[1].disp.pos + wk->wu.cg_jphos >= 1) {
        return;
    }

    win_rno[1]++;
    wk->wu.position_y = 0;
    wk->wu.xyz[1].cal = 0;
    wk->wu.mvxy.a[1].sp = 0;
    char_move_cmja(&wk->wu);
}

static void urien_dash_land(PLW* wk) {
    char_move(&wk->wu);

    if (wk->wu.cg_type != 64) {
        return;
    }

    if (urien_dash_chk(wk)) {
        win_rno[1]++;
    } else {
        win_rno[1] = 0;
    }
}

void urien_dash(PLW* wk) {
    switch (win_rno[1]) {
    case 0:
        urien_dash_launch(wk);

        /* fallthrough */

    case 1:
        if (wk->wu.cg_type == 1) {
            win_rno[1]++;
            add_mvxy_speed(&wk->wu);
        } else {
            char_move(&wk->wu);
        }

        break;

    case 2:
        urien_dash_fall(wk);
        break;

    case 3:
        urien_dash_land(wk);
        break;

    case 4:
        char_move(&wk->wu);

        if (wk->wu.cg_type != 0xFF) {
            break;
        }

        win_rno[1]++;
        /* fallthrough */

    case 5:
        win_rno[1]++;
        set_char_move_init(&wk->wu, 9, 36);
        break;

    case 6:
        char_move(&wk->wu);
        break;
    }
}
