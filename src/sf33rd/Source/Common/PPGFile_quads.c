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
/* The geometry every run of one quad is placed against: the quad's origin and
 * span in screen space, the chunk's dimensions, and the half-texel insets. The
 * insets are computed and clamped and then overwritten with zero, which is what
 * the original does and what the hardware was given; the arithmetic is kept
 * because it is what the file says. */
typedef struct {
    f32 ox;
    f32 oy;
    f32 pxs;
    f32 pys;
    f32 ppgwf;
    f32 ppghf;
    u32 ppgw;
    f32 sadd;
    f32 tadd;
} TransQuadGeom;

/* One run's cell inside the chunk: where it starts in texels and how big it is.
 * Both sizes are stored one less than their extent in the file. */
typedef struct {
    u32 sx;
    u32 sy;
    u8 xs;
    u8 ys;
} TransRunCell;

static void set_run_xy(Vertex* qvtx, const TransQuadGeom* g, const TransRunCell* c, s32 flip) {
    if (flip & 1) {
        qvtx[3].x = g->ox + (g->pxs * (g->ppgw - c->sx) / g->ppgwf);
        qvtx[0].x = g->ox + (g->pxs * (g->ppgw - (c->sx + c->xs)) / g->ppgwf);
    } else {
        qvtx[0].x = g->ox + (c->sx * g->pxs / g->ppgwf);
        qvtx[3].x = g->ox + (g->pxs * (c->sx + c->xs) / g->ppgwf);
    }

    if (flip & 2) {
        qvtx[3].y = g->oy + (g->pys * (g->ppgw - c->sy) / g->ppghf);
        qvtx[0].y = g->oy + (g->pys * (g->ppgw - (c->sy + c->ys)) / g->ppghf);
    } else {
        qvtx[0].y = g->oy + (c->sy * g->pys / g->ppghf);
        qvtx[3].y = g->oy + (g->pys * (c->sy + c->ys) / g->ppghf);
    }
}

static void set_run_st(Vertex* qvtx, const TransQuadGeom* g, const TransRunCell* c, s32 flip) {
    if (flip & 1) {
        qvtx[3].s = (c->sx / g->ppgwf) - g->sadd;
        qvtx[0].s = ((c->sx + c->xs) / g->ppgwf) - g->sadd;
    } else {
        qvtx[0].s = g->sadd + (c->sx / g->ppgwf);
        qvtx[3].s = g->sadd + ((c->sx + c->xs) / g->ppgwf);
    }

    if (flip & 2) {
        qvtx[3].t = (c->sy / g->ppghf) - g->tadd;
        qvtx[0].t = ((c->sy + c->ys) / g->ppghf) - g->tadd;
    } else {
        qvtx[0].t = g->tadd + (c->sy / g->ppghf);
        qvtx[3].t = g->tadd + ((c->sy + c->ys) / g->ppghf);
    }
}

static TransQuadGeom quad_trans_geometry(const Vertex* pos, const PPGFileHeader* ppg) {
    TransQuadGeom g;

    g.ox = pos->x;
    g.oy = pos->y;
    g.ppgwf = ppg->width;
    g.ppgw = ppg->width;
    g.ppghf = ppg->height;
    g.pxs = pos[3].x - pos[0].x;
    g.pys = pos[3].y - pos[0].y;
    g.sadd = 0.5f / g.pxs;
    g.tadd = 0.5f / g.pys;

    if (g.sadd >= (1.0f / (16.0f * g.ppgwf))) {
        g.sadd = 1.0f / (16.0f * g.ppgwf);
    }

    if (g.tadd >= (1.0f / (16.0f * g.ppghf))) {
        g.tadd = 1.0f / (16.0f * g.ppghf);
    }

    g.sadd = 0;
    g.tadd = 0;
    return g;
}

static s32 write_transparent_runs(Vertex* pos, const PPGQuadTransArgs* a, const PPGTransRun* run) {
    TransQuadGeom g = quad_trans_geometry(pos, run->ppg);
    TransRunCell c;
    Vertex qvtx[4];
    s32 i;
    u16 palhan;
    u8* tran;
    u8 cofsXY;
    u16 iPoint;

    tran = (u8*)&run->ppg[1];
    qvtx[0].z = pos[0].z;
    qvtx[3].z = pos[3].z;

    for (i = 0; i < run->transTotal; i++) {
        if (run->ix_ofs & 0x4000) {
            palhan = run->phan[*tran + a->pal];
        }

        tran++;
        iPoint = *tran++;
        cofsXY = *tran++;
        c.xs = (cofsXY >> 4) + 1;
        c.ys = (cofsXY & 0xF) + 1;
        c.sx = iPoint % g.ppgw;
        c.sy = iPoint / g.ppgw;
        set_run_xy(qvtx, &g, &c, a->flip);

        if (!quad_is_onscreen(qvtx)) {
            continue;
        }

        set_run_st(qvtx, &g, &c, a->flip);
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

/* B and B2 resolved their handles identically and differed in nothing but the
 * pair of writers they handed them to - the quad pair or the sprite pair - and
 * in how the same shift was spelled. The resolution is written once, with the
 * two writers as parameters. */
static s32 ppgWriteQuadWithST_Bx(Vertex* pos, const PPGQuadArgs* a, s32 (*no_list)(Vertex*, u32),
                                 void (*write)(Vertex*, u32, u32)) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    PPGDataList* tb = a->tb;

    u16 texhan;
    u16 palhan = 0;

    if (tb == NULL) {
        tb = ppg_w.cur;

        if (tb == NULL) {
            return no_list(pos, a->col);
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

    write(pos, a->col, texhan | (palhan << 0x10));
    return 1;
}

s32 ppgWriteQuadWithST_B(Vertex* pos, const PPGQuadArgs* a) {
    return ppgWriteQuadWithST_Bx(pos, a, ppgWriteQuadWithST_A, ppgWriteQuadOnly);
}

s32 ppgWriteQuadWithST_B2(Vertex* pos, const PPGQuadArgs* a) {
    return ppgWriteQuadWithST_Bx(pos, a, ppgWriteQuadWithST_A2, ppgWriteQuadOnly2);
}

/* The chunk header this texture index reads from, and how many transparent runs
 * it lists, byte-swapped. NULL when the texture has no chunk data behind it, in
 * which case the run count is not written. */
static PPGFileHeader* transparent_run_header(const Texture* tex, u16 ix_ofs, u16* transTotal) {
    PPGFileHeader* ppg;

    if (tex->srcAdrs == NULL) {
        return NULL;
    }

    ppg = (PPGFileHeader*)(tex->srcAdrs + tex->offset[ix_ofs & 0xFFF]);
    *transTotal = ((ppg->transNums >> 8) & 0xFF) | ((ppg->transNums & 0xFF) << 8);
    return ppg;
}

/* The palette handle a quad draws with: the one the caller asked for by index,
 * or the current one when it asked for none. */
static u16 resolve_palette_handle(const u16* phan, s32 cix) {
    if (cix < 0) {
        return ppg_w.hanPal;
    }

    return phan[cix];
}

/* The two corners the sprite is drawn from, in the order the flip asks for.
 * Only corners 0 and 3 carry texture coordinates for a sprite. */
static void set_quad_corner_st(Vertex* pos, s32 flip) {
    switch (flip) {
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
}

/* What a transparent-run quad draws with: the texture handle, the handle word
 * the chunk offset and the CI flag live in, and the palette table behind it.
 * 0 when there is nothing to draw - no texture handle, or a CI texture whose
 * palette table is missing. The palette pointer is left alone for a texture
 * that is not CI, which is what the caller's own declaration did. */
typedef struct {
    u16 texhan;
    u16 ix_ofs;
    u16* phan;
} TransQuadHandles;

static s32 resolve_trans_quad_handles(const PPGDataList* tb, s32 tix, TransQuadHandles* h) {
    h->texhan = tb->tex->handle[tix - tb->tex->ixNum1st].b16[0];
    h->ix_ofs = tb->tex->handle[tix - tb->tex->ixNum1st].b16[1];

    if (h->texhan == 0) {
        return 0;
    }

    if (h->ix_ofs & 0x4000) {
        h->phan = tb->pal->handle;

        if (h->phan == NULL) {
            return 0;
        }
    }

    return 1;
}

s32 ppgWriteQuadUseTrans(Vertex* pos, const PPGQuadTransArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    PPGDataList* tb = a->tb;

    TransQuadHandles h;
    u16 palhan = 0;
    u16 transTotal;
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

    if (!resolve_trans_quad_handles(tb, a->tix, &h)) {
        return 0;
    }

    ppg = transparent_run_header(tb->tex, h.ix_ofs, &transTotal);

    if ((ppg != NULL) && (transTotal != 0)) {
        return write_transparent_runs(pos, a, &(PPGTransRun){ ppg, transTotal, h.ix_ofs, h.phan, h.texhan });
    }

    if (h.ix_ofs & 0x4000) {
        palhan = resolve_palette_handle(h.phan, a->cix);
    }

    set_quad_corner_st(pos, a->flip);
    ppgWriteQuadOnly2(pos, a->col, h.texhan | (palhan << 0x10));
    return 1;
}
