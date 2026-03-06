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
    DEBUG_SYSTEM_INFO = 2,
    DEBUG_NINDOWS = 3,
    DEBUG_SOUND_CODE_DISP = 4,
    DEBUG_SOUND_SEAMLESS = 5,
    DEBUG_384x224 = 6,
    DEBUG_ROUND_OFF = 7,
    DEBUG_PLAYER_COLOR_TEST = 8,
    DEBUG_1SHOT_SA = 9,
    DEBUG_RAMCNT_FREE_AREA = 10,
    DEBUG_TEX_CASH_FREE = 11,
    DEBUG_SIZE_OF_STRUCT = 12,
    DEBUG_PUL_PUL_PROCESS = 13,
    DEBUG_LDREQ_QUEUE = 14,
    DEBUG_PLAYER_NO_DISP = 15,
    DEBUG_OBJ_SIZE_LINE = 16,
    DEBUG_CURRENT_BOX_DATA = 17,
    DEBUG_DISP_PLAYER_TYPE = 18,
    DEBUG_DISP_BODY = 19,
    DEBUG_DISP_ATTACK = 20,
    DEBUG_DISP_CAT_CAU = 21,
    DEBUG_DISP_OSHI = 22,
    DEBUG_DISP_EFFECT_TYPE = 23,
    DEBUG_TIME_STOP = 24,
    DEBUG_PLAYER_1_NO_LIFE = 25,
    DEBUG_PLAYER_2_NO_LIFE = 26,
    DEBUG_PLAYER_1_INVINCIBLE = 27,
    DEBUG_PLAYER_2_INVINCIBLE = 28,
    DEBUG_MY_CHAR_PL1 = 29,
    DEBUG_MY_CHAR_PL2 = 30,
    DEBUG_STAGE_SELECT = 31,
    DEBUG_CPU_SA = 32,
    DEBUG_DISP_REC_STATUS = 33,
    DEBUG_NO_UPDATE_TEXCASH = 34,
    DEBUG_NO_DISP_SHADOW = 35,
    DEBUG_NO_DISP_SPR_PAL = 36,
    DEBUG_NO_DISP_SPR_CP3 = 37,
    DEBUG_NO_DISP_SPR_RGB = 38,
    DEBUG_NO_DISP_TYPE_SB = 39,
    DEBUG_EFF_NOT_MOVE = 40,
    DEBUG_EFF_NUM_DISP = 41,
    DEBUG_BG_DRAW_OFF = 42,
    DEBUG_AUTO_RAPID_SHOT = 43,
    DEBUG_PUB_BGM_OFF = 44,
    DEBUG_MC_FAVORITE_PLNUM = 45,
    DEBUG_BONUS_CHECK = 46,
    DEBUG_ENDING_CHECK = 47,
    DEBUG_OPENING_TEST = 48,
    DEBUG_CPU_REPLAY_TEST = 49,
    DEBUG_DISPLAY_LEVER = 50,
    DEBUG_USE_GILL = 51,
    DEBUG_SYSTEM_DIRECTION = 52,
    DEBUG_NEW_COLOR = 53,
    DEBUG_ACTIVE_NO = 54,
    DEBUG_PASSIVE_NO = 55,
    DEBUG_DISP_CPU_DATA = 56,
    DEBUG_DISP_FREE_WORK = 57,
    DEBUG_USE_EXTRA_OPTION = 58,
    DEBUG_DISP_RANDOM = 59,
    DEBUG_FREE_60 = 60,
    DEBUG_VM_SAME_CHECK = 61,
    DEBUG_BG_POSITION = 62,
    DEBUG_FREE_63 = 63,
    DEBUG_RANKING_TEST = 64,
    DEBUG_VM_ERROR_TEST = 65,
    DEBUG_VM_BLOCK_TEST = 66,
    DEBUG_BLUE_BACK = 67,
    DEBUG_MESSAGE_TEST = 68,
    DEBUG_TURBO = 69,
    DEBUG_YOSHIZUMI_EXP = 70,
    DEBUG_NAKAGAWA_EXPERIMENT = 71,
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

/// @brief Debug profile name data
extern const char* debug_profile_name_data[5];

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
