#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#if defined(DEBUG)

#include <stdbool.h>

typedef struct DebugConfig {
    bool player_1_invincible;
    bool player_2_invincible;
    bool player_1_one_hit_ko;
    bool player_2_one_hit_ko;
} DebugConfig;

extern DebugConfig debug_config;

void DebugConfig_Init();

#endif

#endif
