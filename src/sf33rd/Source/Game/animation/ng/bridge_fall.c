/**
 * @file bridge_fall.c
 * Players falling from the bridge of Elena's stage in New Generation.
 */

#include "sf33rd/Source/Game/animation/ng/bridge_fall.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/manage.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/ng/bg370.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

#define FALL_FRAMES 110
#define PAT_STATUS_LYING 0x26
#define LYING_Y_OFFSET -0x28

// The jump in the air. NG plays its own fall (koc 9 0x3D), which only NG's characters have.
#define FALL_KOC 0
#define FALL_CHAR_INDEX 15

static void teeter(PLW* wk) {
    if (wk->wu.now_koc != 0 || wk->wu.char_index != 0) {
        set_char_move_init(&wk->wu, 0, 0);
    }
}

static bool is_attack_held(s16 id) {
    return ((id == 0 ? p1sw_0 : p2sw_0) & SWK_ATTACKS) != 0;
}

static void fall(PLW* wk, s8* step, s16* timer, s32 speed_y, bool is_winner) {
    switch (*step) {
    case 0:
        if (bg_w.bgw[1].r_no_1 < 1) {
            char_move(&wk->wu);
            break;
        }

        (*step)++;
        set_char_move_init(&wk->wu, FALL_KOC, FALL_CHAR_INDEX);
        wk->wu.mvxy.a[1].sp = speed_y;
        wk->wu.mvxy.d[1].sp = -0x4000;
        *timer = FALL_FRAMES;
        break;

    case 1:
        // Attack buttons hurry the round on
        if (is_winner && is_attack_held(wk->wu.id)) {
            C_Timer--;
        }

        if (--*timer < 0) {
            wk->wu.routine_no[3]++;
        } else {
            add_y_sub_ng(&wk->wu);
        }

        /* fallthrough */

    case 2:
        char_move(&wk->wu);
        wk->wu.kage_flag = 0;
        break;
    }
}

bool bridge_fall_winner(PLW* wk) {
    BGBridge* bridge = &bg_w.bridge;

    if (!bg370_is_on_bridge() || save_w[Present_Mode].Battle_Number[Play_Type] <= 0) {
        return false;
    }

    bg_app_stop = 1;

    switch (wk->wu.routine_no[3]) {
    case 0:
        complete_victory_pause();
        wk->wu.routine_no[3]++;
        teeter(wk);
        bridge->winner_step = 0;
        break;

    case 1:
    case 9:
        fall(wk, &bridge->winner_step, &bridge->winner_timer[wk->wu.id], 0x68000, true);
        break;
    }

    return true;
}

bool bridge_fall_loser(PLW* wk) {
    BGBridge* bridge = &bg_w.bridge;

    if (!bg370_is_on_bridge()) {
        return false;
    }

    if (wk->wu.routine_no[3] == 0) {
        wk->wu.routine_no[3]++;
        bridge->loser_step = 0;
        bridge->loser_fall_step = 0;
        bridge->loser_lying = 0;
    }

    switch (bridge->loser_step) {
    case 0:
        bridge->loser_step++;
        bridge->loser_fall_step = 0;

        if (wk->wu.pat_status == PAT_STATUS_LYING) {
            bridge->loser_lying = 1;
            wk->wu.kage_flag = 0;
        } else {
            bridge->loser_lying = 0;
            teeter(wk);
        }

        break;

    case 1:
        if (bridge->loser_lying) {
            wk->wu.kage_flag = 0;
            wk->wu.xyz[1].disp.pos = wk->wu.position_y = bridge->y + LYING_Y_OFFSET;
        } else {
            // Unlike the winner's, NG's loser fall doesn't stop when the fall is over
            fall(wk, &bridge->loser_fall_step, &bridge->loser_timer[wk->wu.id], 0x60000, false);
        }

        break;
    }

    return true;
}
