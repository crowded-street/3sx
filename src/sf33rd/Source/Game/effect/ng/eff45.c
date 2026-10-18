/**
 * @file eff45.c
 * River in the valley of Elena's stage from New Generation.
 */

#include "sf33rd/Source/Game/effect/ng/eff45.h"
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

#define COLOR_CODE 0x212C // NG 0x2040
#define CHAR_INDEX 8
#define START_X -4
#define START_Y 80
#define DRIFT_SPEED 0x8000
#define DRIFT_FRAMES 32

void effect_ng45_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_pos[0] = 0;
        set_char_move_init(&ewk->wu, 0, CHAR_INDEX);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            ewk->wu.xyz[0].cal -= DRIFT_SPEED;
            ewk->wu.old_pos[0]++;

            if (ewk->wu.old_pos[0] > DRIFT_FRAMES) {
                ewk->wu.old_pos[0] = 0;
                ewk->wu.xyz[0].cal = START_X;
                char_move_z(&ewk->wu);
            }

            ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
        }

        disp_pos_trans_entry_s_ng(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng45_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG45_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.charset_id = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 1;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = START_X;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = START_Y;
    ewk->wu.my_priority = ewk->wu.position_z = 100;
    ewk->wu.sync_suzi = 1;
    ewk->wu.char_table[0] = _ng_afc_a_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    suzi_offset_set(ewk);
    return 0;
}
