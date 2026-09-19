/**
 * @file PPGFile.c
 * PPG and PPL file handling
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

const u8 pplColorModeWidth[4] = { 0xF, 0x3F, 0xFF, 0 };

PPG_W ppg_w;
s16* dctex_linear;
void ppg_Initialize(void* lcmAdrs, s32 lcmSize) {
    if (lcmAdrs == NULL) {
        flLogOut("ppg_Initialize: lcmAdrs is NULL");
    }

    mmHeapInitialize(&ppg_w.mm, lcmAdrs, lcmSize, ALIGN_UP(sizeof(_MEMMAN_CELL), 16));
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

/* One palette slot given back: the handle released if it holds one, and the
 * slot cleared either way. Both arms of the release did this, over all slots or
 * over one. */
static void release_one_palette_handle(Palette* pch, s32 i) {
    u16 han = pch->handle[i];

    if (han) {
        flReleasePaletteHandle(han);
    }

    pch->handle[i] = 0;
}

/* Every slot, which is what a negative index asks for. */
static void release_all_palette_handles(Palette* pch) {
    s32 i;

    for (i = 0; i < pch->total; i++) {
        release_one_palette_handle(pch, i);
    }
}

s32 ppgReleasePaletteHandle(Palette* pch, s32 ixNum) {
    s32 ix;

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
        release_all_palette_handles(pch);
    } else {
        ix = ixNum - pch->ixNum1st;

        if ((ix >= 0) && (ix < pch->total)) {
            release_one_palette_handle(pch, ix);
        }
    }

    return ppgCheckPaletteDataBe(pch);
}

static bool ppgTextureIndexIsInRange(s32 ix, const Texture* tch) {
    return (ix >= 0) && (ix < tch->total);
}

/* The same for a texture slot, which also clears the handle word when the
 * chunk is a sequential one. */
static void release_one_texture_handle(Texture* tch, s32 i) {
    u16 han = tch->handle[i].b16[0];

    if (han) {
        flReleaseTextureHandle(han);
    }

    tch->handle[i].b16[0] = 0;

    if (tch->flags & 0x80) {
        tch->handle[i].b16[1] = 0;
    }
}

/* Every slot, which is what a negative index asks for. */
static void release_all_texture_handles(Texture* tch) {
    s32 i;

    for (i = 0; i < tch->total; i++) {
        release_one_texture_handle(tch, i);
    }
}

s32 ppgReleaseTextureHandle(Texture* tch, s32 ixNum) {
    s32 ix;

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
        release_all_texture_handles(tch);
    } else {
        ix = ixNum - tch->ixNum1st;

        if (ppgTextureIndexIsInRange(ix, tch)) {
            release_one_texture_handle(tch, ix);
        }
    }

    return ppgCheckTextureDataBe(tch);
}

/* Nothing holds a handle any more, so the chunk's own tables go back and the
 * texture stops being in use. */
static void free_texture_tables(Texture* tch) {
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
        free_texture_tables(tch);
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

