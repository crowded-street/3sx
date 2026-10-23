/**
 * @file eff61.c
 * Spectator on Sean's stage from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff61.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/ng/eff62.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <stdbool.h>

#define COLOR_CODE 0x144 // NG 0x58
#define START_X 392
#define START_Y 58
#define PRIORITY 77


#define CHAR_BASE 26
#define CHAR_IDLE (CHAR_BASE + 0)
#define CHAR_IDLE_2 (CHAR_BASE + 1)
#define CHAR_WIN_REACTION (CHAR_BASE + 2)
#define CHAR_FACE_LEFT (CHAR_BASE + 3)
#define CHAR_FACE_RIGHT (CHAR_BASE + 4)

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static void eff61_set_char(WORK_Other* ewk, s16 char_index) {
    ewk->wu.disp_flag = 1;
    set_char_move_init(&ewk->wu, 0, char_index);
}

static void eff61_check_round_won(WORK_Other* ewk, bool late) {
    if (!is_round_won()) {
        return;
    }

    ewk->wu.routine_no[1] = 0;

    if (Winner_id == ewk->wu.old_rno[0]) {
        if (late) {
            ewk->wu.routine_no[0] = 4;
            ewk->wu.char_index = CHAR_WIN_REACTION;
        } else {
            ewk->wu.routine_no[0] = 6;
        }
    } else if (late) {
        ewk->wu.routine_no[0] = 7;
    } else {
        ewk->wu.routine_no[0] = 3;
    }
}

static void eff61_start(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.routine_no[1] = 0;
    ewk->wu.old_rno[0] = 0;
    eff61_set_char(ewk, CHAR_IDLE);
    ewk->wu.xyz[0].disp.pos = START_X;
    suzi_sync_pos_set(ewk);
    sort_push_request4(&ewk->wu);
}

static void eff61_idle(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (!is_paused()) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[1]++;
            }
        }

        eff61_check_round_won(ewk, false);
        break;

    case 1:
        eff61_check_round_won(ewk, true);
        ewk->wu.routine_no[1]++;
        eff61_set_char(ewk, CHAR_IDLE_2);
        break;

    case 2:
        eff61_check_round_won(ewk, true);
        ewk->wu.routine_no[1]++;
        break;

    case 3:
        if (is_paused()) {
            break;
        }

        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0) {
            eff61_check_round_won(ewk, true);
        } else {
            ewk->wu.routine_no[0] = 0;
        }

        break;
    }

    disp_pos_trans_entry_s_ng(ewk);
}

static void eff61_turn(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.char_index =
            check_work_position(&ewk->wu, &plw[ewk->wu.old_rno[0]].wu) ? CHAR_FACE_LEFT : CHAR_FACE_RIGHT;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        ewk->wu.routine_no[1]++;
        break;

    case 2:
        if (!is_paused()) {
            char_move(&ewk->wu);
        }

        break;
    }

    disp_pos_trans_entry_s_ng(ewk);
}

static void eff61_win_reaction(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, CHAR_WIN_REACTION);
        break;

    case 1:
        ewk->wu.routine_no[1]++;
        break;

    case 2:
        if (!is_paused()) {
            char_move(&ewk->wu);
        }

        break;
    }

    disp_pos_trans_entry_s_ng(ewk);
}

static void restart_if_new_round(WORK_Other* ewk) {
    if (ewk->wu.routine_no[0] >= 3 && !is_round_over()) {
        ewk->wu.routine_no[0] = 0;
        ewk->wu.routine_no[1] = 0;
    }
}

void effect_ng61_move(WORK_Other* ewk) {
    restart_if_new_round(ewk);

    switch (ewk->wu.routine_no[0]) {
    case 0:
        eff61_start(ewk);
        break;

    case 1:
        eff61_idle(ewk);
        break;

    case 3:
        eff61_turn(ewk);
        break;

    case 4:
        eff61_win_reaction(ewk);
        break;

    default:
        // Holds the current frame
        disp_pos_trans_entry_s_ng(ewk);
        break;
    }
}

s32 effect_ng61_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG61_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.xyz[0].disp.pos = START_X;
    ewk->wu.xyz[1].disp.pos = START_Y;
    ewk->wu.my_priority = ewk->wu.position_z = PRIORITY;
    ewk->wu.char_index = CHAR_IDLE;
    ewk->wu.hit_stop = 1;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _ng_nyc_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    effect_ng62_init(ewk);
    return 0;
}
