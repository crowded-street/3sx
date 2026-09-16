/**
 * @file effi3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effi3.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

const I3_Data i3_data[6] = { { 3, 2, 0 }, { 1, 1, 0 }, { 2, 0, 0 }, { 2, 0, 0 }, { 2, 0, 0 }, { 2, 0, 0 } };

static s32 should_end_effect(const WORK_Other* ewk) {
    return ewk->wu.dead_f == 1 || Suicide[0] != 0;
}

/* Non-zero when this effect is done. Which test applies comes from its table
 * entry: flag 1 runs a timer down, flag 2 watches the master's cel. The timer
 * is decremented inside the condition and only when flag 1 is set, exactly as
 * the original expression had it. */
static s32 i3_effect_expired(WORK_Other* ewk, const WORK* mwk) {
    return (i3_data[ewk->wu.type].flag & 1 && --ewk->wu.dir_timer < 0) ||
           (i3_data[ewk->wu.type].flag & 2 &&
            (ewk->wu.now_koc != mwk->now_koc || ewk->wu.char_index != mwk->char_index));
}

static void effi3_spawn(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    bg_stop = 1;

    switch (i3_data[ewk->wu.type].sour) {
    case 1:
        if ((ewk->wu.dir_timer = ewk->wu.hit_stop) < 0) {
            ewk->wu.dir_timer = -ewk->wu.dir_timer;
        }

        break;

    case 2:
        ewk->wu.dir_timer = ewk->wu.cg_type;
        break;

    default:
        ewk->wu.dir_timer = i3_data[ewk->wu.type].tm;
        break;
    }
}

static void effi3_animate(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;

    if (should_end_effect(ewk)) {
        ewk->wu.routine_no[0]++;
        return;
    }

    if (EXE_flag != 0 || Game_pause != 0) {
        return;
    }

    if (i3_effect_expired(ewk, mwk)) {
        ewk->wu.routine_no[0] = 2;
    }
}

void effect_I3_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        effi3_spawn(ewk);
        /* fallthrough */

    case 1:
        effi3_animate(ewk);
        break;

    case 2:
    default:
        bg_stop = 0;
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_I3_init(WORK* wk, u8 tix) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 183;
    ewk->wu.work_id = 16;
    ewk->my_master = wk;
    ewk->wu.type = tix;
    ewk->wu.cg_type = wk->cg_type;
    ewk->wu.hit_stop = wk->hit_stop;
    ewk->wu.now_koc = wk->now_koc;
    ewk->wu.char_index = wk->char_index;
    return 0;
}
