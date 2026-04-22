/**
 * @brief Render dispatch through the active host backend.
 */

#include "core/renderer.h"

#if CRS_APP_DRIVER_SDL
#include "platform/video/sdl_generic/sdl_generic_renderer.h"
#elif CRS_APP_DRIVER_ARM
#include "platform/video/software/software_renderer.h"
#elif CRS_VIDEO_DRIVER_PSP
#include "platform/video/psp/psp_renderer.h"
#endif

void Renderer_CreateTexture(unsigned int th) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_CreateTexture(th);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_CreateTexture(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_CreateTexture(th);
#endif
}

void Renderer_DestroyTexture(unsigned int texture_handle) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_DestroyTexture(texture_handle);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_DestroyTexture(texture_handle);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DestroyTexture(texture_handle);
#endif
}

void Renderer_UnlockTexture(unsigned int th) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_UnlockTexture(th);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_UnlockTexture(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_UnlockTexture(th);
#endif
}

void Renderer_CreatePalette(unsigned int ph) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_CreatePalette(ph);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_CreatePalette(ph);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_CreatePalette(ph);
#endif
}

void Renderer_DestroyPalette(unsigned int palette_handle) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_DestroyPalette(palette_handle);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_DestroyPalette(palette_handle);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DestroyPalette(palette_handle);
#endif
}

void Renderer_UnlockPalette(unsigned int th) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_UnlockPalette(th);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_UnlockPalette(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_UnlockPalette(th);
#endif
}

void Renderer_SetTexture(unsigned int th) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_SetTexture(th);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_SetTexture(th);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_SetTexture(th);
#endif
}

void Renderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_DrawTexturedQuad(sprite, color);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_DrawTexturedQuad(sprite, color);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawTexturedQuad(sprite, color);
#endif
}

void Renderer_DrawSprite(const Sprite* sprite, unsigned int color) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_DrawSprite(sprite, color);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_DrawSprite(sprite, color);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSprite(sprite, color);
#endif
}

void Renderer_DrawSprite2(const Sprite2* sprite2) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_DrawSprite2(sprite2);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_DrawSprite2(sprite2);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSprite2(sprite2);
#endif
}

void Renderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
#if CRS_APP_DRIVER_SDL
    SDLGenericRenderer_DrawSolidQuad(quad, color);
#elif CRS_APP_DRIVER_ARM
    SoftwareRenderer_DrawSolidQuad(quad, color);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSolidQuad(quad, color);
#endif
}
