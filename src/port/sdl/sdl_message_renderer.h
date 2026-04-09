#ifndef SDL_MESSAGE_RENDERER_H
#define SDL_MESSAGE_RENDERER_H

#include <SDL3/SDL.h>

extern SDL_Texture* message_canvas;

void SDLMessageRenderer_Initialize(SDL_Renderer* renderer);
void SDLMessageRenderer_Shutdown();
void SDLMessageRenderer_BeginFrame();

void SDLMessageRenderer_CreateTexture(int width, int height, void* pixels, int format);
void SDLMessageRenderer_DrawTexture(int x0, int y0, int x1, int y1, int u0, int v0, int u1, int v1, unsigned int color);

#endif
