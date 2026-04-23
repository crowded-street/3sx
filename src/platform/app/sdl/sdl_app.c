#if CRS_APP_DRIVER_SDL

#include "platform/app/sdl/sdl_app.h"
#include "arcade/arcade_balance.h"
#include "args.h"
#include "common.h"
#include "main.h"
#include "platform/video/sdl_generic/sdl_generic_renderer.h"
#include "port/config/config.h"
#include "port/config/keymap.h"
#include "port/sdl/sdl_debug_text.h"
#include "port/sdl/sdl_message_renderer.h"
#include "port/sound/adx.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#if CRS_INPUT_DRIVER_SDL
#include "platform/input/sdl/sdl_pad.h"
#endif

#if NETPLAY_ENABLED
#include "port/sdl/netplay_screen.h"
#include "port/sdl/netstats_renderer.h"
#endif

#if DEBUG
#include "sf33rd/Source/Game/debug/debug_config.h"
#endif

#if DEBUG && IMGUI
#include "imgui/imgui_wrapper.h"
#endif

#if STATCHECK
#include "test/test_runner.h"
#endif

#include "port/io/afs.h"
#include "port/resources.h"

#include <SDL3/SDL.h>

#if _WIN32 && DEBUG
#include <windef.h>

#include <ConsoleApi.h>
#include <stdio.h>
#endif

typedef enum ScaleMode {
    SCALEMODE_NEAREST,
    SCALEMODE_SQUARE_PIXELS,
    SCALEMODE_INTEGER,
} ScaleMode;

typedef enum AppPhase {
    APP_PHASE_INIT,
    APP_PHASE_COPYING_RESOURCES,
    APP_PHASE_INITIALIZED,
} AppPhase;

static AppPhase phase = APP_PHASE_INIT;

static const char* app_name = "Street Fighter III: 3rd Strike";
static const float display_target_ratio = 4.0 / 3.0;
static const int window_min_width = 384;
static const int window_min_height = (int)(window_min_width / display_target_ratio);
static const Uint64 target_frame_time_ns = 1000000000.0 / TARGET_FPS;

static SDL_Window* window = NULL;
static ScaleMode scale_mode = SCALEMODE_NEAREST;

static Uint64 frame_deadline = 0;
static FrameMetrics frame_metrics = { 0 };
static Uint64 last_frame_end_time = 0;

static Uint64 last_mouse_motion_time = 0;
static const int mouse_hide_delay_ms = 2000; // 2 seconds

static void init_scalemode() {
    const char* raw_scalemode = Config_GetString(CFG_KEY_SCALEMODE);

    if (raw_scalemode == NULL) {
        return;
    }

    if (SDL_strcmp(raw_scalemode, "nearest") == 0) {
        scale_mode = SCALEMODE_NEAREST;
    } else if (SDL_strcmp(raw_scalemode, "square-pixels") == 0) {
        scale_mode = SCALEMODE_SQUARE_PIXELS;
    } else if (SDL_strcmp(raw_scalemode, "integer") == 0) {
        scale_mode = SCALEMODE_INTEGER;
    }
}

static bool init_window() {
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

    if (Config_GetBool(CFG_KEY_FULLSCREEN)) {
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }

    int window_width = Config_GetInt(CFG_KEY_WINDOW_WIDTH);
    window_width = SDL_max(window_width, window_min_width);

    int window_height = Config_GetInt(CFG_KEY_WINDOW_HEIGHT);
    window_height = SDL_max(window_height, window_min_height);

    window = SDLGenericRenderer_Init(&(SDLRenderBackendInitInfo) {
        .app_name = app_name,
        .window_width = window_width,
        .window_height = window_height,
        .window_flags = window_flags,
    });

    if (window == NULL) {
        return false;
    }

    return true;
}

static int pre_init() {
    SDL_SetAppMetadata(app_name, "0.1", NULL);
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

#if _WIN32 && DEBUG
static void init_windows_console() {
    // attaches to an existing console for printouts. Works with windows CMD but not MSYS2
    if (AttachConsole(ATTACH_PARENT_PROCESS) == 0) {
        // if fails, then allocate a new console
        AllocConsole();
    }
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}
#endif

static int full_init() {
    Config_Init();
    Keymap_Init();
    init_scalemode();

    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (!init_window()) {
        SDL_Log("Couldn't initialize SDL window: %s", SDL_GetError());
        return 1;
    }

// #if DEBUG
//     SDLDebugText_Initialize(renderer);
// #endif

// Initialize pads
#if CRS_INPUT_DRIVER_SDL
    SDLPad_Init();
#endif

#if _WIN32 && DEBUG
    init_windows_console();
#endif

    ArcadeBalance_Init();
    AFS_Init(Resources_GetAFSPath(), 256 * 1024);

#if DEBUG
    DebugConfig_Init();
#endif

    Main_Init();
    return 0;
}

static void cleanup() {
    AFS_Finish();
    Config_Destroy();
    SDLGenericRenderer_Quit();
}

#if DEBUG && IMGUI
static void toggle_debug_window_visibility(SDL_KeyboardEvent* event) {
    if ((event->key == SDLK_GRAVE) && event->down && !event->repeat) {
        ImGuiW_ToggleVisivility();
    }
}
#endif

static void handle_fullscreen_toggle(SDL_KeyboardEvent* event) {
    const bool is_alt_enter = (event->key == SDLK_RETURN) && (event->mod & SDL_KMOD_ALT);
    const bool is_f11 = (event->key == SDLK_F11);
    const bool correct_key = (is_alt_enter || is_f11);

    if (!correct_key || !event->down || event->repeat) {
        return;
    }

    const SDL_WindowFlags flags = SDL_GetWindowFlags(window);

    if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_SetWindowFullscreen(window, false);
    } else {
        SDL_SetWindowFullscreen(window, true);
    }
}

static void handle_mouse_motion() {
    last_mouse_motion_time = SDL_GetTicks();
    SDL_ShowCursor();
}

static void hide_cursor_if_needed() {
    const Uint64 now = SDL_GetTicks();

    if ((last_mouse_motion_time > 0) && ((now - last_mouse_motion_time) > mouse_hide_delay_ms)) {
        SDL_HideCursor();
    }
}

static bool poll_events() {
    SDL_Event event;
    bool continue_running = true;

    while (SDL_PollEvent(&event)) {
#if DEBUG && IMGUI
        ImGuiW_ProcessEvent(&event);
#endif

        switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMOVED:
#if CRS_INPUT_DRIVER_SDL
            SDLPad_HandleGamepadDeviceEvent(&event.gdevice);
#endif
            break;

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
#if DEBUG && IMGUI
            toggle_debug_window_visibility(&event.key);
#endif

            handle_fullscreen_toggle(&event.key);
            break;

        case SDL_EVENT_MOUSE_MOTION:
            handle_mouse_motion();
            break;

        case SDL_EVENT_QUIT:
            continue_running = false;
            break;
        }
    }

    return continue_running;
}

static void begin_frame() {
#if STATCHECK
    TestRunner_Prologue();
#endif

#if DEBUG && IMGUI
    ImGuiW_NewFrame();
#endif

    AFS_RunServer();
}

static void center_rect(SDL_Rect* rect, int win_w, int win_h) {
    rect->x = (win_w - rect->w) / 2;
    rect->y = (win_h - rect->h) / 2;
}

static SDL_Rect fit_4_by_3_rect(int win_w, int win_h) {
    SDL_Rect rect;
    rect.w = win_w;
    rect.h = (int)((float)win_w / display_target_ratio);

    if (rect.h > win_h) {
        rect.h = win_h;
        rect.w = (int)((float)win_h * display_target_ratio);
    }

    center_rect(&rect, win_w, win_h);
    return rect;
}

static SDL_Rect fit_integer_rect(int win_w, int win_h, int pixel_w, int pixel_h) {
    const int virtual_w = win_w / pixel_w;
    const int virtual_h = win_h / pixel_h;
    const int scale_w = virtual_w / 384;
    const int scale_h = virtual_h / 224;
    int scale = (scale_h < scale_w) ? scale_h : scale_w;

    // Better to show a cropped image than nothing at all
    if (scale < 1) {
        scale = 1;
    }

    SDL_Rect rect;
    rect.w = scale * 384 * pixel_w;
    rect.h = scale * 224 * pixel_h;
    center_rect(&rect, win_w, win_h);
    return rect;
}

static SDL_Rect get_letterbox_rect(int win_w, int win_h) {
    switch (scale_mode) {
    case SCALEMODE_NEAREST:
        return fit_4_by_3_rect(win_w, win_h);

    case SCALEMODE_INTEGER:
        // In order to scale a 384x224 buffer to 4:3 we need to stretch the image vertically by 9 / 7
        return fit_integer_rect(win_w, win_h, 7, 9);

    case SCALEMODE_SQUARE_PIXELS:
        return fit_integer_rect(win_w, win_h, 1, 1);

    default:
        return fit_4_by_3_rect(win_w, win_h);
    }
}

static void update_metrics(Uint64 sleep_time) {
    const Uint64 new_frame_end_time = SDL_GetTicksNS();
    const Uint64 frame_time = new_frame_end_time - last_frame_end_time;
    const float frame_time_ms = (float)frame_time / 1e6;

    frame_metrics.frame_time[frame_metrics.head] = frame_time_ms;
    frame_metrics.idle_time[frame_metrics.head] = (float)sleep_time / 1e6;
    frame_metrics.fps[frame_metrics.head] = 1000 / frame_time_ms;

    frame_metrics.head = (frame_metrics.head + 1) % SDL_arraysize(frame_metrics.frame_time);
    last_frame_end_time = new_frame_end_time;
}

static void end_frame() {
#if STATCHECK
    TestRunner_Epilogue();
#endif

    // Run sound processing
    ADX_ProcessTracks();

    // Render

#if NETPLAY_ENABLED
    // This should come before SDLGameRenderer_RenderFrame,
    // because NetstatsRenderer uses the existing SFIII rendering pipeline
    NetplayScreen_Render();
    NetstatsRenderer_Render();
#endif

#if DEBUG
    // Render debug text
    // SDLDebugText_Render();
#endif

    int window_width;
    int window_height;
    SDL_GetWindowSizeInPixels(window, &window_width, &window_height);
    SDLGenericRenderer_RenderFrame(get_letterbox_rect(window_width, window_height));

    // Handle cursor hiding
    hide_cursor_if_needed();

    // Do frame pacing
    Uint64 now = SDL_GetTicksNS();

    if (frame_deadline == 0) {
        frame_deadline = now + target_frame_time_ns;
    }

    Uint64 sleep_time = 0;

    if (now < frame_deadline) {
        sleep_time = frame_deadline - now;
        SDL_DelayNS(sleep_time);
        now = SDL_GetTicksNS();
    }

    frame_deadline += target_frame_time_ns;

    // If we fell behind by more than one frame, resync to avoid spiraling
    if (now > frame_deadline + target_frame_time_ns) {
        frame_deadline = now + target_frame_time_ns;
    }

    // Measure
    update_metrics(sleep_time);
}

// Entrypoint

static bool sdl_poll_helper() {
    SDL_Event event;
    bool continue_running = true;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            continue_running = false;
        }
    }

    return continue_running;
}

static int loop() {
    bool is_running = true;

    while (is_running) {
        switch (phase) {
        case APP_PHASE_INIT:
            pre_init();

            if (Resources_Check()) {
                const int init_status = full_init();

                if (init_status != 0) {
                    is_running = false;
                    break;
                }

                phase = APP_PHASE_INITIALIZED;
            } else {
                phase = APP_PHASE_COPYING_RESOURCES;
            }

            break;

        case APP_PHASE_COPYING_RESOURCES:
            is_running = sdl_poll_helper();

            if (!is_running) {
                break;
            }

            SDL_Delay(16);

            const bool resource_flow_ended = Resources_RunResourceCopyingFlow();

            if (resource_flow_ended) {
                const int init_status = full_init();

                if (init_status != 0) {
                    is_running = false;
                    break;
                }

                phase = APP_PHASE_INITIALIZED;
            }

            break;

        case APP_PHASE_INITIALIZED:
            is_running = poll_events();

            if (!is_running) {
                break;
            }

            begin_frame();
            Main_StepFrame();
            end_frame();
            Main_FinishFrame();
            break;
        }
    }

    cleanup();
    SDL_Quit();
    return 0;
}

int main(int argc, const char* argv[]) {
    init_args(argc, argv);
    return loop();
}

// Public API

const FrameMetrics* SDLApp_GetFrameMetrics() {
    return &frame_metrics;
}

void SDLApp_Exit() {
    SDL_Event quit_event;
    quit_event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&quit_event);
}

#endif // CRS_APP_DRIVER_SDL
