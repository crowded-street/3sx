#if DEBUG && IMGUI

#ifndef IMGUI_WRAPPER_H
#define IMGUI_WRAPPER_H

#include "imgui/dcimgui/dcimgui_impl_sdlgpu3.h"
#include <SDL3/SDL.h>

void ImGuiW_Init(SDL_Window* window, ImGui_ImplSDLGPU3_InitInfo* init_info);
void ImGuiW_Finish();
void ImGuiW_ProcessEvent(const SDL_Event* event);
void ImGuiW_NewFrame();
void ImGuiW_PrepareDrawData(SDL_GPUCommandBuffer* command_buffer);
void ImGuiW_RenderDrawData(SDL_GPUCommandBuffer* command_buffer, SDL_GPURenderPass* render_pass);
void ImGuiW_ToggleVisivility();

#endif

#endif
