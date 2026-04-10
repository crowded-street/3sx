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

const FrameMetrics* SDLApp_GetFrameMetrics();
void SDLApp_Exit();

#endif
