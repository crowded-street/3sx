/**
 * @file sc_sub_gauges.c
 * HUD gauges: vital, stun and super art
 *
 * Split out of sc_sub.c, which the UI parameter-object conversion and the
 * re-wrap that followed it had carried past 1000 lines. These are the meters
 * themselves - the vital and stun bars with their silver overlays and frames,
 * the super-art gauge, the player name plate and the stun and max marks - and
 * they are where the file's Code Duplication findings sat.
 */

#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "common.h"
#include "constants.h"
#include "core/input.h"
#include "core/renderer.h"
#include "port/config/config.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/glyph_renderer.h"
#include "sf33rd/Source/Game/ui/sc_data.h"
#include "structs.h"

#if !PSP
#include "core/xbox_buttons.h"
#endif

// forward decls
void silver_stun_put(u8 Pl_Num, s16 len);

void vital_put(const VitalBar* v, u16 priority) {
    u8 Pl_Num = v->Pl_Num;
    s8 atr = v->atr;
    s16 vital = v->vital;
    u8 kind = v->kind;

    if (No_Trans) {
        return;
    }

    if (vital == 0) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);

    if (vital == -1) {
        vital = 0;
    }

    scrscrntex[0].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(0, atr & 0x3F);

    if (kind) {
        scrscrntex[0].u = 0.0f;
        scrscrntex[3].u = 8.0f / 256.0f;
        scrscrntex[0].v = TO_UV_256(64.0f);
        scrscrntex[3].v = TO_UV_256(72.0f);
    } else {
        scrscrntex[0].u = 0.0f;
        scrscrntex[3].u = 8.0f / 256.0f;
        scrscrntex[0].v = TO_UV_256(72.0f);
        scrscrntex[3].v = TO_UV_256(80.0f);
    }

    if (Pl_Num == 0) {
        scrscrntex[0].x = (168 - vital);
        scrscrntex[3].x = 168.0f;
    } else {
        scrscrntex[0].x = 216.0f;
        scrscrntex[3].x = (vital + 216);
    }

    scrscrntex[0].y = 16.0f;
    scrscrntex[3].y = 24.0f;
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = -1;
    njDrawSprite(scrscrntex, 4, 0, 1);
}

void silver_vital_put(u8 Pl_Num) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[TopHUDPriority];
    njSetPaletteBankNumG(0, 9);
    scrscrntex[0].u = 224.0f / 256.0f;
    scrscrntex[3].u = 232.0f / 256.0f;
    scrscrntex[0].v = TO_UV_256(176.0f);
    scrscrntex[3].v = TO_UV_256(184.0f);

    if (Pl_Num == 0) {
        scrscrntex[0].x = 8.0f;
        scrscrntex[3].x = 168.0f;
    } else {
        scrscrntex[0].x = 216.0f;
        scrscrntex[3].x = 376.0f;
    }

    scrscrntex[0].y = 16.0f;
    scrscrntex[3].y = 24.0f;
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = -1;
    njDrawSprite(scrscrntex, 4, 0, 1);
}

static void fill_quad_corners(PAL_CURSOR_P* pos) {
    pos[1].x = pos[3].x;
    pos[1].y = pos[0].y;
    pos[2].x = pos[0].x;
    pos[2].y = pos[3].y;
}

void vital_base_put(u8 Pl_Num) {
    PAL_CURSOR vtx;
    PAL_CURSOR_P pos[4];
    PAL_CURSOR_COL col;

    if (No_Trans || SA_shadow_on) {
        return;
    }

    njColorBlendingMode(0, 1);
    vtx.p = pos;
    vtx.col = &col;
    col.color = 0x40000000;

    if (Pl_Num == 0) {
        pos[0].x = 8.0f;
        pos[3].x = 168.0f;
    } else {
        pos[0].x = 216.0f;
        pos[3].x = 376.0f;
    }

    pos[0].y = 18.0f;
    pos[3].y = 23.0f;
    fill_quad_corners(pos);
    njDrawPolygon2D(&vtx, 4, PrioBase[TopHUDFacePriority], 96);
}

void spgauge_base_put(u8 Pl_Num, s16 len) {
    PAL_CURSOR vtx;
    PAL_CURSOR_P pos[4];
    PAL_CURSOR_COL col;

    if (omop_cockpit == 0) {
        return;
    }

    if (omop_sa_bar_disp[Pl_Num] == 0) {
        return;
    }

    if (No_Trans || SA_shadow_on) {
        return;
    }

    njColorBlendingMode(0, 1);
    vtx.p = pos;
    vtx.col = &col;
    col.color = 0x80000000;

    if (Pl_Num == 0) {
        pos[0].x = 48.0f;
        pos[3].x = ((len * 8) + 48);
    } else {
        pos[0].x = 336.0f;
        pos[3].x = (336 - (len * 8));
    }

    pos[0].y = 210.0f;
    pos[3].y = 217.0f;
    fill_quad_corners(pos);
    njDrawPolygon2D(&vtx, 4, PrioBase[4], 96);
}

void stun_put(u8 Pl_Num, u8 stun) {
    if (No_Trans) {
        return;
    }

    if (stun == 0) {
        return;
    }

    if (omop_st_bar_disp[Pl_Num] == 0) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[TopHUDFacePriority];
    njSetPaletteBankNumG(0, 10);
    scrscrntex[0].u = 0.0f;
    scrscrntex[3].u = 8.0f / 256.0f;
    scrscrntex[0].v = TO_UV_256(96.0f);
    scrscrntex[3].v = TO_UV_256(104.0f);

    if (Pl_Num == 0) {
        scrscrntex[0].x = (168 - stun);
        scrscrntex[3].x = 168.0f;
    } else {
        scrscrntex[0].x = 216.0f;
        scrscrntex[3].x = (stun + 216);
    }

    scrscrntex[0].y = 24.0f;
    scrscrntex[3].y = 32.0f;
    scrscrntex[0].col = scrscrntex[3].col = -1;
    njDrawSprite(scrscrntex, 4, 0, 0);
}

void stun_base_put(u8 Pl_Num, s16 len) {
    PAL_CURSOR vtx;
    PAL_CURSOR_P pos[4];
    PAL_CURSOR_COL col;

    if (No_Trans || SA_shadow_on) {
        return;
    }

    njColorBlendingMode(0, 1);
    vtx.p = pos;
    vtx.col = &col;
    col.color = 0x90000000;

    if (Pl_Num == 0) {
        pos[0].x = (168 - (len * 8));
        pos[3].x = 168.0f;
    } else {
        pos[0].x = 216.0f;
        pos[3].x = ((len * 8) + 216);
    }

    pos[0].y = 25.0f;
    pos[3].y = 31.0f;
    fill_quad_corners(pos);
    // Fudged priority to fix overlap with stun_put
    njDrawPolygon2D(&vtx, 4, PrioBase[TopHUDFacePriority + 1], 96);
}

void player_name() {
    u8 pl1;
    u8 pl2;

    if (omop_cockpit == 0) {
        return;
    }

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    pl1 = My_char[0];
    pl2 = My_char[1];
    pl1 += chkNameAkuma(pl1, 6);
    pl2 += chkNameAkuma(pl2, 6);
    scfont_sqput(
        &(ScFontSquare){ 6, 3, 1, 1, Player_Name_Pos_TBL[pl1][0], Player_Name_Pos_TBL[pl1][1], 5, 1 },
        TopHUDPriority
    );
    scfont_sqput(
        &(ScFontSquare){ 37, 3, 1, 1, Player_Name_Pos_TBL[pl2][0], Player_Name_Pos_TBL[pl2][1], 5, 1 },
        TopHUDPriority
    );
}

void stun_mark_write(u8 Pl_Num, s16 Len) {
    s16 tlen;

    if (No_Trans) {
        return;
    }

    if (omop_st_bar_disp[Pl_Num] == 0) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    tlen = Len - 7;
    scfont_sqput(
        &(ScFontSquare){ smark_pos_tbl[tlen][Pl_Num],
                         3,
                         10,
                         0,
                         (smark_kind_tbl[tlen] * 4) + 1,
                         2,
                         smark_kind_tbl[tlen] + 4,
                         1 },
        TopHUDPriority
    );
}

void max_mark_write(s8 Pl_Num, u8 Gauge_Len, u8 Mchar, u8 Mass_Len) {
    if (Pl_Num == 0) {
        scfont_sqput2(
            &(ScFontSquareInv){ Mass_Len + 6,
                                26,
                                17,
                                0,
                                0,
                                Max_Pos_TBL[Mchar - 5][0],
                                Max_Pos_TBL[Mchar - 5][1],
                                Mchar,
                                1 }
        );
    } else {
        scfont_sqput2(
            &(ScFontSquareInv){ 42 - Gauge_Len + Mass_Len,
                                26,
                                17,
                                0,
                                0,
                                Max_Pos_TBL[Mchar - 5][0],
                                Max_Pos_TBL[Mchar - 5][1],
                                Mchar,
                                1 }
        );
    }
}

void ci_set(u8 type, u8 atr) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scfont_sqput(
        &(ScFontSquare){ ci_tbl[type][4],
                         ci_tbl[type][5],
                         atr,
                         cip_tbl[type],
                         ci_tbl[type][0],
                         ci_tbl[type][1],
                         ci_tbl[type][2],
                         ci_tbl[type][3] },
        2
    );
}

void nw_set(u8 PL_num, u8 atr) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    PL_num += chkNameAkuma(PL_num, 6);
    scfont_sqput(
        &(ScFontSquare){ nwdata_tbl[PL_num][3],
                         9,
                         atr,
                         nwdata_tbl[PL_num][4],
                         nwdata_tbl[PL_num][0],
                         nwdata_tbl[PL_num][1],
                         nwdata_tbl[PL_num][2],
                         4 },
        2
    );
    scfont_sqput(&(ScFontSquare){ nwdata_tbl[PL_num][5], 9, atr, 2, 17, 22, 13, 4 }, 2);
}

void stun_gauge_waku_write(s16 p1len, s16 p2len) {
    if (omop_cockpit == 0) {
        return;
    }

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);

    if (omop_st_bar_disp[0]) {
        scfont_sqput(&(ScFontSquare){ 21 - p1len, 3, 10, 0, 12 - p1len, p1len + 1, p1len, 1 }, TopHUDShadowPriority);
    } else {
        silver_stun_put(0, p1len);
    }

    scfont_sqput(&(ScFontSquare){ 11, 3, 1, 0, 2, p1len + 1, 10 - p1len, 1 }, TopHUDShadowPriority);

    if (omop_st_bar_disp[1]) {
        scfont_sqput(&(ScFontSquare){ 27, 3, 10, 0, 2, p2len + 12, p2len, 1 }, TopHUDShadowPriority);
    } else {
        silver_stun_put(1, p2len);
    }

    scfont_sqput(&(ScFontSquare){ p2len + 27, 3, 1, 0, p2len + 2, p2len + 12, 10 - p2len, 1 }, TopHUDShadowPriority);
}

void silver_stun_put(u8 Pl_Num, s16 len) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[TopHUDShadowPriority];
    njSetPaletteBankNumG(0, 1);

    scrscrntex[0].u = 240.0f / 256.0f;
    scrscrntex[3].u = 248.0f / 256.0f;
    scrscrntex[0].v = TO_UV_256(176.0f);
    scrscrntex[3].v = TO_UV_256(184.0f);

    if (Pl_Num == 0) {
        scrscrntex[0].x = ((21 - len) * 8);
        scrscrntex[3].x = 168.0f;
    } else {
        scrscrntex[0].x = 216.0f;
        scrscrntex[3].x = ((len + 27) * 8);
    }

    scrscrntex[0].y = 24.0f;
    scrscrntex[3].y = 32.0f;
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = 0xFFFFFFFF;
    njDrawSprite(scrscrntex, 4, 0, 1);
}

void sa_stock_trans(s16 St_Num, s16 Spg_Col, s8 Stpl_Num) {
    if (Stpl_Num == 0) {
        scfont_put2(&(ScFontCell){ 3, 25, sa_color_data_tbl[Spg_Col], 2, St_Num + 21, 4 });
        scfont_put2(&(ScFontCell){ 3, 26, sa_color_data_tbl[Spg_Col], 2, St_Num + 21, 5 });
    } else {
        scfont_put2(&(ScFontCell){ 44, 25, sa_color_data_tbl[Spg_Col], 2, St_Num + 21, 4 });
        scfont_put2(&(ScFontCell){ 44, 26, sa_color_data_tbl[Spg_Col], 2, St_Num + 21, 5 });
    }
}

void sa_fullstock_trans(s16 St_Num, s16 Spg_Col, s8 Stpl_Num) {
    if (Stpl_Num == 0) {
        scfont_put2(&(ScFontCell){ 1, 26, sa_color_data_tbl[Spg_Col], 2, St_Num + 21, 6 });
    } else {
        scfont_put2(&(ScFontCell){ 46, 26, sa_color_data_tbl[Spg_Col], 2, St_Num + 21, 7 });
    }
}

void sa_number_write(s8 Stpl_Num, u16 x) {
    if (Stpl_Num == 0) {
        if (My_char[0] == 0) {
            scfont_sqput2(&(ScFontSquareInv){ x, 26, 14, 0, 2, 27, 2, 2, 2 });
        } else {
            scfont_sqput2(&(ScFontSquareInv){ x, 26, 14, 0, 2, (Super_Arts[0] * 2) + 21, 2, 2, 2 });
        }
    } else if (My_char[1] == 0) {
        scfont_sqput2(&(ScFontSquareInv){ x, 26, 142, 1, 2, 27, 2, 2, 2 });
    } else {
        scfont_sqput2(&(ScFontSquareInv){ x, 26, 142, 1, 2, (Super_Arts[1] * 2) + 21, 2, 2, 2 });
    }
}
