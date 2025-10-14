#include "sf33rd/Source/PS2/ps2Quad.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "structs.h"

#include "port/sdl/sdl_game_renderer.h"

typedef struct {
    Vec4 vec;
    u32 c;
    TexCoord t;
} VecUnk;

typedef struct {
    u32 s;
    u32 t;
} _TexCoord;

typedef struct {
    Vec3 v[4];
    _TexCoord t[4];
    u32 texCode;
} _Sprite;

typedef struct {
    Vec3 v[2];
    _TexCoord t[2];
    u32 vtxColor;
    u32 texCode;
    u32 id;
} _Sprite2;

typedef struct {
    Vec3 vec3;
    f32 w;
} _Vec4;

typedef struct {
    _Vec4 vec;
    u32 c;
    TexCoord t;
} _VecUnk;

static s32 FilterMode;

void ps2QuadTexture(VecUnk* ptr, u32 num);
void ps2QuadSolid(VecUnk* ptr, u32 num);

void ps2SeqsRenderQuad_Ax(Sprite2* spr) {
    SDLGameRenderer_DrawSprite2(spr);
}

void ps2SeqsRenderQuad_A2(Sprite* spr, u32 col) {
    SDLGameRenderer_DrawSprite(spr, col);
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
    SDLGameRenderer_DrawTexturedQuad(ptr);
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
    SDLGameRenderer_DrawSolidQuad(ptr);
}
