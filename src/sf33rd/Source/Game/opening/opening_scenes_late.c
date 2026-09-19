/**
 * @file opening_scenes_late.c
 * Opening
 * The opening scenes from 109 on, and the steps they share.
 */

#include "sf33rd/Source/Game/opening/opening.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/demo/demo00.h"
#include "sf33rd/Source/Game/effect/eff36.h"
#include "sf33rd/Source/Game/effect/eff48.h"
#include "sf33rd/Source/Game/effect/effe1.h"
#include "sf33rd/Source/Game/effect/efff5.h"
#include "sf33rd/Source/Game/effect/efff6.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/opening/op_sub.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/Game/opening/opening_internal.h"


const s16 op_103_sound[12] = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 11, 12, 13 };

void advance_opening_step(s16 index) {
    op_w.r_no_2 += 1;
    op_work_clear();
    op_w.index = index;
}

/* One step of an opening scene. When the step's cue has come the scene moves on
 * to the next step - and whatever else that step starts, the caller does, which
 * is where it was - and otherwise the background keeps running on the scene
 * this step shows. The cue is evaluated at the call site, where it was, and
 * every step's own values are written out there in full.
 *
 * Returns whether the scene advanced. */
bool opening_cue_step(bool cue, s16 next, s16 bg) {
    if (cue) {
        advance_opening_step(next);
        return true;
    }

    op_bg_move(bg);
    return false;
}

/* The same step, for the steps that also turn the scene's objects on or off.
 * The assignment is the one the arm made, in the place it made it. */
bool opening_cue_step_disp(bool cue, s16 next, s16 bg, s16 disp) {
    if (opening_cue_step(cue, next, bg)) {
        op_obj_disp = disp;
        return true;
    }

    return false;
}

/* The steps that turn the scene's objects off and start an effect as they
 * advance. Every one of them turns the objects off, so the flag is not a
 * parameter here - the effect is. */
void opening_cue_step_effect(bool cue, s16 next, s16 bg, s16 effect) {
    if (opening_cue_step_disp(cue, next, bg, 0)) {
        effect_48_init(effect);
    }
}

const s16 op_109_sound[5] = { 0, 3, 5, 7, 11 };

static bool op_109_sound_ready() {
    return (gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6D);
}

void op_109_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_scrn_end = 0;
        op_work_clear();
        op_w.index = 54;
        op_bg_move(54);
        op_obj_disp = 0;
        effect_48_init(16);
        break;

    case 1:
        opening_cue_step_disp(op_109_sound_ready(), 55, 54, 1);

        break;

    case 2:
        opening_cue_step(gSeqStatus[0] >= op_109_sound[op_w.r_no_2], 56, 55);
        break;

    case 3:
        if (gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_scrn_end = 0;
            op_work_clear();
            op_w.index = 57;
            op_obj_disp = 0;
            effect_48_init(17);
            return;
        }

        op_bg_move(56);
        break;

    case 4:
        opening_cue_step_disp(gSeqStatus[0] >= op_109_sound[op_w.r_no_2], 58, 57, 1);

        break;

    default:
        op_bg_move(58);
        break;
    }
}

const s16 op_110_sound[6] = { 0, 0, 3, 4, 7, 9 };

static bool op_110_sound_ready() {
    return (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6E);
}

void op_110_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        Zoom_Value_Set(64);
        op_work_clear();
        op_w.index = 59;
        op_bg_move(59);
        op_obj_disp = 0;
        effect_48_init(2);
        break;

    case 1:
        opening_cue_step_disp(op_110_sound_ready(), 60, 59, 1);

        break;

    case 2:
        opening_cue_step_effect(gSeqStatus[0] >= op_110_sound[op_w.r_no_2], 61, 60, 3);
        break;

    case 3:
        opening_cue_step_disp(gSeqStatus[0] >= op_110_sound[op_w.r_no_2], 62, 61, 1);

        break;

    case 4:
        opening_cue_step_effect(gSeqStatus[0] >= op_110_sound[op_w.r_no_2], 63, 62, 4);
        break;

    case 5:
        opening_cue_step_disp(gSeqStatus[0] >= op_110_sound[op_w.r_no_2], 64, 63, 1);

        break;

    default:
        op_bg_move(64);
        break;
    }
}

const s16 op_111_sound[5] = { 0, 2, 4, 7, 11 };

static bool op_111_sound_ready() {
    return (gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6F);
}

void op_111_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 65;
        op_bg_move(65);
        effect_F6_init(41);
        effect_F6_init(42);
        effect_F6_init(43);
        break;

    case 1:
        opening_cue_step(op_111_sound_ready(), 66, 65);
        break;

    case 2:
        opening_cue_step(gSeqStatus[0] >= op_111_sound[op_w.r_no_2], 67, 66);
        break;

    case 3:
        opening_cue_step(gSeqStatus[0] >= op_111_sound[op_w.r_no_2], 68, 67);
        break;

    case 4:
        opening_cue_step(gSeqStatus[0] >= op_111_sound[op_w.r_no_2], 69, 68);
        break;

    default:
        op_bg_move(69);
        break;
    }
}

const s16 op_112_sound[9] = { 0, 8, 8, 14, 19, 26, 34, 40, 44 };

static bool op_112_sound_ready() {
    return (gSeqStatus[0] >= op_112_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x70);
}

/* A step of scene 112 that is timed by a frame counter rather than by the sound
 * position: the counter ticks, and the step's own entry in op_112_sound is how
 * many frames it lasts. */
static bool op_112_timed_step(s16 next, s16 bg) {
    op_w.mv_ctr += 1;
    return opening_cue_step(op_w.mv_ctr >= op_112_sound[op_w.r_no_2], next, bg);
}

/* The frame-timed step, for the steps that also turn the objects on or off. */
static bool op_112_timed_step_disp(s16 next, s16 bg, s16 disp) {
    if (op_112_timed_step(next, bg)) {
        op_obj_disp = disp;
        return true;
    }

    return false;
}

static void start_op_112_timed_sequence(void) {
    if (opening_cue_step(op_112_sound_ready(), 71, 70)) {
        op_w.mv_ctr = 0;
    }
}

static void update_op_112_scene_71_transition(void) {
    op_112_timed_step(72, 71);
}

static void update_op_112_scene_72_transition(void) {
    if (op_112_timed_step(73, 72)) {
        op_obj_disp = 0;
        effect_48_init(18);
    }
}

/* Scene 112 runs nine steps. Its tail is reached through the default arm of the
 * head, which is what keeps either switch readable; no case label is
 * renumbered. */
static void op_112_move_late() {
    switch (op_w.r_no_2) {
    case 5:
        op_112_timed_step(75, 74);
        break;

    case 6:
        if (op_112_timed_step(76, 75)) {
            op_obj_disp = 0;
            effect_48_init(19);
        }

        break;

    case 7:
        op_112_timed_step_disp(77, 76, 1);

        break;

    case 8:
        op_112_timed_step(78, 77);
        break;

    default:
        op_bg_move(78);
        break;
    }
}

void op_112_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 70;
        op_bg_move(70);
        effect_F6_init(54);
        effect_F6_init(44);
        effect_F6_init(45);
        effect_F6_init(46);
        effect_F6_init(47);
        effect_F6_init(48);
        effect_F6_init(49);
        effect_F6_init(50);
        effect_F6_init(51);
        break;

    case 1:
        start_op_112_timed_sequence();
        break;

    case 2:
        update_op_112_scene_71_transition();
        break;

    case 3:
        update_op_112_scene_72_transition();
        break;

    case 4:
        op_112_timed_step_disp(74, 73, 1);

        break;

    default:
        op_112_move_late();
        break;
    }
}

const s16 op_113_sound[4] = { 0, 3, 7, 11 };

static bool op_113_sound_ready() {
    return (gSeqStatus[0] >= op_113_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x71);
}

void op_113_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_scrn_end = 0;
        op_work_clear();
        op_w.index = 79;
        op_bg_move(79);
        effect_F6_init(52);
        effect_F6_init(53);
        op_obj_disp = 0;
        effect_48_init(20);
        break;

    case 1:
        opening_cue_step_disp(op_113_sound_ready(), 80, 79, 1);

        break;

    case 2:
        if (gSeqStatus[0] >= op_113_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_scrn_end = 0;
            op_work_clear();
            op_w.index = 81;
            op_obj_disp = 0;
            effect_48_init(21);
            return;
        }

        op_bg_move(80);
        break;

    case 3:
        opening_cue_step_disp(gSeqStatus[0] >= op_113_sound[op_w.r_no_2], 82, 81, 1);

        break;

    default:
        op_bg_move(82);
        break;
    }
}

const s16 op_114_sound[6] = { 0, 2, 3, 4, 7, 9 };

static bool op_114_sound_ready() {
    return (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x72);
}

/* The one step of scene 114 that does not fit the shared shape: it zooms
 * between advancing the step counter and clearing the work, so the order is
 * kept by leaving the block whole rather than calling advance_opening_step. */
static void start_op_114_zoom_step(void) {
    if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
        op_w.r_no_2 += 1;
        Zoom_Value_Set(64);
        op_work_clear();
        op_w.index = 87;
        op_obj_disp = 0;
        effect_48_init(7);
        return;
    }

    op_bg_move(86);
}

void op_114_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 83;
        op_bg_move(83);
        op_obj_disp = 0;
        effect_48_init(5);
        break;

    case 1:
        opening_cue_step_disp(op_114_sound_ready(), 84, 83, 1);

        break;

    case 2:
        opening_cue_step_effect(gSeqStatus[0] >= op_114_sound[op_w.r_no_2], 85, 84, 6);
        break;

    case 3:
        opening_cue_step_disp(gSeqStatus[0] >= op_114_sound[op_w.r_no_2], 86, 85, 1);

        break;

    case 4:
        start_op_114_zoom_step();
        break;

    case 5:
        opening_cue_step_disp(gSeqStatus[0] >= op_114_sound[op_w.r_no_2], 88, 87, 1);

        break;

    default:
        op_bg_move(88);
        break;
    }
}

const s16 op_115_sound[2] = { 0, 7 };

static bool op_115_sound_ready() {
    return (gSeqStatus[0] >= op_115_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x73);
}

void op_115_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        Zoom_Value_Set(64);
        op_work_clear();
        op_w.index = 89;
        op_bg_move(89);
        effect_36_init(16);
        break;

    case 1:
        opening_cue_step(op_115_sound_ready(), 90, 89);
        break;

    default:
        op_bg_move(90);
        break;
    }
}

static bool should_complete_opening_fade() {
    return (FadeOut(0, 8, 8) != 0) && (--op_w.mv_ctr <= 0);
}

void op_116_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 91;
        op_bg_move(91);
        effect_36_init(17);
        effect_36_init(28);
        op_w.mv_ctr = 88;
        FadeInit();
        break;

    case 1:
        op_bg_move(91);

        if (op_w.mv_ctr < 4) {
            FadeOut(0, 8, 8);
        }

        if (--op_w.mv_ctr <= 0) {
            op_w.r_no_2 += 1;
            op_w.mv_ctr = 16;
            return;
        }

        break;

    case 2:
        if (should_complete_opening_fade()) {
            op_w.index = 0x8000 - 1;
            op_end_flag = 1;
            op_w.r_no_2 += 1;
        }

        op_bg_move(91);
        break;

    default:
        op_bg_move(91);
        break;
    }
}

void op_117_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        Zoomf_Init();
        op_work_clear();
        op_w.index = 92;
        op_bg_move(92);
        effect_E1_init(1, 0, 1);
        effect_E1_init(0, 0, 1);
        effect_F5_init(16);
        effect_F5_init(17);
        effect_F5_init(18);
        effect_F5_init(9);
        op_bg_move(92);
        op_w.r_no_1 += 1;
        op_w.r_no_2 = 0;
        op_work_clear();
        break;
    }
}

static void update_opening_copyright_display(void) {
    if (Check_Fade_Complete() != 0) {
        op_w.r_no_2 += 1;
        Disp_Copyright();
        op_w.mv_ctr = 240;
    }

    op_w.index = 93;
    op_bg_move(93);
}

static void update_opening_delay(void) {
    op_w.mv_ctr -= 1;

    if (op_w.mv_ctr < 0) {
        op_w.r_no_2 += 1;
        return;
    }
}

void op_118_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_w.mv_ctr = 60;
        break;

    case 1:
        update_opening_delay();
        break;

    case 2:
        if (Request_Fade(0x28) != 0) {
            op_w.r_no_2 += 1;
            return;
        }

        break;

    case 3:
        update_opening_copyright_display();
        break;

    case 4:
        update_opening_delay();
        break;

    case 5:
        op_end_flag = 1;
        Bg_Off_W(14);
        op_bg_move(93);
        break;
    }
}
