/**
 * @file PPGFile_palettes.c
 * Setting up palette chunks, from a PPL chunk list or from a header held in a
 * directory. Split out of PPGFile_chunks.c, which keeps the texture side.
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


/* One handle per palette in the chunk, each reading the next col_items entries
 * of the decompressed data. */
static void ppgCreatePaletteHandles(Palette* pch, plContext* bits, s32 col_items) {
    s32 i;

    for (i = 0; i < pch->total; i++) {
        pch->handle[i] = flCreatePaletteHandle(bits, 0);

        if (pch->handle[i] == 0) {
            flLogOut("ppgSetupPalChunk: Failed to acquire palette handle");
        }

        bits->ptr = (u8*)bits->ptr + (col_items * bits->bitdepth);
    }
}

/* The palette data, ready for handles to be created over it: a decompression
 * area when the chunk is compressed and the chunk's own bytes when it is not,
 * decompressed into it, and byte-swapped. Everything it needs but the size and
 * the bit depth comes off the PPL header, which the caller has not written to.
 * The caller pushes the decompression area back once the handles are made. */
static void* ppgPreparePaletteData(PPLFileHeader* ppl, s32 mltSize, s32 bitdepth) {
    s32 koCmpr = ppl->compress & 3;
    s32 cmpSize = SDL_Swap32BE(ppl->fileSize) - sizeof(PPLFileHeader);
    void* cmpAdrs = ppl + 1;
    void* mltAdrs;

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

    ppgChangeDataEndian(mltAdrs, &(PPGEndianArgs){mltSize, ppl->c_mode & 4, ppl->formARGB == 0x8888, bitdepth});

    if (koCmpr == 0) {
        ppl->c_mode |= 4;
    }

    return mltAdrs;
}

s32 ppgSetupPalChunk(Palette* pch, const PPGPalChunkArgs* a) {
    PPLFileHeader* ppl;
    plContext bits;
    s32 i;
    s32 col_items;
    s32 koCmpr;
    s32 mltSize;
    void* mltAdrs;

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

    ppl = ppgFindChunk(a->adrs, MAGIC_TO_INT("pPAL"), a->num);

    if (ppl == NULL) {
        return -1;
    }

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
    mltAdrs = ppgPreparePaletteData(ppl, mltSize, bits.bitdepth);
    bits.ptr = mltAdrs;
    ppgCreatePaletteHandles(pch, &bits, col_items);

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

/* One handle per palette in a directory-loaded chunk, each over the next
 * srcSize bytes. Returns 0 at the first handle the renderer refuses, which is
 * what the goto into error_handler used to carry. */
static s32 ppgCreateDirPaletteHandles(Palette* pch, plContext* bits, u8* adrs) {
    s32 i;

    for (i = 0; i < pch->total; i++) {
        bits->ptr = adrs;
        pch->handle[i] = flCreatePaletteHandle(bits, 0);

        if (pch->handle[i] == 0) {
            return 0;
        }

        adrs = &adrs[pch->srcSize];
    }

    return 1;
}

s32 ppgSetupPalChunkDir(Palette* pch, const PPGPalChunkDirArgs* a) {
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
            a->adrs,
            &(PPGEndianArgs){ pch->total * (bits.pitch * bits.height),
                              a->ppl->c_mode & 4,
                              a->ppl->formARGB == 0x8888,
                              bits.bitdepth }
        );

        a->ppl->c_mode |= 4;

        if (!ppgCreateDirPaletteHandles(pch, &bits, a->adrs)) {
            goto error_handler;
        }

        pch->be = 1;
        return 1;
    }

error_handler:
    ppgReleaseFailedPaletteHandles(pch);
    pch->handle = NULL;
    flLogOut("ppgSetupPalChunkDir: Failed to acquire palette handle");
}
