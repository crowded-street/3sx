#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#if DEBUG

#include "types.h"

#include <stdbool.h>

typedef enum DebugOption {
    DEBUG_1SHOT_SA = 9,
    DEBUG_TIME_STOP = 24,
    DEBUG_PLAYER_1_NO_LIFE = 25,
    DEBUG_PLAYER_2_NO_LIFE = 26,
    DEBUG_PLAYER_1_INVINCIBLE = 27,
    DEBUG_PLAYER_2_INVINCIBLE = 28,
    DEBUG_MY_CHAR_PL1 = 29,
    DEBUG_MY_CHAR_PL2 = 30,
    DEBUG_STAGE_SELECT = 31,
    DEBUG_CPU_SA = 32,
    DEBUG_AUTO_RAPID_SHOT = 43,
    DEBUG_BONUS_CHECK = 46,
    DEBUG_ENDING_CHECK = 47,
    DEBUG_ACTIVE_NO = 54,
    DEBUG_PASSIVE_NO = 55,
    DEBUG_OPTION_COUNT = 72
} DebugOption;

typedef struct {
    bool one_button_sa;
    u8 time_stop;
    bool player_no_life[2];
    bool player_invincible[2];
    u8 character_override[2];
    u8 stage_override;
    u8 cpu_sa;
    bool turbo_buttons;
    u8 bonus_stage_override;
    bool skip_to_ending;
    u8 cpu_active_override;
    u8 cpu_passive_override;
} DebugConfig;

extern DebugConfig debug_config;
extern u8 Debug_w[DEBUG_OPTION_COUNT];

#endif // DEBUG

#endif // DEBUG_CONFIG_H
