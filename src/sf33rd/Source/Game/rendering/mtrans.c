/**
 * @file mtrans.c
 * Main Graphics Rendering and Transformation Engine
 */

#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/chren3rd.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans_internal.h"
#include "sf33rd/Source/Game/rendering/mtrans_seqs.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/rendering/texgroup_data.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

#include "core/renderer.h"

#include <SDL3/SDL.h>

s32 curr_bright;

static const u16 flptbl[4] = { 0x0000, 0x8000, 0x4000, 0xC000 };

static const u32 bright_type[4][16] = { { 0x00FFFFFF,
                                          0x00EEEEEE,
                                          0x00DDDDDD,
                                          0x00CCCCCC,
                                          0x00BBBBBB,
                                          0x00AAAAAA,
                                          0x00999999,
                                          0x00888888,
                                          0x00777777,
                                          0x00666666,
                                          0x00555555,
                                          0x00444444,
                                          0x00333333,
                                          0x00222222,
                                          0x00111111,
                                          0x00000000 },
                                        { 0x00FFFFFF,
                                          0x00FFEEEE,
                                          0x00FFDDDD,
                                          0x00FFCCCC,
                                          0x00FFBBBB,
                                          0x00FFAAAA,
                                          0x00FF9999,
                                          0x00FF8888,
                                          0x00FF7777,
                                          0x00FF6666,
                                          0x00FF5555,
                                          0x00FF4444,
                                          0x00FF3333,
                                          0x00FF2222,
                                          0x00FF1111,
                                          0x00FF0000 },
                                        { 0x00FFFFFF,
                                          0x00EEFFEE,
                                          0x00DDFFDD,
                                          0x00CCFFCC,
                                          0x00BBFFBB,
                                          0x00AAFFAA,
                                          0x0099FF99,
                                          0x0088FF88,
                                          0x0077FF77,
                                          0x0066FF66,
                                          0x0055FF55,
                                          0x0044FF44,
                                          0x0033FF33,
                                          0x0022FF22,
                                          0x0011FF11,
                                          0x0000FF00 },
                                        { 0x00FFFFFF,
                                          0x00EEEEFF,
                                          0x00DDDDFF,
                                          0x00CCCCFF,
                                          0x00BBBBFF,
                                          0x00AAAAFF,
                                          0x009999FF,
                                          0x008888FF,
                                          0x007777FF,
                                          0x006666FF,
                                          0x005555FF,
                                          0x004444FF,
                                          0x003333FF,
                                          0x002222FF,
                                          0x001111FF,
                                          0x000000FF } };

// forward decls, below TransRun because they name it
void mlt_obj_trans_ext(MultiTexture* mt, WORK* wk, s32 base_y);
void mlt_obj_trans_cp3_ext(MultiTexture* mt, WORK* wk, s32 base_y);
void mlt_obj_trans_rgb_ext(MultiTexture* mt, WORK* wk, s32 base_y);

static bool is_matching_trans_entry(TileMapEntry* trsptr, s32 cods, s32 atrs) {
    return !(trsptr->attr & 0x1000) && (trsptr->code == cods) && ((trsptr->attr & 0xF) == atrs);
}

// One rewrite of a melted tile across a group's trans tables: which table and
// which range of it to walk, the code and attribute to match, and the pair to
// put in their place. These are search_trsptr's seven arguments, in order.
typedef struct {
    void* trstbl;
    s32 i;
    s32 n;
    s32 cods;
    s32 atrs;
    s32 codd;
    s32 atrd;
} TransSwap;

static void search_trsptr(const TransSwap* sw) {
    s32 atrd = sw->atrd;
    s32 j;
    u16* tmpbas;
    s32 ctemp;
    TileMapEntry* tmpptr;
    TileMapEntry* unused_s4;

    atrd &= 0x3FFF;

    for (j = sw->i; j < sw->n; j++) {
        tmpbas = (u16*)(sw->trstbl + ((u32*)sw->trstbl)[j]);
        ctemp = *tmpbas;
        tmpbas++;
        tmpptr = (TileMapEntry*)tmpbas;

        while (ctemp != 0) {
            if (is_matching_trans_entry(tmpptr, sw->cods, sw->atrs)) {
                tmpptr->code = sw->codd;
                tmpptr->attr = (tmpptr->attr & 0xC000) | atrd;
            }

            ctemp--;
            unused_s4 = tmpptr;
            tmpptr = unused_s4 + 1;
        }
    }
}

// A group whose trans table never loaded is not something the draw can recover
// from, so the original hangs here rather than reading it. Eight copies,
// identical character for character; this one never returns either.
static void require_valid_trans_group(s32 i) {
    if (texgrplds[i].ok == 0) {
        // The trans data is not valid. Group number: %d\n
        flLogOut("トランスデータが有効ではありません。グループ番号：%d\n", i);
        while (1) {}
    }
}

// Every mlt_obj_* entry point opens its draw with this: the work's brightness
// tint, or none, and then the object matrix. Eight copies, identical character
// for character.
static void setup_bright_and_matrix(WORK* wk, s32 base_y) {
    if (wk->my_bright_type) {
        curr_bright = bright_type[wk->my_bright_type - 1][wk->my_bright_level];
    } else {
        curr_bright = 0xFFFFFF;
    }

    mlt_obj_matrix(wk, base_y);
}

void mlt_obj_disp(MultiTexture* mt, WORK* wk, s32 base_y) {
    u16* trsbas;
    TileMapEntry* trsptr;
    s32 rnum;
    s32 attr;
    s32 palo;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s32 dw;
    s32 dh;

    ppgSetupCurrentDataList(&mt->texList);
    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    require_valid_trans_group(i);

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    attr = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd & 0xF;

    setup_bright_and_matrix(wk, base_y);

    while (count--) {
        x = advance_trans_x(x, attr, trsptr);
        y = advance_trans_y(y, attr, trsptr);

        dw = ((trsptr->attr & 0xC00) >> 7) + 8;
        dh = ((trsptr->attr & 0x300) >> 5) + 8;

        if (!(trsptr->attr & 0x2000)) {
            rnum = seqsStoreChip(&(SequenceChip) { x - (dw * BOOL(attr & 0x8000)),
                                                   y + (dh * BOOL(attr & 0x4000)),
                                                   dw,
                                                   dh,
                                                   mt->mltgidx16,
                                                   trsptr->code,
                                                   palo + ((trsptr->attr ^ attr) & 0xE00F),
                                                   wk->my_clear_level,
                                                   mt->id });
        } else {
            rnum = seqsStoreChip(&(SequenceChip) { x - dw * BOOL(attr & 0x8000),
                                                   y + dh * BOOL(attr & 0x4000),
                                                   dw,
                                                   dh,
                                                   mt->mltgidx32,
                                                   trsptr->code,
                                                   palo + ((trsptr->attr ^ attr) & 0xE00F),
                                                   wk->my_clear_level,
                                                   mt->id });
        }

        if (rnum == 0) {
            break;
        }

        trsptr += 1;
    }

    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

void mlt_obj_disp_rgb(MultiTexture* mt, WORK* wk, s32 base_y) {
    u16* trsbas;
    TileMapEntry* trsptr;
    s32 rnum;
    s32 attr;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s32 dw;
    s32 dh;

    ppgSetupCurrentDataList(&mt->texList);
    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    require_valid_trans_group(i);

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    attr = flptbl[wk->cg_flip ^ wk->rl_flag];

    setup_bright_and_matrix(wk, base_y);

    while (count--) {
        x = advance_trans_x(x, attr, trsptr);
        y = advance_trans_y(y, attr, trsptr);

        dw = ((trsptr->attr & 0xC00) >> 7) + 8;
        dh = ((trsptr->attr & 0x300) >> 5) + 8;

        if (!(trsptr->attr & 0x2000)) {
            rnum = seqsStoreChip(&(SequenceChip) { x - (dw * BOOL(attr & 0x8000)),
                                                   y + (dh * BOOL(attr & 0x4000)),
                                                   dw,
                                                   dh,
                                                   mt->mltgidx16,
                                                   trsptr->code,
                                                   (trsptr->attr ^ attr) & 0xE000,
                                                   wk->my_clear_level,
                                                   mt->id });
        } else {
            rnum = seqsStoreChip(&(SequenceChip) { x - (dw * BOOL(attr & 0x8000)),
                                                   y + (dh * BOOL(attr & 0x4000)),
                                                   dw,
                                                   dh,
                                                   mt->mltgidx32,
                                                   trsptr->code,
                                                   (trsptr->attr ^ attr) & 0xE000,
                                                   wk->my_clear_level,
                                                   mt->id });
        }

        if (rnum == 0) {
            break;
        }

        trsptr++;
    }

    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

s16 getObjectHeight(u16 cgnum) {
    s32 count;
    TileMapEntry* trsptr;
    s16 maxHeight;
    u16* trsbas;
    s32 i = obj_group_table[cgnum];
    s16 height;

    if (i == 0) {
        return 0;
    }

    if (texgrplds[i].ok == 0) {
        return 0;
    }

    cgnum -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)((s8*)texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[cgnum]);
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;

    for (maxHeight = height = 0; count--; trsptr++) {
        height = height + trsptr->y;

        if (height > maxHeight) {
            maxHeight = height;
        }
    }

    if (height) {
        // do nothing
    }

    return maxHeight;
}

// mlt_obj_trans and mlt_obj_trans_cp3 differ in nothing but which extended
// entry point they hand an extended texture to and which tile pass they run.
// Each names its own pair in full at its own call site; the struct is built
// there and never stored.
typedef struct {
    void (*ext)(MultiTexture* mt, WORK* wk, s32 base_y);
    void (*store_tiles)(const TransRun* run);
} TransVariant;

/* What a transfer pass resolves before it can queue anything: the texture table
 * and tile map the work's current pattern points at, how many entries it has,
 * the flip word and palette offset, and the group they came from. */
typedef struct {
    u32* textbl;
    TileMapEntry* trsptr;
    s32 count;
    s32 flip;
    s32 palo;
    s32 group;
} TransSetup;

/* Open a transfer: resolve the work's pattern into a tile run and set the
 * brightness and matrix. Returns 0 where the group table says there is nothing
 * to draw, which is the early return both passes wrote. */
static s32 begin_obj_trans(WORK* wk, s32 base_y, TransSetup* out) {
    u16* trsbas;
    s32 n = wk->cg_number;
    s32 i = obj_group_table[n];

    if (i == 0) {
        return 0;
    }

    require_valid_trans_group(i);

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    out->textbl = (u32*)texgrplds[i].texture_table;
    out->count = *trsbas;
    trsbas++;
    out->trsptr = (TileMapEntry*)trsbas;
    out->flip = flptbl[wk->cg_flip ^ wk->rl_flag];
    out->palo = wk->colcd;
    out->group = i;

    setup_bright_and_matrix(wk, base_y);

    return 1;
}

/* Close a transfer. */
static void end_obj_trans(MultiTexture* mt, WORK* wk) {
    seqs_w.up[mt->id] = 1;
    appRenewTempPriority(wk->position_z);
}

static void mlt_obj_trans_common(MultiTexture* mt, WORK* wk, s32 base_y, const TransVariant* variant) {
    TransSetup ts;
    PatternCode cc;

    ppgSetupCurrentDataList(&mt->texList);

    if (mt->ext) {
        variant->ext(mt, wk, base_y);
        return;
    }

    if (begin_obj_trans(wk, base_y, &ts) == 0) {
        return;
    }

    cc.parts.group = ts.group;
    variant->store_tiles(&(TransRun) { mt, wk, ts.textbl, ts.trsptr, ts.count, ts.flip, ts.palo, 0.0f, 0.0f, cc });

    end_obj_trans(mt, wk);
}

void mlt_obj_trans(MultiTexture* mt, WORK* wk, s32 base_y) {
    mlt_obj_trans_common(mt, wk, base_y, &(TransVariant) { mlt_obj_trans_ext, store_trans_tiles });
}

void mlt_obj_trans_cp3(MultiTexture* mt, WORK* wk, s32 base_y) {
    mlt_obj_trans_common(mt, wk, base_y, &(TransVariant) { mlt_obj_trans_cp3_ext, store_trans_cp3_tiles });
}

// The three extended transfer entry points differ in nothing but which pair of
// tile passes they call and what they seed the pattern code's group with. Each
// names its own pair and its own seed in full at its own call site; this struct
// exists only so the shared body stays inside four arguments, and it is built
// at the call site and never stored.
typedef struct {
    s32 group_code;
    void (*store_cached)(const TransRun* run, s32 group);
    void (*store_new)(const TransRun* run, s32 group, PatternInstance* cp);
} ExtTransVariant;

static void mlt_obj_trans_ext_common(MultiTexture* mt, WORK* wk, s32 base_y, const ExtTransVariant* variant) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    s32 flip;
    s32 palo;
    s32 count;
    s32 n;
    s32 i;
    f32 x;
    f32 y;
    s16 ix;
    PatternCode cc;
    PatternInstance* cp;

    (void)textbl;

    n = wk->cg_number;
    i = obj_group_table[n];

    if (i == 0) {
        return;
    }

    require_valid_trans_group(i);

    n -= texgrpdat[i].num_of_1st;
    trsbas = (u16*)(texgrplds[i].trans_table + ((u32*)texgrplds[i].trans_table)[n]);
    textbl = (u32*)texgrplds[i].texture_table;
    count = *trsbas;
    trsbas++;
    trsptr = (TileMapEntry*)trsbas;
    x = y = 0.0f;
    flip = flptbl[wk->cg_flip ^ wk->rl_flag];
    palo = wk->colcd;

    setup_bright_and_matrix(wk, base_y);
    cc.parts.group = variant->group_code;
    cc.parts.offset = wk->cg_number;
    ix = check_patcash_ex_trans(mt->cpat, cc.code);

    if (!(ix < 0)) {
        cp = mt->cpat->adr[ix];
        cp->curr_disp = 1;
        cp->time = mt->mltcshtime16;
        makeup_tpu_free(mt->mltnum16 / 256, mt->mltnum32 / 64, &cp->map);
        variant->store_cached(&(TransRun) { mt, wk, textbl, trsptr, count, flip, palo, x, y, cc }, i);

        seqs_w.up[mt->id] = 1;
        appRenewTempPriority(wk->position_z);
        return;
    }

    {
        ix = get_free_patcash_index(mt->cpat);
        cp = &mt->cpat->patt[ix];
        mt->cpat->adr[mt->cpat->kazu] = cp;
        mt->cpat->kazu += 1;
        cp->curr_disp = 1;
        cp->time = mt->mltcshtime16;
        cp->cg.code = cc.code;
        cp->x16 = 0;
        cp->x32 = 0;
        SDL_zero(cp->map);
        variant->store_new(&(TransRun) { mt, wk, textbl, trsptr, count, flip, palo, x, y, cc }, i, cp);

        seqs_w.up[mt->id] = 1;
        appRenewTempPriority(wk->position_z);
    }
}

void mlt_obj_trans_ext(MultiTexture* mt, WORK* wk, s32 base_y) {
    mlt_obj_trans_ext_common(
        mt, wk, base_y, &(ExtTransVariant) { 0, store_cached_trans_ext_tiles, store_new_trans_ext_tiles }
    );
}

void mlt_obj_trans_cp3_ext(MultiTexture* mt, WORK* wk, s32 base_y) {
    mlt_obj_trans_ext_common(
        mt, wk, base_y, &(ExtTransVariant) { 0, store_cached_trans_cp3_ext_tiles, store_new_trans_cp3_ext_tiles }
    );
}

void mlt_obj_trans_rgb_ext(MultiTexture* mt, WORK* wk, s32 base_y) {
    mlt_obj_trans_ext_common(
        mt,
        wk,
        base_y,
        &(ExtTransVariant) { wk->colcd, store_cached_trans_rgb_ext_tiles, store_new_trans_rgb_ext_tiles }
    );
}

f32 advance_trans_x(f32 x, s32 flip, TileMapEntry* trsptr) {
    if (flip & 0x8000) {
        x += trsptr->x;
    } else {
        x -= trsptr->x;
    }

    return x;
}

f32 advance_trans_y(f32 y, s32 flip, TileMapEntry* trsptr) {
    if (flip & 0x4000) {
        y -= trsptr->y;
    } else {
        y += trsptr->y;
    }

    return y;
}

void mlt_obj_trans_rgb(MultiTexture* mt, WORK* wk, s32 base_y) {
    TransSetup ts;

    ppgSetupCurrentDataList(&mt->texList);

    if (mt->ext) {
        mlt_obj_trans_rgb_ext(mt, wk, base_y);
        return;
    }

    if (begin_obj_trans(wk, base_y, &ts) == 0) {
        return;
    }

    store_trans_rgb_tiles(&(RgbTileRun) { mt, wk, ts.textbl, ts.trsptr, ts.count, ts.flip, ts.palo, ts.group });

    end_obj_trans(mt, wk);
}

void mlt_obj_matrix(WORK* wk, s32 base_y) {
    njSetMatrix(NULL, &BgMATRIX[wk->my_family]);
    njTranslate(NULL, wk->position_x, wk->position_y + base_y, PrioBase[wk->position_z]);

    if (wk->my_mr_flag) {
        njScale(NULL, (1.0f / 64.0f) * (wk->my_mr.size.x + 1), (1.0f / 64.0f) * (wk->my_mr.size.y + 1), 1.0f);
    }
}

// The tail of get_mltbuf16's scan: take the slot the scan set aside, or hang if
// it found none. It returns the slot rather than writing it, because the other
// exit never comes back.
void draw_box(const BoxRect* rect, u32 col, u32 attr, s16 prio) {
    f32 px;
    f32 py;
    f32 sx;
    f32 sy;
    Vec3 point[2];
    PAL_CURSOR line;
    PAL_CURSOR_P xy[4];
    PAL_CURSOR_COL cc[4];

    px = rect->arg0;
    py = rect->arg1;
    sx = rect->arg2;
    sy = rect->arg3;
    point[0].x = px;
    point[0].y = py;
    point[0].z = 0.0f;
    point[1].x = px + sx;
    point[1].y = py + sy;
    point[1].z = 0.0f;
    njCalcPoints(NULL, point, point, 2);
    line.p = xy;
    line.col = cc;
    line.tex = NULL;
    line.num = 4;
    line.p[0].x = line.p[2].x = point[0].x;
    line.p[1].x = line.p[3].x = point[1].x;
    line.p[0].y = line.p[1].y = point[0].y;
    line.p[2].y = line.p[3].y = point[1].y;
    line.col[0].color = line.col[1].color = line.col[2].color = line.col[3].color = col;
    njDrawPolygon2D(&line, 4, PrioBase[prio], attr);
    appRenewTempPriority(prio);
}

// One tile of a melt pass: the texture it comes from, where it lands, and the
// group range whose other references to it have to be rewritten. These are
// reload_melt16_tile's ten arguments, in order.
typedef struct {
    MultiTexture* mt;
    TEX* texptr;
    s32 size;
    s32 dd;
    TileMapEntry* trsptr;
    void* trans_table;
    s32 group_index;
    s32 group_count;
    s32 palt;
    s32 code;
} MeltTile;

static s32 reload_melt16_tile(const MeltTile* tile) {
    MultiTexture* mt = tile->mt;
    TileMapEntry* trsptr = tile->trsptr;
    s32 palt = tile->palt;
    s32 code = tile->code;
    s32 attr;

    lz_ext_p6_fx(&((u8*)tile->texptr)[1], mt->mltbuf, tile->size);
    njReLoadTexturePartNumG(mt->mltgidx16 + (code >> 8), (s8*)mt->mltbuf, code & 0xFF, tile->size);
    attr = (trsptr->attr & 0xC000) | 0x1000 | tile->dd;
    trsptr->attr |= 0x1000;
    attr |= palt;
    search_trsptr(&(TransSwap) {
        tile->trans_table, tile->group_index, tile->group_count, trsptr->code, palt, code, attr });
    trsptr->code = code;
    trsptr->attr = attr;
    code += 1;

    return code;
}

void mlt_obj_melt2(MultiTexture* mt, u16 cg_number) {
    u32* textbl;
    u16* trsbas;
    TileMapEntry* trsptr;
    TEX* texptr;
    TEX_GRP_LD* grplds;
    s32 count;
    s32 n;
    s32 i;
    s32 cd16;
    s32 cd32;
    s32 size;
    s32 attr;
    s32 palt;
    s32 wh;
    s32 dd;

    ppgSetupCurrentDataList(&mt->texList);
    grplds = &texgrplds[obj_group_table[cg_number]];

    require_valid_trans_group(obj_group_table[cg_number]);

    n = *(u32*)grplds->trans_table / 4;
    textbl = (u32*)grplds->texture_table;
    cd16 = 0;
    cd32 = 0;

    for (i = 0; i < n; i++) {
        trsbas = (u16*)(grplds->trans_table + ((u32*)grplds->trans_table)[i]);
        count = *trsbas;
        trsbas++;
        trsptr = (TileMapEntry*)trsbas;

        while (count != 0) {
            attr = trsptr->attr;

            if (attr & 0x1000) {
                count -= 1;
                trsptr++;
                continue;
            }

            texptr = (TEX*)((uintptr_t)textbl + ((u32*)textbl)[trsptr->code]);
            dd = (((texptr->wh & 0xE0) << 5) - 0x400) | (((texptr->wh & 0x1C) << 6) - 0x100);
            wh = (texptr->wh & 3) + 1;
            size = (wh * wh) << 6;
            palt = attr & 3;

            switch (wh) {
            case 1:
            case 2:
                cd16 = reload_melt16_tile(&(MeltTile) {
                    mt, texptr, size, dd, trsptr, grplds->trans_table, i, n, palt, cd16 });
                break;

            case 4:
                lz_ext_p6_fx(&((u8*)texptr)[1], mt->mltbuf, size);
                njReLoadTexturePartNumG(mt->mltgidx32 + (cd32 >> 6), (s8*)mt->mltbuf, cd32 & 0x3F, size);
                attr = (attr & 0xC000) | 0x3000 | dd;
                trsptr->attr |= 0x1000;
                attr |= palt;
                search_trsptr(&(TransSwap) { grplds->trans_table, i, n, trsptr->code, palt, cd32, attr });
                trsptr->code = cd32;
                trsptr->attr = attr;
                cd32 += 1;
                break;
            }

            count -= 1;
            trsptr++;
        }
    }

    ppgRenewTexChunkSeqs(NULL);
}
