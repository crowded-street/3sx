/**
 * @file eff73.c
 * Kitten next to the cat in Oro's cave from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff73.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x2144 // NG 0x2058
#define CHAR_IDLE 17
#define CHAR_REACTION 18
#define CG_TYPE_REACTION_DONE 2

static void eff73_idle(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_IDLE);
        break;

    case 1:
        char_move(&ewk->wu);
        break;
    }
}

static void eff73_react(WORK_Other* ewk) {
    if (ewk->wu.cg_type == CG_TYPE_REACTION_DONE) {
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, CHAR_REACTION);
        break;

    case 1:
        char_move(&ewk->wu);
        break;
    }
}

void effect_ng73_move(WORK_Other* ewk) {
    const WORK* master = ewk->my_master;

    if (compel_dead_check(ewk)) {
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (master->routine_no[0] != ewk->wu.routine_no[0]) {
        ewk->wu.routine_no[0] = master->routine_no[0];
        ewk->wu.routine_no[1] = 0;
    }

    if (!EXE_flag && !Game_pause) {
        if (ewk->wu.routine_no[0] == 0) {
            eff73_idle(ewk);
        } else if (ewk->wu.routine_no[0] == 1) {
            eff73_react(ewk);
        }
    }

    // NG skips the check of `obr_no_disp_check` here
    if (range_x_check_ng(ewk)) {
        suzi_sync_pos_set(ewk);
        sort_push_request4(&ewk->wu);
    }
}

s32 effect_ng73_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG73_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->my_master = master;
    ewk->wu.dead_f = 1;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.my_priority = ewk->wu.position_z = 75;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _ng_brz_char_table;
    ewk->wu.xyz[0].disp.pos = 493;
    ewk->wu.xyz[1].disp.pos = 81;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    return 0;
}
