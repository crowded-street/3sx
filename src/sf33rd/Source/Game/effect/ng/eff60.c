/**
 * @file eff60.c
 * Blinking stage objects from New Generation (effect 60 in the NG arcade ROM). Unlike 3S's effect 60, they always
 * blink and have no blink timer.
 */

#include "sf33rd/Source/Game/effect/ng/eff60.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

/// Same layout as the effect 05 records: dead_f, my_family, my_col_code, x, y, priority, char_index, animate,
/// sync_suzi. Colour codes are converted from NG's stage palette rows like in eff64.c.
static const s16 ng_eff60_data_tbl[2][9] = {
    // Stage 2
    { 1, 2, 8492, 272, 96, 86, 61, 1, 0 },

    // Alex's stage, after the car breaks
    { 1, 2, 8492, 304, 48, 76, 32, 1, 0 },
};

void effect_ng60_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.disp_flag = 2;
        ewk->wu.blink_timing = 1;
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if (!EXE_flag && !Game_pause && !EXE_obroll && ewk->wu.hit_stop) {
            char_move(&ewk->wu);
        }

        disp_pos_trans_entry_rs(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng60_init(s16 variant) {
    WORK_Other* ewk;
    s16 ix;
    const s16* data = ng_eff60_data_tbl[variant];

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG60_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.type = variant;
    ewk->wu.dead_f = data[0];
    ewk->wu.my_family = data[1];
    ewk->wu.my_col_code = data[2];
    ewk->wu.xyz[0].disp.pos = data[3];
    ewk->wu.xyz[1].disp.pos = data[4];
    ewk->wu.my_priority = ewk->wu.position_z = data[5];
    ewk->wu.char_index = data[6];
    ewk->wu.hit_stop = data[7];
    ewk->wu.sync_suzi = data[8];
    ewk->wu.char_table[0] = char_add[bg_w.bg_index];
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
