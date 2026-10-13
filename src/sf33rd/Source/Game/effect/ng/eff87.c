/**
 * @file eff87.c
 * Extra stage objects from New Generation (effect 87 in the NG arcade ROM). They behave exactly like effect 05's, so
 * they move with effect_05_move.
 */

#include "sf33rd/Source/Game/effect/ng/eff87.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

#include <SDL3/SDL.h>

typedef struct Eff87Variant {
    const s16 (*objects)[9];
    s16 object_count;
} Eff87Variant;

/// First area of Yun's stage. Colour codes are converted from palette row 0x55 to the PS2 BG palette row 0x141.
static const s16 eff87_yun_a_data[1][9] = {
    { 1, 2, 8513, 416, 72, 80, 28, 0, 0 },
};

static const Eff87Variant eff87_variants[] = {
    [2] = { eff87_yun_a_data, SDL_arraysize(eff87_yun_a_data) },
};

s32 effect_ng87_init(s16 variant) {
    const Eff87Variant* data = &eff87_variants[variant];
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
        ewk->wu.id = EFFECT_NG87_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.type = variant;
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
