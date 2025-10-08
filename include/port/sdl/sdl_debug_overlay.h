#ifndef SDL_DEBUG_OVERLAY_H
#define SDL_DEBUG_OVERLAY_H

#include <SDL3/SDL.h>
#include <stdbool.h>

// Render debug overlays
void SDLDebugOverlay_Render(SDL_Renderer* renderer, float fps, bool show_fps, bool show_inputs);

#endif
