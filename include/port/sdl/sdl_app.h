#ifndef SDL_APP_H
#define SDL_APP_H

#include <SDL3/SDL.h>

extern SDL_Window* window;

int SDLApp_Init();
void SDLApp_Quit();

/// @brief Poll SDL events.
/// @return `true` if the main loop should continue running, `false` otherwise.
bool SDLApp_PollEvents();

void SDLApp_BeginFrame();
void SDLApp_EndFrame();
void SDLApp_Exit();

#endif
