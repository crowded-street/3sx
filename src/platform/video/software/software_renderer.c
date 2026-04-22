#if CRS_VIDEO_DRIVER_SOFTWARE

#include "platform/video/software/software_renderer.h"

#include "common.h"
#include "platform/video/software/sw_blit.h"
#include "platform/video/software/sw_convert.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/common/plcommon.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include "stb/stb_ds.h"

#include <libgraph.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Software renderer. Collect quads, sort, then rasterise.

#define QUADS_MAX 512
#define CANVAS_W 384
#define CANVAS_H 224
#define CANVAS_PITCH CANVAS_W

#ifndef CRS_SW_INDEXED8_KERNEL_DEFAULT
#define CRS_SW_INDEXED8_KERNEL_DEFAULT 'b'
#endif

// Keep this on for the active software targets.
#ifndef CRS_SW_SCALED_CKEY
#define CRS_SW_SCALED_CKEY 1
#endif

// Optional fullscreen-solid dedupe.
#ifndef CRS_SW_DEDUP_FS_SOLIDS
#define CRS_SW_DEDUP_FS_SOLIDS 0
#endif

typedef uint8_t SWPaletteType;

enum {
    SW_PALETTE_NONE = 0,
    SW_PALETTE_4,
    SW_PALETTE_8,
};

typedef struct SWVec2 {
    float x;
    float y;
} SWVec2;

typedef struct SWTexCoord {
    float s;
    float t;
} SWTexCoord;

typedef struct SWTexture {
    // Borrowed source pointer.
    const void* pixels;
    // Converted copy for direct-colour textures.
    uint32_t* converted;
    uint16_t width;
    uint16_t height;
    // GS textures are power-of-two, so wrap is just `& mask`.
    uint16_t width_mask;
    uint16_t height_mask;
    SWPaletteType palette_type;
    // Highest index seen in a PSMT8 texture.
    uint8_t max_index;
} SWTexture;

typedef struct SWPalette {
    uint32_t colors[256];
#if defined(CRS_SW_CANVAS_16BPP)
    // Pre-converted RGB565 mirror.
    uint16_t colors565[256];
    // Cache version for the colour-key scratch.
    uint32_t version;
#endif
    uint16_t count;
    // Entry 0 transparent, entries 1..N opaque.
    uint8_t ckey_extent;
} SWPalette;

typedef struct SWTextureSpec {
    int16_t texture_index; // -1 when unset
    int16_t palette_index; // -1 when unset
} SWTextureSpec;

typedef struct SWQuad {
    SWVec2 positions[4];
    SWTexCoord tex_coords[4];
    float z;
    uint32_t index;
    uint32_t modulate;
    SWTextureSpec texture_spec;
} SWQuad;

typedef struct RBCtx {
    int band_y0;
    int band_y1;
} RBCtx;

static SWCanvasPixel* canvas = NULL;
static SWQuad* quads = NULL;

static SWTexture textures[FL_TEXTURE_MAX] = { 0 };
static SWPalette palettes[FL_PALETTE_MAX] = { 0 };

static SWTextureSpec latest_texture_spec = { -1, -1 };

static bool renderer_nearest_filter = true;
static int renderer_scale = 1;

#if defined(CRS_SW_CANVAS_16BPP)
// Cached u32 mirror of the RGB565 palette for the colour-key path.
static uint32_t ckey_pal_u32[256];
static const SWPalette* ckey_pal_cached = NULL;
static uint32_t ckey_pal_cached_version = 0;

static inline const uint32_t* ckey_pal_get(const SWPalette* pal) {
    if (ckey_pal_cached != pal || ckey_pal_cached_version != pal->version) {
        uint32_t* s = ckey_pal_u32;
        const uint16_t* src = pal->colors565;
        for (int i = 0; i < 256; i++) {
            s[i] = (uint32_t)src[i];
        }
        ckey_pal_cached = pal;
        ckey_pal_cached_version = pal->version;
    }
    return ckey_pal_u32;
}
#endif

// Preferred 8bpp kernel.
static char indexed8_kernel = CRS_SW_INDEXED8_KERNEL_DEFAULT;

static void quad_infer_positions(SWQuad* quad) {
    quad->positions[1].x = quad->positions[3].x;
    quad->positions[1].y = quad->positions[0].y;
    quad->positions[2].x = quad->positions[0].x;
    quad->positions[2].y = quad->positions[3].y;
}

static void quad_infer_tex_coords(SWQuad* quad) {
    quad->tex_coords[1].s = quad->tex_coords[3].s;
    quad->tex_coords[1].t = quad->tex_coords[0].t;
    quad->tex_coords[2].s = quad->tex_coords[0].s;
    quad->tex_coords[2].t = quad->tex_coords[3].t;
}

// Sort an index array instead of moving full quads around.
static inline uint32_t float_to_sortable_u32(float f) {
    uint32_t b;
    memcpy(&b, &f, sizeof(b));
    return (b & 0x80000000u) ? ~b : (b | 0x80000000u);
}

static void insertion_sort_idx(uint16_t* idx, int lo, int hi, const uint64_t* keys) {
    for (int i = lo + 1; i <= hi; i++) {
        const uint16_t x = idx[i];
        const uint64_t xk = keys[x];
        int j = i - 1;
        while (j >= lo && keys[idx[j]] > xk) {
            idx[j + 1] = idx[j];
            j--;
        }
        idx[j + 1] = x;
    }
}

static void quick_sort_idx(uint16_t* idx, int lo, int hi, const uint64_t* keys) {
    while (hi - lo > 15) {
        const int mid = lo + ((hi - lo) >> 1);
        if (keys[idx[lo]] > keys[idx[mid]]) {
            uint16_t t = idx[lo];
            idx[lo] = idx[mid];
            idx[mid] = t;
        }
        if (keys[idx[lo]] > keys[idx[hi]]) {
            uint16_t t = idx[lo];
            idx[lo] = idx[hi];
            idx[hi] = t;
        }
        if (keys[idx[mid]] > keys[idx[hi]]) {
            uint16_t t = idx[mid];
            idx[mid] = idx[hi];
            idx[hi] = t;
        }
        const uint16_t pivot_i = idx[mid];
        const uint64_t pivot_k = keys[pivot_i];
        idx[mid] = idx[hi - 1];
        idx[hi - 1] = pivot_i;
        int i = lo, j = hi - 1;
        for (;;) {
            while (keys[idx[++i]] < pivot_k) { /* */
            }
            while (keys[idx[--j]] > pivot_k) { /* */
            }
            if (i >= j)
                break;
            uint16_t t = idx[i];
            idx[i] = idx[j];
            idx[j] = t;
        }
        {
            uint16_t t = idx[i];
            idx[i] = idx[hi - 1];
            idx[hi - 1] = t;
        }
        if (i - lo < hi - i) {
            quick_sort_idx(idx, lo, i - 1, keys);
            lo = i + 1;
        } else {
            quick_sort_idx(idx, i + 1, hi, keys);
            hi = i - 1;
        }
    }
    insertion_sort_idx(idx, lo, hi, keys);
}

static uint16_t* sort_idx = NULL;
static SWQuad* sort_scratch = NULL;
static uint64_t* sort_keys64 = NULL;
static int sort_cap = 0;

static void sort_quads_fast() {
    const int n = (int)arrlen(quads);
    if (n <= 1)
        return;
    if (n > sort_cap) {
        int new_cap = sort_cap ? sort_cap : 256;
        while (new_cap < n)
            new_cap *= 2;
        sort_idx = (uint16_t*)realloc(sort_idx, new_cap * sizeof(uint16_t));
        sort_scratch = (SWQuad*)realloc(sort_scratch, new_cap * sizeof(SWQuad));
        sort_keys64 = (uint64_t*)realloc(sort_keys64, new_cap * sizeof(uint64_t));
        sort_cap = new_cap;
    }
    for (int i = 0; i < n; i++) {
        const uint32_t zk = float_to_sortable_u32(quads[i].z);
        sort_keys64[i] = ((uint64_t)zk << 32) | (uint32_t)(~quads[i].index);
        sort_idx[i] = (uint16_t)i;
    }
    quick_sort_idx(sort_idx, 0, n - 1, sort_keys64);
    for (int i = 0; i < n; i++)
        sort_scratch[i] = quads[sort_idx[i]];
    memcpy(quads, sort_scratch, n * sizeof(SWQuad));
}

#if CRS_SW_DEDUP_FS_SOLIDS
// Reuse `index` as a dead flag after sorting.
#define SW_QUAD_DEAD UINT32_MAX

static bool quad_is_opaque_fullscreen_solid(const SWQuad* q) {
    if (q->texture_spec.texture_index >= 0)
        return false;
    if ((q->modulate >> 24) != 0xFFu)
        return false;
    const float fx0 = q->positions[0].x;
    const float fy0 = q->positions[0].y;
    const float fx1 = q->positions[3].x;
    const float fy1 = q->positions[3].y;
    const float xmin = fx0 < fx1 ? fx0 : fx1;
    const float xmax = fx0 > fx1 ? fx0 : fx1;
    const float ymin = fy0 < fy1 ? fy0 : fy1;
    const float ymax = fy0 > fy1 ? fy0 : fy1;
    // Allow slightly oversized fullscreen fades.
    return xmin <= 0.0f && ymin <= 0.0f && xmax >= (float)CANVAS_W && ymax >= (float)CANVAS_H;
}

// Drop repeated fullscreen solids after sort.
static int dedup_fs_solids() {
    const int n = (int)arrlen(quads);
    if (n < 2)
        return 0;
    int hits = 0;
    for (int i = 0; i + 1 < n; i++) {
        const SWQuad* cur = &quads[i];
        const SWQuad* nxt = &quads[i + 1];
        if (cur->modulate != nxt->modulate)
            continue;
        if (!quad_is_opaque_fullscreen_solid(cur))
            continue;
        if (!quad_is_opaque_fullscreen_solid(nxt))
            continue;
        quads[i].index = SW_QUAD_DEAD;
        hits++;
    }
    return hits;
}
#endif

static int clamp_i(int v, int lo, int hi) {
    if (v < lo) {
        return lo;
    }

    if (v > hi) {
        return hi;
    }

    return v;
}

static const void* texture_source_pixels(const FLTexture* fl) {
    if (fl->wkVram != NULL) {
        return fl->wkVram;
    }

    if (fl->mem_handle != 0) {
        return flPS2GetSystemBuffAdrs(fl->mem_handle);
    }

    return NULL;
}

typedef struct QuadExtents {
    // Clipped rect.
    int x0, y0, x1, y1;
    // Unclipped size.
    int full_w, full_h;
    // Leading clip.
    int left_clip, top_clip;
    // Original float bounds.
    float fx0, fy0, fx1, fy1;
} QuadExtents;

// Round half away from zero.
static int round_nearest(float f) {
    return (int)((f >= 0.0f) ? (f + 0.5f) : (f - 0.5f));
}

static float absf_local(float f) {
    return (f < 0.0f) ? -f : f;
}

static bool nearly_equalf(float a, float b, float eps) {
    return absf_local(a - b) <= eps;
}

static float clamp_uv_hi_local(float uv_texel, int tex_extent) {
    const float tex_extent_f = (float)tex_extent;

    if (uv_texel > tex_extent_f) {
        return uv_texel;
    }

    // Nudge exact edge UVs inward so UI quads hit the last texel instead of wrapping.
    if (uv_texel >= tex_extent_f) {
        return tex_extent_f - (1.0f / 65536.0f);
    }

    return uv_texel;
}

static float min3f(float a, float b, float c) {
    float m = (a < b) ? a : b;
    return (c < m) ? c : m;
}

static float max3f(float a, float b, float c) {
    float m = (a > b) ? a : b;
    return (c > m) ? c : m;
}

static int floor_to_int(float f) {
    const int i = (int)f;
    return ((float)i > f) ? (i - 1) : i;
}

static int ceil_to_int(float f) {
    const int i = (int)f;
    return ((float)i < f) ? (i + 1) : i;
}

static float edge_functionf(float ax, float ay, float bx, float by, float px, float py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

static bool is_top_left_edge(float ax, float ay, float bx, float by) {
    const float dy = by - ay;
    const float dx = bx - ax;
    return (dy < 0.0f) || (dy == 0.0f && dx > 0.0f);
}

static uint32_t sw_modulate_pixel_local(uint32_t src, uint32_t modulate) {
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

static SWCanvasPixel sw_blend_argb_onto_canvas_local(uint32_t src, SWCanvasPixel dst_px) {
    const uint32_t sa = (src >> 24) & 0xFF;

    if (sa == 0xFF) {
        return sw_argb_to_canvas(src);
    }

    if (sa == 0x00) {
        return dst_px;
    }

    const uint32_t ia = 255 - sa;
    const uint32_t sr = (src >> 16) & 0xFF;
    const uint32_t sg = (src >> 8) & 0xFF;
    const uint32_t sb = src & 0xFF;
    const uint32_t dst_argb = sw_canvas_to_argb(dst_px);
    const uint32_t dr = (dst_argb >> 16) & 0xFF;
    const uint32_t dg = (dst_argb >> 8) & 0xFF;
    const uint32_t db = dst_argb & 0xFF;

    const uint32_t rr = (sr * sa + dr * ia + 128) >> 8;
    const uint32_t rg = (sg * sa + dg * ia + 128) >> 8;
    const uint32_t rb = (sb * sa + db * ia + 128) >> 8;
    return sw_argb_to_canvas(0xFF000000u | (rr << 16) | (rg << 8) | rb);
}

// Covered bounds for the quad.
static QuadExtents quad_extents(const SWQuad* q, int band_y0, int band_y1) {
    float fx0 = q->positions[0].x;
    float fy0 = q->positions[0].y;
    float fx1 = q->positions[0].x;
    float fy1 = q->positions[0].y;

    for (int i = 1; i < 4; i++) {
        if (q->positions[i].x < fx0)
            fx0 = q->positions[i].x;
        if (q->positions[i].x > fx1)
            fx1 = q->positions[i].x;
        if (q->positions[i].y < fy0)
            fy0 = q->positions[i].y;
        if (q->positions[i].y > fy1)
            fy1 = q->positions[i].y;
    }

    const int ux0 = round_nearest(fx0);
    const int uy0 = round_nearest(fy0);
    const int ux1 = round_nearest(fx1);
    const int uy1 = round_nearest(fy1);

    QuadExtents e;
    e.full_w = ux1 - ux0;
    e.full_h = uy1 - uy0;
    e.x0 = clamp_i(ux0, 0, CANVAS_W);
    e.y0 = clamp_i(uy0, band_y0, band_y1);
    e.x1 = clamp_i(ux1, 0, CANVAS_W);
    e.y1 = clamp_i(uy1, band_y0, band_y1);
    e.left_clip = e.x0 - ux0;
    e.top_clip = e.y0 - uy0;
    e.fx0 = fx0;
    e.fy0 = fy0;
    e.fx1 = fx1;
    e.fy1 = fy1;
    return e;
}

static void rasterize_solid(const SWQuad* q, RBCtx* ctx) {
    const QuadExtents e = quad_extents(q, ctx->band_y0, ctx->band_y1);

    if (e.x1 <= e.x0 || e.y1 <= e.y0) {
        return;
    }

    for (int y = e.y0; y < e.y1; y++) {
        sw_fill_solid_row(canvas + y * CANVAS_PITCH + e.x0, q->modulate, e.x1 - e.x0);
    }
}

// Slow path for the sheared background quads used by some stages.
static bool rasterize_parallelogram(const SWQuad* q, RBCtx* ctx) {
    const float edge_eps = 0.5f;

    // Sort into TL/TR/BL/BR so winding does not matter.
    int idx[4] = { 0, 1, 2, 3 };
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            const float yi = q->positions[idx[i]].y, yj = q->positions[idx[j]].y;
            const float xi = q->positions[idx[i]].x, xj = q->positions[idx[j]].x;

            if (yj < yi || (yj == yi && xj < xi)) {
                const int tmp = idx[i];
                idx[i] = idx[j];
                idx[j] = tmp;
            }
        }
    }

    const int tl = idx[0], tr = idx[1], bl = idx[2], br = idx[3];
    const float plx = q->positions[tl].x, ply = q->positions[tl].y;
    const float prx = q->positions[tr].x, pry = q->positions[tr].y;
    const float blx = q->positions[bl].x, bly = q->positions[bl].y;
    const float brx = q->positions[br].x, bry = q->positions[br].y;

    // Only handle the horizontal-shear case we actually use.
    if (!nearly_equalf(pry, ply, edge_eps) || !nearly_equalf(bry, bly, edge_eps) || bly <= ply) {
        return false;
    }

    const float e1x = prx - plx;
    const float e2x = blx - plx;
    const float e2y = bly - ply;
    const float expected_brx = prx + e2x;
    const float expected_bry = pry + e2y;

    if (e1x <= edge_eps) {
        return false;
    }

    // Reject the skinny logo wedges and other non-parallelograms.
    if (!nearly_equalf(brx, expected_brx, edge_eps) || !nearly_equalf(bry, expected_bry, edge_eps)) {
        return false;
    }

    const SWTexture* tex = &textures[q->texture_spec.texture_index];

    if (tex->pixels == NULL && tex->converted == NULL) {
        return false;
    }

    if (tex->palette_type == SW_PALETTE_NONE && tex->converted == NULL) {
        return false;
    }

    const SWPalette* pal = NULL;

    if (q->texture_spec.palette_index >= 0) {
        pal = &palettes[q->texture_spec.palette_index];
    }

    const int tex_w = tex->width;
    const int tex_h = tex->height;
    const int uy_mask = tex->height_mask;
    const uint32_t u_mask_fx = ((uint32_t)tex_w << 16) - 1u;

    const float tls = q->tex_coords[tl].s * (float)tex_w;
    const float tlt = q->tex_coords[tl].t * (float)tex_h;
    const float trs = q->tex_coords[tr].s * (float)tex_w;
    const float bls = q->tex_coords[bl].s * (float)tex_w;
    const float blt = q->tex_coords[bl].t * (float)tex_h;

    const float du_tex = (trs - tls) / e1x;
    const uint32_t du_fx = (uint32_t)(du_tex * 65536.0f);

    if (du_tex <= 0.0f) {
        return false;
    }

    int y0 = round_nearest(ply);
    int y1 = round_nearest(bly);

    if (y0 < ctx->band_y0) {
        y0 = ctx->band_y0;
    }

    if (y1 > ctx->band_y1) {
        y1 = ctx->band_y1;
    }

    if (y0 >= y1) {
        return false;
    }

    for (int y = y0; y < y1; y++) {
        const float v_frac = ((float)y + 0.5f - ply) / e2y;
        const float left_edge_x = plx + v_frac * e2x;
        const float right_edge_x = left_edge_x + e1x;
        const float v_tex = tlt + v_frac * (blt - tlt);
        const float u_left_tex = tls + v_frac * (bls - tls);

        int x0 = round_nearest(left_edge_x);
        int x1 = round_nearest(right_edge_x);

        if (x0 < 0) {
            x0 = 0;
        }

        if (x1 > CANVAS_W) {
            x1 = CANVAS_W;
        }

        if (x0 >= x1) {
            continue;
        }

        const int count = x1 - x0;
        const float left_clip = (float)x0 + 0.5f - left_edge_x;
        const float u_start_tex = u_left_tex + left_clip * du_tex;
        const uint32_t u_start_fx = (uint32_t)(u_start_tex * 65536.0f);
        const uint32_t u0 = u_start_fx & u_mask_fx;
        const int vy = ((int)v_tex) & uy_mask;

        SWCanvasPixel* drow = canvas + y * CANVAS_PITCH + x0;

        if (tex->palette_type == SW_PALETTE_8) {
            const uint8_t* idx_row = (const uint8_t*)tex->pixels + vy * tex_w;
            uint32_t u_cur = u0;
            int remaining = count;
            SWCanvasPixel* d = drow;

            while (remaining > 0) {
                int chunk = (du_fx == 0) ? remaining : (int)((u_mask_fx - u_cur) / du_fx) + 1;

                if (chunk > remaining) {
                    chunk = remaining;
                }

                sw_blit_scaled_indexed8_row(d, idx_row, pal->colors, u_cur, du_fx, q->modulate, chunk);
                d += chunk;
                remaining -= chunk;
                u_cur = (u_cur + (uint32_t)chunk * du_fx) & u_mask_fx;
            }
        } else if (tex->palette_type == SW_PALETTE_4) {
            const uint8_t* packed_base = (const uint8_t*)tex->pixels + vy * (tex_w / 2);
            uint32_t u_cur = u0;
            int remaining = count;
            SWCanvasPixel* d = drow;

            while (remaining > 0) {
                int chunk = (du_fx == 0) ? remaining : (int)((u_mask_fx - u_cur) / du_fx) + 1;

                if (chunk > remaining) {
                    chunk = remaining;
                }

                sw_blit_scaled_indexed4_row(d, packed_base, pal->colors, u_cur, du_fx, q->modulate, chunk);
                d += chunk;
                remaining -= chunk;
                u_cur = (u_cur + (uint32_t)chunk * du_fx) & u_mask_fx;
            }
        } else {
            const uint32_t* src_row = tex->converted + vy * tex_w;
            uint32_t u_cur = u0;
            int remaining = count;
            SWCanvasPixel* d = drow;

            while (remaining > 0) {
                int chunk = (du_fx == 0) ? remaining : (int)((u_mask_fx - u_cur) / du_fx) + 1;

                if (chunk > remaining) {
                    chunk = remaining;
                }

                sw_blit_scaled_direct_row(d, src_row, u_cur, du_fx, q->modulate, chunk);
                d += chunk;
                remaining -= chunk;
                u_cur = (u_cur + (uint32_t)chunk * du_fx) & u_mask_fx;
            }
        }
    }

    return true;
}

static uint32_t sample_texture_argb(const SWTexture* tex, const SWPalette* pal, float s, float t) {
    int tx = (int)(s * (float)tex->width);
    int ty = (int)(t * (float)tex->height);

    // Clamp exact edge UVs for the triangle fallback.
    tx = clamp_i(tx, 0, tex->width - 1);
    ty = clamp_i(ty, 0, tex->height - 1);

    if (tex->palette_type == SW_PALETTE_8) {
        const uint8_t* idx = (const uint8_t*)tex->pixels + ty * tex->width;
        return pal->colors[idx[tx]];
    }

    if (tex->palette_type == SW_PALETTE_4) {
        const uint8_t* packed = (const uint8_t*)tex->pixels + ty * (tex->width / 2);
        const uint8_t byte = packed[tx >> 1];
        const uint8_t idx = (tx & 1) ? (byte >> 4) : (byte & 0x0F);
        return pal->colors[idx];
    }

    return tex->converted[ty * tex->width + tx];
}

static void rasterize_textured_triangle(
    const SWQuad* q, RBCtx* ctx, int i0, int i1, int i2, const SWTexture* tex, const SWPalette* pal
) {
    float x0 = q->positions[i0].x;
    float y0 = q->positions[i0].y;
    float x1 = q->positions[i1].x;
    float y1 = q->positions[i1].y;
    float x2 = q->positions[i2].x;
    float y2 = q->positions[i2].y;
    float s0 = q->tex_coords[i0].s;
    float t0 = q->tex_coords[i0].t;
    float s1 = q->tex_coords[i1].s;
    float t1 = q->tex_coords[i1].t;
    float s2 = q->tex_coords[i2].s;
    float t2 = q->tex_coords[i2].t;

    float area = edge_functionf(x0, y0, x1, y1, x2, y2);

    if (area == 0.0f) {
        return;
    }

    if (area < 0.0f) {
        float tmp;

        tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
        tmp = s1;
        s1 = s2;
        s2 = tmp;
        tmp = t1;
        t1 = t2;
        t2 = tmp;
        area = -area;
    }

    const float min_xf = min3f(x0, x1, x2);
    const float max_xf = max3f(x0, x1, x2);
    const float min_yf = min3f(y0, y1, y2);
    const float max_yf = max3f(y0, y1, y2);

    const int min_x = clamp_i(floor_to_int(min_xf), 0, CANVAS_W);
    const int max_x = clamp_i(ceil_to_int(max_xf), 0, CANVAS_W);
    const int min_y = clamp_i(floor_to_int(min_yf), ctx->band_y0, ctx->band_y1);
    const int max_y = clamp_i(ceil_to_int(max_yf), ctx->band_y0, ctx->band_y1);

    if (min_x >= max_x || min_y >= max_y) {
        return;
    }

    const float inv_area = 1.0f / area;
    const bool tl01 = is_top_left_edge(x0, y0, x1, y1);
    const bool tl12 = is_top_left_edge(x1, y1, x2, y2);
    const bool tl20 = is_top_left_edge(x2, y2, x0, y0);

    for (int y = min_y; y < max_y; y++) {
        SWCanvasPixel* drow = canvas + y * CANVAS_PITCH;

        for (int x = min_x; x < max_x; x++) {
            const float px = (float)x + 0.5f;
            const float py = (float)y + 0.5f;
            const float e01 = edge_functionf(x0, y0, x1, y1, px, py);
            const float e12 = edge_functionf(x1, y1, x2, y2, px, py);
            const float e20 = edge_functionf(x2, y2, x0, y0, px, py);

            if (e01 < 0.0f || (e01 == 0.0f && !tl01) || e12 < 0.0f || (e12 == 0.0f && !tl12) || e20 < 0.0f ||
                (e20 == 0.0f && !tl20)) {
                continue;
            }

            const float w0 = e12 * inv_area;
            const float w1 = e20 * inv_area;
            const float w2 = e01 * inv_area;

            const float s = w0 * s0 + w1 * s1 + w2 * s2;
            const float t = w0 * t0 + w1 * t1 + w2 * t2;
            const uint32_t src = sw_modulate_pixel_local(sample_texture_argb(tex, pal, s, t), q->modulate);
            drow[x] = sw_blend_argb_onto_canvas_local(src, drow[x]);
        }
    }
}

static void rasterize_triangles(const SWQuad* q, RBCtx* ctx) {
    const SWTexture* tex = &textures[q->texture_spec.texture_index];

    if (tex->pixels == NULL && tex->converted == NULL) {
        return;
    }

    if (tex->palette_type == SW_PALETTE_NONE && tex->converted == NULL) {
        return;
    }

    const SWPalette* pal = NULL;

    if (q->texture_spec.palette_index >= 0) {
        pal = &palettes[q->texture_spec.palette_index];
    }

    rasterize_textured_triangle(q, ctx, 0, 1, 2, tex, pal);
    rasterize_textured_triangle(q, ctx, 2, 1, 3, tex, pal);
}

static void rasterize_textured(const SWQuad* q, RBCtx* ctx) {
    // Treat near-AABB quads as AABB so UI/logo quads stay on the fast path.
    {
        const float edge_eps = 0.5f;
        const float p0x = q->positions[0].x, p0y = q->positions[0].y;
        const float p1x = q->positions[1].x, p1y = q->positions[1].y;
        const float p2x = q->positions[2].x, p2y = q->positions[2].y;
        const float p3x = q->positions[3].x, p3y = q->positions[3].y;
        const bool row_major_aabb = nearly_equalf(p0y, p1y, edge_eps) && nearly_equalf(p2y, p3y, edge_eps) &&
                                    nearly_equalf(p0x, p2x, edge_eps) && nearly_equalf(p1x, p3x, edge_eps);
        const bool col_major_aabb = nearly_equalf(p0x, p1x, edge_eps) && nearly_equalf(p2x, p3x, edge_eps) &&
                                    nearly_equalf(p0y, p2y, edge_eps) && nearly_equalf(p1y, p3y, edge_eps);

        if (!row_major_aabb && !col_major_aabb) {
            if (rasterize_parallelogram(q, ctx)) {
                return;
            }

            rasterize_triangles(q, ctx);
            return;
        }
    }

    const QuadExtents e = quad_extents(q, ctx->band_y0, ctx->band_y1);

    if (e.x1 <= e.x0 || e.y1 <= e.y0) {
        return;
    }

    const int dst_w = e.x1 - e.x0;
    const int dst_h = e.y1 - e.y0;

    const SWTexture* tex = &textures[q->texture_spec.texture_index];

    if (tex->pixels == NULL && tex->converted == NULL) {
        return;
    }

    // PSMCT16 draws need the converted buffer.
    if (tex->palette_type == SW_PALETTE_NONE && tex->converted == NULL) {
        return;
    }

    // Sample at pixel centres.
    const float s0 = q->tex_coords[0].s * tex->width;
    const float t0 = q->tex_coords[0].t * tex->height;
    const float s3 = q->tex_coords[3].s * tex->width;
    const float t3 = q->tex_coords[3].t * tex->height;

    // Flipped sprites have reversed UVs.
    const bool flip_x = s3 < s0;
    const bool flip_y = t3 < t0;

    const float u_lo = flip_x ? s3 : s0;
    float u_hi = flip_x ? s0 : s3;
    const float v_lo = flip_y ? t3 : t0;
    float v_hi = flip_y ? t0 : t3;

    u_hi = clamp_uv_hi_local(u_hi, tex->width);
    v_hi = clamp_uv_hi_local(v_hi, tex->height);

    // Use float bounds here so neighbouring tiles keep the same texel rate.
    const float flt_w = e.fx1 - e.fx0;
    const float flt_h = e.fy1 - e.fy0;
    const float flt_du = flt_w > 0.0f ? (u_hi - u_lo) / flt_w : 0.0f;
    const float flt_dv = flt_h > 0.0f ? (v_hi - v_lo) / flt_h : 0.0f;

    const uint32_t du_fx = (uint32_t)(flt_du * 65536.0f);
    const uint32_t dv_fx = (uint32_t)(flt_dv * 65536.0f);

    const bool unscaled = (du_fx == 0x10000u && dv_fx == 0x10000u);
    const SWPalette* pal = NULL;

    if (q->texture_spec.palette_index >= 0) {
        pal = &palettes[q->texture_spec.palette_index];
    }

    const float flt_u_lead = ((float)e.x0 + 0.5f - e.fx0) * flt_du;
    const float flt_v_lead = ((float)e.y0 + 0.5f - e.fy0) * flt_dv;

    // Reverse blits read from the high end, so keep a separate start value.
    const uint32_t u_start_lo_fx = (uint32_t)((u_lo + flt_u_lead) * 65536.0f);
    const uint32_t u_start_hi_fx = (uint32_t)((u_hi - flt_u_lead + 1.0f) * 65536.0f);

    uint32_t v_start_fx;

    if (flip_y) {
        v_start_fx = (uint32_t)((v_hi - flt_v_lead) * 65536.0f);
    } else {
        v_start_fx = (uint32_t)((v_lo + flt_v_lead) * 65536.0f);
    }

    uint32_t v_fx = v_start_fx;

    // GS textures wrap with simple masks.
    const int tex_w = tex->width;
    const int ux_mask = tex->width_mask;
    const int uy_mask = tex->height_mask;

    const int src_x_start_fwd = ((int)(u_start_lo_fx >> 16)) & ux_mask;
    const int src_x_start_rev = ((int)((u_start_hi_fx - 0x10000u) >> 16)) & ux_mask;
    const uint32_t u_mask_fx = ((uint32_t)tex_w << 16) - 1u;
    const uint32_t u_scaled_fwd = u_start_lo_fx & u_mask_fx;
    const uint32_t u_scaled_rev = (u_start_hi_fx - du_fx) & u_mask_fx;

    // Fast path is valid when the palette covers every live index.
    const bool ckey_ok = (pal != NULL) && ((tex->palette_type == SW_PALETTE_8 && pal->ckey_extent >= tex->max_index) ||
                                           (tex->palette_type == SW_PALETTE_4 && pal->ckey_extent >= 15));

#if defined(CRS_SW_CANVAS_16BPP)
    const bool ckey_ready = (ckey_ok && q->modulate == 0xFFFFFFFFu);
    const uint32_t* ckey_pal_u32_hoisted = ckey_ready ? ckey_pal_get(pal) : NULL;
#endif

    for (int y = 0; y < dst_h; y++) {
        const int vy = ((int)(v_fx >> 16)) & uy_mask;
        SWCanvasPixel* drow = canvas + (e.y0 + y) * CANVAS_PITCH + e.x0;

        if (tex->palette_type == SW_PALETTE_8) {
            const uint8_t* idx_row = (const uint8_t*)tex->pixels + vy * tex_w;

            if (flip_x) {
                if (unscaled) {
                    int remaining = dst_w;
                    int src = src_x_start_rev;
                    SWCanvasPixel* d = drow;

                    while (remaining > 0) {
                        int chunk = src + 1;

                        if (chunk > remaining) {
                            chunk = remaining;
                        }

#if defined(CRS_SW_CANVAS_16BPP)
                        if (q->modulate == 0xFFFFFFFFu) {
                            if (ckey_ok) {
                                sw_blit_indexed8_row_rev_ckey_565(d, idx_row + src, ckey_pal_u32_hoisted, chunk);
                            } else {
                                sw_blit_indexed8_row_rev_565(d, idx_row + src, pal->colors, pal->colors565, chunk);
                            }
                        } else {
                            sw_blit_indexed8_row_rev(d, idx_row + src, pal->colors, q->modulate, chunk);
                        }
#else
                        sw_blit_indexed8_row_rev(d, idx_row + src, pal->colors, q->modulate, chunk);
#endif
                        d += chunk;
                        remaining -= chunk;
                        src = tex_w - 1;
                    }
                } else {
                    // Split before wrap so the row kernel never crosses an atlas edge.
                    uint32_t u_cur = u_scaled_rev;
                    int remaining = dst_w;
                    SWCanvasPixel* d = drow;

                    while (remaining > 0) {
                        int chunk = (du_fx == 0) ? remaining : (int)(u_cur / du_fx) + 1;

                        if (chunk > remaining) {
                            chunk = remaining;
                        }

#if defined(CRS_SW_CANVAS_16BPP) && CRS_SW_SCALED_CKEY
                        if (ckey_ok && q->modulate == 0xFFFFFFFFu) {
                            sw_blit_scaled_indexed8_row_rev_ckey_565(d, idx_row, pal->colors565, u_cur, du_fx, chunk);
                        } else {
                            sw_blit_scaled_indexed8_row_rev(d, idx_row, pal->colors, u_cur, du_fx, q->modulate, chunk);
                        }
#else
                        sw_blit_scaled_indexed8_row_rev(d, idx_row, pal->colors, u_cur, du_fx, q->modulate, chunk);
#endif
                        d += chunk;
                        remaining -= chunk;
                        u_cur = (u_cur - (uint32_t)chunk * du_fx) & u_mask_fx;
                    }
                }
            } else if (unscaled) {
                int remaining = dst_w;
                int src = src_x_start_fwd;
                SWCanvasPixel* d = drow;

                while (remaining > 0) {
                    int chunk = tex_w - src;

                    if (chunk > remaining) {
                        chunk = remaining;
                    }

#if defined(CRS_SW_CANVAS_16BPP)
                    if (q->modulate == 0xFFFFFFFFu) {
                        if (ckey_ok) {
                            sw_blit_indexed8_row_ckey_565(d, idx_row + src, ckey_pal_u32_hoisted, chunk);
                        } else {
                            sw_blit_indexed8_row_565(d, idx_row + src, pal->colors, pal->colors565, chunk);
                        }
                    } else {
                        sw_blit_indexed8_row(d, idx_row + src, pal->colors, q->modulate, chunk, indexed8_kernel);
                    }
#else
                    sw_blit_indexed8_row(d, idx_row + src, pal->colors, q->modulate, chunk, indexed8_kernel);
#endif
                    d += chunk;
                    remaining -= chunk;
                    src = 0;
                }
            } else {
                // Split before wrap so the row kernel never crosses an atlas edge.
                uint32_t u_cur = u_scaled_fwd;
                int remaining = dst_w;
                SWCanvasPixel* d = drow;

                while (remaining > 0) {
                    int chunk = (du_fx == 0) ? remaining : (int)((u_mask_fx - u_cur) / du_fx) + 1;

                    if (chunk > remaining) {
                        chunk = remaining;
                    }

#if defined(CRS_SW_CANVAS_16BPP) && CRS_SW_SCALED_CKEY
                    if (ckey_ok && q->modulate == 0xFFFFFFFFu) {
                        sw_blit_scaled_indexed8_row_ckey_565(d, idx_row, pal->colors565, u_cur, du_fx, chunk);
                    } else {
                        sw_blit_scaled_indexed8_row(d, idx_row, pal->colors, u_cur, du_fx, q->modulate, chunk);
                    }
#else
                    sw_blit_scaled_indexed8_row(d, idx_row, pal->colors, u_cur, du_fx, q->modulate, chunk);
#endif
                    d += chunk;
                    remaining -= chunk;
                    u_cur = (u_cur + (uint32_t)chunk * du_fx) & u_mask_fx;
                }
            }
        } else if (tex->palette_type == SW_PALETTE_4) {
            const uint8_t* packed_base = (const uint8_t*)tex->pixels + vy * (tex_w / 2);

            if (flip_x) {
                if (unscaled) {
                    int remaining = dst_w;
                    int src = src_x_start_rev;
                    SWCanvasPixel* d = drow;

                    while (remaining > 0) {
                        int chunk = src + 1;

                        if (chunk > remaining) {
                            chunk = remaining;
                        }

#if defined(CRS_SW_CANVAS_16BPP)
                        if (q->modulate == 0xFFFFFFFFu) {
                            if (ckey_ok) {
                                sw_blit_indexed4_row_rev_ckey_565(d, packed_base, pal->colors565, chunk, src);
                            } else {
                                sw_blit_indexed4_row_rev_565(d, packed_base, pal->colors, pal->colors565, chunk, src);
                            }
                        } else {
                            sw_blit_indexed4_row_rev(d, packed_base, pal->colors, q->modulate, chunk, src);
                        }
#else
                        sw_blit_indexed4_row_rev(d, packed_base, pal->colors, q->modulate, chunk, src);
#endif
                        d += chunk;
                        remaining -= chunk;
                        src = tex_w - 1;
                    }
                } else {
                    uint32_t u_cur = u_scaled_rev;
                    int remaining = dst_w;
                    SWCanvasPixel* d = drow;

                    while (remaining > 0) {
                        int chunk = (du_fx == 0) ? remaining : (int)(u_cur / du_fx) + 1;

                        if (chunk > remaining) {
                            chunk = remaining;
                        }

                        sw_blit_scaled_indexed4_row_rev(d, packed_base, pal->colors, u_cur, du_fx, q->modulate, chunk);
                        d += chunk;
                        remaining -= chunk;
                        u_cur = (u_cur - (uint32_t)chunk * du_fx) & u_mask_fx;
                    }
                }
            } else if (unscaled) {
                int remaining = dst_w;
                int src = src_x_start_fwd;
                SWCanvasPixel* d = drow;

                while (remaining > 0) {
                    int chunk = tex_w - src;

                    if (chunk > remaining) {
                        chunk = remaining;
                    }

                    const int x_lsb = src & 1;
#if defined(CRS_SW_CANVAS_16BPP)
                    if (q->modulate == 0xFFFFFFFFu) {
                        if (ckey_ok) {
                            sw_blit_indexed4_row_ckey_565(d, packed_base + (src >> 1), pal->colors565, chunk, x_lsb);
                        } else {
                            sw_blit_indexed4_row_565(
                                d, packed_base + (src >> 1), pal->colors, pal->colors565, chunk, x_lsb
                            );
                        }
                    } else {
                        sw_blit_indexed4_row(d, packed_base + (src >> 1), pal->colors, q->modulate, chunk, x_lsb);
                    }
#else
                    sw_blit_indexed4_row(d, packed_base + (src >> 1), pal->colors, q->modulate, chunk, x_lsb);
#endif
                    d += chunk;
                    remaining -= chunk;
                    src = 0;
                }
            } else {
                uint32_t u_cur = u_scaled_fwd;
                int remaining = dst_w;
                SWCanvasPixel* d = drow;

                while (remaining > 0) {
                    int chunk = (du_fx == 0) ? remaining : (int)((u_mask_fx - u_cur) / du_fx) + 1;

                    if (chunk > remaining) {
                        chunk = remaining;
                    }

                    sw_blit_scaled_indexed4_row(d, packed_base, pal->colors, u_cur, du_fx, q->modulate, chunk);
                    d += chunk;
                    remaining -= chunk;
                    u_cur = (u_cur + (uint32_t)chunk * du_fx) & u_mask_fx;
                }
            }
        } else {
            const uint32_t* src_row = tex->converted + vy * tex_w;

            if (flip_x) {
                if (unscaled) {
                    int remaining = dst_w;
                    int src = src_x_start_rev;
                    SWCanvasPixel* d = drow;

                    while (remaining > 0) {
                        int chunk = src + 1;

                        if (chunk > remaining) {
                            chunk = remaining;
                        }

                        sw_blit_direct_row_rev(d, src_row + src, q->modulate, chunk);
                        d += chunk;
                        remaining -= chunk;
                        src = tex_w - 1;
                    }
                } else {
                    uint32_t u_cur = u_scaled_rev;
                    int remaining = dst_w;
                    SWCanvasPixel* d = drow;

                    while (remaining > 0) {
                        int chunk = (du_fx == 0) ? remaining : (int)(u_cur / du_fx) + 1;

                        if (chunk > remaining) {
                            chunk = remaining;
                        }

                        sw_blit_scaled_direct_row_rev(d, src_row, u_cur, du_fx, q->modulate, chunk);
                        d += chunk;
                        remaining -= chunk;
                        u_cur = (u_cur - (uint32_t)chunk * du_fx) & u_mask_fx;
                    }
                }
            } else if (unscaled) {
                int remaining = dst_w;
                int src = src_x_start_fwd;
                SWCanvasPixel* d = drow;

                while (remaining > 0) {
                    int chunk = tex_w - src;

                    if (chunk > remaining) {
                        chunk = remaining;
                    }

                    sw_blit_direct_row(d, src_row + src, q->modulate, chunk);
                    d += chunk;
                    remaining -= chunk;
                    src = 0;
                }
            } else {
                uint32_t u_cur = u_scaled_fwd;
                int remaining = dst_w;
                SWCanvasPixel* d = drow;

                while (remaining > 0) {
                    int chunk = (du_fx == 0) ? remaining : (int)((u_mask_fx - u_cur) / du_fx) + 1;

                    if (chunk > remaining) {
                        chunk = remaining;
                    }

                    sw_blit_scaled_direct_row(d, src_row, u_cur, du_fx, q->modulate, chunk);
                    d += chunk;
                    remaining -= chunk;
                    u_cur = (u_cur + (uint32_t)chunk * du_fx) & u_mask_fx;
                }
            }
        }

        v_fx = flip_y ? (v_fx - dv_fx) : (v_fx + dv_fx);
    }
}

static void clear_band(RBCtx* ctx) {
    for (int y = ctx->band_y0; y < ctx->band_y1; y++) {
        sw_fill_solid_row(canvas + y * CANVAS_PITCH, 0xFF000000u, CANVAS_W);
    }
}

static void render_band(RBCtx* ctx) {
    // Clear first so partial redraws do not leave stale rows behind.
    clear_band(ctx);

    const int n = (int)arrlen(quads);
    for (int i = 0; i < n; i++) {
        const SWQuad* q = &quads[i];

#if CRS_SW_DEDUP_FS_SOLIDS
        if (q->index == SW_QUAD_DEAD)
            continue;
#endif

        if (q->texture_spec.texture_index < 0) {
            rasterize_solid(q, ctx);
        } else {
            rasterize_textured(q, ctx);
        }
    }
}

void SoftwareRenderer_CreateTexture(unsigned int th) {
    const int texture_index = LO_16_BITS(th) - 1;
    const FLTexture* fl_texture = &flTexture[texture_index];
    const void* pixels = texture_source_pixels(fl_texture);

    if (pixels == NULL) {
        return;
    }

    SWTexture* slot = &textures[texture_index];

    if (slot->converted != NULL) {
        free(slot->converted);
        slot->converted = NULL;
    }

    slot->pixels = pixels;
    slot->width = (uint16_t)fl_texture->width;
    slot->height = (uint16_t)fl_texture->height;
    slot->width_mask = (uint16_t)(fl_texture->width - 1);
    slot->height_mask = (uint16_t)(fl_texture->height - 1);

    switch (fl_texture->format) {
    case SCE_GS_PSMT8:
        slot->palette_type = SW_PALETTE_8;
        {
            const uint8_t* px = (const uint8_t*)pixels;
            const int n = (int)fl_texture->width * (int)fl_texture->height;
            uint8_t m = 0;
            for (int i = 0; i < n; i++) {
                if (px[i] > m) {
                    m = px[i];
                }
            }
            slot->max_index = m;
        }
        break;

    case SCE_GS_PSMT4:
        slot->palette_type = SW_PALETTE_4;
        break;

    case SCE_GS_PSMCT16: {
        slot->palette_type = SW_PALETTE_NONE;
        const int count = fl_texture->width * fl_texture->height;

        if (count <= 0) {
            slot->converted = NULL;
            break;
        }

        slot->converted = malloc(sizeof(uint32_t) * (size_t)count);

        if (slot->converted == NULL) {
            break;
        }

        sw_convert_psmct16_to_argb(slot->converted, pixels, count);
        break;
    }

    default:
        fatal_error("Unhandled pixel format: %d", fl_texture->format);
        break;
    }
}

void SoftwareRenderer_DestroyTexture(unsigned int texture_handle) {
    (void)texture_handle;
    // Match the GL backend: texture slots are reused in place.
}

void SoftwareRenderer_UnlockTexture(unsigned int th) {
    SoftwareRenderer_CreateTexture(th);
}

void SoftwareRenderer_CreatePalette(unsigned int ph) {
    const int palette_index = HI_16_BITS(ph) - 1;
    const FLTexture* fl_palette = &flPalette[palette_index];
    const void* pixels = texture_source_pixels(fl_palette);

    if (pixels == NULL) {
        return;
    }

    const int color_count = fl_palette->width * fl_palette->height;
    SWPalette* slot = &palettes[palette_index];
    slot->count = (uint16_t)color_count;

    switch (fl_palette->format) {
    case SCE_GS_PSMCT32:
        sw_convert_psmct32_to_argb(slot->colors, pixels, color_count);
        break;

    case SCE_GS_PSMCT16:
        sw_convert_psmct16_to_argb(slot->colors, pixels, color_count);
        break;

    default:
        fatal_error("Unhandled pixel format: %d", fl_palette->format);
        break;
    }

#if defined(CRS_SW_CANVAS_16BPP)
    for (int i = 0; i < 256; i++) {
        slot->colors565[i] = sw_argb_to_canvas(slot->colors[i]);
    }
#endif

    // Track how far the colour-key pattern holds.
    uint8_t ckey = 0;
    if ((slot->colors[0] >> 24) == 0x00u) {
        for (int i = 1; i < 256; i++) {
            if ((slot->colors[i] >> 24) != 0xFFu) {
                break;
            }
            ckey = (uint8_t)i;
        }
    }
    slot->ckey_extent = ckey;

#if defined(CRS_SW_CANVAS_16BPP)
    slot->version++;
#endif
}

void SoftwareRenderer_DestroyPalette(unsigned int palette_handle) {
    (void)palette_handle;
}

void SoftwareRenderer_UnlockPalette(unsigned int ph) {
    SoftwareRenderer_CreatePalette(ph << 16);
}

void SoftwareRenderer_SetTexture(unsigned int th) {
    const int texture_handle = LO_16_BITS(th);
    latest_texture_spec.texture_index = (int16_t)(texture_handle - 1);

    const int palette_handle = HI_16_BITS(th);

    if (palette_handle > 0) {
        latest_texture_spec.palette_index = (int16_t)(palette_handle - 1);
    } else {
        latest_texture_spec.palette_index = -1;
    }
}

void SoftwareRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
    if (arrlen(quads) >= QUADS_MAX) {
        return;
    }

    SWQuad* quad = arraddnptr(quads, 1);

    for (int i = 0; i < 4; i++) {
        quad->positions[i].x = sprite->v[i].x;
        quad->positions[i].y = sprite->v[i].y;
        quad->tex_coords[i].s = sprite->t[i].s;
        quad->tex_coords[i].t = sprite->t[i].t;
    }

    quad->z = flPS2ConvScreenFZ(sprite->v[0].z);
    quad->index = (uint32_t)arrlen(quads);
    quad->modulate = color;
    quad->texture_spec = latest_texture_spec;
}

void SoftwareRenderer_DrawSprite(const Sprite* sprite, unsigned int color) {
    if (arrlen(quads) >= QUADS_MAX) {
        return;
    }

    SWQuad* quad = arraddnptr(quads, 1);

    quad->positions[0].x = sprite->v[0].x;
    quad->positions[0].y = sprite->v[0].y;
    quad->positions[3].x = sprite->v[3].x;
    quad->positions[3].y = sprite->v[3].y;
    quad_infer_positions(quad);

    quad->tex_coords[0].s = sprite->t[0].s;
    quad->tex_coords[0].t = sprite->t[0].t;
    quad->tex_coords[3].s = sprite->t[3].s;
    quad->tex_coords[3].t = sprite->t[3].t;
    quad_infer_tex_coords(quad);

    quad->z = flPS2ConvScreenFZ(sprite->v[0].z);
    quad->index = (uint32_t)arrlen(quads);
    quad->modulate = color;
    quad->texture_spec = latest_texture_spec;
}

void SoftwareRenderer_DrawSprite2(const Sprite2* sprite2) {
    if (arrlen(quads) >= QUADS_MAX) {
        return;
    }

    SWQuad* quad = arraddnptr(quads, 1);

    quad->positions[0].x = sprite2->v[0].x;
    quad->positions[0].y = sprite2->v[0].y;
    quad->positions[3].x = sprite2->v[1].x;
    quad->positions[3].y = sprite2->v[1].y;
    quad_infer_positions(quad);

    quad->tex_coords[0].s = sprite2->t[0].s;
    quad->tex_coords[0].t = sprite2->t[0].t;
    quad->tex_coords[3].s = sprite2->t[1].s;
    quad->tex_coords[3].t = sprite2->t[1].t;
    quad_infer_tex_coords(quad);

    quad->z = flPS2ConvScreenFZ(sprite2->v[0].z);
    quad->index = (uint32_t)arrlen(quads);
    quad->modulate = sprite2->vertex_color;
    quad->texture_spec = latest_texture_spec;
}

void SoftwareRenderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
    if (arrlen(quads) >= QUADS_MAX) {
        return;
    }

    SWQuad* _quad = arraddnptr(quads, 1);

    for (int i = 0; i < 4; i++) {
        _quad->positions[i].x = quad->v[i].x;
        _quad->positions[i].y = quad->v[i].y;
    }

    _quad->z = flPS2ConvScreenFZ(quad->v[0].z);
    _quad->index = (uint32_t)arrlen(quads);
    _quad->modulate = color;
    _quad->texture_spec = (SWTextureSpec) { -1, -1 };
}

bool SoftwareRenderer_Init(bool nearest_filter, int scale) {
    renderer_nearest_filter = nearest_filter;
    renderer_scale = scale;

    arrsetcap(quads, QUADS_MAX);

    const size_t canvas_bytes = sizeof(SWCanvasPixel) * CANVAS_PITCH * CANVAS_H;
    canvas = malloc(canvas_bytes);

    if (canvas == NULL) {
        return false;
    }

    // Clear to opaque black.
    const SWCanvasPixel clear_px = sw_argb_to_canvas(0xFF000000u);
    for (size_t i = 0; i < (size_t)CANVAS_PITCH * CANVAS_H; i++) {
        canvas[i] = clear_px;
    }

    latest_texture_spec = (SWTextureSpec) { -1, -1 };

    return true;
}

void SoftwareRenderer_Quit() {
    for (int i = 0; i < FL_TEXTURE_MAX; i++) {
        if (textures[i].converted != NULL) {
            free(textures[i].converted);
            textures[i].converted = NULL;
        }
    }

    arrfree(quads);
    quads = NULL;
    free(canvas);
    canvas = NULL;
}

void SoftwareRenderer_RenderFrame() {
    sort_quads_fast();
#if CRS_SW_DEDUP_FS_SOLIDS
    dedup_fs_solids();
#endif

    RBCtx main_ctx;
    main_ctx.band_y0 = 0;
    main_ctx.band_y1 = CANVAS_H;
    render_band(&main_ctx);

    arrsetlen(quads, 0);
}

const SWCanvasPixel* SoftwareRenderer_GetCanvas(int* out_width, int* out_height, int* out_pitch_bytes) {
    if (out_width != NULL) {
        *out_width = CANVAS_W;
    }

    if (out_height != NULL) {
        *out_height = CANVAS_H;
    }

    if (out_pitch_bytes != NULL) {
        *out_pitch_bytes = CANVAS_PITCH * (int)sizeof(SWCanvasPixel);
    }

    return canvas;
}

bool SoftwareRenderer_UsesNearestFilter() {
    return renderer_nearest_filter;
}

#endif // CRS_VIDEO_DRIVER_SOFTWARE
