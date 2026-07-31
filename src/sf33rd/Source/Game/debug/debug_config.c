#include "sf33rd/Source/Game/debug/debug_config.h"
#include "common.h"

#if DEBUG

// Default values for all debug options (from original NAKAI_debug_data)
static const s8 debug_defaults[DEBUG_OPTION_COUNT] = { 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 1,
                                                       0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

DebugConfig debug_config = { 0 };

void DebugConfig_Init() {
    // Initialize all debug values from defaults
    for (int i = 0; i < DEBUG_OPTION_COUNT; i++) {
        debug_config.values[i] = debug_defaults[i];
    }

    // Example customizations (uncomment to enable):
    // debug_config.values[DEBUG_PLAYER_1_NO_LIFE] = 1;
    // debug_config.values[DEBUG_PLAYER_2_INVINCIBLE] = 1;
}

s8 DebugConfig_Get(DebugOption option) {
    if (option >= DEBUG_OPTION_COUNT) {
        return 0;
    }
    return debug_config.values[option];
}

#endif // DEBUG
