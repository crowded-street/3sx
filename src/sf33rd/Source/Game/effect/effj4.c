/**
 * @file effj4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effj4.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

static s32 should_end_effect(const WORK_Other* ewk) {
    return ewk->wu.dead_f == 1 || Suicide[0] != 0;
}

/* Non-zero on the frame the effect's timer runs out. 0x7FFF means it has no
 * timer at all, and the decrement sits behind the pause checks, so a paused
 * frame does not consume one - exactly as the original condition had it. */
static s32 j4_timer_expired(WORK_Other* ewk) {
    return ewk->wu.dir_timer != 0x7FFF && !Game_pause && !EXE_flag && --ewk->wu.dir_timer <= 0;
}

static void effj4_spawn(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.disp_flag = 1;
    ewk->wu.blink_timing = 0;
    ewk->wu.rl_flag = 0;

    if (ewk->wu.type) {
        SA_shadow_on = 1;
    }

    if (ewk->wu.dir_timer == 0xFF) {
        ewk->wu.dir_timer = 0x7FFF;
        ewk->wu.my_clear_level = 160;
    }

    ewk->wu.my_priority = ewk->wu.position_z = 71;
    ewk->wu.shell_ix[0] = 0;
    ewk->wu.shell_ix[1] = 384;
    ewk->wu.shell_ix[2] = -base_y_pos;
    ewk->wu.shell_ix[3] = 224;
    ewk->wu.position_x = 0;
    ewk->wu.position_y = 0;
}

static void effj4_animate(WORK_Other* ewk) {
    if (should_end_effect(ewk)) {
        goto jump;
    }

    if (Game_pause == 129 && !pcon_dp_flag) {
        return;
    }

    if (j4_timer_expired(ewk)) {
    jump:
        ewk->wu.routine_no[0]++;

        if (ewk->wu.type) {
            SA_shadow_on = 0;
        }
    }

    sort_push_requestA(&ewk->wu);
}

void effect_J4_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        effj4_spawn(ewk);
        break;

    case 1:
        effj4_animate(ewk);
        break;
    case 2:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_J4_init(u8 data2) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(5)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 194;
    ewk->wu.dir_timer = data2;
    ewk->wu.work_id = 16;
    ewk->wu.my_family = 8;

    if (data2 != 0xFF) {
        ewk->wu.type = 1;
    }

    ewk->wu.my_col_code = 1;
    ewk->wu.my_clear_level = 144;
    return 0;
}

s32 setup_sa_shadow(u8 /* unused */, u8 data) {
    if (test_flag) {
        return -1;
    }

    return effect_J4_init(data);
}
