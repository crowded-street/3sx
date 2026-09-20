#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/common/prilay.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include "core/renderer.h"

#include <SDL3/SDL.h>

#include <libgraph.h>

#include <assert.h>
#include <memory.h>

#define ERR_STOP                                                                                                       \
    while (1) {}

static s32 flPS2ConvertTextureFromContext(plContext* lpcontext, FLTexture* lpflTexture, u32 type);
u32 flPS2GetTextureSize(u32 format, s32 dw, s32 dh, s32 bnum);
/* What locking a texture or a palette needs. The first and last parameters of
 * flPS2LockTexture were unused and are kept as fields so the two call sites
 * still pass exactly what they passed. */
typedef struct {
    Rect* unused_rect;
    FLTexture* lpflTexture;
    plContext* lpcontext;
    u32 flag;
    s32 unused;
} FlLockArgs;

s32 flPS2LockTexture(const FlLockArgs* a);
s32 flPS2UnlockTexture(FLTexture*);

/* The three pixel layouts this file sets, each exactly the run of assignments
 * that stood at every one of its call sites. */

/* the 16-bit RGBA5551 layout */
static void set_pixelformat_rgba5551(plContext* c) {
    c->pixelformat.rl = 5;
    c->pixelformat.rs = 0xA;
    c->pixelformat.rm = 0x1F;
    c->pixelformat.gl = 5;
    c->pixelformat.gs = 5;
    c->pixelformat.gm = 0x1F;
    c->pixelformat.bl = 5;
    c->pixelformat.bs = 0;
    c->pixelformat.bm = 0x1F;
    c->pixelformat.al = 1;
    c->pixelformat.as = 0xF;
    c->pixelformat.am = 1;
}

/* the 24-bit RGB888 layout */
static void set_pixelformat_rgb888(plContext* c) {
    c->pixelformat.rl = 8;
    c->pixelformat.rs = 0x10;
    c->pixelformat.rm = 0xFF;
    c->pixelformat.gl = 8;
    c->pixelformat.gs = 8;
    c->pixelformat.gm = 0xFF;
    c->pixelformat.bl = 8;
    c->pixelformat.bs = 0;
    c->pixelformat.bm = 0xFF;
    c->pixelformat.al = 0;
    c->pixelformat.as = 0;
    c->pixelformat.am = 0;
}

/* the 32-bit RGBA8888 layout */
static void set_pixelformat_rgba8888(plContext* c) {
    c->pixelformat.rl = 8;
    c->pixelformat.rs = 0x10;
    c->pixelformat.rm = 0xFF;
    c->pixelformat.gl = 8;
    c->pixelformat.gs = 8;
    c->pixelformat.gm = 0xFF;
    c->pixelformat.bl = 8;
    c->pixelformat.bs = 0;
    c->pixelformat.bm = 0xFF;
    c->pixelformat.al = 8;
    c->pixelformat.as = 0x18;
    c->pixelformat.am = 0xFF;
}

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

/* A mipmap chain is valid when every level after the first is half the size of
 * the one before it, and when there are no more than seven of them. Returns 0
 * where the caller used to return 0, having logged and asserted the same way. */
static s32 mipmap_levels_are_valid(plContext* bits, s32 bnum) {
    plContext* lpcon;
    s32 lp0;
    s32 dw;
    s32 dh;

    if (bnum <= 1) {
        return 1;
    }

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

    return 1;
}

/* The GS pixel format for a context's bit depth. Returns 0 for a depth the
 * hardware has no format for, which is where the caller returned 0. */
static s32 set_texture_format(FLTexture* lpflTexture, s32 bitdepth) {
    switch (bitdepth) {
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

    return 1;
}

s32 flPS2GetTextureInfoFromContext(plContext* bits, s32 bnum, u32 th, u32 flag) {
    FLTexture* lpflTexture;

    lpflTexture = &flTexture[LO_16_BITS(th) - 1];

    if (!mipmap_levels_are_valid(bits, bnum)) {
        return 0;
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

    if (!set_texture_format(lpflTexture, bits->bitdepth)) {
        return 0;
    }

    lpflTexture->size =
        flPS2GetTextureSize(lpflTexture->format, lpflTexture->width, lpflTexture->height, lpflTexture->tex_num);
    return 1;
}

s32 flPS2CreateTextureHandle(u32 th, u32 flag) {
    Renderer_CreateTexture(th);
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
        fatal_error("ERROR flPS2GetTextureHandle flps2vram.c");
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
    Renderer_CreatePalette(ph);
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
        fatal_error("ERROR flPS2GetPaletteHandle flps2vram.c");
    }

    return (i + 1) << 16;
}

/* A handle that was never taken, is past the end of its table, or names an
 * entry that is not in use. The texture and palette releases asked this the
 * same way, differing only in which maximum applies. */
static s32 handle_is_invalid(u32 handle, u32 max, const FLTexture* entry) {
    return (handle == 0) || (handle > max) || (entry->be_flag == 0);
}

/* Give a released entry's system memory back and clear it. The texture and
 * palette releases end the same way, differing only in which entry. */
static s32 release_handle_entry(FLTexture* entry) {
    if (entry->mem_handle != 0) {
        flPS2ReleaseSystemMemory(entry->mem_handle);
    }

    SDL_zerop(entry);
    return 1;
}

s32 flReleaseTextureHandle(u32 texture_handle) {
    FLTexture* lpflTexture = &flTexture[texture_handle - 1];

    if (handle_is_invalid(texture_handle, FL_TEXTURE_MAX, lpflTexture)) {
        fatal_error("ERROR flReleaseTextureHandle flps2vram.c");
    }

    Renderer_DestroyTexture(texture_handle);

    return release_handle_entry(lpflTexture);
}

s32 flReleasePaletteHandle(u32 palette_handle) {
    FLTexture* lpflPalette = &flPalette[palette_handle - 1];

    if (handle_is_invalid(palette_handle, FL_PALETTE_MAX, lpflPalette)) {
        fatal_error("ERROR flReleasePaletteHandle flps2vram.c");
    }

    Renderer_DestroyPalette(palette_handle);

    return release_handle_entry(lpflPalette);
}

s32 flLockTexture(Rect* lprect, u32 th, plContext* lpcontext, u32 flag) {
    FLTexture* lpflTexture = &flTexture[th - 1];

    if (th > FL_TEXTURE_MAX) {
        return 0;
    }

    if (!lpflTexture->be_flag) {
        return 0;
    }

    return flPS2LockTexture(&(FlLockArgs){ lprect, lpflTexture, lpcontext, flag, 0 });
}

s32 flLockPalette(Rect* lprect, u32 th, plContext* lpcontext, u32 flag) {
    FLTexture* lpflPalette = &flPalette[th - 1];

    if (th > FL_PALETTE_MAX) {
        return 0;
    }

    if (!lpflPalette->be_flag) {
        return 0;
    }

    if (flPS2LockTexture(&(FlLockArgs){ lprect, lpflPalette, lpcontext, flag, 1 }) == 0) {
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

/* The context fields a lock fills in for the texture's format. Three of the four
 * lock modes wrote this switch out identically, character for character. */
static void set_lock_context_format(plContext* lpcontext, u32 format) {
    switch (format) {
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
        set_pixelformat_rgba5551(lpcontext);
        lpcontext->pixelformat.rs = 0;
        lpcontext->pixelformat.bs = 0xA;
        lpcontext->pixelformat.gl = 5;
        lpcontext->pixelformat.gm = 0x1F;
        lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
        break;

    case 1:
        lpcontext->bitdepth = 3;
        set_pixelformat_rgb888(lpcontext);
        lpcontext->pixelformat.rs = 0;
        lpcontext->pixelformat.bs = 0x10;
        lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
        break;

    case 0:
        lpcontext->bitdepth = 4;
        set_pixelformat_rgba8888(lpcontext);
        lpcontext->pixelformat.rs = 0;
        lpcontext->pixelformat.bs = 0x10;
        lpcontext->pitch = lpcontext->width * lpcontext->bitdepth;
        break;
    }
}

/* Mode 0 hands the caller a converted copy: the texture's own bytes in one
 * context and the lock buffer in another, converted between them for the
 * formats that need it and copied straight across for the ones that do not.
 * This is the arm's own switch, moved out whole. */
/* The direct-colour half of the lock conversion, reached through the default
 * arm of the paletted half. No case label is renumbered. */
/* The two ways this file dresses a context for a direct-colour convert: the
 * layout as it stands, and the same layout with red and blue swapped back. */

static void setup_rgb888_context(plContext* c) {
    c->bitdepth = 3;
    set_pixelformat_rgb888(c);
    c->pitch = c->width * c->bitdepth;
}

static void setup_bgr888_context(plContext* c) {
    c->bitdepth = 3;
    set_pixelformat_rgb888(c);
    c->pixelformat.rs = 0;
    c->pixelformat.bs = 0x10;
    c->pitch = c->width * c->bitdepth;
}

static void setup_rgba8888_context(plContext* c) {
    c->bitdepth = 4;
    set_pixelformat_rgba8888(c);
    c->pitch = c->width * c->bitdepth;
}

static void setup_bgra8888_context(plContext* c) {
    c->bitdepth = 4;
    set_pixelformat_rgba8888(c);
    c->pixelformat.rs = 0;
    c->pixelformat.bs = 0x10;
    c->pitch = c->width * c->bitdepth;
}

static void lock_convert_direct_formats(const FlLockArgs* a, plContext* src, u8* buff_ptr, u8* buff_ptr1) {
    switch (a->lpflTexture->format) {
    case 1:
        setup_rgb888_context(a->lpcontext);
        setup_bgr888_context(src);
        plConvertContext(a->lpcontext, src);
        break;

    case 0:
        setup_rgba8888_context(a->lpcontext);
        setup_bgra8888_context(src);
        plConvertContext(a->lpcontext, src);
        break;
    }
}

static void lock_convert_by_format(const FlLockArgs* a, plContext* src, u8* buff_ptr, u8* buff_ptr1) {
    switch (a->lpflTexture->format) {
    case 20:
        a->lpcontext->bitdepth = 0;
        a->lpcontext->pitch = a->lpcontext->width / 2;
        flMemcpy(buff_ptr, buff_ptr1, a->lpflTexture->size);
        break;

    case 19:
        a->lpcontext->bitdepth = 1;
        a->lpcontext->pitch = a->lpcontext->width * a->lpcontext->bitdepth;
        flMemcpy(buff_ptr, buff_ptr1, a->lpflTexture->size);
        break;

    case 2:
        a->lpcontext->bitdepth = 2;
        set_pixelformat_rgba5551(a->lpcontext);
        a->lpcontext->pitch = a->lpcontext->width * a->lpcontext->bitdepth;
        src->bitdepth = 2;
        set_pixelformat_rgba5551(src);
        src->pixelformat.rs = 0;
        src->pixelformat.bs = 0xA;
        src->pixelformat.gl = 5;
        src->pixelformat.gm = 0x1F;
        src->pitch = src->width * src->bitdepth;
        plConvertContext(a->lpcontext, src);
        break;

    default:
        lock_convert_direct_formats(a, src, buff_ptr, buff_ptr1);
        break;
    }
}

s32 flPS2LockTexture(const FlLockArgs* a) {
    u8* buff_ptr;
    u8* buff_ptr1;
    plContext src;

    a->lpflTexture->lock_flag = a->flag;
    a->lpcontext->desc = a->lpflTexture->desc;
    a->lpcontext->width = a->lpflTexture->width;
    a->lpcontext->height = a->lpflTexture->height;

    switch (a->flag & 3) {
    case 0:
        if (a->lpflTexture->mem_handle == 0) {
            buff_ptr1 = mflTemporaryUse(a->lpflTexture->size * 2);
            buff_ptr = buff_ptr1 + a->lpflTexture->size;
            // Loading an image from VRAM used to be here
        } else {
            buff_ptr = mflTemporaryUse(a->lpflTexture->size);
            buff_ptr1 = flPS2GetSystemBuffAdrs(a->lpflTexture->mem_handle);
        }

        a->lpflTexture->lock_ptr = (uintptr_t)buff_ptr;
        a->lpcontext->ptr = buff_ptr;
        src.desc = a->lpcontext->desc;
        src.width = a->lpcontext->width;
        src.height = a->lpcontext->height;
        src.ptr = buff_ptr1;

        lock_convert_by_format(a, &src, buff_ptr, buff_ptr1);

        break;

    case 1:
        buff_ptr = mflTemporaryUse(a->lpflTexture->size);

        if (a->lpflTexture->mem_handle == 0) {
            // Loading an image from VRAM used to be here
        } else {
            buff_ptr1 = flPS2GetSystemBuffAdrs(a->lpflTexture->mem_handle);
            flMemcpy(buff_ptr, buff_ptr1, a->lpflTexture->size);
        }

        a->lpflTexture->lock_ptr = (uintptr_t)buff_ptr;
        a->lpcontext->ptr = buff_ptr;

        set_lock_context_format(a->lpcontext, a->lpflTexture->format);

        break;

    /* Modes 2 and 3 had the same twelve lines written out twice. */
    case 2:
    case 3:
        if (a->lpflTexture->mem_handle == 0) {
            buff_ptr = mflTemporaryUse(a->lpflTexture->size);
        } else {
            buff_ptr = flPS2GetSystemBuffAdrs(a->lpflTexture->mem_handle);
        }

        a->lpflTexture->lock_ptr = (uintptr_t)buff_ptr;
        a->lpcontext->ptr = buff_ptr;

        set_lock_context_format(a->lpcontext, a->lpflTexture->format);

        break;
    }

    a->lpcontext->desc = a->lpcontext->desc | 2;
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
    Renderer_UnlockTexture(th);
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
    Renderer_UnlockPalette(th);
    return ret;
}

/* Unlocking a read-write lock puts the buffer back the way the hardware wants
 * it: a straight copy for the paletted formats, and a conversion between the
 * two contexts the caller has just pointed at the lock buffer and the texture's
 * own memory for the rest. This is the arm's own switch, moved out whole; the
 * two buffers it copied between are the two contexts' own pointers. */
/* The direct-colour half of the unlock conversion, reached the same way. */
static void unlock_convert_direct_formats(FLTexture* lpflTexture, plContext* src, plContext* dst) {
    switch (lpflTexture->format) {
    case 1:
        setup_rgb888_context(src);
        setup_bgr888_context(dst);
        plConvertContext(dst, src);
        break;

    case 0:
        setup_rgba8888_context(src);
        setup_bgra8888_context(dst);
        plConvertContext(dst, src);
        break;
    }
}

static void unlock_convert_by_format(FLTexture* lpflTexture, plContext* src, plContext* dst) {
    switch (lpflTexture->format) {
    case 20:
    case 19:
        flMemcpy(dst->ptr, src->ptr, lpflTexture->size);
        break;

    case 2:
        src->bitdepth = 2;
        set_pixelformat_rgba5551(src);
        src->pitch = src->width * src->bitdepth;
        dst->bitdepth = 2;
        set_pixelformat_rgba5551(dst);
        dst->pixelformat.rs = 0;
        dst->pixelformat.bs = 0xA;
        dst->pixelformat.gl = 5;
        dst->pixelformat.gm = 0x1F;
        dst->pitch = dst->width * dst->bitdepth;
        plConvertContext(dst, src);
        break;

    default:
        unlock_convert_direct_formats(lpflTexture, src, dst);
        break;
    }
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

        unlock_convert_by_format(lpflTexture, &src, &dst);

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

/* One mipmap level written into the texture's own memory, and how many bytes it
 * takes there. The paletted formats copy straight across; the direct-colour
 * ones convert through the level's context. The size is left uninitialised for
 * a format the hardware has none for, which is what the switch this replaces
 * did. */
static s32 convert_texture_level(plContext* lpcontext, plContext* tcon, FLTexture* lpflTexture, u32 type) {
    s32 tex_size;

    switch (lpflTexture->format) {
    default:
        flLogOut("Not supported texture bit depth @flPS2ConvertTextureFromContext");
        break;

    case SCE_GS_PSMT4:
        tex_size = (tcon->width * tcon->height) >> 1;
        flMemcpy(tcon->ptr, lpcontext->ptr, lpflTexture->size);
        break;

    case SCE_GS_PSMT8:
        tex_size = tcon->width * tcon->height;
        flMemcpy(tcon->ptr, lpcontext->ptr, lpflTexture->size);
        break;

    case SCE_GS_PSMCT16:
        tex_size = tcon->width * tcon->height * 2;
                set_pixelformat_rgba5551(tcon);
        tcon->pixelformat.rs = 0;
        tcon->pixelformat.bs = 0xA;
        tcon->pixelformat.gl = 5;
        tcon->pixelformat.gm = 0x1F;
        flPS2ConvertContext(lpcontext, tcon, 0, type);
        break;

    case SCE_GS_PSMCT24:
        tex_size = tcon->width * tcon->height * 4;
                set_pixelformat_rgb888(tcon);
        tcon->pixelformat.rs = 0;
        tcon->pixelformat.bs = 0x10;
        flPS2ConvertContext(lpcontext, tcon, 0, type);
        break;

    case SCE_GS_PSMCT32:
        tex_size = tcon->width * tcon->height * 4;
                set_pixelformat_rgba8888(tcon);
        tcon->pixelformat.rs = 0;
        tcon->pixelformat.bs = 0x10;
        flPS2ConvertContext(lpcontext, tcon, 0, type);
        break;
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
        tcon.ptr = dst_ptr;

        tex_size = convert_texture_level(lpcontext, &tcon, lpflTexture, type);

        dst_ptr = &dst_ptr[tex_size];
        dw >>= 1;
        dh >>= 1;
        lpcontext++;
    }

    return 1;
}

/* One source pixel, read at the width its context says. The variable is left
 * uninitialised for any other width, which is what the switch this replaces
 * did; only 2, 3 and 4 reach here. */
static u32 read_source_pixel(s32 bitdepth, const u8* src) {
    u32 color;

    switch (bitdepth) {
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

    return color;
}

/* The three channels a 24-bit write puts down one byte at a time; the other
 * widths write the packed colour instead. */
typedef struct {
    u32 r;
    u32 g;
    u32 b;
} ConvertedRGB;

static void write_converted_pixel(s32 bitdepth, u8* dst, u32 color, const ConvertedRGB* rgb) {
    switch (bitdepth) {
    case 2:
        ((u16*)dst)[0] = color;
        break;

    case 3:
        dst[0] = rgb->r;
        dst[1] = rgb->g;
        dst[2] = rgb->b;
        break;

    case 4:
        ((u32*)dst)[0] = color;
        break;
    }
}

/* Alpha between the eight-bit form and the PS2's seven-bit-plus-one form:
 * outward, full alpha becomes 0x80 and anything else halves without reaching
 * zero; inward, 0x80 becomes full and anything else doubles. Only a 32-bit
 * source carries alpha, which is why the caller asks only then. */
static u32 convert_alpha(u32 a, u32 direction) {
    if (direction != 0) {
        if (a == 0x80) {
            return 0xFF;
        }

        return a * 2;
    }

    if (a == 0xFF) {
        return 0x80;
    }

    if (a == 0) {
        return 0;
    }

    a >>= 1;

    if (a == 0) {
        return 1;
    }

    return a;
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

    keep_src = lpSrc->ptr;
    keep_dst = lpDst->ptr;
    wk0 = 0;
    wk1 = 0;

    for (y = 0; y < lpDst->height; y++) {
        for (x = 0; x < lpDst->width; x++) {
            if ((type == 1) && (direction == 1)) {
                src = keep_src;
                src += lpSrc->bitdepth * wk0;
            } else {
                src = keep_src + wk1;
                wk1 += lpSrc->bitdepth;
            }

            color = read_source_pixel(lpSrc->bitdepth, src);

            r = (lpSrc->pixelformat.rm & (color >> lpSrc->pixelformat.rs)) << (8 - lpSrc->pixelformat.rl);
            g = (lpSrc->pixelformat.gm & (color >> lpSrc->pixelformat.gs)) << (8 - lpSrc->pixelformat.gl);
            b = (lpSrc->pixelformat.bm & (color >> lpSrc->pixelformat.bs)) << (8 - lpSrc->pixelformat.bl);
            a = (lpSrc->pixelformat.am & (color >> lpSrc->pixelformat.as)) << (8 - lpSrc->pixelformat.al);

            if ((type == 1) && (direction == 0)) {
                dst = keep_dst;
                dst += lpDst->bitdepth * wk0;
            } else {
                dst = keep_dst + (lpDst->pitch * y) + (lpDst->bitdepth * x);
            }

            if (lpSrc->bitdepth == 4) {
                a = convert_alpha(a, direction);
            }

            color = ((lpDst->pixelformat.am & (a >> (8 - lpDst->pixelformat.al))) << lpDst->pixelformat.as) |
                    (((lpDst->pixelformat.bm & (b >> (8 - lpDst->pixelformat.bl))) << lpDst->pixelformat.bs) |
                     (((lpDst->pixelformat.rm & (r >> (8 - lpDst->pixelformat.rl))) << lpDst->pixelformat.rs) |
                      ((lpDst->pixelformat.gm & (g >> (8 - lpDst->pixelformat.gl))) << lpDst->pixelformat.gs)));

            write_converted_pixel(lpSrc->bitdepth, dst, color, &(ConvertedRGB){ r, g, b });

            wk0 += 1;
        }
    }

    return 1;
}
