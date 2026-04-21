#if CRS_VIDEO_DRIVER_SDL_GPU

#ifndef SDL_GPU_RENDERER_H
#define SDL_GPU_RENDERER_H

#include "core/render_primitives.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

// Public

void SDLGPURenderer_CreateTexture(Uint32 th);
void SDLGPURenderer_DestroyTexture(Uint32 texture_handle);
void SDLGPURenderer_UnlockTexture(Uint32 th);
void SDLGPURenderer_CreatePalette(Uint32 ph);
void SDLGPURenderer_DestroyPalette(Uint32 palette_handle);
void SDLGPURenderer_UnlockPalette(Uint32 ph);
void SDLGPURenderer_SetTexture(Uint32 th);
void SDLGPURenderer_DrawTexturedQuad(const Sprite* sprite, Uint32 color);
void SDLGPURenderer_DrawSprite(const Sprite* sprite, Uint32 color);
void SDLGPURenderer_DrawSprite2(const Sprite2* sprite2);
void SDLGPURenderer_DrawSolidQuad(const Quad* quad, Uint32 color);

// Internal

typedef struct SDLGPURendererContext {
    SDL_Window* window;
    SDL_GPUDevice* device;
    SDL_GPUPresentMode present_mode;
} SDLGPURendererContext;

bool SDLGPURenderer_Init(const SDLGPURendererContext* context);
void SDLGPURenderer_Quit(const SDLGPURendererContext* context);
void SDLGPURenderer_RenderFrame(const SDLGPURendererContext* context, SDL_Rect viewport);

#endif

#endif // CRS_VIDEO_DRIVER_SDL_GPU
