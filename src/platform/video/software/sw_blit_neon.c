#if CRS_VIDEO_DRIVER_SOFTWARE && defined(__ARM_NEON) && defined(__aarch64__) && !defined(CRS_SW_CANVAS_16BPP)

#include "platform/video/software/sw_blit.h"

#include <arm_neon.h>

// AArch64 NEON kernels.
//
// Conventions:
//   * ARGB8888 canvas in little-endian memory => byte order is B, G, R, A.
//   * sw_blit.c provides scalar fallbacks we call via sw_blit_indexed8_row_scalar
//     when kernel == 'b'.
//   * The scalar divide-by-255 mirror uses (x + 128) >> 8; NEON uses the exact
//     same approximation so the two paths agree within ±1/255 per channel.

void sw_blit_indexed8_row_scalar(uint32_t* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate, int count);

// Deinterleave four 128-bit ARGB lanes into separate channel vectors.
static inline void deinterleave_argb(uint8x16x4_t v, uint8x16_t* b, uint8x16_t* g, uint8x16_t* r, uint8x16_t* a) {
    *b = v.val[0];
    *g = v.val[1];
    *r = v.val[2];
    *a = v.val[3];
}

// Fast "(x * y + 128) >> 8" approximation of (x * y) / 255 for 8-bit x/y.
static inline uint8x16_t mul_u8_approx(uint8x16_t x, uint8x16_t y) {
    uint16x8_t lo = vmull_u8(vget_low_u8(x), vget_low_u8(y));
    uint16x8_t hi = vmull_u8(vget_high_u8(x), vget_high_u8(y));
    lo = vaddq_u16(lo, vdupq_n_u16(128));
    hi = vaddq_u16(hi, vdupq_n_u16(128));
    return vcombine_u8(vshrn_n_u16(lo, 8), vshrn_n_u16(hi, 8));
}

// src_over blend: out = src * sa + dst * (255 - sa), per channel. Output alpha is
// forced to 0xFF (canvas never needs src-alpha retention). `src_alpha_zero_mask`
// preserves dst pixels where src alpha is 0 (avoids blending transparent texels).
static inline uint8x16x4_t blend_src_over_neon(uint8x16_t sb, uint8x16_t sg, uint8x16_t sr, uint8x16_t sa,
                                               uint8x16x4_t dst_v) {
    uint8x16_t db = dst_v.val[0];
    uint8x16_t dg = dst_v.val[1];
    uint8x16_t dr = dst_v.val[2];
    uint8x16_t ia = vsubq_u8(vdupq_n_u8(255), sa);

    uint8x16_t ob = vqaddq_u8(mul_u8_approx(sb, sa), mul_u8_approx(db, ia));
    uint8x16_t og = vqaddq_u8(mul_u8_approx(sg, sa), mul_u8_approx(dg, ia));
    uint8x16_t or_ = vqaddq_u8(mul_u8_approx(sr, sa), mul_u8_approx(dr, ia));

    // Preserve destination where src alpha is exactly 0 (transparent pixel).
    uint8x16_t zero_mask = vceqq_u8(sa, vdupq_n_u8(0));
    ob = vbslq_u8(zero_mask, db, ob);
    og = vbslq_u8(zero_mask, dg, og);
    or_ = vbslq_u8(zero_mask, dr, or_);

    uint8x16x4_t out = { { ob, og, or_, vdupq_n_u8(0xFF) } };
    return out;
}

static inline bool all_opaque(uint8x16_t a) {
    uint8x16_t cmp = vceqq_u8(a, vdupq_n_u8(0xFF));
    return vminvq_u8(cmp) == 0xFF;
}

void sw_fill_solid_row(uint32_t* dst, uint32_t argb, int count) {
    const uint32_t a = (argb >> 24) & 0xFF;

    if (a == 0xFF) {
        uint32x4_t c = vdupq_n_u32(argb);
        int i = 0;

        for (; i + 16 <= count; i += 16) {
            vst1q_u32(dst + i + 0, c);
            vst1q_u32(dst + i + 4, c);
            vst1q_u32(dst + i + 8, c);
            vst1q_u32(dst + i + 12, c);
        }

        for (; i < count; i++) {
            dst[i] = argb;
        }

        return;
    }

    // Blended solid fill (rare path).
    uint8x16_t sb = vdupq_n_u8(argb & 0xFF);
    uint8x16_t sg = vdupq_n_u8((argb >> 8) & 0xFF);
    uint8x16_t sr = vdupq_n_u8((argb >> 16) & 0xFF);
    uint8x16_t sa = vdupq_n_u8(a);
    int i = 0;

    for (; i + 16 <= count; i += 16) {
        uint8x16x4_t dv = vld4q_u8((const uint8_t*)(dst + i));
        uint8x16x4_t ov = blend_src_over_neon(sb, sg, sr, sa, dv);
        vst4q_u8((uint8_t*)(dst + i), ov);
    }

    if (i < count) {
        // Tail via scalar path in sw_blit.c isn't exported for this kernel; do it here.
        const uint32_t ia = 255 - a;
        const uint32_t sr_s = (argb >> 16) & 0xFF;
        const uint32_t sg_s = (argb >> 8) & 0xFF;
        const uint32_t sb_s = argb & 0xFF;

        for (; i < count; i++) {
            const uint32_t d = dst[i];
            const uint32_t dr = (d >> 16) & 0xFF;
            const uint32_t dg = (d >> 8) & 0xFF;
            const uint32_t db = d & 0xFF;
            const uint32_t rr = (sr_s * a + dr * ia + 128) >> 8;
            const uint32_t rg = (sg_s * a + dg * ia + 128) >> 8;
            const uint32_t rb = (sb_s * a + db * ia + 128) >> 8;
            dst[i] = 0xFF000000u | (rr << 16) | (rg << 8) | rb;
        }
    }
}

static inline uint8x16x4_t modulate_if_needed(uint8x16x4_t src_v, uint32_t modulate) {
    if (modulate == 0xFFFFFFFFu) {
        return src_v;
    }

    uint8x16_t mb = vdupq_n_u8(modulate & 0xFF);
    uint8x16_t mg = vdupq_n_u8((modulate >> 8) & 0xFF);
    uint8x16_t mr = vdupq_n_u8((modulate >> 16) & 0xFF);
    uint8x16_t ma = vdupq_n_u8((modulate >> 24) & 0xFF);

    uint8x16x4_t out;
    out.val[0] = mul_u8_approx(src_v.val[0], mb);
    out.val[1] = mul_u8_approx(src_v.val[1], mg);
    out.val[2] = mul_u8_approx(src_v.val[2], mr);
    out.val[3] = mul_u8_approx(src_v.val[3], ma);
    return out;
}

void sw_blit_direct_row(uint32_t* dst, const uint32_t* src, uint32_t modulate, int count) {
    int i = 0;

    for (; i + 16 <= count; i += 16) {
        uint8x16x4_t sv = vld4q_u8((const uint8_t*)(src + i));
        sv = modulate_if_needed(sv, modulate);

        if (all_opaque(sv.val[3]) && modulate == 0xFFFFFFFFu) {
            vst4q_u8((uint8_t*)(dst + i), sv);
            continue;
        }

        uint8x16x4_t dv = vld4q_u8((const uint8_t*)(dst + i));
        uint8x16x4_t ov = blend_src_over_neon(sv.val[0], sv.val[1], sv.val[2], sv.val[3], dv);
        vst4q_u8((uint8_t*)(dst + i), ov);
    }

    for (; i < count; i++) {
        uint32_t s = src[i];
        const uint32_t sa = (s >> 24) & 0xFF;

        if (sa == 0) {
            continue;
        }

        // Scalar tail mirrors sw_blit.c's modulate_pixel + blend_src_over.
        if (modulate != 0xFFFFFFFFu) {
            const uint32_t ma = (modulate >> 24) & 0xFF;
            const uint32_t mr = (modulate >> 16) & 0xFF;
            const uint32_t mg = (modulate >> 8) & 0xFF;
            const uint32_t mb = modulate & 0xFF;
            const uint32_t ra = (sa * ma + 128) >> 8;
            const uint32_t rr = (((s >> 16) & 0xFF) * mr + 128) >> 8;
            const uint32_t rg = (((s >> 8) & 0xFF) * mg + 128) >> 8;
            const uint32_t rb = ((s & 0xFF) * mb + 128) >> 8;
            s = (ra << 24) | (rr << 16) | (rg << 8) | rb;
        }

        const uint32_t ca = (s >> 24) & 0xFF;

        if (ca == 0xFF) {
            dst[i] = s;
            continue;
        }

        const uint32_t ia = 255 - ca;
        const uint32_t d = dst[i];
        const uint32_t rr = (((s >> 16) & 0xFF) * ca + ((d >> 16) & 0xFF) * ia + 128) >> 8;
        const uint32_t rg = (((s >> 8) & 0xFF) * ca + ((d >> 8) & 0xFF) * ia + 128) >> 8;
        const uint32_t rb = ((s & 0xFF) * ca + (d & 0xFF) * ia + 128) >> 8;
        dst[i] = 0xFF000000u | (rr << 16) | (rg << 8) | rb;
    }
}

// 4bpp hero path.  16 palette entries, so one vqtbl1q_u8 per channel is enough.
void sw_blit_indexed4_row(uint32_t* dst, const uint8_t* packed, const uint32_t* pal16, uint32_t modulate, int count,
                          int x_lsb) {
    // Deinterleave 16-entry palette into 4 channel vectors (each 16 bytes).
    uint32x4_t pv0 = vld1q_u32(pal16 + 0);
    uint32x4_t pv1 = vld1q_u32(pal16 + 4);
    uint32x4_t pv2 = vld1q_u32(pal16 + 8);
    uint32x4_t pv3 = vld1q_u32(pal16 + 12);
    uint8x16x4_t pal_v = vld4q_u8((const uint8_t*)pal16);
    (void)pv0;
    (void)pv1;
    (void)pv2;
    (void)pv3;
    uint8x16_t pal_b = pal_v.val[0];
    uint8x16_t pal_g = pal_v.val[1];
    uint8x16_t pal_r = pal_v.val[2];
    uint8x16_t pal_a = pal_v.val[3];

    int i = 0;

    // When x_lsb is 0 we can consume 16 pixels from 8 packed bytes; otherwise the
    // first pixel is a high nibble and the remaining 15 interleave awkwardly, so
    // we walk one pixel scalar-style to realign, then take the fast path.
    if (x_lsb != 0 && count > 0) {
        const uint8_t byte = packed[0];
        const uint8_t nib = (byte >> 4) & 0x0F;
        const uint32_t p = pal16[nib];
        const uint32_t sa = (p >> 24) & 0xFF;

        if (sa == 0xFF && modulate == 0xFFFFFFFFu) {
            dst[0] = p;
        } else if (sa != 0) {
            // Scalar blend (rare)
            const uint32_t m = modulate;
            uint32_t s = p;

            if (m != 0xFFFFFFFFu) {
                const uint32_t ma = (m >> 24) & 0xFF;
                const uint32_t mr = (m >> 16) & 0xFF;
                const uint32_t mg = (m >> 8) & 0xFF;
                const uint32_t mb = m & 0xFF;
                s = (((sa * ma + 128) >> 8) << 24) | ((((p >> 16) & 0xFF) * mr + 128) >> 8) << 16 |
                    ((((p >> 8) & 0xFF) * mg + 128) >> 8) << 8 | (((p & 0xFF) * mb + 128) >> 8);
            }

            const uint32_t ca = (s >> 24) & 0xFF;

            if (ca == 0xFF) {
                dst[0] = s;
            } else {
                const uint32_t ia = 255 - ca;
                const uint32_t d = dst[0];
                const uint32_t rr = (((s >> 16) & 0xFF) * ca + ((d >> 16) & 0xFF) * ia + 128) >> 8;
                const uint32_t rg = (((s >> 8) & 0xFF) * ca + ((d >> 8) & 0xFF) * ia + 128) >> 8;
                const uint32_t rb = ((s & 0xFF) * ca + (d & 0xFF) * ia + 128) >> 8;
                dst[0] = 0xFF000000u | (rr << 16) | (rg << 8) | rb;
            }
        }

        i = 1;
        packed += 1;
    }

    // 16-pixel SIMD body.  Consumes 8 packed bytes per iteration.
    for (; i + 16 <= count; i += 16) {
        uint8x8_t packed8 = vld1_u8(packed);
        uint8x8_t lo = vand_u8(packed8, vdup_n_u8(0x0F));
        uint8x8_t hi = vshr_n_u8(packed8, 4);
        uint8x8x2_t zz = vzip_u8(lo, hi);
        uint8x16_t idx = vcombine_u8(zz.val[0], zz.val[1]);

        uint8x16_t b = vqtbl1q_u8(pal_b, idx);
        uint8x16_t g = vqtbl1q_u8(pal_g, idx);
        uint8x16_t r = vqtbl1q_u8(pal_r, idx);
        uint8x16_t a = vqtbl1q_u8(pal_a, idx);

        uint8x16x4_t sv = { { b, g, r, a } };
        sv = modulate_if_needed(sv, modulate);

        if (all_opaque(sv.val[3]) && modulate == 0xFFFFFFFFu) {
            vst4q_u8((uint8_t*)(dst + i), sv);
        } else {
            uint8x16x4_t dv = vld4q_u8((const uint8_t*)(dst + i));
            uint8x16x4_t ov = blend_src_over_neon(sv.val[0], sv.val[1], sv.val[2], sv.val[3], dv);
            vst4q_u8((uint8_t*)(dst + i), ov);
        }

        packed += 8;
    }

    // Scalar tail.
    int local_x = (x_lsb != 0) ? 1 : 0;

    for (; i < count; i++, local_x++) {
        const uint8_t byte = packed[(local_x - ((x_lsb != 0) ? 1 : 0)) >> 1];
        const uint8_t nib = ((local_x & 1) == 0) ? (byte & 0x0F) : ((byte >> 4) & 0x0F);
        const uint32_t p = pal16[nib];
        const uint32_t sa = (p >> 24) & 0xFF;

        if (sa == 0) {
            continue;
        }

        uint32_t s = p;

        if (modulate != 0xFFFFFFFFu) {
            const uint32_t ma = (modulate >> 24) & 0xFF;
            const uint32_t mr = (modulate >> 16) & 0xFF;
            const uint32_t mg = (modulate >> 8) & 0xFF;
            const uint32_t mb = modulate & 0xFF;
            s = (((sa * ma + 128) >> 8) << 24) | ((((p >> 16) & 0xFF) * mr + 128) >> 8) << 16 |
                ((((p >> 8) & 0xFF) * mg + 128) >> 8) << 8 | (((p & 0xFF) * mb + 128) >> 8);
        }

        const uint32_t ca = (s >> 24) & 0xFF;

        if (ca == 0xFF) {
            dst[i] = s;
            continue;
        }

        const uint32_t ia = 255 - ca;
        const uint32_t d = dst[i];
        const uint32_t rr = (((s >> 16) & 0xFF) * ca + ((d >> 16) & 0xFF) * ia + 128) >> 8;
        const uint32_t rg = (((s >> 8) & 0xFF) * ca + ((d >> 8) & 0xFF) * ia + 128) >> 8;
        const uint32_t rb = ((s & 0xFF) * ca + (d & 0xFF) * ia + 128) >> 8;
        dst[i] = 0xFF000000u | (rr << 16) | (rg << 8) | rb;
    }
}

// 8bpp palette via the quadrant TBL trick. Each 64-byte quadrant is addressed with
// an adjusted index (idx - 64*q); out-of-range indices return 0 from vqtbl4q_u8,
// so OR-ing the four quadrant results reconstructs the full lookup per channel.
static void sw_blit_indexed8_row_neon(uint32_t* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate,
                                      int count) {
    // Deinterleave the 256-entry palette into 4 channel arrays of 256 bytes each.
    // We store them on the stack once per call; the compiler may hoist for inner loops.
    uint8_t __attribute__((aligned(16))) pal_b[256];
    uint8_t __attribute__((aligned(16))) pal_g[256];
    uint8_t __attribute__((aligned(16))) pal_r[256];
    uint8_t __attribute__((aligned(16))) pal_a[256];

    for (int i = 0; i < 256; i += 16) {
        uint8x16x4_t v = vld4q_u8((const uint8_t*)(pal + i));
        vst1q_u8(pal_b + i, v.val[0]);
        vst1q_u8(pal_g + i, v.val[1]);
        vst1q_u8(pal_r + i, v.val[2]);
        vst1q_u8(pal_a + i, v.val[3]);
    }

    int i = 0;
    const uint8x16_t k64 = vdupq_n_u8(64);
    const uint8x16_t k128 = vdupq_n_u8(128);
    const uint8x16_t k192 = vdupq_n_u8(192);

    for (; i + 16 <= count; i += 16) {
        uint8x16_t ix = vld1q_u8(idx + i);
        uint8x16_t i1 = vsubq_u8(ix, k64);
        uint8x16_t i2 = vsubq_u8(ix, k128);
        uint8x16_t i3 = vsubq_u8(ix, k192);

#define LOAD_Q4(base, off) *((const uint8x16x4_t*)(const void*)((base) + (off)))

        uint8x16x4_t tb0 = LOAD_Q4(pal_b, 0);
        uint8x16x4_t tb1 = LOAD_Q4(pal_b, 64);
        uint8x16x4_t tb2 = LOAD_Q4(pal_b, 128);
        uint8x16x4_t tb3 = LOAD_Q4(pal_b, 192);
        uint8x16_t b = vqtbl4q_u8(tb0, ix);
        b = vorrq_u8(b, vqtbl4q_u8(tb1, i1));
        b = vorrq_u8(b, vqtbl4q_u8(tb2, i2));
        b = vorrq_u8(b, vqtbl4q_u8(tb3, i3));

        uint8x16x4_t tg0 = LOAD_Q4(pal_g, 0);
        uint8x16x4_t tg1 = LOAD_Q4(pal_g, 64);
        uint8x16x4_t tg2 = LOAD_Q4(pal_g, 128);
        uint8x16x4_t tg3 = LOAD_Q4(pal_g, 192);
        uint8x16_t g = vqtbl4q_u8(tg0, ix);
        g = vorrq_u8(g, vqtbl4q_u8(tg1, i1));
        g = vorrq_u8(g, vqtbl4q_u8(tg2, i2));
        g = vorrq_u8(g, vqtbl4q_u8(tg3, i3));

        uint8x16x4_t tr0 = LOAD_Q4(pal_r, 0);
        uint8x16x4_t tr1 = LOAD_Q4(pal_r, 64);
        uint8x16x4_t tr2 = LOAD_Q4(pal_r, 128);
        uint8x16x4_t tr3 = LOAD_Q4(pal_r, 192);
        uint8x16_t r = vqtbl4q_u8(tr0, ix);
        r = vorrq_u8(r, vqtbl4q_u8(tr1, i1));
        r = vorrq_u8(r, vqtbl4q_u8(tr2, i2));
        r = vorrq_u8(r, vqtbl4q_u8(tr3, i3));

        uint8x16x4_t ta0 = LOAD_Q4(pal_a, 0);
        uint8x16x4_t ta1 = LOAD_Q4(pal_a, 64);
        uint8x16x4_t ta2 = LOAD_Q4(pal_a, 128);
        uint8x16x4_t ta3 = LOAD_Q4(pal_a, 192);
        uint8x16_t a = vqtbl4q_u8(ta0, ix);
        a = vorrq_u8(a, vqtbl4q_u8(ta1, i1));
        a = vorrq_u8(a, vqtbl4q_u8(ta2, i2));
        a = vorrq_u8(a, vqtbl4q_u8(ta3, i3));

#undef LOAD_Q4

        uint8x16x4_t sv = { { b, g, r, a } };
        sv = modulate_if_needed(sv, modulate);

        if (all_opaque(sv.val[3]) && modulate == 0xFFFFFFFFu) {
            vst4q_u8((uint8_t*)(dst + i), sv);
        } else {
            uint8x16x4_t dv = vld4q_u8((const uint8_t*)(dst + i));
            uint8x16x4_t ov = blend_src_over_neon(sv.val[0], sv.val[1], sv.val[2], sv.val[3], dv);
            vst4q_u8((uint8_t*)(dst + i), ov);
        }
    }

    if (i < count) {
        sw_blit_indexed8_row_scalar(dst + i, idx + i, pal, modulate, count - i);
    }
}

void sw_blit_indexed8_row(uint32_t* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate, int count,
                          char kernel) {
    if (kernel == 'a') {
        sw_blit_indexed8_row_neon(dst, idx, pal, modulate, count);
    } else {
        sw_blit_indexed8_row_scalar(dst, idx, pal, modulate, count);
    }
}

// Present-time upscale: nearest neighbour fast path via NEON gather emulation.
// Bilinear stays scalar (rare mode; the GL soft-linear path is rarely selected on
// RPi where linear mode delegates to the SDL renderer's scaler instead).
void sw_present_scale_argb(uint32_t* dst, int dst_pitch_px, int dst_w, int dst_h, const uint32_t* src, int src_pitch_px,
                           int src_w, int src_h, bool nearest) {
    const uint32_t du_fx = (uint32_t)(((uint64_t)src_w << 16) / (uint64_t)dst_w);
    const uint32_t dv_fx = (uint32_t)(((uint64_t)src_h << 16) / (uint64_t)dst_h);

    if (!nearest) {
        // Fall through to a scalar bilinear; matching sw_blit.c implementation.
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
                const uint32_t a00 = r0[ux];
                const uint32_t a10 = r0[ux1];
                const uint32_t a01 = r1[ux];
                const uint32_t a11 = r1[ux1];

                const uint32_t ia = 256 - tu;
                const uint32_t rr_top = (((a00 >> 16) & 0xFF) * ia + ((a10 >> 16) & 0xFF) * tu) >> 8;
                const uint32_t rg_top = (((a00 >> 8) & 0xFF) * ia + ((a10 >> 8) & 0xFF) * tu) >> 8;
                const uint32_t rb_top = ((a00 & 0xFF) * ia + (a10 & 0xFF) * tu) >> 8;
                const uint32_t rr_bot = (((a01 >> 16) & 0xFF) * ia + ((a11 >> 16) & 0xFF) * tu) >> 8;
                const uint32_t rg_bot = (((a01 >> 8) & 0xFF) * ia + ((a11 >> 8) & 0xFF) * tu) >> 8;
                const uint32_t rb_bot = ((a01 & 0xFF) * ia + (a11 & 0xFF) * tu) >> 8;
                const uint32_t iv = 256 - tv;
                const uint32_t rr = (rr_top * iv + rr_bot * tv) >> 8;
                const uint32_t rg = (rg_top * iv + rg_bot * tv) >> 8;
                const uint32_t rb = (rb_top * iv + rb_bot * tv) >> 8;
                drow[x] = 0xFF000000u | (rr << 16) | (rg << 8) | rb;
                u_fx += du_fx;
            }

            v_fx += dv_fx;
        }

        return;
    }

    // Integer 2x/3x/4x fast paths — common for HDMI on Pi (1280x720 is exactly
    // 3.33x vertically from 224, but 768p and custom modes hit integer multiples).
    const bool integer = (src_w * ((uint32_t)dst_w / (uint32_t)src_w) == (uint32_t)dst_w) &&
                         (src_h * ((uint32_t)dst_h / (uint32_t)src_h) == (uint32_t)dst_h);

    if (integer) {
        const int sx = dst_w / src_w;
        const int sy = dst_h / src_h;

        for (int y = 0; y < src_h; y++) {
            const uint32_t* srow = src + y * src_pitch_px;
            uint32_t* drow_base = dst + y * sy * dst_pitch_px;

            // First destination row: replicate each source pixel sx times.
            for (int x = 0; x < src_w; x++) {
                uint32_t c = srow[x];
                uint32_t* d = drow_base + x * sx;

                for (int k = 0; k < sx; k++) {
                    d[k] = c;
                }
            }

            // Copy the first row down sy-1 times.
            for (int k = 1; k < sy; k++) {
                uint32_t* drow_k = drow_base + k * dst_pitch_px;
                int j = 0;

                for (; j + 16 <= dst_w; j += 16) {
                    vst1q_u32(drow_k + j + 0, vld1q_u32(drow_base + j + 0));
                    vst1q_u32(drow_k + j + 4, vld1q_u32(drow_base + j + 4));
                    vst1q_u32(drow_k + j + 8, vld1q_u32(drow_base + j + 8));
                    vst1q_u32(drow_k + j + 12, vld1q_u32(drow_base + j + 12));
                }

                for (; j < dst_w; j++) {
                    drow_k[j] = drow_base[j];
                }
            }
        }

        return;
    }

    // Generic fixed-point nearest.
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
}

#endif // CRS_VIDEO_DRIVER_SOFTWARE && AArch64 NEON
