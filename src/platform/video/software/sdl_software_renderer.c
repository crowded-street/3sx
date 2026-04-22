#if CRS_VIDEO_DRIVER_SDL_GENERIC && CRS_VIDEO_DRIVER_SOFTWARE

#include "platform/video/software/sdl_software_renderer.h"

#include "platform/video/software/software_renderer.h"

#include <SDL3/SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* canvas_texture = NULL;

static SDL_PixelFormat get_canvas_format() {
#if defined(CRS_SW_CANVAS_16BPP)
    return SDL_PIXELFORMAT_RGB565;
#else
    return SDL_PIXELFORMAT_ARGB8888;
#endif
}

static bool create_canvas_texture() {
    int canvas_width = 0;
    int canvas_height = 0;

    SoftwareRenderer_GetCanvas(&canvas_width, &canvas_height, NULL);

    canvas_texture =
        SDL_CreateTexture(renderer, get_canvas_format(), SDL_TEXTUREACCESS_STREAMING, canvas_width, canvas_height);

    if (canvas_texture == NULL) {
        SDL_Log("Couldn't create software canvas texture: %s", SDL_GetError());
        return false;
    }

    SDL_SetTextureScaleMode(
        canvas_texture, SoftwareRenderer_UsesNearestFilter() ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR
    );

    return true;
}

static void destroy_host_objects() {
    if (canvas_texture != NULL) {
        SDL_DestroyTexture(canvas_texture);
        canvas_texture = NULL;
    }

    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

static SDL_Window* SDLSoftwareRenderer_Init(const SDLRenderBackendInitInfo* init_info) {
    window = SDL_CreateWindow(
        init_info->app_name, init_info->window_width, init_info->window_height, init_info->window_flags
    );

    if (window == NULL) {
        SDL_Log("Couldn't create SDL window: %s", SDL_GetError());
        return NULL;
    }

    renderer = SDL_CreateRenderer(window, "software");

    if (renderer == NULL) {
        SDL_Log("Couldn't create SDL software renderer, trying default renderer: %s", SDL_GetError());
        renderer = SDL_CreateRenderer(window, NULL);
    }

    if (renderer == NULL) {
        SDL_Log("Couldn't create SDL renderer: %s", SDL_GetError());
        destroy_host_objects();
        return NULL;
    }

    SDL_SetRenderVSync(renderer, 0);

    if (!SoftwareRenderer_Init(true, 1)) {
        SDL_Log("Couldn't initialize software renderer");
        destroy_host_objects();
        return NULL;
    }

    if (!create_canvas_texture()) {
        SoftwareRenderer_Quit();
        destroy_host_objects();
        return NULL;
    }

    return window;
}

static void SDLSoftwareRenderer_Quit() {
    SoftwareRenderer_Quit();
    destroy_host_objects();
}

static void SDLSoftwareRenderer_RenderFrame(SDL_Rect viewport) {
    int pitch_bytes = 0;
    const SWCanvasPixel* canvas = NULL;

    SoftwareRenderer_RenderFrame();
    canvas = SoftwareRenderer_GetCanvas(NULL, NULL, &pitch_bytes);

    SDL_UpdateTexture(canvas_texture, NULL, canvas, pitch_bytes);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, canvas_texture, NULL, &viewport);
    SDL_RenderPresent(renderer);
}

const SDLRenderBackend sdl_software_render_backend = {
    .name = "SDL software",
    .init = SDLSoftwareRenderer_Init,
    .quit = SDLSoftwareRenderer_Quit,
    .render_frame = SDLSoftwareRenderer_RenderFrame,
    .create_texture = SoftwareRenderer_CreateTexture,
    .destroy_texture = SoftwareRenderer_DestroyTexture,
    .unlock_texture = SoftwareRenderer_UnlockTexture,
    .create_palette = SoftwareRenderer_CreatePalette,
    .destroy_palette = SoftwareRenderer_DestroyPalette,
    .unlock_palette = SoftwareRenderer_UnlockPalette,
    .set_texture = SoftwareRenderer_SetTexture,
    .draw_textured_quad = SoftwareRenderer_DrawTexturedQuad,
    .draw_sprite = SoftwareRenderer_DrawSprite,
    .draw_sprite2 = SoftwareRenderer_DrawSprite2,
    .draw_solid_quad = SoftwareRenderer_DrawSolidQuad,
};

#endif // CRS_VIDEO_DRIVER_SDL_GENERIC && CRS_VIDEO_DRIVER_SOFTWARE
