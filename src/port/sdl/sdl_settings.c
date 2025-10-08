#include "port/sdl/sdl_settings.h"
#include "port/sdl/sdl_config.h"

void SDLSettings_Init(SDLSettings* settings) {
    // Initialize input settings with defaults
    SDLInputConfig_Init(&settings->input);

    // Initialize video settings with defaults
    SDLVideoConfig_Init(&settings->video);

    // Initialize debug settings with defaults
    SDLDebugConfig_Init(&settings->debug);
}

bool SDLSettings_Load(SDLSettings* settings, const char* filepath) {
    SDLConfig* cfg = SDLConfig_Create();
    if (!cfg) {
        return false;
    }

    // Try to load the file
    bool file_exists = SDLConfig_LoadFile(cfg, filepath);

    if (file_exists) {
        // File exists - initialize with defaults first
        SDLSettings_Init(settings);

        // Then load settings from config, overriding defaults
        SDLInputConfig_LoadFromConfig(&settings->input, cfg);

        // Load video config
        SDLVideoConfig_LoadFromConfig(&settings->video, cfg);

        // Load debug config
        SDLDebugConfig_LoadFromConfig(&settings->debug, cfg);

        SDLConfig_Destroy(cfg);

        // Save the config to write any new default settings that weren't in the file
        SDLSettings_Save(settings, filepath);

        return true;
    } else {
        // File doesn't exist - create it with defaults
        SDLConfig_Destroy(cfg);

        // Initialize with defaults
        SDLSettings_Init(settings);

        // Save default config
        SDLSettings_Save(settings, filepath);

        return false; // Indicate that we created a new file
    }
}

bool SDLSettings_Save(const SDLSettings* settings, const char* filepath) {
    SDLConfig* cfg = SDLConfig_Create();
    if (!cfg) {
        return false;
    }

    // Load existing config to preserve any manual edits or comments
    SDLConfig_LoadFile(cfg, filepath);

    // Save video config
    SDLVideoConfig_SaveToConfig(&settings->video, cfg);

    // Save input config (creates gamepad and keyboard sections)
    SDLInputConfig_SaveToConfig(&settings->input, cfg);

    // Save debug config
    SDLDebugConfig_SaveToConfig(&settings->debug, cfg);

    // Write config to file
    bool success = SDLConfig_SaveFile(cfg, filepath);
    SDLConfig_Destroy(cfg);
    return success;
}
