/**
 * @file eff72.c
 * Cat lying in Oro's cave from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff72.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/ng/eff73.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x2144 // NG 0x2058
#define CHAR_IDLE 13
#define CHAR_REACTION 51

static void eff72_idle(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_IDLE);
        break;

    case 1:
        if (is_special_move_active()) {
            ewk->wu.routine_no[0] = 1;
            ewk->wu.routine_no[1] = 0;
            break;
        }

        char_move(&ewk->wu);
        break;
    }
}

static void eff72_react(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        set_char_move_init(&ewk->wu, 0, CHAR_REACTION);
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.routine_no[1] = 0;
        }

        break;
    }
}

void effect_ng72_move(WORK_Other* ewk) {
    void (*const states[2])(WORK_Other*) = { eff72_idle, eff72_react };

    if (!EXE_flag && !Game_pause) {
        states[ewk->wu.routine_no[0]](ewk);
    }

    disp_pos_trans_entry_rs_ng(ewk);
}

s32 effect_ng72_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG72_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.my_priority = ewk->wu.position_z = 75;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _ng_brz_char_table;
    ewk->wu.xyz[0].disp.pos = 411;
    ewk->wu.xyz[1].disp.pos = 80;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    effect_ng73_init(ewk);
    return 0;
}
