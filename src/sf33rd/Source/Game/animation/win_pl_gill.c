/**
 * @file win_pl_gill.c
 * Gill's Winning Behaviour
 *
 * Split out of win_pl.c: the three jijii sequences Gill runs after his
 * winning pose, reached from Win_01000.
 */

#include "sf33rd/Source/Game/animation/win_pl_gill.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

void jijii_nebukuro(PLW* wk) {
    bg_app_stop = 1;

    switch (win_rno[1]) {
    case 0:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            win_rno[1]++;
            char_move_z(&wk->wu);
            wk->wu.mvxy.a[1].sp = 0xF0000;
            wk->wu.mvxy.d[1].sp = -0x600;
        }

        break;

    case 1:
        if (wk->wu.cg_type != 2) {
            char_move(&wk->wu);
        }

        add_y_sub((WORK_Other*)wk);

        if (wk->wu.xyz[1].disp.pos > 256) {
            win_rno[1]++;
            win_sp_flag = 2;
            set_char_move_init(&wk->wu, 9, 40);
            wk->wu.xyz[1].disp.pos = 200;
        }

        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

static void jijii_jump_launch(PLW* wk) {
    char_move(&wk->wu);
    if (wk->wu.cg_type == 9) {
        win_rno[1]++;

        if (wk->wu.rl_flag) {
            wk->wu.mvxy.a[0].sp = 0x60000;
            wk->wu.mvxy.d[0].sp = 0x1000;
        } else {
            wk->wu.mvxy.a[0].sp = -0x60000;
            wk->wu.mvxy.d[0].sp = -0x1000;
        }

        wk->wu.mvxy.a[1].sp = 0xA0000;
        wk->wu.mvxy.d[1].sp = -0x600;
    }
}

static void jijii_jump_fly_off(PLW* wk) {
    if (wk->wu.cg_type != 99) {
        char_move(&wk->wu);
    }

    add_x_sub((WORK_Other*)wk);
    add_y_sub((WORK_Other*)wk);

    if (wk->wu.rl_flag) {
        if (wk->wu.xyz[0].disp.pos > bg_w.bgw[1].xy[0].disp.pos + 320) {
            win_rno[1]++;
            effect_work_kill(3, 13);
        }

        return;
    }

    if (wk->wu.xyz[0].disp.pos < bg_w.bgw[1].xy[0].disp.pos - 320) {
        win_rno[1]++;
        effect_work_kill(3, 13);
    }
}

static void jijii_jump_return(PLW* wk) {
    win_rno[1]++;

    if (wk->wu.rl_flag) {
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].xy[0].disp.pos - 328;
        wk->wu.mvxy.a[0].sp = 0x18000;
    } else {
        wk->wu.xyz[0].disp.pos = bg_w.bgw[1].xy[0].disp.pos + 328;
        wk->wu.mvxy.a[0].sp = -0x18000;
    }

    wk->wu.mvxy.d[0].sp = 0;
    wk->wu.xyz[1].cal = 0;
}

void jijii_jump(PLW* wk) {
    s16 id_w;

    bg_app_stop = 1;
    id_w = wk->wu.id ^ 1;
    wk->wu.position_z = plw[id_w].wu.position_z - 1;
    wk->wu.my_priority = wk->wu.position_z;

    switch (win_rno[1]) {
    case 0:
        jijii_jump_launch(wk);

        break;

    case 1:
        jijii_jump_fly_off(wk);

        break;

    case 2:
        win_rno[1]++;
        set_char_move_init2(&wk->wu, 9, 36, 7, 0);
        win_free[wk->wu.id] = 48;
        break;

    case 3:
        win_free[wk->wu.id]--;

        if (win_free[wk->wu.id] > 0) {
            break;
        }

        jijii_jump_return(wk);
        /* fallthrough */

    case 4:
        add_x_sub((WORK_Other*)wk);
        char_move(&wk->wu);

        break;
    }
}

void jijii_full(PLW* wk) {
    bg_app_stop = 1;

    switch (win_rno[1]) {
    case 0:
        char_move(&wk->wu);
        if (wk->wu.cmwk[0] == 1) {
            win_rno[1]++;
            break;
        }

        break;

    case 1:
        char_move(&wk->wu);
        wk->wu.xyz[1].cal += 0x10000;

        if (wk->wu.xyz[1].disp.pos >= 42) {
            win_rno[1]++;
            wk->wu.cmwk[0] = 2;
            set_char_move_init(&wk->wu, 9, 43);
            break;
        }

        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}
