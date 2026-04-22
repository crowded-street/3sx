#if CRS_VIDEO_DRIVER_SOFTWARE

#include "platform/video/software/sw_convert.h"

// PSMCT32 in the PS2 pipeline is stored BGRA little-endian; the engine everywhere else
// works in ARGB8888 (A<<24 | R<<16 | G<<8 | B). This swaps R and B in place per pixel.
void sw_convert_psmct32_to_argb(uint32_t* dst, const void* src, int count) {
    const uint8_t* s = (const uint8_t*)src;

    for (int i = 0; i < count; i++) {
        const uint32_t b = s[0];
        const uint32_t g = s[1];
        const uint32_t r = s[2];
        const uint32_t a = s[3];
        dst[i] = (a << 24) | (r << 16) | (g << 8) | b;
        s += 4;
    }
}

// PSMCT16 is 1-5-5-5 packed, little-endian.  We expand each 5-bit channel to 8 bits
// using the standard (v << 3) | (v >> 2) trick so 0 maps to 0 and 31 maps to 255.
void sw_convert_psmct16_to_argb(uint32_t* dst, const void* src, int count) {
    const uint16_t* s = (const uint16_t*)src;

    for (int i = 0; i < count; i++) {
        const uint16_t v = s[i];
        const uint32_t r5 = v & 0x1F;
        const uint32_t g5 = (v >> 5) & 0x1F;
        const uint32_t b5 = (v >> 10) & 0x1F;
        const uint32_t a1 = (v >> 15) & 0x1;

        const uint32_t r = (r5 << 3) | (r5 >> 2);
        const uint32_t g = (g5 << 3) | (g5 >> 2);
        const uint32_t b = (b5 << 3) | (b5 >> 2);
        const uint32_t a = a1 ? 0xFF : 0x00;

        dst[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}

#endif // CRS_VIDEO_DRIVER_SOFTWARE
