#if CRS_APP_DRIVER_ARM

#include "platform/app/arm/arm_app.h"
#include "arcade/arcade_balance.h"
#include "main.h"
#include "platform/app/arm/arm_display.h"
#include "platform/video/software/software_renderer.h"
#include "port/config/config.h"
#include "port/input_backend.h"
#include "port/io/afs.h"
#include "port/resources.h"
#include "port/sound/adx.h"

#include <SDL3/SDL.h>

#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// ARM Linux app path: SDL for input/audio, direct display for video.

#define TARGET_FPS 59.59949

typedef enum ArmAppPhase {
    ARM_APP_PHASE_INIT,
    ARM_APP_PHASE_COPYING_RESOURCES,
    ARM_APP_PHASE_INITIALIZED,
} ArmAppPhase;

static ArmAppPhase phase = ARM_APP_PHASE_INIT;
static volatile sig_atomic_t should_exit = 0;
static Uint64 frame_deadline = 0;
static const Uint64 target_frame_time_ns = (Uint64)(1000000000.0 / TARGET_FPS);

static void on_signal(int sig) {
    (void)sig;
    should_exit = 1;
}

static bool pre_init() {
    // Stay off X/Wayland.
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");

    // Keep SDL keyboard state alive without opening a real window.
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    Config_Init();
    return true;
}

static bool full_init() {
    if (!ArmDisplay_Init()) {
        return false;
    }

    int display_w = 0;
    int display_h = 0;
    ArmDisplay_GetResolution(&display_w, &display_h);
    SDL_Log("ARM display: %dx%d", display_w, display_h);

    // ARM stays on the base 384x224 canvas.
    if (!SoftwareRenderer_Init(true, 1)) {
        return false;
    }

    InputBackend_Init();
    AFS_Init(Resources_GetAFSPath(), 256 * 1024);
    ArcadeBalance_Init();
    Main_Init();

    return true;
}

static void cleanup() {
    AFS_Finish();
    Config_Destroy();
    SoftwareRenderer_Quit();
    ArmDisplay_Shutdown();
}

static bool poll_events() {
    if (should_exit) {
        return false;
    }

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMOVED:
            InputBackend_HandleGamepadDeviceEvent(&event.gdevice);
            break;

        case SDL_EVENT_QUIT:
            return false;
        }
    }

    return true;
}

static bool copying_resources_poll() {
    if (should_exit) {
        return false;
    }

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }
    }

    return true;
}

static void begin_frame() {
    AFS_RunServer();
}

static void end_frame() {
    ADX_ProcessTracks();

    SoftwareRenderer_RenderFrame();

    int canvas_w = 0;
    int canvas_h = 0;
    int canvas_pitch_bytes = 0;
    const uint32_t* canvas_pixels = SoftwareRenderer_GetCanvas(&canvas_w, &canvas_h, &canvas_pitch_bytes);
    ArmDisplay_Present(canvas_pixels, canvas_w, canvas_h);

    // DRM blocks on vblank; fbdev does not. Keep the same pacing either way.
    Uint64 now = SDL_GetTicksNS();

    if (frame_deadline == 0) {
        frame_deadline = now + target_frame_time_ns;
    }

    if (now < frame_deadline) {
        SDL_DelayNS(frame_deadline - now);
        now = SDL_GetTicksNS();
    }

    frame_deadline += target_frame_time_ns;

    if (now > frame_deadline + target_frame_time_ns) {
        frame_deadline = now + target_frame_time_ns;
    }
}

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    if (!pre_init()) {
        return 1;
    }

    bool is_running = true;

    while (is_running) {
        switch (phase) {
        case ARM_APP_PHASE_INIT:
            if (Resources_Check()) {
                if (!full_init()) {
                    cleanup();
                    SDL_Quit();
                    return 1;
                }

                phase = ARM_APP_PHASE_INITIALIZED;
            } else {
                SDL_Log("Required resources are missing at %s", Resources_GetAFSPath());
                SDL_Log("Copy SF33RD.AFS into the resources directory before running the ARM build.");
                is_running = false;
            }

            break;

        case ARM_APP_PHASE_COPYING_RESOURCES:
            is_running = copying_resources_poll();
            break;

        case ARM_APP_PHASE_INITIALIZED: {
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
    }

    cleanup();
    SDL_Quit();
    return 0;
}

void ArmApp_Exit() {
    SDL_Event quit_event;
    quit_event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&quit_event);
}

#endif // CRS_APP_DRIVER_ARM
