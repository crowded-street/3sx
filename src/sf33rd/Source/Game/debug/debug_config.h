#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#include "types.h"

/// @brief Debug option string data structure
///
/// Associates a debug option name with its maximum value.
typedef struct {
    u8 max;           ///< Maximum value for this debug option
    const char* name; ///< Name string for the debug option
} DEBUG_STR_DAT;

/// @brief Debug configuration options
///
/// Enumeration of all available debug options that can be toggled or adjusted.
typedef enum DebugOption {
    DEBUG_SLOW = 0,
    DEBUG_FAST = 1,
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

/// @brief Debug configuration state
///
/// Stores the current values for all debug options.
typedef struct {
    s8 values[DEBUG_OPTION_COUNT];
} DebugConfig;

#if DEBUG

/// @brief Global debug configuration state (only available in debug builds)
extern DebugConfig debug_config;

/// @brief Debug option metadata array containing names and max values
extern const DEBUG_STR_DAT debug_string_data[DEBUG_OPTION_COUNT];

/// @brief Initialize debug configuration system
///
/// Sets all debug options to their default values.
/// Must be called before using the debug system.
void DebugConfig_Init();

/// @brief Get the current value of a debug option
///
/// @param option The debug option to query
/// @return The current value of the specified option
s8 DebugConfig_Get(DebugOption option);

/// @brief Set the value of a debug option
///
/// @param option The debug option to modify
/// @param value The new value to set (should be within option's max range)
void DebugConfig_Set(DebugOption option, s8 value);

/// @brief Backward compatibility macro for legacy debug array access
///
/// Allows existing code to use Debug_w[index] syntax while internally
/// accessing the new debug_config.values array.
///
/// @todo Replace all Debug_w[index] usage with DebugConfig_Get/Set
#define Debug_w debug_config.values

#else // !DEBUG

/// @brief Stub debug array for release builds
///
/// Exists for compatibility with code that accesses Debug_w outside of DEBUG blocks.
/// Has no functional effect in release builds.
extern s8 Debug_w[DEBUG_OPTION_COUNT];

#endif // DEBUG

#endif // DEBUG_CONFIG_H
