/**
 * @file eff77.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff77.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

s16 chk77_flag;

const s16 eff77_data_tbl[7][2] = { { 64, 0 }, { 2, 0 }, { 1, 1 }, { 32, 1 }, { 22, 1 }, { 1, 0 }, { 1, 0 } };

const u32 eff77_col_tbl[2] = { 0xFFFFFFFF, 0xFF000000 };

static s32 effect_update_is_blocked(void) {
    return Game_pause || EXE_flag;
}

static void set_stage_backgrounds_77(void (*set_background)(u16)) {
    u16 bg = ake_bg_off[bg_w.stage];
    u16 mask = 1;
    u16 assign;
    s16 i;

    for (i = 0; i < 4; i++, assign = mask *= 2) {
        if (bg & mask) {
            set_background(1 << i);
        }
    }
}

static s32 initialize_effect_77(WORK_Other* ewk) {
    if (effect_update_is_blocked()) {
        return 0;
    }

    ewk->wu.routine_no[0]++;
    plw[0].wu.disp_flag = 0;
    plw[1].wu.disp_flag = 0;

    if (ewk->wu.type == 0) {
        Extra_Break = 1;
    }

    return 1;
}

static void start_effect_77(WORK_Other* ewk) {
    if (effect_update_is_blocked()) {
        return;
    }

    ewk->wu.routine_no[0]++;
    sa_pa_flag = 1;
    set_stage_backgrounds_77(Bg_Off_R);
    ewk->wu.old_rno[0] = eff77_data_tbl[ewk->wu.type][0];
    ewk->wu.old_rno[1] = eff77_data_tbl[ewk->wu.type][1];
    overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
}

static s32 prepare_effect_77(WORK_Other* ewk) {
    if (effect_update_is_blocked()) {
        overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
        return 0;
    }

    ewk->wu.routine_no[0]++;
    set_stage_backgrounds_77(Bg_Off_R);
    sa_pa_flag = 1;
    return 1;
}

static void update_effect_77(WORK_Other* ewk) {
    if (Suicide[6]) {
        ewk->wu.old_rno[0] = 0;
    }

    if (!Game_pause && !EXE_flag) {
        ewk->wu.old_rno[0]--;

        if (ewk->wu.old_rno[0] <= 0) {
            sa_pa_flag = 0;
            ewk->wu.routine_no[0]++;
            plw[0].wu.disp_flag = 1;
            plw[1].wu.disp_flag = 1;
            overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
            set_stage_backgrounds_77(Bg_On_R);
            return;
        }

        sa_pa_flag = 1;
    }

    overwrite_panel(eff77_col_tbl[ewk->wu.old_rno[1]], 0x46);
}

static void finish_effect_77(WORK_Other* ewk) {
    if (effect_update_is_blocked()) {
        return;
    }

    ewk->wu.routine_no[0]++;

    if (ewk->wu.type == 0) {
        Extra_Break = 0;
    }
}

static void dispose_effect_77(WORK_Other* ewk) {
    if (!Game_pause && !EXE_flag) {
        sa_pa_flag = 0;
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
    }
}

static void prepare_frame_77(WORK_Other* ewk) {
    s16 i;

    another_bg[0] = another_bg[1] = 0;
    Flash_MT[0] = Flash_MT[1] = 0;

    if (akebono_flag) {
        ewk->wu.routine_no[0] = 99;
    }

    for (i = 0; i < 3; i++) {
        scr_calc(i);
    }
}

static void run_initial_state_77(WORK_Other* ewk) {
    if (initialize_effect_77(ewk)) {
        start_effect_77(ewk);
    }
}

static void run_prepare_state_77(WORK_Other* ewk) {
    if (prepare_effect_77(ewk)) {
        update_effect_77(ewk);
    }
}

void effect_77_move(WORK_Other* ewk) {
    prepare_frame_77(ewk);

    switch (ewk->wu.routine_no[0]) {
    case 0:
        run_initial_state_77(ewk);
        break;

    case 1:
        start_effect_77(ewk);
        break;

    case 2:
        run_prepare_state_77(ewk);
        break;

    case 3:
        update_effect_77(ewk);
        break;

    case 4:
        finish_effect_77(ewk);
        break;

    default:
        dispose_effect_77(ewk);
        break;
    }
}

s32 effect_77_init(u8 /* unused */, u8 data) {
    WORK_Other* ewk;
    s16 ix;

    chk77_flag = 0;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 77;
    ewk->wu.type = data;
    another_bg[0] = another_bg[1] = 0;
    Flash_MT[0] = Flash_MT[1] = 0;
    sa_pa_flag = 1;
    return 0;
}
