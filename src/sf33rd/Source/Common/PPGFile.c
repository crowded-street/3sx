#include "sf33rd/Source/Common/PPGFile.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/ps2/flps2asm.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Compress/Lz77/Lz77Dec.h"
#include "sf33rd/Source/Compress/zlibApp.h"
#include "sf33rd/Source/PS2/ps2Quad.h"
#include "structs.h"

#define MAGIC_TO_INT(str) ((str[0] << 0x18) | (str[1] << 0x10) | (str[2] << 0x8) | (str[3]))
#define REVERT_U32(val)                                                                                                \
    (((val & 0xFF) << 0x18) | ((val & 0xFF00) << 8) | ((val >> 8) & 0xFF00) | ((val >> 0x18) & 0xFF))
#define REVERT_U16(val) (((val >> 8) & 0xFF) | ((val & 0xFF) << 8))
#define REVERT_U8(val) (((val << 4) & 0xF0) | ((val >> 4) & 0xF))

#define CODE_0(val) ((val & 0xF0) << 8) + ((val & 0xF) << 4)
#define CODE_1(val) ((val & 0x38) << 0xA) + ((val & 7) << 5)

typedef struct {
    // total size: 0x34
    PPGDataList* cur; // offset 0x0, size 0x4
    u16 hanPal;       // offset 0x4, size 0x2
    u16 hanTex;       // offset 0x6, size 0x2
    _MEMMAN_OBJ mm;   // offset 0x8, size 0x2C
} PPG_W;

typedef struct {
    Vec3 v;
    TexCoord t;
} _Vertex;

const u8 pplColorModeWidth[4] = { 0xF, 0x3F, 0xFF, 0 };

PPG_W ppg_w;
s16* dctex_linear;

s32 ppgCheckPaletteDataBe(Palette* pch);
void ppgWriteQuadOnly(Vertex* pos, u32 col, u32 texCode);
void ppgWriteQuadOnly2(Vertex* pos, u32 col, u32 texCode);
void ppgChangeDataEndian(u8* adrs, s32 size, s32 dendL, s32 col4, s32 depth, s32 excdot);
void ppgSetupContextFromPPL(PPLFileHeader* ppl, plContext* bits);
void ppgSetupContextFromPPG(PPGFileHeader* ppg, plContext* bits);

void ppg_Initialize(void* lcmAdrs, s32 lcmSize) {
    if (lcmAdrs == NULL) {
        while (1) {}
    }

    mmHeapInitialize(&ppg_w.mm, lcmAdrs, lcmSize, ALIGN_UP(sizeof(_MEMMAN_CELL), 16), "- for PPG -");
}

void* ppgMallocF(s32 size) {
    return mmAlloc(&ppg_w.mm, size, 0);
}

void* ppgMallocR(s32 size) {
    return mmAlloc(&ppg_w.mm, size, 1);
}

void ppgFree(void* adrs) {
    mmFree(&ppg_w.mm, adrs);
}

void* ppgPullDecBuff(s32 size) {
    return ppgMallocR(size);
}

void ppgPushDecBuff(void* adrs) {
    ppgFree(adrs);
}

void ppgTexSrcDataReleased(Texture* tex) {
    if (tex == NULL) {
        tex = ppg_w.cur->tex;
    }

    tex->srcAdrs = NULL;
    tex->srcSize = 0;
    ppgCheckTextureDataBe(tex);
}

void ppgPalSrcDataReleased(Palette* pal) {
    if (pal == NULL) {
        pal = ppg_w.cur->pal;
    }

    pal->srcAdrs = NULL;
    pal->srcSize = 0;
    ppgCheckPaletteDataBe(pal);
}

void ppgSourceDataReleased(PPGDataList* dlist) {
    if (dlist == NULL) {
        dlist = ppg_w.cur;
    }

    if (dlist->tex != NULL) {
        ppgTexSrcDataReleased(dlist->tex);
    }

    if (dlist->pal != NULL) {
        ppgPalSrcDataReleased(dlist->pal);
    }
}

void ppgSetupCurrentDataList(PPGDataList* dlist) {
    ppg_w.cur = dlist;
}

void ppgSetupCurrentPaletteNumber(Palette* pal, s32 num) {
    if (pal == NULL) {
        pal = ppg_w.cur->pal;

        if (pal == NULL) {
            return;
        }
    }

    if (num < pal->total) {
        ppg_w.hanPal = pal->handle[num];
    }
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
        prm.v[i] = ((_Vertex*)pos)[i].v;
        prm.t[i] = ((_Vertex*)pos)[i].t;
    }

    ps2SeqsRenderQuad_A(&prm, col);
}

void ppgWriteQuadOnly2(Vertex* pos, u32 col, u32 texCode) {
    Sprite prm;

    flSetRenderState(FLRENDER_TEXSTAGE0, texCode);

    prm.v[0] = ((_Vertex*)pos)[0].v;
    prm.t[0] = ((_Vertex*)pos)[0].t;
    prm.v[3] = ((_Vertex*)pos)[3].v;
    prm.t[3] = ((_Vertex*)pos)[3].t;

    ps2SeqsRenderQuad_A2(&prm, col);
}

s32 ppgWriteQuadWithST_B(Vertex* pos, u32 col, PPGDataList* tb, s32 tix, s32 cix) {
    u16 texhan;
    u16 palhan = 0;

    if (tb == NULL) {
        tb = ppg_w.cur;

        if (tb == NULL) {
            return ppgWriteQuadWithST_A(pos, col);
        }
    }

    if (tix < 0) {
        texhan = ppg_w.hanTex;
    } else {
        texhan = tb->tex->handle[tix - tb->tex->ixNum1st].b16[0];

        if (texhan == 0) {
            return 0;
        }
    }

    if (tb->tex->handle[tix - tb->tex->ixNum1st].b16[1] & 0x4000) {
        if (cix < 0) {
            palhan = ppg_w.hanPal;
        } else {
            palhan = tb->pal->handle[cix];
        }
    }

    ppgWriteQuadOnly(pos, col, texhan | (palhan << 0x10));
    return 1;
}

s32 ppgWriteQuadWithST_B2(Vertex* pos, u32 col, PPGDataList* tb, s32 tix, s32 cix) {
    u16 texhan;
    u16 palhan = 0;

    if (tb == NULL) {
        tb = ppg_w.cur;

        if (tb == NULL) {
            return ppgWriteQuadWithST_A2(pos, col);
        }
    }

    if (tix < 0) {
        texhan = ppg_w.hanTex;
    } else {
        texhan = tb->tex->handle[tix - tb->tex->ixNum1st].b16[0];

        if (texhan == 0) {
            return 0;
        }
    }

    if (tb->tex->handle[tix - tb->tex->ixNum1st].b16[1] & 0x4000) {
        if (cix < 0) {
            palhan = ppg_w.hanPal;
        } else {
            palhan = tb->pal->handle[cix];
        }
    }

    ppgWriteQuadOnly2(pos, col, texhan | (palhan << 16));
    return 1;
}

s32 ppgWriteQuadUseTrans(Vertex* pos, u32 col, PPGDataList* tb, s32 tix, s32 cix, s32 flip, s32 pal) {
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

    if ((pos[0].x >= 384.0f) || (pos[3].x < 0.0f) || (pos[0].y >= 224.0f) || (pos[3].y < 0.0f)) {
        return 0;
    }

    if (tb == NULL) {
        tb = ppg_w.cur;

        if (tb == NULL) {
            return ppgWriteQuadWithST_A2(pos, col);
        }
    }

    texhan = tb->tex->handle[tix - tb->tex->ixNum1st].b16[0];
    ix_ofs = tb->tex->handle[tix - tb->tex->ixNum1st].b16[1];

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
            tran = (u8*)&ppg[1];
            ppgwf = ppg->width;
            ppgw = ppg->width;
            ppghf = ppg->height;
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

#if !defined(TARGET_PS2)
            sadd = 0;
            tadd = 0;
#endif

            qvtx[0].z = pos[0].z;
            qvtx[3].z = pos[3].z;

            for (i = 0; i < transTotal; i++) {
                if (ix_ofs & 0x4000) {
                    palhan = phan[*tran + pal];
                }

                tran++;
                iPoint = *tran++;
                cofsXY = *tran++;
                xs = (cofsXY >> 4) + 1;
                ys = (cofsXY & 0xF) + 1;
                sx = iPoint % ppgw;
                sy = iPoint / ppgw;

                if (flip & 1) {
                    qvtx[3].x = pos->x + (pxs * (ppgw - sx) / ppgwf);
                    qvtx[0].x = pos->x + (pxs * (ppgw - (sx + xs)) / ppgwf);
                } else {
                    qvtx[0].x = pos->x + (sx * pxs / ppgwf);
                    qvtx[3].x = pos->x + (pxs * (sx + xs) / ppgwf);
                }

                if (flip & 2) {
                    qvtx[3].y = pos->y + (pys * (ppgw - sy) / ppghf);
                    qvtx[0].y = pos->y + (pys * (ppgw - (sy + ys)) / ppghf);
                } else {
                    qvtx[0].y = pos->y + (sy * pys / ppghf);
                    qvtx[3].y = pos->y + (pys * (sy + ys) / ppghf);
                }

                if ((qvtx[0].x < 384.0f) && (qvtx[3].x >= 0.0f) && (qvtx[0].y < 224.0f) && (qvtx[3].y >= 0.0f)) {
                    if (flip & 1) {
                        qvtx[3].s = (sx / ppgwf) - sadd;
                        qvtx[0].s = ((sx + xs) / ppgwf) - sadd;
                    } else {
                        qvtx[0].s = sadd + (sx / ppgwf);
                        qvtx[3].s = sadd + ((sx + xs) / ppgwf);
                    }

                    if (flip & 2) {
                        qvtx[3].t = (sy / ppghf) - tadd;
                        qvtx[0].t = ((sy + ys) / ppghf) - tadd;
                    } else {
                        qvtx[0].t = tadd + (sy / ppghf);
                        qvtx[3].t = tadd + ((sy + ys) / ppghf);
                    }

                    ppgWriteQuadOnly2(qvtx, col, texhan | (palhan << 0x10));
                }
            }

            return 1;
        }
    }

    if (ix_ofs & 0x4000) {
        if (cix < 0) {
            palhan = ppg_w.hanPal;
        } else {
            palhan = phan[cix];
        }
    }

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

    ppgWriteQuadOnly2(pos, col, texhan | (palhan << 0x10));
    return 1;
}

ssize_t ppgDecompress(s32 koCmpr, void* srcAdrs, s32 srcSize, void* dstAdrs, s32 dstSize) {
    u8* src;
    u8* dst;
    s32 i;
    ssize_t rnum = 0;

    switch (koCmpr) {
    default:
        if (srcAdrs != dstAdrs) {
            src = srcAdrs;
            dst = dstAdrs;

            for (i = 0; i < dstSize; i++) {
                *dst++ = *src++;
            }
        }

        rnum = srcSize;
        break;

    case 1:
        rnum = decLZ77withSizeCheck(srcAdrs, dstAdrs, dstSize);
        rnum *= dstSize;
        break;

    case 2:
        rnum = zlib_Decompress(srcAdrs, srcSize, dstAdrs, dstSize);
        break;
    }

    return rnum;
}

s32 ppgSetupCmpChunk(u8* srcAdrs, s32 num, u8* dstAdrs) {
    PPXFileHeader* ppx;
    void* cmpAdrs;
    s32 cmpSize;
    s32 mltSize;
    s32 koCmpr;
    s32 ofs;

    ofs = 0;

    while (1) {
        ppx = (PPXFileHeader*)(srcAdrs + ofs);

        if (MAGIC_TO_INT("pEND") == REVERT_U32(ppx->magic)) {
            return -1;
        }

        if (MAGIC_TO_INT("pCMP") != REVERT_U32(ppx->magic)) {
            ofs += (REVERT_U32(ppx->fileSize) + 3) & ~3;
            continue;
        }

        if (num > 0) {
            num -= 1;
            ofs += (REVERT_U32(ppx->fileSize) + 3) & ~3;
            continue;
        }

        break;
    }

    mltSize = REVERT_U32(ppx->expSize);
    cmpSize = REVERT_U32(ppx->fileSize) - 0x10;
    cmpAdrs = ppx + 1;
    koCmpr = ppx->compress & 3;

    if (mltSize != ppgDecompress(koCmpr, cmpAdrs, cmpSize, dstAdrs, mltSize)) {
        flLogOut("圧縮データの解凍に失敗しました。\n"); // Failed to decompress the compressed data.
        while (1) {}
    }

    return 1;
}

s32 ppgSetupPalChunk(Palette* pch, u8* adrs, s32 size, s32 ixNum1st, s32 num, s32 /* unused */) {
    PPLFileHeader* ppl;
    plContext bits;
    s32 i;
    s32 col_items;
    s32 koCmpr;
    s32 cmpSize;
    s32 mltSize;
    void* cmpAdrs;
    void* mltAdrs;
    u32 ofs = 0;

    if (pch == NULL) {
        pch = ppg_w.cur->pal;
    }

    if (pch->be) {
        while (1) {}
    }

    pch->be = 0;
    pch->ixNum1st = ixNum1st;
    pch->srcAdrs = adrs;
    pch->srcSize = size;
    pch->handle = NULL;
    mltAdrs = NULL;
    koCmpr = 0;

    while (1) {
        ppl = (PPLFileHeader*)(adrs + ofs);

        if (MAGIC_TO_INT("pEND") == REVERT_U32(ppl->magic)) {
            return -1;
        }

        if (MAGIC_TO_INT("pPAL") != REVERT_U32(ppl->magic)) {
            ofs += (REVERT_U32(ppl->fileSize) + 3) & ~3;
            continue;
        }

        if (num > 0) {
            num -= 1;
            ofs += (REVERT_U32(ppl->fileSize) + 3) & ~3;
            continue;
        }

        break;
    }

    cmpSize = REVERT_U32(ppl->fileSize) - 16;
    cmpAdrs = ppl + 1;
    pch->c_mode = ppl->c_mode & 3;
    pch->total = REVERT_U16(ppl->palettes);
    col_items = pplColorModeWidth[pch->c_mode] + 1;
    koCmpr = ppl->compress & 3;
    ppgSetupContextFromPPL(ppl, &bits);
    pch->handle = ppgMallocF(pch->total * 2);

    if (pch->handle != NULL) {
        for (i = 0; i < pch->total; i++) {
            pch->handle[i] = 0;
        }

        mltSize = bits.bitdepth * (pch->total * col_items);

        if (koCmpr != 0) {
            mltAdrs = ppgPullDecBuff(mltSize);
        } else {
            mltAdrs = cmpAdrs;
        }

        if (mltAdrs == NULL) {
            // Failed to allocate palette data decompression area.
            flLogOut("パレットデータ解凍領域の確保に失敗しました。\n");
            goto error_handler;
        }

        if (mltSize != ppgDecompress(koCmpr, cmpAdrs, cmpSize, mltAdrs, mltSize)) {
            flLogOut("パレットデータの解凍に失敗しました。\n"); // Failed to decompress the palette data.
            ppgPushDecBuff(mltAdrs);
            goto error_handler;
        }

        ppgChangeDataEndian(mltAdrs, mltSize, ppl->c_mode & 4, ppl->formARGB == 0x8888, bits.bitdepth, 0);

        if (koCmpr == 0) {
            ppl->c_mode |= 4;
        }

        bits.ptr = mltAdrs;

        for (i = 0; i < pch->total; i++) {
            pch->handle[i] = flCreatePaletteHandle(&bits, 0);

            if (pch->handle[i] == 0) {
                flLogOut("パレットハンドルの取得に失敗しました。\n"); // Failed to acquire palette handle.

                if (koCmpr == 0) {
                    goto error_handler;
                }

                ppgPushDecBuff(mltAdrs);
                goto error_handler;
            }

            bits.ptr = (u8*)bits.ptr + (col_items * bits.bitdepth);
        }

        if (koCmpr != 0) {
            ppgPushDecBuff(mltAdrs);
        }
        pch->be = 1;
        return 1;
    }

error_handler:
    if (pch->handle != NULL) {
        for (i = 0; i < pch->total; i++) {
            if (pch->handle[i]) {
                flReleasePaletteHandle(pch->handle[i]);
            }
        }

        ppgFree(pch->handle);
    }

    if ((koCmpr != 0) && (mltAdrs != NULL)) {
        ppgPushDecBuff(mltAdrs);
    }

    pch->handle = NULL;
    while (1) {}
}

s32 ppgSetupPalChunkDir(Palette* pch, PPLFileHeader* ppl, u8* adrs, s32 ixNum1st, s32 /* unused */) {
    plContext bits;
    s32 i;

    if (pch == NULL) {
        pch = ppg_w.cur->pal;
    }

    if (pch->be) {
        while (1) {}
    }

    pch->be = 0;
    pch->ixNum1st = ixNum1st;
    pch->srcAdrs = NULL;
    pch->c_mode = ppl->c_mode & 3;
    ppgSetupContextFromPPL(ppl, &bits);
    pch->srcSize = bits.pitch * bits.height;
    pch->total = REVERT_U16(ppl->palettes);
    pch->handle = ppgMallocF(pch->total * 2);

    if (pch->handle != NULL) {
        for (i = 0; i < pch->total; i++) {
            pch->handle[i] = 0;
        }

        ppgChangeDataEndian(
            adrs, pch->total * (bits.pitch * bits.height), ppl->c_mode & 4, ppl->formARGB == 0x8888, bits.bitdepth, 0);
        ppl->c_mode |= 4;

        for (i = 0; i < pch->total; i++) {
            bits.ptr = adrs;
            pch->handle[i] = flCreatePaletteHandle(&bits, 0);

            if (pch->handle[i] == 0) {
                goto error_handler;
            }

            adrs = &adrs[pch->srcSize];
        }

        pch->be = 1;
        return 1;
    }

error_handler:
    if (pch->handle != NULL) {
        for (i = 0; i < pch->total; i++) {
            if (pch->handle[i]) {
                flReleasePaletteHandle(pch->handle[i]);
            }
        }

        ppgFree(pch->handle);
    }

    pch->handle = NULL;
    flLogOut("パレットハンドルの取得に失敗しました。( dir )\n"); // Failed to acquire palette handle. (dir)
    while (1) {}
}

void ppgChangeDataEndian(u8* adrs, s32 size, s32 dendL, s32 col4, s32 depth, s32 excdot) {
    s32 i;
    u32* c4;
    u16* c2;

    if (depth == 1) {
        return;
    }

    if (depth != 0) {
        if (dendL == 0) {
            if (col4 != 0) {
                c4 = (u32*)adrs;

                for (i = 0; i < size / 4; i++) {
                    c4[i] = REVERT_U32(c4[i]);
                }
            } else {
                c2 = (u16*)adrs;

                for (i = 0; i < size / 2; i++) {
                    c2[i] = REVERT_U16(c2[i]);
                }
            }
        }

        return;
    }

    if (excdot != 0) {
        for (i = 0; i < size; i++) {
            adrs[i] = REVERT_U8(adrs[i]);
        }
    }
}

s32 ppgSetupTexChunkSeqs(Texture* tch, PPGFileHeader* ppg, u8* adrs, s32 ixNum1st, s32 ixNums, u32 attribute) {
    plContext bits;
    s32 i;
    s32 ci_flag = 0;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch->be) {
        while (1) {}
    }

    tch->be = 0;
    tch->textures = ixNums;
    tch->accnum = ixNums;
    tch->ixNum1st = ixNum1st;
    tch->total = ixNums;
    tch->flags = 0x80;
    tch->arCnt = 0;
    tch->arInit = 0;
    tch->handle = NULL;
    tch->offset = NULL;
    tch->srcAdrs = NULL;
    tch->srcSize = 0;
    tch->handle = ppgMallocF(ixNums * 4);

    if (tch->handle == NULL) {
        flLogOut("テクスチャハンドル記憶領域が確保できませんでした。\n"); // Failed to allocate texture handle memory.
        while (1) {}
    }

    for (i = 0; i < ixNums; i++) {
        tch->handle[i].b16[0] = 0;
        tch->handle[i].b16[1] = 0x8000;
    }

    ppgSetupContextFromPPG(ppg, &bits);
    tch->srcAdrs = adrs;
    tch->srcSize = bits.pitch * bits.height;

    for (i = 0; i < tch->srcSize * ixNums; i++) {
        adrs[i] = 0;
    }

    if (bits.bitdepth < 2) {
        ci_flag = 0x4000;
    }

    for (i = 0; i < ixNums; i++) {
        bits.ptr = adrs;
        tch->handle[i].b16[1] = ci_flag;
        tch->handle[i].b16[0] = flCreateTextureHandle(&bits, attribute);

        if (tch->handle[i].b16[0] == 0) {
            goto error_handler;
        }

        adrs += tch->srcSize;
    }

    tch->be = 1;
    return 1;

error_handler:
    for (i = 0; i < ixNums; i++) {
        if (tch->handle[i].b16[0]) {
            flReleaseTextureHandle(tch->handle[i].b16[0]);
        }
    }

    ppgFree(tch->handle);
    tch->handle = NULL;
    flLogOut("スプライト用テクスチャハンドルの取得に失敗しました。\n"); // Failed to acquire sprite texture handle.
    while (1) {}
}

void ppgRenewDotDataSeqs(Texture* tch, u32 gix, u32* srcRam, u32 code, u32 size) {
    s32 ix;
    s32 i;
    s32 j;
    u16* dstRam16;
    u16* srcRam16;
    u16* tix;
    u8* dstRam8;
    u8* srcRam8;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch->be != 0) {
        ix = gix - tch->ixNum1st;

        if ((ix < 0) || (ix >= tch->total)) {
            return;
        }

        if (tch->handle[ix].b16[0] != 0) {
            tch->handle[ix].b16[1] |= 0x2000;

            switch (size) {
            case 0x40:
                srcRam8 = (u8*)srcRam;
                dstRam8 = (u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_0(code));

                for (i = 0; i < 8; i++) {
                    for (j = 0; j < 8; j++) {
                        *dstRam8++ = srcRam8[dctex_linear[j + (i << 5)]];
                    }

                    dstRam8 += 0xF8;
                }

                break;

            case 0x100:
                srcRam8 = (u8*)srcRam;
                dstRam8 = (u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_0(code));

                for (i = 0; i < 0x10; i++) {
                    for (j = 0; j < 0x10; j++) {
                        *dstRam8++ = srcRam8[dctex_linear[j + (i << 5)]];
                    }

                    dstRam8 += 0xF0;
                }

                break;

            case 0x400:
                srcRam8 = (u8*)srcRam;
                dstRam8 = (u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_1(code));
                tix = (u16*)dctex_linear;

                for (i = 0; i < 0x20; i++) {
                    for (j = 0; j < 0x20; j++) {
                        *dstRam8++ = srcRam8[*tix++];
                    }

                    dstRam8 += 0xE0;
                }

                break;

            case 0x80:
                srcRam16 = (u16*)srcRam;
                dstRam16 = (u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_0(code)) * 2);

                for (i = 0; i < 8; i++) {
                    for (j = 0; j < 8; j++) {
                        *dstRam16++ = srcRam16[dctex_linear[j + (i << 5)]];
                    }

                    dstRam16 += 0xF8;
                }

                break;

            case 0x200:
                srcRam16 = (u16*)srcRam;
                dstRam16 = (u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_0(code)) * 2);

                for (i = 0; i < 0x10; i++) {
                    for (j = 0; j < 0x10; j++) {
                        *dstRam16++ = srcRam16[dctex_linear[j + (i << 5)]];
                    }

                    dstRam16 += 0xF0;
                }

                break;

            case 0x800:
                srcRam16 = (u16*)srcRam;
                dstRam16 = (u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_1(code)) * 2);
                tix = (u16*)dctex_linear;

                for (i = 0; i < 0x20; i++) {
                    for (j = 0; j < 0x20; j++) {
                        *dstRam16++ = srcRam16[*tix++];
                    }

                    dstRam16 += 0xE0;
                }

                break;
            }
        }
    }
}

void ppgMakeConvTableTexDC() {
    s16 seed[32] = {
        0x0000, 0x0002, 0x0008, 0x000A, 0x0020, 0x0022, 0x0028, 0x002A, 0x0080, 0x0082, 0x0088,
        0x008A, 0x00A0, 0x00A2, 0x00A8, 0x00AA, 0x0200, 0x0202, 0x0208, 0x020A, 0x0220, 0x0222,
        0x0228, 0x022A, 0x0280, 0x0282, 0x0288, 0x028A, 0x02A0, 0x02A2, 0x02A8, 0x02AA,
    };

    s16 seedAdd[16] = {
        0x0000, 0x0004, 0x0010, 0x0014, 0x0040, 0x0044, 0x0050, 0x0054,
        0x0100, 0x0104, 0x0110, 0x0114, 0x0140, 0x0144, 0x0150, 0x0154,
    };

    s32 i;
    s32 j;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 32; j++) {
            dctex_linear[j + i * 64] = seed[j] + seedAdd[i];
        }

        for (j = 0; j < 32; j++) {
            dctex_linear[j + (i * 64 + 32)] = dctex_linear[j + i * 64] + 1;
        }
    }
}

s32 ppgRenewTexChunkSeqs(Texture* tch) {
    plContext bits;
    s32 i;
    s32* srcRam;
    s32* dstRam;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;

        if (tch == NULL) {
            return 0;
        }
    }

    if (tch->be == 0) {
        return 0;
    }

    for (i = 0; i < tch->total; i++) {
        if (tch->handle[i].b16[1] & 0x2000) {
            tch->handle[i].b16[1] &= 0xDFFF;
            flLockTexture(NULL, tch->handle[i].b16[0], &bits, 3);
            dstRam = bits.ptr;
            srcRam = (s32*)(tch->srcAdrs + tch->srcSize * i);
            flPS2_Mem_move64(srcRam, dstRam, tch->srcSize >> 6);
            flUnlockTexture(tch->handle[i].b16[0]);
        }
    }

    return 1;
}

s32 ppgSetupTexChunk_1st(Texture* tch, u8* adrs, ssize_t size, s32 ixNum1st, s32 ixNums, s32 ar, s32 arcnt) {
    PPGFileHeader* ppg;
    s32 i;
    s32 ofs;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch->be) {
        while (1) {}
    }

    tch->be = 0;
    tch->textures = 0;
    tch->accnum = 0;
    tch->ixNum1st = ixNum1st;
    tch->total = ixNums;
    tch->flags = ar != 0;
    tch->arCnt = 0;
    tch->arInit = arcnt;
    tch->offset = NULL;
    tch->srcAdrs = adrs;
    tch->srcSize = size;
    tch->handle = (TextureHandle*)ppgMallocF(ixNums * 4);

    if (tch->handle == NULL) {
        flLogOut("テクスチャハンドル記憶領域が確保できませんでした。\n"); // Failed to allocate texture handle memory.
        goto error_handler;
    }

    for (i = 0; i < ixNums; i++) {
        tch->handle[i].b16[0] = 0;
        tch->handle[i].b16[1] = 0x8000;
    }

    ofs = 0;

    while (1) {
        ppg = (PPGFileHeader*)(tch->srcAdrs + ofs);

        if (MAGIC_TO_INT("pEND") != REVERT_U32(ppg->magic)) {
            if (MAGIC_TO_INT("pTEX") == REVERT_U32(ppg->magic)) {
                tch->textures += 1;
            }

            ofs += (REVERT_U32(ppg->fileSize) + 3) & ~3;
        } else {
            break;
        }
    }

    if (tch->textures == 0) {
        flLogOut("テクスチャデータが見つかりませんでした。\n"); // Texture data was not found.
        goto error_handler;
    }

    tch->offset = ppgMallocF(tch->textures * 4);

    if (tch->offset == NULL) {
        // Failed to allocate memory for the texture data offset table.
        flLogOut("テクスチャデータオフセットテーブルの記憶領域が確保できませんでした。\n");
        goto error_handler;
    }

    ofs = 0;

    while (1) {
        ppg = (PPGFileHeader*)(tch->srcAdrs + ofs);

        if (MAGIC_TO_INT("pEND") != REVERT_U32(ppg->magic)) {
            if (MAGIC_TO_INT("pTEX") == REVERT_U32(ppg->magic)) {
                tch->offset[tch->accnum++] = ofs;
            }

            ofs += (REVERT_U32(ppg->fileSize) + 3) & ~3;
        } else {
            break;
        }
    }

    tch->accnum = 0;
    tch->be = 1;
    return 1;

error_handler:
    if (tch->handle != NULL) {
        ppgFree(tch->handle);
    }

    if (tch->offset != NULL) {
        ppgFree(tch->offset);
    }

    tch->handle = NULL;
    tch->offset = NULL;
    while (1) {}
}

s32 ppgSetupTexChunk_1st_Accnum(Texture* tch, u16 accnum) {
    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    tch->accnum = accnum;
    return 0;
}

s32 ppgSetupTexChunk_2nd(Texture* tch, s32 ixNum) {
    PPGFileHeader* ppg;
    TextureHandle* hnof;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch->textures <= tch->accnum) {
        // Handle acquisition process has been called more times than the number of data stored in the texture chunk.
        flLogOut("ハンドル取得処理がテクスチャチャンクに格納されているデータ数以上に呼ばれました。\n");
        while (1) {}
    }

    hnof = tch->handle + (ixNum - tch->ixNum1st);
    hnof->b16[1] = tch->accnum++;

    if (tch->srcAdrs == NULL) {
        // Texture chunk data has already been lost.
        flLogOut("テクスチャチャンクデータが既に失われています。\n");
        while (1) {}
    }

    ppg = (PPGFileHeader*)(tch->srcAdrs + tch->offset[hnof->b16[1]]);

    if ((ppg->pixel & 3) < 2) {
        hnof->b16[1] |= 0x4000;
    }

    return tch->accnum;
}

s32 ppgSetupTexChunk_3rd(Texture* tch, s32 ixNum, u32 attribute) {
    plContext bits;
    PPGFileHeader* ppg;
    TextureHandle* hnof;
    s32 koCmpr;
    s32 cmpSize;
    s32 mltSize;
    void* cmpAdrs;
    void* mltAdrs;

    s32 unused_s5;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch->flags & 1) {
        tch->arCnt = tch->arInit;
    }

    hnof = tch->handle + (ixNum - tch->ixNum1st);

    if (hnof->b16[0]) {
        return 1;
    }

    if (tch->srcAdrs == NULL) {
        // Texture chunk data has already been lost.
        flLogOut("テクスチャチャンクデータが既に失われています。\n");
        while (1) {}
    }

    ppg = (PPGFileHeader*)(tch->srcAdrs + (tch->offset[hnof->b16[1] & 0xFFF]));
    ppgSetupContextFromPPG(ppg, &bits);
    koCmpr = ppg->compress & 3;
    cmpSize = (u16)REVERT_U16(ppg->transNums) * 3 + 0x10;
    cmpAdrs = (u8*)ppg + cmpSize;
    cmpSize = REVERT_U32(ppg->fileSize) - cmpSize;
    mltSize = bits.height * bits.pitch;
    mltAdrs = ppgPullDecBuff(mltSize);

    if (mltAdrs == NULL) {
        // Failed to allocate texture data expansion area.
        flLogOut("テクスチャデータ展開領域が確保できませんでした。\n");
        while (1) {}
    }

    if (mltSize != ppgDecompress(koCmpr, cmpAdrs, cmpSize, mltAdrs, mltSize)) {
        // Failed to acquire sprite texture handle.
        flLogOut("テクスチャデータの解凍に失敗しました。\n");
        ppgPushDecBuff(mltAdrs);
        while (1) {}
    }

    unused_s5 = 0;
    ppgChangeDataEndian(mltAdrs, mltSize, ppg->pixel & 4, ppg->formARGB == 0x8888, bits.bitdepth, unused_s5);
    bits.ptr = mltAdrs;
    hnof->b16[0] = flCreateTextureHandle(&bits, attribute);
    ppgPushDecBuff(mltAdrs);

    if (hnof->b16[0] == 0) {
        // Failed to acquire texture handle.
        flLogOut("テクスチャハンドルの取得に失敗しました。\n");
        while (1) {}
    }

    return 1;
}

void ppgSetupContextFromPPL(PPLFileHeader* ppl, plContext* bits) {
    bits->desc = 0;
    bits->width = pplColorModeWidth[ppl->c_mode & 3] < 17 ? 16 : 256;
    bits->height = 1;
    bits->bitdepth = ppl->formARGB != 0x8888 ? 2 : 4;
    bits->pitch = bits->width * bits->bitdepth;
    bits->ptr = NULL;

    switch ((u16)REVERT_U16(ppl->formARGB)) {
    case 0x1555:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xA;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 5;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x1F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 1;
        bits->pixelformat.as = 0xF;
        bits->pixelformat.am = 1;
        break;

    case 0x565:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xB;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 6;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x3F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 0;
        bits->pixelformat.as = 0;
        bits->pixelformat.am = 0;
        break;

    case 0x4444:
        bits->pixelformat.rl = 4;
        bits->pixelformat.rs = 8;
        bits->pixelformat.rm = 0xF;
        bits->pixelformat.gl = 4;
        bits->pixelformat.gs = 4;
        bits->pixelformat.gm = 0xF;
        bits->pixelformat.bl = 4;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xF;
        bits->pixelformat.al = 4;
        bits->pixelformat.as = 0xC;
        bits->pixelformat.am = 0xF;
        break;

    default:
        bits->pixelformat.rl = 8;
        bits->pixelformat.rs = 0x10;
        bits->pixelformat.rm = 0xFF;
        bits->pixelformat.gl = 8;
        bits->pixelformat.gs = 8;
        bits->pixelformat.gm = 0xFF;
        bits->pixelformat.bl = 8;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xFF;
        bits->pixelformat.al = 8;
        bits->pixelformat.as = 0x18;
        bits->pixelformat.am = 0xFF;
        break;
    }
}

void ppgSetupContextFromPPG(PPGFileHeader* ppg, plContext* bits) {
    bits->desc = 0;
    bits->width = ppg->width * 16;
    bits->height = ppg->height * 16;

    switch (ppg->pixel & 3) {
    case 0:
        if (ppg->pixel & 0x20) {
            bits->desc |= 0x24;
        } else {
            bits->desc |= 0x14;
        }

        bits->bitdepth = 0;
        bits->pitch = bits->width / 2;
        break;

    case 1:
        bits->desc = bits->desc | 4;
        bits->bitdepth = 1;
        bits->pitch = bits->width;
        break;

    case 2:
        bits->bitdepth = 2;
        bits->pitch = bits->width * 2;
        break;

    default:
        bits->bitdepth = 4;
        bits->pitch = bits->width * 4;
        break;
    }

    switch ((u16)REVERT_U16(ppg->formARGB)) {
    case 0x1555:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xA;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 5;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x1F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 1;
        bits->pixelformat.as = 0xF;
        bits->pixelformat.am = 1;
        break;

    case 0x565:
        bits->pixelformat.rl = 5;
        bits->pixelformat.rs = 0xB;
        bits->pixelformat.rm = 0x1F;
        bits->pixelformat.gl = 6;
        bits->pixelformat.gs = 5;
        bits->pixelformat.gm = 0x3F;
        bits->pixelformat.bl = 5;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0x1F;
        bits->pixelformat.al = 0;
        bits->pixelformat.as = 0;
        bits->pixelformat.am = 0;
        break;

    case 0x4444:
        bits->pixelformat.rl = 4;
        bits->pixelformat.rs = 8;
        bits->pixelformat.rm = 0xF;
        bits->pixelformat.gl = 4;
        bits->pixelformat.gs = 4;
        bits->pixelformat.gm = 0xF;
        bits->pixelformat.bl = 4;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xF;
        bits->pixelformat.al = 4;
        bits->pixelformat.as = 0xC;
        bits->pixelformat.am = 0xF;
        break;

    case 0x8888:
        bits->pixelformat.rl = 8;
        bits->pixelformat.rs = 0x10;
        bits->pixelformat.rm = 0xFF;
        bits->pixelformat.gl = 8;
        bits->pixelformat.gs = 8;
        bits->pixelformat.gm = 0xFF;
        bits->pixelformat.bl = 8;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0xFF;
        bits->pixelformat.al = 8;
        bits->pixelformat.as = 0x18;
        bits->pixelformat.am = 0xFF;
        break;

    default:
        bits->pixelformat.rl = 0;
        bits->pixelformat.rs = 0;
        bits->pixelformat.rm = 0;
        bits->pixelformat.gl = 0;
        bits->pixelformat.gs = 0;
        bits->pixelformat.gm = 0;
        bits->pixelformat.bl = 0;
        bits->pixelformat.bs = 0;
        bits->pixelformat.bm = 0;
        bits->pixelformat.al = 0;
        bits->pixelformat.as = 0;
        bits->pixelformat.am = 0;
        break;
    }
}

s32 ppgReleasePaletteHandle(Palette* pch, s32 ixNum) {
    s32 i;
    s32 ix;
    u16 han;

    if (pch == NULL) {
        pch = ppg_w.cur->pal;
    }

    if (pch == NULL) {
        return 0;
    }

    if (pch->be == 0) {
        return 0;
    }

    if (ixNum < 0) {
        for (i = 0; i < pch->total; i++) {
            han = pch->handle[i];

            if (han) {
                flReleasePaletteHandle(han);
            }

            pch->handle[i] = 0;
        }

    } else {
        ix = ixNum - pch->ixNum1st;

        if ((ix >= 0) && (ix < pch->total)) {
            han = pch->handle[ix];

            if (han) {
                flReleasePaletteHandle(han);
            }

            pch->handle[ix] = 0;
        }
    }

    return ppgCheckPaletteDataBe(pch);
}

s32 ppgReleaseTextureHandle(Texture* tch, s32 ixNum) {
    s32 i;
    s32 ix;
    u16 han;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch == NULL) {
        return 0;
    }

    if (tch->be == 0) {
        return 0;
    }

    if (ixNum < 0) {
        for (i = 0; i < tch->total; i++) {
            han = tch->handle[i].b16[0];

            if (han) {
                flReleaseTextureHandle(han);
            }

            tch->handle[i].b16[0] = 0;

            if (tch->flags & 0x80) {
                tch->handle[i].b16[1] = 0;
            }
        }
    } else {
        ix = ixNum - tch->ixNum1st;

        if ((ix >= 0) && (ix < tch->total)) {
            han = tch->handle[ix].b16[0];

            if (han) {
                flReleaseTextureHandle(han);
            }

            tch->handle[ix].b16[0] = 0;

            if (tch->flags & 0x80) {
                tch->handle[ix].b16[1] = 0;
            }
        }
    }

    return ppgCheckTextureDataBe(tch);
}

s32 ppgCheckTextureDataBe(Texture* tch) {
    s32 i;

    if (tch->be == 0) {
        return 0;
    }

    for (i = 0; i < tch->total; i++) {
        if (tch->handle[i].b16[0]) {
            break;
        }
    }

    if (i == tch->total) {
        if (tch->handle != NULL) {
            ppgFree(tch->handle);
        }

        if (tch->offset != NULL) {
            ppgFree(tch->offset);
        }

        tch->handle = NULL;
        tch->offset = NULL;
        tch->be = 0;
    }

    return tch->be;
}

s32 ppgCheckPaletteDataBe(Palette* pch) {
    s32 i;

    if (pch->be == 0) {
        return 0;
    }

    for (i = 0; i < pch->total; i++) {
        if (pch->handle[i]) {
            break;
        }
    }

    if (i == pch->total) {
        if (pch->handle != NULL) {
            ppgFree(pch->handle);
        }

        pch->handle = NULL;
        pch->be = 0;
    }

    return pch->be;
}

s32 ppgGetUsingTextureHandle(Texture* tch, s32 ixNums) {
    if (tch == NULL) {
        tch = ppg_w.cur->tex;

        if (tch == NULL) {
            return 0;
        }
    }

    if (tch->be == 0) {
        return 0;
    }

    if (tch->handle == NULL) {
        return 0;
    }

    ixNums -= tch->ixNum1st;

    if (ixNums < 0 || ixNums >= tch->textures) {
        return 0;
    } else {
        return tch->handle[ixNums].b16[0];
    }
}

s32 ppgGetUsingPaletteHandle(Palette* pch, s32 ixNums) {
    if (pch == NULL) {
        pch = ppg_w.cur->pal;

        if (pch == NULL) {
            return 0;
        }
    }

    if (pch->be == 0) {
        return 0;
    }

    if (pch->handle == NULL) {
        return 0;
    }

    ixNums -= pch->ixNum1st;

    if (ixNums < 0 || ixNums >= pch->total) {
        return 0;
    } else {
        return pch->handle[ixNums];
    }
}

s32 ppgCheckTextureNumber(Texture* tex, s32 num) {
    u16 ix;

    if (tex == NULL) {
        tex = ppg_w.cur->tex;

        if (tex == NULL) {
            return 0;
        }
    }

    if (tex->be == 0) {
        return 0;
    }

    ix = num - tex->ixNum1st;

    if (ix >= tex->total) {
        return 0;
    }

    if (tex->handle[ix].b16[0]) {
        return 1;
    }

    return 0;
}
