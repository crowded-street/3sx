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

#define ERR_STOP                                                                                                       \
    while (1) {}

static s32 flPS2ConvertTextureFromContext(plContext* lpcontext, FLTexture* lpflTexture, u32 type);
u32 flPS2GetTextureSize(u32 format, s32 dw, s32 dh, s32 bnum);
s32 flPS2LockTexture(Rect* /* unused */, FLTexture* lpflTexture, plContext* lpcontext, u32 flag, s32 /* unused */);
s32 flPS2UnlockTexture(FLTexture*);

u32 flCreateTextureHandle(plContext* bits, u32 flag) {
    FLTexture* lpflTexture;
    u32 th = flPS2GetTextureHandle();

    if (th == 0) {
        return 0;
    }

    lpflTexture = &flTexture[LO_16_BITS(th) - 1];
    flPS2GetTextureInfoFromContext(bits, 1, th, flag);

    if (bits->ptr == NULL) {
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
    lpflPalette->mem_handle = 0;
    lpflPalette->lock_ptr = 0;
    lpflPalette->lock_flag = 0;
    lpflPalette->tex_num = 1;
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
    u8* buff_ptr;
    u8* buff_ptr1;
    plContext src;
    plContext dst;

    switch (lpflTexture->lock_flag & 3) {
    case 0:
        if (lpflTexture->mem_handle != 0) {
            buff_ptr = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
            buff_ptr1 = (u8*)lpflTexture->lock_ptr;
        } else {
            buff_ptr = mflTemporaryUse(lpflTexture->size);
            buff_ptr1 = (u8*)lpflTexture->lock_ptr;
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
        } else {
            buff_ptr = (u8*)lpflTexture->lock_ptr;
        }

        break;

    case 2:
    case 3:
        break;
    }

    lpflTexture->desc &= ~2;

    return 1;
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
            flMemcpy(dst_ptr, lpcontext->ptr, lpflTexture->size);

            break;

        case SCE_GS_PSMT8:
            tex_size = dw * dh;
            flMemcpy(dst_ptr, lpcontext->ptr, lpflTexture->size);

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
