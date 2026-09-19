/**
 * @file sc_sub.c
 * HUD elements and screen transitions
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

#define TO_UV_128(val) ((val) / 128.0f)

/// Trim values for ASCII characters (high nibble = left trim, low nibble = right trim)
const u8 ascProData[128] = {
    0x00, // 0x00
    0x12, // 0x01
    0x00, // 0x02
    0x00, // 0x03
    0x00, // 0x04
    0x00, // 0x05
    0x00, // 0x06
    0x00, // 0x07
    0x00, // 0x08
    0x00, // 0x09
    0x00, // 0x0A
    0x00, // 0x0B
    0x00, // 0x0C
    0x00, // 0x0D
    0x00, // 0x0E
    0x00, // 0x0F
    0x00, // 0x10
    0x00, // 0x11
    0x00, // 0x12
    0x00, // 0x13
    0x00, // 0x14
    0x00, // 0x15
    0x00, // 0x16
    0x00, // 0x17
    0x00, // 0x18
    0x00, // 0x19
    0x00, // 0x1A
    0x00, // 0x1B
    0x00, // 0x1C
    0x00, // 0x1D
    0x00, // 0x1E
    0x00, // 0x1F
    0x22, // space
    0x13, // !
    0x12, // "
    0x00, // #
    0x00, // $
    0x00, // %
    0x00, // &
    0x22, // '
    0x22, // (
    0x22, // )
    0x01, // *
    0x01, // +
    0x22, // ,
    0x01, // -
    0x22, // .
    0x00, // /
    0x00, // 0
    0x12, // 1
    0x00, // 2
    0x00, // 3
    0x00, // 4
    0x00, // 5
    0x00, // 6
    0x00, // 7
    0x00, // 8
    0x00, // 9
    0x22, // :
    0x22, // ;
    0x11, // <
    0x00, // =
    0x11, // >
    0x00, // ?
    0x00, // @
    0x00, // A
    0x00, // B
    0x00, // C
    0x00, // D
    0x00, // E
    0x00, // F
    0x00, // G
    0x00, // H
    0x22, // I
    0x00, // J
    0x00, // K
    0x00, // L
    0x00, // M
    0x00, // N
    0x00, // O
    0x00, // P
    0x00, // Q
    0x00, // R
    0x00, // S
    0x00, // T
    0x00, // U
    0x00, // V
    0x00, // W
    0x00, // X
    0x00, // Y
    0x00, // Z
    0x11, // [
    0x00, // backslash
    0x11, // ]
    0x01, // ^
    0x00, // _
    0x22, // `
    0x00, // a
    0x00, // b
    0x00, // c
    0x00, // d
    0x00, // e
    0x00, // f
    0x00, // g
    0x00, // h
    0x22, // i
    0x02, // j
    0x00, // k
    0x22, // l
    0x00, // m
    0x00, // n
    0x00, // o
    0x00, // p
    0x00, // q
    0x10, // r
    0x00, // s
    0x00, // t
    0x00, // u
    0x00, // v
    0x00, // w
    0x00, // x
    0x00, // y
    0x00, // z
    0x12, // {
    0x23, // |
    0x21, // }
    0x00, // ~
    0x21, // 0x7F
};

SAFrame sa_frame[3][48];
ColoredVertex scrscrntex[4];
u8 WipeLimit;
u8 FadeLimit;
s16 Hnc_Num;
FadeData fd_dat;

int TopHUDPriority;
int TopHUDShadowPriority;
int TopHUDFacePriority;
int TopHUDVitalPriority;

// forward decls
s32 SSGetDrawSizePro(const char* str);
s16 SSPutStrTexInputPro(u16 x, u16 y, u16 ix);
void face_base_put();
void silver_stun_put(u8 Pl_Num, s16 len);

void HUD_Shift_Init() {
    if (Config_GetBool(CFG_DRAW_PLAYERS_ABOVE_HUD)) {
        TopHUDPriority = 2 + HUD_SHIFT;
        TopHUDShadowPriority = 3 + HUD_SHIFT;
        TopHUDFacePriority = 4 + HUD_SHIFT;
        TopHUDVitalPriority = 5 + HUD_SHIFT;
    } else {
        TopHUDPriority = 2;
        TopHUDShadowPriority = 3;
        TopHUDFacePriority = 4;
        TopHUDVitalPriority = 5;
    }
}

void Scrscreen_Init() {
    void* loadAdrs;
    u32 loadSize;
    s16 i;
    s16 key;

    ppgScrList.tex = ppgScrListFace.tex = ppgScrListShot.tex = ppgScrListOpt.tex = &ppgScrTex;
    ppgScrList.pal = &ppgScrPal;
    ppgScrListFace.pal = &ppgScrPalFace;
    ppgScrListShot.pal = &ppgScrPalShot;
    ppgScrListOpt.pal = &ppgScrPalOpt;
    ppgSetupCurrentDataList(&ppgScrList);
    loadSize = load_it_use_any_key2(10, &loadAdrs, &key, 2, 0); // scrscrn.ppg

    if (loadSize == 0) {
        flLogOut("Couldn't load scrscrn.ppg\n");
    }

    ppgSetupPalChunk(&ppgScrPalOpt, &(PPGPalChunkArgs){loadAdrs, loadSize, 0, 3, 1});
    ppgSetupPalChunk(&ppgScrPalShot, &(PPGPalChunkArgs){loadAdrs, loadSize, 0, 2, 1});
    ppgSetupPalChunk(&ppgScrPalFace, &(PPGPalChunkArgs){loadAdrs, loadSize, 0, 1, 1});
    ppgSetupPalChunk(NULL, &(PPGPalChunkArgs){loadAdrs, loadSize, 0, 0, 1});
    ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){loadAdrs, loadSize, 0, 6, 0, 0});

    for (i = 0; i < 3; i++) {
        ppgSetupTexChunk_2nd(NULL, i);
        ppgSetupTexChunk_3rd(NULL, i, 1);
    }

    for (i = 3; i < ppgScrTex.textures; i++) {
        ppgSetupTexChunk_2nd(NULL, i);
        ppgSetupTexChunk_3rd(NULL, i, 1);
    }

    Push_ramcnt_key(key);
    ppgSourceDataReleased(NULL);
    Sa_frame_Clear();

#if !PSP
    XboxButtons_Init();
#endif

    GlyphRenderer_Init();
}

static void clear_sa_frame_range(u8 first, u8 limit) {
    u8 i;
    u8 j;

    for (j = 0; j < 3; j++) {
        for (i = first; i < limit; i++) {
            sa_frame[j][i].atr = 0;
            sa_frame[j][i].page = 0;
            sa_frame[j][i].cx = 0;
            sa_frame[j][i].cy = 0;
        }
    }
}

void Sa_frame_Clear() {
    clear_sa_frame_range(0, 48);
}

void Sa_frame_Clear2(u8 pl) {
    clear_sa_frame_range(pl * 24, (pl * 24) + 24);
}

static void write_sa_frame_range(u8 first, u8 limit) {
    u8 i;
    u8 j;

    for (j = 0; j < 3; j++) {
        for (i = first; i < limit; i++) {
            if (sa_frame[j][i].atr != 0) {
                scfont_put(
                    &(ScFontCell){ i,
                                   j + 25,
                                   sa_frame[j][i].atr,
                                   sa_frame[j][i].page,
                                   sa_frame[j][i].cx,
                                   sa_frame[j][i].cy },
                    2
                );
            }
        }
    }
}

void Sa_frame_Write() {
    if (omop_cockpit == 0) {
        return;
    }

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);

    if (omop_sa_bar_disp[0]) {
        write_sa_frame_range(0, 24);
    }

    if (omop_sa_bar_disp[1]) {
        write_sa_frame_range(24, 48);
    }
}

static void set_glyph_quad_pos(u16 x, u16 y) {
    scrscrntex[0].x = x;
    scrscrntex[3].x = (x + 8);
    scrscrntex[0].y = y;
    scrscrntex[3].y = (y + 8);
}

void SSPutStrTexInput(u16 x, u16 y, const char* str) {
    s32 u = ((*str & 0xF) * 8) + 0x80;
    s32 v = ((*str & 0xF0) >> 4) * 8;

    scrscrntex[0].u = TO_UV_256(u);
    scrscrntex[3].u = TO_UV_256(u + 8);
    scrscrntex[0].v = TO_UV_256(v);
    scrscrntex[3].v = TO_UV_256(v + 8);
    set_glyph_quad_pos(x, y);
}

void SSPutStrTexInput2(u16 x, u16 y, u8 str) {
    s32 u;

    u = (str * 8) + 128;

    scrscrntex[0].u = TO_UV_256(u);
    scrscrntex[3].u = TO_UV_256(u + 8);
    scrscrntex[0].v = TO_UV_256(0.0f);
    scrscrntex[3].v = TO_UV_256(8.0f);
    set_glyph_quad_pos(x, y);
}

void SSPutStr(const ScStr* s, u16 priority) {
    u16 x = s->x;
    u16 y = s->y;
    u8 atr = s->atr;
    const char* str = s->str;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = 0xFFFFFFFF;
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(1, atr & 0x3F);
    x = x * 8;
    y = y * 8;

    while (*str != '\0') {
        if (*str != ',') {
            SSPutStrTexInput(x, y, str);
        } else {
            SSPutStrTexInput(x, y + 2, str);
        }

        njDrawSprite(scrscrntex, 4, 1, 1);
        x += 8;
        str++;
    }
}

s32 SSPutStrPro(const ScStrPro* s) {
    u16 flag = s->flag;
    u16 x = s->x;
    u16 y = s->y;
    u8 atr = s->atr;
    u32 vtxcol = s->vtxcol;
    const char* str = s->str;

    s32 usex;
    s16 step;

    if (No_Trans) {
        return x;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = vtxcol;
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[2];
    njSetPaletteBankNumG(1, atr & 0x3F);

    if (flag) {
        x = (x - SSGetDrawSizePro(str)) / 2;
    }

    usex = x;

    while (*str != '\0') {
        if (*str != ',') {
            step = SSPutStrTexInputPro(x, y, *str);
        } else {
            step = SSPutStrTexInputPro(x, y + 2, *str);
        }

        str++;
        x += step;
        njDrawSprite(scrscrntex, 4, 1, 1);
    }

    return usex;
}

s16 SSPutStrTexInputPro(u16 x, u16 y, u16 ix) {
    s16 slide;
    s16 sideL;
    s16 sideR;
    s32 u;
    s32 v;

    u = (ix & 0xF) * 8 + 0x80;
    v = ((ix & 0xF0) >> 4) * 8;

    sideL = (ascProData[ix] >> 4) & 0xF;
    sideR = ascProData[ix] & 0xF;
    scrscrntex[0].u = TO_UV_256(u + sideL);
    scrscrntex[3].u = TO_UV_256(u + 8 - sideR);
    scrscrntex[0].v = TO_UV_256(v);
    scrscrntex[3].v = TO_UV_256(v + 8);
    slide = (8 - sideL) - sideR;
    scrscrntex[0].x = x;
    scrscrntex[3].x = (x + slide);
    scrscrntex[0].y = y;
    scrscrntex[3].y = (y + 8);
    return slide;
}

s32 SSGetDrawSizePro(const char* str) {
    s32 ix;
    s32 size = 0;

    while (*str != '\0') {
        ix = *str++;
        ix &= 0x7F;
        size += 8 - ((ascProData[ix] >> 4) & 0xF) - (ascProData[ix] & 0xF);
    }

    return size;
}

void SSPutStr2(u16 x, u16 y, u8 atr, const char* str) {
    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = -1;
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[1];
    njSetPaletteBankNumG(1, atr & 0x3F);
    x = x * 8;
    y = y * 8;

    while (*str != '\0') {
        SSPutStrTexInput(x, y, str);
        njDrawSprite(scrscrntex, 4, 1, 1);
        x += 8;
        str++;
    }
}

void SSPutStrTexInputB(f32 x, f32 y, const char* str, f32 sc) {
    s32 u = ((*str & 0xF) * 8) + 128;
    s32 v = ((*str & 0xF0) >> 4) * 8;

    scrscrntex[0].u = scrscrntex[1].u = TO_UV_256(u);
    scrscrntex[2].u = scrscrntex[3].u = TO_UV_256(u + 8);
    scrscrntex[0].v = scrscrntex[2].v = TO_UV_256(v);
    scrscrntex[1].v = scrscrntex[3].v = TO_UV_256(v + 8);
    scrscrntex[0].x = scrscrntex[1].x = x;
    scrscrntex[2].x = scrscrntex[3].x = (x + (8.0f * sc));
    scrscrntex[0].y = scrscrntex[2].y = y;
    scrscrntex[1].y = scrscrntex[3].y = (y + (8.0f * sc));
}

void SSPutStrTexInputB2(f32 x, f32 y, s8 str) {
    s32 u = str * 11;

    scrscrntex[0].u = scrscrntex[1].u = TO_UV_256(u);
    scrscrntex[2].u = scrscrntex[3].u = TO_UV_256(u + 11);
    scrscrntex[0].v = scrscrntex[2].v = TO_UV_256(200.0f);
    scrscrntex[1].v = scrscrntex[3].v = TO_UV_256(208.0f);
    scrscrntex[0].x = scrscrntex[1].x = x;
    scrscrntex[2].x = scrscrntex[3].x = (11.0f + x);
    scrscrntex[0].y = scrscrntex[2].y = y;
    scrscrntex[1].y = scrscrntex[3].y = (8.0f + y);
}

void SSPutStr_Bigger(const ScStrBig* s, f32 sc, u8 gr, u16 priority) {
    u16 x = s->x;
    u16 y = s->y;
    u8 atr = s->atr;
    const char* str = s->str;

    f32 xx;
    f32 yy;
    u8 i;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);

    for (i = 0; i < 4; i++) {
        scrscrntex[i].col = bigger_col_tbl[gr][i];
    }

    scrscrntex[0].z = scrscrntex[1].z = scrscrntex[2].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(1, atr & 0x3F);
    xx = x;
    yy = y;

    while (*str != '\0') {
        if (*str == '$') {
            str++;
            xx += 4.0f * sc;
            continue;
        }

        SSPutStrTexInputB(xx, yy, str, sc);
        njDrawTexture(scrscrntex, 4, 1, 1);
        xx += 8.0f * sc;
        str++;
    }
}

static void put_dec_high_digits(u16 x, u16 y, const s8* str, u8 size) {
    u8 zero_sw = 0;

    x -= 16;

    if (size == 3 && str[0] != 0) {
        SSPutStrTexInput2(x, y, str[0]);
        njDrawSprite(scrscrntex, 4, 1, 1);
        zero_sw = 1;
    }

    x += 8;

    if (zero_sw == 1) {
        SSPutStrTexInput2(x, y, str[1]);
        njDrawSprite(scrscrntex, 4, 1, 1);
    } else if (size > 1 && str[1] != 0) {
        SSPutStrTexInput2(x, y, str[1]);
        njDrawSprite(scrscrntex, 4, 1, 1);
    }
}

void SSPutDec(const ScDec* d, u8 size) {
    u16 x = d->x;
    u16 y = d->y;
    u8 atr = d->atr;
    u8 dec = d->dec;

    s8 str[3];
    u8 work;
    u8 num;
    u8 i;

    if (No_Trans) {
        return;
    }

    if (size == 0) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = -1;
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[2];
    njSetPaletteBankNumG(1, atr & 0x3F);
    x = x * 8;
    y = y * 8;
    work = 100;

    for (i = 0; i < 3; i++) {
        for (num = 0; dec + 1 > work; dec = dec - work, num++) {}

        str[i] = num;
        work = work / 10;
    }

    SSPutStrTexInput2(x, y, str[2]);
    njDrawSprite(scrscrntex, 4, 1, 1);

    if (size == 0) {
        return;
    }

    put_dec_high_digits(x, y, str, size);
}

void SSPutDec3(const ScDec3* d, u8 size, u8 gr, u16 priority) {
    u16 x = d->x;
    u16 y = d->y;
    u8 atr = d->atr;
    s16 dec = d->dec;

    s8 str[3];
    s16 work;
    u8 num;
    u8 i;
    u8 zero_sw;
    f32 xx;
    f32 yy;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);

    for (i = 0; i < 4; i++) {
        scrscrntex[i].col = bigger_col_tbl[gr][i];
    }

    scrscrntex[0].z = scrscrntex[1].z = scrscrntex[2].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(1, atr & 0x3F);

    xx = x;
    yy = y;
    zero_sw = 0;
    work = 100;

    for (i = 0; i < 3; i++) {
        for (num = 0; dec + 1 > work; dec = dec - work, num++) {}

        str[i] = num;
        work = work / 10;
    }

    SSPutStrTexInputB2(xx, yy, str[2]);
    njDrawTexture(scrscrntex, 4, 4, 1);

    if (size == 0) {
        return;
    }

    xx -= 22.0f;

    if (size == 3 && str[0] != 0) {
        SSPutStrTexInputB2(xx, yy, str[0]);
        njDrawSprite(scrscrntex, 4, 4, 1);
        zero_sw = 1;
    }

    xx += 11.0f;

    if (zero_sw == 1) {
        SSPutStrTexInputB2(xx, yy, str[1]);
        njDrawSprite(scrscrntex, 4, 4, 1);
    } else if (size > 1 && str[1] != 0) {
        SSPutStrTexInputB2(xx, yy, str[1]);
        njDrawSprite(scrscrntex, 4, 4, 1);
    }
}

void scfont_put(const ScFontCell* c, u16 priority) {
    u16 x = c->x;
    u16 y = c->y;
    u8 atr = c->atr;
    u8 page = c->page;
    u8 cx = c->cx;
    u8 cy = c->cy;

    s32 u;
    s32 v;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);
    scrscrntex[0].col = scrscrntex[3].col = -1;
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(page, atr & 0x3F);
    x = x * 8;
    y = y * 8;
    u = cx * 8;
    v = cy * 8;

    if (atr & 0x80) {
        scrscrntex[3].u = TO_UV_256_NEG(u);
        scrscrntex[0].u = TO_UV_256_NEG(u + 8);
    } else {
        scrscrntex[0].u = TO_UV_256(u);
        scrscrntex[3].u = TO_UV_256(u + 8);
    }

    if (atr & 0x40) {
        scrscrntex[3].v = TO_UV_256_NEG(v);
        scrscrntex[0].v = TO_UV_256_NEG(v + 8);
    } else {
        scrscrntex[0].v = TO_UV_256(v);
        scrscrntex[3].v = TO_UV_256(v + 8);
    }

    set_glyph_quad_pos(x, y);
    njDrawSprite(scrscrntex, 4, page, 1);
}

void scfont_put2(const ScFontCell* c) {
    u16 x = c->x;
    u16 y = c->y;
    u8 atr = c->atr;
    u8 page = c->page;
    u8 cx = c->cx;
    u8 cy = c->cy;

    sa_frame[y - 25][x].atr = atr;
    sa_frame[y - 25][x].page = page;
    sa_frame[y - 25][x].cx = cx;
    sa_frame[y - 25][x].cy = cy;
}

void scfont_sqput(const ScFontSquare* c, u16 priority) {
    u16 x = c->x;
    u16 y = c->y;
    u8 atr = c->atr;
    u8 page = c->page;
    u8 cx1 = c->cx1;
    u8 cy1 = c->cy1;
    u8 cx2 = c->cx2;
    u8 cy2 = c->cy2;

    s32 u1;
    s32 u2;
    s32 v1;
    s32 v2;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);

    scrscrntex[0].col = scrscrntex[3].col = -1;
    scrscrntex[0].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(page, atr & 0x3F);
    x = x * 8;
    y = y * 8;
    u1 = cx1 * 8;
    u2 = u1 + (cx2 * 8);
    v1 = cy1 * 8;
    v2 = v1 + (cy2 * 8);

    if (atr & 0x80) {
        scrscrntex[3].u = TO_UV_256_NEG(u1);
        scrscrntex[0].u = TO_UV_256_NEG(u2);
    } else {
        scrscrntex[0].u = TO_UV_256(u1);
        scrscrntex[3].u = TO_UV_256(u2);
    }

    if (atr & 0x40) {
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

void scfont_sqput2(const ScFontSquareInv* c) {
    u16 x = c->x;
    u16 y = c->y;
    u8 atr = c->atr;
    u8 inverse = c->inverse;
    u8 page = c->page;
    u8 cx1 = c->cx1;
    u8 cy1 = c->cy1;
    u8 cx2 = c->cx2;
    u8 cy2 = c->cy2;

    u8 i;
    u8 j;

    if (inverse == 0) {
        for (j = 0; j < cy2; j++) {
            for (i = 0; i < cx2; i++) {
                sa_frame[y - 25 + j][x + i].atr = atr;
                sa_frame[y - 25 + j][x + i].page = page;
                sa_frame[y - 25 + j][x + i].cx = cx1 + i;
                sa_frame[y - 25 + j][x + i].cy = cy1 + j;
            }
        }
    } else {
        for (j = 0; j < cy2; j++) {
            for (i = 0; i < cx2; i++) {
                sa_frame[y - 25 + j][x + i].atr = atr;
                sa_frame[y - 25 + j][x + i].page = page;
                sa_frame[y - 25 + j][x + i].cx = (cx1 + (cx2 - 1)) - i;
                sa_frame[y - 25 + j][x + i].cy = cy1 + j;
            }
        }
    }
}

void scfont_sqput3(const ScFontSquareWide* c, u8 gr, u16 priority) {
    u16 x = c->x;
    u16 y = c->y;
    u8 atr = c->atr;
    u8 page = c->page;
    u16 cx1 = c->cx1;
    u16 cy1 = c->cy1;
    u16 cx2 = c->cx2;
    u16 cy2 = c->cy2;

    s32 u1;
    s32 u2;
    s32 v1;
    s32 v2;
    u8 i;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    njColorBlendingMode(0, 1);

    for (i = 0; i < 4; i++) {
        scrscrntex[i].col = bigger_col_tbl[gr][i];
    }

    scrscrntex[0].z = scrscrntex[1].z = scrscrntex[2].z = scrscrntex[3].z = PrioBase[priority];
    njSetPaletteBankNumG(page, atr & 0x3F);
    u1 = cx1;
    u2 = u1 + cx2;
    v1 = cy1;
    v2 = v1 + cy2;

    scrscrntex[0].u = scrscrntex[1].u = TO_UV_256(u1);
    scrscrntex[2].u = scrscrntex[3].u = TO_UV_256(u2);
    scrscrntex[0].v = scrscrntex[2].v = TO_UV_256(v1);
    scrscrntex[1].v = scrscrntex[3].v = TO_UV_256(v2);
    scrscrntex[0].x = scrscrntex[1].x = x;
    scrscrntex[2].x = scrscrntex[3].x = (x + (u2 - u1));
    scrscrntex[0].y = scrscrntex[2].y = y;
    scrscrntex[1].y = scrscrntex[3].y = (y + (v2 - v1));
    njDrawTexture(scrscrntex, 4, page, 1);
}

void sc_clear(u16 sposx, u16 sposy, u16 eposx, u16 eposy) {
    u16 i;
    u16 j;

    for (j = 0; j < (eposy - sposy) + 1; j++) {
        for (i = 0; i < (eposx - sposx) + 1; i++) {
            sa_frame[sposy - 25 + j][sposx + i].atr = 0;
            sa_frame[sposy - 25 + j][sposx + i].page = 0;
            sa_frame[sposy - 25 + j][sposx + i].cx = 0;
            sa_frame[sposy - 25 + j][sposx + i].cy = 0;
        }
    }
}

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

void sc_ram_to_vram(s8 sc_num) {
    uintptr_t* sc_tbl_ptr;
    u8* sc_pos_ptr;
    u8* sc_uv_ptr;
    u16 loop;
    u16 i;

    sc_tbl_ptr = (uintptr_t*)sc_ram_vram_tbl[sc_num];
    sc_pos_ptr = (u8*)*sc_tbl_ptr;
    sc_tbl_ptr++;
    sc_uv_ptr = (u8*)*sc_tbl_ptr;
    loop = *sc_uv_ptr++;

    for (i = 0; i < loop; i++) {
        sa_frame[sc_pos_ptr[1] - 25][sc_pos_ptr[0]].atr = sa_ram_vram_col[sc_num][0];
        sa_frame[sc_pos_ptr[1] - 25][sc_pos_ptr[0]].page = sa_ram_vram_col[sc_num][1];
        sa_frame[sc_pos_ptr[1] - 25][sc_pos_ptr[0]].cx = *sc_uv_ptr++;
        sa_frame[sc_pos_ptr[1] - 25][sc_pos_ptr[0]].cy = *sc_uv_ptr++;
        sc_pos_ptr += 2;
    }
}

void sc_ram_to_vram_opc(s8 sc_num, s8 x, s8 y, u16 atr) {
    uintptr_t* sc_tbl_ptr;
    u8* sc_pos_ptr;
    u8* sc_uv_ptr;
    u16 loop;
    u16 i;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgScrList);
    sc_tbl_ptr = (uintptr_t*)sc_ram_vram_tbl[sc_num];
    sc_pos_ptr = (u8*)*sc_tbl_ptr;
    sc_tbl_ptr++;
    sc_uv_ptr = (u8*)*sc_tbl_ptr;
    loop = *sc_uv_ptr++;

    for (i = 0; i < loop; i++) {
        scfont_put(
            &(ScFontCell){ sc_pos_ptr[0] + x,
                           sc_pos_ptr[1] + y,
                           atr,
                           sa_ram_vram_col[sc_num][1],
                           sc_uv_ptr[0],
                           sc_uv_ptr[1] },
            3
        );
        sc_uv_ptr += 2;
        sc_pos_ptr += 2;
    }
}

void sq_paint_chenge(const SqPaint* q, u16 atr) {
    u16 x = q->x;
    u16 y = q->y;
    u16 sx = q->sx;
    u16 sy = q->sy;

    u16 i;
    u16 j;

    for (j = 0; j < sy; j++) {
        for (i = 0; i < sx; i++) {
            sa_frame[y - 25 + j][x + i].atr = atr;
        }
    }
}

const u8 scrnAddTex1UV[9][4] = { { 96, 0, 32, 32 },  { 63, 0, 32, 32 },  { 0, 96, 32, 32 },
                                 { 0, 64, 32, 32 },  { 0, 0, 32, 32 },   { 31, 0, 32, 32 },
                                 { 32, 96, 32, 32 }, { 32, 64, 32, 32 }, { 128, 0, 96, 128 } };

static void set_ps2_button_texture(Sprite* sprite, ButtonIcon icon) {
    sprite->tex_code = ppgGetUsingTextureHandle(&ppgScrTex, 5) | (ppgGetUsingPaletteHandle(&ppgScrPalShot, 0) << 16);
    sprite->t[0].s = scrnAddTex1UV[icon][0] / 256.0f;
    sprite->t[3].s = (scrnAddTex1UV[icon][0] + scrnAddTex1UV[icon][2]) / 256.0f;
    sprite->t[0].t = scrnAddTex1UV[icon][1] / 128.0f;
    sprite->t[3].t = (scrnAddTex1UV[icon][1] + scrnAddTex1UV[icon][3]) / 128.0f;
}

static void _dispButtonImage(const ButtonImage* b, bool invert_y) {
    PAL_CURSOR_COL oricol;
    Sprite prm;

    if (No_Trans) {
        return;
    }

    oricol.color = 0xFFFFFFFF;
    oricol.argb.a = (0xFF - b->cl);
    prm.v[0].x = b->px;
    prm.v[0].y = b->py;
    prm.v[3].x = b->px + b->sx;

    if (invert_y) {
        prm.v[3].y = b->py - b->sy;
        njCalcPoint(NULL, &prm.v[0], &prm.v[0]);
        njCalcPoint(NULL, &prm.v[3], &prm.v[3]);
    } else {
        prm.v[3].y = b->py + b->sy;
    }

    prm.v[0].z = prm.v[3].z = PrioBase[b->pz];

#if PSP
    set_ps2_button_texture(&prm, b->icon);
#else
    const Input_PadType pad_type = Input_GetPadType(b->player_id);

    switch (pad_type) {
    case INPUT_PAD_TYPE_PLAYSTATION:
        set_ps2_button_texture(&prm, b->icon);
        break;

    case INPUT_PAD_TYPE_UNKNOWN:
    case INPUT_PAD_TYPE_KEYBOARD:
    case INPUT_PAD_TYPE_XBOX:
        XboxButtons_SetTextureParams(&prm, b->icon);
        break;
    }
#endif

    flSetRenderState(FLRENDER_TEXSTAGE0, prm.tex_code);
    Renderer_DrawSprite(&prm, oricol.color);
}

void dispButtonImage(const ButtonImage* b) {
    _dispButtonImage(b, true);
}

void dispButtonImage2(const ButtonImage* b) {
    _dispButtonImage(b, false);
}
