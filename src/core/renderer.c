/**
 * @brief Render dispatch through the appropriate host render backend.
 */

#include "core/renderer.h"

#if CRS_VIDEO_DRIVER_SDL_GENERIC
#include "args.h"

#include "platform/video/sdl_generic/sdl_generic_renderer.h"
#elif CRS_VIDEO_DRIVER_PSP
#include "platform/video/psp/psp_renderer.h"
#endif

#if CRS_VIDEO_DRIVER_SDL_GENERIC
static bool skip_rendering() {
#if STATCHECK
    return get_args()->statcheck.headless;
#elif NETPLAY_ENABLED
    return get_args()->netplay.stress;
#else
    return false;
#endif
}
#endif

/* The seven handle entry points are one dispatch written seven times: the
 * backend call this build has, behind the skip the SDL backend checks. They
 * differ only in the name they call, which is written out at each call site.
 * The pointer type is declared per configuration because the two backends
 * spell the handle differently - Uint32 against unsigned int - and casting a
 * function pointer to make two signatures fit is forbidden. */
#if CRS_VIDEO_DRIVER_SDL_GENERIC
static void renderer_handle_op(Uint32 handle, void (*op)(Uint32)) {
    if (skip_rendering()) {
        return;
    }

    op(handle);
}
#elif CRS_VIDEO_DRIVER_PSP
static void renderer_handle_op(unsigned int handle, void (*op)(unsigned int)) {
    op(handle);
}
#endif

void Renderer_CreateTexture(unsigned int th) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_handle_op(th, SDLGenericRenderer_CreateTexture);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_handle_op(th, PSPRenderer_CreateTexture);
#endif
}

void Renderer_DestroyTexture(unsigned int texture_handle) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_handle_op(texture_handle, SDLGenericRenderer_DestroyTexture);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_handle_op(texture_handle, PSPRenderer_DestroyTexture);
#endif
}

void Renderer_UnlockTexture(unsigned int th) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_handle_op(th, SDLGenericRenderer_UnlockTexture);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_handle_op(th, PSPRenderer_UnlockTexture);
#endif
}

void Renderer_CreatePalette(unsigned int ph) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_handle_op(ph, SDLGenericRenderer_CreatePalette);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_handle_op(ph, PSPRenderer_CreatePalette);
#endif
}

void Renderer_DestroyPalette(unsigned int palette_handle) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_handle_op(palette_handle, SDLGenericRenderer_DestroyPalette);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_handle_op(palette_handle, PSPRenderer_DestroyPalette);
#endif
}

void Renderer_UnlockPalette(unsigned int th) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_handle_op(th, SDLGenericRenderer_UnlockPalette);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_handle_op(th, PSPRenderer_UnlockPalette);
#endif
}

void Renderer_SetTexture(unsigned int th) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_handle_op(th, SDLGenericRenderer_SetTexture);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_handle_op(th, PSPRenderer_SetTexture);
#endif
}

/* The two sprite draws, the same dispatch as the handle ones. */
#if CRS_VIDEO_DRIVER_SDL_GENERIC
static void renderer_sprite_op(const Sprite* sprite, Uint32 color, void (*op)(const Sprite*, Uint32)) {
    if (skip_rendering()) {
        return;
    }

    op(sprite, color);
}
#elif CRS_VIDEO_DRIVER_PSP
static void renderer_sprite_op(const Sprite* sprite, unsigned int color,
                               void (*op)(const Sprite*, unsigned int)) {
    op(sprite, color);
}
#endif

void Renderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_sprite_op(sprite, color, SDLGenericRenderer_DrawTexturedQuad);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_sprite_op(sprite, color, PSPRenderer_DrawTexturedQuad);
#endif
}

void Renderer_DrawSprite(const Sprite* sprite, unsigned int color) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    renderer_sprite_op(sprite, color, SDLGenericRenderer_DrawSprite);
#elif CRS_VIDEO_DRIVER_PSP
    renderer_sprite_op(sprite, color, PSPRenderer_DrawSprite);
#endif
}

void Renderer_DrawSprite2(const Sprite2* sprite2) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    if (skip_rendering()) {
        return;
    }

    SDLGenericRenderer_DrawSprite2(sprite2);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSprite2(sprite2);
#endif
}

void Renderer_DrawSolidQuad(const Quad* quad, unsigned int color) {
#if CRS_VIDEO_DRIVER_SDL_GENERIC
    if (skip_rendering()) {
        return;
    }

    SDLGenericRenderer_DrawSolidQuad(quad, color);
#elif CRS_VIDEO_DRIVER_PSP
    PSPRenderer_DrawSolidQuad(quad, color);
#endif
}
