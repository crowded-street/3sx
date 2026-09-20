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
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
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

void store_cached_trans_ext_tiles(const TransRun* run, s32 group) {
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TEX* texptr;
    s32 rnum;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;

    (void)dw;
    (void)dh;

    cc.parts.group = group;

    while (count--) {
        x = advance_trans_x(x, run->flip, trsptr);
        y = advance_trans_y(y, run->flip, trsptr);

        texptr = (TEX*)((uintptr_t)run->textbl + ((u32*)run->textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            code = get_mltbuf16_ext(run->mt, cc.code, 0);

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                run->palo | ((trsptr->attr ^ run->flip) & 0xC000)
            );

            break;

        case 4:
            code = get_mltbuf32_ext(run->mt, cc.code, 0);

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

void store_new_trans_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp) {
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

    (void)dw;
    (void)dh;

    cc.parts.group = group;

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
            if (get_mltbuf16_ext_2(&(MltbufExtLookup) { run->mt, cc.code, 0, &code, cp }) != 0) {
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
            if (get_mltbuf32_ext_2(&(MltbufExtLookup) { run->mt, cc.code, 0, &code, cp }) != 0) {
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
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TEX* texptr;
    s32 rnum;
    s32 code;
    s32 wh;
    s32 dw;
    s32 dh;
    s32 attr;
    s32 palt;

    (void)dw;
    (void)dh;

    cc.parts.group = group;

    while (count--) {
        x = advance_trans_x(x, run->flip, trsptr);
        y = advance_trans_y(y, run->flip, trsptr);

        texptr = (TEX*)((uintptr_t)run->textbl + ((u32*)run->textbl)[trsptr->code]);
        dw = (texptr->wh & 0xE0) >> 2;
        dh = (texptr->wh & 0x1C) * 2;
        wh = (texptr->wh & 3) + 1;
        attr = trsptr->attr;
        palt = (attr & 0x1FF) + run->palo;
        attr = (attr ^ run->flip) & 0xC000;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            code = get_mltbuf16_ext(run->mt, cc.code, 0);

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                attr | palt
            );

            break;

        case 4:
            code = get_mltbuf32_ext(run->mt, cc.code, 0);

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx32,
                code,
                (attr | 0x2000) | palt
            );

            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }
}

void store_new_trans_cp3_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp) {
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

    (void)dw;
    (void)dh;

    cc.parts.group = group;

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
            if (get_mltbuf16_ext_2(&(MltbufExtLookup) { run->mt, cc.code, 0, &code, cp }) != 0) {
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
            if (get_mltbuf32_ext_2(&(MltbufExtLookup) { run->mt, cc.code, 0, &code, cp }) != 0) {
                lz_ext_p6_fx(&((u8*)texptr)[1], run->mt->mltbuf, size);
                njReLoadTexturePartNumG(run->mt->mltgidx32 + (code >> 6), (s8*)run->mt->mltbuf, code & 0x3F, size);
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx32,
                code,
                (attr | 0x2000) | palt
            );

            break;
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }
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
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TEX* texptr;
    s32 rnum;
    s32 code;
    s32 attr;
    s32 palt;
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
        attr = trsptr->attr;
        palt = (attr & 0x1FF) + run->palo;
        attr = (attr ^ run->flip) & 0xC000;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            code = get_mltbuf16_ext(run->mt, cc.code, palt);

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                attr
            );
            break;

        case 4:
            code = get_mltbuf32_ext(run->mt, cc.code, palt);

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

void store_new_trans_rgb_ext_tiles(const TransRun* run, s32 group, PatternInstance* cp) {
    TileMapEntry* trsptr = run->trsptr;
    s32 count = run->count;
    f32 x = run->x, y = run->y;
    PatternCode cc = run->cc;
    TEX* texptr;
    s32 rnum;
    s32 size;
    s32 code;
    s32 attr;
    s32 palt;
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
        attr = trsptr->attr;
        palt = (attr & 0x1FF) + run->palo;
        attr = (attr ^ run->flip) & 0xC000;
        cc.parts.offset = trsptr->code;

        switch (wh) {
        case 1:
        case 2:
            if (get_mltbuf16_ext_2(&(MltbufExtLookup) { run->mt, cc.code, palt, &code, cp }) != 0) {
                lz_ext_p6_cx(&((u8*)texptr)[1], (u16*)run->mt->mltbuf, size, (u16*)(ColorRAM[palt]));
                njReLoadTexturePartNumG(run->mt->mltgidx16 + (code >> 8), (s8*)run->mt->mltbuf, code & 0xFF, size * 2);
            }

            rnum = store_trans_chip(
                &(ChipPlacement) { x, y, dw, dh, run->flip, run->wk->my_clear_level, run->mt->id },
                run->mt->mltgidx16,
                code,
                attr
            );
            break;

        case 4:
            if (get_mltbuf32_ext_2(&(MltbufExtLookup) { run->mt, cc.code, palt, &code, cp }) != 0) {
                lz_ext_p6_cx(&((u8*)texptr)[1], (u16*)run->mt->mltbuf, size, (u16*)(ColorRAM[palt]));
                njReLoadTexturePartNumG(run->mt->mltgidx32 + (code >> 6), (s8*)run->mt->mltbuf, code & 0x3F, size * 2);
            }

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

static s32 load_trans_rgb16(const RgbTile* tile) {
    MultiTexture* mt = tile->mt;
    s32 palt = tile->palt;
    s32 code;

    if (get_mltbuf16(mt, tile->pattern_code, palt, &code) != 0) {
        lz_ext_p6_cx(&((u8*)tile->texptr)[1], (u16*)mt->mltbuf, tile->size, (u16*)(ColorRAM[palt]));
        njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, tile->size * 2);
    }

    return code;
}

static s32 load_trans_rgb32(const RgbTile* tile) {
    MultiTexture* mt = tile->mt;
    s32 palt = tile->palt;
    s32 code;

    if (get_mltbuf32(mt, tile->pattern_code, palt, &code) != 0) {
        lz_ext_p6_cx(&((u8*)tile->texptr)[1], (u16*)mt->mltbuf, tile->size, (u16*)(ColorRAM[palt]));
        njReLoadTexturePartNumG(mt->mltgidx32 + (code >> 6), (s8*)mt->mltbuf, code & 0x3F, tile->size * 2);
    }

    return code;
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
