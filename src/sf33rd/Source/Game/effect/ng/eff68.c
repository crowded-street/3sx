/**
 * @file eff68.c
 * Punk pacing in the second area of Dudley's stage from New Generation.
 */

#include "sf33rd/Source/Game/effect/ng/eff68.h"
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

#define PACING_CHAR 17
#define REACTION_CHAR 18
#define STANDING_CG_TYPE 2
#define REACTION_RANGE 32

static bool is_paused() {
    return EXE_flag || Game_pause;
}

static void eff68_pace(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, PACING_CHAR);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == STANDING_CG_TYPE &&
            (is_near(plw[0].wu.position_x, ewk->wu.position_x, REACTION_RANGE) ||
             is_near(plw[1].wu.position_x, ewk->wu.position_x, REACTION_RANGE))) {
            ewk->wu.routine_no[0] = 1;
            ewk->wu.routine_no[1] = 0;
        }

        break;
    }
}

static void eff68_react(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, REACTION_CHAR);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.routine_no[1] = 0;
        }

        break;
    }
}

void effect_ng68_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        if (!is_paused()) {
            eff68_pace(ewk);
        }

        disp_pos_trans_entry_rs_ng(ewk);
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0] = 99;
            break;
        }

        // NG also draws him a second time in this state while the game runs
        if (!is_paused()) {
            eff68_react(ewk);
        }

        disp_pos_trans_entry_s_ng(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng68_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG68_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.dead_f = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = 0x2144; // NG 0x2058
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = 160;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = 47;
    ewk->wu.my_priority = ewk->wu.position_z = 75;
    ewk->wu.sync_suzi = 1;
    ewk->wu.char_table[0] = _ng_eng_char_table; // NG also sets step_xy_table, but his animations never read it
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
