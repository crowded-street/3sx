#ifndef SCANLINE_RENDERER_H
#define SCANLINE_RENDERER_H

#include <SDL3/SDL.h>

void ScanlineRenderer_Init(SDL_Renderer* renderer);
void ScanlineRenderer_Destroy();
void ScanlineRenderer_Render(const SDL_FRect* game_rect);

#endif
