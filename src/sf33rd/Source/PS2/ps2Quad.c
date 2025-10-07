#include "sf33rd/Source/PS2/ps2Quad.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2dma.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "structs.h"

#if !defined(TARGET_PS2)
#include "port/sdl/sdl_game_renderer.h"
#endif

#include <eestruct.h>
#include <libgraph.h>

typedef struct {
    // total size: 0x1C
    Vec4 vec;   // offset 0x0, size 0x10
    u32 c;      // offset 0x10, size 0x4
    TexCoord t; // offset 0x14, size 0x8
} VecUnk;

typedef struct {
    // total size: 0x8
    u32 s; // offset 0x0, size 0x4
    u32 t; // offset 0x4, size 0x4
} _TexCoord;

typedef struct {
    // total size: 0x54
    Vec3 v[4];      // offset 0x0, size 0x30
    _TexCoord t[4]; // offset 0x30, size 0x20
    u32 texCode;    // offset 0x50, size 0x4
} _Sprite;

typedef struct {
    // total size: 0x34
    Vec3 v[2];      // offset 0x0, size 0x18
    _TexCoord t[2]; // offset 0x18, size 0x10
    u32 vtxColor;   // offset 0x28, size 0x4
    u32 texCode;    // offset 0x2C, size 0x4
    u32 id;         // offset 0x30, size 0x4
} _Sprite2;

typedef struct {
    Vec3 vec3;
    f32 w;
} _Vec4;

typedef struct {
    // total size: 0x1C
    _Vec4 vec;  // offset 0x0, size 0x10
    u32 c;      // offset 0x10, size 0x4
    TexCoord t; // offset 0x14, size 0x8
} _VecUnk;

static s32 FilterMode;
static s32 CP3toPS2DrawFlag;

void ps2QuadTexture(VecUnk* ptr, u32 num);
void ps2QuadSolid(VecUnk* ptr, u32 num);

void ps2SeqsRenderQuadInit_A() {
    // Do nothing
}

void ps2SeqsRenderQuadInit_B() {
    // Do nothing
}

void ps2SeqsRenderQuad_Ax(Sprite2* spr) {
#if !defined(TARGET_PS2)
    SDLGameRenderer_DrawSprite2(spr);
#else
    u32 data_ptr;
    u32 col;
    u64 rgbaq;
    u64* p;
    s32 x;
    s32 y;
    s32 z;
    u8 cA;
    u8 cR;
    u8 cG;
    u8 cB;

    data_ptr = flPS2GetSystemTmpBuff(80, 16);
    p = (u64*)data_ptr;

    *p++ = 0x80000000 | DMAend | 4;
    *((u32*)p)++ = SCE_VIF1_SET_NOP(0);
    *((u32*)p)++ = SCE_VIF1_SET_DIRECTHL(4, 0);

    *p++ = SCE_GIF_SET_TAG(1, 1, 0, 0, SCE_GIF_REGLIST, 6);
    *p++ = SCE_GS_PRIM | SCE_GS_RGBAQ << 4 | SCE_GS_ST << 8 | SCE_GS_XYZ2 << 12 | SCE_GS_ST << 16 | SCE_GS_XYZ2 << 20;

    col = flPS2ConvColor(spr->vtxColor, 0);
    cA = (col >> 24) & 0xFF;
    cR = (col >> 16) & 0xFF;
    cG = (col >> 8) & 0xFF;
    cB = col & 0xFF;
    rgbaq = SCE_GS_SET_RGBAQ(cR, cG, cB, cA, 0x3F800000);
    z = flPS2ConvScreenFZ(spr->v[0].z);

    *p++ = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE,
                           /* Gourand */ 0,
                           /* Textured */ 1,
                           /* fog */ 0,
                           /* Alpha blend */ 1,
                           /* AA */ 0,
                           /* UV */ 0,
                           /* ctx2 */ 0,
                           /* frag */ 0);
    *p++ = rgbaq;

    *p++ = SCE_GS_SET_ST(((_Sprite2*)spr)->t[0].s, ((_Sprite2*)spr)->t[0].t);

    x = spr->v[0].x + 0.5f;
    y = spr->v[0].y + 0.5f;
    *p++ = SCE_GS_SET_XYZ((flPs2State.D2dOffsetX + x) << 4, (flPs2State.D2dOffsetY + y) << 4, (u32)z);

    *p++ = SCE_GS_SET_ST(((_Sprite2*)spr)->t[1].s, ((_Sprite2*)spr)->t[1].t);

    x = spr->v[1].x + 0.5f;
    y = spr->v[1].y + 0.5f;
    *p++ = SCE_GS_SET_XYZ((flPs2State.D2dOffsetX + x) << 4, (flPs2State.D2dOffsetY + y) << 4, (u32)z);

    flPS2DmaAddQueue2(0, DMArefs | (data_ptr & 0xFFFFFFF), data_ptr, &flPs2VIF1Control);
#endif
}

void ps2SeqsRenderQuad_A2(Sprite* spr, u32 col) {
#if !defined(TARGET_PS2)
    SDLGameRenderer_DrawSprite(spr, col);
#else
    u32 data_ptr;
    u64 rgbaq;
    u64* p;
    s32 x;
    s32 y;
    s32 z;
    u8 cA;
    u8 cR;
    u8 cG;
    u8 cB;

    data_ptr = flPS2GetSystemTmpBuff(0x50, 0x10);
    p = (u64*)data_ptr;

    *p++ = 0x80000000 | DMAend | 4;
    *((u32*)p)++ = SCE_VIF1_SET_NOP(0);
    *((u32*)p)++ = SCE_VIF1_SET_DIRECTHL(4, 0);

    *p++ = SCE_GIF_SET_TAG(1, SCE_GS_TRUE, SCE_GS_FALSE, 0, SCE_GIF_REGLIST, 6);
    *p++ = SCE_GS_PRIM | SCE_GS_RGBAQ << 4 | SCE_GS_ST << 8 | SCE_GS_XYZ2 << 12 | SCE_GS_ST << 16 | SCE_GS_XYZ2 << 20;

    col = flPS2ConvColor(col, 0);
    cA = (col >> 24) & 0xFF;
    cR = (col >> 16) & 0xFF;
    cG = (col >> 8) & 0xFF;
    cB = col & 0xFF;
    rgbaq = SCE_GS_SET_RGBAQ(cR, cG, cB, cA, 0x3F800000);
    z = flPS2ConvScreenFZ(spr->v[0].z);

    *p++ = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE,
                           /* Gourand */ 0,
                           /* Textured */ 1,
                           /* fog */ 0,
                           /* Alpha blend */ 1,
                           /* AA */ 0,
                           /* UV */ 0,
                           /* ctx2 */ 0,
                           /* frag */ 0);

    *p++ = rgbaq;

    *p++ = SCE_GS_SET_ST(((_Sprite*)spr)->t[0].s, ((_Sprite*)spr)->t[0].t);

    x = spr->v[0].x + 0.5f;
    y = spr->v[0].y + 0.5f;
    *p++ = SCE_GS_SET_XYZ((flPs2State.D2dOffsetX + x) << 4, (flPs2State.D2dOffsetY + y) << 4, (u32)z);

    *p++ = SCE_GS_SET_ST(((_Sprite*)spr)->t[3].s, ((_Sprite*)spr)->t[3].t);

    x = spr->v[3].x + 0.5f;
    y = spr->v[3].y + 0.5f;
    *p++ = SCE_GS_SET_XYZ((flPs2State.D2dOffsetX + x) << 4, (flPs2State.D2dOffsetY + y) << 4, (u32)z);

    flPS2DmaAddQueue2(0, DMArefs | (data_ptr & 0xFFFFFFF), data_ptr, &flPs2VIF1Control);
#endif
}

void ps2SeqsRenderQuad_A(Sprite* spr, u32 col) {
    VecUnk vptr[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        ((_VecUnk*)vptr)[i].vec.vec3 = spr->v[i];
        vptr[i].vec.w = 1.0f;
        vptr[i].c = col;
        vptr[i].t = spr->t[i];
    }

    ps2QuadTexture(vptr, 4);
}

void ps2QuadTexture(VecUnk* ptr, u32 num) {
#if !defined(TARGET_PS2)
    SDLGameRenderer_DrawTexturedQuad(ptr);
    return;
#endif

    u32 qwc;
    u32 work;
    uintptr_t data_ptr;
    QWORD* dma_data;
    u64* vtx_data;
    s32 x;
    s32 y;
    s32 z;
    u32 col;
    u8 cA;
    u8 cR;
    u8 cG;
    u8 cB;

    qwc = 4;
    work = num * 3;
    work = (work + 1) >> 1;
    qwc += work;
    data_ptr = flPS2GetSystemTmpBuff(qwc * 16, 16);
    dma_data = (QWORD*)data_ptr;

    dma_data->UI64[0] = qwc + 0xEFFFFFFF;
    dma_data->UI32[2] = 0x13000000;
    dma_data->UI32[3] = (qwc - 1) | 0x51000000;

    dma_data++;
    dma_data->UI64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, SCE_GIF_PACKED, 1);
    dma_data->UI64[1] = SCE_GIF_PACKED_AD;

    dma_data++;
    dma_data->UI64[0] = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP,
                                        /* Gourand */ 0,
                                        /* Textured */ 1,
                                        /* fog */ 0,
                                        /* Alpha blend */ 1,
                                        /* AA */ 0,
                                        /* UV */ 0,
                                        /* ctx2 */ 0,
                                        /* frag */ 0);
    dma_data->UI64[1] = SCE_GS_PRIM;

    dma_data++;
    dma_data->UI64[0] = SCE_GIF_SET_TAG(num, 1, 0, 0, SCE_GIF_REGLIST, 3);
    dma_data->UI64[1] = SCE_GS_RGBAQ | SCE_GS_ST << 4 | SCE_GS_XYZ2 << 8;

    dma_data++;
    vtx_data = (u64*)dma_data;
    col = flPS2ConvColor(ptr->c, 0);
    cA = (col >> 24) & 0xFF;
    cR = (col >> 16) & 0xFF;
    cG = (col >> 8) & 0xFF;
    cB = col & 0xFF;
    z = flPS2ConvScreenFZ(ptr->vec.z);

    do {
        *vtx_data++ = SCE_GS_SET_RGBAQ(cR, cG, cB, cA, REINTERPRET_AS_U32(ptr->vec.w));
        *vtx_data++ = SCE_GS_SET_ST(REINTERPRET_AS_U32(ptr->t.s), REINTERPRET_AS_U32(ptr->t.t));

        x = ptr->vec.x + 0.5f;
        y = ptr->vec.y + 0.5f;
        *vtx_data++ = SCE_GS_SET_XYZ((flPs2State.D2dOffsetX + x) << 4, (flPs2State.D2dOffsetY + y) << 4, z);

        ptr++;
    } while (--num);

    flPS2DmaAddQueue2(0, (data_ptr & 0xFFFFFFF) | 0x40000000, data_ptr, &flPs2VIF1Control);
}

void ps2SeqsRenderQuad_B(Quad* spr, u32 col) {
    VecUnk vptr[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        ((_VecUnk*)vptr)[i].vec.vec3 = spr->v[i];
        vptr[i].vec.w = 1.0f;
        vptr[i].c = col;
    }

    ps2QuadSolid(vptr, 4);
}

void ps2QuadSolid(VecUnk* ptr, u32 num) {
#if !defined(TARGET_PS2)
    SDLGameRenderer_DrawSolidQuad(ptr);
#else
    u32 qwc;
    u32 work;
    u32 data_ptr;
    QWORD* dma_data;
    u64* vtx_data;
    f32 x;
    f32 y;
    f32 z;
    u32 col;
    u8 cA;
    u8 cR;
    u8 cG;
    u8 cB;

    qwc = 4;
    work = num << 1;
    work = (work + 1) >> 1;
    qwc += work;
    data_ptr = flPS2GetSystemTmpBuff(qwc * 0x10, 0x10);
    dma_data = (QWORD*)data_ptr;

    dma_data->UI64[0] = qwc + 0xEFFFFFFF;
    dma_data->UI32[2] = 0x13000000;
    dma_data->UI32[3] = (qwc - 1) | 0x51000000;

    dma_data++;
    dma_data->UI64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, SCE_GIF_PACKED, 1);
    dma_data->UI64[1] = SCE_GIF_PACKED_AD;

    dma_data++;
    dma_data->UI64[0] = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP,
                                        /* Gourand */ 0,
                                        /* Textured */ 0,
                                        /* fog */ 0,
                                        /* Alpha blend */ 1,
                                        /* AA */ 0,
                                        /* UV */ 0,
                                        /* ctx2 */ 0,
                                        /* frag */ 0);
    dma_data->UI64[1] = SCE_GS_PRIM;

    dma_data++;
    dma_data->UI64[0] = SCE_GIF_SET_TAG(num, 1, 0, 0, SCE_GIF_REGLIST, 2);
    dma_data->UI64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4;

    dma_data++;
    vtx_data = (u64*)dma_data;
    col = flPS2ConvColor(ptr->c, 1);
    cA = (col >> 24) & 0xFF;
    cR = (col >> 16) & 0xFF;
    cG = (col >> 8) & 0xFF;
    cB = col & 0xFF;

    do {
        *vtx_data++ = SCE_GS_SET_RGBAQ(cR, cG, cB, cA, REINTERPRET_AS_U32(ptr->vec.w));

        x = flPS2ConvScreenFX(ptr->vec.x);
        y = flPS2ConvScreenFY(ptr->vec.y);
        z = flPS2ConvScreenFZ(ptr->vec.z);
        *vtx_data++ =
            SCE_GS_SET_XYZ((s32)(flPs2State.D2dOffsetX + x) << 4, (s32)(flPs2State.D2dOffsetY + y) << 4, (u32)z);

        ptr++;
    } while (--num);

    flPS2DmaAddQueue2(0, (data_ptr & 0xFFFFFFF) | 0x40000000, data_ptr, &flPs2VIF1Control);
#endif
}

void ps2SeqsRenderQuadEnd() {
    // Do nothing
}

s32 getCP3toFullScreenDrawFlag() {
    return CP3toPS2DrawFlag;
}

void CP3toPS2DrawOn() {
    CP3toPS2DrawFlag = 1;
}

void CP3toPS2DrawOff() {
    CP3toPS2DrawFlag = 0;
}

void CP3toPS2Draw() {
#if defined(TARGET_PS2)
    s32 ofx;
    s32 ofy;
    s32 m;
    s32 xs;
    s32 ys;
    s32 tw;
    s32 th;
    s32 fw;
    s32 fh;
    s32 bw;
    s32 bh;
    s32 ox;
    s32 oy;
    s32 ew;
    s32 eh;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 sbp;
    s32 sbw;
    s32 psm;
    s32 dbp;
    s32 dbw;
    u32 top;
    u32 qwc;
    u32* p;

    FilterMode = sys_w.screen_mode;

    if (CP3toPS2DrawFlag == 0) {
        return;
    }

    bw = 384;
    bh = 224;
    fw = flPs2State.DispWidth;
    fh = flPs2State.DispHeight;
    m = flPs2State.Dbi + flPS2FlipCancelFlag;
    sbp = (m & 1) ? flPs2State.FrameBuffAdrs1 : flPs2State.FrameBuffAdrs0;
    sbw = fw / 64;
    psm = flPs2State.FrameBuffForm;
    dbp = flPs2State.ZBuffAdrs;
    dbw = bw / 64;

    qwc = 20;
    top = flPS2GetSystemTmpBuff((qwc + 1) * 16, 16);
    p = (u32*)top;

    *p++ = qwc | 0x70000000 | 0x80000000;
    *p++ = 0;
    *p++ = 0;
    *p++ = qwc | 0x51000000;

    *((u64*)p)++ = SCE_GIF_SET_TAG(6, 0, 0, 0, SCE_GIF_PACKED, 1);
    *((u64*)p)++ = SCE_GIF_PACKED_AD;
    *((u64*)p)++ = 0;
    *((u64*)p)++ = SCE_GS_TEXFLUSH;
    *((u64*)p)++ = SCE_GS_SET_BITBLTBUF(sbp, sbw, psm, dbp, dbw, psm);
    *((u64*)p)++ = SCE_GS_BITBLTBUF;
    *((u64*)p)++ = SCE_GS_SET_TRXPOS(0, 0, 0, 0, 0);
    *((u64*)p)++ = SCE_GS_TRXPOS;
    *((u64*)p)++ = SCE_GS_SET_TRXREG(bw, bh);
    *((u64*)p)++ = SCE_GS_TRXREG;
    *((u64*)p)++ = SCE_GS_SET_TRXDIR(2);
    *((u64*)p)++ = SCE_GS_TRXDIR;
    *((u64*)p)++ = SCE_GS_SET_TEST_1(1, 0, 0, 1, 0, 0, 1, 1);
    *((u64*)p)++ = SCE_GS_TEST_1;

    ofx = ((4096 - fw) / 2) << 4;
    ofy = ((4096 - fh) / 2) << 4;
    x0 = ofx;
    y0 = ofy;
    x1 = ofx + (fw << 4);
    y1 = ofy + (fh << 4);

    *((u64*)p)++ = SCE_GIF_SET_TAG(1, 0, 0, 0, SCE_GIF_REGLIST, 4);
    *((u64*)p)++ = SCE_GS_PRIM | SCE_GS_RGBAQ << 4 | SCE_GS_XYZ2 << 8 | SCE_GS_XYZ2 << 12;

    *((u64*)p)++ = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE,
                                   /* Gourand */ 0,
                                   /* Textured */ 0,
                                   /* fog */ 0,
                                   /* Alpha blend */ 0,
                                   /* AA */ 0,
                                   /* UV */ 0,
                                   /* ctx2 */ 0,
                                   /* frag */ 0);

    *((u64*)p)++ = SCE_GS_SET_RGBAQ(0, 0, 0, 0, 0);
    *((u64*)p)++ = SCE_GS_SET_XYZ(x0, y0, 0);
    *((u64*)p)++ = SCE_GS_SET_XYZ(x1, y1, 0);

    ew = fw * Disp_Size_H / 100;
    eh = fh * Disp_Size_V / 100;
    ox = (fw - ew) / 2;
    oy = (fh - eh) / 2;
    tw = 9;
    th = 8;
    xs = bw;
    ys = bh;
    x0 = ofx + ox * 16;
    y0 = ofy + oy * 16;
    x1 = ofx + (ox + xs * ew / xs) * 16;
    y1 = ofy + (oy + ys * eh / ys) * 16;
    u0 = 8;
    v0 = 8;
    u1 = (xs * 16) + 8;
    v1 = (ys * 16) + 8;
    m = FilterMode;

    *((u64*)p)++ = SCE_GIF_SET_TAG(5, 0, 0, 0, SCE_GIF_PACKED, 1);
    *((u64*)p)++ = SCE_GIF_PACKED_AD;
    *((u64*)p)++ = 0;
    *((u64*)p)++ = SCE_GS_TEXFLUSH;
    *((u64*)p)++ = SCE_GS_SET_TEX0_1(dbp, dbw, psm, tw, th, 0, 0, 0, 0, 0, 0, 0);
    *((u64*)p)++ = SCE_GS_TEX0_1;
    *((u64*)p)++ = SCE_GS_SET_TEX1_1(0, 0, m, m, 0, 0, 0);
    *((u64*)p)++ = SCE_GS_TEX1_1;
    *((u64*)p)++ = SCE_GS_SET_TEXA(0x80, 0, 0x80);
    *((u64*)p)++ = SCE_GS_TEXA;
    *((u64*)p)++ = SCE_GS_SET_CLAMP_1(10, 0, 0, bw - 1, 0, bh - 1);
    *((u64*)p)++ = SCE_GS_CLAMP_1;

    *((u64*)p)++ = SCE_GIF_SET_TAG(1, 1, 0, 0, SCE_GIF_REGLIST, 6);
    *((u64*)p)++ =
        SCE_GS_PRIM | SCE_GS_RGBAQ << 4 | SCE_GS_UV << 8 | SCE_GS_XYZ2 << 12 | SCE_GS_UV << 16 | SCE_GS_XYZ2 << 20;

    *((u64*)p)++ = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE,
                                   /* Gourand */ 0,
                                   /* Textured */ 1,
                                   /* fog */ 0,
                                   /* Alpha blend */ 1,
                                   /* AA */ 0,
                                   /* UV */ 1,
                                   /* ctx2 */ 0,
                                   /* frag */ 0);

    *((u64*)p)++ = SCE_GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0);
    *((u64*)p)++ = SCE_GS_SET_UV(u0, v0);
    *((u64*)p)++ = SCE_GS_SET_XYZ2(x0, y0, 0);
    *((u64*)p)++ = SCE_GS_SET_UV(u1, v1);
    *((u64*)p)++ = SCE_GS_SET_XYZ2(x1, y1, 0);

    flPS2DmaAddQueue2(0, (top & 0xFFFFFFF) | 0x40000000, top, &flPs2VIF1Control);
#endif
}
