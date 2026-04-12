#ifndef SDL_GAME_RENDERER_H
#define SDL_GAME_RENDERER_H

#include "core/render_primitives.h"

#include <SDL3/SDL.h>

typedef struct SDLGameRenderer_Vertex {
    struct {
        float x;
        float y;
        float z;
        float w;
    } coord;
    unsigned int color;
    TexCoord tex_coord;
} SDLGameRenderer_Vertex;

// Internal

void SDLGameRenderer_Init(SDL_Renderer* renderer);
void SDLGameRenderer_Shutdown();
void SDLGameRenderer_BeginFrame();
void SDLGameRenderer_RenderFrame();
void SDLGameRenderer_EndFrame();
SDL_Texture* SDLGameRenderer_GetCanvas();

// Public

void SDLGameRenderer_CreateTexture(unsigned int th);
void SDLGameRenderer_DestroyTexture(unsigned int texture_handle);
void SDLGameRenderer_UnlockTexture(unsigned int th);
void SDLGameRenderer_CreatePalette(unsigned int ph);
void SDLGameRenderer_DestroyPalette(unsigned int palette_handle);
void SDLGameRenderer_UnlockPalette(unsigned int ph);
void SDLGameRenderer_SetTexture(unsigned int th);
void SDLGameRenderer_DrawTexturedQuad(const Sprite* sprite, unsigned int color);
void SDLGameRenderer_DrawSprite(const Sprite* sprite, unsigned int color);
void SDLGameRenderer_DrawSprite2(const Sprite2* sprite2);
void SDLGameRenderer_DrawSolidQuad(const Quad* quad, unsigned int color);

#endif
