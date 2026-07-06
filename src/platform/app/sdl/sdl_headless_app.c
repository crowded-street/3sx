#if CRS_APP_DRIVER_SDL && STATCHECK

#include "platform/app/sdl/sdl_headless_app.h"
#include "arcade/arcade_balance.h"
#include "main.h"
#include "port/config/config.h"
#include "port/io/afs.h"
#include "port/resources.h"
#include "test/test_runner.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

static int init() {
    Config_Init();

    if (!Resources_Check()) {
        SDL_Log("Missing or invalid resources: %s", Resources_GetAFSPath());
        return 1;
    }

    ArcadeBalance_Init();

    if (!AFS_Init(Resources_GetAFSPath(), 256 * 1024)) {
        SDL_Log("Couldn't initialize AFS: %s", Resources_GetAFSPath());
        return 1;
    }

    Main_Init();
    return 0;
}

static void cleanup() {
    AFS_Finish();
    Config_Destroy();
}

static void begin_frame() {
    TestRunner_Prologue();
    AFS_RunServer();
}

static void end_frame() {
    TestRunner_Epilogue();
}

int SDLHeadlessApp_Run() {
    if (init() != 0) {
        Config_Destroy();
        return 1;
    }

    while (true) {
        begin_frame();
        Main_StepFrame();
        end_frame();
        Main_FinishFrame();
    }

    cleanup();
    return 0;
}

#endif // CRS_APP_DRIVER_SDL && STATCHECK
