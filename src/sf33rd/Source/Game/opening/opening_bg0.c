/**
 * @file opening_bg0.c
 * Opening
 * The first opening background layer and its sixteen states.
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


void op_bg_move(s16 r_index) {
    op_bg0_move(r_index);
    op_bg1_move(r_index);
    op_bg2_move(r_index);
}

void op_bg0_move(s16 r_index) {
    void (*op_bg0_move_jp[94])(
        s16) = { op_bg0_0000, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001,
                 op_bg0_0015, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0015, op_bg0_0001,
                 op_bg0_0000, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000,
                 op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0001, op_bg0_0001,
                 op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001,
                 op_bg0_0000, op_bg0_0002, op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0002,
                 op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0000, op_bg0_0004, op_bg0_0001,
                 op_bg0_0001, op_bg0_0004, op_bg0_0005, op_bg0_0002, op_bg0_0001, op_bg0_0002, op_bg0_0001, op_bg0_0002,
                 op_bg0_0006, op_bg0_0001, op_bg0_0001, op_bg0_0001, op_bg0_0007, op_bg0_0008, op_bg0_0000, op_bg0_0000,
                 op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0000, op_bg0_0004,
                 op_bg0_0001, op_bg0_0004, op_bg0_0001, op_bg0_0002, op_bg1_0003, op_bg0_0002, op_bg0_0010, op_bg0_0002,
                 op_bg0_0011, op_bg0_0012, op_bg0_0013, op_bg0_0014, op_bg0_0002, op_bg0_0016 };

    opw_ptr = &op_w.bgw[0];
    bgw_ptr = &bg_w.bgw[0];
    op_bg0_move_jp[r_index](r_index);
}

void op_bg0_0000(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_Off_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0001(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 0x1:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x30, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x31, 2, 0, 1});
            break;

        case 0x3:
            oh_bg_blk_wv(op_w.bgw, &(Op_Bg_Blk_Args){0x45, 1, 0, 1});
            oh_bg_blk_wv(op_w.bgw, &(Op_Bg_Blk_Args){0x46, 2, 0, 1});
            op_w.bgw[0].map[1][0].col.full = -0x01000000;
            op_w.bgw[0].map[2][0].col.full = -0x01000000;
            break;

        case 0x4:
        case 0x16:
        case 0x21:
            oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x35, 1, 0, 1});
            oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x34, 2, 0, 1});
            break;

        case 0x7:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x3F, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x40, 2, 0, 1});
            break;

        case 0x9:
        case 0x48:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x36, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x37, 2, 0, 1});
            break;

        case 0xB:
        case 0x1E:
            oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x33, 1, 0, 1});
            oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x32, 2, 0, 1});
            break;

        case 0xC:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x43, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x44, 2, 0, 1});
            break;

        case 0xF:
            oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x3C, 2, 0, 1});
            oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x3D, 1, 0, 1});
            break;

        case 0x12:
            oh_bg_blk_wv(op_w.bgw, &(Op_Bg_Blk_Args){0x32, 1, 0, 1});
            oh_bg_blk_wv(op_w.bgw, &(Op_Bg_Blk_Args){0x33, 2, 0, 1});
            break;

        case 0x13:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x47, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x48, 2, 0, 1});
            break;

        case 0x15:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x38, 1, 0, 1});
            op_w.bgw[0].map[2][0].g_no = 0;
            break;

        case 0x1F:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x49, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x4A, 2, 0, 1});
            break;

        case 0x20:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x4B, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x4C, 2, 0, 1});
            break;

        case 0x23:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x59, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x5A, 2, 0, 1});
            break;

        case 0x24:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x22, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x23, 2, 0, 1});
            break;

        case 0x26:
        case 0x27:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x3D, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x3E, 2, 0, 1});
            break;

        case 0x37:
        case 0x38:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x4D, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x4E, 2, 0, 1});
            break;

        case 0x3C:
        case 0x3E:
        case 0x50:
        case 0x52:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x57, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x58, 2, 0, 1});
            break;

        case 0x41:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x4F, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x50, 2, 0, 1});
            break;

        case 0x42:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x51, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x52, 2, 0, 1});
            break;

        case 0x49:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x53, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x54, 2, 0, 1});
            break;

        case 0x4B:
            op_w.bgw[0].map[1][0].g_no = 0;
            op_w.bgw[0].map[2][0].g_no = 0;
            oh_bg_blk_whv(op_w.bgw, &(Op_Bg_Blk_Args){0x53, 2, 0, 1});
            oh_bg_blk_whv(op_w.bgw, &(Op_Bg_Blk_Args){0x54, 1, 0, 1});
            break;

        case 0x4C:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x55, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x56, 2, 0, 1});
            break;
        }

        /* fallthrough */

    case 1:
        bgw_ptr->free -= 1;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 1;
            bgw_ptr->frame_deff += 1;
            bgw_ptr->frame_deff &= 0xF;
            bgw_ptr->xy[1].disp.pos += op_quake_y_tbl0[bgw_ptr->frame_deff];
        }

        break;
    }

    op_scrn_pos_set2(0);
}

static void draw_bg0_fade_overlay(PAL_CURSOR* beta_poly) {
    if (!No_Trans) {
        njDrawPolygon2D(beta_poly, 4, PrioBase[75], 0x20);
    }
}

void op_bg0_0002(s16 r_index) {
    PAL_CURSOR beta_poly;
    PAL_CURSOR_P beta_p[4];
    PAL_CURSOR_COL beta_col[4];

    beta_poly.p = beta_p;
    beta_poly.col = beta_col;
    beta_poly.num = 4;
    beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color = 0xFF000000;
    beta_p[0].x = beta_p[2].x = 0.0f;
    beta_p[1].x = beta_p[3].x = 384.0f;
    beta_p[0].y = beta_p[1].y = 0.0f;
    beta_p[2].y = beta_p[3].y = 224.0f;

    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;

        Bg_Off_W(1);

        if (r_index == 87) {
            Zoomf_Init();
        }

        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        draw_bg0_fade_overlay(&beta_poly);

        break;

    case 1:
        draw_bg0_fade_overlay(&beta_poly);

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0003(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 0x2A:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x24, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x25, 2, 0, 1});
            break;

        case 0x2C:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x26, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x27, 2, 0, 1});
            break;

        case 0x2E:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x28, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x29, 2, 0, 1});
            break;

        case 0x30:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x2A, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x2B, 2, 0, 1});
            break;

        case 0x32:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x2C, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x2D, 2, 0, 1});
            break;

        case 0x34:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x2E, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x2F, 2, 0, 1});
            break;
        }

        break;

    case 1:
        break;
    }

    op_scrn_pos_set2(0);
}

const s32 ot_bg0_0004_tbl[6] = { 0xFF00A0B0, 0xFF005888, 0xFF00A0B0, 0xFF005888, 0xFF000058, 0xFF000000 };

static void advance_bg0_fade(PAL_CURSOR_COL* beta_col) {
    bgw_ptr->free -= 1;

    if (bgw_ptr->free <= 0) {
        bgw_ptr->l_limit += 1;

        if (bgw_ptr->l_limit >= 6) {
            opw_ptr->r_no_0 += 1;
        } else {
            bgw_ptr->free = 1;
            beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color =
                ot_bg0_0004_tbl[bgw_ptr->l_limit];
        }
    }
}

void op_bg0_0004(s16 r_index) {
    PAL_CURSOR beta_poly;
    PAL_CURSOR_P beta_p[4];
    PAL_CURSOR_COL beta_col[4];

    beta_poly.p = beta_p;
    beta_poly.col = beta_col;
    beta_poly.num = 4;
    beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color = 0xFF000000;
    beta_p[0].x = beta_p[2].x = 0.0f;
    beta_p[1].x = beta_p[3].x = 384.0f;
    beta_p[0].y = beta_p[1].y = 0.0f;
    beta_p[2].y = beta_p[3].y = 224.0f;

    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_Off_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        draw_bg0_fade_overlay(&beta_poly);
        break;

    case 1:
        draw_bg0_fade_overlay(&beta_poly);

        if (!op_scrn_end) {
            break;
        }

        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->l_limit = 0;

        /* fallthrough */

    case 2:
        advance_bg0_fade(beta_col);
        draw_bg0_fade_overlay(&beta_poly);
        break;

    case 3:
        beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color = ot_bg0_0004_tbl[5];

        draw_bg0_fade_overlay(&beta_poly);
        break;
    }

    op_scrn_pos_set2(0);
}

const s16 op_bg0_0005_tbl[16] = { 0x0008, 0xFFF8, 0x0007, 0xFFF9, 0x0005, 0xFFFB, 0x0004, 0xFFFC,
                                  0x0002, 0xFFFE, 0x0001, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000 };

void op_bg0_0005(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){5, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){6, 2, 0, 0});
        Zoom_Value_Set(0x40);
        bgw_ptr->frame_deff = 12;
        Frame_Up(0xC0, 0xE0, bgw_ptr->frame_deff);
        break;

    case 1:
        bgw_ptr->frame_deff -= 2;

        if (bgw_ptr->frame_deff <= 0) {
            opw_ptr->r_no_0 += 1;
            Zoomf_Init();
            Zoom_Value_Set(0x40);
            bgw_ptr->free = 0;
            bgw_ptr->wxy[0].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
            bgw_ptr->xy[1].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
        } else {
            Frame_Down(0xC0, 0xE0, 2);
        }

        break;

    case 2:
        bgw_ptr->free += 1;

        if (bgw_ptr->free >= 0x10) {
            opw_ptr->r_no_0 += 1;
        } else {
            bgw_ptr->wxy[0].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
            bgw_ptr->xy[1].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0006(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0xC00000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0xD, 0, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0xE, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0xF, 2, 0, 0});
        break;

    case 1:
        bgw_ptr->wxy[0].cal += 0x40000;

        if (bgw_ptr->wxy[0].disp.pos > 0x2C0) {
            opw_ptr->r_no_0 += 1;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0007(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x1D00000;
        bgw_ptr->xy[1].cal = 0xFFF00000;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x16, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x17, 2, 0, 0});
        break;

    case 1:
        if (bgw_ptr->wxy[0].disp.pos < 0x200) {
            bgw_ptr->wxy[0].cal += 0x7FFF + 0x4001;
        } else {
            bgw_ptr->wxy[0].cal = 0x2000000;
        }

        if (bgw_ptr->xy[1].disp.pos < 0) {
            bgw_ptr->xy[1].cal += 0x4000;
        } else {
            bgw_ptr->xy[1].cal = 0;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0008(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2300000;
        bgw_ptr->xy[1].cal = 0xFFF00000;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x18, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x19, 2, 0, 0});
        /* fallthrough */

    case 1:
        if (bgw_ptr->wxy[0].disp.pos > 0x200) {
            bgw_ptr->wxy[0].cal -= 0x8000 + 0x4000;
        } else {
            bgw_ptr->wxy[0].cal = 0x2000000;
        }

        if (bgw_ptr->xy[1].disp.pos < 0) {
            bgw_ptr->xy[1].cal += 0x4000;
        } else {
            bgw_ptr->xy[1].cal = 0;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0010(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        Zoomf_Init();
        Zoom_Value_Set(0x40);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x12, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x13, 2, 0, 0});
        break;

    case 1:
        opw_ptr->r_no_0 += 1;
        break;

    case 2:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->frame_deff = 12;
        Frame_Up(0xC0, 0xE0, bgw_ptr->frame_deff);
        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0011(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1);
        Zoomf_Init();
        Zoom_Value_Set(0x40);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x14, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x15, 2, 0, 0});
        break;

    case 1:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->frame_deff = 0x38;
        Frame_Up(0xC0, 0x40, bgw_ptr->frame_deff);
        bgw_ptr->xy[1].cal = 0xFFE00000;
        /* fallthrough */

    case 2:
        if (bgw_ptr->xy[1].disp.pos < 0) {
            bgw_ptr->xy[1].cal += 0x20000;
        }

        if (bgw_ptr->frame_deff > 0) {
            bgw_ptr->frame_deff -= 1;
            Frame_Down(0xC0, 0x40, 1);
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0012(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        Zoomf_Init();
        Zoom_Value_Set(0x40);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0x1000000;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x1E, 1, 1, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x1F, 2, 1, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x20, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x21, 2, 0, 0});
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0xFFFE0000;

        if (bgw_ptr->xy[1].disp.pos < 0x61) {
            opw_ptr->r_no_0 += 1;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0013(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0x600000;
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x1A, 1, 1, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x1B, 2, 1, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x1C, 1, 0, 0});
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x1D, 2, 0, 0});
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0x20000;

        if (bgw_ptr->xy[1].disp.pos > 0x100) {
            opw_ptr->r_no_0 += 1;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0014(s16 r_index) {
    op_bg0_0000(r_index);
}

const s32 ot_bg0_0015_tbl[6] = { 0xFF00A0B0, 0xFF005888, 0xFF00A0B0, 0xFF005888, 0xFF000058, 0xFF000000 };

static void advance_bg0_tile_fade(void) {
    bgw_ptr->free -= 1;

    if (bgw_ptr->free <= 0) {
        bgw_ptr->l_limit += 1;

        if (bgw_ptr->l_limit >= 6) {
            opw_ptr->r_no_0 += 1;
        } else {
            bgw_ptr->free = 1;
            op_w.bgw[0].map[1][0].col.full = ot_bg0_0015_tbl[bgw_ptr->l_limit];
            op_w.bgw[0].map[2][0].col.full = ot_bg0_0015_tbl[bgw_ptr->l_limit];
        }
    }
}

void op_bg0_0015(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->l_limit = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 8:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x41, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x42, 2, 0, 1});
            break;

        case 14:
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x45, 1, 0, 1});
            oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x46, 2, 0, 1});
            break;
        }

        op_w.bgw[0].map[1][0].col.full = 0xFF000000;
        op_w.bgw[0].map[2][0].col.full = 0xFF000000;
        break;

    case 1:
        op_w.bgw[0].map[1][0].col.full = 0xFF000000;
        op_w.bgw[0].map[2][0].col.full = 0xFF000000;
        advance_bg0_tile_fade();
        break;

    case 2:
        op_w.bgw[0].map[1][0].col.full = 0xFF000000;
        op_w.bgw[0].map[2][0].col.full = 0xFF000000;
        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0016(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        op_scrn_end = 0;
        bgw_ptr->frame_deff = 0x13;
        Frame_Up(0xC0, 0x70, bgw_ptr->frame_deff);
        bgw_ptr->free = 0xA;
        break;

    case 1:
        bgw_ptr->free -= 1;

        if (bgw_ptr->free <= 0) {
            opw_ptr->r_no_0 += 1;
        }

        break;

    case 2:
        bgw_ptr->frame_deff -= 1;

        if (bgw_ptr->frame_deff >= 0) {
            Frame_Down(0xC0, 0x70, 1);
        } else {
            opw_ptr->r_no_0 += 1;
            op_scrn_end = 1;
        }

        break;

    case 3:
        break;
    }

    op_scrn_pos_set2(0);
}
