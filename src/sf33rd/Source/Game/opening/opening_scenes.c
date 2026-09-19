/**
 * @file opening_scenes.c
 * Opening
 * The opening scenes up to 108, one op_1NN_move each.
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
void op_100_move() {
    op_w.r_no_1 += 1;
    Go_BGM();
    op_101_move();
}

const s16 op_101_sound[2] = { 0, 11 };

static bool op_101_sound_ready() {
    return (gSeqStatus[0] >= op_101_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x65);
}

void op_101_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        ToneDown(0xFF, 0);
        op_bg_move(0);
        effect_F6_init(0);
        effect_F6_init(1);
        op_obj_disp = 0;
        effect_48_init(0);
        break;

    case 1:
        if (op_101_sound_ready()) {
            op_w.r_no_2 += 1;
            op_w.index = 1;
            op_obj_disp = 1;
            op_work_clear();
            break;
        }

        op_bg_move(0);
        break;

    default:
        op_bg_move(1);
        break;
    }
}

const s16 op_102_sound[3] = { 0, 9, 12 };

static bool op_102_sound_ready() {
    return (gSeqStatus[0] >= op_102_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x66);
}

void op_102_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_w.index = 2;
        effect_F6_init(2);
        effect_F6_init(3);
        effect_F6_init(4);
        op_obj_disp = 0;
        effect_48_init(1);
        op_work_clear();
        op_bg_move(2);
        break;

    case 1:
        if (op_102_sound_ready()) {
            op_w.r_no_2 += 1;
            op_w.index = 3;
            op_obj_disp = 1;
            op_work_clear();
            op_bg_move(3);
            return;
        }

        op_bg_move(2);
        break;

    case 2:
        if (gSeqStatus[0] >= op_102_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_w.index = 4;
            op_work_clear();
            op_bg_move(4);
            return;
        }

        op_bg_move(3);
        break;

    default:
        op_bg_move(4);
        break;
    }
}

static bool op_103_sound_ready() {
    return (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x67);
}

static void update_op_103_effect_sequence(void) {
    if (op_103_sound_ready()) {
        op_w.r_no_2 += 1;
        op_w.index = 6;
        op_work_clear();
        effect_F6_init(11);
        effect_F6_init(12);
        effect_F6_init(13);
        effect_F6_init(14);
        effect_F6_init(15);
        effect_F6_init(16);
        return;
    }

    op_bg_move(5);
}

static void op_103_move_from_9() {
    switch (op_w.r_no_2) {
    case 9:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            advance_opening_step(14);
            op_scrn_end = 0;
            effect_36_init(23);
            return;
        }

        op_bg_move(13);
        break;

    case 10:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            advance_opening_step(15);
        }

        op_bg_move(14);
        break;

    case 11:
        opening_cue_step(gSeqStatus[0] >= op_103_sound[op_w.r_no_2], 16, 15);
        break;

    default:
        op_bg_move(16);
        break;
    }
}

static void op_103_move_from_5() {
    switch (op_w.r_no_2) {
    case 5:
        opening_cue_step(gSeqStatus[0] >= op_103_sound[op_w.r_no_2], 10, 9);
        break;

    case 6:
        opening_cue_step(gSeqStatus[0] >= op_103_sound[op_w.r_no_2], 11, 10);
        break;

    case 7:
        opening_cue_step_effect(gSeqStatus[0] >= op_103_sound[op_w.r_no_2], 12, 11, 9);
        break;

    case 8:
        opening_cue_step_disp(gSeqStatus[0] >= op_103_sound[op_w.r_no_2], 13, 12, 1);
        break;

    default:
        op_103_move_from_9();
        break;
    }
}

void op_103_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_w.index = 5;
        effect_F6_init(5);
        effect_F6_init(6);
        effect_F6_init(7);
        effect_F6_init(8);
        effect_F6_init(9);
        effect_F6_init(10);
        op_work_clear();
        op_bg_move(5);
        break;

    case 1:
        update_op_103_effect_sequence();
        break;

    case 2:
        opening_cue_step_effect(gSeqStatus[0] >= op_103_sound[op_w.r_no_2], 7, 6, 8);
        break;

    case 3:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            advance_opening_step(8);
            op_obj_disp = 1;
            op_scrn_end = 0;
            effect_36_init(22);
            return;
        }

        op_bg_move(7);
        break;

    case 4:
        opening_cue_step(gSeqStatus[0] >= op_103_sound[op_w.r_no_2], 9, 8);
        break;

    default:
        op_103_move_from_5();
        break;
    }
}

s16 op_104_sound[7] = { 0, 5, 6, 7, 9, 10, 11 };

static bool op_104_sound_ready() {
    return (gSeqStatus[0] >= op_104_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x68);
}

/* Scene 104's tail, reached through the default arm; its last step and the
 * out-of-range default share an arm and stay together. */
static void op_104_move_late() {
    switch (op_w.r_no_2) {
    case 4:
        opening_cue_step_effect(gSeqStatus[0] >= op_104_sound[op_w.r_no_2], 21, 20, 11);
        break;

    case 5:
        opening_cue_step_disp(gSeqStatus[0] >= op_104_sound[op_w.r_no_2], 22, 21, 1);
        break;

    case 6:
        opening_cue_step(gSeqStatus[0] >= op_104_sound[op_w.r_no_2], 23, 22);
        break;

    case 7:
    default:
        op_bg_move(23);
        break;
    }
}

void op_104_move() {
    switch (op_w.r_no_2) {
    case 0:
        advance_opening_step(17);
        op_bg_move(17);
        effect_F6_init(17);
        effect_F6_init(18);
        effect_F6_init(19);
        effect_F6_init(20);
        effect_F6_init(21);
        effect_F6_init(22);
        effect_F6_init(23);
        break;

    case 1:
        opening_cue_step(op_104_sound_ready(), 18, 17);
        break;

    case 2:
        opening_cue_step_effect(gSeqStatus[0] >= op_104_sound[op_w.r_no_2], 19, 18, 10);
        break;

    case 3:
        opening_cue_step_disp(gSeqStatus[0] >= op_104_sound[op_w.r_no_2], 20, 19, 1);
        break;

    default:
        op_104_move_late();
        break;
    }
}

void op_105_move() {
    switch (op_w.r_no_2) {
    case 0:
        advance_opening_step(24);
        op_bg_move(24);
        effect_F6_init(24);
        op_obj_disp = 0;
        effect_48_init(12);
        break;

    case 1:
    default:
        op_bg_move(24);
        break;
    }
}

const s16 op_106_sound[4] = { 0, 1, 3, 7 };

static bool op_106_sound_ready() {
    return (gSeqStatus[0] >= op_106_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6A);
}

void op_106_move() {
    switch (op_w.r_no_2) {
    case 0:
        advance_opening_step(25);
        op_obj_disp = 1;
        op_bg_move(25);
        effect_F6_init(25);
        effect_F6_init(26);
        effect_F6_init(27);
        effect_F6_init(28);
        effect_36_init(18);
        effect_36_init(19);
        effect_36_init(20);
        effect_36_init(21);
        break;

    case 1:
        opening_cue_step(op_106_sound_ready(), 26, 25);
        break;

    case 2:
        opening_cue_step(gSeqStatus[0] >= op_106_sound[op_w.r_no_2], 27, 26);
        break;

    case 3:
        opening_cue_step_effect(gSeqStatus[0] >= op_106_sound[op_w.r_no_2], 28, 27, 15);
        break;

    default:
        op_bg_move(28);
        break;
    }
}

const s16 op_107_sound[12] = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 11, 12, 13 };

static bool op_107_sound_ready() {
    return (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6B);
}

static void update_op_107_effect_sequence(void) {
    if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
        advance_opening_step(34);
        effect_F6_init(35);
        effect_F6_init(36);
        effect_F6_init(37);
        effect_F6_init(38);
        effect_F6_init(39);
        effect_F6_init(40);
        return;
    }

    op_bg_move(33);
}

/* Scene 107 runs twelve steps; its tail is reached through the default arm.
 * Step 6 ended in `return` rather than `break`, which is the same here and
 * stays as it was. */
static void op_107_move_late() {
    switch (op_w.r_no_2) {
    case 6:
        opening_cue_step(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 35, 34);
        return;

    case 7:
        opening_cue_step_effect(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 36, 35, 14);
        break;

    case 8:
        opening_cue_step_disp(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 37, 36, 1);
        break;

    case 9:
        opening_cue_step(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 38, 37);
        break;

    case 10:
        opening_cue_step(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 39, 38);
        break;

    case 11:
        opening_cue_step(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 40, 39);
        break;

    default:
        op_bg_move(40);
        break;
    }
}

void op_107_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_plmove_timer += 1;
        op_obj_disp = 1;
        advance_opening_step(29);
        op_bg_move(29);
        effect_F6_init(29);
        effect_F6_init(30);
        effect_F6_init(31);
        effect_F6_init(32);
        effect_F6_init(33);
        effect_F6_init(34);
        break;

    case 1:
        opening_cue_step(op_107_sound_ready(), 30, 29);
        break;

    case 2:
        opening_cue_step(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 31, 30);
        break;

    case 3:
        opening_cue_step_effect(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 32, 31, 13);
        break;

    case 4:
        opening_cue_step_disp(gSeqStatus[0] >= op_107_sound[op_w.r_no_2], 33, 32, 1);
        break;

    case 5:
        update_op_107_effect_sequence();
        break;

    default:
        op_107_move_late();
        break;
    }
}

const s16 op_108_sound[13] = { 0, 4, 20, 24, 28, 32, 48, 52, 60, 64, 76, 80, 86 };

static void update_op_108_first_step(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(42);
        op_bg_move(42);
        return;
    }

    op_bg_move(41);
}

static void update_op_108_effect_sequence(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(43);
        op_bg_move(43);
        effect_36_init(8);
        effect_36_init(9);
        effect_36_init(10);
        effect_36_init(11);
        effect_36_init(12);
        effect_36_init(13);
        effect_36_init(14);
        effect_36_init(15);
        return;
    }
}

static void update_op_108_followup_step(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(44);
        op_bg_move(44);
        return;
    }

    op_bg_move(43);
}

static void update_op_108_timed_transition(s16 next_index) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(next_index);
        op_bg_move(next_index);
        return;
    }
}

static void update_op_108_scene_45_transition(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(46);
        op_bg_move(46);
        return;
    }

    op_bg_move(45);
}

static void update_op_108_scene_47_transition(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(48);
        op_bg_move(48);
        return;
    }

    op_bg_move(47);
}

static void update_op_108_scene_49_transition(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(50);
        op_bg_move(50);
        return;
    }

    op_bg_move(49);
}

static void update_op_108_scene_51_transition(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(52);
        op_bg_move(52);
        return;
    }

    op_bg_move(51);
}

static void update_op_108_final_transition(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
        advance_opening_step(53);
        op_bg_move(53);
    }
}

/* Scene 108 runs thirteen steps, more than a switch can hold under the
 * complexity threshold. The tail is reached through the default arm, twice
 * over; no case label is renumbered, and the empty default the scene already
 * had for an out-of-range step is what each link is written into. */
static void op_108_move_last() {
    switch (op_w.r_no_2) {
    case 8:
        update_op_108_timed_transition(49);
        break;

    case 9:
        update_op_108_scene_49_transition();
        break;

    case 10:
        update_op_108_timed_transition(51);
        break;

    case 11:
        update_op_108_scene_51_transition();
        break;

    case 12:
        update_op_108_final_transition();
        break;
    }
}

static void op_108_move_late() {
    switch (op_w.r_no_2) {
    case 3:
        update_op_108_followup_step();
        break;

    case 4:
        update_op_108_timed_transition(45);
        break;

    case 5:
        update_op_108_scene_45_transition();
        break;

    case 6:
        update_op_108_timed_transition(47);
        break;

    case 7:
        update_op_108_scene_47_transition();
        break;

    default:
        op_108_move_last();
        break;
    }
}

void op_108_move() {
    switch (op_w.r_no_2) {
    case 0:
        advance_opening_step(41);
        op_bg_move(41);
        op_w.mv_ctr = 0;
        effect_36_init(0);
        effect_36_init(1);
        effect_36_init(2);
        effect_36_init(3);
        effect_36_init(4);
        effect_36_init(5);
        effect_36_init(6);
        effect_36_init(7);
        break;

    case 1:
        update_op_108_first_step();
        break;

    case 2:
        update_op_108_effect_sequence();
        break;

    default:
        op_108_move_late();
        break;
    }
}
