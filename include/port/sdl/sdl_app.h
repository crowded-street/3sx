#ifndef SDL_APP_H
#define SDL_APP_H

#include <SDL3/SDL.h>

#define TARGET_FPS 59.59949
#define FRAME_METRICS_COUNT 240

typedef struct FrameMetrics {
    size_t head;
    float fps[FRAME_METRICS_COUNT];
    float frame_time[FRAME_METRICS_COUNT];
    float idle_time[FRAME_METRICS_COUNT];
} FrameMetrics;

int SDLApp_PreInit();
int SDLApp_FullInit();
void SDLApp_Quit();

/// @brief Poll SDL events.
/// @return `true` if the main loop should continue running, `false` otherwise.
bool SDLApp_PollEvents();

void SDLApp_BeginFrame();
void SDLApp_EndFrame();
void SDLApp_Exit();

const FrameMetrics* SDLApp_GetFrameMetrics();

#endif
