/**
 * @file eff09_endgame.c
 * Effect 09 endgame phases 22000 through 25000.
 */

#include "sf33rd/Source/Game/effect/eff09_endgame.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/effb4.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/caldir.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static void eff09_endgame_abort_on_test(WORK_Other* ewk) {
    if (test_flag) {
        ewk->wu.routine_no[1] = 99;
    }
}

static s32 eff09_endgame_updates_enabled(void) {
    return !EXE_flag && !Game_pause;
}

static void start_eff09_endgame_effect(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void initialize_eff09_22000(WORK_Other* ewk) {
    start_eff09_endgame_effect(ewk);
}

static void advance_eff09_22000_parent_animation(WORK_Other* ewk, const WORK* oya_ptr) {
    s16 work;

    if (eff09_endgame_updates_enabled()) {
        if (oya_ptr->cg_type == 99) {
            ewk->wu.routine_no[1]++;
            set_char_move_init(&ewk->wu, 0, 62);
        } else if (oya_ptr->cg_ix != ewk->wu.cg_ix) {
            work = oya_ptr->cg_ix / oya_ptr->cgd_type;
            set_char_move_init2(&ewk->wu, 0, 61, work + 1, 0);
            ewk->wu.cg_ix = oya_ptr->cg_ix;
        }
    }
}

static void advance_eff09_22000_animation(WORK_Other* ewk) {
    if (eff09_endgame_updates_enabled()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1]++;
            ewk->wu.disp_flag = 0;
        }
    }
}

void eff09_22000(WORK_Other* ewk) {
    WORK* oya_ptr;

    eff09_endgame_abort_on_test(ewk);

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_22000(ewk);
        break;

    case 1:
        advance_eff09_22000_parent_animation(ewk, oya_ptr);
        pl_eff_trans_entry(ewk);
        break;

    case 2:
        advance_eff09_22000_animation(ewk);
        pl_eff_trans_entry(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static s32 eff09_23000_parent_animation_started(const WORK* oya_ptr) {
    return !EXE_flag && !Game_pause && oya_ptr->cg_type == 1;
}

static void initialize_eff09_23000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;
    ewk->wu.kage_flag = 1;
    ewk->wu.kage_hx = 1;
    ewk->wu.kage_hy = -2;
    ewk->wu.kage_prio = 71;
    ewk->wu.kage_char = 11;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void advance_eff09_23000_launch(WORK_Other* ewk) {
    if (eff09_endgame_updates_enabled()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1]++;

            if (ewk->wu.rl_flag) {
                ewk->wu.mvxy.a[0].sp = -0x20000;
            } else {
                ewk->wu.mvxy.a[0].sp = 0x20000;
            }

            ewk->wu.mvxy.d[0].sp = 0;
            set_char_move_init(&ewk->wu, 0, 64);
        }
    }
}

static void advance_eff09_23000_exit(WORK_Other* ewk) {
    if (eff09_endgame_updates_enabled()) {
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);

        if (range_x_check3(ewk, 64) == 0) {
            ewk->wu.routine_no[1]++;
        }
    }
}

void eff09_23000(WORK_Other* ewk) {
    WORK* oya_ptr;

    eff09_endgame_abort_on_test(ewk);

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_23000(ewk);
        break;

    case 1:
        if (eff09_23000_parent_animation_started(oya_ptr)) {
            ewk->wu.routine_no[1]++;
        }

        pl_eff_trans_entry(ewk);
        break;

    case 2:
        advance_eff09_23000_launch(ewk);
        pl_eff_trans_entry(ewk);
        break;

    case 3:
        advance_eff09_23000_exit(ewk);
        pl_eff_trans_entry(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

static void initialize_eff09_24000(WORK_Other* ewk) {
    ewk->wu.routine_no[1]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.dead_f = 1;

    if (ewk->wu.type == 38) {
        ewk->wu.rl_flag ^= 1;
    }

    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static void advance_eff09_24000_parent_animation(WORK_Other* ewk, const WORK* oya_ptr) {
    if (eff09_23000_parent_animation_started(oya_ptr)) {
        ewk->wu.routine_no[1]++;

        if (ewk->wu.type == 38) {
            ewk->wu.routine_no[1] = 99;
        }
    }
}

static void advance_eff09_24000_launch(WORK_Other* ewk) {
    if (eff09_endgame_updates_enabled()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 9) {
            ewk->wu.routine_no[1]++;
            char_move_z(&ewk->wu);

            if (ewk->wu.type == 35) {
                ewk->wu.mvxy.a[0].sp = 0x18000;
            } else {
                ewk->wu.mvxy.a[0].sp = -0x18000;
            }

            if (ewk->wu.rl_flag) {
                ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
            }

            ewk->wu.mvxy.d[0].sp = 0;
        }
    }
}

static void advance_eff09_24000_exit(WORK_Other* ewk) {
    if (eff09_endgame_updates_enabled()) {
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);

        if (ewk->wu.type == 34) {
            if (range_x_check3(ewk, 176) == 0) {
                ewk->wu.routine_no[1]++;
            }
        } else if (range_x_check3(ewk, 88) == 0) {
            ewk->wu.routine_no[1]++;
        }
    }
}

void eff09_24000(WORK_Other* ewk) {
    WORK* oya_ptr;

    eff09_endgame_abort_on_test(ewk);

    oya_ptr = (WORK*)ewk->my_master;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_24000(ewk);
        return;

    case 1:
        advance_eff09_24000_parent_animation(ewk, oya_ptr);
        pl_eff_trans_entry(ewk);
        return;

    case 2:
        advance_eff09_24000_launch(ewk);
        pl_eff_trans_entry(ewk);
        return;

    case 3:
        advance_eff09_24000_exit(ewk);
        pl_eff_trans_entry(ewk);
        return;

    default:
        push_effect_work(&ewk->wu);
        return;
    }
}

static void initialize_eff09_25000(WORK_Other* ewk) {
    start_eff09_endgame_effect(ewk);
    ewk->wu.mvxy.d[0].sp = 0;
    ewk->wu.mvxy.d[1].sp = -0x6000;

    if (ewk->wu.type == 36) {
        ewk->wu.mvxy.a[0].sp = 0x48000;
        ewk->wu.mvxy.a[1].sp = 0x10000;
    } else {
        ewk->wu.mvxy.a[0].sp = 0x28000;
        ewk->wu.mvxy.a[1].sp = 0x30000;
    }

    if (ewk->wu.rl_flag) {
        ewk->wu.mvxy.a[0].sp = -ewk->wu.mvxy.a[0].sp;
    }
}

static void advance_eff09_25000(WORK_Other* ewk) {
    if (eff09_endgame_updates_enabled()) {
        char_move(&ewk->wu);
        add_x_sub(&ewk->wu);
        add_y_sub(&ewk->wu);

        if (range_x_check3(ewk, 16) == 0) {
            ewk->wu.routine_no[1]++;
        } else if (ewk->wu.xyz[1].disp.pos < 0) {
            ewk->wu.xyz[1].cal = 0;
            ewk->wu.mvxy.a[1].sp = 0x40000;
            Sound_SE((ewk->master_id * 0x300) + 0x157);
        }
    }
}

void eff09_25000(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case 0:
        initialize_eff09_25000(ewk);
        pl_eff_trans_entry(ewk);
        break;

    case 1:
        advance_eff09_25000(ewk);
        pl_eff_trans_entry(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}
