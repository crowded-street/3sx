/**
 * @file eff28.c
 * Object of the second area of Yun's stage from New Generation (effect 28 in the NG arcade ROM). Effect 26's first
 * object starts it next to the effect 25 object that it belongs to. It reacts when that object breaks.
 */

#include "sf33rd/Source/Game/effect/ng/eff28.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define CHAR_BASE 76
#define MASTER_HIT_TYPE 1

static bool is_paused() {
    return EXE_flag || Game_pause;
}

/// Switches to the reaction once the master breaks
static bool check_master_broken(WORK_Other* ewk) {
    const WORK_Other* master = (WORK_Other*)ewk->my_master;

    if (master->wu.routine_no[1] > 1) {
        ewk->wu.routine_no[0] = 5;
        set_char_move_init(&ewk->wu, 0, CHAR_BASE + 4);
        return true;
    }

    return false;
}

void effect_ng28_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;

        if (eff_hit_flag[MASTER_HIT_TYPE]) {
            set_char_move_init(&ewk->wu, 0, CHAR_BASE + 5);
            ewk->wu.routine_no[0] = 6;
        } else {
            set_char_move_init(&ewk->wu, 0, CHAR_BASE);
        }

        break;

    case 1:
        if (!is_paused()) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                set_char_move_init(&ewk->wu, 0, CHAR_BASE + 1);
                ewk->wu.routine_no[0]++;
            }
        }

        break;

    case 2:
        if (!is_paused() && !check_master_broken(ewk)) {
            char_move(&ewk->wu);
        }

        break;

    case 3:
    case 4:
        if (!is_paused() && !check_master_broken(ewk)) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[0] = 2;
                set_char_move_init(&ewk->wu, 0, CHAR_BASE + 1);
            }
        }

        break;

    case 5:
        if (!is_paused()) {
            char_move(&ewk->wu);

            if (ewk->wu.cg_type) {
                ewk->wu.routine_no[0]++;
                set_char_move_init(&ewk->wu, 0, CHAR_BASE + 5);
            }
        }

        break;

    case 6:
        if (!is_paused()) {
            char_move(&ewk->wu);
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    disp_pos_trans_entry_ng(ewk);
}

s32 effect_ng28_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG28_ID;
    ewk->wu.work_id = 16;
    ewk->my_master = master;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = 0x13C; // NG 0x50
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = 496;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = 64;
    ewk->wu.my_priority = ewk->wu.position_z = 83;
    ewk->wu.char_table[0] = _ng_hkg_b_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
