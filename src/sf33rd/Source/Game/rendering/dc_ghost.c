/**
 * @file dc_ghost.c
 * Compatibility Layer for Sega Dreamcast's Ninja SDK
 */

#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/PS2/ps2Quad.h"
#include "structs.h"

#include <string.h>

#define NTH_BYTE(value, n) ((((value >> n * 8) & 0xFF) << n * 8))

typedef struct {
    Vertex v;
    u32 col;
} _Polygon;

// `col` needs to be `uintptr_t` because it sometimes stores a pointer to `WORK`
typedef struct {
    Vec3 v[4];
    uintptr_t col;
    u32 type;
    s32 next;
} NJDP2D_PRIM;

typedef struct {
    s16 ix1st;
    s16 total;
    NJDP2D_PRIM prim[100];
} NJDP2D_W;

NJDP2D_W njdp2d_w;
MTX cmtx;

static void matmul(MTX* dst, const MTX* a, const MTX* b) {
    MTX result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.a[i][j] =
                a->a[i][0] * b->a[0][j] + a->a[i][1] * b->a[1][j] + a->a[i][2] * b->a[2][j] + a->a[i][3] * b->a[3][j];
        }
    }

    memcpy(dst, &result, sizeof(MTX));
}

void njUnitMatrix(MTX* mtx) {
    if (mtx == NULL) {
        mtx = &cmtx;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mtx->a[i][j] = (i == j);
        }
    }
}

void njGetMatrix(MTX* m) {
    *m = cmtx;
}

void njSetMatrix(MTX* md, MTX* ms) {
    if (md == NULL) {
        md = &cmtx;
    }

    *md = *ms;
}

void njScale(MTX* mtx, f32 x, f32 y, f32 z) {
    if (mtx == NULL) {
        mtx = &cmtx;
    }

    for (int i = 0; i < 4; i++) {
        mtx->a[0][i] *= x;
        mtx->a[1][i] *= y;
        mtx->a[2][i] *= z;
    }
}

void njTranslate(MTX* mtx, f32 x, f32 y, f32 z) {
    if (mtx == NULL) {
        mtx = &cmtx;
    }

    MTX translation_matrix;

    njUnitMatrix(&translation_matrix);
    translation_matrix.a[3][0] = x;
    translation_matrix.a[3][1] = y;
    translation_matrix.a[3][2] = z;

    matmul(mtx, &translation_matrix, mtx);
}

void njSetBackColor(u32 c0, u32 c1, u32 c2) {
    c0 = c0 | c1 | c2;
    flSetRenderState(FLRENDER_BACKCOLOR, NTH_BYTE(c0, 3) | NTH_BYTE(c0, 2) | NTH_BYTE(c0, 1) | NTH_BYTE(c0, 0));
}

void njColorBlendingMode(s32 target, s32 mode) {
    flSetRenderState(FLRENDER_ALPHABLENDMODE, 0x32);
}

void njCalcPoint(MTX* mtx, Vec3* ps, Vec3* pd) {
    if (mtx == NULL) {
        mtx = &cmtx;
    }

    const f32 x = ps->x;
    const f32 y = ps->y;
    const f32 z = ps->z;
    const f32 w = 1.0f;

    pd->x = x * mtx->a[0][0] + y * mtx->a[1][0] + z * mtx->a[2][0] + w * mtx->a[3][0];
    pd->y = x * mtx->a[0][1] + y * mtx->a[1][1] + z * mtx->a[2][1] + w * mtx->a[3][1];
    pd->z = x * mtx->a[0][2] + y * mtx->a[1][2] + z * mtx->a[2][2] + w * mtx->a[3][2];
}

void njCalcPoints(MTX* mtx, Vec3* ps, Vec3* pd, s32 num) {
    s32 i;

    if (mtx == NULL) {
        mtx = &cmtx;
    }

    for (i = 0; i < num; i++) {
        njCalcPoint(mtx, ps++, pd++);
    }
}

void njDrawTexture(Polygon* polygon, s32 /* unused */, s32 tex, s32 /* unused */) {
    Vertex vtx[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        vtx[i] = ((_Polygon*)polygon)[i].v;
    }

    ppgWriteQuadWithST_B(vtx, polygon[0].col, NULL, tex, -1);
}

void njDrawSprite(Polygon* polygon, s32 /* unused */, s32 tex, s32 /* unused */) {
    Vertex vtx[4];

    if ((polygon[0].x >= 384.0f) || (polygon[3].x < 0.0f) || (polygon[0].y >= 224.0f) || (polygon[3].y < 0.0f)) {
        return;
    }

    vtx[0] = ((_Polygon*)polygon)[0].v;
    vtx[3] = ((_Polygon*)polygon)[3].v;

    ppgWriteQuadWithST_B2(vtx, polygon[0].col, 0, tex, -1);
}

void njdp2d_init() {
    njdp2d_w.ix1st = -1;
    njdp2d_w.total = 0;
}

void njdp2d_draw() {
    Quad prm;
    s32 i;

    for (i = njdp2d_w.ix1st; i != -1; i = njdp2d_w.prim[i].next) {
        switch (njdp2d_w.prim[i].type) {
        case 0:
            prm.v[0] = njdp2d_w.prim[i].v[0];
            prm.v[1] = njdp2d_w.prim[i].v[1];
            prm.v[2] = njdp2d_w.prim[i].v[2];
            prm.v[3] = njdp2d_w.prim[i].v[3];

            ps2SeqsRenderQuad_B(&prm, njdp2d_w.prim[i].col);
            break;

        case 1:
            shadow_drawing((WORK*)njdp2d_w.prim[i].col, njdp2d_w.prim[i].v[0].y);
            break;
        }
    }

    njdp2d_init();
}

// `col` needs to be `uintptr_t` because it sometimes stores a pointer to `WORK`
void njdp2d_sort(f32* pos, f32 pri, uintptr_t col, s32 flag) {
    s32 i;
    s32 ix = njdp2d_w.total;
    s32 prev;

    if (ix >= 100) {
        // The 2D polygon display request has exceeded the buffer\n
        flLogOut("２Ｄポリゴンの表示要求がバッファをオーバーしました\n");
        return;
    }

    if (flag == 0) {
        njdp2d_w.prim[ix].v[0].z = njdp2d_w.prim[ix].v[1].z = njdp2d_w.prim[ix].v[2].z = njdp2d_w.prim[ix].v[3].z = pri;
        njdp2d_w.prim[ix].v[0].x = pos[0];
        njdp2d_w.prim[ix].v[0].y = pos[1];
        njdp2d_w.prim[ix].v[1].x = pos[2];
        njdp2d_w.prim[ix].v[1].y = pos[3];
        njdp2d_w.prim[ix].v[2].x = pos[4];
        njdp2d_w.prim[ix].v[2].y = pos[5];
        njdp2d_w.prim[ix].v[3].x = pos[6];
        njdp2d_w.prim[ix].v[3].y = pos[7];
        njdp2d_w.prim[ix].type = 0;
        njdp2d_w.prim[ix].col = col;
    }

    if (flag == 1) {
        njdp2d_w.prim[ix].v[0].z = pri;
        njdp2d_w.prim[ix].v[0].y = pos[0];
        njdp2d_w.prim[ix].type = 1;
        njdp2d_w.prim[ix].col = col;
    }

    njdp2d_w.prim[ix].next = -1;

    if (njdp2d_w.ix1st == -1) {
        njdp2d_w.ix1st = njdp2d_w.total;
    } else {
        i = njdp2d_w.ix1st;
        prev = -1;

        while (1) {
            if (pri > njdp2d_w.prim[i].v[0].z) {
                if (prev == -1) {
                    njdp2d_w.ix1st = ix;
                    njdp2d_w.prim[ix].next = i;
                } else {
                    njdp2d_w.prim[prev].next = ix;
                    njdp2d_w.prim[ix].next = i;
                }

                break;
            }

            if (njdp2d_w.prim[i].next == -1) {
                njdp2d_w.prim[i].next = ix;
                break;
            }

            prev = i;
            i = njdp2d_w.prim[i].next;
        }
    }

    njdp2d_w.total += 1;
}

void njDrawPolygon2D(PAL_CURSOR* p, s32 /* unused */, f32 pri, u32 attr) {
    if (attr & 0x20) {
        njdp2d_sort((f32*)p->p, pri, p->col->color, 0);
    }
}

void njSetPaletteBankNumG(u32 globalIndex, s32 bank) {
    ppgSetupCurrentPaletteNumber(0, bank);
}

void njSetPaletteData(s32 offset, s32 count, void* data) {
    palCopyGhostDC(offset, count, data);
    palUpdateGhostDC();
}

s32 njReLoadTexturePartNumG(u32 gix, s8* srcAdrs, u32 ofs, u32 size) {
    ppgRenewDotDataSeqs(0, gix, (u32*)srcAdrs, ofs, size);
    return 1;
}
