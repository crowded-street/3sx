#include "sf33rd/Source/Game/debug/debug_config.h"
#include "common.h"

#if DEBUG

const DEBUG_STR_DAT debug_string_data[72] = { { .max = 255, .name = "SLOW" },
                                              { .max = 10, .name = "FAST" },
                                              { .max = 2, .name = "SYSTEM INFO" },
                                              { .max = 1, .name = "NINDOWS" },
                                              { .max = 1, .name = "SOUND CODE DISP" },
                                              { .max = 1, .name = "SOUND SEAMLESS" },
                                              { .max = 1, .name = "384x224" },
                                              { .max = 5, .name = "ROUND OFF" },
                                              { .max = 1, .name = "PLAYER COLOR TEST" },
                                              { .max = 1, .name = "1-SHOT S.A." },
                                              { .max = 2, .name = "RAMCNT FREE AREA" },
                                              { .max = 1, .name = "TEX CASH FREE" },
                                              { .max = 1, .name = "SIZE OF STRUCT" },
                                              { .max = 1, .name = "PUL PUL PROCESS" },
                                              { .max = 1, .name = "LDREQ QUEUE" },
                                              { .max = 1, .name = "PLAYER NO DISP" },
                                              { .max = 1, .name = "OBJ Size Line" },
                                              { .max = 15, .name = "CURRENT BOX DATA" },
                                              { .max = 1, .name = "Disp PLAYER_TYPE" },
                                              { .max = 2, .name = "Disp BODY" },
                                              { .max = 2, .name = "Disp ATTACK" },
                                              { .max = 2, .name = "Disp CAT-CAU" },
                                              { .max = 2, .name = "Disp OSHI" },
                                              { .max = 1, .name = "Disp EFFECT_TYPE" },
                                              { .max = 255, .name = "TIME STOP" },
                                              { .max = 1, .name = "NO LIFE PL1" },
                                              { .max = 1, .name = "NO LIFE PL2" },
                                              { .max = 1, .name = "NO DEATH PL1" },
                                              { .max = 1, .name = "NO DEATH PL2" },
                                              { .max = 20, .name = "MY CHAR PL1" },
                                              { .max = 20, .name = "MY CHAR PL2" },
                                              { .max = 21, .name = "STAGE SELECT" },
                                              { .max = 3, .name = "CPU S.A" },
                                              { .max = 1, .name = "DISP REC STATUS" },
                                              { .max = 1, .name = "NO UPDATE TEXCASH" },
                                              { .max = 1, .name = "NO DISP SHADOW" },
                                              { .max = 1, .name = "NO DISP SPR PAL" },
                                              { .max = 1, .name = "NO DISP SPR CP3" },
                                              { .max = 1, .name = "NO DISP SPR RGB" },
                                              { .max = 3, .name = "NO DISP TYPE SB" },
                                              { .max = 1, .name = "EFF NOT MOVE" },
                                              { .max = 1, .name = "EFF NUM DISP" },
                                              { .max = 1, .name = "BG DRAW OFF" },
                                              { .max = 1, .name = "AUTO RAPID SHOT" },
                                              { .max = 1, .name = "PUB BGM OFF" },
                                              { .max = 21, .name = "MC_FAVORITE_PLNUM" },
                                              { .max = 255, .name = "BONUS CHECK" },
                                              { .max = 1, .name = "ENDING CHECK" },
                                              { .max = 255, .name = "OPENING TEST" },
                                              { .max = 3, .name = "CPU REPLAY TEST" },
                                              { .max = 255, .name = "DISPLAY LEVER" },
                                              { .max = 1, .name = "USE GILL" },
                                              { .max = 3, .name = "SYSTEM DIRECTION +" },
                                              { .max = 1, .name = "NEW COLOR" },
                                              { .max = 255, .name = "ACTIVE NO" },
                                              { .max = 255, .name = "PASSIVE NO" },
                                              { .max = 255, .name = "DISP CPU DATA" },
                                              { .max = 1, .name = "DISP FREE WORK" },
                                              { .max = 1, .name = "USE EXTRA OPTION" },
                                              { .max = 255, .name = "DISP RANDOM" },
                                              { .max = 1, .name = "FREE" },
                                              { .max = 255, .name = "VM SAME CHECK" },
                                              { .max = 7, .name = "BG POSITION" },
                                              { .max = 1, .name = "FREE" },
                                              { .max = 5, .name = "RANKING TEST" },
                                              { .max = 255, .name = "VM ERROR TEST" },
                                              { .max = 99, .name = "VM BLOCK TEST" },
                                              { .max = 1, .name = "BLUE BACK" },
                                              { .max = 255, .name = "MESSAGE TEST" },
                                              { .max = 255, .name = "TURBO" },
                                              { .max = 255, .name = "YOSHIZUMI EXP" },
                                              { .max = 255, .name = "NAKAGAWA EXPERIMENT" } };

// Debug menu profile names
const char* debug_profile_name_data[5] = {
    "For NAKAI 600", "For GENTLEMAN", "For NAKAGAWA JIKKEN", "For Oh!Ya!", "For IBARAKI UNCLE"
};

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

void DebugConfig_Set(DebugOption option, s8 value) {
    if (option >= DEBUG_OPTION_COUNT) {
        return;
    }

    // Clamp to valid range
    if (value > debug_string_data[option].max) {
        value = debug_string_data[option].max;
    }

    debug_config.values[option] = value;
}

#else // !DEBUG

// Stub for release builds - Debug_w array that does nothing
s8 Debug_w[72] = { 0 };

#endif // DEBUG
