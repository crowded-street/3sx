/**
 * @file eff88.c
 * Extra stage objects from New Generation.
 */

#include "sf33rd/Source/Game/effect/ng/eff88.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

#include <SDL3/SDL.h>

typedef struct Eff88Variant {
    const s16 (*objects)[9];
    s16 object_count;
} Eff88Variant;

static const s16 eff88_ryu_b_data[3][9] = {
    { 0, 2, 300, -96, 112, 82, 32, 0, 1 },
    { 0, 2, 300, 79, 129, 82, 30, 0, 1 },
    { 0, 2, 300, 208, 65, 82, 31, 0, 1 },
};

static const s16 eff88_elena_b_data_0[4][9] = {
    { 1, 2, 8524, 265, 89, 80, 6, 0, 0 },
    { 1, 2, 8524, 327, 88, 81, 8, 0, 0 },
    { 1, 2, 8524, 272, 80, 72, 9, 0, 0 },
    { 1, 2, 8524, 352, 56, 71, 16, 1, 0 },
};

static const s16 eff88_elena_b_data_2[3][9] = {
    { 1, 2, 8524, 265, 89, 80, 6, 0, 0 },
    { 1, 2, 8524, 327, 88, 81, 8, 0, 0 },
    { 1, 2, 8524, 319, 65, 71, 11, 1, 0 },
};

static const s16 eff88_elena_b_data_3[1][9] = {
    { 1, 2, 8524, 319, 65, 71, 11, 1, 0 },
};

#define EFF88_VARIANT(data) { data, SDL_arraysize(data) }

static const Eff88Variant eff88_variants[5][4] = {
    [1] = { EFF88_VARIANT(eff88_elena_b_data_0),
            EFF88_VARIANT(eff88_elena_b_data_0),
            EFF88_VARIANT(eff88_elena_b_data_2),
            EFF88_VARIANT(eff88_elena_b_data_3) },
    [4] = { EFF88_VARIANT(eff88_ryu_b_data),
            EFF88_VARIANT(eff88_ryu_b_data),
            EFF88_VARIANT(eff88_ryu_b_data),
            EFF88_VARIANT(eff88_ryu_b_data) },
};

s32 effect_ng88_init(s16 variant) {
    const Eff88Variant* data = &eff88_variants[variant][bg_w.object_set];
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < data->object_count; i++) {
        const s16* object = data->objects[i];

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG88_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.char_table[0] = char_add[bg_w.bg_index];
        ewk->wu.dead_f = object[0];
        ewk->wu.my_family = object[1];
        ewk->wu.my_col_code = object[2];
        ewk->wu.xyz[0].disp.pos = object[3];
        ewk->wu.xyz[1].disp.pos = object[4];
        ewk->wu.my_priority = ewk->wu.position_z = object[5];
        ewk->wu.char_index = object[6];
        ewk->wu.hit_stop = object[7]; // Animate every frame
        ewk->wu.sync_suzi = object[8];
        suzi_offset_set(ewk);
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
