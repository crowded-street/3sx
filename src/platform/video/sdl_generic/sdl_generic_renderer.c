#if CRS_VIDEO_DRIVER_SDL_GENERIC

#include "platform/video/sdl_generic/sdl_generic_renderer.h"

#if CRS_VIDEO_DRIVER_SDL_GPU
#include "platform/video/sdl_gpu/sdl_gpu_renderer.h"
#endif

#if CRS_VIDEO_DRIVER_OPENGL
#include "platform/video/opengl/opengl_renderer.h"
#endif

#include <SDL3/SDL.h>

static SDLRenderBackend* backend_candidates[] = {
#if CRS_VIDEO_DRIVER_SDL_GPU
    &sdl_gpu_render_backend,
#endif
#if CRS_VIDEO_DRIVER_OPENGL
    &opengl_render_backend,
#endif
};

static SDLRenderBackend* chosen_backend = NULL;

// Public

void SDLGenericRenderer_CreateTexture(Uint32 th) {
    chosen_backend->create_texture(th);
}

void SDLGenericRenderer_DestroyTexture(Uint32 texture_handle) {
    chosen_backend->destroy_texture(texture_handle);
}

void SDLGenericRenderer_UnlockTexture(Uint32 th) {
    chosen_backend->unlock_texture(th);
}

void SDLGenericRenderer_CreatePalette(Uint32 ph) {
    chosen_backend->create_palette(ph);
}

void SDLGenericRenderer_DestroyPalette(Uint32 palette_handle) {
    chosen_backend->destroy_palette(palette_handle);
}

void SDLGenericRenderer_UnlockPalette(Uint32 ph) {
    chosen_backend->unlock_palette(ph);
}

void SDLGenericRenderer_SetTexture(Uint32 th) {
    chosen_backend->set_texture(th);
}

void SDLGenericRenderer_DrawTexturedQuad(const Sprite* sprite, Uint32 color) {
    chosen_backend->draw_textured_quad(sprite, color);
}

void SDLGenericRenderer_DrawSprite(const Sprite* sprite, Uint32 color) {
    chosen_backend->draw_sprite(sprite, color);
}

void SDLGenericRenderer_DrawSprite2(const Sprite2* sprite2) {
    chosen_backend->draw_sprite2(sprite2);
}

void SDLGenericRenderer_DrawSolidQuad(const Quad* quad, Uint32 color) {
    chosen_backend->draw_solid_quad(quad, color);
}

// Internal

SDL_Window* SDLGenericRenderer_Init(const SDLRenderBackendInitInfo* init_info) {
    SDL_Log("Initializing SDL render backend");

    for (int i = 0; i < SDL_arraysize(backend_candidates); i++) {
        const SDLRenderBackend* candidate = backend_candidates[i];
        SDL_Log("Trying %s", candidate->name);

        SDL_Window* window = candidate->init(init_info);

        if (window != NULL) {
            chosen_backend = candidate;
            return window;
        }
    }

    SDL_Log("All render backends failed to initialize");
    return NULL;
}

void SDLGenericRenderer_Quit() {
    chosen_backend->quit();
}

void SDLGenericRenderer_RenderFrame(SDL_Rect viewport) {
    chosen_backend->render_frame(viewport);
}

#endif // CRS_VIDEO_DRIVER_SDL_GENERIC
