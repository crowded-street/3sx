/**
 * @file sc_sub_logo.c
 * Logo, Portraits and Honour Wipe
 *
 * Split out of sc_sub.c: the SF3 logo animation, the player portrait panels,
 * and the honour-roll wipe that shares their tile handling.
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
#include "core/xbox_buttons.h"

void face_base_put();

static void draw_sf3_logo_open(Vertex* pos, u8 step) {
    s32 i;

    pos[0].x = pos[1].x = 128.0f;
    pos[2].y = pos[3].y = 128.0f;
    pos[0].s = pos[1].s = TO_UV_256(pos[0].x);
    pos[2].t = pos[3].t = TO_UV_256(240.0f);

    for (i = 48; i > 0; i -= 8) {
        pos[0].y = i + 80;
        pos[1].y = pos[0].y - step;
        pos[2].x = 176 - i;
        pos[3].x = pos[2].x + step;
        pos[0].t = TO_UV_256(i + 192);
        pos[1].t = TO_UV_256((i + 192) - step);
        pos[2].s = TO_UV_256(176 - i);
        pos[3].s = TO_UV_256((176 - i) + step);
        ppgWriteQuadWithST_B(pos, &(PPGQuadArgs){-1, NULL, 0, -1});
    }

    pos[0].y = pos[1].y = 80.0f;
    pos[2].y = pos[3].y = 128.0f;
    pos[0].t = pos[1].t = TO_UV_256(192.0f);
    pos[2].t = pos[3].t = TO_UV_256(240.0f);

    for (i = 128; i < 208; i += 8) {
        pos[0].x = i;
        pos[1].x = i + step;
        pos[2].x = 48.0f + pos[0].x;
        pos[3].x = 48.0f + pos[1].x;
        pos[0].s = TO_UV_256(pos[0].x);
        pos[1].s = TO_UV_256(pos[1].x);
        pos[2].s = TO_UV_256(pos[2].x);
        pos[3].s = TO_UV_256(pos[3].x);
        ppgWriteQuadWithST_B(pos, &(PPGQuadArgs){-1, NULL, 0, -1});
    }

    pos[0].y = pos[1].y = 80.0f;
    pos[2].x = pos[3].x = 256.0f;
    pos[0].t = pos[1].t = TO_UV_256(192.0f);
    pos[2].s = pos[3].s = TO_UV_256(256.0f);

    for (i = 0; i < 48; i += 8) {
        pos[0].x = i + 208;
        pos[1].x = pos[0].x + step;
        pos[2].y = 128 - i;
        pos[3].y = pos[2].y - step;
        pos[0].s = TO_UV_256(pos[0].x);
        pos[1].s = TO_UV_256(pos[1].x);
        pos[2].t = TO_UV_256(240 - i);
        pos[3].t = TO_UV_256((240 - i) - step);
        ppgWriteQuadWithST_B(pos, &(PPGQuadArgs){-1, NULL, 0, -1});
    }
}

static void draw_sf3_logo_close(Vertex* pos, u8 step) {
    s32 i;

    step -= 8;
    pos[0].x = pos[1].x = 128.0f;
    pos[2].y = pos[3].y = 128.0f;
    pos[0].s = pos[1].s = TO_UV_256(pos[0].x);
    pos[2].t = pos[3].t = TO_UV_256(240.0f);

    for (i = 40; i >= 0; i -= 8) {
        pos[1].y = i + 80;
        pos[0].y = (8.0f + pos[1].y) - step;
        pos[3].x = (176 - i);
        pos[2].x = (pos[3].x - 8.0f) + step;
        pos[0].t = TO_UV_256((i + 200) - step);
        pos[1].t = TO_UV_256(i + 192);
        pos[2].s = TO_UV_256((168 - i) + step);
        pos[3].s = TO_UV_256(176 - i);
        ppgWriteQuadWithST_B(pos, &(PPGQuadArgs){-1, NULL, 0, -1});
    }

    pos[0].y = pos[1].y = 80.0f;
    pos[2].y = pos[3].y = 128.0f;
    pos[0].t = pos[1].t = TO_UV_256(192.0f);
    pos[2].t = pos[3].t = TO_UV_256(240.0f);

    for (i = 128; i < 208; i += 8) {
        pos[0].x = (i + step);
        pos[1].x = (i + 8);
        pos[2].x = 48.0f + pos[0].x;
        pos[3].x = 48.0f + pos[1].x;
        pos[0].s = TO_UV_256(pos[0].x);
        pos[1].s = TO_UV_256(pos[1].x);
        pos[2].s = TO_UV_256(pos[2].x);
        pos[3].s = TO_UV_256(pos[3].x);
        ppgWriteQuadWithST_B(pos, &(PPGQuadArgs){-1, NULL, 0, -1});
    }

    pos[0].y = pos[1].y = 80.0f;
    pos[2].x = pos[3].x = 256.0f;
    pos[0].t = pos[1].t = TO_UV_256(192.0f);
    pos[2].s = pos[3].s = TO_UV_256(256.0f);

    for (i = 0; i < 48; i += 8) {
        pos[0].x = i + 208 + step;
        pos[1].x = i + 216;
        pos[2].y = 128 - i - step;
        pos[3].y = 120 - i;
        pos[0].s = TO_UV_256(pos[0].x);
        pos[1].s = TO_UV_256(pos[1].x);
        pos[2].t = TO_UV_256(240 - i - step);
        pos[3].t = TO_UV_256(232 - i);
        ppgWriteQuadWithST_B(pos, &(PPGQuadArgs){-1, NULL, 0, -1});
    }
}

void SF3_logo(u8 step) {
    Vertex pos[4];

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);
    njSetPaletteBankNumG(0, 29);
    pos[0].z = pos[1].z = pos[2].z = pos[3].z = PrioBase[2];

    if (step < 9) {
        draw_sf3_logo_open(pos, step);
    } else {
        draw_sf3_logo_close(pos, step);
    }
}

void player_face_init() {
    // Do nothing
}

typedef struct {
    u16 x;
    u16 y;
    u16 atr;
    u8 page;
    u8 cx1;
    u8 cy1;
    u8 cx2;
    u8 cy2;
} ScFontFace;

void scfont_sqput_face(const ScFontFace* c, u16 priority) {
    u16 x = c->x;
    u16 y = c->y;
    u16 atr = c->atr;
    u8 page = c->page;
    u8 cx1 = c->cx1;
    u8 cy1 = c->cy1;
    u8 cx2 = c->cx2;
    u8 cy2 = c->cy2;

    s32 u1;
    s32 u2;
    s32 v1;
    s32 v2;

    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = -1;
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(0, atr & 0x3FFF);
    x = x * 8;
    y = y * 8;
    u1 = cx1 * 8;
    u2 = u1 + (cx2 * 8);
    v1 = cy1 * 8;
    v2 = v1 + (cy2 * 8);

    if (atr & 0x8000) {
        scrscrntex[3].u = TO_UV_256_NEG(u1);
        scrscrntex[0].u = TO_UV_256_NEG(u2);
    } else {
        scrscrntex[0].u = TO_UV_256(u1);
        scrscrntex[3].u = TO_UV_256(u2);
    }

    if (atr & 0x4000) {
        scrscrntex[3].v = TO_UV_256_NEG(v1);
        scrscrntex[0].v = TO_UV_256_NEG(v2);
    } else {
        scrscrntex[0].v = TO_UV_256(v1);
        scrscrntex[3].v = TO_UV_256(v2);
    }

    scrscrntex[0].x = x;
    scrscrntex[3].x = (x + (u2 - u1));
    scrscrntex[0].y = y;
    scrscrntex[3].y = (y + (v2 - v1));
    njDrawSprite(scrscrntex, 4, page, 1);
}

void player_face() {
    u8 grade_tmp;

    if (omop_cockpit == 0) {
        return;
    }

    if (No_Trans) {
        return;
    }

    face_base_put();
    ppgSetupCurrentDataList(&ppgScrListFace);
    scfont_sqput_face(
        &(ScFontFace){ 0,
                       3,
                       Player_Color[0] + (My_char[0] * 13),
                       0,
                       Face_Pos_TBL[My_char[0]][0],
                       Face_Pos_TBL[My_char[0]][1],
                       5,
                       3 },
        TopHUDPriority
    );

    if (My_char[1] == 0) {
        scfont_sqput_face(
            &(ScFontFace){ 0x2B,
                           3,
                           (Player_Color[1] + (My_char[1] * 13)) | 0x8000,
                           0,
                           Face_Pos_TBL[20][0],
                           Face_Pos_TBL[20][1],
                           5,
                           3 },
            TopHUDPriority
        );
    } else {
        scfont_sqput_face(
            &(ScFontFace){ 0x2B,
                           3,
                           (Player_Color[1] + (My_char[1] * 13)) | 0x8000,
                           0,
                           Face_Pos_TBL[My_char[1]][0],
                           Face_Pos_TBL[My_char[1]][1],
                           5,
                           3 },
            TopHUDPriority
        );
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scfont_put(&(ScFontCell){ 5, 3, 1, 0, 0, 19 }, TopHUDPriority);
    scfont_put(&(ScFontCell){ 5, 4, 1, 0, 0, 20 }, TopHUDPriority);
    scfont_put(&(ScFontCell){ 42, 3, 129, 0, 0, 19 }, TopHUDPriority);
    scfont_put(&(ScFontCell){ 42, 4, 129, 0, 0, 20 }, TopHUDPriority);

    if (Play_Type == 0) {
        return;
    }

    if (Keep_Grade[Champion] == 0) {
        return;
    }

    grade_tmp = Keep_Grade[Champion] - 1;

    if (grade_tmp < 0x18) {
        scfont_sqput(
            &(ScFontSquare){ (Champion * 41) + 1,
                             1,
                             27,
                             2,
                             Grade_Pos_TBL[grade_tmp][0],
                             Grade_Pos_TBL[grade_tmp][1],
                             5,
                             1 },
            TopHUDPriority
        );
    } else {
        scfont_sqput(
            &(ScFontSquare){ (Champion * 41) + 1,
                             1,
                             28,
                             2,
                             Grade_Pos_TBL[grade_tmp][0],
                             Grade_Pos_TBL[grade_tmp][1],
                             5,
                             1 },
            TopHUDPriority
        );
    }
}

void face_base_put() {
    PAL_CURSOR vtx;
    PAL_CURSOR_P pos[4];
    PAL_CURSOR_COL col;

    if (No_Trans || SA_shadow_on) {
        return;
    }

    njColorBlendingMode(0, 1);
    vtx.p = pos;
    vtx.col = &col;
    col.color = 0x50000000;
    pos[0].x = 5.6f;
    pos[3].x = 34.4f;
    pos[0].y = 25.0f;
    pos[3].y = 45.0f;
    pos[1].x = pos[3].x;
    pos[1].y = pos[0].y;
    pos[2].x = pos[0].x;
    pos[2].y = pos[3].y;
    njDrawPolygon2D(&vtx, 4, PrioBase[TopHUDFacePriority], 0x60);
    pos[0].x = 348.8f;
    pos[3].x = 377.6f;
    pos[1].x = pos[3].x;
    pos[2].x = pos[0].x;
    njDrawPolygon2D(&vtx, 4, PrioBase[TopHUDFacePriority], 0x60);
}

void hnc_set(u8 num, u8 atr) {
    u8 i;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[TopHUDPriority];
    njSetPaletteBankNumG(1, atr & 0x3F);
    njColorBlendingMode(0, 1);

    for (i = 0; i < 2; i++) {
        if (i) {
            scrscrntex[0].u = TO_UV_256(0.0f);
            scrscrntex[3].u = TO_UV_256(num * 8);
            scrscrntex[0].v = TO_UV_256(96.0f);
            scrscrntex[3].v = TO_UV_256(120.0f);
            scrscrntex[0].x = 184.0f;
            scrscrntex[3].x = ((num + 23) * 8);
        } else {
            scrscrntex[0].u = TO_UV_256((23 - num) * 8);
            scrscrntex[3].u = TO_UV_256(184.0f);
            scrscrntex[0].v = TO_UV_256(72.0f);
            scrscrntex[3].v = TO_UV_256(96.0f);
            scrscrntex[0].x = ((23 - num) * 8);
            scrscrntex[3].x = 184.0f;
        }

        scrscrntex[0].y = 88.0f;
        scrscrntex[3].y = 112.0f;
        scrscrntex[0].col = scrscrntex[3].col = -1;
        njDrawSprite(scrscrntex, 4, 1, 1);
    }
}

void hnc_wipeinit(u8 atr) {
    ColoredVertex dmyvtx[4];
    u8 i;
    u8 j;
    u8 k;

    ppgSetupCurrentDataList(&ppgScrList);
    Hnc_Num = 0;
    scrscrntex[0].z = scrscrntex[1].z = scrscrntex[2].z = scrscrntex[3].z = PrioBase[2];
    njSetPaletteBankNumG(1, atr & 0x3F);
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[1].col = scrscrntex[2].col = scrscrntex[3].col = -1;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 26; j++) {
            for (k = 0; k < 4; k++) {
                scrscrntex[k].u = hnc_wipe_tbl1[j][k * 2] / 256.0f;
                scrscrntex[k].v = ((i * 24) + hnc_wipe_tbl1[j][(k * 2) + 1]) / 256.0f;
                scrscrntex[k].x = ((i * 184) + hnc_wipe_tbl1[j][k * 2]);
                scrscrntex[k].y = (hnc_wipe_tbl1[j][(k * 2) + 1] + 16);
                dmyvtx[k] = scrscrntex[k];
            }

            if (!No_Trans) {
                njDrawTexture(dmyvtx, 4, 1, 1);
            }
        }
    }
}

static void normalise_hnc_uv(ColoredVertex* vtx) {
    u8 k;

    for (k = 0; k < 4; k++) {
        vtx[k].u /= 256.0f;
        vtx[k].v /= 256.0f;
    }
}

s32 hnc_wipeout(u8 atr) {
    ColoredVertex vtx[4];
    u8 i;
    u8 j;
    s32 ipx;
    s32 ipy;
    s32 ipu;
    s32 ipv;
    s32 len;

    if (!No_Trans) {
        ppgSetupCurrentDataList(&ppgScrList);
        njSetPaletteBankNumG(1, atr & 0x3F);
        njColorBlendingMode(0, 1);
        vtx[0].z = vtx[1].z = vtx[2].z = vtx[3].z = PrioBase[2];
        vtx[0].col = vtx[1].col = vtx[2].col = vtx[3].col = -1;
        ipx = 8;
        ipy = 88;
        ipu = 8;
        ipv = 72;
        len = 8 - Hnc_Num;

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 23; j++) {
                vtx[0].x = ipx;
                vtx[1].x = vtx[0].x - len;
                vtx[2].x = 16.0f + vtx[0].x;
                vtx[3].x = vtx[2].x - len;
                vtx[0].y = vtx[1].y = ipy;
                vtx[2].y = vtx[3].y = ipy + 24;
                vtx[0].u = ipu;
                vtx[1].u = vtx[0].u - len;
                vtx[2].u = 16.0f + vtx[0].u;
                vtx[3].u = vtx[2].u - len;
                vtx[0].v = vtx[1].v = ipv;
                vtx[2].v = vtx[3].v = ipv + 24;

                normalise_hnc_uv(vtx);

                njDrawTexture(vtx, 4, 1, 1);
                ipx += 8;
                ipu += 8;
            }

            ipu = 8;
            ipv += 24;
        }

        ipx = 184;
        ipu = 0;
        ipv -= 24;

        for (j = 0; j < 2; j++) {
            vtx[0].x = vtx[1].x = ipx;
            vtx[2].x = vtx[0].x + (16 - (j * 8));
            vtx[3].x = vtx[2].x - len;
            vtx[0].y = ipy;
            vtx[1].y = vtx[0].y + ((24.0f * len) / 16.0f);
            vtx[2].y = vtx[3].y = vtx[0].y + (0x18 - (j * 12));
            vtx[0].u = vtx[1].u = ipu;
            vtx[2].u = vtx[0].u + (16 - (j * 8));
            vtx[3].u = vtx[2].u - len;
            vtx[0].v = ipv;
            vtx[1].v = vtx[0].v + ((24.0f * len) / 16.0f);
            vtx[2].v = vtx[3].v = vtx[0].v + (24 - (j * 12));

            normalise_hnc_uv(vtx);

            njDrawTexture(vtx, 4, 1, 1);
            ipy += 12;
            ipv += 12;
        }
    }

    Hnc_Num++;

    if (Hnc_Num == 8) {
        return 1;
    }

    return 0;
}
