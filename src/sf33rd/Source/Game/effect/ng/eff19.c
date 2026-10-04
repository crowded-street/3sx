/**
 * @file eff19.c
 * Lava details of Gill's stage from New Generation (effect 19 in the NG arcade ROM). They play their animation once
 * when the master lava flow (effect 47) reaches certain frames.
 */

#include "sf33rd/Source/Game/effect/ng/eff19.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define OBJECT_COUNT 7

/// NG gives these objects a char table that starts 11 animations into Gill's stage table. 3S char tables hold offsets
/// from their own start, so the full table is used with the animations offset instead.
#define FIRST_CHAR_INDEX 11

typedef struct Eff19Data {
    s16 x;
    s16 y;
    s16 priority;
} Eff19Data;

static const Eff19Data eff19_data_tbl[OBJECT_COUNT] = {
    { 256, 256, 78 }, { 208, 208, 78 }, { 256, 160, 78 }, { 672, 160, 85 },
    { 568, 152, 78 }, { 432, 96, 79 },  { 96, 192, 94 },
};

/// Shows the object when the master flow reaches the object's frame. Object 0 never shows.
static void eff19_check_start(WORK_Other* ewk) {
    const WORK_Other* master = (WORK_Other*)ewk->my_master;

    switch (master->wu.cg_ix) {
    case 0:
        if (ewk->wu.type < 4) {
            ewk->wu.disp_flag = 1;
        }

        break;

    case 12:
        if (ewk->wu.type == 4) {
            ewk->wu.disp_flag = 1;
        }

        break;

    case 16:
        if (ewk->wu.type > 4) {
            ewk->wu.disp_flag = 1;
        }

        break;
    }
}

/// Plays the animation while the object shows. At the end of the animation, hides it and rewinds.
static void eff19_animate(WORK_Other* ewk) {
    if (!ewk->wu.disp_flag) {
        return;
    }

    char_move(&ewk->wu);

    if (ewk->wu.cg_type) {
        ewk->wu.cg_type = 0;
        ewk->wu.disp_flag = 0;
        char_move_z(&ewk->wu);
    }
}

void effect_ng19_move(WORK_Other* ewk) {
    if (compel_dead_check(ewk)) {
        ewk->wu.routine_no[0] = 99;
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 0;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.direction = ((WORK_Other*)ewk->my_master)->wu.cg_ix;
        ewk->wu.dead_f = 1;
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            if (ewk->wu.type == 0) {
                ewk->wu.disp_flag = 0;
            } else {
                eff19_check_start(ewk);
                eff19_animate(ewk);
            }
        }

        if (!obr_no_disp_check()) {
            sort_push_request4(&ewk->wu);
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng19_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;
    s16 i;

    // NG also pulls one work here that it never uses or gives back
    for (i = 0; i < OBJECT_COUNT; i++) {
        const Eff19Data* data = &eff19_data_tbl[i];

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG19_ID;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.work_id = 16;
        ewk->my_master = master;
        ewk->wu.type = i;
        ewk->wu.char_index = FIRST_CHAR_INDEX + i;
        ewk->wu.my_family = 2;
        ewk->wu.position_x = data->x;
        ewk->wu.position_y = data->y;
        ewk->wu.my_priority = ewk->wu.position_z = data->priority;
        ewk->wu.char_table[0] = _ng_fnl_char_table;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.my_col_code = 8492;
        ewk->wu.sync_suzi = 0;
        ewk->wu.cg_ix = 0;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    }

    return 0;
}
