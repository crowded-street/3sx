#if CRS_VIDEO_DRIVER_SOFTWARE

#ifndef SOFTWARE_RENDERER_H
#define SOFTWARE_RENDERER_H

#include "core/render_primitives.h"
#include "platform/video/software/sw_blit.h"

#include <stdbool.h>
#include <stdint.h>

// Public

void SoftwareRenderer_CreateTexture(unsigned int th);
void SoftwareRenderer_DestroyTexture(unsigned int texture_handle);
void SoftwareRenderer_UnlockTexture(unsigned int th);
void SoftwareRenderer_CreatePalette(unsigned int ph);
void SoftwareRenderer_DestroyPalette(unsigned int palette_handle);
void SoftwareRenderer_UnlockPalette(unsigned int ph);
void SoftwareRenderer_SetTexture(unsigned int th);
void SoftwareRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color);
void SoftwareRenderer_DrawSprite(const Sprite* sprite, unsigned int color);
void SoftwareRenderer_DrawSprite2(const Sprite2* sprite2);
void SoftwareRenderer_DrawSolidQuad(const Quad* quad, unsigned int color);

// Internal

bool SoftwareRenderer_Init(bool nearest_filter, int scale);
void SoftwareRenderer_Quit();
void SoftwareRenderer_RenderFrame();

// Canvas accessor for the host app driver to present (SDL streaming texture, DRM dumb buffer, etc.).
// Pixel layout is ARGB8888 (default) or RGB565 (with CRS_SW_CANVAS_16BPP) — see SWCanvasPixel in
// sw_blit.h. Tightly packed (pitch == width * sizeof(SWCanvasPixel)) unless pitch_bytes reports otherwise.
const SWCanvasPixel* SoftwareRenderer_GetCanvas(int* out_width, int* out_height, int* out_pitch_bytes);
bool SoftwareRenderer_UsesNearestFilter();

#endif

#endif // CRS_VIDEO_DRIVER_SOFTWARE
