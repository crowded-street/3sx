/**
 * @file eff97.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff97.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

static s32 game_is_active(void) {
    return !EXE_flag && !Game_pause;
}


static s32 player_reached_target_state(const WORK_Other* ewk) {
    return plw[ewk->master_id].wu.routine_no[2] == 1 &&
           plw[ewk->master_id].wu.routine_no[3] == 0;
}


/* Show the effect and start its first animation. */
static void e97_start(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.disp_flag = 1;
    set_char_move_init(&ewk->wu, 0, 44);
}

/* Run the first animation until the player reaches the state this effect waits
 * for, then arm the turn timer. */
static void e97_await_player(WORK_Other* ewk) {
    if (game_is_active()) {
        char_move(&ewk->wu);

        if (player_reached_target_state(ewk)) {
            ewk->wu.routine_no[0]++;
            ewk->wu.old_rno[0] = 16;
        }
    }
}

/* Turn the effect around and launch it: up, and away from the player's side. */
static void e97_launch(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.rl_flag ^= 1;
    set_char_move_init(&ewk->wu, 0, 45);
    ewk->wu.old_rno[0] = 16;
    ewk->wu.mvxy.a[1].sp = 0xE8000;
    ewk->wu.mvxy.d[1].sp = -0x6000;

    if (plw[ewk->master_id].wu.id) {
        ewk->wu.mvxy.a[0].sp = -0xA8000;
        ewk->wu.mvxy.d[0].sp = -0x1000;
    } else {
        ewk->wu.mvxy.a[0].sp = 0xA8000;
        ewk->wu.mvxy.d[0].sp = 0x1000;
    }
}

/* Count the turn timer down and launch once it has run. */
static void e97_await_turn(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] > 0) {
            e97_launch(ewk);
        }
    }
}

/* Fly until the flight timer runs out, then hide the effect. */
static void e97_fly(WORK_Other* ewk) {
    if (!EXE_flag && !Game_pause) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] > 0) {
            add_x_sub(&ewk->wu);
            add_y_sub(&ewk->wu);
        } else {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 0;
        }
    }
}

/* Place the effect and hand it to the renderer - the tail states 1 to 3 share. */
static void e97_sync_and_push(WORK_Other* ewk) {
    suzi_sync_pos_set(ewk);
    sort_push_request4(&ewk->wu);
}

void effect_97_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        e97_start(ewk);
        /* fallthrough */

    case 1:
        e97_await_player(ewk);
        e97_sync_and_push(ewk);
        break;

    case 2:
        e97_await_turn(ewk);
        e97_sync_and_push(ewk);
        break;

    case 3:
        e97_fly(ewk);
        e97_sync_and_push(ewk);
        break;

    case 4:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_97_init(PLW* oya) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 97;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->my_master = oya;
    ewk->master_id = oya->wu.id;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = oya->wu.my_col_code;
    ewk->wu.my_family = 2;
    ewk->wu.my_priority = ewk->wu.position_z = oya->wu.my_priority;
    ewk->wu.sync_suzi = 0;
    ewk->wu.xyz[1].disp.pos = 40;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 0;
    ewk->wu.kage_hy = 40;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 20;
    *ewk->wu.char_table = _etc_char_table;

    if (oya->wu.id) {
        ewk->wu.rl_flag = 0;
        ewk->wu.xyz[0].disp.pos = oya->wu.xyz[0].disp.pos - 108;
    } else {
        ewk->wu.rl_flag = 1;
        ewk->wu.xyz[0].disp.pos = oya->wu.xyz[0].disp.pos + 108;
    }

    ewk->wu.my_mts = 14;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    return 0;
}
