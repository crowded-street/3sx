#include "sf33rd/AcrSDK/common/prilay.h"
#include "common.h"
#include "structs.h"
#include <stdio.h>

#if defined(TARGET_PS2)
#include "mw_stdarg.h"
#else
#include <stdarg.h>
#endif

s8 plReportMessage[2048];

s32 plReport(s8* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(plReportMessage, format, args);
    return 1;
}

void plMemset(void* dst, u32 pat, s32 size) {
    s32 i;
    s8* now = dst;

    for (i = 0; i < size; i++) {
        *(now++) = pat;
    }
}

void plMemmove(void* dst, void* src, s32 size) {
    s32 i;
    s8* now[2];

    now[0] = dst;
    now[1] = src;

    if (src < dst && (now[1] + size) > now[0]) {
        now[0] += size - 1;
        now[1] += size - 1;

        for (i = 0; i < size; i++) {
            *(now[0]--) = *(now[1]--);
        }

        return;
    }

    for (i = 0; i < size; i++) {
        *(now[0]++) = *(now[1]++);
    }
}

void* plCalcAddress(s32 x, s32 y, plContext* lpcontext) {
    if ((x < 0) || (y < 0) || (x >= lpcontext->width) || (y >= lpcontext->height)) {
        return NULL;
    }

    if (lpcontext->bitdepth == 0) {
        if (lpcontext->desc & 0x40) {
            return (s8*)lpcontext->ptr + (lpcontext->pitch * y) + x;
        }

        return (s8*)lpcontext->ptr + (lpcontext->pitch * y) + x / 2;
    }

    return (s8*)lpcontext->ptr + (lpcontext->pitch * y) + (x * lpcontext->bitdepth);
}

s32 plDrawPixel(plContext* dst, Pixel* ptr) {
    u8* lp;
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    u32 color;

    s32 unused_s3;

    lp = plCalcAddress(ptr->x, ptr->y, dst);

    if (lp == NULL) {
        return 0;
    }

    if (dst->desc & 4) {
        switch (dst->bitdepth) {
        case 4:
            ((u32*)lp)[0] = ptr->c;
            break;

        case 2:
            ((u16*)lp)[0] = ptr->c;
            break;

        case 1:
            lp[0] = ptr->c;
            break;

        case 0:
            if (dst->desc & 0x40) {
                lp[0] = ptr->c;
            } else {
                color = ptr->c;
                r = lp[0];

                if (dst->desc & 0x10) {
                    unused_s3 = 1;
                } else {
                    unused_s3 = 0;
                }

                if (((ptr->x & 1) ^ unused_s3) != 0) {
                    color &= 0xF;
                    r &= 0xF0;
                } else {
                    color = (color & 0xF) * 0x10;
                    r &= 0xF;
                }

                lp[0] = r | color;
            }

            break;
        }
    } else {
        a = (ptr->c >> 24) & 0xFF;
        r = (ptr->c >> 16) & 0xFF;
        g = (ptr->c >> 8) & 0xFF;
        b = ptr->c & 0xFF;

        color = (r * dst->pixelformat.rm / 255 << dst->pixelformat.rs) |
                (g * dst->pixelformat.gm / 255 << dst->pixelformat.gs) |
                (b * dst->pixelformat.bm / 255 << dst->pixelformat.bs) |
                (a * dst->pixelformat.am / 255 << dst->pixelformat.as);

        switch (dst->bitdepth) {
        case 2:
            ((u16*)lp)[0] = color;
            break;

        case 3:
            lp[0] = color & 0xFF;
            lp[1] = (color >> 8) & 0xFF;
            lp[2] = (color >> 16) & 0xFF;
            break;

        case 4:
            ((u32*)lp)[0] = color;
            break;
        }
    }

    return 1;
}

s32 plDrawPixel_3(plContext* dst, s32 x, s32 y, u32 color) {
    Pixel pixel;
    pixel.x = x;
    pixel.y = y;
    pixel.c = color;
    return plDrawPixel(dst, &pixel);
}

u32 plGetColor(s32 x, s32 y, plContext* lpcontext) {
    u8* lp;
    u32 color;
    s32 r;
    s32 g;
    s32 b;
    s32 a;

    lp = plCalcAddress(x, y, lpcontext);

    if (lp == NULL) {
        return 0;
    }

    if (lpcontext->desc & 4) {
        switch (lpcontext->bitdepth) {
        case 0:
            if (lpcontext->desc & 0x40) {
                color = lp[0];
            } else {
                color = lp[0];

                if (((x & 1) ^ ((lpcontext->desc & 0x10) != 0 ? 1 : 0)) != 0) {
                    color &= 0xF;
                } else {
                    color = (color >> 4) & 0xF;
                }
            }

            break;

        case 1:
            color = lp[0];
            break;

        case 2:
            color = ((u16*)lp)[0];
            break;

        case 4:
            color = ((u32*)lp)[0];
            break;
        }

        return color;
    }

    switch (lpcontext->bitdepth) {
    case 2:
        color = ((u16*)lp)[0];
        break;

    case 3:
        color = (lp[2] << 0x10) | (lp[1] << 8) | lp[0];
        break;

    case 4:
        color = ((u32*)lp)[0];
        break;
    }

    if (lpcontext->pixelformat.al != 0) {
        a = ((lpcontext->pixelformat.am & (color >> lpcontext->pixelformat.as)) * 0xFF) / lpcontext->pixelformat.am;
    } else {
        a = 0xFF;
    }

    if (lpcontext->pixelformat.rl != 0) {
        r = ((lpcontext->pixelformat.rm & (color >> lpcontext->pixelformat.rs)) * 0xFF) / lpcontext->pixelformat.rm;
    } else {
        r = 0;
    }

    if (lpcontext->pixelformat.gl != 0) {
        g = ((lpcontext->pixelformat.gm & (color >> lpcontext->pixelformat.gs)) * 0xFF) / lpcontext->pixelformat.gm;
    } else {
        g = 0;
    }

    if (lpcontext->pixelformat.bl != 0) {
        b = ((lpcontext->pixelformat.bm & (color >> lpcontext->pixelformat.bs)) * 0xFF) / lpcontext->pixelformat.bm;
    } else {
        b = 0;
    }

    return a << 24 | r << 16 | g << 8 | b;
}

s32 plConvertContext(plContext* dst, plContext* src) {
    s32 x;
    s32 y;
    u32 color;

    for (y = 0; y < dst->height; y++) {
        for (x = 0; x < dst->width; x++) {
            color = plGetColor(x, y, src);
            plDrawPixel_3(dst, x, y, color);
        }
    }

    return 1;
}
