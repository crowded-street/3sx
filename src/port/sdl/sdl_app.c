#include "port/sdl/sdl_app.h"
#include "common.h"
#include "imgui/imgui_wrapper.h"
#include "port/config/config.h"
#include "port/config/keymap.h"
#include "port/sdl/netplay_screen.h"
#include "port/sdl/netstats_renderer.h"
#include "port/sdl/scanline_renderer.h"
#include "port/sdl/sdl_debug_text.h"
#include "port/sdl/sdl_game_renderer.h"
#include "port/sdl/sdl_message_renderer.h"
#include "port/sdl/sdl_pad.h"
#include "port/sound/adx.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"

#include <SDL3/SDL.h>

typedef enum ScaleMode {
    SCALEMODE_NEAREST,
    SCALEMODE_LINEAR,
    SCALEMODE_SOFT_LINEAR,
    SCALEMODE_SQUARE_PIXELS,
    SCALEMODE_INTEGER,
} ScaleMode;

static const char* app_name = "Street Fighter III: 3rd Strike";
static const float display_target_ratio = 4.0 / 3.0;
static const int window_min_width = 384;
static const int window_min_height = (int)(window_min_width / display_target_ratio);
static const Uint64 target_frame_time_ns = 1000000000.0 / TARGET_FPS;

SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* screen_texture = NULL;
static ScaleMode scale_mode = SCALEMODE_SOFT_LINEAR;

static Uint64 frame_deadline = 0;
static FrameMetrics frame_metrics = { 0 };
static Uint64 last_frame_end_time = 0;

static bool should_save_screenshot = false;
static Uint64 last_mouse_motion_time = 0;
static const int mouse_hide_delay_ms = 2000; // 2 seconds

static SDL_ScaleMode screen_texture_scale_mode() {
    switch (scale_mode) {
    case SCALEMODE_LINEAR:
    case SCALEMODE_SOFT_LINEAR:
        return SDL_SCALEMODE_LINEAR;

    case SCALEMODE_NEAREST:
    case SCALEMODE_SQUARE_PIXELS:
    case SCALEMODE_INTEGER:
        return SDL_SCALEMODE_NEAREST;
    default:
        return SDL_SCALEMODE_INVALID;
    }
}

static SDL_Point screen_texture_size() {
    SDL_Point size;
    SDL_GetRenderOutputSize(renderer, &size.x, &size.y);

    if (scale_mode == SCALEMODE_SOFT_LINEAR) {
        size.x *= 2;
        size.y *= 2;
    }

    return size;
}

static void create_screen_texture() {
    if (screen_texture != NULL) {
        SDL_DestroyTexture(screen_texture);
    }

    const SDL_Point size = screen_texture_size();
    screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
    SDL_SetTextureScaleMode(screen_texture, screen_texture_scale_mode());
}

static void init_scalemode() {
    const char* raw_scalemode = Config_GetString(CFG_KEY_SCALEMODE);

    if (raw_scalemode == NULL) {
        return;
    }

    if (SDL_strcmp(raw_scalemode, "nearest") == 0) {
        scale_mode = SCALEMODE_NEAREST;
    } else if (SDL_strcmp(raw_scalemode, "linear") == 0) {
        scale_mode = SCALEMODE_LINEAR;
    } else if (SDL_strcmp(raw_scalemode, "soft-linear") == 0) {
        scale_mode = SCALEMODE_SOFT_LINEAR;
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

    if (window_width < window_min_width) {
        window_width = window_min_width;
    }

    int window_height = Config_GetInt(CFG_KEY_WINDOW_HEIGHT);

    if (window_height < window_min_height) {
        window_height = window_min_height;
    }

    if (!SDL_CreateWindowAndRenderer(app_name, window_width, window_height, window_flags, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return true;
}

int SDLApp_PreInit() {
    SDL_SetAppMetadata(app_name, "0.1", NULL);
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

int SDLApp_FullInit() {
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

    // Initialize rendering subsystems
    SDLMessageRenderer_Initialize(renderer);
    SDLGameRenderer_Init(renderer);
    ScanlineRenderer_Init(renderer);

#if DEBUG
    SDLDebugText_Initialize(renderer);
#endif

    // Initialize screen texture
    create_screen_texture();

    // Initialize pads
    SDLPad_Init();

#if DEBUG
    ImGuiW_Init(window, renderer);
#endif

    return 0;
}

void SDLApp_Quit() {
    Config_Destroy();
    ScanlineRenderer_Destroy();

#if DEBUG
    ImGuiW_Finish();
#endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// static void set_screenshot_flag_if_needed(SDL_KeyboardEvent* event) {
//     if ((event->key == SDLK_GRAVE) && event->down && !event->repeat) {
//         should_save_screenshot = true;
//     }
// }

#if DEBUG
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

bool SDLApp_PollEvents() {
    SDL_Event event;
    bool continue_running = true;

    while (SDL_PollEvent(&event)) {
#if DEBUG
        ImGuiW_ProcessEvent(&event);
#endif

        switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMOVED:
            SDLPad_HandleGamepadDeviceEvent(&event.gdevice);
            break;

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            // set_screenshot_flag_if_needed(&event.key);

#if DEBUG
            toggle_debug_window_visibility(&event.key);
#endif

            handle_fullscreen_toggle(&event.key);
            break;

        case SDL_EVENT_MOUSE_MOTION:
            handle_mouse_motion();
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            create_screen_texture();
            break;

        case SDL_EVENT_QUIT:
            continue_running = false;
            break;
        }
    }

    return continue_running;
}

void SDLApp_BeginFrame() {
    // Clear window
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);

    SDLMessageRenderer_BeginFrame();
    SDLGameRenderer_BeginFrame();

#if DEBUG
    ImGuiW_BeginFrame();
#endif
}

static void center_rect(SDL_FRect* rect, int win_w, int win_h) {
    rect->x = (win_w - rect->w) / 2;
    rect->y = (win_h - rect->h) / 2;
}

static SDL_FRect fit_4_by_3_rect(int win_w, int win_h) {
    SDL_FRect rect;
    rect.w = win_w;
    rect.h = win_w / display_target_ratio;

    if (rect.h > win_h) {
        rect.h = win_h;
        rect.w = win_h * display_target_ratio;
    }

    center_rect(&rect, win_w, win_h);
    return rect;
}

static SDL_FRect fit_integer_rect(int win_w, int win_h, int pixel_w, int pixel_h) {
    const int virtual_w = win_w / pixel_w;
    const int virtual_h = win_h / pixel_h;
    const int scale_w = virtual_w / 384;
    const int scale_h = virtual_h / 224;
    int scale = (scale_h < scale_w) ? scale_h : scale_w;

    // Better to show a cropped image than nothing at all
    if (scale < 1) {
        scale = 1;
    }

    SDL_FRect rect;
    rect.w = scale * 384 * pixel_w;
    rect.h = scale * 224 * pixel_h;
    center_rect(&rect, win_w, win_h);
    return rect;
}

static SDL_FRect get_letterbox_rect(int win_w, int win_h) {
    switch (scale_mode) {
    case SCALEMODE_NEAREST:
    case SCALEMODE_LINEAR:
    case SCALEMODE_SOFT_LINEAR:
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

static void save_texture(SDL_Texture* texture, const char* filename) {
    SDL_SetRenderTarget(renderer, texture);
    const SDL_Surface* rendered_surface = SDL_RenderReadPixels(renderer, NULL);
    SDL_SaveBMP(rendered_surface, filename);
    SDL_DestroySurface(rendered_surface);
}

void SDLApp_EndFrame() {
    // Run sound processing
    ADX_ProcessTracks();

    // Render

    // This should come before SDLGameRenderer_RenderFrame,
    // because NetstatsRenderer uses the existing SFIII rendering pipeline
    NetplayScreen_Render();
    NetstatsRenderer_Render();
    SDLGameRenderer_RenderFrame();

    if (should_save_screenshot) {
        save_texture(cps3_canvas, "screenshot_cps3.bmp");
    }

    SDL_SetRenderTarget(renderer, screen_texture);

    // Render window background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black bars
    SDL_RenderClear(renderer);

    // Render content
    const SDL_FRect dst_rect = get_letterbox_rect(screen_texture->w, screen_texture->h);
    SDL_RenderTexture(renderer, cps3_canvas, NULL, &dst_rect);
    SDL_RenderTexture(renderer, message_canvas, NULL, &dst_rect);

    // Render screen texture to screen
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderTexture(renderer, screen_texture, NULL, NULL);

    // Apply scanlines using a cached overlay texture.
    int win_w, win_h;
    SDL_GetRenderOutputSize(renderer, &win_w, &win_h);
    const SDL_FRect game_rect = get_letterbox_rect(win_w, win_h);
    ScanlineRenderer_Render(&game_rect);

    if (should_save_screenshot) {
        save_texture(screen_texture, "screenshot_screen.bmp");
    }

#if DEBUG
    // Render debug text
    SDLDebugText_Render();

    ImGuiW_EndFrame(renderer);
#endif

    SDL_RenderPresent(renderer);

    // Cleanup
    SDLGameRenderer_EndFrame();
    should_save_screenshot = false;

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

void SDLApp_Exit() {
    SDL_Event quit_event;
    quit_event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&quit_event);
}

const FrameMetrics* SDLApp_GetFrameMetrics() {
    return &frame_metrics;
}
