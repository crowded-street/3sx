/**
 * @file effc2.c
 * Ropes of the bridge on Elena's stage from New Generation.
 */

#include "sf33rd/Source/Game/effect/ng/effc2.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#include <SDL3/SDL.h>

#define COLOR_CODE 0x212C // NG 0x2040

typedef struct Rope {
    s16 x;
    s16 y;
    s16 priority;
    s16 char_index;
} Rope;

static const Rope ropes[] = {
    { 784, 32, 20, 9 },
    { 720, 64, 77, 10 },
    { 240, 32, 20, 11 },
    { 304, 64, 77, 12 },
};

void effect_ngc2_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        if (bg_w.bridge.breaking) {
            ewk->wu.routine_no[0]++;
        }

        disp_pos_trans_entry(ewk);
        break;

    case 2:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[0]++;
            bg_w.bridge.timer = 0;
        }

        disp_pos_trans_entry(ewk);
        break;

    case 3:
        if (bg_w.bgw[1].r_no_1 > 1) {
            ewk->wu.routine_no[0]++;
        }

        disp_pos_trans_entry(ewk);
        break;

    case 4:
        ewk->wu.routine_no[0]++;
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ngc2_init() {
    WORK_Other* ewk;
    s16 ix;

    for (s16 i = 0; i < SDL_arraysize(ropes); i++) {
        const Rope* rope = &ropes[i];

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NGC2_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.disp_flag = 0;
        ewk->wu.charset_id = 0;
        ewk->wu.type = i;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = COLOR_CODE;
        ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = rope->x;
        ewk->wu.xyz[0].disp.low = 0;
        ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = rope->y;
        ewk->wu.xyz[1].disp.low = 0;
        ewk->wu.my_priority = ewk->wu.position_z = rope->priority;
        ewk->wu.char_index = rope->char_index;
        ewk->wu.char_table[0] = _ng_afc_a_char_table;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
