/**
 * @file eff07.c
 * A pair of objects on Necro's stage from New Generation. 
 * Effect 07 plays its animations in a fixed order. 
 * Effect 08 joins in whenever effect 07 plays its third animation.
 */

#include "sf33rd/Source/Game/effect/ng/eff07.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"

#define EFF07_CHAR_BASE 34
#define EFF08_CHAR_BASE 18
#define EFF07_SHARED_CHAR 2

static const s16 eff07_char_sequence[16] = { 0, 0, 1, 1, 2, 3, 0, 1, 3, 0, 1, 3, 2, 1, 0, 3 };

static bool is_paused() {
    return EXE_flag || Game_pause;
}

void effect_ng07_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (!is_paused()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.cg_type = 0;
            ewk->wu.old_rno[0] = (ewk->wu.old_rno[0] + 1) & 15;
            ewk->wu.char_index = EFF07_CHAR_BASE + eff07_char_sequence[ewk->wu.old_rno[0]];
            ewk->wu.routine_no[0] = 0;
        }
    }

    sort_push_request4(&ewk->wu);
}

void effect_ng08_move(WORK_Other* ewk) {
    const WORK_Other* master = (WORK_Other*)ewk->my_master;
    const bool master_shares_char = master->wu.char_index == EFF07_CHAR_BASE + EFF07_SHARED_CHAR;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
        break;

    case 1:
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (!is_paused()) {
        if (master->wu.routine_no[0] == 0) {
            // The master is starting its next animation
            if (master_shares_char) {
                ewk->wu.char_index = EFF08_CHAR_BASE;
                ewk->wu.routine_no[0] = 0;
            }
        } else if (master_shares_char) {
            char_move(&ewk->wu);
        }
    }

    sort_push_request4(&ewk->wu);
}

static void setup_work(WORK_Other* ewk, s16 id) {
    ewk->wu.be_flag = 1;
    ewk->wu.id = id;
    ewk->wu.work_id = 16;
    ewk->wu.my_priority = 64;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.char_table[0] = _ng_rca_char_table;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
}

static s32 effect_ng08_init(WORK_Other* master) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    setup_work(ewk, EFFECT_NG08_ID);
    ewk->my_master = master;
    ewk->wu.my_col_code = 8492; // NG 0x2040
    ewk->wu.position_x = 0x1B0;
    ewk->wu.position_y = 0x76;
    ewk->wu.position_z = 70;
    ewk->wu.char_index = EFF08_CHAR_BASE;
    return 0;
}

s32 effect_ng07_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    setup_work(ewk, EFFECT_NG07_ID);
    ewk->wu.my_col_code = 325; // NG 0x59
    ewk->wu.position_x = 0x1CF;
    ewk->wu.position_y = 0x4D;
    ewk->wu.position_z = 74;
    ewk->wu.char_index = EFF07_CHAR_BASE + eff07_char_sequence[0];
    ewk->wu.old_rno[0] = 0;

    // NG ignores whether this fails
    effect_ng08_init(ewk);
    return 0;
}
