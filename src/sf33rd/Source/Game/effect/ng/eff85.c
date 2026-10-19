/**
 * @file eff85.c
 * Two fighters sparring on the far cliff in the second and third areas of Elena's stage from New Generation. This
 * effect draws one of them until they clinch, then both in one animation. Effect 86 draws the other one until then.
 */

#include "sf33rd/Source/Game/effect/ng/eff85.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/ng/eff86.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x14C // NG 0x60
#define SPAR_CHAR_INDEX 12
#define CLINCH_CHAR_INDEX 14

void effect_ng85_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, SPAR_CHAR_INDEX);
        break;

    case 1:
        if (!EXE_flag && !Game_pause) {
            if (compel_dead_check(ewk)) {
                ewk->wu.routine_no[0]++;
                break;
            }

            switch (ewk->wu.routine_no[1]) {
            case 0:
                char_move(&ewk->wu);

                if (ewk->wu.cg_type != 0) {
                    ewk->wu.routine_no[1]++;
                }

                break;

            case 1:
                // Effect 86 hides while this draws both fighters
                ewk->wu.routine_no[1]++;
                ewk->wu.disp_flag = 1;
                set_char_move_init(&ewk->wu, 0, CLINCH_CHAR_INDEX);
                break;

            case 2:
                ewk->wu.routine_no[1]++;
                break;

            case 3:
                // NG never leaves this state, so states 4 and 5 (back to sparring) are unused
                char_move(&ewk->wu);
                break;

            case 4:
                ewk->wu.routine_no[1]++;
                break;

            case 5:
                char_move(&ewk->wu);

                if (ewk->wu.cg_type != 0) {
                    ewk->wu.routine_no[0] = 0;
                }

                break;
            }
        }

        disp_pos_trans_entry_ng(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng85_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG85_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.dead_f = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->wu.xyz[0].disp.pos = 604;
    ewk->wu.xyz[1].disp.pos = 94;
    ewk->wu.my_priority = ewk->wu.position_z = 79;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _ng_afc_b_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    effect_ng86_init(ewk);
    return 0;
}
