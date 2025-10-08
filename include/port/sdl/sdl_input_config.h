#ifndef SDL_INPUT_CONFIG_H
#define SDL_INPUT_CONFIG_H

#include "port/sdl/sdl_config.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

// Game action enum - represents abstract game inputs
typedef enum SDLInputConfig_Action {
    SDLINPUT_ACTION_DPAD_UP = 0,
    SDLINPUT_ACTION_DPAD_DOWN,
    SDLINPUT_ACTION_DPAD_LEFT,
    SDLINPUT_ACTION_DPAD_RIGHT,
    SDLINPUT_ACTION_NORTH,
    SDLINPUT_ACTION_SOUTH,
    SDLINPUT_ACTION_EAST,
    SDLINPUT_ACTION_WEST,
    SDLINPUT_ACTION_LEFT_SHOULDER,
    SDLINPUT_ACTION_RIGHT_SHOULDER,
    SDLINPUT_ACTION_LEFT_TRIGGER,
    SDLINPUT_ACTION_RIGHT_TRIGGER,
    SDLINPUT_ACTION_L3,
    SDLINPUT_ACTION_R3,
    SDLINPUT_ACTION_START,
    SDLINPUT_ACTION_BACK,
    SDLINPUT_ACTION_COUNT
} SDLInputConfig_Action;

// Gamepad mapping: action -> gamepad button/axis
typedef enum SDLInputConfig_GamepadInputType {
    SDLINPUT_GAMEPAD_BUTTON,
    SDLINPUT_GAMEPAD_AXIS
} SDLInputConfig_GamepadInputType;

typedef struct SDLInputConfig_GamepadInput {
    SDLInputConfig_GamepadInputType type;
    union {
        SDL_GamepadButton button;
        SDL_GamepadAxis axis;
    };
} SDLInputConfig_GamepadInput;

typedef struct SDLInputConfig_GamepadMapping {
    SDLInputConfig_GamepadInput mapping[SDLINPUT_ACTION_COUNT];
} SDLInputConfig_GamepadMapping;

// Keyboard mapping: action -> scancode
typedef struct SDLInputConfig_KeyboardMapping {
    SDL_Scancode mapping[SDLINPUT_ACTION_COUNT];
} SDLInputConfig_KeyboardMapping;

// Player-specific config structure
typedef struct SDLInputConfig_Player {
    SDLInputConfig_GamepadMapping gamepad;
    SDLInputConfig_KeyboardMapping keyboard;
} SDLInputConfig_Player;

// Main config structure supporting multiple players
#define SDLINPUT_MAX_PLAYERS 2

typedef struct SDLInputConfig {
    SDLInputConfig_Player players[SDLINPUT_MAX_PLAYERS];
} SDLInputConfig;

// Initialize with defaults
void SDLInputConfig_Init(SDLInputConfig* config);

// Load from SDLConfig object (no file I/O)
void SDLInputConfig_LoadFromConfig(SDLInputConfig* input_config, const SDLConfig* config);

// Save to SDLConfig object (no file I/O)
void SDLInputConfig_SaveToConfig(const SDLInputConfig* input_config, SDLConfig* config);

#endif
