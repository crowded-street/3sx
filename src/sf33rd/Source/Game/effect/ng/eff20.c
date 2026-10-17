/**
 * @file eff20.c
 * Falling leaves of Ibuki's stage from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff20.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/ng/eff67.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"

#define GROUP_COUNT 2
#define FIRST_DELAY 60

static const s16 max_leaves[GROUP_COUNT] = { 7, 4 };

static const s16 leaf_delays[32] = { 18,  34,  48,  54,  60,  68,  80,  88,  98,  104, 118, 124, 128, 138, 144, 154,
                                     158, 168, 174, 188, 200, 218, 224, 248, 268, 288, 300, 328, 338, 460, 480, 600 };

static bool is_paused() {
    return EXE_flag || Game_pause;
}

s16* effect_ng20_leaf_count(WORK_Other* spawner, s16 group) {
    return group == 0 ? &spawner->wu.direction : &spawner->wu.dir_old;
}

static void drop_first_leaves(WORK_Other* ewk, s16 group) {
    const s16 count = (random_32() & 2) ? 2 : 1;

    for (s16 i = 0; i < count; i++) {
        effect_ng67_init(ewk, group);
    }
}

static void update_group(WORK_Other* ewk, s16 group) {
    s16* state = &ewk->wu.routine_no[2 + group];
    s16* timer = group == 0 ? &ewk->wu.vitality : &ewk->wu.vital_new;

    switch (*state) {
    case 0:
        if (*timer == 0) {
            (*state)++;
        } else {
            (*timer)--;
        }

        break;

    case 1:
        if (*effect_ng20_leaf_count(ewk, group) < max_leaves[group]) {
            effect_ng67_init(ewk, group);
            *timer = leaf_delays[random_32()];
            (*state)--;
        }

        break;
    }
}

void effect_ng20_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[1] = 0;
        ewk->wu.routine_no[2] = 0;
        ewk->wu.routine_no[3] = 0;
        ewk->wu.routine_no[0]++;
        break;

    case 1:
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (ewk->wu.dead_f == 1) {
        ewk->wu.routine_no[0]++;
        return;
    }

    if (is_paused() || effect_ng67_is_hidden(ewk)) {
        return;
    }

    switch (ewk->wu.routine_no[1]) {
    case 0:
        drop_first_leaves(ewk, 0);
        ewk->wu.vitality = FIRST_DELAY;
        drop_first_leaves(ewk, 1);
        ewk->wu.vital_new = FIRST_DELAY;
        ewk->wu.routine_no[1]++;
        break;

    case 1:
        update_group(ewk, 0);
        update_group(ewk, 1);
        break;
    }
}

s32 effect_ng20_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = EFFECT_NG20_ID;
    ewk->wu.work_id = 16;
    ewk->wu.direction = 0;
    ewk->wu.dir_old = 0;
    return 0;
}
