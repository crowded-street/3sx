#if CRS_VIDEO_DRIVER_SDL_GENERIC

#ifndef SDL_GENERIC_RENDERER_H
#define SDL_GENERIC_RENDERER_H

#include "core/render_primitives.h"
#include "platform/video/sdl_generic/sdl_render_backend.h"

#include <SDL3/SDL.h>

// Public

void SDLGenericRenderer_CreateTexture(Uint32 th);
void SDLGenericRenderer_DestroyTexture(Uint32 texture_handle);
void SDLGenericRenderer_UnlockTexture(Uint32 th);
void SDLGenericRenderer_CreatePalette(Uint32 ph);
void SDLGenericRenderer_DestroyPalette(Uint32 palette_handle);
void SDLGenericRenderer_UnlockPalette(Uint32 ph);
void SDLGenericRenderer_SetTexture(Uint32 th);
void SDLGenericRenderer_DrawTexturedQuad(const Sprite* sprite, Uint32 color);
void SDLGenericRenderer_DrawSprite(const Sprite* sprite, Uint32 color);
void SDLGenericRenderer_DrawSprite2(const Sprite2* sprite2);
void SDLGenericRenderer_DrawSolidQuad(const Quad* quad, Uint32 color);

// Internal

SDL_Window* SDLGenericRenderer_Init(const SDLRenderBackendInitInfo* init_info);
void SDLGenericRenderer_Quit();
void SDLGenericRenderer_RenderFrame(SDL_Rect viewport);

#endif

#endif // CRS_VIDEO_DRIVER_SDL_GENERIC
