#if CRS_VIDEO_DRIVER_SOFTWARE

#ifndef SW_BLIT_H
#define SW_BLIT_H

#include <stdbool.h>
#include <stdint.h>

// Canvas format. Default is ARGB8888; 16bpp builds use RGB565.

#if defined(CRS_SW_CANVAS_16BPP)
typedef uint16_t SWCanvasPixel;
#else
typedef uint32_t SWCanvasPixel;
#endif

// Convert ARGB8888 into the canvas format.
static inline SWCanvasPixel sw_argb_to_canvas(uint32_t argb) {
#if defined(CRS_SW_CANVAS_16BPP)
    const uint32_t r = (argb >> 8) & 0xF800u;
    const uint32_t g = (argb >> 5) & 0x07E0u;
    const uint32_t b = (argb >> 3) & 0x001Fu;
    return (SWCanvasPixel)(r | g | b);
#else
    return (SWCanvasPixel)argb;
#endif
}

// Convert a canvas pixel back to ARGB8888 for blending.
static inline uint32_t sw_canvas_to_argb(SWCanvasPixel px) {
#if defined(CRS_SW_CANVAS_16BPP)
    uint32_t r = (px >> 11) & 0x1Fu;
    uint32_t g = (px >> 5)  & 0x3Fu;
    uint32_t b =  px        & 0x1Fu;
    r = (r << 3) | (r >> 2);
    g = (g << 2) | (g >> 4);
    b = (b << 3) | (b >> 2);
    return 0xFF000000u | (r << 16) | (g << 8) | b;
#else
    return (uint32_t)px;
#endif
}

// Row kernels write `count` pixels into `dst`.

// Solid fill.
void sw_fill_solid_row(SWCanvasPixel* dst, uint32_t argb, int count);

// Unscaled direct-colour blit.
void sw_blit_direct_row(SWCanvasPixel* dst, const uint32_t* src, uint32_t modulate, int count);

// Unscaled 8bpp indexed blit. `kernel` picks the preferred backend.
void sw_blit_indexed8_row(SWCanvasPixel* dst, const uint8_t* idx, const uint32_t* pal, uint32_t modulate, int count,
                          char kernel);

#if defined(CRS_SW_CANVAS_16BPP)
// 8bpp RGB565 fast path.
void sw_blit_indexed8_row_565(uint16_t* dst, const uint8_t* idx, const uint32_t* pal, const uint16_t* pal565,
                              int count);
void sw_blit_indexed8_row_rev_565(uint16_t* dst, const uint8_t* idx_last, const uint32_t* pal, const uint16_t* pal565,
                                  int count);

// 8bpp colour-key fast path.
void sw_blit_indexed8_row_ckey_565(uint16_t* dst, const uint8_t* idx, const uint32_t* pal_u32, int count);
void sw_blit_indexed8_row_rev_ckey_565(uint16_t* dst, const uint8_t* idx_last, const uint32_t* pal_u32, int count);

// 4bpp RGB565 fast path.
void sw_blit_indexed4_row_565(uint16_t* dst, const uint8_t* packed, const uint32_t* pal16, const uint16_t* pal565_16,
                              int count, int x_lsb);
void sw_blit_indexed4_row_rev_565(uint16_t* dst, const uint8_t* packed, const uint32_t* pal16,
                                  const uint16_t* pal565_16, int count, int start_nibble);

// 4bpp colour-key fast path.
void sw_blit_indexed4_row_ckey_565(uint16_t* dst, const uint8_t* packed, const uint16_t* pal565_16, int count,
                                   int x_lsb);
void sw_blit_indexed4_row_rev_ckey_565(uint16_t* dst, const uint8_t* packed, const uint16_t* pal565_16, int count,
                                       int start_nibble);

// Scaled 8bpp colour-key fast path.
void sw_blit_scaled_indexed8_row_ckey_565(uint16_t* dst, const uint8_t* idx_row, const uint16_t* pal565,
                                          uint32_t u_fx, uint32_t du_fx, int count);
void sw_blit_scaled_indexed8_row_rev_ckey_565(uint16_t* dst, const uint8_t* idx_row, const uint16_t* pal565,
                                              uint32_t u_fx, uint32_t du_fx, int count);
#endif

// Unscaled 4bpp indexed blit.
void sw_blit_indexed4_row(SWCanvasPixel* dst, const uint8_t* packed, const uint32_t* pal16, uint32_t modulate,
                          int count, int x_lsb);

// Scaled row blits. `u_fx` and `du_fx` are 16.16.
void sw_blit_scaled_indexed8_row(SWCanvasPixel* dst, const uint8_t* idx_row, const uint32_t* pal, uint32_t u_fx,
                                 uint32_t du_fx, uint32_t modulate, int count);
void sw_blit_scaled_indexed4_row(SWCanvasPixel* dst, const uint8_t* packed_row, const uint32_t* pal16, uint32_t u_fx,
                                 uint32_t du_fx, uint32_t modulate, int count);
void sw_blit_scaled_direct_row(SWCanvasPixel* dst, const uint32_t* src_row, uint32_t u_fx, uint32_t du_fx,
                               uint32_t modulate, int count);

// X-flipped variants.
void sw_blit_direct_row_rev(SWCanvasPixel* dst, const uint32_t* src_last, uint32_t modulate, int count);
void sw_blit_indexed8_row_rev(SWCanvasPixel* dst, const uint8_t* idx_last, const uint32_t* pal, uint32_t modulate,
                              int count);
void sw_blit_scaled_indexed8_row_rev(SWCanvasPixel* dst, const uint8_t* idx_row, const uint32_t* pal, uint32_t u_fx,
                                     uint32_t du_fx, uint32_t modulate, int count);
void sw_blit_scaled_indexed4_row_rev(SWCanvasPixel* dst, const uint8_t* packed_row, const uint32_t* pal16, uint32_t u_fx,
                                     uint32_t du_fx, uint32_t modulate, int count);
void sw_blit_scaled_direct_row_rev(SWCanvasPixel* dst, const uint32_t* src_row, uint32_t u_fx, uint32_t du_fx,
                                   uint32_t modulate, int count);
void sw_blit_indexed4_row_rev(SWCanvasPixel* dst, const uint8_t* packed, const uint32_t* pal16, uint32_t modulate,
                              int count, int start_nibble);

// Present-time canvas scale for the SDL software path.
#if !defined(CRS_SW_CANVAS_16BPP)
void sw_present_scale_argb(uint32_t* dst, int dst_pitch_px, int dst_w, int dst_h, const uint32_t* src, int src_pitch_px,
                           int src_w, int src_h, bool nearest);
void sw_present_scale_argb_scalar(uint32_t* dst, int dst_pitch_px, int dst_w, int dst_h, const uint32_t* src,
                                  int src_pitch_px, int src_w, int src_h, bool nearest);
#endif

#endif

#endif // CRS_VIDEO_DRIVER_SOFTWARE
