#if CRS_VIDEO_DRIVER_SDL_GENERIC

#ifndef SDL_RENDER_BACKEND_H
#define SDL_RENDER_BACKEND_H

#include "core/render_primitives.h"

#include <SDL3/SDL.h>

typedef struct SDLRenderBackendInitInfo {
    const char* app_name;
    int window_width;
    int window_height;
    SDL_WindowFlags window_flags;
} SDLRenderBackendInitInfo;

typedef struct SDLRenderBackend {
    const char* name;
    SDL_Window* (*init)(const SDLRenderBackendInitInfo* init_info);
    void (*quit)();
    void (*render_frame)(SDL_Rect viewport);

    void (*create_texture)(Uint32 th);
    void (*destroy_texture)(Uint32 texture_handle);
    void (*unlock_texture)(Uint32 th);
    void (*create_palette)(Uint32 ph);
    void (*destroy_palette)(Uint32 palette_handle);
    void (*unlock_palette)(Uint32 ph);
    void (*set_texture)(Uint32 th);
    void (*draw_textured_quad)(const Sprite* sprite, Uint32 color);
    void (*draw_sprite)(const Sprite* sprite, Uint32 color);
    void (*draw_sprite2)(const Sprite2* sprite2);
    void (*draw_solid_quad)(const Quad* quad, Uint32 color);
} SDLRenderBackend;

#endif

#endif // CRS_VIDEO_DRIVER_SDL_GENERIC
