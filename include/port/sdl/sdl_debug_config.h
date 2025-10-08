#ifndef SDL_DEBUG_CONFIG_H
#define SDL_DEBUG_CONFIG_H

#include <stdbool.h>

typedef struct SDLConfig SDLConfig;

typedef struct SDLDebugConfig {
    bool show_fps;
    bool show_inputs;
} SDLDebugConfig;

// Initialize debug config with defaults
void SDLDebugConfig_Init(SDLDebugConfig* config);

// Load debug config from SDLConfig object
void SDLDebugConfig_LoadFromConfig(SDLDebugConfig* config, const SDLConfig* cfg);

// Save debug config to SDLConfig object
void SDLDebugConfig_SaveToConfig(const SDLDebugConfig* config, SDLConfig* cfg);

#endif
