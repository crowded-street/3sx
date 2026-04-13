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
    AFS_Init(Resources_GetAFSPath());
    PSPRenderer_Init();
    Main_Init();
    return true;
}

static void poll_sdl_events() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_GAMEPAD_ADDED:
        case SDL_EVENT_GAMEPAD_REMOVED:
            InputBackend_HandleGamepadDeviceEvent(&event.gdevice);
            break;
        }
    }
}

static void begin_frame() {
    PSPRenderer_BeginFrame();
    AFS_RunServer();
}

static void end_frame() {
    PSPRenderer_RenderFrame();
    PSPRenderer_EndFrame();
}

int main() {
    if (!init()) {
        return 1;
    }

    while (true) {
        poll_sdl_events();
        begin_frame();
        Main_StepFrame();
        end_frame();
        Main_FinishFrame();
    }

    return 0;
}

#endif
