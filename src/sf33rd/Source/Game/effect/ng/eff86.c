/**
 * @file eff86.c
 * The second sparring fighter of effect 85 on Elena's stage from New Generation. It hides while effect 85 draws both
 * fighters clinched.
 */

#include "sf33rd/Source/Game/effect/ng/eff86.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x14C // NG 0x60
#define CHAR_INDEX 13

void effect_ng86_move(WORK_Other* ewk) {
    const WORK_Other* master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_INDEX);
        /* fallthrough */

    case 1:
        if (compel_dead_check(ewk)) {
            ewk->wu.routine_no[0]++;
            break;
        }

        master = (const WORK_Other*)ewk->my_master;

        switch (master->wu.routine_no[1]) {
        case 2:
        case 3:
            // Effect 85 draws both fighters
            break;

        case 4:
            set_char_move_init(&ewk->wu, 0, CHAR_INDEX);
            break;

        default:
            if (!EXE_flag && !Game_pause) {
                char_move(&ewk->wu);
            }

            disp_pos_trans_entry_ng(ewk);
            break;
        }

        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_ng86_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG86_ID;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.dead_f = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_col_code = COLOR_CODE;
    ewk->my_master = master;
    ewk->wu.my_family = 2;
    ewk->wu.xyz[0].disp.pos = 549;
    ewk->wu.xyz[1].disp.pos = 92;
    ewk->wu.my_priority = ewk->wu.position_z = 80;
    ewk->wu.char_table[0] = _ng_afc_b_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    return 0;
}
