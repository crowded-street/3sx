#if CRS_APP_DRIVER_PSP

#include "main.h"
#include "platform/video/psp/psp_renderer.h"
#include "port/input_backend.h"
#include "port/io/afs.h"
#include "port/resources.h"

#include <SDL3/SDL.h>
#include <pspuser.h>

#include <stdbool.h>

PSP_MODULE_INFO("3SX", PSP_MODULE_USER, 0, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);
PSP_HEAP_THRESHOLD_SIZE_KB(1024);

static bool init() {
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    InputBackend_Init();
    AFS_Init(Resources_GetAFSPath(), 16 * 1024);
    PSPRenderer_Init();
    Main_Init();
    return true;
}

static bool poll_sdl_events() {
    SDL_Event event;
    bool continue_running = true;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMOVED:
            InputBackend_HandleGamepadDeviceEvent(&event.gdevice);
            break;

        case SDL_EVENT_QUIT:
            continue_running = false;
            break;
        }
    }

    return continue_running;
}

static void begin_frame() {
    PSPRenderer_BeginFrame();
    AFS_RunServer();
}

static void end_frame() {
    PSPRenderer_RenderFrame();
    PSPRenderer_EndFrame();
}

void PSPApp_Exit() {
    SDL_Event quit_event;
    quit_event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&quit_event);
}

int main() {
    if (!init()) {
        return 1;
    }

    bool is_running = true;

    while (is_running) {
        is_running = poll_sdl_events();

        if (!is_running) {
            break;
        }

        begin_frame();
        Main_StepFrame();
        end_frame();
        Main_FinishFrame();
    }

    return 0;
}

#endif // CRS_APP_DRIVER_PSP
