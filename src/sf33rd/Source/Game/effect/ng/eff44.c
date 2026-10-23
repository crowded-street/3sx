/**
 * @file eff44.c
 * Buildings of Sean's stage from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff44.h"
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

typedef struct Eff44Data {
    s16 dead_f;
    s16 my_family;
    s16 my_col_code;
    s16 x;
    s16 y;
    s16 priority;
    s16 char_index;
    s16 animate;
    s16 sync_suzi;
} Eff44Data;

static const Eff44Data eff44_data_tbl[14] = {
    { 0, 3, 300, 512, 80, 100, 6, 1, 0 },   { 0, 3, 300, 512, 96, 100, 7, 0, 0 },
    { 1, 2, 8492, 260, 320, 100, 0, 0, 0 }, { 0, 2, 8492, 528, 8, 95, 1, 0, 0 },
    { 0, 5, 8492, 512, 124, 104, 2, 0, 0 }, { 0, 5, 300, 512, 112, 103, 3, 0, 0 },
    { 0, 2, 8492, 326, 252, 99, 8, 0, 0 },  { 0, 2, 8492, 398, 288, 101, 9, 0, 0 },
    { 0, 2, 8492, 144, 112, 93, 11, 0, 0 }, { 0, 2, 8492, 272, 112, 93, 12, 0, 0 },
    { 0, 2, 8492, 352, 112, 93, 13, 0, 0 }, { 0, 2, 8492, 624, 112, 93, 14, 0, 0 },
    { 0, 2, 8492, 720, 112, 93, 15, 0, 0 }, { 1, 2, 8492, 832, 112, 93, 16, 0, 0 },
};

void effect_ng44_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        if (ewk->wu.hit_stop && !EXE_flag && !Game_pause) {
            char_move(&ewk->wu);
        }

        // Only the first building is drawn wherever the camera is
        if (ewk->wu.type == 0 || range_x_check_ng(ewk)) {
            disp_pos_trans_entry_s_ng(ewk);
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng44_init() {
    WORK_Other* ewk;
    s16 ix;

    for (s16 i = 0; i < SDL_arraysize(eff44_data_tbl); i++) {
        const Eff44Data* data = &eff44_data_tbl[i];

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG44_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4000; // Without NG's 6bpp flag (0x200)
        ewk->wu.type = i;
        ewk->wu.dead_f = data->dead_f;
        ewk->wu.my_family = data->my_family;
        ewk->wu.my_col_code = data->my_col_code;
        ewk->wu.xyz[0].disp.pos = data->x;
        ewk->wu.xyz[1].disp.pos = data->y;
        ewk->wu.my_priority = ewk->wu.position_z = data->priority;
        ewk->wu.char_index = data->char_index;
        ewk->wu.hit_stop = data->animate;
        ewk->wu.sync_suzi = data->sync_suzi;
        ewk->wu.char_table[0] = _ng_eff44_char_table;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
        suzi_offset_set(ewk);
    }

    return 0;
}
