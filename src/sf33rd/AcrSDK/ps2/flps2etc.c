#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/fbms.h"
#include "sf33rd/AcrSDK/common/memfound.h"
#include "sf33rd/AcrSDK/common/plapx.h"
#include "sf33rd/AcrSDK/common/plbmp.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/common/plpic.h"
#include "sf33rd/AcrSDK/common/pltim2.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/flps2vram.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "structs.h"

#include <libgraph.h>
#include <sifdev.h>

#include <stdio.h>
#include <string.h>

#if !defined(TARGET_PS2) && !defined(_WIN32)
#include <ctype.h>

s8* strupr(s8* s) {
    s8* p = s;

    while (*p) {
        *p = toupper((u8)*p);
        p++;
    }

    return s;
}
#endif

void flCompact();
void flPS2ConvertAlpha(void* lpPtr, s32 width, s32 height);
u32 flCreateTextureFromApx(s8* apx_file, u32 flag);
u32 flCreateTextureFromApx_mem(void* mem, u32 flag);
u32 flCreateTextureFromTim2(s8* tim2_file, u32 flag);
u32 flCreateTextureFromTim2_mem(void* mem, u32 flag);
u32 flCreateTextureFromBMP(s8* bmp_file, u32 flag);
u32 flCreateTextureFromBMP_mem(void* mem, u32 flag);
u32 flCreateTextureFromPIC(s8* pic_file, u32 flag);
u32 flCreateTextureFromPIC_mem(void* mem, u32 flag);

s32 flFileRead(s8* filename, void* buf, s32 len) {
    s32 fd;
    s8 temp[2048];
    s8* p;

    strcpy(temp, "cdrom0:\\THIRD\\");
    p = strlen(temp) + temp;
    strcat(temp, filename);
    strupr(p);
    strcat(temp, ";1");

    fd = sceOpen(temp, SCE_RDONLY);
    printf("flFileRead: \"%s\" (fd = %d)\n", temp, fd);

    if (fd < 0) {
        return 0;
    }

    sceRead(fd, buf, len);
    sceClose(fd);
    return 1;
}

s32 flFileWrite(s8* filename, void* buf, s32 len) {
    s32 fd;
    s8 temp[2048];
    s8* p;

    strcpy(temp, "cdrom0:\\THIRD\\");
    p = strlen(temp) + temp;
    strcat(temp, filename);
    strupr(p);
    strcat(temp, ";1");

    if ((fd = sceOpen(temp, SCE_WRONLY | SCE_CREAT | SCE_TRUNC)) < 0) {
        return 0;
    }

    sceWrite(fd, buf, len);
    sceClose(fd);
    return 1;
}

s32 flFileAppend(s8* filename, void* buf, ssize_t len) {
    s32 fd;
    s8 temp[2048];
    s8* p;

    strcpy(temp, "cdrom0:\\THIRD\\");
    p = strlen(temp) + temp;
    strcat(temp, filename);
    strupr(p);
    strcat(temp, ";1");

    if ((fd = sceOpen(temp, SCE_WRONLY)) < 0) {
        return 0;
    }

    sceLseek(fd, 0, 2);
    sceWrite(fd, buf, (s32)len);
    sceClose(fd);
    return 1;
}

s32 flFileLength(s8* filename) {
    s32 fd;
    s8 temp[2048];
    s8* p;
    s32 length;

    strcpy(temp, "cdrom0:\\THIRD\\");
    p = strlen(temp) + temp;
    strcat(temp, filename);
    strupr(p);
    strcat(temp, ";1");

    if ((fd = sceOpen(temp, SCE_RDONLY)) < 0) {
        return 0;
    }

    length = sceLseek(fd, 0, SCE_SEEK_END);
    sceClose(fd);
    return length;
}

// FIXME: use memset instead
void flMemset(void* dst, u32 pat, s32 size) {
    s32 i;
    u8* now = dst;

    for (i = 0; i < size; i++) {
        *now++ = pat;
    }
}

// FIXME: use memcpy instead
void flMemcpy(void* dst, void* src, s32 size) {
    s32 i;
    s8* now[2];

    now[0] = dst;
    now[1] = src;

    for (i = 0; i < size; i++) {
        *now[0]++ = *now[1]++;
    }
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
            flPS2SystemError(0, "ERROR flPS2GetSystemMemoryHandle flps2etc.c");
            while (1) {}
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
        flPS2SystemError(0, "ERROR flPS2GetSystemTmpBuff flps2etc.c");
        now = flPs2State.SystemTmpBuffStartAdrs;
        new_now = now + len;
    }

    flPs2State.SystemTmpBuffNow = new_now;
    return now;
}

u32 flCreateTextureFromFile(s8* file, u32 flag) {
    s8* tmp = file;

    while (*tmp != 0) {
        tmp++;
    }

    do {
        tmp--;
    } while (*tmp != '.');

    tmp++;

    if (((tmp[0] == 'A') || (tmp[0] == 'a')) && ((tmp[1] == 'P') || (tmp[1] == 'p')) &&
        ((tmp[2] == 'X') || (tmp[2] == 'x'))) {
        return flCreateTextureFromApx(file, flag);
    }

    if (((tmp[0] == 'T') || (tmp[0] == 't')) && ((tmp[1] == 'M') || (tmp[1] == 'm')) &&
        ((tmp[2] == '2') || (tmp[2] == '2'))) {
        return flCreateTextureFromTim2(file, flag);
    }

    if (((tmp[0] == 'B') || (tmp[0] == 'b')) && ((tmp[1] == 'M') || (tmp[1] == 'm')) &&
        ((tmp[2] == 'P') || (tmp[2] == 'p'))) {
        return flCreateTextureFromBMP(file, flag);
    }

    if (((tmp[0] == 'P') || (tmp[0] == 'p')) && ((tmp[1] == 'I') || (tmp[1] == 'i')) &&
        ((tmp[2] == 'C') || (tmp[2] == 'c'))) {
        return flCreateTextureFromPIC(file, flag);
    }

    return 0;
}

u32 flCreateTextureFromApx(s8* apx_file, u32 flag) {
    s32 len = flFileLength(apx_file);
    s8* file_ptr = mflTemporaryUse(len);

    if (flFileRead(apx_file, file_ptr, len) == 0) {
        return 0;
    }

    return flCreateTextureFromApx_mem(file_ptr, flag);
}

u32 flCreateTextureFromApx_mem(void* mem, u32 flag) {
    u8* dst;
    u8* src;
    plContext context[7];
    plContext pal_context;
    plContext tmp_context;
    u32 th;
    u32 ph;
    FLTexture* lpflTexture;
    FLTexture* lpflPalette;
    s32 mip_num;
    s32 lp0;
    s32 dw;
    s32 dh;
    s32 tex_size;

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
    dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    dw = lpflTexture->width;
    dh = lpflTexture->height;

    for (lp0 = 0; lp0 <= mip_num; lp0++) {
        switch (context[lp0].bitdepth) {
        case 0:
            tex_size = dw * dh >> 1;
            src = plAPXGetPixelAddressFromImage(mem, lp0);

            if (lpflTexture->dma_type == 0) {
                flMemcpy(dst, src, tex_size);
            } else {
                flPS2Conv4_8_32(dw, dh, src, dst, 0);
            }

            break;

        case 1:
            tex_size = dw * dh;
            src = plAPXGetPixelAddressFromImage(mem, lp0);

            if (lpflTexture->dma_type == 0) {
                flMemcpy(dst, src, tex_size);
            } else {
                flPS2Conv4_8_32(dw, dh, src, dst, 1);
            }

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

    flPS2CreateTextureHandle(th, flag);

    if ((lpflTexture->format == 0x14) || (lpflTexture->format == 0x13)) {
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
    }

    return th | ph;
}

u32 flCreateTextureFromTim2(s8* tim2_file, u32 flag) {
    s32 len = flFileLength(tim2_file);
    s8* file_ptr = mflTemporaryUse(len);

    if (flFileRead(tim2_file, file_ptr, len) == 0) {
        return 0;
    }

    return flCreateTextureFromTim2_mem(file_ptr, flag);
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
    s32 lp0;
    s32 dw;
    s32 dh;
    s32 tex_size;

    th = flPS2GetTextureHandle();
    lpflTexture = &flTexture[LO_16_BITS(th) - 1];
    mip_num = plTIM2GetMipmapTextureNum(mem);

    if (plTIM2SetContextFromImage(context, mem) == 0) {
        return 0;
    }

    flPS2GetTextureInfoFromContext(context, mip_num + 1, th, flag);
    lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    dw = lpflTexture->width;
    dh = lpflTexture->height;

    for (lp0 = 0; lp0 <= mip_num; lp0++) {
        switch (context[lp0].bitdepth) {
        case 0:
            tex_size = dw * dh >> 1;
            src = plTIM2GetPixelAddressFromImage(mem, lp0);

            if (lpflTexture->dma_type == 0) {
                flMemcpy(dst, src, tex_size);
            } else {
                flPS2Conv4_8_32(dw, dh, src, dst, 0);
            }

            break;

        case 1:
            tex_size = dw * dh;
            src = plTIM2GetPixelAddressFromImage(mem, lp0);

            if (lpflTexture->dma_type == 0) {
                flMemcpy(dst, src, tex_size);
            } else {
                flPS2Conv4_8_32(dw, dh, src, dst, 1);
            }

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

void flPS2ConvertAlpha(void* lpPtr, s32 width, s32 height) {
    s32 x;
    s32 y;
    u8* ptr = lpPtr;
    u8 alpha;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            alpha = ptr[3];

            if (alpha == 255) {
                alpha = 128;
            } else if (alpha != 0) {
                alpha >>= 1;

                if (alpha == 0) {
                    alpha = 1;
                }
            }

            ptr[3] = alpha;
            ptr += 4;
        }
    }
}

u32 flCreateTextureFromBMP(s8* bmp_file, u32 flag) {
    s32 len = flFileLength(bmp_file);
    char* file_ptr = mflTemporaryUse(len);

    if (flFileRead(bmp_file, file_ptr, len) == 0) {
        return 0;
    }

    return flCreateTextureFromBMP_mem(file_ptr, flag);
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

    th = flPS2GetTextureHandle();
    lpflTexture = &flTexture[LO_16_BITS(th) - 1];
    plBMPSetContextFromImage(&context, mem);

    if (context.bitdepth != 3) {
        return 0;
    }

    flPS2GetTextureInfoFromContext(&context, 1, th, flag);
    lpflTexture->mem_handle = flPS2GetSystemMemoryHandle(lpflTexture->size, 2);
    dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
    keep = plBMPGetPixelAddressFromImage(mem);

    switch (context.bitdepth) {
    case 3:
        for (y = 0; y < context.height; y++) {
            for (x = 0; x < context.width; x++) {
                src = keep + x * context.bitdepth + (context.height - 1 - y) * context.pitch;
                b = *src++;
                g = *src++;
                r = *src++;
                *dst++ = r;
                *dst++ = g;
                *dst++ = b;
            }
        }

        break;

    default:
        return 0;
    }

    flPS2CreateTextureHandle(th, flag);
    return th;
}

u32 flCreateTextureFromPIC(s8* pic_file, u32 flag) {
    s32 len = flFileLength(pic_file);
    s8* file_ptr = mflTemporaryUse(len);

    if (flFileRead(pic_file, file_ptr, len) == 0) {
        return 0;
    }

    return flCreateTextureFromPIC_mem(file_ptr, flag);
}

u32 flCreateTextureFromPIC_mem(void* mem, u32 flag) {
    s32 x;
    s32 y;
    u8* lpdst;
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
        {
            s32 cx;
            s32 ax;

            lpdst = dst + (y * context.pitch);
            x = 0;

            while (x < context.width) {
                ax = *lpsrc++;

                if (ax == 0x80) {
                    cx = (lpsrc[0] << 8) | lpsrc[1];
                    lpsrc += 2;
                    x += cx;

                    while (cx-- != 0) {
                        lpdst[0] = lpsrc[0];
                        lpdst[1] = lpsrc[1];
                        lpdst[2] = lpsrc[2];
                        lpdst += context.bitdepth;
                    }

                    lpsrc += 3;
                } else if (ax > 0x80) {
                    cx = ax - 0x7F;
                    x += cx;

                    while (cx-- != 0) {
                        lpdst[0] = lpsrc[0];
                        lpdst[1] = lpsrc[1];
                        lpdst[2] = lpsrc[2];
                        lpdst += context.bitdepth;
                    }

                    lpsrc += 3;
                } else {
                    cx = ax + 1;
                    x += cx;

                    while (cx-- != 0) {
                        lpdst[0] = lpsrc[0];
                        lpdst[1] = lpsrc[1];
                        lpdst[2] = lpsrc[2];
                        lpdst += context.bitdepth;
                        lpsrc += 3;
                    }
                }
            }
        }

        {
            s32 cx;
            s32 ax;

            if (context.bitdepth != 3) {
                lpdst = dst + (y * context.pitch) + 3;
                x = 0;

                while (x < context.width) {
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
            }
        }
    }

    if (context.bitdepth == 4) {
        dst = flPS2GetSystemBuffAdrs(lpflTexture->mem_handle);
        flPS2ConvertAlpha(dst, lpflTexture->width, lpflTexture->height);
    }

    flPS2CreateTextureHandle(th, flag);
    return th;
}
