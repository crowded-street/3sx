/**
 * @file PPGFile_chunks.c
 * Setting up and renewing texture and palette chunks from a PPG or PPL file.
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


ssize_t ppgDecompress(s32 koCmpr, const PPGDecompressArgs* a) {
    u8* src;
    u8* dst;
    s32 i;
    ssize_t rnum = 0;

    switch (koCmpr) {
    default:
        if (a->srcAdrs != a->dstAdrs) {
            src = a->srcAdrs;
            dst = a->dstAdrs;

            for (i = 0; i < a->dstSize; i++) {
                *dst++ = *src++;
            }
        }

        rnum = a->srcSize;
        break;

    case 1:
        rnum = decLZ77withSizeCheck(a->srcAdrs, a->dstAdrs, a->dstSize);
        rnum *= a->dstSize;
        break;

    case 2:
        rnum = zlib_Decompress(a->srcAdrs, a->srcSize, a->dstAdrs, a->dstSize);
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

        if (MAGIC_TO_INT("pEND") == SDL_Swap32BE(ppx->magic)) {
            return -1;
        }

        if (MAGIC_TO_INT("pCMP") != SDL_Swap32BE(ppx->magic)) {
            ofs += ALIGN_UP(SDL_Swap32BE(ppx->fileSize), 4);
            continue;
        }

        if (num > 0) {
            num -= 1;
            ofs += ALIGN_UP(SDL_Swap32BE(ppx->fileSize), 4);
            continue;
        }

        break;
    }

    mltSize = SDL_Swap32BE(ppx->expSize);
    cmpSize = SDL_Swap32BE(ppx->fileSize) - sizeof(PPXFileHeader);
    cmpAdrs = ppx + 1;
    koCmpr = ppx->compress & 3;

    if (mltSize != ppgDecompress(koCmpr, &(PPGDecompressArgs){cmpAdrs, cmpSize, dstAdrs, mltSize})) {
        flLogOut("ppgSetupCmpChunk: Failed to decompress data");
    }

    return 1;
}

s32 ppgSetupPalChunk(Palette* pch, const PPGPalChunkArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    s32 num = a->num;

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
        flLogOut("ppgSetupPalChunk: palette is already in use");
    }

    pch->be = 0;
    pch->ixNum1st = a->ixNum1st;
    pch->srcAdrs = a->adrs;
    pch->srcSize = a->size;
    pch->handle = NULL;
    mltAdrs = NULL;
    koCmpr = 0;

    while (1) {
        ppl = (PPLFileHeader*)(a->adrs + ofs);

        if (MAGIC_TO_INT("pEND") == SDL_Swap32BE(ppl->magic)) {
            return -1;
        }

        if (MAGIC_TO_INT("pPAL") != SDL_Swap32BE(ppl->magic)) {
            ofs += ALIGN_UP(SDL_Swap32BE(ppl->fileSize), 4);
            continue;
        }

        if (num > 0) {
            num -= 1;
            ofs += ALIGN_UP(SDL_Swap32BE(ppl->fileSize), 4);
            continue;
        }

        break;
    }

    cmpSize = SDL_Swap32BE(ppl->fileSize) - sizeof(PPLFileHeader);
    cmpAdrs = ppl + 1;
    pch->c_mode = ppl->c_mode & 3;
    pch->total = SDL_Swap16BE(ppl->palettes);
    col_items = pplColorModeWidth[pch->c_mode] + 1;
    koCmpr = ppl->compress & 3;
    ppgSetupContextFromPPL(ppl, &bits);
    pch->handle = ppgMallocF(pch->total * 2);

    if (pch->handle == NULL) {
        flLogOut("ppgSetupPalChunk: Failed to allocate palette memory");
    }

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
        flLogOut("ppgSetupPalChunk: Failed to allocate palette data decompression area");
    }

    if (mltSize != ppgDecompress(koCmpr, &(PPGDecompressArgs){cmpAdrs, cmpSize, mltAdrs, mltSize})) {
        flLogOut("ppgSetupPalChunk: Failed to decompress the palette data");
    }

    ppgChangeDataEndian(mltAdrs, &(PPGEndianArgs){mltSize, ppl->c_mode & 4, ppl->formARGB == 0x8888, bits.bitdepth});

    if (koCmpr == 0) {
        ppl->c_mode |= 4;
    }

    bits.ptr = mltAdrs;

    for (i = 0; i < pch->total; i++) {
        pch->handle[i] = flCreatePaletteHandle(&bits, 0);

        if (pch->handle[i] == 0) {
            flLogOut("ppgSetupPalChunk: Failed to acquire palette handle");
        }

        bits.ptr = (u8*)bits.ptr + (col_items * bits.bitdepth);
    }

    if (koCmpr != 0) {
        ppgPushDecBuff(mltAdrs);
    }

    pch->be = 1;
    return 1;
}

static void ppgReleaseFailedPaletteHandles(Palette* pch) {
    s32 i;

    if (pch->handle != NULL) {
        for (i = 0; i < pch->total; i++) {
            if (pch->handle[i]) {
                flReleasePaletteHandle(pch->handle[i]);
            }
        }

        ppgFree(pch->handle);
    }
}

s32 ppgSetupPalChunkDir(Palette* pch, const PPGPalChunkDirArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    u8* adrs = a->adrs;

    plContext bits;
    s32 i;

    if (pch == NULL) {
        pch = ppg_w.cur->pal;
    }

    if (pch->be) {
        flLogOut("ppgSetupPalChunkDir: Palette is already in use");
    }

    pch->be = 0;
    pch->ixNum1st = a->ixNum1st;
    pch->srcAdrs = NULL;
    pch->c_mode = a->ppl->c_mode & 3;
    ppgSetupContextFromPPL(a->ppl, &bits);
    pch->srcSize = bits.pitch * bits.height;
    pch->total = SDL_Swap16BE(a->ppl->palettes);
    pch->handle = ppgMallocF(pch->total * 2);

    if (pch->handle != NULL) {
        for (i = 0; i < pch->total; i++) {
            pch->handle[i] = 0;
        }

        ppgChangeDataEndian(
            adrs,
            &(PPGEndianArgs){ pch->total * (bits.pitch * bits.height),
                              a->ppl->c_mode & 4,
                              a->ppl->formARGB == 0x8888,
                              bits.bitdepth }
        );

        a->ppl->c_mode |= 4;

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
    ppgReleaseFailedPaletteHandles(pch);
    pch->handle = NULL;
    flLogOut("ppgSetupPalChunkDir: Failed to acquire palette handle");
}

void ppgChangeDataEndian(u8* adrs, const PPGEndianArgs* a) {
    if ((a->depth == 1) || (a->depth == 0) || (a->dendL != 0)) {
        return;
    }

    if (a->col4 != 0) {
        u32* c4 = adrs;

        for (int i = 0; i < a->size / 4; i++) {
            c4[i] = SDL_Swap32BE(c4[i]);
        }
    } else {
        u16* c2 = adrs;

        for (int i = 0; i < a->size / 2; i++) {
            c2[i] = SDL_Swap16BE(c2[i]);
        }
    }
}

s32 ppgSetupTexChunkSeqs(Texture* tch, const PPGTexSeqsArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    u8* adrs = a->adrs;

    plContext bits;
    s32 i;
    s32 ci_flag = 0;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch->be) {
        flLogOut("ppgSetupTexChunkSeqs: texture is already in use");
    }

    tch->be = 0;
    tch->textures = a->ixNums;
    tch->accnum = a->ixNums;
    tch->ixNum1st = a->ixNum1st;
    tch->total = a->ixNums;
    tch->flags = 0x80;
    tch->arCnt = 0;
    tch->arInit = 0;
    tch->handle = NULL;
    tch->offset = NULL;
    tch->srcAdrs = NULL;
    tch->srcSize = 0;
    tch->handle = ppgMallocF(a->ixNums * 4);

    if (tch->handle == NULL) {
        flLogOut("ppgSetupTexChunkSeqs: Failed to allocate memory for texture handle");
    }

    for (i = 0; i < a->ixNums; i++) {
        tch->handle[i].b16[0] = 0;
        tch->handle[i].b16[1] = 0x8000;
    }

    ppgSetupContextFromPPG(a->ppg, &bits);
    tch->srcAdrs = adrs;
    tch->srcSize = bits.pitch * bits.height;

    for (i = 0; i < tch->srcSize * a->ixNums; i++) {
        adrs[i] = 0;
    }

    if (bits.bitdepth < 2) {
        ci_flag = 0x4000;
    }

    for (i = 0; i < a->ixNums; i++) {
        bits.ptr = adrs;
        tch->handle[i].b16[1] = ci_flag;
        tch->handle[i].b16[0] = flCreateTextureHandle(&bits, a->attribute);

        if (tch->handle[i].b16[0] == 0) {
            goto error_handler;
        }

        adrs += tch->srcSize;
    }

    tch->be = 1;
    return 1;

error_handler:
    for (i = 0; i < a->ixNums; i++) {
        if (tch->handle[i].b16[0]) {
            flReleaseTextureHandle(tch->handle[i].b16[0]);
        }
    }

    ppgFree(tch->handle);
    tch->handle = NULL;
    flLogOut("ppgSetupTexChunkSeqs: Failed to acquire sprite texture handle");
}

void ppgRenewDotDataSeqs(Texture* tch, const PPGDotDataArgs* a) {
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

    if (!(tch->be != 0)) {
        return;
    }

    ix = a->gix - tch->ixNum1st;

    if ((ix < 0) || (ix >= tch->total)) {
        return;
    }

    if (!(tch->handle[ix].b16[0] != 0)) {
        return;
    }

    tch->handle[ix].b16[1] |= 0x2000;

    switch (a->size) {
    case 0x40:
        srcRam8 = (u8*)a->srcRam;
        dstRam8 = (u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_0(a->code));

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                *dstRam8++ = srcRam8[dctex_linear[j + (i << 5)]];
            }

            dstRam8 += 0xF8;
        }

        break;

    case 0x100:
        srcRam8 = (u8*)a->srcRam;
        dstRam8 = (u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_0(a->code));

        for (i = 0; i < 0x10; i++) {
            for (j = 0; j < 0x10; j++) {
                *dstRam8++ = srcRam8[dctex_linear[j + (i << 5)]];
            }

            dstRam8 += 0xF0;
        }

        break;

    case 0x400:
        srcRam8 = (u8*)a->srcRam;
        dstRam8 = (u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_1(a->code));
        tix = (u16*)dctex_linear;

        for (i = 0; i < 0x20; i++) {
            for (j = 0; j < 0x20; j++) {
                *dstRam8++ = srcRam8[*tix++];
            }

            dstRam8 += 0xE0;
        }

        break;

    case 0x80:
        srcRam16 = (u16*)a->srcRam;
        dstRam16 = (u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_0(a->code)) * 2);

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                *dstRam16++ = srcRam16[dctex_linear[j + (i << 5)]];
            }

            dstRam16 += 0xF8;
        }

        break;

    case 0x200:
        srcRam16 = (u16*)a->srcRam;
        dstRam16 = (u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_0(a->code)) * 2);

        for (i = 0; i < 0x10; i++) {
            for (j = 0; j < 0x10; j++) {
                *dstRam16++ = srcRam16[dctex_linear[j + (i << 5)]];
            }

            dstRam16 += 0xF0;
        }

        break;

    case 0x800:
        srcRam16 = (u16*)a->srcRam;
        dstRam16 = (u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_1(a->code)) * 2);
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
            SDL_memmove(dstRam, srcRam, tch->srcSize);
            flUnlockTexture(tch->handle[i].b16[0]);
        }
    }

    return 1;
}

s32 ppgSetupTexChunk_1st(Texture* tch, const PPGTexChunk1stArgs* a) {
    PPGFileHeader* ppg;
    s32 i;
    s32 ofs;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch->be) {
        flLogOut("ppgSetupTexChunk_1st: Texture is already in use");
    }

    tch->be = 0;
    tch->textures = 0;
    tch->accnum = 0;
    tch->ixNum1st = a->ixNum1st;
    tch->total = a->ixNums;
    tch->flags = a->ar != 0;
    tch->arCnt = 0;
    tch->arInit = a->arcnt;
    tch->offset = NULL;
    tch->srcAdrs = a->adrs;
    tch->srcSize = a->size;
    tch->handle = (TextureHandle*)ppgMallocF(a->ixNums * sizeof(TextureHandle));

    if (tch->handle == NULL) {
        flLogOut("ppgSetupTexChunk_1st: Failed to allocate memory for texture handle");
    }

    for (i = 0; i < a->ixNums; i++) {
        tch->handle[i].b16[0] = 0;
        tch->handle[i].b16[1] = 0x8000;
    }

    ofs = 0;

    while (1) {
        ppg = (PPGFileHeader*)(tch->srcAdrs + ofs);

        if (MAGIC_TO_INT("pEND") != SDL_Swap32BE(ppg->magic)) {
            if (MAGIC_TO_INT("pTEX") == SDL_Swap32BE(ppg->magic)) {
                tch->textures += 1;
            }

            ofs += ALIGN_UP(SDL_Swap32BE(ppg->fileSize), 4);
        } else {
            break;
        }
    }

    if (tch->textures == 0) {
        flLogOut("ppgSetupTexChunk_1st: Texture data was not found");
    }

    tch->offset = ppgMallocF(tch->textures * 4);

    if (tch->offset == NULL) {
        flLogOut("ppgSetupTexChunk_1st: Failed to allocate memory for the texture offset table");
    }

    ofs = 0;

    while (1) {
        ppg = (PPGFileHeader*)(tch->srcAdrs + ofs);

        if (MAGIC_TO_INT("pEND") != SDL_Swap32BE(ppg->magic)) {
            if (MAGIC_TO_INT("pTEX") == SDL_Swap32BE(ppg->magic)) {
                tch->offset[tch->accnum++] = ofs;
            }

            ofs += ALIGN_UP(SDL_Swap32BE(ppg->fileSize), 4);
        } else {
            break;
        }
    }

    tch->accnum = 0;
    tch->be = 1;
    return 1;
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
        flLogOut("ppgSetupTexChunk_2nd: Handle acquisition process has been called too many times");
    }

    hnof = tch->handle + (ixNum - tch->ixNum1st);
    hnof->b16[1] = tch->accnum++;

    if (tch->srcAdrs == NULL) {
        flLogOut("ppgSetupTexChunk_2nd: Texture data is NULL");
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
        flLogOut("ppgSetupTexChunk_3rd: Texture chunk data is NULL");
    }

    ppg = (PPGFileHeader*)(tch->srcAdrs + (tch->offset[hnof->b16[1] & 0xFFF]));
    ppgSetupContextFromPPG(ppg, &bits);
    koCmpr = ppg->compress & 3;
    cmpSize = (u16)SDL_Swap16BE(ppg->transNums) * 3 + 0x10;
    cmpAdrs = (u8*)ppg + cmpSize;
    cmpSize = SDL_Swap32BE(ppg->fileSize) - cmpSize;
    mltSize = bits.height * bits.pitch;
    mltAdrs = ppgPullDecBuff(mltSize);

    if (mltAdrs == NULL) {
        flLogOut("ppgSetupTexChunk_3rd: Failed to allocate texture data buffer");
    }

    if (mltSize != ppgDecompress(koCmpr, &(PPGDecompressArgs){cmpAdrs, cmpSize, mltAdrs, mltSize})) {
        flLogOut("ppgSetupTexChunk_3rd: Failed to acquire sprite texture handle");
    }

    ppgChangeDataEndian(mltAdrs, &(PPGEndianArgs){mltSize, ppg->pixel & 4, ppg->formARGB == 0x8888, bits.bitdepth});
    bits.ptr = mltAdrs;
    hnof->b16[0] = flCreateTextureHandle(&bits, attribute);
    ppgPushDecBuff(mltAdrs);

    if (hnof->b16[0] == 0) {
        flLogOut("ppgSetupTexChunk_3rd: Failed to acquire texture handle");
    }

    return 1;
}
