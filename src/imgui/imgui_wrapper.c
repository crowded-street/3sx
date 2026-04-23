#if DEBUG && IMGUI

#include "imgui/imgui_wrapper.h"
#include "platform/app/sdl/sdl_app.h"
#include "port/paths.h"
#include "sf33rd/Source/Game/debug/debug_config.h"

#include "imgui/dcimgui/dcimgui.h"
#include "imgui/dcimgui/dcimgui_impl_sdl3.h"
#include "imgui/dcimgui/dcimgui_impl_sdlgpu3.h"
#include <SDL3/SDL.h>

static bool initialized = false;
// static bool show_imgui_demo = true;
static bool show_debug_window = false;
static char* imgui_ini_path = NULL;

static ImDrawData* draw_data = NULL;

static void plot(const char* label, const float* values, int value_count, int values_offset, ImVec2 scale) {
    const int last_index = (values_offset + value_count - 1) % value_count;
    const float last_value = values[last_index];
    const char overlay[128] = { 0 };
    SDL_snprintf(overlay, sizeof(overlay), "%.02f", last_value);

    ImGui_PlotLinesEx(
        label, values, value_count, values_offset, overlay, scale.x, scale.y, (ImVec2) { 0, 80 }, sizeof(float)
    );
}

static void build_debug_window() {
    if (!show_debug_window) {
        return;
    }

    const FrameMetrics* frame_metrics = SDLApp_GetFrameMetrics();

    ImGui_Begin("Debug", &show_debug_window, 0);

    if (ImGui_CollapsingHeader("Frame metrics", 0)) {
        plot("FPS", frame_metrics->fps, SDL_arraysize(frame_metrics->fps), frame_metrics->head, (ImVec2) { 0, 60 });

        plot(
            "Frame time",
            frame_metrics->frame_time,
            SDL_arraysize(frame_metrics->frame_time),
            frame_metrics->head,
            (ImVec2) { 0, 30 }
        );

        plot(
            "Idle time",
            frame_metrics->idle_time,
            SDL_arraysize(frame_metrics->idle_time),
            frame_metrics->head,
            (ImVec2) { 0, 20 }
        );
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

void ImGuiW_Init(SDL_Window* window, ImGui_ImplSDLGPU3_InitInfo* init_info) {
    CIMGUI_CHECKVERSION();
    ImGui_CreateContext(NULL);

    ImGuiIO* io = ImGui_GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    SDL_asprintf(&imgui_ini_path, "%s/imgui.ini", Paths_GetPrefPath());
    io->IniFilename = imgui_ini_path;

    const float main_scale = SDL_GetWindowDisplayScale(window);
    ImGui_StyleColorsDark(NULL);
    ImGuiStyle* style = ImGui_GetStyle();
    ImGuiStyle_ScaleAllSizes(style, main_scale);
    style->FontScaleDpi = main_scale;
    io->ConfigDpiScaleFonts = true;

    cImGui_ImplSDL3_InitForSDLGPU(window);
    cImGui_ImplSDLGPU3_Init(init_info);
    initialized = true;
}

void ImGuiW_Finish() {
    if (!initialized) {
        return;
    }

    cImGui_ImplSDLGPU3_Shutdown();
    cImGui_ImplSDL3_Shutdown();
    ImGui_DestroyContext(NULL);
    initialized = false;
}

void ImGuiW_ProcessEvent(const SDL_Event* event) {
    if (!initialized) {
        return;
    }

    cImGui_ImplSDL3_ProcessEvent(event);
}

void ImGuiW_NewFrame() {
    if (!initialized) {
        return;
    }

    cImGui_ImplSDLGPU3_NewFrame();
    cImGui_ImplSDL3_NewFrame();
    ImGui_NewFrame();

    ImGui_DockSpaceOverViewportEx(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode, NULL);
}

void ImGuiW_PrepareDrawData(SDL_GPUCommandBuffer* command_buffer) {
    if (!initialized) {
        return;
    }

    // ImGui_ShowDemoWindow(&show_imgui_demo);
    build_debug_window();
    ImGui_Render();

    draw_data = ImGui_GetDrawData();
    cImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);
}

void ImGuiW_RenderDrawData(SDL_GPUCommandBuffer* command_buffer, SDL_GPURenderPass* render_pass) {
    if (!initialized) {
        return;
    }

    cImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);
}

void ImGuiW_ToggleVisivility() {
    if (!initialized) {
        return;
    }

    show_debug_window = !show_debug_window;
}

#endif
