#include "core/app.h"

#if CRS_APP_DRIVER_SDL
#include "platform/app/sdl/sdl_app.h"
#elif CRS_APP_DRIVER_PSP
#include "platform/app/psp/psp_app.h"
#endif

bool App_SupportsExit() {
#if CRS_APP_DRIVER_SDL || CRS_APP_DRIVER_PSP
    return true;
#else
    return false;
#endif
}

void App_Exit() {
#if CRS_APP_DRIVER_SDL
    SDLApp_Exit();
#elif CRS_APP_DRIVER_PSP
    PSPApp_Exit();
#endif
}
