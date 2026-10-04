/**
 * @file eff47.c
 * Lava flows of Gill's stage from New Generation (effect 47 in the NG arcade ROM)
 */

#include "sf33rd/Source/Game/effect/ng/eff47.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define OBJECT_COUNT 5
#define FRAME_COUNT 9

/// NG CG 0x6390, the first lava CG, in texture group 102
#define FIRST_CG 38332

typedef struct Eff47Data {
    /// Converted from NG's stage palette row 0x40 to the PS2 BG palette row 0x12C. NG ignores it and draws every flow
    /// with row 0x40 plus the part palettes, which is the same thing on PS2.
    s16 my_col_code;

    s16 x;
    s16 y;
    s16 priority;
} Eff47Data;

static const Eff47Data eff47_data_tbl[OBJECT_COUNT] = {
    { 8492, 288, 96, 84 }, { 8492, 416, 64, 84 }, { 300, 480, 64, 84 }, { 300, 544, 80, 84 }, { 300, 672, 112, 84 },
};

/// Each flow's CGs, as offsets from NG CG 0x6390
static const s16 eff47_cg_tbl[OBJECT_COUNT][FRAME_COUNT] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8 },          { 9, 10, 11, 12, 13, 14, 15, 16, 17 },
    { 18, 19, 20, 21, 22, 23, 24, 25, 26 }, { 64, 65, 66, 67, 68, 69, 70, 71, 72 },
    { 73, 74, 75, 76, 77, 78, 79, 80, 80 },
};

static const u8 eff47_wait_tbl[FRAME_COUNT] = { 8, 8, 8, 8, 8, 8, 8, 8, 8 };

static void eff47_animate(WORK_Other* ewk) {
    if (EXE_flag || Game_pause) {
        return;
    }

    if (--ewk->wu.cg_type != 0) {
        return;
    }

    ewk->wu.cg_ix += 2;

    if (ewk->wu.cg_ix > (FRAME_COUNT - 1) * 2) {
        ewk->wu.cg_ix = 0;
    }

    ewk->wu.cg_type = eff47_wait_tbl[ewk->wu.cg_ix / 2];
}

static void eff47_disp(WORK_Other* ewk) {
    if (obr_no_disp_check() || (ewk->wu.type != 2 && !range_x_check_ng(ewk))) {
        return;
    }

    ewk->wu.cg_number = FIRST_CG + eff47_cg_tbl[ewk->wu.type][ewk->wu.cg_ix / 2];
    sort_push_request4(&ewk->wu);
}

void effect_ng47_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.cg_type = 8;
        ewk->wu.cg_ix = 0;
        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0x3FF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0x3FF;
        break;

    case 1:
        if (!Fade_Flag) {
            ewk->wu.routine_no[0]++;
        }

        /* fallthrough */

    case 2:
        eff47_animate(ewk);
        eff47_disp(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng47_init() {
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    for (i = 0; i < OBJECT_COUNT; i++) {
        const Eff47Data* data = &eff47_data_tbl[i];

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG47_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.char_table[0] = _ng_fnl_char_table;
        ewk->wu.type = i;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_code = data->my_col_code;
        ewk->wu.xyz[0].disp.pos = data->x;
        ewk->wu.xyz[1].disp.pos = data->y;
        ewk->wu.my_priority = ewk->wu.position_z = data->priority;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    // TODO: Port NG effect 19. NG starts it here with the last flow as its master.
    return 0;
}
