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

}
