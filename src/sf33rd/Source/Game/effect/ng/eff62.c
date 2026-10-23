/**
 * @file eff62.c
 * Spectator on Sean's stage from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff62.h"
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

#include <stdbool.h>

#define COLOR_CODE 0x144 // NG 0x58
#define START_X 448
#define START_Y 58
#define PRIORITY 77
#define CHAR_IDLE 25 // NG gives the effect a char table that starts at char 25 of the stage's

static bool is_shown(const WORK_Other* master) {
    switch (master->wu.routine_no[0]) {
    case 1:
        return master->wu.routine_no[1] != 2 && master->wu.routine_no[1] != 3;

    case 3:
    case 5:
    case 6:
        return true;

    default:
        return false;
    }
}

void effect_ng62_move(WORK_Other* ewk) {
    WORK_Other* master = (WORK_Other*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        ewk->wu.old_rno[0] = master->wu.old_rno[0] ^ 1;
        /* fallthrough */

    case 1:
        // NG also has a variant that reacts to the round's winner, but its chars are past the end of its char table
        // and nothing starts it
        if (is_shown(master)) {
            if (!EXE_flag && !Game_pause) {
                char_move(&ewk->wu);
            }

            disp_pos_trans_entry_rs_ng(ewk);
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng62_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG62_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.type = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->my_master = master;
    ewk->wu.my_family = 2;
    ewk->wu.sync_suzi = 0;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.xyz[0].disp.pos = START_X;
    ewk->wu.xyz[1].disp.pos = START_Y;
    ewk->wu.my_priority = ewk->wu.position_z = PRIORITY;
    ewk->wu.char_index = CHAR_IDLE;
    ewk->wu.char_table[0] = _ng_nyc_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    return 0;
}
