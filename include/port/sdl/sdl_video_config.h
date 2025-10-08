#ifndef SDL_VIDEO_CONFIG_H
#define SDL_VIDEO_CONFIG_H

#include <stdbool.h>

typedef struct SDLConfig SDLConfig;

typedef struct SDLVideoConfig {
    bool fullscreen;
    int window_width;
    int window_height;
} SDLVideoConfig;

// Initialize video config with defaults
void SDLVideoConfig_Init(SDLVideoConfig* config);

// Load video config from SDLConfig object
void SDLVideoConfig_LoadFromConfig(SDLVideoConfig* config, const SDLConfig* cfg);

// Save video config to SDLConfig object
void SDLVideoConfig_SaveToConfig(const SDLVideoConfig* config, SDLConfig* cfg);

#endif
