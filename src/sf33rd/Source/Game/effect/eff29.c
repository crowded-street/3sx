/**
 * @file eff29.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff29.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

const s16 eff29_vanish_time[8] = { 480, 600, 300, 240, 200, 340, 500, 360 };

static s32 can_update_effect(void) {
    return !EXE_flag && !Game_pause && !EXE_obroll;
}


static s32 effect_can_advance(void) {
    return !EXE_flag && !Game_pause && !EXE_obroll;
}


/* Count the hidden interval down, then show the effect and start its
 * animation. */
static void e29_await_reappear(WORK_Other* ewk) {
    if (can_update_effect()) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            ewk->wu.routine_no[0]++;
            ewk->wu.disp_flag = 1;
            set_char_move_init(&ewk->wu, 0, 0);
        }
    }
}

/* Run the animation out, then hide the effect again and pick the next interval
 * from the table at random. */
static void e29_show_until_done(WORK_Other* ewk) {
    s16 work;

    if (effect_can_advance()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type) {
            ewk->wu.routine_no[0] = 0;
            ewk->wu.disp_flag = 0;
            work = random_16();
            work &= 7;
            ewk->wu.old_rno[0] = eff29_vanish_time[work];
        }
    }
}

void effect_29_move(WORK_Other* ewk) {
    if (obr_no_disp_check()) {
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        e29_await_reappear(ewk);
        break;

    case 1:
        e29_show_until_done(ewk);
        disp_pos_trans_entry_r(ewk);
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_29_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 29;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.rl_flag = 0;
    ewk->wu.dead_f = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_col_code = 300;
    ewk->wu.my_mts = 7;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.xyz[0].disp.pos = 512;
    ewk->wu.xyz[1].disp.pos = 0;
    ewk->wu.my_priority = ewk->wu.position_z = 82;
    ewk->wu.char_index = 0;
    ewk->wu.sync_suzi = 0;
    ewk->wu.char_table[0] = _hkg_char_table;
    suzi_offset_set(ewk);
    ewk->wu.old_rno[0] = 120;
    ewk->wu.disp_flag = 0;
    return 0;
}
