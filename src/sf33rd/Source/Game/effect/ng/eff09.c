/**
 * @file eff09.c
 * Short animations that stage objects from New Generation start from their char tables. 
 * Like 3S's effect 09 with its first routine, but with NG's data and draw range.
 */

#include "sf33rd/Source/Game/effect/ng/eff09.h"
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

#include <SDL3/SDL.h>

typedef struct Eff09Data {
    s16 family;
    s16 col_code;
    s16 x;
    s16 y;
    s16 priority;
    s16 char_index;
    s16 animate;
    s16 sync_suzi;
} Eff09Data;

static const Eff09Data eff09_data[4] = {
    { 2, 8492, 383, 48, 70, 0, 1, 0 },
    { 2, 8517, 253, 130, 74, 1, 1, 0 },
    { 2, 8517, 252, 135, 74, 2, 1, 0 },
    { 2, 8517, 257, 130, 74, 3, 1, 0 },
};

void effect_ng09_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[1] = 99;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        ewk->wu.routine_no[1]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.dead_f = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (!EXE_flag && !Game_pause && ewk->wu.hit_stop) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[1]++;
                ewk->wu.disp_flag = 0;
            }
        }

        disp_pos_trans_entry_rs_ng(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng09_init(WORK* /* unused */, u8 data) {
    const Eff09Data* object;
    WORK_Other* ewk;
    s16 ix;

    if (data >= SDL_arraysize(eff09_data)) {
        return 0;
    }

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    object = &eff09_data[data];
    ewk = (WORK_Other*)frw[ix];
    ewk->wu.type = data;
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG09_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.char_table[0] = _ng_eff09_char_table;
    ewk->wu.my_family = object->family;
    ewk->wu.my_col_code = object->col_code;
    ewk->wu.xyz[0].disp.pos = object->x;
    ewk->wu.xyz[1].disp.pos = object->y;
    ewk->wu.my_priority = ewk->wu.position_z = object->priority;
    ewk->wu.char_index = object->char_index;
    ewk->wu.hit_stop = object->animate;
    ewk->wu.sync_suzi = object->sync_suzi;
    suzi_offset_set(ewk);
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
