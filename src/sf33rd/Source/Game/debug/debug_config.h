#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#if DEBUG

#include "types.h"

#include <stdbool.h>

typedef struct {
    bool one_button_sa;
    u8 time_stop;
    bool player_no_life[2];
    bool player_invincible[2];
    int character_override[2];
    int stage_override;
    u8 cpu_sa;
    bool turbo_buttons;
    int bonus_stage_override;
    bool skip_to_ending;
    u8 cpu_active_override;
    u8 cpu_passive_override;
} DebugConfig;

extern DebugConfig debug_config;

#endif // DEBUG

#endif // DEBUG_CONFIG_H
