#if DEBUG

#ifndef IMGUI_WRAPPER_H
#define IMGUI_WRAPPER_H

#include <SDL3/SDL.h>

void ImGuiW_Init(SDL_Window* window, SDL_Renderer* renderer);
void ImGuiW_Finish();
void ImGuiW_ProcessEvent(const SDL_Event* event);
void ImGuiW_BeginFrame();
void ImGuiW_EndFrame(SDL_Renderer* renderer);
void ImGuiW_ToggleVisivility();

#endif

#endif
