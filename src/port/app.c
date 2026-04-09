#include "port/app.h"

#include "port/sdl/sdl_app.h"

int App_PreInit() {
    return SDLApp_PreInit();
}

int App_FullInit() {
    return SDLApp_FullInit();
}

void App_Quit() {
    SDLApp_Quit();
}

bool App_PollEvents() {
    return SDLApp_PollEvents();
}

void App_BeginFrame() {
    SDLApp_BeginFrame();
}

void App_EndFrame() {
    SDLApp_EndFrame();
}

void App_Exit() {
    SDLApp_Exit();
}
