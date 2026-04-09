#ifndef SDL_DEBUG_TEXT_H
#define SDL_DEBUG_TEXT_H

#include <SDL3/SDL.h>

/// @brief Initialize the debug text renderer
/// @param renderer SDL renderer to use for debug text
void SDLDebugText_Initialize(SDL_Renderer* renderer);

/// @brief Render debug text buffer to screen
/// Called each frame to display debug text from flPrintL calls.
void SDLDebugText_Render();

/// @brief Destroy the debug text renderer
void SDLDebugText_Destroy();

#endif // SDL_DEBUG_TEXT_H
