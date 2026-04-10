#if DEBUG

#include "imgui/imgui_wrapper.h"
#include "platform/app/sdl/sdl_app.h"
#include "port/paths.h"
#include "sf33rd/Source/Game/debug/debug_config.h"

#include "imgui/dcimgui/dcimgui.h"
#include "imgui/dcimgui/dcimgui_impl_sdl3.h"
#include "imgui/dcimgui/dcimgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>

// static bool show_imgui_demo = true;
static bool show_debug_window = false;
static char* imgui_ini_path = NULL;

static void plot(const char* label, const float* values, int value_count, int values_offset, ImVec2 scale) {
    const int last_index = (values_offset + value_count - 1) % value_count;
    const float last_value = values[last_index];
    const char overlay[128];
    SDL_snprintf(overlay, sizeof(overlay), "%.02f", last_value);

    ImGui_PlotLinesEx(
        label, values, value_count, values_offset, overlay, scale.x, scale.y, (ImVec2) { 0, 80 }, sizeof(float));
}

static void build_debug_window() {
    if (!show_debug_window) {
        return;
    }

    const FrameMetrics* frame_metrics = SDLApp_GetFrameMetrics();

    ImGui_Begin("Debug", &show_debug_window, 0);

    if (ImGui_CollapsingHeader("Frame metrics", 0)) {
        plot("FPS", frame_metrics->fps, SDL_arraysize(frame_metrics->fps), frame_metrics->head, (ImVec2) { 0, 60 });

        plot("Frame time",
             frame_metrics->frame_time,
             SDL_arraysize(frame_metrics->frame_time),
             frame_metrics->head,
             (ImVec2) { 0, 30 });

        plot("Idle time",
             frame_metrics->idle_time,
             SDL_arraysize(frame_metrics->idle_time),
             frame_metrics->head,
             (ImVec2) { 0, 20 });
    }

    if (ImGui_CollapsingHeader("Debug config", 0)) {
        ImGui_AlignTextToFramePadding();
        ImGui_Text("Invincibility:");
        ImGui_SameLine();
        ImGui_Checkbox("P1##invincibility", &debug_config.values[DEBUG_PLAYER_1_INVINCIBLE]);
        ImGui_SameLine();
        ImGui_Checkbox("P2##invincibility", &debug_config.values[DEBUG_PLAYER_2_INVINCIBLE]);

        ImGui_AlignTextToFramePadding();
        ImGui_Text("No life:");
        ImGui_SameLine();
        ImGui_Checkbox("P1##nolife", &debug_config.values[DEBUG_PLAYER_1_NO_LIFE]);
        ImGui_SameLine();
        ImGui_Checkbox("P2##nolife", &debug_config.values[DEBUG_PLAYER_2_NO_LIFE]);
    }

    ImGui_End();
}

void ImGuiW_Init(SDL_Window* window, SDL_Renderer* renderer) {
    CIMGUI_CHECKVERSION();
    ImGui_CreateContext(NULL);

    ImGuiIO* io = ImGui_GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SDL_asprintf(&imgui_ini_path, "%s/imgui.ini", Paths_GetPrefPath());
    io->IniFilename = imgui_ini_path;

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
