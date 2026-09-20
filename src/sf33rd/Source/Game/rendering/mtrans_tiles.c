/**
 * @file mtrans_tiles.c
 * The store_* tile passes, split out of mtrans.c
 *
 * Nine passes write a pattern's tiles into the chip queue: plain, cached and
 * new, each in a 16/32 paletted, a CPS3 paletted and a true-colour flavour.
 * They are the bulk of mtrans.c's lines and the whole of its duplication web,
 * and they talk to the rest of the engine through one struct and one queue
 * call, so they cut away cleanly.
 *
 * Like mtrans_buffers.c before it, the functions here were `static` only
 * because everything lived in one file, and every one of them was created by
 * this campaign. Nothing the decompilation wrote is widened.
 */

#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/Source/Game/rendering/chren3rd.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/mtrans_internal.h"
#include "sf33rd/Source/Game/rendering/mtrans_seqs.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

// Every store_* pass queues its chip with this call. Across all eighteen of
// them the nine arguments are identical character for character except two -
// which texture index the chip comes from, and its attribute word - and those
// two are written out in full at each call site. The placement fields are the
// seven that never differ; the helper passes all of them straight through and
// does nothing else with them.
typedef struct {
    f32 x;
    f32 y;
    s32 dw;
    s32 dh;
    s32 flip;
    s32 alpha;
    s32 id;
} ChipPlacement;

static s32 store_trans_chip(const ChipPlacement* p, s32 gidx, s32 code, s32 attr) {
    return seqsStoreChip(&(SequenceChip) { p->x - (p->dw * BOOL(p->flip & 0x8000)),
                                           p->y + (p->dh * BOOL(p->flip & 0x4000)),
                                           p->dw,
                                           p->dh,
                                           gidx,
                                           code,
                                           attr,
                                           p->alpha,
                                           p->id });
}

/* What a cached extended pass makes of one tile map entry: the palette it looks
 * the tile up under, and the attribute word each bank's chip is queued with.
 * These three expressions are the whole of what separates the three passes. */
typedef struct {
    s32 palt;
    s32 attr_16;
    s32 attr_32;
} TransTileAttrs;

static TransTileAttrs cached_ext_attrs(const TransRun* run, const TileMapEntry* trsptr) {
    s32 attr = (trsptr->attr ^ run->flip) & 0xC000;

    return (TransTileAttrs) { 0, run->palo | attr, run->palo | (attr | 0x2000) };
}

static TransTileAttrs cached_cp3_ext_attrs(const TransRun* run, const TileMapEntry* trsptr) {
    s32 attr = trsptr->attr;
    s32 palt = (attr & 0x1FF) + run->palo;

    attr = (attr ^ run->flip) & 0xC000;

    return (TransTileAttrs) { 0, attr | palt, (attr | 0x2000) | palt };
}

static TransTileAttrs cached_rgb_ext_attrs(const TransRun* run, const TileMapEntry* trsptr) {
    s32 attr = trsptr->attr;
    s32 palt = (attr & 0x1FF) + run->palo;

    attr = (attr ^ run->flip) & 0xC000;

    return (TransTileAttrs) { palt, attr, attr | 0x2000 };
}

/* The three cached extended passes walk the same tile run and queue the same
 * chips. They differ only in the three values above, so the pass takes the
 * function that produces them.
 *
 * The rgb pass is the one that looks its tiles up under a real palette; the
 * other two pass zero, as they wrote. */
static void store_cached_trans_ext_run(
    const TransRun* run, s32 group, TransTileAttrs (*attrs_of)(const TransRun* run, const TileMapEntry* trsptr)
) {
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TransTileAttrs at;
    TEX* texptr;
    s32 rnum;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;

    cc.parts.group = group;

    while (count--) {
        x = advance_trans_x(x, run->flip, trsptr);
        y = advance_trans_y(y, run->flip, trsptr);

        texptr = (TEX*)((uintptr_t)run->textbl + ((u32*)run->textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        at = attrs_of(run, trsptr);
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            code = get_mltbuf16_ext(run->mt, cc.code, at.palt);

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                at.attr_16
            );
            break;

        case 4:
            code = get_mltbuf32_ext(run->mt, cc.code, at.palt);

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx32,
                code,
                at.attr_32
            );
            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }
}

void store_cached_trans_ext_tiles(const TransRun* run, s32 group) {
    store_cached_trans_ext_run(run, group, cached_ext_attrs);
}

/* One tile about to be made resident, and the bank it is going into: where the
 * tile comes from, how big it is, the palette it expands through, and the group
 * index and code split of its bank. */
typedef struct {
    const TEX* texptr;
    s32 size;
    s32 code;
    s32 palt;
    s32 gidx;
    s32 code_shift;
    s32 code_mask;
} TransTileLoad;

/* Paletted tiles go into the buffer as they are. */
static void make_resident_paletted(const TransRun* run, const TransTileLoad* ld) {
    lz_ext_p6_fx(&((u8*)ld->texptr)[1], run->mt->mltbuf, ld->size);
    njReLoadTexturePartNumG(
        ld->gidx + (ld->code >> ld->code_shift), (s8*)run->mt->mltbuf, ld->code & ld->code_mask, ld->size
    );
}

/* True-colour tiles expand through the colour RAM and occupy twice the bytes. */
static void make_resident_true_color(const TransRun* run, const TransTileLoad* ld) {
    lz_ext_p6_cx(&((u8*)ld->texptr)[1], (u16*)run->mt->mltbuf, ld->size, (u16*)(ColorRAM[ld->palt]));
    njReLoadTexturePartNumG(
        ld->gidx + (ld->code >> ld->code_shift), (s8*)run->mt->mltbuf, ld->code & ld->code_mask, ld->size * 2
    );
}

static TransTileAttrs new_ext_attrs(const TransRun* run, const TileMapEntry* trsptr) {
    s32 attr = (trsptr->attr ^ run->flip) & 0xC000;

    return (TransTileAttrs) { 0, run->palo | attr, run->palo | (attr | 0x2000) };
}

static TransTileAttrs new_cp3_ext_attrs(const TransRun* run, const TileMapEntry* trsptr) {
    s32 attr = trsptr->attr;
    s32 palt = (attr & 0x1FF) + run->palo;

    attr = (attr ^ run->flip) & 0xC000;

    return (TransTileAttrs) { 0, attr | palt, (attr | 0x2000) | palt };
}

static TransTileAttrs new_rgb_ext_attrs(const TransRun* run, const TileMapEntry* trsptr) {
    s32 attr = trsptr->attr;
    s32 palt = (attr & 0x1FF) + run->palo;

    attr = (attr ^ run->flip) & 0xC000;

    return (TransTileAttrs) { palt, attr, attr | 0x2000 };
}

/* What separates the three new extended passes: how a tile map entry becomes a
 * palette and two attribute words, and how a tile that missed its cache is made
 * resident. */
typedef struct {
    TransTileAttrs (*attrs_of)(const TransRun* run, const TileMapEntry* trsptr);
    void (*make_resident)(const TransRun* run, const TransTileLoad* ld);
} TransNewExtOps;

/* The three new extended passes walk the same tile run, look every tile up in
 * the same two extended caches, and queue the same chips. */
static void store_new_trans_ext_run(const TransRun* run, s32 group, PatternInstance* cp, const TransNewExtOps* ops) {
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TransTileAttrs at;
    TEX* texptr;
    s32 rnum;
    s32 size;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;

    cc.parts.group = group;

    while (count--) {
        x = advance_trans_x(x, run->flip, trsptr);
        y = advance_trans_y(y, run->flip, trsptr);

        texptr = (TEX*)((uintptr_t)run->textbl + ((u32*)run->textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        size = (wh * wh) << 6;
        at = ops->attrs_of(run, trsptr);
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            if (get_mltbuf16_ext_2(&(MltbufExtLookup) { run->mt, cc.code, at.palt, &code, cp }) != 0) {
                ops->make_resident(run, &(TransTileLoad) { texptr, size, code, at.palt, run->mt->mltgidx16, 8, 0xFF });
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                at.attr_16
            );
            break;

        case 4:
            if (get_mltbuf32_ext_2(&(MltbufExtLookup) { run->mt, cc.code, at.palt, &code, cp }) != 0) {
                ops->make_resident(run, &(TransTileLoad) { texptr, size, code, at.palt, run->mt->mltgidx32, 6, 0x3F });
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx32,
                code,
                at.attr_32
            );
            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }
}

void store_new_trans_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp) {
    store_new_trans_ext_run(run, group, cp, &(TransNewExtOps) { new_ext_attrs, make_resident_paletted });
}

void store_trans_tiles(const TransRun* run) {
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TEX* texptr;
    s32 rnum;
    s32 size;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;

    while (count--) {
        x = advance_trans_x(x, run->flip, trsptr);
        y = advance_trans_y(y, run->flip, trsptr);

        texptr = (TEX*)((uintptr_t)run->textbl + ((u32*)run->textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        size = (wh * wh) << 6;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            if (get_mltbuf16(run->mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], run->mt->mltbuf, size);
                njReLoadTexturePartNumG(run->mt->mltgidx16 + (code >> 8), (s8*)run->mt->mltbuf, code & 0xFF, size);
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                run->palo | ((trsptr->attr ^ run->flip) & 0xC000)
            );

            break;

        case 4:
            if (get_mltbuf32(run->mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], run->mt->mltbuf, size);
                njReLoadTexturePartNumG(run->mt->mltgidx32 + (code >> 6), (s8*)run->mt->mltbuf, code & 0x3F, size);
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx32,
                code,
                run->palo | (((trsptr->attr ^ run->flip) & 0xC000) | 0x2000)
            );

            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }
}

void store_cached_trans_cp3_ext_tiles(const TransRun* run, s32 group) {
    store_cached_trans_ext_run(run, group, cached_cp3_ext_attrs);
}

void store_new_trans_cp3_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp) {
    store_new_trans_ext_run(run, group, cp, &(TransNewExtOps) { new_cp3_ext_attrs, make_resident_paletted });
}

void store_trans_cp3_tiles(const TransRun* run) {
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TEX* texptr;
    s32 rnum;
    s32 size;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;
    s32 attr;
    s32 palt;

    while (count--) {
        x = advance_trans_x(x, run->flip, trsptr);
        y = advance_trans_y(y, run->flip, trsptr);

        texptr = (TEX*)((uintptr_t)run->textbl + ((u32*)run->textbl)[trsptr->code]);
        dw = (s32)(texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        size = (wh * wh) << 6;
        attr = trsptr->attr;
        palt = (attr & 0x1FF) + run->palo;
        attr = (attr ^ run->flip) & 0xC000;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            if (get_mltbuf16(run->mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], run->mt->mltbuf, size);
                njReLoadTexturePartNumG(run->mt->mltgidx16 + (code >> 8), (s8*)run->mt->mltbuf, code & 0xFF, size);
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                attr | palt
            );

            break;

        case 4:
            if (get_mltbuf32(run->mt, cc.code, 0, &code) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], run->mt->mltbuf, size);
                njReLoadTexturePartNumG(run->mt->mltgidx32 + (code >> 6), (s8*)run->mt->mltbuf, code & 0x3F, size);
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx32,
                code,
                attr | 0x2000 | palt
            );

            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }
}

void store_cached_trans_rgb_ext_tiles(const TransRun* run, s32 group) {
    store_cached_trans_ext_run(run, group, cached_rgb_ext_attrs);
}

void store_new_trans_rgb_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp) {
    store_new_trans_ext_run(run, group, cp, &(TransNewExtOps) { new_rgb_ext_attrs, make_resident_true_color });
}

// One true-colour tile to make resident: where it comes from, how big it is,
// and which pattern and palette it answers to. These are load_trans_rgb16's
// and load_trans_rgb32's five arguments, which are the same list in the same
// order with the same types.
typedef struct {
    MultiTexture* mt;
    TEX* texptr;
    s32 size;
    u32 pattern_code;
    s32 palt;
} RgbTile;

/* Which of the two multi-texture banks a tile is loaded into: the cache lookup,
 * the group index it counts from, and how a slot code splits into a group and a
 * part within it. */
typedef struct {
    s32 (*get_mltbuf)(MultiTexture* mt, u32 code, u32 palt, s32* ret);
    s32 gidx;
    s32 code_shift;
    s32 code_mask;
} TransRgbBank;

/* The 16- and 32-bit rgb loads are the same four lines over a different bank. The
 * group index is read at the call site rather than inside the branch; texcash.c
 * writes mltgidx16 and mltgidx32 once, when the multi-texture is built, so it is
 * the same value either way. */
static s32 load_trans_rgb(const RgbTile* tile, const TransRgbBank* bank) {
    MultiTexture* mt = tile->mt;
    s32 palt = tile->palt;
    s32 code;

    if (bank->get_mltbuf(mt, tile->pattern_code, palt, &code) != 0) {
        lz_ext_p6_cx(&((u8*)tile->texptr)[1], (u16*)mt->mltbuf, tile->size, (u16*)(ColorRAM[palt]));
        njReLoadTexturePartNumG(
            bank->gidx + (code >> bank->code_shift), (s8*)mt->mltbuf, code & bank->code_mask, tile->size * 2
        );
    }

    return code;
}

static s32 load_trans_rgb16(const RgbTile* tile) {
    return load_trans_rgb(tile, &(TransRgbBank) { get_mltbuf16, tile->mt->mltgidx16, 8, 0xFF });
}

static s32 load_trans_rgb32(const RgbTile* tile) {
    return load_trans_rgb(tile, &(TransRgbBank) { get_mltbuf32, tile->mt->mltgidx32, 6, 0x3F });
}

void store_trans_rgb_tiles(const RgbTileRun* run) {
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    TEX* texptr;
    s32 rnum;
    f32 x;
    f32 y;
    PatternCode cc;
    s32 size;
    s32 code;
    s32 attr;
    s32 palt;
    s32 wh;
    s32 dw;
    s32 dh;

    x = y = 0.0f;
    cc.parts.group = run->group;

    while (count--) {
        x = advance_trans_x(x, run->flip, trsptr);
        y = advance_trans_y(y, run->flip, trsptr);

        texptr = (TEX*)((uintptr_t)run->textbl + ((u32*)run->textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        size = (wh * wh) << 6;
        attr = trsptr->attr;
        palt = (attr & 0x1FF) + run->palo;
        attr = (attr ^ run->flip) & 0xC000;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            code = load_trans_rgb16(&(RgbTile) { run->mt, texptr, size, cc.code, palt });

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                attr
            );
            break;

        case 4:
            code = load_trans_rgb32(&(RgbTile) { run->mt, texptr, size, cc.code, palt });

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx32,
                code,
                attr | 0x2000
            );
            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }
}
