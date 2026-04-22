#if CRS_VIDEO_DRIVER_SOFTWARE

#include "platform/video/software/sw_blit.h"

#include <string.h>

// Scalar fallback and reference path.

// Keep this on for the active targets.
#ifndef CRS_SW_CKEY8_FAST_PATH
#define CRS_SW_CKEY8_FAST_PATH 1
#endif

static inline uint32_t modulate_pixel(uint32_t src, uint32_t modulate) {
    if (modulate == 0xFFFFFFFFu) {
        return src;
    }

    const uint32_t sa = (src >> 24) & 0xFF;
    const uint32_t sr = (src >> 16) & 0xFF;
    const uint32_t sg = (src >> 8) & 0xFF;
    const uint32_t sb = src & 0xFF;
    const uint32_t ma = (modulate >> 24) & 0xFF;
    const uint32_t mr = (modulate >> 16) & 0xFF;
    const uint32_t mg = (modulate >> 8) & 0xFF;
    const uint32_t mb = modulate & 0xFF;

    const uint32_t ra = (sa * ma + 128) >> 8;
    const uint32_t rr = (sr * mr + 128) >> 8;
    const uint32_t rg = (sg * mg + 128) >> 8;
    const uint32_t rb = (sb * mb + 128) >> 8;
    return (ra << 24) | (rr << 16) | (rg << 8) | rb;
}

// Src-over in ARGB8888.
static inline uint32_t blend_src_over_argb(uint32_t src, uint32_t dst_argb) {
    const uint32_t sa = (src >> 24) & 0xFF;

    if (sa == 0xFF) {
        return src;
    }

    if (sa == 0x00) {
        return dst_argb;
    }

    const uint32_t ia = 255 - sa;
    const uint32_t sr = (src >> 16) & 0xFF;
    const uint32_t sg = (src >> 8) & 0xFF;
    const uint32_t sb = src & 0xFF;
    const uint32_t dr = (dst_argb >> 16) & 0xFF;
    const uint32_t dg = (dst_argb >> 8) & 0xFF;
    const uint32_t db = dst_argb & 0xFF;

    const uint32_t rr = (sr * sa + dr * ia + 128) >> 8;
    const uint32_t rg = (sg * sa + dg * ia + 128) >> 8;
    const uint32_t rb = (sb * sa + db * ia + 128) >> 8;
    return 0xFF000000u | (rr << 16) | (rg << 8) | rb;
}

// Blend ARGB onto the canvas.
static inline SWCanvasPixel blend_argb_onto_canvas(uint32_t src, SWCanvasPixel dst_px) {
    const uint32_t sa = (src >> 24) & 0xFF;

    if (sa == 0xFF) {
        return sw_argb_to_canvas(src);
    }

    if (sa == 0x00) {
        return dst_px;
    }

#if defined(CRS_SW_CANVAS_16BPP)
    // RGB565 blend with 5-bit alpha.
    const uint32_t src_565 = (uint32_t)sw_argb_to_canvas(src);
    const uint32_t dst_565 = (uint32_t)dst_px;
    const uint32_t sa_5 = sa >> 3;          // 0..31
    const uint32_t ia_5 = 32u - sa_5;       // 1..32
    const uint32_t s_rb = src_565 & 0xF81Fu;
    const uint32_t s_g  = src_565 & 0x07E0u;
    const uint32_t d_rb = dst_565 & 0xF81Fu;
    const uint32_t d_g  = dst_565 & 0x07E0u;
    const uint32_t rb = ((s_rb * sa_5 + d_rb * ia_5) >> 5) & 0xF81Fu;
    const uint32_t g  = ((s_g  * sa_5 + d_g  * ia_5) >> 5) & 0x07E0u;
    return (SWCanvasPixel)(rb | g);
#else
    const uint32_t dst_argb = sw_canvas_to_argb(dst_px);
    const uint32_t blended = blend_src_over_argb(src, dst_argb);
    return sw_argb_to_canvas(blended);
#endif
}

// Scalar solid/direct kernels.

#if !defined(__ARM_NEON)

void sw_fill_solid_row(SWCanvasPixel* dst, uint32_t argb, int count) {
    const uint32_t a = (argb >> 24) & 0xFF;

    if (a == 0xFF) {
        const SWCanvasPixel px = sw_argb_to_canvas(argb);
        int i = 0;
#if defined(__GNUC__) || defined(__clang__)
        for (; i + 16 <= count; i += 16) {
            __builtin_prefetch(dst + i + 32, 1);
            dst[i + 0]  = px; dst[i + 1]  = px; dst[i + 2]  = px; dst[i + 3]  = px;
            dst[i + 4]  = px; dst[i + 5]  = px; dst[i + 6]  = px; dst[i + 7]  = px;
            dst[i + 8]  = px; dst[i + 9]  = px; dst[i + 10] = px; dst[i + 11] = px;
            dst[i + 12] = px; dst[i + 13] = px; dst[i + 14] = px; dst[i + 15] = px;
        }
#endif
        for (; i < count; i++) {
            dst[i] = px;
        }
        return;
    }

    if (a == 0u) {
        return;
    }

#if defined(CRS_SW_CANVAS_16BPP)
    // Stay at 2-pixel unroll here; 4-pixel regressed on real hardware.
    const uint32_t src_565 = (uint32_t)sw_argb_to_canvas(argb);
    const uint32_t sa_5 = a >> 3;           // 0..31
    const uint32_t ia_5 = 32u - sa_5;       // 1..32
    const uint32_t s_rb_a = (src_565 & 0xF81Fu) * sa_5;
    const uint32_t s_g_a  = (src_565 & 0x07E0u) * sa_5;
    int i = 0;
    for (; i + 2 <= count; i += 2) {
        const uint32_t d0 = (uint32_t)dst[i + 0];
        const uint32_t d1 = (uint32_t)dst[i + 1];
        const uint32_t rb0 = ((s_rb_a + (d0 & 0xF81Fu) * ia_5) >> 5) & 0xF81Fu;
        const uint32_t g0  = ((s_g_a  + (d0 & 0x07E0u) * ia_5) >> 5) & 0x07E0u;
        const uint32_t rb1 = ((s_rb_a + (d1 & 0xF81Fu) * ia_5) >> 5) & 0xF81Fu;
        const uint32_t g1  = ((s_g_a  + (d1 & 0x07E0u) * ia_5) >> 5) & 0x07E0u;
        dst[i + 0] = (SWCanvasPixel)(rb0 | g0);
        dst[i + 1] = (SWCanvasPixel)(rb1 | g1);
    }
    for (; i < count; i++) {
        const uint32_t d = (uint32_t)dst[i];
        const uint32_t rb = ((s_rb_a + (d & 0xF81Fu) * ia_5) >> 5) & 0xF81Fu;
        const uint32_t g  = ((s_g_a  + (d & 0x07E0u) * ia_5) >> 5) & 0x07E0u;
        dst[i] = (SWCanvasPixel)(rb | g);
    }
#else
    for (int i = 0; i < count; i++) {
        dst[i] = blend_argb_onto_canvas(argb, dst[i]);
    }
#endif
}

void sw_blit_direct_row(SWCanvasPixel* dst, const uint32_t* src, uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const uint32_t s = modulate_pixel(src[i], modulate);
        dst[i] = blend_argb_onto_canvas(s, dst[i]);
    }
}

#endif // !__ARM_NEON

// Main 8bpp indexed path.

static inline void blit_pixel_opaque_or_blend(SWCanvasPixel* dst, uint32_t p) {
    const uint32_t a = p >> 24;
    if (a == 0xFFu) {
        *dst = sw_argb_to_canvas(p);
    } else if (a != 0u) {
        *dst = blend_argb_onto_canvas(p, *dst);
    }
}

static void sw_blit_indexed8_scalar(SWCanvasPixel* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate,
                                    int count) {
    int i = 0;

    if (modulate == 0xFFFFFFFFu) {
        // x4 unroll with prefetch.
        for (; i + 4 <= count; i += 4) {
#if defined(__GNUC__) || defined(__clang__)
            __builtin_prefetch(idx + i + 64);
#endif
            const uint32_t p0 = pal[idx[i + 0]];
            const uint32_t p1 = pal[idx[i + 1]];
            const uint32_t p2 = pal[idx[i + 2]];
            const uint32_t p3 = pal[idx[i + 3]];

            if (((p0 & p1 & p2 & p3) & 0xFF000000u) == 0xFF000000u) {
                dst[i + 0] = sw_argb_to_canvas(p0);
                dst[i + 1] = sw_argb_to_canvas(p1);
                dst[i + 2] = sw_argb_to_canvas(p2);
                dst[i + 3] = sw_argb_to_canvas(p3);
                continue;
            }

            blit_pixel_opaque_or_blend(&dst[i + 0], p0);
            blit_pixel_opaque_or_blend(&dst[i + 1], p1);
            blit_pixel_opaque_or_blend(&dst[i + 2], p2);
            blit_pixel_opaque_or_blend(&dst[i + 3], p3);
        }

        for (; i < count; i++) {
            blit_pixel_opaque_or_blend(&dst[i], pal[idx[i]]);
        }

        return;
    }

    for (; i + 4 <= count; i += 4) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(idx + i + 64);
#endif
        const uint32_t p0 = pal[idx[i + 0]];
        const uint32_t p1 = pal[idx[i + 1]];
        const uint32_t p2 = pal[idx[i + 2]];
        const uint32_t p3 = pal[idx[i + 3]];
        dst[i + 0] = blend_argb_onto_canvas(modulate_pixel(p0, modulate), dst[i + 0]);
        dst[i + 1] = blend_argb_onto_canvas(modulate_pixel(p1, modulate), dst[i + 1]);
        dst[i + 2] = blend_argb_onto_canvas(modulate_pixel(p2, modulate), dst[i + 2]);
        dst[i + 3] = blend_argb_onto_canvas(modulate_pixel(p3, modulate), dst[i + 3]);
    }

    for (; i < count; i++) {
        const uint32_t p = pal[idx[i]];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
    }
}

#if !defined(__ARM_NEON)
void sw_blit_indexed8_row(SWCanvasPixel* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate, int count,
                          char kernel) {
    (void)kernel;
    sw_blit_indexed8_scalar(dst, idx, pal, modulate, count);
}
#endif // !__ARM_NEON

// RGB565 fast paths.

#if defined(CRS_SW_CANVAS_16BPP)

static inline void blit_pixel_565(uint16_t* dst, uint32_t p, uint16_t p565) {
    const uint32_t a = p >> 24;
    if (a == 0xFFu) {
        *dst = p565;
    } else if (a != 0u) {
        *dst = blend_argb_onto_canvas(p, *dst);
    }
}

// 8bpp colour-key path.
static inline int any_byte_zero(uint32_t w) {
    return ((w - 0x01010101u) & ~w & 0x80808080u) != 0;
}

void sw_blit_indexed8_row_ckey_565(uint16_t* dst, const uint8_t* idx, const uint32_t* pal_u32, int count) {
    int i = 0;

    // 8-pixel inner loop.
    for (; i + 8 <= count; i += 8) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(idx + i + 64);
        __builtin_prefetch(dst + i + 32, 1);
#endif
        uint32_t w0, w1;
        memcpy(&w0, idx + i,     4);
        memcpy(&w1, idx + i + 4, 4);
        // Keep these as uint32_t to avoid extra zero-extends.
        const uint32_t i0 = w0 & 0xFFu;
        const uint32_t i1 = (w0 >> 8) & 0xFFu;
        const uint32_t i2 = (w0 >> 16) & 0xFFu;
        const uint32_t i3 = w0 >> 24;
        const uint32_t i4 = w1 & 0xFFu;
        const uint32_t i5 = (w1 >> 8) & 0xFFu;
        const uint32_t i6 = (w1 >> 16) & 0xFFu;
        const uint32_t i7 = w1 >> 24;

#if CRS_SW_CKEY8_FAST_PATH
        if (!any_byte_zero(w0) && !any_byte_zero(w1)) {
            const uint32_t p01 = pal_u32[i0] | (pal_u32[i1] << 16);
            const uint32_t p23 = pal_u32[i2] | (pal_u32[i3] << 16);
            const uint32_t p45 = pal_u32[i4] | (pal_u32[i5] << 16);
            const uint32_t p67 = pal_u32[i6] | (pal_u32[i7] << 16);
            memcpy(dst + i + 0, &p01, 4);
            memcpy(dst + i + 2, &p23, 4);
            memcpy(dst + i + 4, &p45, 4);
            memcpy(dst + i + 6, &p67, 4);
            continue;
        }
#endif

        if (i0) dst[i + 0] = (uint16_t)pal_u32[i0];
        if (i1) dst[i + 1] = (uint16_t)pal_u32[i1];
        if (i2) dst[i + 2] = (uint16_t)pal_u32[i2];
        if (i3) dst[i + 3] = (uint16_t)pal_u32[i3];
        if (i4) dst[i + 4] = (uint16_t)pal_u32[i4];
        if (i5) dst[i + 5] = (uint16_t)pal_u32[i5];
        if (i6) dst[i + 6] = (uint16_t)pal_u32[i6];
        if (i7) dst[i + 7] = (uint16_t)pal_u32[i7];
    }

    if (i + 4 <= count) {
        uint32_t w;
        memcpy(&w, idx + i, 4);
        const uint32_t i0 = w & 0xFFu;
        const uint32_t i1 = (w >> 8) & 0xFFu;
        const uint32_t i2 = (w >> 16) & 0xFFu;
        const uint32_t i3 = w >> 24;
        if (!any_byte_zero(w)) {
            const uint32_t p01 = pal_u32[i0] | (pal_u32[i1] << 16);
            const uint32_t p23 = pal_u32[i2] | (pal_u32[i3] << 16);
            memcpy(dst + i + 0, &p01, 4);
            memcpy(dst + i + 2, &p23, 4);
        } else {
            if (i0) dst[i + 0] = (uint16_t)pal_u32[i0];
            if (i1) dst[i + 1] = (uint16_t)pal_u32[i1];
            if (i2) dst[i + 2] = (uint16_t)pal_u32[i2];
            if (i3) dst[i + 3] = (uint16_t)pal_u32[i3];
        }
        i += 4;
    }

    for (; i < count; i++) {
        const uint32_t ix = idx[i];
        if (ix) dst[i] = (uint16_t)pal_u32[ix];
    }
}

void sw_blit_indexed8_row_rev_ckey_565(uint16_t* dst, const uint8_t* idx_last, const uint32_t* pal_u32, int count) {
    int i = 0;

    // Reverse path keeps the same packed store layout as the forward one.
    for (; i + 8 <= count; i += 8) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(idx_last - (i + 64));
        __builtin_prefetch(dst + i + 32, 1);
#endif
        uint32_t raw0, raw1;
        memcpy(&raw0, idx_last - (i + 3), 4);
        memcpy(&raw1, idx_last - (i + 7), 4);
        const uint32_t w0 = __builtin_bswap32(raw0);
        const uint32_t w1 = __builtin_bswap32(raw1);
        const uint32_t i0 = w0 & 0xFFu;
        const uint32_t i1 = (w0 >> 8) & 0xFFu;
        const uint32_t i2 = (w0 >> 16) & 0xFFu;
        const uint32_t i3 = w0 >> 24;
        const uint32_t i4 = w1 & 0xFFu;
        const uint32_t i5 = (w1 >> 8) & 0xFFu;
        const uint32_t i6 = (w1 >> 16) & 0xFFu;
        const uint32_t i7 = w1 >> 24;

#if CRS_SW_CKEY8_FAST_PATH
        if (!any_byte_zero(w0) && !any_byte_zero(w1)) {
            const uint32_t p01 = pal_u32[i0] | (pal_u32[i1] << 16);
            const uint32_t p23 = pal_u32[i2] | (pal_u32[i3] << 16);
            const uint32_t p45 = pal_u32[i4] | (pal_u32[i5] << 16);
            const uint32_t p67 = pal_u32[i6] | (pal_u32[i7] << 16);
            memcpy(dst + i + 0, &p01, 4);
            memcpy(dst + i + 2, &p23, 4);
            memcpy(dst + i + 4, &p45, 4);
            memcpy(dst + i + 6, &p67, 4);
            continue;
        }
#endif

        if (i0) dst[i + 0] = (uint16_t)pal_u32[i0];
        if (i1) dst[i + 1] = (uint16_t)pal_u32[i1];
        if (i2) dst[i + 2] = (uint16_t)pal_u32[i2];
        if (i3) dst[i + 3] = (uint16_t)pal_u32[i3];
        if (i4) dst[i + 4] = (uint16_t)pal_u32[i4];
        if (i5) dst[i + 5] = (uint16_t)pal_u32[i5];
        if (i6) dst[i + 6] = (uint16_t)pal_u32[i6];
        if (i7) dst[i + 7] = (uint16_t)pal_u32[i7];
    }

    if (i + 4 <= count) {
        uint32_t raw;
        memcpy(&raw, idx_last - (i + 3), 4);
        const uint32_t w = __builtin_bswap32(raw);
        const uint32_t i0 = w & 0xFFu;
        const uint32_t i1 = (w >> 8) & 0xFFu;
        const uint32_t i2 = (w >> 16) & 0xFFu;
        const uint32_t i3 = w >> 24;
        if (!any_byte_zero(w)) {
            const uint32_t p01 = pal_u32[i0] | (pal_u32[i1] << 16);
            const uint32_t p23 = pal_u32[i2] | (pal_u32[i3] << 16);
            memcpy(dst + i + 0, &p01, 4);
            memcpy(dst + i + 2, &p23, 4);
        } else {
            if (i0) dst[i + 0] = (uint16_t)pal_u32[i0];
            if (i1) dst[i + 1] = (uint16_t)pal_u32[i1];
            if (i2) dst[i + 2] = (uint16_t)pal_u32[i2];
            if (i3) dst[i + 3] = (uint16_t)pal_u32[i3];
        }
        i += 4;
    }

    for (; i < count; i++) {
        const uint32_t ix = idx_last[-i];
        if (ix) dst[i] = (uint16_t)pal_u32[ix];
    }
}

void sw_blit_indexed8_row_565(uint16_t* dst, const uint8_t* idx, const uint32_t* pal, const uint16_t* pal565,
                              int count) {
    int i = 0;

    for (; i + 4 <= count; i += 4) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(idx + i + 64);
#endif
        const uint8_t i0 = idx[i + 0];
        const uint8_t i1 = idx[i + 1];
        const uint8_t i2 = idx[i + 2];
        const uint8_t i3 = idx[i + 3];
        const uint32_t p0 = pal[i0];
        const uint32_t p1 = pal[i1];
        const uint32_t p2 = pal[i2];
        const uint32_t p3 = pal[i3];

        if (((p0 & p1 & p2 & p3) & 0xFF000000u) == 0xFF000000u) {
            dst[i + 0] = pal565[i0];
            dst[i + 1] = pal565[i1];
            dst[i + 2] = pal565[i2];
            dst[i + 3] = pal565[i3];
            continue;
        }

        blit_pixel_565(&dst[i + 0], p0, pal565[i0]);
        blit_pixel_565(&dst[i + 1], p1, pal565[i1]);
        blit_pixel_565(&dst[i + 2], p2, pal565[i2]);
        blit_pixel_565(&dst[i + 3], p3, pal565[i3]);
    }

    for (; i < count; i++) {
        const uint8_t ix = idx[i];
        blit_pixel_565(&dst[i], pal[ix], pal565[ix]);
    }
}

void sw_blit_indexed8_row_rev_565(uint16_t* dst, const uint8_t* idx_last, const uint32_t* pal, const uint16_t* pal565,
                                  int count) {
    int i = 0;

    for (; i + 4 <= count; i += 4) {
        const uint8_t i0 = idx_last[-(i + 0)];
        const uint8_t i1 = idx_last[-(i + 1)];
        const uint8_t i2 = idx_last[-(i + 2)];
        const uint8_t i3 = idx_last[-(i + 3)];
        const uint32_t p0 = pal[i0];
        const uint32_t p1 = pal[i1];
        const uint32_t p2 = pal[i2];
        const uint32_t p3 = pal[i3];

        if (((p0 & p1 & p2 & p3) & 0xFF000000u) == 0xFF000000u) {
            dst[i + 0] = pal565[i0];
            dst[i + 1] = pal565[i1];
            dst[i + 2] = pal565[i2];
            dst[i + 3] = pal565[i3];
            continue;
        }

        blit_pixel_565(&dst[i + 0], p0, pal565[i0]);
        blit_pixel_565(&dst[i + 1], p1, pal565[i1]);
        blit_pixel_565(&dst[i + 2], p2, pal565[i2]);
        blit_pixel_565(&dst[i + 3], p3, pal565[i3]);
    }

    for (; i < count; i++) {
        const uint8_t ix = idx_last[-i];
        blit_pixel_565(&dst[i], pal[ix], pal565[ix]);
    }
}

// 4bpp RGB565 fast path.
void sw_blit_indexed4_row_565(uint16_t* dst, const uint8_t* packed, const uint32_t* pal16, const uint16_t* pal565_16,
                              int count, int x_lsb) {
    int i = 0;
    int x = x_lsb;

    // Peel one pixel so the loop stays byte-aligned.
    if ((x & 1) && count > 0) {
        const uint8_t byte = packed[x >> 1];
        const uint8_t nib = (byte >> 4) & 0x0F;
        blit_pixel_565(&dst[i], pal16[nib], pal565_16[nib]);
        i++;
        x++;
    }

    // x4 unroll.
    for (; i + 4 <= count; i += 4, x += 4) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(packed + (x >> 1) + 32);
#endif
        const uint8_t b0 = packed[(x >> 1) + 0];
        const uint8_t b1 = packed[(x >> 1) + 1];
        const uint8_t n0 = b0 & 0x0F;
        const uint8_t n1 = (b0 >> 4) & 0x0F;
        const uint8_t n2 = b1 & 0x0F;
        const uint8_t n3 = (b1 >> 4) & 0x0F;
        const uint32_t p0 = pal16[n0];
        const uint32_t p1 = pal16[n1];
        const uint32_t p2 = pal16[n2];
        const uint32_t p3 = pal16[n3];

        if (((p0 & p1 & p2 & p3) & 0xFF000000u) == 0xFF000000u) {
            dst[i + 0] = pal565_16[n0];
            dst[i + 1] = pal565_16[n1];
            dst[i + 2] = pal565_16[n2];
            dst[i + 3] = pal565_16[n3];
            continue;
        }

        blit_pixel_565(&dst[i + 0], p0, pal565_16[n0]);
        blit_pixel_565(&dst[i + 1], p1, pal565_16[n1]);
        blit_pixel_565(&dst[i + 2], p2, pal565_16[n2]);
        blit_pixel_565(&dst[i + 3], p3, pal565_16[n3]);
    }

    if (i + 2 <= count) {
        const uint8_t byte = packed[x >> 1];
        const uint8_t n0 = byte & 0x0F;
        const uint8_t n1 = (byte >> 4) & 0x0F;
        const uint32_t p0 = pal16[n0];
        const uint32_t p1 = pal16[n1];

        if (((p0 & p1) & 0xFF000000u) == 0xFF000000u) {
            dst[i + 0] = pal565_16[n0];
            dst[i + 1] = pal565_16[n1];
        } else {
            blit_pixel_565(&dst[i + 0], p0, pal565_16[n0]);
            blit_pixel_565(&dst[i + 1], p1, pal565_16[n1]);
        }
        i += 2;
        x += 2;
    }

    if (i < count) {
        const uint8_t byte = packed[x >> 1];
        const uint8_t nib = (x & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        blit_pixel_565(&dst[i], pal16[nib], pal565_16[nib]);
    }
}

void sw_blit_indexed4_row_rev_565(uint16_t* dst, const uint8_t* packed, const uint32_t* pal16,
                                  const uint16_t* pal565_16, int count, int start_nibble) {
    int i = 0;
    int n = start_nibble;

    // Peel one pixel so the loop lines up.
    if (!(n & 1) && count > 0) {
        const uint8_t byte = packed[n >> 1];
        const uint8_t nib = byte & 0x0F;
        blit_pixel_565(&dst[i], pal16[nib], pal565_16[nib]);
        i++;
        n--;
    }

    // x4 unroll.
    for (; i + 4 <= count; i += 4, n -= 4) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(packed + (n >> 1) - 32);
#endif
        const uint8_t b0 = packed[(n >> 1) - 0];     // high-then-low of byte n>>1
        const uint8_t b1 = packed[(n >> 1) - 1];     // high-then-low of byte (n>>1)-1
        const uint8_t n0 = (b0 >> 4) & 0x0F;
        const uint8_t n1 = b0 & 0x0F;
        const uint8_t n2 = (b1 >> 4) & 0x0F;
        const uint8_t n3 = b1 & 0x0F;
        const uint32_t p0 = pal16[n0];
        const uint32_t p1 = pal16[n1];
        const uint32_t p2 = pal16[n2];
        const uint32_t p3 = pal16[n3];

        if (((p0 & p1 & p2 & p3) & 0xFF000000u) == 0xFF000000u) {
            dst[i + 0] = pal565_16[n0];
            dst[i + 1] = pal565_16[n1];
            dst[i + 2] = pal565_16[n2];
            dst[i + 3] = pal565_16[n3];
            continue;
        }

        blit_pixel_565(&dst[i + 0], p0, pal565_16[n0]);
        blit_pixel_565(&dst[i + 1], p1, pal565_16[n1]);
        blit_pixel_565(&dst[i + 2], p2, pal565_16[n2]);
        blit_pixel_565(&dst[i + 3], p3, pal565_16[n3]);
    }

    if (i + 2 <= count) {
        const uint8_t byte = packed[n >> 1];
        const uint8_t n0 = (byte >> 4) & 0x0F;
        const uint8_t n1 = byte & 0x0F;
        const uint32_t p0 = pal16[n0];
        const uint32_t p1 = pal16[n1];

        if (((p0 & p1) & 0xFF000000u) == 0xFF000000u) {
            dst[i + 0] = pal565_16[n0];
            dst[i + 1] = pal565_16[n1];
        } else {
            blit_pixel_565(&dst[i + 0], p0, pal565_16[n0]);
            blit_pixel_565(&dst[i + 1], p1, pal565_16[n1]);
        }
        i += 2;
        n -= 2;
    }

    for (; i < count; i++, n--) {
        const uint8_t byte = packed[n >> 1];
        const uint8_t nib = (n & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        blit_pixel_565(&dst[i], pal16[nib], pal565_16[nib]);
    }
}

// 4bpp colour-key path.
void sw_blit_indexed4_row_ckey_565(uint16_t* dst, const uint8_t* packed, const uint16_t* pal565_16, int count,
                                   int x_lsb) {
    int i = 0;
    int x = x_lsb;

    if ((x & 1) && count > 0) {
        const uint8_t nib = (packed[x >> 1] >> 4) & 0x0F;
        if (nib) dst[i] = pal565_16[nib];
        i++;
        x++;
    }

    // Keep this at 4 pixels; 8 regressed on target hardware.
    for (; i + 4 <= count; i += 4, x += 4) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(packed + (x >> 1) + 32);
        __builtin_prefetch(dst + i + 32, 1);
#endif
        const uint8_t b0 = packed[(x >> 1) + 0];
        const uint8_t b1 = packed[(x >> 1) + 1];
        const uint16_t w = (uint16_t)b0 | ((uint16_t)b1 << 8);
        const uint8_t n0 = b0 & 0x0F;
        const uint8_t n1 = (b0 >> 4) & 0x0F;
        const uint8_t n2 = b1 & 0x0F;
        const uint8_t n3 = (b1 >> 4) & 0x0F;

        if (!(((w - 0x1111u) & ~(uint32_t)w) & 0x8888u)) {
            dst[i + 0] = pal565_16[n0];
            dst[i + 1] = pal565_16[n1];
            dst[i + 2] = pal565_16[n2];
            dst[i + 3] = pal565_16[n3];
            continue;
        }

        if (n0) dst[i + 0] = pal565_16[n0];
        if (n1) dst[i + 1] = pal565_16[n1];
        if (n2) dst[i + 2] = pal565_16[n2];
        if (n3) dst[i + 3] = pal565_16[n3];
    }

    if (i + 2 <= count) {
        const uint8_t byte = packed[x >> 1];
        const uint8_t n0 = byte & 0x0F;
        const uint8_t n1 = (byte >> 4) & 0x0F;
        if (n0) dst[i + 0] = pal565_16[n0];
        if (n1) dst[i + 1] = pal565_16[n1];
        i += 2;
        x += 2;
    }

    if (i < count) {
        const uint8_t byte = packed[x >> 1];
        const uint8_t nib = (x & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        if (nib) dst[i] = pal565_16[nib];
    }
}

void sw_blit_indexed4_row_rev_ckey_565(uint16_t* dst, const uint8_t* packed, const uint16_t* pal565_16, int count,
                                       int start_nibble) {
    int i = 0;
    int n = start_nibble;

    if (!(n & 1) && count > 0) {
        const uint8_t nib = packed[n >> 1] & 0x0F;
        if (nib) dst[i] = pal565_16[nib];
        i++;
        n--;
    }

    // Keep this at 4 pixels; 8 regressed on target hardware.
    for (; i + 4 <= count; i += 4, n -= 4) {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_prefetch(packed + (n >> 1) - 32);
        __builtin_prefetch(dst + i + 32, 1);
#endif
        const uint8_t b0 = packed[(n >> 1) - 0];
        const uint8_t b1 = packed[(n >> 1) - 1];
        const uint16_t w = (uint16_t)b0 | ((uint16_t)b1 << 8);
        const uint8_t n0 = (b0 >> 4) & 0x0F;
        const uint8_t n1 = b0 & 0x0F;
        const uint8_t n2 = (b1 >> 4) & 0x0F;
        const uint8_t n3 = b1 & 0x0F;

        if (!(((w - 0x1111u) & ~(uint32_t)w) & 0x8888u)) {
            dst[i + 0] = pal565_16[n0];
            dst[i + 1] = pal565_16[n1];
            dst[i + 2] = pal565_16[n2];
            dst[i + 3] = pal565_16[n3];
            continue;
        }

        if (n0) dst[i + 0] = pal565_16[n0];
        if (n1) dst[i + 1] = pal565_16[n1];
        if (n2) dst[i + 2] = pal565_16[n2];
        if (n3) dst[i + 3] = pal565_16[n3];
    }

    if (i + 2 <= count) {
        const uint8_t byte = packed[n >> 1];
        const uint8_t n0 = (byte >> 4) & 0x0F;
        const uint8_t n1 = byte & 0x0F;
        if (n0) dst[i + 0] = pal565_16[n0];
        if (n1) dst[i + 1] = pal565_16[n1];
        i += 2;
        n -= 2;
    }

    for (; i < count; i++, n--) {
        const uint8_t byte = packed[n >> 1];
        const uint8_t nib = (n & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        if (nib) dst[i] = pal565_16[nib];
    }
}

#endif // CRS_SW_CANVAS_16BPP

// 4bpp indexed path.

static inline uint8_t nibble_at(const uint8_t* packed, int x) {
    const uint8_t byte = packed[x >> 1];
    return (x & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
}

#if !defined(__ARM_NEON)
void sw_blit_indexed4_row(SWCanvasPixel* dst, const uint8_t* packed, const uint32_t* pal16, uint32_t modulate,
                          int count, int x_lsb) {
    if (modulate == 0xFFFFFFFFu) {
        int i = 0;
        int x = x_lsb;

        // Peel one pixel so the loop is byte-aligned.
        if ((x & 1) && count > 0) {
            const uint8_t byte = packed[x >> 1];
            blit_pixel_opaque_or_blend(&dst[i], pal16[(byte >> 4) & 0x0F]);
            i++;
            x++;
        }

        for (; i + 2 <= count; i += 2, x += 2) {
            const uint8_t byte = packed[x >> 1];
            const uint32_t p0 = pal16[byte & 0x0F];
            const uint32_t p1 = pal16[(byte >> 4) & 0x0F];

            if (((p0 & p1) & 0xFF000000u) == 0xFF000000u) {
                dst[i + 0] = sw_argb_to_canvas(p0);
                dst[i + 1] = sw_argb_to_canvas(p1);
                continue;
            }

            blit_pixel_opaque_or_blend(&dst[i + 0], p0);
            blit_pixel_opaque_or_blend(&dst[i + 1], p1);
        }

        if (i < count) {
            const uint8_t byte = packed[x >> 1];
            const uint32_t p = pal16[(x & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F)];
            blit_pixel_opaque_or_blend(&dst[i], p);
        }

        return;
    }

    for (int i = 0; i < count; i++) {
        const uint8_t nib = nibble_at(packed, i + x_lsb);
        const uint32_t p = pal16[nib];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
    }
}
#endif // !__ARM_NEON

// Scaled variants.

void sw_blit_scaled_indexed8_row(SWCanvasPixel* dst, const uint8_t* idx_row, const uint32_t* pal, uint32_t u_fx,
                                 uint32_t du_fx, uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const uint8_t nx = idx_row[u_fx >> 16];
        const uint32_t p = pal[nx];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
        u_fx += du_fx;
    }
}

void sw_blit_scaled_indexed4_row(SWCanvasPixel* dst, const uint8_t* packed_row, const uint32_t* pal16, uint32_t u_fx,
                                 uint32_t du_fx, uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const int n = (int)(u_fx >> 16);
        const uint8_t byte = packed_row[n >> 1];
        const uint8_t nib = (n & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        const uint32_t p = pal16[nib];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
        u_fx += du_fx;
    }
}

void sw_blit_scaled_direct_row(SWCanvasPixel* dst, const uint32_t* src_row, uint32_t u_fx, uint32_t du_fx,
                               uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const uint32_t s = modulate_pixel(src_row[u_fx >> 16], modulate);
        dst[i] = blend_argb_onto_canvas(s, dst[i]);
        u_fx += du_fx;
    }
}

// X-flip variants.

void sw_blit_direct_row_rev(SWCanvasPixel* dst, const uint32_t* src_last, uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const uint32_t s = modulate_pixel(src_last[-i], modulate);
        dst[i] = blend_argb_onto_canvas(s, dst[i]);
    }
}

void sw_blit_indexed8_row_rev(SWCanvasPixel* dst, const uint8_t* idx_last, const uint32_t* pal, uint32_t modulate,
                              int count) {
    for (int i = 0; i < count; i++) {
        const uint32_t p = pal[idx_last[-i]];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
    }
}

void sw_blit_scaled_indexed8_row_rev(SWCanvasPixel* dst, const uint8_t* idx_row, const uint32_t* pal, uint32_t u_fx,
                                     uint32_t du_fx, uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const uint8_t nx = idx_row[u_fx >> 16];
        const uint32_t p = pal[nx];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
        u_fx -= du_fx;
    }
}

void sw_blit_scaled_indexed4_row_rev(SWCanvasPixel* dst, const uint8_t* packed_row, const uint32_t* pal16, uint32_t u_fx,
                                     uint32_t du_fx, uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const int n = (int)(u_fx >> 16);
        const uint8_t byte = packed_row[n >> 1];
        const uint8_t nib = (n & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        const uint32_t p = pal16[nib];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
        u_fx -= du_fx;
    }
}

void sw_blit_scaled_direct_row_rev(SWCanvasPixel* dst, const uint32_t* src_row, uint32_t u_fx, uint32_t du_fx,
                                   uint32_t modulate, int count) {
    for (int i = 0; i < count; i++) {
        const uint32_t s = modulate_pixel(src_row[u_fx >> 16], modulate);
        dst[i] = blend_argb_onto_canvas(s, dst[i]);
        u_fx -= du_fx;
    }
}

void sw_blit_indexed4_row_rev(SWCanvasPixel* dst, const uint8_t* packed, const uint32_t* pal16, uint32_t modulate,
                              int count, int start_nibble) {
    for (int i = 0; i < count; i++) {
        const int n = start_nibble - i;
        const uint8_t byte = packed[n >> 1];
        const uint8_t nib = (n & 1) ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        const uint32_t p = pal16[nib];
        dst[i] = blend_argb_onto_canvas(modulate_pixel(p, modulate), dst[i]);
    }
}

#if defined(CRS_SW_CANVAS_16BPP)
// Scaled 8bpp colour-key path. Keep this on the direct pal565 route.
void sw_blit_scaled_indexed8_row_ckey_565(uint16_t* dst, const uint8_t* idx_row, const uint16_t* pal565,
                                          uint32_t u_fx, uint32_t du_fx, int count) {
    for (int i = 0; i < count; i++) {
        const uint32_t ix = idx_row[u_fx >> 16];
        if (ix) dst[i] = pal565[ix];
        u_fx += du_fx;
    }
}

void sw_blit_scaled_indexed8_row_rev_ckey_565(uint16_t* dst, const uint8_t* idx_row, const uint16_t* pal565,
                                              uint32_t u_fx, uint32_t du_fx, int count) {
    for (int i = 0; i < count; i++) {
        const uint32_t ix = idx_row[u_fx >> 16];
        if (ix) dst[i] = pal565[ix];
        u_fx -= du_fx;
    }
}
#endif

// Present-time canvas scale for the SDL software path.

#if !defined(CRS_SW_CANVAS_16BPP)

static inline uint32_t lerp_argb(uint32_t a, uint32_t b, uint32_t t /* 0..256 */) {
    const uint32_t ia = 256 - t;
    const uint32_t ar = (a >> 16) & 0xFF;
    const uint32_t ag = (a >> 8) & 0xFF;
    const uint32_t ab = a & 0xFF;
    const uint32_t br = (b >> 16) & 0xFF;
    const uint32_t bg = (b >> 8) & 0xFF;
    const uint32_t bb = b & 0xFF;
    const uint32_t rr = (ar * ia + br * t) >> 8;
    const uint32_t rg = (ag * ia + bg * t) >> 8;
    const uint32_t rb = (ab * ia + bb * t) >> 8;
    return 0xFF000000u | (rr << 16) | (rg << 8) | rb;
}

void sw_present_scale_argb_scalar(uint32_t* dst, int dst_pitch_px, int dst_w, int dst_h, const uint32_t* src,
                                  int src_pitch_px, int src_w, int src_h, bool nearest) {
    const uint32_t du_fx = (uint32_t)(((uint64_t)src_w << 16) / (uint64_t)dst_w);
    const uint32_t dv_fx = (uint32_t)(((uint64_t)src_h << 16) / (uint64_t)dst_h);

    if (nearest) {
        uint32_t v_fx = 0;

        for (int y = 0; y < dst_h; y++) {
            const uint32_t* srow = src + (v_fx >> 16) * src_pitch_px;
            uint32_t* drow = dst + y * dst_pitch_px;
            uint32_t u_fx = 0;

            for (int x = 0; x < dst_w; x++) {
                drow[x] = srow[u_fx >> 16];
                u_fx += du_fx;
            }

            v_fx += dv_fx;
        }

        return;
    }

    // Bilinear with a half-texel offset.
    const uint32_t u0 = du_fx >> 1;
    const uint32_t v0 = dv_fx >> 1;
    uint32_t v_fx = v0;

    for (int y = 0; y < dst_h; y++) {
        int vy = (int)(v_fx >> 16);
        int vy1 = vy + 1;

        if (vy1 >= src_h) {
            vy1 = src_h - 1;
        }

        const uint32_t tv = ((v_fx >> 8) & 0xFF) + (((v_fx >> 8) & 0xFF) >> 7);
        const uint32_t* r0 = src + vy * src_pitch_px;
        const uint32_t* r1 = src + vy1 * src_pitch_px;
        uint32_t* drow = dst + y * dst_pitch_px;
        uint32_t u_fx = u0;

        for (int x = 0; x < dst_w; x++) {
            int ux = (int)(u_fx >> 16);
            int ux1 = ux + 1;

            if (ux1 >= src_w) {
                ux1 = src_w - 1;
            }

            const uint32_t tu = ((u_fx >> 8) & 0xFF) + (((u_fx >> 8) & 0xFF) >> 7);
            const uint32_t a = lerp_argb(r0[ux], r0[ux1], tu);
            const uint32_t b = lerp_argb(r1[ux], r1[ux1], tu);
            drow[x] = lerp_argb(a, b, tv);
            u_fx += du_fx;
        }

        v_fx += dv_fx;
    }
}

#if !defined(__ARM_NEON)
void sw_present_scale_argb(uint32_t* dst, int dst_pitch_px, int dst_w, int dst_h, const uint32_t* src, int src_pitch_px,
                           int src_w, int src_h, bool nearest) {
    sw_present_scale_argb_scalar(dst, dst_pitch_px, dst_w, dst_h, src, src_pitch_px, src_w, src_h, nearest);
}
#endif

#endif // !CRS_SW_CANVAS_16BPP

// Export scalar helpers for the NEON file.
void sw_blit_indexed8_row_scalar(SWCanvasPixel* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate,
                                 int count) {
    sw_blit_indexed8_scalar(dst, idx, pal, modulate, count);
}

#endif // CRS_VIDEO_DRIVER_SOFTWARE
