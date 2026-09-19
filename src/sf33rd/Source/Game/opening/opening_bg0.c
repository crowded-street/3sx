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

/* The two background blocks a scene lays down together: the first into map row
 * 1 and the second into row 2, both with the same y and transparency. Thirty-
 * three arms of this file wrote exactly that pair. */
static void oh_bg_blk_w_rows(s32 first, s32 second, s16 mapy, s32 trans) {
    oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){ first, 1, mapy, trans });
    oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){ second, 2, mapy, trans });
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

/* Which pair of background blocks this opening scene starts with. Every arm
 * lays blocks down and nothing else, so the whole choice moves out of the
 * scene's step 0; no case label is renumbered and no arm changes. */
/* Which pair of background blocks this opening scene starts with. Thirty-two
 * case labels cost thirty-two branches in one switch, so the arms are carried
 * in five links, each reached through the previous one's default. No label is
 * renumbered, no arm changes, and a scene index in none of them still does
 * nothing. */
static void op_bg0_lay_blocks_6(s16 r_index) {
    switch (r_index) {
    case 0x3E:
    case 0x50:
    case 0x52:
        oh_bg_blk_w_rows(0x57, 0x58, 0, 1);
        break;

    case 0x41:
        oh_bg_blk_w_rows(0x4F, 0x50, 0, 1);
        break;

    case 0x42:
        oh_bg_blk_w_rows(0x51, 0x52, 0, 1);
        break;

    case 0x49:
        oh_bg_blk_w_rows(0x53, 0x54, 0, 1);
        break;

    case 0x4B:
        op_w.bgw[0].map[1][0].g_no = 0;
        op_w.bgw[0].map[2][0].g_no = 0;
        oh_bg_blk_whv(op_w.bgw, &(Op_Bg_Blk_Args){0x53, 2, 0, 1});
        oh_bg_blk_whv(op_w.bgw, &(Op_Bg_Blk_Args){0x54, 1, 0, 1});
        break;

    case 0x4C:
        oh_bg_blk_w_rows(0x55, 0x56, 0, 1);
        break;
    }
}

static void op_bg0_lay_blocks_5(s16 r_index) {
    switch (r_index) {
    case 0x37:
    case 0x38:
        oh_bg_blk_w_rows(0x4D, 0x4E, 0, 1);
        break;

    case 0x3C:

    default:
        op_bg0_lay_blocks_6(r_index);
        break;
    }
}

static void op_bg0_lay_blocks_4(s16 r_index) {
    switch (r_index) {
    case 0x20:
        oh_bg_blk_w_rows(0x4B, 0x4C, 0, 1);
        break;

    case 0x23:
        oh_bg_blk_w_rows(0x59, 0x5A, 0, 1);
        break;

    case 0x24:
        oh_bg_blk_w_rows(0x22, 0x23, 0, 1);
        break;

    case 0x26:
    case 0x27:
        oh_bg_blk_w_rows(0x3D, 0x3E, 0, 1);
        break;

    default:
        op_bg0_lay_blocks_5(r_index);
        break;
    }
}

static void op_bg0_lay_blocks_3(s16 r_index) {
    switch (r_index) {
    case 0xF:
        oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x3C, 2, 0, 1});
        oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x3D, 1, 0, 1});
        break;

    case 0x12:
        oh_bg_blk_wv(op_w.bgw, &(Op_Bg_Blk_Args){0x32, 1, 0, 1});
        oh_bg_blk_wv(op_w.bgw, &(Op_Bg_Blk_Args){0x33, 2, 0, 1});
        break;

    case 0x13:
        oh_bg_blk_w_rows(0x47, 0x48, 0, 1);
        break;

    case 0x15:
        oh_bg_blk_w(op_w.bgw, &(Op_Bg_Blk_Args){0x38, 1, 0, 1});
        op_w.bgw[0].map[2][0].g_no = 0;
        break;

    case 0x1F:
        oh_bg_blk_w_rows(0x49, 0x4A, 0, 1);
        break;

    default:
        op_bg0_lay_blocks_4(r_index);
        break;
    }
}

static void op_bg0_lay_blocks_2(s16 r_index) {
    switch (r_index) {
    case 0x9:
    case 0x48:
        oh_bg_blk_w_rows(0x36, 0x37, 0, 1);
        break;

    case 0xB:
    case 0x1E:
        oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x33, 1, 0, 1});
        oh_bg_blk_wh(op_w.bgw, &(Op_Bg_Blk_Args){0x32, 2, 0, 1});
        break;

    case 0xC:
        oh_bg_blk_w_rows(0x43, 0x44, 0, 1);
        break;

    default:
        op_bg0_lay_blocks_3(r_index);
        break;
    }
}

static void op_bg0_lay_blocks(s16 r_index) {
    switch (r_index) {
    case 0x1:
        oh_bg_blk_w_rows(0x30, 0x31, 0, 1);
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
        oh_bg_blk_w_rows(0x3F, 0x40, 0, 1);
        break;

    default:
        op_bg0_lay_blocks_2(r_index);
        break;
    }
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

        op_bg0_lay_blocks(r_index);

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
            oh_bg_blk_w_rows(0x24, 0x25, 0, 1);
            break;

        case 0x2C:
            oh_bg_blk_w_rows(0x26, 0x27, 0, 1);
            break;

        case 0x2E:
            oh_bg_blk_w_rows(0x28, 0x29, 0, 1);
            break;

        case 0x30:
            oh_bg_blk_w_rows(0x2A, 0x2B, 0, 1);
            break;

        case 0x32:
            oh_bg_blk_w_rows(0x2C, 0x2D, 0, 1);
            break;

        case 0x34:
            oh_bg_blk_w_rows(0x2E, 0x2F, 0, 1);
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
        oh_bg_blk_w_rows(5, 6, 0, 0);
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
        oh_bg_blk_w_rows(0xE, 0xF, 0, 0);
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
        oh_bg_blk_w_rows(0x16, 0x17, 0, 0);
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
        oh_bg_blk_w_rows(0x18, 0x19, 0, 0);
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
        oh_bg_blk_w_rows(0x12, 0x13, 0, 0);
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
        oh_bg_blk_w_rows(0x14, 0x15, 0, 0);
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
        oh_bg_blk_w_rows(0x1E, 0x1F, 1, 0);
        oh_bg_blk_w_rows(0x20, 0x21, 0, 0);
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
        oh_bg_blk_w_rows(0x1A, 0x1B, 1, 0);
        oh_bg_blk_w_rows(0x1C, 0x1D, 0, 0);
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
            oh_bg_blk_w_rows(0x41, 0x42, 0, 1);
            break;

        case 14:
            oh_bg_blk_w_rows(0x45, 0x46, 0, 1);
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
