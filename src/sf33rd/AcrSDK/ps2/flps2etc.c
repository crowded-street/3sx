#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "common.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/common/fbms.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/plapx.h"
#include "sf33rd/AcrSDK/common/plbmp.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/common/plpic.h"
#include "sf33rd/AcrSDK/common/pltim2.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "structs.h"

#include <SDL3/SDL.h>

#include <inttypes.h>
#include <libgraph.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef _WIN32
#include <ctype.h>
#endif

void flCompact();
void flPS2ConvertAlpha(void* lpPtr, s32 width, s32 height);
u32 flCreateTextureFromApx(const char* apx_file, u32 flag);
u32 flCreateTextureFromApx_mem(void* mem, u32 flag);
u32 flCreateTextureFromTim2(const char* tim2_file, u32 flag);
u32 flCreateTextureFromTim2_mem(void* mem, u32 flag);
u32 flCreateTextureFromBMP(const char* bmp_file, u32 flag);
u32 flCreateTextureFromBMP_mem(void* mem, u32 flag);
u32 flCreateTextureFromPIC(const char* pic_file, u32 flag);
u32 flCreateTextureFromPIC_mem(void* mem, u32 flag);

bool flFileRead(const char* filename, void* buf, s32 len) {
    SDL_IOStream* io = SDL_IOFromFile(filename, "rb");

    if (io == NULL) {
        SDL_Log("flFileRead: failed to read file at path %s - %s", filename, SDL_GetError());
        return false;
    }

    SDL_ReadIO(io, buf, len);
    SDL_CloseIO(io);
    return true;
}

s32 flFileLength(const char* filename) {
    SDL_IOStream* io = SDL_IOFromFile(filename, "rb");

    if (io == NULL) {
        return 0;
    }

    const s32 size = SDL_GetIOSize(io);
    SDL_CloseIO(io);
    return size;
}

// FIXME: use memcpy/SDL_memcpy instead
void flMemcpy(void* dst, void* src, s32 size) {
    SDL_memcpy(dst, src, size);
}

void* flAllocMemory(s32 size) {
    return fmsAllocMemory(&flFMS, size, 0);
}

s32 flGetFrame(FMS_FRAME* frame) {
    return fmsGetFrame(&flFMS, 0, frame);
}

s32 flGetSpace() {
    return fmsCalcSpace(&flFMS);
}

void* flAllocMemoryS(s32 size) {
    return fmsAllocMemory(&flFMS, size, 1);
}

u32 flPS2GetSystemMemoryHandle(s32 len, s32 type) {
    u32 handle = mflRegisterS(len);

    if (handle == 0) {
        flCompact();
        handle = mflRegister(len);

        if (handle == 0) {
            fatal_error("ERROR flPS2GetSystemMemoryHandle flps2etc.c");
        }
    }

    return handle;
}

void flPS2ReleaseSystemMemory(u32 handle) {
    mflRelease(handle);
}

void* flPS2GetSystemBuffAdrs(u32 handle) {
    return mflRetrieve(handle);
}

void flCompact() {
    mflCompact();
}

void flPS2SystemTmpBuffInit() {
    s32 lp0;

    for (lp0 = 0; lp0 < 2; lp0++) {
        flPs2State.SystemTmpBuffHandle[lp0] = flPS2GetSystemMemoryHandle(0x80000, 1);
    }

    flPS2SystemTmpBuffFlush();
}

void flPS2SystemTmpBuffFlush() {
    u32 len;

    switch (flPs2State.SystemStatus) {
    case 0:
    case 2:
    case 1:
        len = 0x80000;
        flPs2State.SystemTmpBuffStartAdrs =
            (uintptr_t)flPS2GetSystemBuffAdrs(flPs2State.SystemTmpBuffHandle[flPs2State.SystemIndex]);
        flPs2State.SystemTmpBuffNow = flPs2State.SystemTmpBuffStartAdrs;
        flPs2State.SystemTmpBuffEndAdrs = flPs2State.SystemTmpBuffStartAdrs + len;

        break;

    default:
        break;
    }
}

uintptr_t flPS2GetSystemTmpBuff(s32 len, s32 align) {
    uintptr_t now;
    uintptr_t new_now;

    now = flPs2State.SystemTmpBuffNow;
    now = ~(align - 1) & (now + align - 1);
    new_now = now + len;

    if (flPs2State.SystemTmpBuffEndAdrs < new_now) {
        fatal_error("ERROR flPS2GetSystemTmpBuff flps2etc.c");
        now = flPs2State.SystemTmpBuffStartAdrs;
        new_now = now + len;
    }

    flPs2State.SystemTmpBuffNow = new_now;
    return now;
}

/* A three-character file extension, each letter with the two spellings the
 * original tested. The field order is the order the comparisons were written
 * in; '2' appears twice because that is what the decompilation compares. */
typedef struct {
    char u0;
    char l0;
    char u1;
    char l1;
    char u2;
    char l2;
} FlExtension;

/* The four extension tests were the same five-term comparison four times over,
 * differing only in the letters. */
static s32 extension_matches(const char* tmp, const FlExtension* e) {
    return ((tmp[0] == e->u0) || (tmp[0] == e->l0)) && ((tmp[1] == e->u1) || (tmp[1] == e->l1)) &&
           ((tmp[2] == e->u2) || (tmp[2] == e->l2));
}

u32 flCreateTextureFromFile(const char* file, u32 flag) {
    const char* tmp = file;

    while (*tmp != '\0') {
        tmp++;
    }

    do {
        tmp--;
    } while (*tmp != '.');

    tmp++;

    if (extension_matches(tmp, &(FlExtension){ 'A', 'a', 'P', 'p', 'X', 'x' })) {
        return flCreateTextureFromApx(file, flag);
    }

    if (extension_matches(tmp, &(FlExtension){ 'T', 't', 'M', 'm', '2', '2' })) {
        return flCreateTextureFromTim2(file, flag);
    }

    if (extension_matches(tmp, &(FlExtension){ 'B', 'b', 'M', 'm', 'P', 'p' })) {
        return flCreateTextureFromBMP(file, flag);
    }

    if (extension_matches(tmp, &(FlExtension){ 'P', 'p', 'I', 'i', 'C', 'c' })) {
        return flCreateTextureFromPIC(file, flag);
    }

    return 0;
}

u32 flCreateTextureFromApx(const char* apx_file, u32 flag) {
    s32 len = flFileLength(apx_file);
    s8* file_ptr = mflTemporaryUse(len);

    if (!flFileRead(apx_file, file_ptr, len)) {
        return 0;
    }

    return flCreateTextureFromApx_mem(file_ptr, flag);
}

/* The mipmap chain of an APX image, one level per pass, each half the size of
 * the one above it. The destination and the starting dimensions are read from
 * the texture the caller has already set up. */
static void copy_apx_mipmaps(void* mem, const plContext* context, s32 mip_num, const FLTexture* lpflTexture) {
    u8* dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    s32 dw = lpflTexture->width;
    s32 dh = lpflTexture->height;
    u8* src;
    s32 lp0;
    s32 tex_size;

    for (lp0 = 0; lp0 <= mip_num; lp0++) {
        switch (context[lp0].bitdepth) {
        case 0:
            tex_size = dw * dh >> 1;
            src = plAPXGetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            break;

        case 1:
            tex_size = dw * dh;
            src = plAPXGetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            break;

        case 2:
            tex_size = dw * dh << 1;
            src = plAPXGetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            break;

        case 3:
            tex_size = dw * dh << 2;
            src = plAPXGetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            break;

        case 4:
            tex_size = dw * dh << 2;
            src = plAPXGetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            flPS2ConvertAlpha(dst, dw, dh);
            break;
        }

        dw >>= 1;
        dh >>= 1;
        dst = &dst[tex_size];
    }
}

/* The palette an APX image carries, for the two formats that have one. Returns
 * the palette handle, which is the one value the block left behind. */
static u32 create_apx_palette(void* mem, const FLTexture* lpflTexture, u32 flag) {
    plContext pal_context;
    plContext tmp_context;
    FLTexture* lpflPalette;
    u8* dst;
    u8* src;
    u32 ph;

    ph = flPS2GetPaletteHandle();
    lpflPalette = &flPalette[HI_16_BITS(ph) - 1];
    plAPXSetPaletteContextFromImage(&pal_context, mem);
    flPS2GetPaletteInfoFromContext(&pal_context, ph, flag);
    lpflPalette->mem_handle = flPS2GetSystemMemoryHandle(lpflPalette->size, 2);
    dst = flPS2GetSystemBuffAdrs(lpflPalette->mem_handle);
    src = plAPXGetPaletteAddressFromImage(mem, 0);

    if (lpflTexture->format == 0x13) {
        tmp_context = pal_context;
        pal_context.ptr = src;
        tmp_context.ptr = dst;
        flPS2ConvertContext(&pal_context, &tmp_context, 0, 1);
    } else {
        flMemcpy(dst, src, lpflPalette->size);

        if (pal_context.bitdepth == 4) {
            flPS2ConvertAlpha(dst, lpflPalette->width, lpflPalette->height);
        }
    }

    flPS2CreatePaletteHandle(ph, flag);
    return ph;
}

u32 flCreateTextureFromApx_mem(void* mem, u32 flag) {
    plContext context[7];
    u32 th;
    u32 ph;
    FLTexture* lpflTexture;
    s32 mip_num;

    th = 0;
    ph = 0;
    th = flPS2GetTextureHandle();
    lpflTexture = &flTexture[LO_16_BITS(th) - 1];
    mip_num = plAPXGetMipmapTextureNum(mem) - 1;

    if (plAPXSetContextFromImage(&context[0], mem) == 0) {
        return 0;
    }

    flPS2GetTextureInfoFromContext(&context[0], mip_num + 1, th, flag);
    lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    copy_apx_mipmaps(mem, context, mip_num, lpflTexture);

    flPS2CreateTextureHandle(th, flag);

    if ((lpflTexture->format == 0x14) || (lpflTexture->format == 0x13)) {
        ph = create_apx_palette(mem, lpflTexture, flag);
    }

    return th | ph;
}

u32 flCreateTextureFromTim2(const char* tim2_file, u32 flag) {
    s32 len = flFileLength(tim2_file);
    s8* file_ptr = mflTemporaryUse(len);

    if (!flFileRead(tim2_file, file_ptr, len)) {
        return 0;
    }

    return flCreateTextureFromTim2_mem(file_ptr, flag);
}

/* The mipmap chain of a TIM2 image. The same shape as the APX chain above, but
 * not the same text: it reads a different pixel address and advances the
 * destination with `+=` rather than a subscript, which is two differences and
 * so not one family. */
static void copy_tim2_mipmaps(void* mem, const plContext* context, s32 mip_num, const FLTexture* lpflTexture) {
    u8* dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    s32 dw = lpflTexture->width;
    s32 dh = lpflTexture->height;
    u8* src;
    s32 lp0;
    s32 tex_size;

    for (lp0 = 0; lp0 <= mip_num; lp0++) {
        switch (context[lp0].bitdepth) {
        case 0:
            tex_size = dw * dh >> 1;
            src = plTIM2GetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);

            break;

        case 1:
            tex_size = dw * dh;
            src = plTIM2GetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);

            break;

        case 2:
            tex_size = dw * dh << 1;
            src = plTIM2GetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            break;

        case 3:
            tex_size = dw * dh << 2;
            src = plTIM2GetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            break;

        case 4:
            tex_size = dw * dh << 2;
            src = plTIM2GetPixelAddressFromImage(mem, lp0);
            flMemcpy(dst, src, tex_size);
            flPS2ConvertAlpha(dst, dw, dh);
            break;
        }

        dw >>= 1;
        dh >>= 1;
        dst += tex_size;
    }
}

u32 flCreateTextureFromTim2_mem(void* mem, u32 flag) {
    u8* dst;
    u8* src;
    plContext context[7];
    plContext pal_context;
    u32 th = 0;
    u32 ph = 0;
    FLTexture* lpflTexture;
    FLTexture* lpflPalette;
    s32 mip_num;

    th = flPS2GetTextureHandle();
    lpflTexture = &flTexture[LO_16_BITS(th) - 1];
    mip_num = plTIM2GetMipmapTextureNum(mem);

    if (plTIM2SetContextFromImage(context, mem) == 0) {
        return 0;
    }

    flPS2GetTextureInfoFromContext(context, mip_num + 1, th, flag);
    lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    copy_tim2_mipmaps(mem, context, mip_num, lpflTexture);

    flPS2CreateTextureHandle(th, flag);

    if ((lpflTexture->format == SCE_GS_PSMT4) || (lpflTexture->format == SCE_GS_PSMT8)) {
        ph = flPS2GetPaletteHandle();
        lpflPalette = &flPalette[HI_16_BITS(ph) - 1];
        plTIM2SetPaletteContextFromImage(&pal_context, mem);
        flPS2GetPaletteInfoFromContext(&pal_context, ph, flag);
        lpflPalette->mem_handle = flPS2GetSystemMemoryHandle(lpflPalette->size, 2);
        dst = flPS2GetSystemBuffAdrs(lpflPalette->mem_handle);
        src = plTIM2GetPaletteAddressFromImage(mem);
        flMemcpy(dst, src, lpflPalette->size);

        if (pal_context.bitdepth == 4) {
            flPS2ConvertAlpha(dst, lpflPalette->width, lpflPalette->height);
        }

        flPS2CreatePaletteHandle(ph, flag);
    }

    return th | ph;
}

/* The PS2's alpha range is half the PC one: opaque is 128, and a value that
 * halves to nothing is held at 1 rather than becoming transparent. */
static u8 ps2_alpha_of(u8 alpha) {
    if (alpha == 255) {
        alpha = 128;
    } else if (alpha != 0) {
        alpha >>= 1;

        if (alpha == 0) {
            alpha = 1;
        }
    }

    return alpha;
}

void flPS2ConvertAlpha(void* lpPtr, s32 width, s32 height) {
    s32 x;
    s32 y;
    u8* ptr = lpPtr;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            ptr[3] = ps2_alpha_of(ptr[3]);
            ptr += 4;
        }
    }
}

u32 flCreateTextureFromBMP(const char* bmp_file, u32 flag) {
    s32 len = flFileLength(bmp_file);
    char* file_ptr = mflTemporaryUse(len);

    if (!flFileRead(bmp_file, file_ptr, len)) {
        return 0;
    }

    return flCreateTextureFromBMP_mem(file_ptr, flag);
}

/* Neither of the two bit depths the direct-colour loaders accept. The test is
 * the one that stood at the call site, copied rather than inverted. */
static bool is_unsupported_bitdepth(const plContext* context) {
    return context->bitdepth != 3 && context->bitdepth != 4;
}

static void copy_bmp_24bpp(u8* dst, const u8* keep, const plContext* context) {
    s32 x;
    s32 y;
    const u8* src;
    u8 r;
    u8 g;
    u8 b;

    for (y = 0; y < context->height; y++) {
        for (x = 0; x < context->width; x++) {
            src = keep + x * context->bitdepth + (context->height - 1 - y) * context->pitch;
            b = *src++;
            g = *src++;
            r = *src++;
            *dst++ = r;
            *dst++ = g;
            *dst++ = b;
        }
    }
}

u32 flCreateTextureFromBMP_mem(void* mem, u32 flag) {
    s32 x;
    s32 y;
    u8* dst;
    u8* src;
    u8* keep;
    plContext context;
    u32 th = 0;
    FLTexture* lpflTexture;
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    th = flPS2GetTextureHandle();
    lpflTexture = &flTexture[LO_16_BITS(th) - 1];

    if (!plBMPSetContextFromImage(&context, mem)) {
        return 0;
    }

    if (is_unsupported_bitdepth(&context)) {
        return 0;
    }

    flPS2GetTextureInfoFromContext(&context, 1, th, flag);
    lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    keep = plBMPGetPixelAddressFromImage(mem);

    switch (context.bitdepth) {
    case 3:
        copy_bmp_24bpp(dst, keep, &context);

        break;

    case 4:
        for (y = 0; y < context.height; y++) {
            for (x = 0; x < context.width; x++) {
                src = keep + x * context.bitdepth + (context.height - 1 - y) * context.pitch;
                b = *src++;
                g = *src++;
                r = *src++;
                a = *src++;
                *dst++ = b;
                *dst++ = g;
                *dst++ = r;
                *dst++ = a;
            }
        }

        break;

    default:
        return 0;
    }

    flPS2CreateTextureHandle(th, flag);
    return th;
}

u32 flCreateTextureFromPIC(const char* pic_file, u32 flag) {
    s32 len = flFileLength(pic_file);
    s8* file_ptr = mflTemporaryUse(len);

    if (!flFileRead(pic_file, file_ptr, len)) {
        return 0;
    }

    return flCreateTextureFromPIC_mem(file_ptr, flag);
}

/* One row of the PIC run-length stream, decoded into the destination it is
 * given. Each returns the source pointer where it stopped, which is the one
 * value the block carried back out of its braces. */
static u8* decode_pic_rgb_row(u8* lpdst, u8* lpsrc, const plContext* context) {
    s32 cx;
    s32 ax;

    s32 x = 0;

    while (x < context->width) {
        ax = *lpsrc++;

        if (ax == 0x80) {
            cx = (lpsrc[0] << 8) | lpsrc[1];
            lpsrc += 2;
            x += cx;

            while (cx-- != 0) {
                lpdst[0] = lpsrc[0];
                lpdst[1] = lpsrc[1];
                lpdst[2] = lpsrc[2];
                lpdst += context->bitdepth;
            }

            lpsrc += 3;
        } else if (ax > 0x80) {
            cx = ax - 0x7F;
            x += cx;

            while (cx-- != 0) {
                lpdst[0] = lpsrc[0];
                lpdst[1] = lpsrc[1];
                lpdst[2] = lpsrc[2];
                lpdst += context->bitdepth;
            }

            lpsrc += 3;
        } else {
            cx = ax + 1;
            x += cx;

            while (cx-- != 0) {
                lpdst[0] = lpsrc[0];
                lpdst[1] = lpsrc[1];
                lpdst[2] = lpsrc[2];
                lpdst += context->bitdepth;
                lpsrc += 3;
            }
        }
    }

    return lpsrc;
}

static u8* decode_pic_alpha_row(u8* lpdst, u8* lpsrc, const plContext* context) {
    s32 cx;
    s32 ax;

    if (context->bitdepth == 3) {
        return lpsrc;
    }

    s32 x = 0;

    while (x < context->width) {
        ax = *lpsrc++;

        if (ax == 0x80) {
            cx = (lpsrc[0] << 8) | lpsrc[1];
            lpsrc += 2;
            x += cx;

            while (cx-- != 0) {
                lpdst[0] = lpsrc[0];
                lpdst += 4;
            }

            lpsrc += 1;
        } else if (ax > 0x80) {
            cx = ax - 0x7F;
            x += cx;

            while (cx-- != 0) {
                lpdst[0] = lpsrc[0];
                lpdst += 4;
            }

            lpsrc += 1;
        } else {
            cx = ax + 1;
            x += cx;

            while (cx-- != 0) {
                *lpdst = *lpsrc++;
                lpdst += 4;
            }
        }
    }

    return lpsrc;
}

u32 flCreateTextureFromPIC_mem(void* mem, u32 flag) {
    s32 y;
    u8* dst;
    u8* lpsrc;
    plContext context;
    u32 th = 0;
    FLTexture* lpflTexture;

    th = flPS2GetTextureHandle();
    lpflTexture = &flTexture[LO_16_BITS(th) - 1];
    plPICSetContextFromImage(&context, mem);

    if ((context.bitdepth != 3) && (context.bitdepth != 4)) {
        return 0;
    }

    flPS2GetTextureInfoFromContext(&context, 1, th, flag);
    lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    lpsrc = plPICGetPixelAddressFromImage(mem);

    for (y = 0; y < context.height; y++) {
        lpsrc = decode_pic_rgb_row(dst + (y * context.pitch), lpsrc, &context);
        lpsrc = decode_pic_alpha_row(dst + (y * context.pitch) + 3, lpsrc, &context);
    }

    if (context.bitdepth == 4) {
        dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
        flPS2ConvertAlpha(dst, lpflTexture->width, lpflTexture->height);
    }

    flPS2CreateTextureHandle(th, flag);
    return th;
}
