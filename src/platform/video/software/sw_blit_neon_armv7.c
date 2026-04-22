#if CRS_VIDEO_DRIVER_SOFTWARE && defined(__ARM_NEON) && !defined(__aarch64__) && !defined(CRS_SW_CANVAS_16BPP)

#include "platform/video/software/sw_blit.h"

#include <arm_neon.h>
#include <stdint.h>
#include <string.h>

// ARMv7 NEON kernels. 4bpp gets a real vector path; 8bpp still falls back.

void sw_blit_indexed8_row_scalar(uint32_t* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate, int count);

// Fast divide-by-255 approximation.
static inline uint8x16_t mul_u8_approx(uint8x16_t x, uint8x16_t y) {
    uint16x8_t lo = vmull_u8(vget_low_u8(x), vget_low_u8(y));
    uint16x8_t hi = vmull_u8(vget_high_u8(x), vget_high_u8(y));
    lo = vaddq_u16(lo, vdupq_n_u16(128));
    hi = vaddq_u16(hi, vdupq_n_u16(128));
    return vcombine_u8(vshrn_n_u16(lo, 8), vshrn_n_u16(hi, 8));
}

// True when every lane is 0xFF.
static inline bool all_opaque(uint8x16_t alpha) {
    uint8x8_t m = vpmin_u8(vget_low_u8(alpha), vget_high_u8(alpha));
    m = vpmin_u8(m, m);
    m = vpmin_u8(m, m);
    m = vpmin_u8(m, m);
    return vget_lane_u8(m, 0) == 0xFF;
}

// Src-over with forced opaque alpha.
static inline uint8x16x4_t blend_src_over_neon(uint8x16_t sb, uint8x16_t sg, uint8x16_t sr, uint8x16_t sa,
                                               uint8x16x4_t dst_v) {
    uint8x16_t db = dst_v.val[0];
    uint8x16_t dg = dst_v.val[1];
    uint8x16_t dr = dst_v.val[2];
    uint8x16_t ia = vsubq_u8(vdupq_n_u8(255), sa);

    uint8x16_t ob = vqaddq_u8(mul_u8_approx(sb, sa), mul_u8_approx(db, ia));
    uint8x16_t og = vqaddq_u8(mul_u8_approx(sg, sa), mul_u8_approx(dg, ia));
    uint8x16_t or_ = vqaddq_u8(mul_u8_approx(sr, sa), mul_u8_approx(dr, ia));

    uint8x16_t zero_mask = vceqq_u8(sa, vdupq_n_u8(0));
    ob = vbslq_u8(zero_mask, db, ob);
    og = vbslq_u8(zero_mask, dg, og);
    or_ = vbslq_u8(zero_mask, dr, or_);

    uint8x16x4_t out;
    out.val[0] = ob;
    out.val[1] = og;
    out.val[2] = or_;
    out.val[3] = vdupq_n_u8(0xFF);
    return out;
}

static inline void modulate_block(uint8x16_t* b, uint8x16_t* g, uint8x16_t* r, uint8x16_t* a, uint32_t modulate) {
    uint8x16_t mb = vdupq_n_u8((uint8_t)(modulate & 0xFF));
    uint8x16_t mg = vdupq_n_u8((uint8_t)((modulate >> 8) & 0xFF));
    uint8x16_t mr = vdupq_n_u8((uint8_t)((modulate >> 16) & 0xFF));
    uint8x16_t ma = vdupq_n_u8((uint8_t)((modulate >> 24) & 0xFF));
    *b = mul_u8_approx(*b, mb);
    *g = mul_u8_approx(*g, mg);
    *r = mul_u8_approx(*r, mr);
    *a = mul_u8_approx(*a, ma);
}

// Scalar helper for the 4bpp edges and tail.
static inline void blend_one(uint32_t* dst, uint32_t src) {
    const uint32_t sa = (src >> 24) & 0xFF;

    if (sa == 0) {
        return;
    }

    if (sa == 0xFF) {
        *dst = src | 0xFF000000u;
        return;
    }

    const uint32_t sr = (src >> 16) & 0xFF;
    const uint32_t sg = (src >> 8) & 0xFF;
    const uint32_t sb = src & 0xFF;
    const uint32_t d = *dst;
    const uint32_t ia = 255 - sa;
    const uint32_t rr = (sr * sa + ((d >> 16) & 0xFF) * ia + 128) >> 8;
    const uint32_t rg = (sg * sa + ((d >> 8) & 0xFF) * ia + 128) >> 8;
    const uint32_t rb = (sb * sa + (d & 0xFF) * ia + 128) >> 8;
    *dst = 0xFF000000u | (rr << 16) | (rg << 8) | rb;
}

static inline uint32_t apply_modulate(uint32_t p, uint32_t modulate) {
    if (modulate == 0xFFFFFFFFu) {
        return p;
    }

    const uint32_t sa = (((p >> 24) & 0xFF) * ((modulate >> 24) & 0xFF) + 128) >> 8;
    const uint32_t sr = (((p >> 16) & 0xFF) * ((modulate >> 16) & 0xFF) + 128) >> 8;
    const uint32_t sg = (((p >> 8) & 0xFF) * ((modulate >> 8) & 0xFF) + 128) >> 8;
    const uint32_t sb = ((p & 0xFF) * (modulate & 0xFF) + 128) >> 8;
    return (sa << 24) | (sr << 16) | (sg << 8) | sb;
}

void sw_fill_solid_row(uint32_t* dst, uint32_t argb, int count) {
    const uint8_t alpha = (uint8_t)((argb >> 24) & 0xFF);

    if (alpha == 0xFF) {
        uint32x4_t v = vdupq_n_u32(argb);
        int i = 0;

        for (; i + 16 <= count; i += 16) {
            vst1q_u32(dst + i + 0, v);
            vst1q_u32(dst + i + 4, v);
            vst1q_u32(dst + i + 8, v);
            vst1q_u32(dst + i + 12, v);
        }

        for (; i < count; i++) {
            dst[i] = argb;
        }

        return;
    }

    if (alpha == 0) {
        return;
    }

    const uint8x16_t sa = vdupq_n_u8(alpha);
    const uint8x16_t sr = vdupq_n_u8((uint8_t)((argb >> 16) & 0xFF));
    const uint8x16_t sg = vdupq_n_u8((uint8_t)((argb >> 8) & 0xFF));
    const uint8x16_t sb = vdupq_n_u8((uint8_t)(argb & 0xFF));

    int i = 0;

    for (; i + 16 <= count; i += 16) {
        uint8x16x4_t dv = vld4q_u8((uint8_t*)(dst + i));
        uint8x16x4_t ov = blend_src_over_neon(sb, sg, sr, sa, dv);
        vst4q_u8((uint8_t*)(dst + i), ov);
    }

    for (; i < count; i++) {
        blend_one(dst + i, argb);
    }
}

void sw_blit_direct_row(uint32_t* dst, const uint32_t* src, uint32_t modulate, int count) {
    int i = 0;
    const bool modulate_identity = (modulate == 0xFFFFFFFFu);

    for (; i + 16 <= count; i += 16) {
        uint8x16x4_t sv = vld4q_u8((const uint8_t*)(src + i));
        uint8x16_t sb = sv.val[0];
        uint8x16_t sg = sv.val[1];
        uint8x16_t sr = sv.val[2];
        uint8x16_t sa = sv.val[3];

        if (!modulate_identity) {
            modulate_block(&sb, &sg, &sr, &sa, modulate);
        }

        if (modulate_identity && all_opaque(sa)) {
            uint8x16x4_t ov = { { sb, sg, sr, vdupq_n_u8(0xFF) } };
            vst4q_u8((uint8_t*)(dst + i), ov);
            continue;
        }

        uint8x16x4_t dv = vld4q_u8((uint8_t*)(dst + i));
        uint8x16x4_t ov = blend_src_over_neon(sb, sg, sr, sa, dv);
        vst4q_u8((uint8_t*)(dst + i), ov);
    }

    for (; i < count; i++) {
        blend_one(dst + i, apply_modulate(src[i], modulate));
    }
}

// 4bpp fast path.
void sw_blit_indexed4_row(uint32_t* dst, const uint8_t* packed, const uint32_t* pal16, uint32_t modulate, int count,
                          int x_lsb) {
    // Split the 16-entry palette into per-channel tables.
    uint8x16x4_t pal_v = vld4q_u8((const uint8_t*)pal16);
    uint8x8x2_t pal_b = { { vget_low_u8(pal_v.val[0]), vget_high_u8(pal_v.val[0]) } };
    uint8x8x2_t pal_g = { { vget_low_u8(pal_v.val[1]), vget_high_u8(pal_v.val[1]) } };
    uint8x8x2_t pal_r = { { vget_low_u8(pal_v.val[2]), vget_high_u8(pal_v.val[2]) } };
    uint8x8x2_t pal_a = { { vget_low_u8(pal_v.val[3]), vget_high_u8(pal_v.val[3]) } };

    const uint8_t* pb = packed;
    int i = 0;

    // Peel one texel so the main loop stays byte-aligned.
    if (x_lsb != 0 && count > 0) {
        const uint8_t nib = (pb[0] >> 4) & 0x0F;
        blend_one(dst + 0, apply_modulate(pal16[nib], modulate));
        i = 1;
        pb += 1;
    }

    // 8 pixels per loop from 4 packed bytes.
    for (; i + 8 <= count; i += 8) {
        uint32_t raw32;
        raw32 = (uint32_t)pb[0] | ((uint32_t)pb[1] << 8) | ((uint32_t)pb[2] << 16) | ((uint32_t)pb[3] << 24);
        uint8x8_t p4 = vreinterpret_u8_u32(vdup_n_u32(raw32));
        uint8x8_t lo_nib = vand_u8(p4, vdup_n_u8(0x0F));
        uint8x8_t hi_nib = vshr_n_u8(p4, 4);
        uint8x8x2_t zz = vzip_u8(lo_nib, hi_nib);
        uint8x8_t idx_d = zz.val[0];

        uint8x8_t b_d = vtbl2_u8(pal_b, idx_d);
        uint8x8_t g_d = vtbl2_u8(pal_g, idx_d);
        uint8x8_t r_d = vtbl2_u8(pal_r, idx_d);
        uint8x8_t a_d = vtbl2_u8(pal_a, idx_d);

        if (modulate != 0xFFFFFFFFu) {
            uint8x8_t mb = vdup_n_u8((uint8_t)(modulate & 0xFF));
            uint8x8_t mg = vdup_n_u8((uint8_t)((modulate >> 8) & 0xFF));
            uint8x8_t mr = vdup_n_u8((uint8_t)((modulate >> 16) & 0xFF));
            uint8x8_t ma = vdup_n_u8((uint8_t)((modulate >> 24) & 0xFF));
            b_d = vshrn_n_u16(vaddq_u16(vmull_u8(b_d, mb), vdupq_n_u16(128)), 8);
            g_d = vshrn_n_u16(vaddq_u16(vmull_u8(g_d, mg), vdupq_n_u16(128)), 8);
            r_d = vshrn_n_u16(vaddq_u16(vmull_u8(r_d, mr), vdupq_n_u16(128)), 8);
            a_d = vshrn_n_u16(vaddq_u16(vmull_u8(a_d, ma), vdupq_n_u16(128)), 8);
        }

        // Blend per lane so fully transparent texels stay cheap.
        uint8_t bb[8], gg[8], rrr[8], aa[8];
        vst1_u8(bb, b_d);
        vst1_u8(gg, g_d);
        vst1_u8(rrr, r_d);
        vst1_u8(aa, a_d);

        for (int k = 0; k < 8; k++) {
            const uint32_t sa = aa[k];

            if (sa == 0) {
                continue;
            }

            if (sa == 0xFF) {
                dst[i + k] = 0xFF000000u | ((uint32_t)rrr[k] << 16) | ((uint32_t)gg[k] << 8) | (uint32_t)bb[k];
                continue;
            }

            const uint32_t d = dst[i + k];
            const uint32_t ia = 255 - sa;
            const uint32_t or_ = (rrr[k] * sa + ((d >> 16) & 0xFF) * ia + 128) >> 8;
            const uint32_t og = (gg[k] * sa + ((d >> 8) & 0xFF) * ia + 128) >> 8;
            const uint32_t ob = (bb[k] * sa + (d & 0xFF) * ia + 128) >> 8;
            dst[i + k] = 0xFF000000u | (or_ << 16) | (og << 8) | ob;
        }

        pb += 4;
    }

    int parity = 0;

    for (; i < count; i++) {
        const uint8_t byte = *pb;
        const uint8_t nib = parity ? ((byte >> 4) & 0x0F) : (byte & 0x0F);
        blend_one(dst + i, apply_modulate(pal16[nib], modulate));

        if (parity) {
            pb += 1;
        }

        parity ^= 1;
    }
}

// 8bpp stays on the scalar path on ARMv7.
void sw_blit_indexed8_row(uint32_t* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate, int count,
                          char kernel) {
    (void)kernel;
    sw_blit_indexed8_row_scalar(dst, idx, pal, modulate, count);
}

// ARMv7 fbdev scaler. Build each unique row once, then memcpy repeats.
#define SW_PRESENT_SCRATCH_MAX 2048

static void present_scale_nearest_v7(uint32_t* dst, int dst_pitch_px, int dst_w, int dst_h, const uint32_t* src,
                                     int src_pitch_px, int src_w, int src_h) {
    if (dst_w <= 0 || dst_h <= 0 || dst_w > SW_PRESENT_SCRATCH_MAX) {
        sw_present_scale_argb_scalar(dst, dst_pitch_px, dst_w, dst_h, src, src_pitch_px, src_w, src_h, true);
        return;
    }

    const uint32_t du_fx = (uint32_t)(((uint64_t)src_w << 16) / (uint64_t)dst_w);
    const uint32_t dv_fx = (uint32_t)(((uint64_t)src_h << 16) / (uint64_t)dst_h);

    uint32_t scratch[SW_PRESENT_SCRATCH_MAX];
    uint32_t v_fx = 0;
    int prev_src_y = -1;

    for (int y = 0; y < dst_h; y++) {
        const int src_y = (int)(v_fx >> 16);

        if (src_y != prev_src_y) {
            const uint32_t* srow = src + (size_t)src_y * (size_t)src_pitch_px;
            uint32_t u_fx = 0;
            int x = 0;

            for (; x + 4 <= dst_w; x += 4) {
                const uint32_t p0 = srow[u_fx >> 16];
                u_fx += du_fx;
                const uint32_t p1 = srow[u_fx >> 16];
                u_fx += du_fx;
                const uint32_t p2 = srow[u_fx >> 16];
                u_fx += du_fx;
                const uint32_t p3 = srow[u_fx >> 16];
                u_fx += du_fx;
                scratch[x + 0] = p0;
                scratch[x + 1] = p1;
                scratch[x + 2] = p2;
                scratch[x + 3] = p3;
            }

            for (; x < dst_w; x++) {
                scratch[x] = srow[u_fx >> 16];
                u_fx += du_fx;
            }

            prev_src_y = src_y;
        }

        uint32_t* drow = dst + (size_t)y * (size_t)dst_pitch_px;
        memcpy(drow, scratch, (size_t)dst_w * sizeof(uint32_t));
        v_fx += dv_fx;
    }
}

void sw_present_scale_argb(uint32_t* dst, int dst_pitch_px, int dst_w, int dst_h, const uint32_t* src, int src_pitch_px,
                           int src_w, int src_h, bool nearest) {
    if (!nearest) {
        sw_present_scale_argb_scalar(dst, dst_pitch_px, dst_w, dst_h, src, src_pitch_px, src_w, src_h, false);
        return;
    }

    const bool integer_scale =
        (dst_w % src_w) == 0 && (dst_h % src_h) == 0 && (dst_w / src_w) == (dst_h / src_h) && (dst_w / src_w) >= 2;

    if (integer_scale) {
        const int scale = dst_w / src_w;

        for (int y = 0; y < dst_h; y++) {
            const uint32_t* srow = src + (y / scale) * src_pitch_px;
            uint32_t* drow = dst + y * dst_pitch_px;

            for (int x = 0; x < src_w; x++) {
                const uint32_t p = srow[x];
                uint32_t* out = drow + x * scale;

                for (int s = 0; s < scale; s++) {
                    out[s] = p;
                }
            }
        }

        return;
    }

    present_scale_nearest_v7(dst, dst_pitch_px, dst_w, dst_h, src, src_pitch_px, src_w, src_h);
}

#endif // ARMv7 NEON
