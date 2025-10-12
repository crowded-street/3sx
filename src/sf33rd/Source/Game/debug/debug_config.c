#include "sf33rd/Source/Game/debug/debug_config.h"

#if defined(DEBUG)
DebugConfig debug_config = { 0 };

void DebugConfig_Init() {
    // Modify debug_config to adjust gameplay
    // Example:
    // debug_config.player_1_invincible = true;
    // debug_config.player_2_one_hit_ko = true;
}
#endif
