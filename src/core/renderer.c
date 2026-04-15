/**
 * @brief Render dispatch through the appropriate host render backend.
 */

#include "core/renderer.h"

#if CRS_VIDEO_DRIVER_OPENGL
#include "platform/video/opengl/opengl_renderer.h"
#elif CRS_VIDEO_DRIVER_PSP
#include "platform/video/psp/psp_renderer.h"
#endif

void Renderer_CreateTexture(unsigned int th) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_CreateTexture(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_CreateTexture(th);
#endif
}

void Renderer_DestroyTexture(unsigned int texture_handle) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_DestroyTexture(texture_handle);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DestroyTexture(texture_handle);
#endif
}

void Renderer_UnlockTexture(unsigned int th) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_UnlockTexture(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_UnlockTexture(th);
#endif
}

void Renderer_CreatePalette(unsigned int ph) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_CreatePalette(ph);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_CreatePalette(ph);
#endif
}

void Renderer_DestroyPalette(unsigned int palette_handle) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_DestroyPalette(palette_handle);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DestroyPalette(palette_handle);
#endif
}

void Renderer_UnlockPalette(unsigned int th) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_UnlockPalette(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_UnlockPalette(th);
#endif
}

void Renderer_SetTexture(unsigned int th) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_SetTexture(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_SetTexture(th);
#endif
}

void Renderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_DrawTexturedQuad(sprite, color);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawTexturedQuad(sprite, color);
#endif
}

void Renderer_DrawSprite(const Sprite* sprite, unsigned int color) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_DrawSprite(sprite, color);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSprite(sprite, color);
#endif
}

void Renderer_DrawSprite2(const Sprite2* sprite2) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_DrawSprite2(sprite2);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSprite2(sprite2);
#endif
}

void Renderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
#if CRS_VIDEO_DRIVER_OPENGL
    OpenGLRenderer_DrawSolidQuad(quad, color);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSolidQuad(quad, color);
#endif
}
