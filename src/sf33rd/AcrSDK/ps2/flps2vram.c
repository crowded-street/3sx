#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/common/prilay.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include "port/sdl/sdl_game_renderer.h"

#include <libgraph.h>

#include <assert.h>
#include <memory.h>

#define LPVRAM_ERROR ((LPVram*)-1)

#define ERR_STOP                                                                                                       \
    while (1) {}

static s32 flPS2ConvertTextureFromContext(plContext* lpcontext, FLTexture* lpflTexture, u32 type);
s32 flPS2GetVramFreeArea(u32* lpflhTexture, s32 tex_num);
s16 flPS2GetTextureBuffWidth(s16 width);
u32 flPS2GetTextureSize(u32 format, s32 dw, s32 dh, s32 bnum);
s16 flPS2GetTextureVramBlock(FLTexture* lpflTexture);
static s16 flPS2GetPaletteVramBlock(FLTexture* lpflTexture);
s32 flPS2AddVramList(LPVram* lpVram, FLTexture* lpflTexture);
s32 flPS2RewriteVramList(LPVram* lpVram, FLTexture* lpflTexture);
LPVram* flPS2SearchVramChange(FLTexture* lpflTexture, u32* lpflhTexture, s32 tex_num);
LPVram* flPS2SearchVramSpace(u32 size, u32 align);
s32 flPS2DeleteVramList(FLTexture* lpflTexture);
s32 flPS2LockTexture(Rect* /* unused */, FLTexture* lpflTexture, plContext* lpcontext, u32 flag, s32 /* unused */);
s32 flPS2UnlockTexture(FLTexture*);
static void Conv4to32(s32 width, s32 height, u8* p_input, u8* p_output);
static void PageConv4to32(u8* p_input, u8* p_output, s32 p_page_w);
static void BlockConv4to32(u8* p_input, u8* p_output, s32 p_page_w);
static void Conv8to32(s32 width, s32 height, u8* p_input, u8* p_output);
static void PageConv8to32(u8* p_input, u8* p_output, s32 p_page_w);
static void BlockConv8to32(u8* p_input, u8* p_output, s32 p_page_w);

u32 flCreateTextureHandle(plContext* bits, u32 flag) {
    FLTexture* lpflTexture;
    u32 th = flPS2GetTextureHandle();

    if (th == 0) {
        return 0;
    }

    lpflTexture = &flTexture[LO_16_BITS(th) - 1];
    flPS2GetTextureInfoFromContext(bits, 1, th, flag);

    if (bits->ptr == NULL) {
        if (flPS2GetVramFreeArea(&th, 1) == 0) {
            flLogOut("Coundn't allocate texture. Need to call Compact function. @flCreateTextureHandle");
            assert(0);
            return 0;
        }

        lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    } else {
        flPS2ConvertTextureFromContext(bits, lpflTexture, 0);
        flPS2CreateTextureHandle(th, flag);
    }

    return th;
}

s32 flPS2GetTextureInfoFromContext(plContext* bits, s32 bnum, u32 th, u32 flag) {
    FLTexture* lpflTexture;
    s32 lp0;
    s32 dw;
    s32 dh;
    plContext* lpcon;

    lpflTexture = &flTexture[LO_16_BITS(th) - 1];

    if (bnum > 1) {
        if (bnum > 7) {
            flLogOut("Not supported mipmap texture @flPS2GetTextureInfoFromContext");
            assert(0);
            return 0;
        }

        lpcon = bits + 1;
        dw = bits->width;
        dh = bits->height;

        for (lp0 = 1; lp0 < bnum; lp0++) {
            dw >>= 1;
            dh >>= 1;

            if ((lpcon->width != dw) || (lpcon->height != dh)) {
                flLogOut("Not supported mipmap texture @flPS2GetTextureInfoFromContext");
                assert(0);
                return 0;
            }

            lpcon += 1;
        }
    }

    lpflTexture->be_flag = 1;
    lpflTexture->flag = flag;
    lpflTexture->desc = bits->desc;
    lpflTexture->width = bits->width;
    lpflTexture->height = bits->height;
    lpflTexture->dma_type = 0;
    lpflTexture->mem_handle = 0;
    lpflTexture->lock_ptr = 0;
    lpflTexture->lock_flag = 0;
    lpflTexture->tex_num = bnum;

    switch (bits->bitdepth) {
    default:
        flLogOut("Not supported texture bit depth @flPS2GetTextureInfoFromContext");
        assert(0);
        return 0;

    case 0:
        lpflTexture->format = SCE_GS_PSMT4;
        lpflTexture->bitdepth = 0;
        break;

    case 1:
        lpflTexture->format = SCE_GS_PSMT8;
        lpflTexture->bitdepth = 1;
        break;

    case 2:
        lpflTexture->format = SCE_GS_PSMCT16;
        lpflTexture->bitdepth = 2;
        break;

    case 3:
        lpflTexture->format = SCE_GS_PSMCT24;
        lpflTexture->bitdepth = 3;
        break;

    case 4:
        lpflTexture->format = SCE_GS_PSMCT32;
        lpflTexture->bitdepth = 4;
        break;
    }

    switch (bits->width) {
    case 1024:
    case 512:
    case 256:
    case 128:
    case 64:
    case 32:
        break;

    default:
        flLogOut("Not supported width...%d @flPS2GetTextureInfoFromContext", bits->width);
        return 0;
    }

    lpflTexture->tw = flPS2GetTextureBuffWidth(lpflTexture->width);

    switch (bits->height) {
    case 1024:
    case 512:
    case 256:
    case 128:
    case 64:
    case 32:
        break;

    default:
        flLogOut("Not supported height...%d @flPS2GetTextureInfoFromContext", bits->height);
        return 0;
    }

    lpflTexture->th = flPS2GetTextureBuffWidth(lpflTexture->height);
    lpflTexture->block_size = flPS2GetTextureVramBlock(lpflTexture);
    lpflTexture->block_align = 0x20;
    lpflTexture->size =
        flPS2GetTextureSize(lpflTexture->format, lpflTexture->width, lpflTexture->height, lpflTexture->tex_num);
    return 1;
}

s32 flPS2CreateTextureHandle(u32 th, u32 flag) {
    SDLGameRenderer_CreateTexture(th);
    return 1;
}

u32 flPS2GetTextureHandle() {
    s32 i;

    for (i = 0; i < FL_TEXTURE_MAX; i++) {
        if (!flTexture[i].be_flag) {
            break;
        }
    }

    if (i == FL_TEXTURE_MAX) {
        flPS2SystemError(0, "ERROR flPS2GetTextureHandle flps2vram.c");
    }

    return i + 1;
}

u32 flCreatePaletteHandle(plContext* lpcontext, u32 flag) {
    FLTexture* lpflPalette;
    u32 ph = flPS2GetPaletteHandle();

    if (ph == 0) {
        return 0;
    }

    lpflPalette = &flPalette[HI_16_BITS(ph) - 1];
    flPS2GetPaletteInfoFromContext(lpcontext, ph, flag);

    if (lpcontext->ptr == NULL) {
        if (flPS2GetVramFreeArea(&ph, 1) == 0) {
            flLogOut("Coundn't allocate texture. Need to call Compact function. @flCreatePaletteHandle");
            assert(0);
            return 0;
        }

        lpflPalette->mem_handle = flPS2GetSystemMemoryHandle(lpflPalette->size, 2);
    } else {
        if (lpcontext->width == 256) {
            flPS2ConvertTextureFromContext(lpcontext, lpflPalette, 1);
        } else {
            flPS2ConvertTextureFromContext(lpcontext, lpflPalette, 0);
        }

        flPS2CreatePaletteHandle(ph, flag);
    }

    return ph >> 16;
}

s32 flPS2GetPaletteInfoFromContext(plContext* bits, u32 ph, u32 flag) {
    FLTexture* lpflPalette = &flPalette[((ph & 0xFFFF0000) >> 0x10) - 1];

    if (bits->height != 1) {
        flLogOut("Supported only 1 palette. Unallocatable. @flCreatePaletteHandle");
        return 0;
    }

    switch (bits->bitdepth) {
    default:
        flLogOut("Not supported texture bit depth @flCreatePaletteHandle");
        return 0;

    case 2:
        lpflPalette->format = 2;
        lpflPalette->bitdepth = 2;
        break;

    case 3:
        lpflPalette->format = 1;
        lpflPalette->bitdepth = 3;
        break;

    case 4:
        lpflPalette->format = 0;
        lpflPalette->bitdepth = 4;
        break;
    }

    if (bits->width == 256) {
        lpflPalette->width = 16;
        lpflPalette->height = 16;
    } else {
        lpflPalette->width = 8;
        lpflPalette->height = 2;
    }

    lpflPalette->desc = bits->desc;
    lpflPalette->flag = flag;
    lpflPalette->be_flag = 1;
    lpflPalette->tw = flPS2GetTextureBuffWidth(lpflPalette->width);
    lpflPalette->th = flPS2GetTextureBuffWidth(lpflPalette->height);
    lpflPalette->dma_type = 0;
    lpflPalette->mem_handle = 0;
    lpflPalette->lock_ptr = 0;
    lpflPalette->lock_flag = 0;
    lpflPalette->tex_num = 1;
    lpflPalette->block_size = flPS2GetPaletteVramBlock(lpflPalette);
    lpflPalette->block_align = 1;
    lpflPalette->size =
        flPS2GetTextureSize(lpflPalette->format, lpflPalette->width, lpflPalette->height, lpflPalette->tex_num);
    return 1;
}

s32 flPS2CreatePaletteHandle(u32 ph, u32 flag) {
    SDLGameRenderer_CreatePalette(ph);
    return 1;
}

u32 flPS2GetPaletteHandle() {
    s32 i;

    for (i = 0; i < FL_PALETTE_MAX; i++) {
        if (!flPalette[i].be_flag) {
            break;
        }
    }

    if (i == FL_PALETTE_MAX) {
        flPS2SystemError(0, "ERROR flPS2GetPaletteHandle flps2vram.c");
    }

    return (i + 1) << 16;
}

s32 flReleaseTextureHandle(u32 texture_handle) {
    FLTexture* lpflTexture = &flTexture[texture_handle - 1];

    if ((texture_handle == 0) || (texture_handle > FL_TEXTURE_MAX) || (lpflTexture->be_flag == 0)) {
        flPS2SystemError(0, "ERROR flReleaseTextureHandle flps2vram.c");
    }

    SDLGameRenderer_DestroyTexture(texture_handle);
    flPS2DeleteVramList(lpflTexture); // FIXME: we don't really need to keep track of vram

    if (lpflTexture->mem_handle != 0) {
        flPS2ReleaseSystemMemory(lpflTexture->mem_handle);
    }

    flMemset(lpflTexture, 0, sizeof(FLTexture));
    return 1;
}

s32 flReleasePaletteHandle(u32 palette_handle) {
    FLTexture* lpflPalette = &flPalette[palette_handle - 1];

    if ((palette_handle == 0) || (palette_handle > FL_PALETTE_MAX) || (lpflPalette->be_flag == 0)) {
        flPS2SystemError(0, "ERROR flReleasePaletteHandle flps2vram.c");
    }

    SDLGameRenderer_DestroyPalette(palette_handle);
    flPS2DeleteVramList(lpflPalette); // FIXME: we don't really need to keep track of vram

    if (lpflPalette->mem_handle != 0) {
        flPS2ReleaseSystemMemory(lpflPalette->mem_handle);
    }

    flMemset(lpflPalette, 0, sizeof(FLTexture));
    return 1;
}

s32 flLockTexture(Rect* lprect, u32 th, plContext* lpcontext, u32 flag) {
    FLTexture* lpflTexture = &flTexture[th - 1];

    if (th > FL_TEXTURE_MAX) {
        return 0;
    }

    if (!lpflTexture->be_flag) {
        return 0;
    }

    return flPS2LockTexture(lprect, lpflTexture, lpcontext, flag, 0);
}

s32 flLockPalette(Rect* lprect, u32 th, plContext* lpcontext, u32 flag) {
    FLTexture* lpflPalette = &flPalette[th - 1];

    if (th > FL_PALETTE_MAX) {
        return 0;
    }

    if (!lpflPalette->be_flag) {
        return 0;
    }

    if (flPS2LockTexture(lprect, lpflPalette, lpcontext, flag, 1) == 0) {
        return 0;
    }

    if ((lpflPalette->width == 16) && (lpflPalette->height == 16)) {
        lpcontext->width = 256;
        lpcontext->height = 1;
    } else {
        lpcontext->width = 16;
        lpcontext->height = 1;
    }

    lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
    return 1;
}

s32 flPS2LockTexture(Rect* /* unused */, FLTexture* lpflTexture, plContext* lpcontext, u32 flag, s32 /* unused */) {
    u8* buff_ptr;
    u8* buff_ptr1;
    plContext src;

    lpflTexture->lock_flag = flag;
    lpcontext->desc = lpflTexture->desc;
    lpcontext->width = lpflTexture->width;
    lpcontext->height = lpflTexture->height;

    switch (flag & 3) {
    case 0:
        if (lpflTexture->mem_handle == 0) {
            buff_ptr1 = mflTemporaryUse(lpflTexture->size * 2);
            buff_ptr = buff_ptr1 + lpflTexture->size;
            // Loading an image from VRAM used to be here
        } else {
            buff_ptr = mflTemporaryUse(lpflTexture->size);
            buff_ptr1 = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
        }

        lpflTexture->lock_ptr = (uintptr_t)buff_ptr;
        lpcontext->ptr = buff_ptr;
        src.desc = lpcontext->desc;
        src.width = lpcontext->width;
        src.height = lpcontext->height;
        src.ptr = buff_ptr1;

        switch (lpflTexture->format) {
        case 20:
            lpcontext->bitdepth = 0;
            lpcontext->pitch = lpcontext->width / 2;
            flMemcpy(buff_ptr, buff_ptr1, lpflTexture->size);
            break;

        case 19:
            lpcontext->bitdepth = 1;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            flMemcpy(buff_ptr, buff_ptr1, lpflTexture->size);
            break;

        case 2:
            lpcontext->bitdepth = 2;
            lpcontext->pixelformat.rl = 5;
            lpcontext->pixelformat.rs = 0xA;
            lpcontext->pixelformat.rm = 0x1F;
            lpcontext->pixelformat.gl = 5;
            lpcontext->pixelformat.gs = 5;
            lpcontext->pixelformat.gm = 0x1F;
            lpcontext->pixelformat.bl = 5;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0x1F;
            lpcontext->pixelformat.al = 1;
            lpcontext->pixelformat.as = 0xF;
            lpcontext->pixelformat.am = 1;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            src.bitdepth = 2;
            src.pixelformat.rl = 5;
            src.pixelformat.rs = 0xA;
            src.pixelformat.rm = 0x1F;
            src.pixelformat.gl = 5;
            src.pixelformat.gs = 5;
            src.pixelformat.gm = 0x1F;
            src.pixelformat.bl = 5;
            src.pixelformat.bs = 0;
            src.pixelformat.bm = 0x1F;
            src.pixelformat.al = 1;
            src.pixelformat.as = 0xF;
            src.pixelformat.am = 1;
            src.pixelformat.rs = 0;
            src.pixelformat.bs = 0xA;
            src.pixelformat.gl = 5;
            src.pixelformat.gm = 0x1F;
            src.pitch = src.width * src.bitdepth;
            plConvertContext(lpcontext, &src);
            break;

        case 1:
            lpcontext->bitdepth = 3;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 0;
            lpcontext->pixelformat.as = 0;
            lpcontext->pixelformat.am = 0;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            src.bitdepth = 3;
            src.pixelformat.rl = 8;
            src.pixelformat.rs = 0x10;
            src.pixelformat.rm = 0xFF;
            src.pixelformat.gl = 8;
            src.pixelformat.gs = 8;
            src.pixelformat.gm = 0xFF;
            src.pixelformat.bl = 8;
            src.pixelformat.bs = 0;
            src.pixelformat.bm = 0xFF;
            src.pixelformat.al = 0;
            src.pixelformat.as = 0;
            src.pixelformat.am = 0;
            src.pixelformat.rs = 0;
            src.pixelformat.bs = 0x10;
            src.pitch = src.width * src.bitdepth;
            plConvertContext(lpcontext, &src);
            break;

        case 0:
            lpcontext->bitdepth = 4;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 8;
            lpcontext->pixelformat.as = 0x18;
            lpcontext->pixelformat.am = 0xFF;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            src.bitdepth = 4;
            src.pixelformat.rl = 8;
            src.pixelformat.rs = 0x10;
            src.pixelformat.rm = 0xFF;
            src.pixelformat.gl = 8;
            src.pixelformat.gs = 8;
            src.pixelformat.gm = 0xFF;
            src.pixelformat.bl = 8;
            src.pixelformat.bs = 0;
            src.pixelformat.bm = 0xFF;
            src.pixelformat.al = 8;
            src.pixelformat.as = 0x18;
            src.pixelformat.am = 0xFF;
            src.pixelformat.rs = 0;
            src.pixelformat.bs = 0x10;
            src.pitch = src.width * src.bitdepth;
            plConvertContext(lpcontext, &src);
            break;
        }

        break;

    case 1:
        buff_ptr = mflTemporaryUse(lpflTexture->size);

        if (lpflTexture->mem_handle == 0) {
            // Loading an image from VRAM used to be here
        } else {
            buff_ptr1 = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
            flMemcpy(buff_ptr, buff_ptr1, lpflTexture->size);
        }

        lpflTexture->lock_ptr = (uintptr_t)buff_ptr;
        lpcontext->ptr = buff_ptr;

        switch (lpflTexture->format) {
        case 20:
            lpcontext->bitdepth = 0;
            lpcontext->pitch = lpcontext->width / 2;
            break;

        case 19:
            lpcontext->bitdepth = 1;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 2:
            lpcontext->bitdepth = 2;
            lpcontext->pixelformat.rl = 5;
            lpcontext->pixelformat.rs = 0xA;
            lpcontext->pixelformat.rm = 0x1F;
            lpcontext->pixelformat.gl = 5;
            lpcontext->pixelformat.gs = 5;
            lpcontext->pixelformat.gm = 0x1F;
            lpcontext->pixelformat.bl = 5;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0x1F;
            lpcontext->pixelformat.al = 1;
            lpcontext->pixelformat.as = 0xF;
            lpcontext->pixelformat.am = 1;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0xA;
            lpcontext->pixelformat.gl = 5;
            lpcontext->pixelformat.gm = 0x1F;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 1:
            lpcontext->bitdepth = 3;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 0;
            lpcontext->pixelformat.as = 0;
            lpcontext->pixelformat.am = 0;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0x10;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 0:
            lpcontext->bitdepth = 4;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 8;
            lpcontext->pixelformat.as = 0x18;
            lpcontext->pixelformat.am = 0xFF;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0x10;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;
        }

        break;

    case 2:
        if (lpflTexture->mem_handle == 0) {
            buff_ptr = mflTemporaryUse(lpflTexture->size);
        } else {
            buff_ptr = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
        }

        lpflTexture->lock_ptr = (uintptr_t)buff_ptr;
        lpcontext->ptr = buff_ptr;

        switch (lpflTexture->format) {
        case 20:
            lpcontext->bitdepth = 0;
            lpcontext->pitch = lpcontext->width / 2;
            break;

        case 19:
            lpcontext->bitdepth = 1;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 2:
            lpcontext->bitdepth = 2;
            lpcontext->pixelformat.rl = 5;
            lpcontext->pixelformat.rs = 0xA;
            lpcontext->pixelformat.rm = 0x1F;
            lpcontext->pixelformat.gl = 5;
            lpcontext->pixelformat.gs = 5;
            lpcontext->pixelformat.gm = 0x1F;
            lpcontext->pixelformat.bl = 5;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0x1F;
            lpcontext->pixelformat.al = 1;
            lpcontext->pixelformat.as = 0xF;
            lpcontext->pixelformat.am = 1;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0xA;
            lpcontext->pixelformat.gl = 5;
            lpcontext->pixelformat.gm = 0x1F;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 1:
            lpcontext->bitdepth = 3;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 0;
            lpcontext->pixelformat.as = 0;
            lpcontext->pixelformat.am = 0;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0x10;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 0:
            lpcontext->bitdepth = 4;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 8;
            lpcontext->pixelformat.as = 0x18;
            lpcontext->pixelformat.am = 0xFF;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0x10;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;
        }

        break;

    case 3:
        if (lpflTexture->mem_handle == 0) {
            buff_ptr = mflTemporaryUse(lpflTexture->size);
        } else {
            buff_ptr = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
        }

        lpflTexture->lock_ptr = (uintptr_t)buff_ptr;
        lpcontext->ptr = buff_ptr;

        switch (lpflTexture->format) {
        case 20:
            lpcontext->bitdepth = 0;
            lpcontext->pitch = lpcontext->width / 2;
            break;

        case 19:
            lpcontext->bitdepth = 1;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 2:
            lpcontext->bitdepth = 2;
            lpcontext->pixelformat.rl = 5;
            lpcontext->pixelformat.rs = 0xA;
            lpcontext->pixelformat.rm = 0x1F;
            lpcontext->pixelformat.gl = 5;
            lpcontext->pixelformat.gs = 5;
            lpcontext->pixelformat.gm = 0x1F;
            lpcontext->pixelformat.bl = 5;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0x1F;
            lpcontext->pixelformat.al = 1;
            lpcontext->pixelformat.as = 0xF;
            lpcontext->pixelformat.am = 1;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0xA;
            lpcontext->pixelformat.gl = 5;
            lpcontext->pixelformat.gm = 0x1F;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 1:
            lpcontext->bitdepth = 3;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 0;
            lpcontext->pixelformat.as = 0;
            lpcontext->pixelformat.am = 0;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0x10;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;

        case 0:
            lpcontext->bitdepth = 4;
            lpcontext->pixelformat.rl = 8;
            lpcontext->pixelformat.rs = 0x10;
            lpcontext->pixelformat.rm = 0xFF;
            lpcontext->pixelformat.gl = 8;
            lpcontext->pixelformat.gs = 8;
            lpcontext->pixelformat.gm = 0xFF;
            lpcontext->pixelformat.bl = 8;
            lpcontext->pixelformat.bs = 0;
            lpcontext->pixelformat.bm = 0xFF;
            lpcontext->pixelformat.al = 8;
            lpcontext->pixelformat.as = 0x18;
            lpcontext->pixelformat.am = 0xFF;
            lpcontext->pixelformat.rs = 0;
            lpcontext->pixelformat.bs = 0x10;
            lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
            break;
        }

        break;
    }

    lpcontext->desc = lpcontext->desc | 2;
    return 1;
}

s32 flUnlockTexture(u32 th) {
    FLTexture* lpflTexture = &flTexture[th - 1];

    if (th > FL_TEXTURE_MAX) {
        return 0;
    }

    if (!lpflTexture->be_flag) {
        return 0;
    }

    const s32 ret = flPS2UnlockTexture(lpflTexture);
    SDLGameRenderer_UnlockTexture(th);
    return ret;
}

s32 flUnlockPalette(u32 th) {
    FLTexture* lpflPalette = &flPalette[th - 1];

    if (th > FL_PALETTE_MAX) {
        return 0;
    }

    if (!lpflPalette->be_flag) {
        return 0;
    }

    const s32 ret = flPS2UnlockTexture(lpflPalette);
    SDLGameRenderer_UnlockPalette(th);
    return ret;
}

s32 flPS2UnlockTexture(FLTexture* lpflTexture) {
    uintptr_t trans_ptr;
    u8* buff_ptr;
    u8* buff_ptr1;
    plContext src;
    plContext dst;
    u32 dma_size;
    uintptr_t dma_ptr;
    uintptr_t last_tag;

    trans_ptr = NULL;

    switch (lpflTexture->lock_flag & 3) {
    case 0:
        if (lpflTexture->mem_handle != 0) {
            buff_ptr = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
            buff_ptr1 = (u8*)lpflTexture->lock_ptr;
            trans_ptr = (uintptr_t)buff_ptr;
        } else {
            buff_ptr = mflTemporaryUse(lpflTexture->size);
            buff_ptr1 = (u8*)lpflTexture->lock_ptr;
            trans_ptr = (uintptr_t)buff_ptr;
        }

        src.desc = lpflTexture->desc;
        src.width = lpflTexture->width;
        src.height = lpflTexture->height;
        src.ptr = buff_ptr1;
        dst.desc = lpflTexture->desc;
        dst.width = lpflTexture->width;
        dst.height = lpflTexture->height;
        dst.ptr = buff_ptr;

        switch (lpflTexture->format) {
        case 20:
        case 19:
            flMemcpy(buff_ptr, buff_ptr1, lpflTexture->size);
            break;

        case 2:
            src.bitdepth = 2;
            src.pixelformat.rl = 5;
            src.pixelformat.rs = 0xA;
            src.pixelformat.rm = 0x1F;
            src.pixelformat.gl = 5;
            src.pixelformat.gs = 5;
            src.pixelformat.gm = 0x1F;
            src.pixelformat.bl = 5;
            src.pixelformat.bs = 0;
            src.pixelformat.bm = 0x1F;
            src.pixelformat.al = 1;
            src.pixelformat.as = 0xF;
            src.pixelformat.am = 1;
            src.pitch = src.width * src.bitdepth;
            dst.bitdepth = 2;
            dst.pixelformat.rl = 5;
            dst.pixelformat.rs = 0xA;
            dst.pixelformat.rm = 0x1F;
            dst.pixelformat.gl = 5;
            dst.pixelformat.gs = 5;
            dst.pixelformat.gm = 0x1F;
            dst.pixelformat.bl = 5;
            dst.pixelformat.bs = 0;
            dst.pixelformat.bm = 0x1F;
            dst.pixelformat.al = 1;
            dst.pixelformat.as = 0xF;
            dst.pixelformat.am = 1;
            dst.pixelformat.rs = 0;
            dst.pixelformat.bs = 0xA;
            dst.pixelformat.gl = 5;
            dst.pixelformat.gm = 0x1F;
            dst.pitch = dst.width * dst.bitdepth;
            plConvertContext(&dst, &src);
            break;

        case 1:
            src.bitdepth = 3;
            src.pixelformat.rl = 8;
            src.pixelformat.rs = 0x10;
            src.pixelformat.rm = 0xFF;
            src.pixelformat.gl = 8;
            src.pixelformat.gs = 8;
            src.pixelformat.gm = 0xFF;
            src.pixelformat.bl = 8;
            src.pixelformat.bs = 0;
            src.pixelformat.bm = 0xFF;
            src.pixelformat.al = 0;
            src.pixelformat.as = 0;
            src.pixelformat.am = 0;
            src.pitch = src.width * src.bitdepth;
            dst.bitdepth = 3;
            dst.pixelformat.rl = 8;
            dst.pixelformat.rs = 0x10;
            dst.pixelformat.rm = 0xFF;
            dst.pixelformat.gl = 8;
            dst.pixelformat.gs = 8;
            dst.pixelformat.gm = 0xFF;
            dst.pixelformat.bl = 8;
            dst.pixelformat.bs = 0;
            dst.pixelformat.bm = 0xFF;
            dst.pixelformat.al = 0;
            dst.pixelformat.as = 0;
            dst.pixelformat.am = 0;
            dst.pixelformat.rs = 0;
            dst.pixelformat.bs = 0x10;
            dst.pitch = dst.width * dst.bitdepth;
            plConvertContext(&dst, &src);
            break;

        case 0:
            src.bitdepth = 4;
            src.pixelformat.rl = 8;
            src.pixelformat.rs = 0x10;
            src.pixelformat.rm = 0xFF;
            src.pixelformat.gl = 8;
            src.pixelformat.gs = 8;
            src.pixelformat.gm = 0xFF;
            src.pixelformat.bl = 8;
            src.pixelformat.bs = 0;
            src.pixelformat.bm = 0xFF;
            src.pixelformat.al = 8;
            src.pixelformat.as = 0x18;
            src.pixelformat.am = 0xFF;
            src.pitch = src.width * src.bitdepth;
            dst.bitdepth = 4;
            dst.pixelformat.rl = 8;
            dst.pixelformat.rs = 0x10;
            dst.pixelformat.rm = 0xFF;
            dst.pixelformat.gl = 8;
            dst.pixelformat.gs = 8;
            dst.pixelformat.gm = 0xFF;
            dst.pixelformat.bl = 8;
            dst.pixelformat.bs = 0;
            dst.pixelformat.bm = 0xFF;
            dst.pixelformat.al = 8;
            dst.pixelformat.as = 0x18;
            dst.pixelformat.am = 0xFF;
            dst.pixelformat.rs = 0;
            dst.pixelformat.bs = 0x10;
            dst.pitch = dst.width * dst.bitdepth;
            plConvertContext(&dst, &src);
            break;
        }

        break;

    case 1:
        if (lpflTexture->mem_handle != 0) {
            buff_ptr = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
            buff_ptr1 = (u8*)lpflTexture->lock_ptr;
            flMemcpy(buff_ptr, buff_ptr1, lpflTexture->size);
            trans_ptr = (uintptr_t)buff_ptr;
        } else {
            buff_ptr = (u8*)lpflTexture->lock_ptr;
            trans_ptr = (uintptr_t)buff_ptr;
        }

        break;

    case 2:
        trans_ptr = lpflTexture->lock_ptr;
        break;

    case 3:
        trans_ptr = lpflTexture->lock_ptr;
        break;
    }

    lpflTexture->desc &= ~2;

    if (trans_ptr != 0) {
        if (lpflTexture->flag == 1 || lpflTexture->flag == 4) {
            flPS2DeleteVramList(lpflTexture);
            return 1;
        }
    }

    return 1;
}

s16 flPS2GetTextureBuffWidth(s16 width) {
    s32 i = 1;

    while (1) {
        if ((1 << i) >= width) {
            return i;
        }

        i += 1;

        if (i > 10) {
            return 1;
        }
    }
}

u32 flPS2GetTextureSize(u32 format, s32 dw, s32 dh, s32 bnum) {
    u32 tex_size;
    s32 lp0;

    tex_size = 0;

    for (lp0 = 0; lp0 < bnum; lp0++) {
        switch (format) {
        case 0:
        case 1:
            tex_size += dw * dh * 4;
            break;

        case 2:
        case 10:
            tex_size += dw * dh * 2;
            break;

        case 19:
            tex_size += dw * dh;
            break;

        case 20:
            tex_size += (dw * dh) >> 1;
            break;
        }

        dw >>= 1;
        dh >>= 1;
    }

    return tex_size;
}

s16 flPS2GetTextureVramBlock(FLTexture* lpflTexture) {
    s16 vram_block;
    s16 w;
    s16 h;
    s16 dw;
    s16 dh;
    s32 lp0;

    switch (lpflTexture->format) {
    case 0:
    case 1:
        w = 0x40;
        h = 0x20;
        break;

    case 2:
    case 10:
        w = 0x40;
        h = 0x40;
        break;

    case 19:
        w = 0x80;
        h = 0x40;
        break;

    case 20:
        w = 0x80;
        h = 0x80;
        break;
    }

    dw = lpflTexture->width;
    dh = lpflTexture->height;
    vram_block = 0;

    for (lp0 = 0; lp0 < lpflTexture->tex_num; lp0++) {
        vram_block += ((dw + w - 1) / w) * ((dh + h - 1) / h) << 5;
        dw >>= 1;
        dh >>= 1;
    }

    return vram_block;
}

s16 flPS2GetPaletteVramBlock(FLTexture* lpflPalette) {
    s16 vram_block;

    if (lpflPalette->height == 1) {
        vram_block = 2;
    } else {
        switch (lpflPalette->format) {
        case 0:
        case 1:
            vram_block = 4;
            break;

        case 2:
            vram_block = 4;
            break;
        }
    }

    return vram_block;
}

s32 flPS2ConvertTextureFromContext(plContext* lpcontext, FLTexture* lpflTexture, u32 type) {
    s32 lp0;
    s32 dw;
    s32 dh;
    plContext tcon;
    u8* dst_ptr;
    s32 tex_size;

    lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    dst_ptr = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    tcon.bitdepth = lpcontext->bitdepth;
    tcon.desc = lpcontext->desc;
    dw = lpflTexture->width;
    dh = lpflTexture->height;

    for (lp0 = 0; lp0 < lpflTexture->tex_num; lp0++) {
        tcon.width = dw;
        tcon.height = dh;
        tcon.pitch = tcon.width * tcon.bitdepth;

        switch (lpflTexture->format) {
        default:
            flLogOut("Not supported texture bit depth @flPS2ConvertTextureFromContext");
            break;

        case SCE_GS_PSMT4:
            tex_size = (dw * dh) >> 1;

            if (lpflTexture->dma_type == 0) {
                flMemcpy(dst_ptr, lpcontext->ptr, lpflTexture->size);
            } else {
                tcon.ptr = mflTemporaryUse(tex_size);
                flMemcpy(tcon.ptr, lpcontext->ptr, tex_size);
                flPS2Conv4_8_32(dw, dh, tcon.ptr, dst_ptr, 0);
            }

            break;

        case SCE_GS_PSMT8:
            tex_size = dw * dh;

            if (lpflTexture->dma_type == 0) {
                flMemcpy(dst_ptr, lpcontext->ptr, lpflTexture->size);
            } else {
                tcon.ptr = mflTemporaryUse(tex_size);
                flMemcpy(tcon.ptr, lpcontext->ptr, tex_size);
                flPS2Conv4_8_32(dw, dh, tcon.ptr, dst_ptr, 1);
            }

            break;

        case SCE_GS_PSMCT16:
            tex_size = dw * dh * 2;
            tcon.ptr = dst_ptr;
            tcon.pixelformat.rl = 5;
            tcon.pixelformat.rs = 0xA;
            tcon.pixelformat.rm = 0x1F;
            tcon.pixelformat.gl = 5;
            tcon.pixelformat.gs = 5;
            tcon.pixelformat.gm = 0x1F;
            tcon.pixelformat.bl = 5;
            tcon.pixelformat.bs = 0;
            tcon.pixelformat.bm = 0x1F;
            tcon.pixelformat.al = 1;
            tcon.pixelformat.as = 0xF;
            tcon.pixelformat.am = 1;
            tcon.pixelformat.rs = 0;
            tcon.pixelformat.bs = 0xA;
            tcon.pixelformat.gl = 5;
            tcon.pixelformat.gm = 0x1F;
            flPS2ConvertContext(lpcontext, &tcon, 0, type);
            break;

        case SCE_GS_PSMCT24:
            tex_size = dw * dh * 4;
            tcon.ptr = dst_ptr;
            tcon.pixelformat.rl = 8;
            tcon.pixelformat.rs = 0x10;
            tcon.pixelformat.rm = 0xFF;
            tcon.pixelformat.gl = 8;
            tcon.pixelformat.gs = 8;
            tcon.pixelformat.gm = 0xFF;
            tcon.pixelformat.bl = 8;
            tcon.pixelformat.bs = 0;
            tcon.pixelformat.bm = 0xFF;
            tcon.pixelformat.al = 0;
            tcon.pixelformat.as = 0;
            tcon.pixelformat.am = 0;
            tcon.pixelformat.rs = 0;
            tcon.pixelformat.bs = 0x10;
            flPS2ConvertContext(lpcontext, &tcon, 0, type);
            break;

        case SCE_GS_PSMCT32:
            tex_size = dw * dh * 4;
            tcon.ptr = dst_ptr;
            tcon.pixelformat.rl = 8;
            tcon.pixelformat.rs = 0x10;
            tcon.pixelformat.rm = 0xFF;
            tcon.pixelformat.gl = 8;
            tcon.pixelformat.gs = 8;
            tcon.pixelformat.gm = 0xFF;
            tcon.pixelformat.bl = 8;
            tcon.pixelformat.bs = 0;
            tcon.pixelformat.bm = 0xFF;
            tcon.pixelformat.al = 8;
            tcon.pixelformat.as = 0x18;
            tcon.pixelformat.am = 0xFF;
            tcon.pixelformat.rs = 0;
            tcon.pixelformat.bs = 0x10;
            flPS2ConvertContext(lpcontext, &tcon, 0, type);
            break;
        }

        dst_ptr = &dst_ptr[tex_size];
        dw >>= 1;
        dh >>= 1;
        lpcontext++;
    }

    return 1;
}

s32 flPS2ConvertContext(plContext* lpSrc, plContext* lpDst, u32 direction, u32 type) {
    s32 x;
    s32 y;
    u32 r;
    u32 g;
    u32 b;
    u32 a;
    u32 color;
    u32 wk0;
    u32 wk1;
    u8* keep_src;
    u8* keep_dst;
    u8* src;
    u8* dst;

    static u8 clut_tbl[32] = { 0, 1, 2,  3,  4,  5,  6,  7,  16, 17, 18, 19, 20, 21, 22, 23,
                               8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29, 30, 31 };

    keep_src = lpSrc->ptr;
    keep_dst = lpDst->ptr;
    wk0 = 0;
    wk1 = 0;

    for (y = 0; y < lpDst->height; y++) {
        for (x = 0; x < lpDst->width; x++) {
            if ((type == 1) && (direction == 1)) {
                src = keep_src;
                src += lpSrc->bitdepth * ((wk0 & 0xE0) + clut_tbl[wk0 & 0x1F]);
            } else {
                src = keep_src + wk1;
                wk1 += lpSrc->bitdepth;
            }

            switch (lpSrc->bitdepth) {
            case 2:
                color = ((u16*)src)[0];
                break;

            case 3:
                color = (src[2] << 16) | (src[1] << 8) | src[0];
                break;

            case 4:
                color = ((u32*)src)[0];
                break;
            }

            r = (lpSrc->pixelformat.rm & (color >> lpSrc->pixelformat.rs)) << (8 - lpSrc->pixelformat.rl);
            g = (lpSrc->pixelformat.gm & (color >> lpSrc->pixelformat.gs)) << (8 - lpSrc->pixelformat.gl);
            b = (lpSrc->pixelformat.bm & (color >> lpSrc->pixelformat.bs)) << (8 - lpSrc->pixelformat.bl);
            a = (lpSrc->pixelformat.am & (color >> lpSrc->pixelformat.as)) << (8 - lpSrc->pixelformat.al);

            if ((type == 1) && (direction == 0)) {
                dst = keep_dst;
                dst += lpDst->bitdepth * ((wk0 & 0xE0) + clut_tbl[wk0 & 0x1F]);
            } else {
                dst = keep_dst + (lpDst->pitch * y) + (lpDst->bitdepth * x);
            }

            if (lpSrc->bitdepth == 4) {
                if (direction == 0) {
                    if (a == 0xFF) {
                        a = 0x80;
                    } else if (a != 0) {
                        a >>= 1;

                        if (a == 0) {
                            a = 1;
                        }
                    }
                } else if (a == 0x80) {
                    a = 0xFF;
                } else {
                    a *= 2;
                }
            }

            color = ((lpDst->pixelformat.am & (a >> (8 - lpDst->pixelformat.al))) << lpDst->pixelformat.as) |
                    (((lpDst->pixelformat.bm & (b >> (8 - lpDst->pixelformat.bl))) << lpDst->pixelformat.bs) |
                     (((lpDst->pixelformat.rm & (r >> (8 - lpDst->pixelformat.rl))) << lpDst->pixelformat.rs) |
                      ((lpDst->pixelformat.gm & (g >> (8 - lpDst->pixelformat.gl))) << lpDst->pixelformat.gs)));

            switch (lpSrc->bitdepth) {
            case 2:
                ((u16*)dst)[0] = color;
                break;

            case 3:
                dst[0] = r;
                dst[1] = g;
                dst[2] = b;
                break;

            case 4:
                ((u32*)dst)[0] = color;
                break;
            }

            wk0 += 1;
        }
    }

    return 1;
}

// data

static s32 column_tbl4[2][128] = {
    { 0,   260, 8,   268, 16,  276, 24,  284, 1,   261, 9,   269, 17,  277, 25,  285, 2,   262, 10,  270, 18,  278,
      26,  286, 3,   263, 11,  271, 19,  279, 27,  287, 4,   256, 12,  264, 20,  272, 28,  280, 5,   257, 13,  265,
      21,  273, 29,  281, 6,   258, 14,  266, 22,  274, 30,  282, 7,   259, 15,  267, 23,  275, 31,  283, 128, 388,
      136, 396, 144, 404, 152, 412, 129, 389, 137, 397, 145, 405, 153, 413, 130, 390, 138, 398, 146, 406, 154, 414,
      131, 391, 139, 399, 147, 407, 155, 415, 132, 384, 140, 392, 148, 400, 156, 408, 133, 385, 141, 393, 149, 401,
      157, 409, 134, 386, 142, 394, 150, 402, 158, 410, 135, 387, 143, 395, 151, 403, 159, 411 },
    { 4,   256, 12,  264, 20,  272, 28,  280, 5,   257, 13,  265, 21,  273, 29,  281, 6,   258, 14,  266, 22,  274,
      30,  282, 7,   259, 15,  267, 23,  275, 31,  283, 0,   260, 8,   268, 16,  276, 24,  284, 1,   261, 9,   269,
      17,  277, 25,  285, 2,   262, 10,  270, 18,  278, 26,  286, 3,   263, 11,  271, 19,  279, 27,  287, 132, 384,
      140, 392, 148, 400, 156, 408, 133, 385, 141, 393, 149, 401, 157, 409, 134, 386, 142, 394, 150, 402, 158, 410,
      135, 387, 143, 395, 151, 403, 159, 411, 128, 388, 136, 396, 144, 404, 152, 412, 129, 389, 137, 397, 145, 405,
      153, 413, 130, 390, 138, 398, 146, 406, 154, 414, 131, 391, 139, 399, 147, 407, 155, 415 }
};

static s32 column_tbl8[2][64] = {
    { 0,   260, 8,   268, 1,   261, 9,   269, 2,   262, 10,  270, 3,   263, 11,  271, 4,   256, 12,  264, 5,   257,
      13,  265, 6,   258, 14,  266, 7,   259, 15,  267, 128, 388, 136, 396, 129, 389, 137, 397, 130, 390, 138, 398,
      131, 391, 139, 399, 132, 384, 140, 392, 133, 385, 141, 393, 134, 386, 142, 394, 135, 387, 143, 395 },
    { 4,   256, 12,  264, 5,   257, 13,  265, 6,   258, 14,  266, 7,   259, 15,  267, 0,   260, 8,   268, 1,   261,
      9,   269, 2,   262, 10,  270, 3,   263, 11,  271, 132, 384, 140, 392, 133, 385, 141, 393, 134, 386, 142, 394,
      135, 387, 143, 395, 128, 388, 136, 396, 129, 389, 137, 397, 130, 390, 138, 398, 131, 391, 139, 399 }
};

// sdata

static s32 idx32_h[32] = { 0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3,
                           4, 5, 4, 5, 6, 7, 6, 7, 4, 5, 4, 5, 6, 7, 6, 7 };

static s32 idx32_v[32] = { 0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3,
                           0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3 };

static s32 block_table4[32] = { 0,  2,  8,  10, 1,  3,  9,  11, 4,  6,  12, 14, 5,  7,  13, 15,
                                16, 18, 24, 26, 17, 19, 25, 27, 20, 22, 28, 30, 21, 23, 29, 31 };

static s32 block_table8[32] = { 0, 1, 4,  5,  16, 17, 20, 21, 2,  3,  6,  7,  18, 19, 22, 23,
                                8, 9, 12, 13, 24, 25, 28, 29, 10, 11, 14, 15, 26, 27, 30, 31 };

static s32* clm_tbl4_ptr[4] = { column_tbl4[0], column_tbl4[1], column_tbl4[0], column_tbl4[1] };
static s32* clm_tbl8_ptr[4] = { column_tbl8[0], column_tbl8[1], column_tbl8[0], column_tbl8[1] };

s32 flPS2Conv4_8_32(s32 width, s32 height, u8* p_input, u8* p_output, s32 n_bit) {
    switch (n_bit) {
    case 0:
        Conv4to32(width, height, p_input, p_output);
        return 0;

    case 1:
        Conv8to32(width, height, p_input, p_output);
        return 0;

    default:
        return -1;
    }
}

static void Conv4to32(s32 width, s32 height, u8* p_input, u8* p_output) {
    s32 i;
    s32 j;
    s32 k;
    s32 n_page_h;
    s32 n_page_w;
    s32 p_page_w;
    s32 n_input_width;
    s32 n_output_width;
    s32 n_input_height;
    s32 n_output_height;
    u8* pi0;
    u8* pi1;
    u8* po;
    u8 input_page[8192];

    n_page_w = p_page_w = (width - 1) / 128 + 1;
    n_page_h = (height - 1) / 128 + 1;
    n_input_width = 0x40;
    n_output_width = 0x100;
    n_input_height = 0x80;
    n_output_height = 0x20;

    for (i = 0; i < n_page_h; i++) {
        for (j = 0; j < n_page_w; j++) {
            pi0 = p_input + (n_input_width * n_input_height * i * n_page_w) + (j * n_input_width);
            pi1 = input_page;

            for (k = 0; k < n_input_height; k++) {
                memcpy(pi1, pi0, n_input_width);
                pi0 += n_input_width * n_page_w;
                pi1 += n_input_width;
            }

            po = p_output + (n_output_width * n_output_height * i * n_page_w) + (j * n_output_width);
            PageConv4to32(input_page, po, p_page_w);
        }
    }
}

void PageConv4to32(u8* p_input, u8* p_output, s32 p_page_w) {
    s32 i;
    s32 j;
    s32 nb;
    s32* tbl;
    u8* pi;
    u8* po;
    s32 n_width = 4;
    s32 n_height = 8;
    s32 i_size = 0x40;
    s32 o_size = 0x100;

    tbl = block_table4;

    for (i = 0; i < n_height; i++) {
        for (j = 0; j < n_width; j++) {
            nb = *tbl++;
            pi = p_input + (i * 0x10 * i_size) + (j * 0x10);
            po = p_output + (idx32_v[nb] * 8 * o_size * p_page_w) + (idx32_h[nb] << 5);
            BlockConv4to32(pi, po, p_page_w);
        }
    }
}

static void BlockConv4to32(u8* p_input, u8* p_output, s32 p_page_w) {
    s32 i;
    s32 j;
    s32 k;
    s32 cno;
    s32** pTbl;
    s32* tbl;
    u8* pIn;
    u8* pOut;
    u8 ld;
    u8 ud;

    pIn = p_input;
    pOut = p_output;
    pTbl = clm_tbl4_ptr;

    for (k = 0; k < 4; k++) {
        tbl = *pTbl++;

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 0x20; j++) {
                cno = *tbl++;

                if (!(cno & 1)) {
                    ld = pIn[cno / 2] & 0xF;
                } else {
                    ld = (pIn[cno / 2] & 0xF0) >> 4;
                }

                cno = *tbl++;

                if (!(cno & 1)) {
                    ud = (pIn[cno / 2] & 0xF) * 0x10;
                } else {
                    ud = pIn[cno / 2] & 0xF0;
                }

                *pOut++ = ld | ud;
            }

            pOut += (p_page_w << 8) - 0x20;
        }

        pIn += 0x100;
    }
}

static void Conv8to32(s32 width, s32 height, u8* p_input, u8* p_output) {
    s32 i;
    s32 j;
    s32 k;
    s32 n_page_h;
    s32 n_page_w;
    s32 p_page_w;
    s32 n_input_width;
    s32 n_output_width;
    s32 n_input_height;
    s32 n_output_height;
    u8* pi0;
    u8* pi1;
    u8* po;
    u8 input_page[8192];

    p_page_w = n_page_w = (width - 1) / 128 + 1;
    n_page_h = (height - 1) / 64 + 1;
    n_input_width = 0x80;
    n_output_width = 0x100;
    n_input_height = 0x40;
    n_output_height = 0x20;

    for (i = 0; i < n_page_h; i++) {
        for (j = 0; j < n_page_w; j++) {
            pi0 = p_input + (n_input_width * n_input_height * i * n_page_w) + (j * n_input_width);
            pi1 = input_page;

            for (k = 0; k < n_input_height; k++) {
                memcpy(pi1, pi0, n_input_width);
                pi0 += n_input_width * n_page_w;
                pi1 += n_input_width;
            }

            po = p_output + (n_output_width * n_output_height * i * n_page_w) + (j * n_output_width);
            PageConv8to32(input_page, po, p_page_w);
        }
    }
}

static void PageConv8to32(u8* p_input, u8* p_output, s32 p_page_w) {
    s32 i;
    s32 j;
    s32 nb;
    s32* tbl;
    u8* pi;
    u8* po;
    s32 n_width = 8;
    s32 n_height = 4;
    s32 i_size = 0x80;
    s32 o_size = 0x100;

    tbl = block_table8;

    for (i = 0; i < n_height; i++) {
        for (j = 0; j < n_width; j++) {
            nb = *tbl++;
            pi = p_input + (i * 0x10 * i_size) + (j * 0x10);
            po = p_output + (idx32_v[nb] * 8 * o_size * p_page_w) + (idx32_h[nb] << 5);
            BlockConv8to32(pi, po, p_page_w);
        }
    }
}

void BlockConv8to32(u8* p_input, u8* p_output, s32 p_page_w) {
    s32 i;
    s32 j;
    s32 k;
    s32** pTbl;
    s32* tbl;
    u8* pIn = p_input;
    u8* pOut = p_output;

    pTbl = clm_tbl8_ptr;

    for (k = 0; k < 4; k++) {
        tbl = *pTbl++;

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 0x20; j++) {
                *pOut++ = pIn[*tbl++];
            }

            pOut += (p_page_w << 8) - 0x20;
        }

        pIn += 0x200;
    }
}

void flPS2VramInit() {
    s32 i;

    flVramStaticNum = 0;

    for (i = 0; i < VRAM_BLOCK_HEADER_SIZE; i++) {
        flMemset(&flVramStatic[i], 0, sizeof(VRAMBlockHeader));
    }

    flVramList = NULL;

    for (i = 0; i < VRAM_CONTROL_SIZE; i++) {
        flMemset(&flVramControl[i], 0, sizeof(LPVram));
    }
}

#if defined(TARGET_PS2)
LPVram* flPS2PullVramWork()
#else
LPVram* flPS2PullVramWork(LPVram* /* unused */, s32 /* unused */)
#endif
{
    s32 i;

    for (i = 0; i < VRAM_CONTROL_SIZE; i++) {
        if (flVramControl[i].tbp == 0) {
            return &flVramControl[i];
        }
    }

    return NULL;
}

void flPS2PushVramWork(LPVram* lpVram) {
    LPVram* lpParent = (LPVram*)lpVram->parent;
    LPVram* lpChild = (LPVram*)lpVram->child;

    if (lpParent != 0) {
        lpParent->child = (uintptr_t)lpChild;
    } else if (lpChild != 0) {
        flVramList = (LPVram*)lpChild;
    } else {
        flVramList = NULL;
    }

    if (lpChild != 0) {
        lpChild->parent = (uintptr_t)lpParent;
    }

    if (lpVram->tex_ptr != NULL) {
        lpVram->tex_ptr->vram_on_flag = 0;
        lpVram->tex_ptr->wkVram = NULL;
        lpVram->tex_ptr->flag = 4;
    }

    flMemset(lpVram, 0, sizeof(LPVram));
}

void flPS2ChainVramWork(LPVram* lpParent, LPVram* lpVram) {
    LPVram* lpChild = NULL;

    if (lpParent != NULL) {
        lpChild = (LPVram*)lpParent->child;
        lpParent->child = (uintptr_t)lpVram;
    } else if (flVramList == NULL) {
        flVramList = lpVram;
    } else {
        lpChild = flVramList;
        flVramList = lpVram;
    }

    lpVram->parent = (uintptr_t)lpParent;
    lpVram->child = (uintptr_t)lpChild;

    if (lpChild != 0) {
        lpChild->parent = (uintptr_t)lpVram;
    }
}

s16 flPS2GetVramSize() {
    LPVram* lpVram = flVramList;
    s16 size = 0;

    while (lpVram != NULL) {
        if (lpVram->tbp == 0) {
            flPS2SystemError(0, "ERROR flPS2GetVramSize flps2vram.c 1");
        }

        size += lpVram->block_size;
        lpVram = (LPVram*)lpVram->child;
    }

    return size;
}

s32 flPS2AddVramList(LPVram* lpVram, FLTexture* lpflTexture) {
    s16 next_tbp;
    LPVram* lpVramNew;

    if (lpVram == NULL) {
        if ((lpVramNew = flPS2PullVramWork(flVramControl, VRAM_CONTROL_SIZE)) == NULL) {
            return 0;
        }

        flPS2ChainVramWork(lpVram, lpVramNew);
        next_tbp = flPs2State.TextureStartAdrs;
    } else {
        if ((lpVramNew = flPS2PullVramWork(flVramControl, VRAM_CONTROL_SIZE)) == NULL) {
            return 0;
        }

        flPS2ChainVramWork(lpVram, lpVramNew);
        // Should probably use an alignment macro here
        next_tbp =
            ~(lpflTexture->block_align - 1) & (lpflTexture->block_align - 1 + (lpVram->tbp + lpVram->block_size));
    }

    if ((next_tbp >= 0) && (next_tbp < flPs2State.TextureStartAdrs)) {
        flPS2SystemError(0, "ERROR flPS2AddVramList flps2vram.c 1");
    }

    if (next_tbp >= 0x4000) {
        flPS2SystemError(0, "ERROR flPS2AddVramList flps2vram.c 2");
    }

    lpflTexture->tbp = next_tbp;
    lpflTexture->vram_on_flag = 1;
    lpflTexture->wkVram = lpVramNew;
    lpVramNew->tex_ptr = lpflTexture;
    lpVramNew->desc = lpflTexture->desc;
    lpVramNew->flag = lpflTexture->flag;
    lpVramNew->tbp = next_tbp;
    lpVramNew->block_size = lpflTexture->block_size;
    lpVramNew->block_align = lpflTexture->block_align;
    return 1;
}

s32 flPS2RewriteVramList(LPVram* lpVram, FLTexture* lpflTexture) {
    LPVram* lpVramNext;

    if (lpVram == NULL) {
        if ((lpVram = flPS2PullVramWork(flVramControl, VRAM_CONTROL_SIZE)) == NULL) {
            return 0;
        }

        flPS2ChainVramWork(NULL, lpVram);
        lpVram->tbp = flPs2State.TextureStartAdrs;
    } else if (lpVram->child != 0) {
        lpVramNext = (LPVram*)lpVram->child;

        while ((lpVramNext->tbp - lpVram->tbp) < lpflTexture->block_size) {
            flPS2PushVramWork((LPVram*)lpVramNext);

            if (lpVram->child == 0) {
                break;
            }

            lpVramNext = (LPVram*)lpVram->child;
        }
    }

    if (lpVram->tex_ptr != NULL) {
        lpVram->tex_ptr->vram_on_flag = 0;
        lpVram->tex_ptr->wkVram = NULL;
        lpVram->tex_ptr->flag = 4;
    }

    lpflTexture->tbp = lpVram->tbp;
    lpflTexture->vram_on_flag = 1;
    lpflTexture->wkVram = lpVram;
    lpVram->tex_ptr = lpflTexture;
    lpVram->desc = lpflTexture->desc;
    lpVram->flag = lpflTexture->flag;
    lpVram->block_size = lpflTexture->block_size;
    lpVram->block_align = lpflTexture->block_align;
    return 1;
}

s32 flPS2DeleteVramList(FLTexture* lpflTexture) {
    if (lpflTexture->vram_on_flag) {
        flPS2PushVramWork(lpflTexture->wkVram);
    }

    return 1;
}

void flPS2PurgeTextureFromVRAM(u32 th) {
    FLTexture* lpflTexture = &flTexture[th - 1];

    if (th > FL_TEXTURE_MAX) {
        ERR_STOP;
    }

    if (!lpflTexture->be_flag) {
        ERR_STOP;
    }

    if (lpflTexture->vram_on_flag) {
        flPS2PushVramWork(lpflTexture->wkVram);
    }
}

void flPS2PurgePaletteFromVRAM(u32 ph) {
    FLTexture* lpflPalette = &flPalette[ph - 1];

    if (ph > FL_PALETTE_MAX) {
        ERR_STOP;
    }

    if (!lpflPalette->be_flag) {
        ERR_STOP;
    }

    if (lpflPalette->vram_on_flag) {
        flPS2PushVramWork(lpflPalette->wkVram);
    }
}

s32 flPS2GetVramFreeArea(u32* lpflhTexture, s32 tex_num) {
    LPVram* lpVram;
    FLTexture* lpflTexture;
    FLTexture* lpflPalette;
    u32 th;
    s32 i;

    for (i = 0; i < tex_num; i++) {
        th = *lpflhTexture++;

        if (LO_16_BITS(th)) {
            lpflTexture = &flTexture[LO_16_BITS(th) - 1];

            if (lpflTexture->vram_on_flag == 0) {
                if ((lpVram = flPS2SearchVramSpace(lpflTexture->block_size, lpflTexture->block_align)) ==
                    LPVRAM_ERROR) {
                    return 0;
                }

                if (flPS2AddVramList(lpVram, lpflTexture) == 0) {
                    return 0;
                }
            }
        }

        if (HI_16_BITS(th) != 0) {
            lpflPalette = &flPalette[HI_16_BITS(th) - 1];

            if (lpflPalette->vram_on_flag == 0) {
                if ((lpVram = flPS2SearchVramSpace(lpflPalette->block_size, lpflPalette->block_align)) ==
                    LPVRAM_ERROR) {
                    return 0;
                }

                if (flPS2AddVramList(lpVram, lpflPalette) == 0) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

LPVram* flPS2SearchVramSpace(u32 size, u32 align) {
    LPVram* lpVram = flVramList;
    LPVram* lpChild;
    u32 next_tbp;

    if (lpVram == NULL) {
        return NULL;
    }

    if ((lpVram->tbp != flPs2State.TextureStartAdrs) && ((lpVram->tbp - flPs2State.TextureStartAdrs) >= size)) {
        return NULL;
    }

    while (1) {
        lpChild = (LPVram*)lpVram->child;
        next_tbp = lpVram->tbp + lpVram->block_size;
        next_tbp = ~(align - 1) & (next_tbp + (align - 1)); // Should probably use an alignment macro here

        if (lpChild == 0) {
            if ((lpVram->tbp + lpVram->block_size) >= 0x4000) {
                return LPVRAM_ERROR;
            }

            if ((next_tbp + size) >= 0x4000) {
                return LPVRAM_ERROR;
            }

            return lpVram;
        }

        if ((next_tbp < lpChild->tbp) && ((lpChild->tbp - next_tbp) >= size)) {
            return lpVram;
        }

        lpVram = (LPVram*)lpChild;
    }
}

LPVram* flPS2SearchVramChange(FLTexture* lpflTexture, u32* lpflhTexture, s32 tex_num) {
    s32 i;
    LPVram* lpVram;
    LPVram* lpVramNext;
    LPVram* lpVramKeep;
    LPVram* lpVramOld;
    FLTexture* lpPs2tex;
    u32 th;
    s32 loop_num = 0;
    u32 next_tbp;

    while (1) {
        lpVram = flVramList;

        if (lpVram == NULL) {
            return NULL;
        }

        while (1) {
            if ((loop_num != 0) || (lpVram->flag == 1) || (lpVram->flag == 4)) {
                for (i = 0; i < tex_num; i++) {
                    th = lpflhTexture[i];

                    if (LO_16_BITS(th)) {
                        lpPs2tex = &flTexture[LO_16_BITS(th) - 1];

                        if (lpPs2tex == lpVram->tex_ptr) {
                            goto block_38;
                        }
                    }

                    if (HI_16_BITS(th)) {
                        lpPs2tex = &flPalette[HI_16_BITS(th) - 1];

                        if (lpPs2tex == lpVram->tex_ptr) {
                            goto block_38;
                        }
                    }
                }

                next_tbp = ~(lpflTexture->block_align - 1) & (lpVram->tbp + (lpflTexture->block_align - 1));

                if (lpVram->tbp == next_tbp) {
                    if (lpVram->child == 0) {
                        if ((0x4000 - next_tbp) >= lpflTexture->block_size) {
                            return lpVram;
                        }

                        goto block_38;
                    }

                    lpVramKeep = lpVram;
                    lpVramNext = (LPVram*)lpVram->child;

                    while ((lpVramNext->tbp - next_tbp) < lpflTexture->block_size) {
                        lpVram = lpVramNext;

                        if ((loop_num == 0) && (lpVram->flag != 1)) {
                            if (lpVram->flag != 4) {
                                goto block_38;
                            }
                        }

                        for (i = 0; i < tex_num; i++) {
                            th = lpflhTexture[i];

                            if (LO_16_BITS(th)) {
                                lpPs2tex = &flTexture[LO_16_BITS(th) - 1];

                                if (lpPs2tex == lpVram->tex_ptr) {
                                    goto block_38;
                                }
                            }

                            if (HI_16_BITS(th)) {
                                lpPs2tex = &flPalette[HI_16_BITS(th) - 1];

                                if (lpPs2tex == lpVram->tex_ptr) {
                                    goto block_38;
                                }
                            }
                        }

                        if (lpVram->child == 0) {
                            if ((0x4000 - next_tbp) < lpflTexture->block_size) {
                                goto block_38;
                            }

                            return lpVramKeep;
                        }

                        lpVramNext = (LPVram*)lpVram->child;
                    }

                    return lpVramKeep;
                }
            }

        block_38:
            lpVramOld = lpVram;
            lpVram = (LPVram*)lpVram->child;

            if (lpVram == NULL) {
                break;
            }
        }

        if ((0x4000 - (lpVramOld->tbp + lpVramOld->block_size)) >= lpflTexture->block_size) {
            return (LPVram*)1;
        }

        if (loop_num != 0) {
            break;
        }

        loop_num += 1;
    }

    return LPVRAM_ERROR;
}
