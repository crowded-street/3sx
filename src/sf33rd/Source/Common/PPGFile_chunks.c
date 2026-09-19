/**
 * @file PPGFile_chunks.c
 * Setting up and renewing texture chunks from a PPG file. The palette side is
 * in PPGFile_palettes.c.
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

/* Every chunk header in a PPG, PPL or PPX list begins with the same two fields -
 * PPGFileHeader, PPLFileHeader and PPXFileHeader all declare `u32 magic` then
 * `u32 fileSize` - and the scan that finds the num'th chunk of a given magic
 * reads nothing else. It is written once here and each caller casts the result
 * back to its own header type. NULL where the scan used to fall out on pEND. */
typedef struct {
    u32 magic;
    u32 fileSize;
} PPChunkHeader;

void* ppgFindChunk(u8* adrs, u32 magic, s32 num) {
    const PPChunkHeader* chunk;
    u32 ofs = 0;

    while (1) {
        chunk = (const PPChunkHeader*)(adrs + ofs);

        if (MAGIC_TO_INT("pEND") == SDL_Swap32BE(chunk->magic)) {
            return NULL;
        }

        if (magic != SDL_Swap32BE(chunk->magic)) {
            ofs += ALIGN_UP(SDL_Swap32BE(chunk->fileSize), 4);
            continue;
        }

        if (num > 0) {
            num -= 1;
            ofs += ALIGN_UP(SDL_Swap32BE(chunk->fileSize), 4);
            continue;
        }

        return (void*)(adrs + ofs);
    }
}

s32 ppgSetupCmpChunk(u8* srcAdrs, s32 num, u8* dstAdrs) {
    PPXFileHeader* ppx;
    void* cmpAdrs;
    s32 cmpSize;
    s32 mltSize;
    s32 koCmpr;

    ppx = ppgFindChunk(srcAdrs, MAGIC_TO_INT("pCMP"), num);

    if (ppx == NULL) {
        return -1;
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

static void ppgSwapWords(u32* c4, s32 count) {
    for (int i = 0; i < count; i++) {
        c4[i] = SDL_Swap32BE(c4[i]);
    }
}

static void ppgSwapHalves(u16* c2, s32 count) {
    for (int i = 0; i < count; i++) {
        c2[i] = SDL_Swap16BE(c2[i]);
    }
}

/* A depth of 0 or 1 byte has nothing to swap, and dendL says the data was read
 * in the target order already. */
static s32 ppgDataAlreadyInOrder(const PPGEndianArgs* a) {
    return (a->depth == 1) || (a->depth == 0) || (a->dendL != 0);
}

void ppgChangeDataEndian(u8* adrs, const PPGEndianArgs* a) {
    if (ppgDataAlreadyInOrder(a)) {
        return;
    }

    if (a->col4 != 0) {
        ppgSwapWords((u32*)adrs, a->size / 4);
    } else {
        ppgSwapHalves((u16*)adrs, a->size / 2);
    }
}

/* No handle yet, and the slot marked as never having had one. Both chunk
 * setups blank their table this way before they start filling it. */
static void ppgBlankTextureHandles(Texture* tch, s32 ixNums) {
    s32 i;

    for (i = 0; i < ixNums; i++) {
        tch->handle[i].b16[0] = 0;
        tch->handle[i].b16[1] = 0x8000;
    }
}

/* Whatever handles were acquired before one was refused. */
static void ppgReleaseSeqTextureHandles(Texture* tch, s32 ixNums) {
    s32 i;

    for (i = 0; i < ixNums; i++) {
        if (tch->handle[i].b16[0]) {
            flReleaseTextureHandle(tch->handle[i].b16[0]);
        }
    }
}

/* One texture handle per index, each over the next srcSize bytes of the data the
 * caller has just pointed tch->srcAdrs at. The CI flag is decided once, from the
 * context, and is read nowhere else. Returns 0 at the first handle the renderer
 * refuses, which is what the goto into error_handler used to carry. */
static s32 ppgCreateSeqTextureHandles(Texture* tch, plContext* bits, const PPGTexSeqsArgs* a) {
    u8* adrs = tch->srcAdrs;
    s32 ci_flag = 0;
    s32 i;

    if (bits->bitdepth < 2) {
        ci_flag = 0x4000;
    }

    for (i = 0; i < a->ixNums; i++) {
        bits->ptr = adrs;
        tch->handle[i].b16[1] = ci_flag;
        tch->handle[i].b16[0] = flCreateTextureHandle(bits, a->attribute);

        if (tch->handle[i].b16[0] == 0) {
            return 0;
        }

        adrs += tch->srcSize;
    }

    return 1;
}

s32 ppgSetupTexChunkSeqs(Texture* tch, const PPGTexSeqsArgs* a) {
    /* The original took this by value and advanced it; the copy keeps
     * that local, which is what a by-value parameter was. */
    u8* adrs = a->adrs;

    plContext bits;
    s32 i;

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

    ppgBlankTextureHandles(tch, a->ixNums);

    ppgSetupContextFromPPG(a->ppg, &bits);
    tch->srcAdrs = adrs;
    tch->srcSize = bits.pitch * bits.height;

    for (i = 0; i < tch->srcSize * a->ixNums; i++) {
        adrs[i] = 0;
    }

    if (!ppgCreateSeqTextureHandles(tch, &bits, a)) {
        goto error_handler;
    }

    tch->be = 1;
    return 1;

error_handler:
    ppgReleaseSeqTextureHandles(tch, a->ixNums);
    ppgFree(tch->handle);
    tch->handle = NULL;
    flLogOut("ppgSetupTexChunkSeqs: Failed to acquire sprite texture handle");
}

/* One block of dot data copied through the linear-to-twiddled index table. The
 * six arms of ppgRenewDotDataSeqs differ in nothing but the element width, the
 * side of the block and the stride advance at the end of each row.
 *
 * The 0x400 and 0x800 arms walked the table with a `u16*` cursor instead of
 * subscripting it, which is the same read in the same order: for a side of 0x20
 * the subscript j + (i << 5) runs 0 to 0x3FF consecutively, and
 * ppgMakeConvTableTexDC fills every one of those entries with a value between 0
 * and 0x3FF, so the s16 and u16 reads cannot differ. */
static void ppgCopyDotBlock8(u8* dstRam, const u8* srcRam, s32 side, s32 advance) {
    s32 i;
    s32 j;

    for (i = 0; i < side; i++) {
        for (j = 0; j < side; j++) {
            *dstRam++ = srcRam[dctex_linear[j + (i << 5)]];
        }

        dstRam += advance;
    }
}

static void ppgCopyDotBlock16(u16* dstRam, const u16* srcRam, s32 side, s32 advance) {
    s32 i;
    s32 j;

    for (i = 0; i < side; i++) {
        for (j = 0; j < side; j++) {
            *dstRam++ = srcRam[dctex_linear[j + (i << 5)]];
        }

        dstRam += advance;
    }
}

/* Where a dot-data write lands: the texture, which falls back to the current
 * one, and the index inside it. -1 when there is nowhere to write - the texture
 * holds no data, the global index is outside the chunk, or the slot has no
 * handle - and otherwise the slot is marked dirty on the way out, which is what
 * the caller did the moment it had passed all four tests. */
static s32 ppgDotDataTarget(Texture** tchp, const PPGDotDataArgs* a) {
    Texture* tch = *tchp;
    s32 ix;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
        *tchp = tch;
    }

    if (!(tch->be != 0)) {
        return -1;
    }

    ix = a->gix - tch->ixNum1st;

    if ((ix < 0) || (ix >= tch->total)) {
        return -1;
    }

    if (!(tch->handle[ix].b16[0] != 0)) {
        return -1;
    }

    tch->handle[ix].b16[1] |= 0x2000;
    return ix;
}

void ppgRenewDotDataSeqs(Texture* tch, const PPGDotDataArgs* a) {
    s32 ix = ppgDotDataTarget(&tch, a);

    if (ix < 0) {
        return;
    }

    switch (a->size) {
    case 0x40:
        ppgCopyDotBlock8((u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_0(a->code)), (u8*)a->srcRam, 8, 0xF8);
        break;

    case 0x100:
        ppgCopyDotBlock8((u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_0(a->code)), (u8*)a->srcRam, 0x10, 0xF0);
        break;

    case 0x400:
        ppgCopyDotBlock8((u8*)(tch->srcAdrs + tch->srcSize * ix + CODE_1(a->code)), (u8*)a->srcRam, 0x20, 0xE0);
        break;

    case 0x80:
        ppgCopyDotBlock16((u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_0(a->code)) * 2), (u16*)a->srcRam, 8, 0xF8);
        break;

    case 0x200:
        ppgCopyDotBlock16((u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_0(a->code)) * 2), (u16*)a->srcRam, 0x10, 0xF0);
        break;

    case 0x800:
        ppgCopyDotBlock16((u16*)(tch->srcAdrs + tch->srcSize * ix + (CODE_1(a->code)) * 2), (u16*)a->srcRam, 0x20, 0xE0);
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

/* One texture whose dot data has been marked dirty: the mark is cleared, the
 * handle locked, its own srcSize-byte run copied in, and the handle unlocked. */
static void ppgRenewOneTexChunk(Texture* tch, s32 i) {
    plContext bits;
    s32* srcRam;
    s32* dstRam;

    tch->handle[i].b16[1] &= 0xDFFF;
    flLockTexture(NULL, tch->handle[i].b16[0], &bits, 3);
    dstRam = bits.ptr;
    srcRam = (s32*)(tch->srcAdrs + tch->srcSize * i);
    SDL_memmove(dstRam, srcRam, tch->srcSize);
    flUnlockTexture(tch->handle[i].b16[0]);
}

s32 ppgRenewTexChunkSeqs(Texture* tch) {
    s32 i;

    if (tch == NULL) {
        tch = ppg_w.cur->tex;
    }

    if (tch == NULL) {
        return 0;
    }

    if (tch->be == 0) {
        return 0;
    }

    for (i = 0; i < tch->total; i++) {
        if (tch->handle[i].b16[1] & 0x2000) {
            ppgRenewOneTexChunk(tch, i);
        }
    }

    return 1;
}

/* The chunk list is walked twice: once to count the pTEX chunks, so the offset
 * table can be sized, and once to write their offsets into it. The two walks
 * were identical but for what they do at a pTEX chunk, so that is the
 * parameter. Both actions accumulate into tch, which is what they were doing in
 * place. */
static void ppgCountOneTexChunk(Texture* tch, s32 ofs) {
    tch->textures += 1;
}

static void ppgRecordOneTexChunk(Texture* tch, s32 ofs) {
    tch->offset[tch->accnum++] = ofs;
}

static void ppgWalkTexChunks(Texture* tch, void (*at_tex)(Texture*, s32)) {
    PPGFileHeader* ppg;
    s32 ofs = 0;

    while (1) {
        ppg = (PPGFileHeader*)(tch->srcAdrs + ofs);

        if (MAGIC_TO_INT("pEND") != SDL_Swap32BE(ppg->magic)) {
            if (MAGIC_TO_INT("pTEX") == SDL_Swap32BE(ppg->magic)) {
                at_tex(tch, ofs);
            }

            ofs += ALIGN_UP(SDL_Swap32BE(ppg->fileSize), 4);
        } else {
            break;
        }
    }
}

s32 ppgSetupTexChunk_1st(Texture* tch, const PPGTexChunk1stArgs* a) {
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

    ppgBlankTextureHandles(tch, a->ixNums);

    ppgWalkTexChunks(tch, ppgCountOneTexChunk);

    if (tch->textures == 0) {
        flLogOut("ppgSetupTexChunk_1st: Texture data was not found");
    }

    tch->offset = ppgMallocF(tch->textures * 4);

    if (tch->offset == NULL) {
        flLogOut("ppgSetupTexChunk_1st: Failed to allocate memory for the texture offset table");
    }

    ppgWalkTexChunks(tch, ppgRecordOneTexChunk);

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

/* The texture's pixels, ready for a handle to be created over them: pulled into
 * a decompression buffer, decompressed into it and byte-swapped. The compressed
 * run starts past the transparent-run table, whose length the header gives. The
 * caller pushes the buffer back once the handle is made. Written alongside
 * ppgPreparePaletteData, which does the same for a palette chunk. */
static void* ppgPrepareTextureData(PPGFileHeader* ppg, s32 mltSize, s32 bitdepth) {
    s32 koCmpr = ppg->compress & 3;
    s32 cmpSize = (u16)SDL_Swap16BE(ppg->transNums) * 3 + 0x10;
    void* cmpAdrs = (u8*)ppg + cmpSize;
    void* mltAdrs;

    cmpSize = SDL_Swap32BE(ppg->fileSize) - cmpSize;
    mltAdrs = ppgPullDecBuff(mltSize);

    if (mltAdrs == NULL) {
        flLogOut("ppgSetupTexChunk_3rd: Failed to allocate texture data buffer");
    }

    if (mltSize != ppgDecompress(koCmpr, &(PPGDecompressArgs){cmpAdrs, cmpSize, mltAdrs, mltSize})) {
        flLogOut("ppgSetupTexChunk_3rd: Failed to acquire sprite texture handle");
    }

    ppgChangeDataEndian(mltAdrs, &(PPGEndianArgs){mltSize, ppg->pixel & 4, ppg->formARGB == 0x8888, bitdepth});
    return mltAdrs;
}

s32 ppgSetupTexChunk_3rd(Texture* tch, s32 ixNum, u32 attribute) {
    plContext bits;
    PPGFileHeader* ppg;
    TextureHandle* hnof;
    s32 mltSize;
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
    mltSize = bits.height * bits.pitch;
    mltAdrs = ppgPrepareTextureData(ppg, mltSize, bits.bitdepth);
    bits.ptr = mltAdrs;
    hnof->b16[0] = flCreateTextureHandle(&bits, attribute);
    ppgPushDecBuff(mltAdrs);

    if (hnof->b16[0] == 0) {
        flLogOut("ppgSetupTexChunk_3rd: Failed to acquire texture handle");
    }

    return 1;
}
