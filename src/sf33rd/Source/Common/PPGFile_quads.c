/**
 * @file PPGFile_quads.c
 * Writing a textured quad into the display list.
 */

#include "sf33rd/Source/Common/PPGFile.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Compress/Lz77/Lz77Dec.h"
#include "sf33rd/Source/Compress/zlibApp.h"
#include "structs.h"
#include "core/renderer.h"
#include <SDL3/SDL.h>
#include "sf33rd/Source/Common/PPGFile_internal.h"


/* The quad is entirely off the 384x224 screen. Written as the original wrote
 * it - the four bounds ORed together, not the negation of the test below. */
static s32 quad_is_offscreen(const Vertex* v) {
    return (v[0].x >= 384.0f) || (v[3].x < 0.0f) || (v[0].y >= 224.0f) || (v[3].y < 0.0f);
}

/* One transparent-run sub-quad is on screen. Also as written: the four bounds
 * ANDed, which the caller negates. */
static s32 quad_is_onscreen(const Vertex* v) {
    return ((v[0].x < 384.0f) && (v[3].x >= 0.0f) && (v[0].y < 224.0f) && (v[3].y >= 0.0f));
}

/* The transparent-run path: a PPG whose header lists sub-quads draws each of
 * them instead of one quad. The block is ppgWriteQuadUseTrans's own, lifted
 * whole; it always answers 1, which is what the `return 1` at its end was. */
static s32 write_transparent_runs(Vertex* pos, const PPGQuadTransArgs* a, const PPGTransRun* run) {
    Vertex qvtx[4];
    s32 i;
    u32 sx;
    u32 sy;
    u32 ppgw;
    u16 palhan;
    u8* tran;
    u8 cofsXY;
    u8 xs;
    u8 ys;
    u16 iPoint;
    f32 pxs;
    f32 pys;
    f32 sadd;
    f32 tadd;
    f32 ppgwf;
    f32 ppghf;

    tran = (u8*)&run->ppg[1];
    ppgwf = run->ppg->width;
    ppgw = run->ppg->width;
    ppghf = run->ppg->height;
    pxs = pos[3].x - pos[0].x;
    pys = pos[3].y - pos[0].y;
    sadd = 0.5f / pxs;
    tadd = 0.5f / pys;

    if (sadd >= (1.0f / (16.0f * ppgwf))) {
        sadd = 1.0f / (16.0f * ppgwf);
    }

    if (tadd >= (1.0f / (16.0f * ppghf))) {
        tadd = 1.0f / (16.0f * ppghf);
    }

    sadd = 0;
    tadd = 0;

    qvtx[0].z = pos[0].z;
    qvtx[3].z = pos[3].z;

    for (i = 0; i < run->transTotal; i++) {
        if (run->ix_ofs & 0x4000) {
            palhan = run->phan[*tran + a->pal];
        }

        tran++;
        iPoint = *tran++;
        cofsXY = *tran++;
        xs = (cofsXY >> 4) + 1;
        ys = (cofsXY & 0xF) + 1;
        sx = iPoint % ppgw;
        sy = iPoint / ppgw;

        if (a->flip & 1) {
            qvtx[3].x = pos->x + (pxs * (ppgw - sx) / ppgwf);
            qvtx[0].x = pos->x + (pxs * (ppgw - (sx + xs)) / ppgwf);
        } else {
            qvtx[0].x = pos->x + (sx * pxs / ppgwf);
            qvtx[3].x = pos->x + (pxs * (sx + xs) / ppgwf);
        }

        if (a->flip & 2) {
            qvtx[3].y = pos->y + (pys * (ppgw - sy) / ppghf);
            qvtx[0].y = pos->y + (pys * (ppgw - (sy + ys)) / ppghf);
        } else {
            qvtx[0].y = pos->y + (sy * pys / ppghf);
            qvtx[3].y = pos->y + (pys * (sy + ys) / ppghf);
        }

        if (!quad_is_onscreen(qvtx)) {
            continue;
        }

        if (a->flip & 1) {
            qvtx[3].s = (sx / ppgwf) - sadd;
            qvtx[0].s = ((sx + xs) / ppgwf) - sadd;
        } else {
            qvtx[0].s = sadd + (sx / ppgwf);
            qvtx[3].s = sadd + ((sx + xs) / ppgwf);
        }

        if (a->flip & 2) {
            qvtx[3].t = (sy / ppghf) - tadd;
            qvtx[0].t = ((sy + ys) / ppghf) - tadd;
        } else {
            qvtx[0].t = tadd + (sy / ppghf);
            qvtx[3].t = tadd + ((sy + ys) / ppghf);
        }

        ppgWriteQuadOnly2(qvtx, a->col, run->texhan | (palhan << 0x10));
    }

    return 1;
}

s32 ppgWriteQuadWithST_A(Vertex* pos, u32 col) {
    ppgWriteQuadOnly(pos, col, ppg_w.hanTex | (ppg_w.hanPal << 0x10));
    return 1;
}

s32 ppgWriteQuadWithST_A2(Vertex* pos, u32 col) {
    ppgWriteQuadOnly2(pos, col, ppg_w.hanTex | (ppg_w.hanPal << 0x10));
    return 1;
}

void ppgWriteQuadOnly(Vertex* pos, u32 col, u32 texCode) {
    Sprite prm;
    s32 i;

    flSetRenderState(FLRENDER_TEXSTAGE0, texCode);

    for (i = 0; i < 4; i++) {
        prm.v[i].x = pos[i].x;
        prm.v[i].y = pos[i].y;
        prm.v[i].z = pos[i].z;
        prm.t[i].s = pos[i].s;
        prm.t[i].t = pos[i].t;
    }

    Renderer_DrawTexturedQuad(&prm, col);
}

void ppgWriteQuadOnly2(Vertex* pos, u32 col, u32 texCode) {
    Sprite prm;

    flSetRenderState(FLRENDER_TEXSTAGE0, texCode);

    prm.v[0].x = pos[0].x;
    prm.v[0].y = pos[0].y;
    prm.v[0].z = pos[0].z;
    prm.t[0].s = pos[0].s;
    prm.t[0].t = pos[0].t;
    prm.v[3].x = pos[3].x;
    prm.v[3].y = pos[3].y;
    prm.v[3].z = pos[3].z;
    prm.t[3].s = pos[3].s;
    prm.t[3].t = pos[3].t;

    Renderer_DrawSprite(&prm, col);
}

s32 ppgWriteQuadWithST_B(Vertex* pos, const PPGQuadArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    PPGDataList* tb = a->tb;

    u16 texhan;
    u16 palhan = 0;

    if (tb == NULL) {
        tb = ppg_w.cur;

        if (tb == NULL) {
            return ppgWriteQuadWithST_A(pos, a->col);
        }
    }

    if (a->tix < 0) {
        texhan = ppg_w.hanTex;
    } else {
        texhan = tb->tex->handle[a->tix - tb->tex->ixNum1st].b16[0];

        if (texhan == 0) {
            return 0;
        }
    }

    if (tb->tex->handle[a->tix - tb->tex->ixNum1st].b16[1] & 0x4000) {
        if (a->cix < 0) {
            palhan = ppg_w.hanPal;
        } else {
            palhan = tb->pal->handle[a->cix];
        }
    }

    ppgWriteQuadOnly(pos, a->col, texhan | (palhan << 0x10));
    return 1;
}

s32 ppgWriteQuadWithST_B2(Vertex* pos, const PPGQuadArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    PPGDataList* tb = a->tb;

    u16 texhan;
    u16 palhan = 0;

    if (tb == NULL) {
        tb = ppg_w.cur;

        if (tb == NULL) {
            return ppgWriteQuadWithST_A2(pos, a->col);
        }
    }

    if (a->tix < 0) {
        texhan = ppg_w.hanTex;
    } else {
        texhan = tb->tex->handle[a->tix - tb->tex->ixNum1st].b16[0];

        if (texhan == 0) {
            return 0;
        }
    }

    if (tb->tex->handle[a->tix - tb->tex->ixNum1st].b16[1] & 0x4000) {
        if (a->cix < 0) {
            palhan = ppg_w.hanPal;
        } else {
            palhan = tb->pal->handle[a->cix];
        }
    }

    ppgWriteQuadOnly2(pos, a->col, texhan | (palhan << 16));
    return 1;
}

s32 ppgWriteQuadUseTrans(Vertex* pos, const PPGQuadTransArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    PPGDataList* tb = a->tb;

    Vertex qvtx[4];
    s32 i;
    u32 sx;
    u32 sy;
    u32 ppgw;
    u16* phan;
    u16 palhan;
    u16 texhan;
    u8* tran;
    u8 cofsXY;
    u8 xs;
    u8 ys;
    u16 transTotal;
    u16 iPoint;
    u16 ix_ofs;
    f32 pxs;
    f32 pys;
    f32 sadd;
    f32 tadd;
    f32 ppgwf;
    f32 ppghf;
    PPGFileHeader* ppg;

    if (quad_is_offscreen(pos)) {
        return 0;
    }

    if (tb == NULL) {
        tb = ppg_w.cur;
    }

    if (tb == NULL) {
        return ppgWriteQuadWithST_A2(pos, a->col);
    }

    texhan = tb->tex->handle[a->tix - tb->tex->ixNum1st].b16[0];
    ix_ofs = tb->tex->handle[a->tix - tb->tex->ixNum1st].b16[1];

    if (texhan == 0) {
        return 0;
    }

    palhan = 0;

    if (ix_ofs & 0x4000) {
        phan = tb->pal->handle;

        if (phan == NULL) {
            return 0;
        }
    }

    if (tb->tex->srcAdrs != NULL) {
        ppg = (PPGFileHeader*)(tb->tex->srcAdrs + tb->tex->offset[ix_ofs & 0xFFF]);
        transTotal = ((ppg->transNums >> 8) & 0xFF) | ((ppg->transNums & 0xFF) << 8);

        if (transTotal != 0) {
            return write_transparent_runs(pos, a, &(PPGTransRun){ ppg, transTotal, ix_ofs, phan, texhan });
        }
    }

    if (ix_ofs & 0x4000) {
        if (a->cix < 0) {
            palhan = ppg_w.hanPal;
        } else {
            palhan = phan[a->cix];
        }
    }

    switch (a->flip) {
    case 0:
        pos[0].s = pos[0].t = 0.0f;
        pos[3].s = pos[3].t = 1.0f;
        break;

    case 1:
        pos[3].s = pos[0].t = 0.0f;
        pos[0].s = pos[3].t = 1.0f;
        break;

    case 2:
        pos[0].s = pos[3].t = 0.0f;
        pos[3].s = pos[0].t = 1.0f;
        break;

    default:
        pos[0].s = pos[0].t = 1.0f;
        pos[3].s = pos[3].t = 0.0f;
        break;
    }

    ppgWriteQuadOnly2(pos, a->col, texhan | (palhan << 0x10));
    return 1;
}
