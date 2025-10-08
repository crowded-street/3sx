#ifndef SDL_SETTINGS_H
#define SDL_SETTINGS_H

#include "port/sdl/sdl_input_config.h"
#include "port/sdl/sdl_video_config.h"
#include "port/sdl/sdl_debug_config.h"
#include <stdbool.h>

// Centralized settings manager for all SDL subsystems
// Manages config.ini file and provides settings to all subsystems

typedef struct SDLSettings {
    SDLInputConfig input;
    SDLVideoConfig video;
    SDLDebugConfig debug;
} SDLSettings;

// Initialize settings with defaults
void SDLSettings_Init(SDLSettings* settings);

// Load settings from config.ini (creates default file if missing)
// Returns true if loaded successfully, false if created new default file
bool SDLSettings_Load(SDLSettings* settings, const char* filepath);

// Save settings to config.ini
bool SDLSettings_Save(const SDLSettings* settings, const char* filepath);

#endif
