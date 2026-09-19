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
        if (op_109_sound_ready()) {
            advance_opening_step(55);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(54);
        break;

    case 2:
        if (gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) {
            advance_opening_step(56);
            return;
        }

        op_bg_move(55);
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
        if (gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) {
            advance_opening_step(58);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(57);
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
        if (op_110_sound_ready()) {
            advance_opening_step(60);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(59);
        break;

    case 2:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            advance_opening_step(61);
            op_obj_disp = 0;
            effect_48_init(3);
            return;
        }

        op_bg_move(60);
        break;

    case 3:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            advance_opening_step(62);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(61);
        break;

    case 4:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            advance_opening_step(63);
            op_obj_disp = 0;
            effect_48_init(4);
            return;
        }

        op_bg_move(62);
        break;

    case 5:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            advance_opening_step(64);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(63);
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
        if (op_111_sound_ready()) {
            advance_opening_step(66);
            return;
        }

        op_bg_move(65);
        break;

    case 2:
        if (gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) {
            advance_opening_step(67);
            return;
        }

        op_bg_move(66);
        break;

    case 3:
        if (gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) {
            advance_opening_step(68);
            return;
        }

        op_bg_move(67);
        break;

    case 4:
        if (gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) {
            advance_opening_step(69);
            return;
        }

        op_bg_move(68);
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

static void start_op_112_timed_sequence(void) {
    if (op_112_sound_ready()) {
        advance_opening_step(71);
        op_w.mv_ctr = 0;
        return;
    }

    op_bg_move(70);
}

static void update_op_112_scene_71_transition(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
        advance_opening_step(72);
        return;
    }

    op_bg_move(71);
}

static void update_op_112_scene_72_transition(void) {
    op_w.mv_ctr += 1;

    if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
        advance_opening_step(73);
        op_obj_disp = 0;
        effect_48_init(18);
        return;
    }

    op_bg_move(72);
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
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            advance_opening_step(74);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(73);
        break;

    case 5:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            advance_opening_step(75);
            return;
        }

        op_bg_move(74);
        break;

    case 6:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            advance_opening_step(76);
            op_obj_disp = 0;
            effect_48_init(19);
            return;
        }

        op_bg_move(75);
        break;

    case 7:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            advance_opening_step(77);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(76);
        break;

    case 8:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            advance_opening_step(78);
            return;
        }

        op_bg_move(77);
        break;

    default:
        op_bg_move(78);
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
        if (op_113_sound_ready()) {
            advance_opening_step(80);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(79);
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
        if (gSeqStatus[0] >= op_113_sound[op_w.r_no_2]) {
            advance_opening_step(82);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(81);
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
        if (op_114_sound_ready()) {
            advance_opening_step(84);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(83);
        break;

    case 2:
        if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
            advance_opening_step(85);
            op_obj_disp = 0;
            effect_48_init(6);
            return;
        }

        op_bg_move(84);
        break;

    case 3:
        if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
            advance_opening_step(86);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(85);
        break;

    case 4:
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
        break;

    case 5:
        if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
            advance_opening_step(88);
            op_obj_disp = 1;
            return;
        }

        op_bg_move(87);
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
        if (op_115_sound_ready()) {
            advance_opening_step(90);
            return;
        }

        op_bg_move(89);
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
