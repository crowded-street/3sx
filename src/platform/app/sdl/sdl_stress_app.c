#if CRS_APP_DRIVER_SDL && NETPLAY_ENABLED

#include "platform/app/sdl/sdl_stress_app.h"
#include "arcade/arcade_balance.h"
#include "args.h"
#include "main.h"
#include "platform/netplay/netplay_stress.h"
#include "port/config/config.h"
#include "port/io/afs.h"
#include "port/resources.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

static bool init() {
    Config_Init();

    if (!Resources_Check()) {
        SDL_Log("Missing or invalid resources: %s", Resources_GetAFSPath());
        return false;
    }

    ArcadeBalance_Init();

    if (!AFS_Init(Resources_GetAFSPath(), 256 * 1024)) {
        SDL_Log("Couldn't initialize AFS: %s", Resources_GetAFSPath());
        return false;
    }

    Main_Init();
    return true;
}

static void cleanup() {
    AFS_Finish();
    Config_Destroy();
}

int SDLStressApp_Run() {
    const NetplayArgs* netplay = &get_args()->netplay;

    Stress_SetOutputDir(netplay->stress_out);

    if (!init()) {
        Config_Destroy();
        return 1;
    }

    Stress_Begin(netplay->stress_seed, netplay->stress_check_distance, netplay->stress_frames);

    // No event loop to deliver a quit event, so the run ends on its own flag.
    while (!Stress_IsFinished()) {
        AFS_RunServer();
        Main_StepFrame();
        Main_FinishFrame();
    }

    cleanup();
    return 0;
}

#endif // CRS_APP_DRIVER_SDL && NETPLAY_ENABLED
