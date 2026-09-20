/**
 * @file opening_bg1.c
 * Opening
 * The second and third opening background layers.
 */

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
#include "sf33rd/Source/Game/opening/opening.h"
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

void op_bg1_move(s16 r_index) {
    opw_ptr = &op_w.bgw[1];
    bgw_ptr = &bg_w.bgw[1];

    switch (r_index) {
    case 55:
    case 56:
        op_bg1_0001(r_index);
        break;

    case 60:
        op_bg1_0002(r_index);
        break;

    case 62:
        op_bg1_0003(r_index);
        break;

    default:
        op_bg1_0000(r_index);
        break;
    }

    op_scrn_pos_set2(1);
}

void op_bg1_0000(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 0;
        Bg_Off_W(2);
        break;

    case 1:
    case 2:
        break;
    }
}

void op_bg1_0001(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->wxy[0].cal = 0x02000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 0x37:
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 1, 1, 0, 0 });
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 2, 2, 0, 0 });
            break;

        case 0x38:
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 3, 1, 0, 0 });
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 4, 2, 0, 0 });
            break;
        }

        break;

    case 1:
        break;
    }
}

void op_bg1_0002(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1 << bgw_ptr->fam_no);

        switch (r_index) {
        case 0x3C:
            bgw_ptr->wxy[0].cal = 0x01000000;
            bgw_ptr->xy[1].cal = 0;
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 7, 0, 0, 0 });
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 8, 1, 0, 0 });
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 9, 2, 0, 0 });
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp = 0xC0000;
            op_bg_mvxy[bgw_ptr->fam_no].d[0].sp = 0;
            bgw_ptr->r_limit = 0x1E0;
            break;

        default:
            break;
        }

        break;

    case 1:
        op_bg_mvxy[bgw_ptr->fam_no].a[0].sp += op_bg_mvxy[bgw_ptr->fam_no].d[0].sp;
        bgw_ptr->wxy[0].cal += op_bg_mvxy[bgw_ptr->fam_no].a[0].sp;

        if (bgw_ptr->wxy[0].disp.pos >= bgw_ptr->r_limit) {
            opw_ptr->r_no_0 += 1;
        }

        break;

    case 2:
        break;
    }
}

void op_bg1_0003(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1 << bgw_ptr->fam_no);

        switch (r_index) {
        case 83:
            bgw_ptr->wxy[0].cal = 0x2200000;
            bgw_ptr->xy[1].cal = 0;
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args) { 0x10, 1, 0, 0 });
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args) { 0x11, 2, 0, 0 });
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp = 0x80000;
            op_bg_mvxy[bgw_ptr->fam_no].d[0].sp = -0x8000;
            bgw_ptr->r_limit = 0x200;
            break;

        case 62:
            bgw_ptr->wxy[0].cal = 0x1E00000; // low = 0, pos = 480
            bgw_ptr->xy[1].cal = 0;
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 0xA, 0, 0, 0 });
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 0xB, 1, 0, 0 });
            oh_bg_blk_w(&op_w.bgw[1], &(Op_Bg_Blk_Args) { 0xC, 2, 0, 0 });
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp = 0xFFF80000;
            op_bg_mvxy[bgw_ptr->fam_no].d[0].sp = 0;
            break;
        }

        break;

    case 1:
        op_bg_mvxy[bgw_ptr->fam_no].a[0].sp += op_bg_mvxy[bgw_ptr->fam_no].d[0].sp;
        bgw_ptr->wxy[0].cal +=
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp; // Move background horizontally by the specified offset/speed

        if (bgw_ptr->wxy[0].disp.pos <= bgw_ptr->l_limit) {
            opw_ptr->r_no_0 += 1;
        }

        break;

    case 2:
        break;
    }
}

void op_bg2_move(s16 r_index) {
    opw_ptr = &op_w.bgw[2];
    bgw_ptr = &bg_w.bgw[2];

    switch (r_index) {
    case 0:
    case 24:
        op_bg2_0000();
        break;

    case 2:
        op_bg2_0002();
        break;

    case 28:
        op_bg2_0003();
        break;

    default:
        op_bg2_0001();
        break;
    }

    op_scrn_pos_set2(2);
}

/* What each op_bg2 scene does on its second step. */
static void op_bg2_0000_scroll(void) {
    bgw_ptr->wxy[0].cal -= (0x8000 + 0x8000);
}

static void op_bg2_0002_scroll(void) {
    bgw_ptr->xy[1].cal += 0x10000;
}

static void op_bg2_0003_scroll(void) {
    bgw_ptr->wxy[0].cal += 0x10000;
}

/* op_bg2_0000, op_bg2_0002 and op_bg2_0003 are the same three-step scene: place
 * the background, scroll it once, then hold. They disagree on where the
 * background goes and on which way it scrolls, so the position is a parameter
 * and the scroll is an action. */
static void run_op_bg2_scene(s32 x, void (*scroll)(void)) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = x;
        bgw_ptr->xy[1].disp.pos = 0;
        break;

    case 1:
        scroll();
        break;

    case 2:
        break;
    }
}

void op_bg2_0000() {
    run_op_bg2_scene(512, op_bg2_0000_scroll);
}

void op_bg2_0001() {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 0;
        break;

    case 1:
        break;
    }
}

void op_bg2_0002() {
    run_op_bg2_scene(0x200, op_bg2_0002_scroll);
}

void op_bg2_0003() {
    run_op_bg2_scene(0x200, op_bg2_0003_scroll);
}
