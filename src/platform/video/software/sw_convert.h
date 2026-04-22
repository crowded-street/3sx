#if CRS_VIDEO_DRIVER_SOFTWARE

#ifndef SW_CONVERT_H
#define SW_CONVERT_H

#include <stdint.h>

// Converts a PS2-format pixel buffer into the canonical in-engine ARGB8888 layout.
// Destination buffers must be sized to hold width*height uint32_t entries.

void sw_convert_psmct32_to_argb(uint32_t* dst, const void* src, int count);
void sw_convert_psmct16_to_argb(uint32_t* dst, const void* src, int count);

#endif

#endif // CRS_VIDEO_DRIVER_SOFTWARE
