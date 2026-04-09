#include "port/platform_app.h"

#include "port/sdl/sdl_app.h"

int PlatformApp_PreInit(void) {
    return SDLApp_PreInit();
}

int PlatformApp_FullInit(void) {
    return SDLApp_FullInit();
}

void PlatformApp_Quit(void) {
    SDLApp_Quit();
}

bool PlatformApp_PollEvents(void) {
    return SDLApp_PollEvents();
}

void PlatformApp_BeginFrame(void) {
    SDLApp_BeginFrame();
}

void PlatformApp_EndFrame(void) {
    SDLApp_EndFrame();
}

void PlatformApp_Exit(void) {
    SDLApp_Exit();
}
