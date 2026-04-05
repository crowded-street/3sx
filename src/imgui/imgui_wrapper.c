#if DEBUG

#include "imgui/imgui_wrapper.h"

#include "imgui/dcimgui/dcimgui.h"
#include "imgui/dcimgui/dcimgui_impl_sdl3.h"
#include "imgui/dcimgui/dcimgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>

// static bool show_imgui_demo = true;
static bool show_debug_window = true;

static void build_debug_window() {
    if (!show_debug_window) {
        return;
    }

    ImGui_Begin("Debug", &show_debug_window, 0);

    ImGui_Text("This is a debug window!");

    ImGui_End();
}

void ImGuiW_Init(SDL_Window* window, SDL_Renderer* renderer) {
    CIMGUI_CHECKVERSION();
    ImGui_CreateContext(NULL);

    ImGuiIO* io = ImGui_GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    const float main_scale = SDL_GetWindowDisplayScale(window);
    ImGui_StyleColorsDark(NULL);
    ImGuiStyle* style = ImGui_GetStyle();
    ImGuiStyle_ScaleAllSizes(style, main_scale);
    style->FontScaleDpi = main_scale;
    io->ConfigDpiScaleFonts = true;

    cImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    cImGui_ImplSDLRenderer3_Init(renderer);
}

void ImGuiW_Finish() {
    cImGui_ImplSDLRenderer3_Shutdown();
    cImGui_ImplSDL3_Shutdown();
    ImGui_DestroyContext(NULL);
}

void ImGuiW_ProcessEvent(const SDL_Event* event) {
    cImGui_ImplSDL3_ProcessEvent(event);
}

void ImGuiW_BeginFrame() {
    cImGui_ImplSDLRenderer3_NewFrame();
    cImGui_ImplSDL3_NewFrame();
    ImGui_NewFrame();
}

void ImGuiW_EndFrame(SDL_Renderer* renderer) {
    ImGuiIO* io = ImGui_GetIO();

    // ImGui_ShowDemoWindow(&show_imgui_demo);
    build_debug_window();
    ImGui_Render();

    SDL_SetRenderScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
    cImGui_ImplSDLRenderer3_RenderDrawData(ImGui_GetDrawData(), renderer);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

void ImGuiW_ToggleVisivility() {
    show_debug_window = !show_debug_window;
}

#endif
