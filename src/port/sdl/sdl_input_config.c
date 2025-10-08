#include "port/sdl/sdl_input_config.h"
#include "port/sdl/sdl_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INI section names
static const char* SECTION_GAMEPAD_P1 = "gamepad_p1";
static const char* SECTION_GAMEPAD_P2 = "gamepad_p2";
static const char* SECTION_KEYBOARD = "keyboard";

// Convert action enum to string
static const char* action_to_string(SDLInputConfig_Action action) {
    switch (action) {
    case SDLINPUT_ACTION_DPAD_UP:
        return "dpad_up";
    case SDLINPUT_ACTION_DPAD_DOWN:
        return "dpad_down";
    case SDLINPUT_ACTION_DPAD_LEFT:
        return "dpad_left";
    case SDLINPUT_ACTION_DPAD_RIGHT:
        return "dpad_right";
    case SDLINPUT_ACTION_NORTH:
        return "north";
    case SDLINPUT_ACTION_SOUTH:
        return "south";
    case SDLINPUT_ACTION_EAST:
        return "east";
    case SDLINPUT_ACTION_WEST:
        return "west";
    case SDLINPUT_ACTION_LEFT_SHOULDER:
        return "left_shoulder";
    case SDLINPUT_ACTION_RIGHT_SHOULDER:
        return "right_shoulder";
    case SDLINPUT_ACTION_LEFT_TRIGGER:
        return "left_trigger";
    case SDLINPUT_ACTION_RIGHT_TRIGGER:
        return "right_trigger";
    case SDLINPUT_ACTION_L3:
        return "l3";
    case SDLINPUT_ACTION_R3:
        return "r3";
    case SDLINPUT_ACTION_START:
        return "start";
    case SDLINPUT_ACTION_BACK:
        return "back";
    default:
        return "unknown";
    }
}

// Convert string to action enum
static SDLInputConfig_Action string_to_action(const char* str) {
    if (strcmp(str, "dpad_up") == 0)
        return SDLINPUT_ACTION_DPAD_UP;
    if (strcmp(str, "dpad_down") == 0)
        return SDLINPUT_ACTION_DPAD_DOWN;
    if (strcmp(str, "dpad_left") == 0)
        return SDLINPUT_ACTION_DPAD_LEFT;
    if (strcmp(str, "dpad_right") == 0)
        return SDLINPUT_ACTION_DPAD_RIGHT;
    if (strcmp(str, "north") == 0)
        return SDLINPUT_ACTION_NORTH;
    if (strcmp(str, "south") == 0)
        return SDLINPUT_ACTION_SOUTH;
    if (strcmp(str, "east") == 0)
        return SDLINPUT_ACTION_EAST;
    if (strcmp(str, "west") == 0)
        return SDLINPUT_ACTION_WEST;
    if (strcmp(str, "left_shoulder") == 0)
        return SDLINPUT_ACTION_LEFT_SHOULDER;
    if (strcmp(str, "right_shoulder") == 0)
        return SDLINPUT_ACTION_RIGHT_SHOULDER;
    if (strcmp(str, "left_trigger") == 0)
        return SDLINPUT_ACTION_LEFT_TRIGGER;
    if (strcmp(str, "right_trigger") == 0)
        return SDLINPUT_ACTION_RIGHT_TRIGGER;
    if (strcmp(str, "L3") == 0 || strcmp(str, "l3") == 0 || strcmp(str, "left_stick") == 0)
        return SDLINPUT_ACTION_L3;
    if (strcmp(str, "R3") == 0 || strcmp(str, "r3") == 0 || strcmp(str, "right_stick") == 0)
        return SDLINPUT_ACTION_R3;
    if (strcmp(str, "start") == 0)
        return SDLINPUT_ACTION_START;
    if (strcmp(str, "back") == 0)
        return SDLINPUT_ACTION_BACK;

    return SDLINPUT_ACTION_COUNT; // Invalid
}

// Convert SDL scancode name to scancode
static SDL_Scancode string_to_scancode(const char* str) {
    return SDL_GetScancodeFromName(str);
}

// Convert scancode to string
static const char* scancode_to_string(SDL_Scancode scancode) {
    return SDL_GetScancodeName(scancode);
}

// Convert string to gamepad button
static SDL_GamepadButton string_to_gamepad_button(const char* str) {
    if (strcmp(str, "button_dpad_up") == 0)
        return SDL_GAMEPAD_BUTTON_DPAD_UP;
    if (strcmp(str, "button_dpad_down") == 0)
        return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
    if (strcmp(str, "button_dpad_left") == 0)
        return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
    if (strcmp(str, "button_dpad_right") == 0)
        return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
    if (strcmp(str, "button_north") == 0 || strcmp(str, "y") == 0)
        return SDL_GAMEPAD_BUTTON_NORTH;
    if (strcmp(str, "button_south") == 0 || strcmp(str, "a") == 0)
        return SDL_GAMEPAD_BUTTON_SOUTH;
    if (strcmp(str, "button_east") == 0 || strcmp(str, "b") == 0)
        return SDL_GAMEPAD_BUTTON_EAST;
    if (strcmp(str, "button_west") == 0 || strcmp(str, "x") == 0)
        return SDL_GAMEPAD_BUTTON_WEST;
    if (strcmp(str, "button_left_shoulder") == 0 || strcmp(str, "lb") == 0 || strcmp(str, "l1") == 0)
        return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
    if (strcmp(str, "button_right_shoulder") == 0 || strcmp(str, "rb") == 0 || strcmp(str, "r1") == 0)
        return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
    if (strcmp(str, "button_L3") == 0 || strcmp(str, "button_l3") == 0 || strcmp(str, "L3") == 0 || strcmp(str, "l3") == 0 || strcmp(str, "button_left_stick") == 0)
        return SDL_GAMEPAD_BUTTON_LEFT_STICK;
    if (strcmp(str, "button_R3") == 0 || strcmp(str, "button_r3") == 0 || strcmp(str, "R3") == 0 || strcmp(str, "r3") == 0 || strcmp(str, "button_right_stick") == 0)
        return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
    if (strcmp(str, "button_start") == 0)
        return SDL_GAMEPAD_BUTTON_START;
    if (strcmp(str, "button_back") == 0 || strcmp(str, "select") == 0)
        return SDL_GAMEPAD_BUTTON_BACK;

    return SDL_GAMEPAD_BUTTON_INVALID;
}

// Convert gamepad button to string
static const char* gamepad_button_to_string(SDL_GamepadButton button) {
    switch (button) {
    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        return "button_dpad_up";
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        return "button_dpad_down";
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        return "button_dpad_left";
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        return "button_dpad_right";
    case SDL_GAMEPAD_BUTTON_NORTH:
        return "button_north";
    case SDL_GAMEPAD_BUTTON_SOUTH:
        return "button_south";
    case SDL_GAMEPAD_BUTTON_EAST:
        return "button_east";
    case SDL_GAMEPAD_BUTTON_WEST:
        return "button_west";
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
        return "button_left_shoulder";
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
        return "button_right_shoulder";
    case SDL_GAMEPAD_BUTTON_LEFT_STICK:
        return "button_l3";
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
        return "button_r3";
    case SDL_GAMEPAD_BUTTON_START:
        return "button_start";
    case SDL_GAMEPAD_BUTTON_BACK:
        return "button_back";
    default:
        return "button_invalid";
    }
}

// Convert string to gamepad axis
static SDL_GamepadAxis string_to_gamepad_axis(const char* str) {
    if (strcmp(str, "axis_left_trigger") == 0 || strcmp(str, "lt") == 0 || strcmp(str, "l2") == 0)
        return SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
    if (strcmp(str, "axis_right_trigger") == 0 || strcmp(str, "rt") == 0 || strcmp(str, "r2") == 0)
        return SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
    if (strcmp(str, "axis_leftx") == 0 || strcmp(str, "left_stick_x") == 0)
        return SDL_GAMEPAD_AXIS_LEFTX;
    if (strcmp(str, "axis_lefty") == 0 || strcmp(str, "left_stick_y") == 0)
        return SDL_GAMEPAD_AXIS_LEFTY;
    if (strcmp(str, "axis_rightx") == 0 || strcmp(str, "right_stick_x") == 0)
        return SDL_GAMEPAD_AXIS_RIGHTX;
    if (strcmp(str, "axis_righty") == 0 || strcmp(str, "right_stick_y") == 0)
        return SDL_GAMEPAD_AXIS_RIGHTY;

    return SDL_GAMEPAD_AXIS_INVALID;
}

// Convert gamepad axis to string
static const char* gamepad_axis_to_string(SDL_GamepadAxis axis) {
    switch (axis) {
    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
        return "axis_left_trigger";
    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
        return "axis_right_trigger";
    case SDL_GAMEPAD_AXIS_LEFTX:
        return "axis_leftx";
    case SDL_GAMEPAD_AXIS_LEFTY:
        return "axis_lefty";
    case SDL_GAMEPAD_AXIS_RIGHTX:
        return "axis_rightx";
    case SDL_GAMEPAD_AXIS_RIGHTY:
        return "axis_righty";
    default:
        return "axis_invalid";
    }
}

static void init_player_defaults(SDLInputConfig_Player* player, int player_index) {
    // Default gamepad mappings (standard console layout)
    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_UP].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_UP].button = SDL_GAMEPAD_BUTTON_DPAD_UP;

    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_DOWN].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_DOWN].button = SDL_GAMEPAD_BUTTON_DPAD_DOWN;

    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_LEFT].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_LEFT].button = SDL_GAMEPAD_BUTTON_DPAD_LEFT;

    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_RIGHT].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_DPAD_RIGHT].button = SDL_GAMEPAD_BUTTON_DPAD_RIGHT;

    player->gamepad.mapping[SDLINPUT_ACTION_NORTH].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_NORTH].button = SDL_GAMEPAD_BUTTON_NORTH;

    player->gamepad.mapping[SDLINPUT_ACTION_SOUTH].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_SOUTH].button = SDL_GAMEPAD_BUTTON_SOUTH;

    player->gamepad.mapping[SDLINPUT_ACTION_EAST].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_EAST].button = SDL_GAMEPAD_BUTTON_EAST;

    player->gamepad.mapping[SDLINPUT_ACTION_WEST].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_WEST].button = SDL_GAMEPAD_BUTTON_WEST;

    player->gamepad.mapping[SDLINPUT_ACTION_LEFT_SHOULDER].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_LEFT_SHOULDER].button = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;

    player->gamepad.mapping[SDLINPUT_ACTION_RIGHT_SHOULDER].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_RIGHT_SHOULDER].button = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;

    player->gamepad.mapping[SDLINPUT_ACTION_LEFT_TRIGGER].type = SDLINPUT_GAMEPAD_AXIS;
    player->gamepad.mapping[SDLINPUT_ACTION_LEFT_TRIGGER].axis = SDL_GAMEPAD_AXIS_LEFT_TRIGGER;

    player->gamepad.mapping[SDLINPUT_ACTION_RIGHT_TRIGGER].type = SDLINPUT_GAMEPAD_AXIS;
    player->gamepad.mapping[SDLINPUT_ACTION_RIGHT_TRIGGER].axis = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;

    player->gamepad.mapping[SDLINPUT_ACTION_L3].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_L3].button = SDL_GAMEPAD_BUTTON_LEFT_STICK;

    player->gamepad.mapping[SDLINPUT_ACTION_R3].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_R3].button = SDL_GAMEPAD_BUTTON_RIGHT_STICK;

    player->gamepad.mapping[SDLINPUT_ACTION_START].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_START].button = SDL_GAMEPAD_BUTTON_START;

    player->gamepad.mapping[SDLINPUT_ACTION_BACK].type = SDLINPUT_GAMEPAD_BUTTON;
    player->gamepad.mapping[SDLINPUT_ACTION_BACK].button = SDL_GAMEPAD_BUTTON_BACK;

    // Default keyboard mappings
    player->keyboard.mapping[SDLINPUT_ACTION_DPAD_UP] = SDL_SCANCODE_W;
    player->keyboard.mapping[SDLINPUT_ACTION_DPAD_DOWN] = SDL_SCANCODE_S;
    player->keyboard.mapping[SDLINPUT_ACTION_DPAD_LEFT] = SDL_SCANCODE_A;
    player->keyboard.mapping[SDLINPUT_ACTION_DPAD_RIGHT] = SDL_SCANCODE_D;
    player->keyboard.mapping[SDLINPUT_ACTION_NORTH] = SDL_SCANCODE_I;
    player->keyboard.mapping[SDLINPUT_ACTION_SOUTH] = SDL_SCANCODE_J;
    player->keyboard.mapping[SDLINPUT_ACTION_EAST] = SDL_SCANCODE_K;
    player->keyboard.mapping[SDLINPUT_ACTION_WEST] = SDL_SCANCODE_U;
    player->keyboard.mapping[SDLINPUT_ACTION_LEFT_SHOULDER] = SDL_SCANCODE_SEMICOLON;
    player->keyboard.mapping[SDLINPUT_ACTION_RIGHT_SHOULDER] = SDL_SCANCODE_O;
    player->keyboard.mapping[SDLINPUT_ACTION_LEFT_TRIGGER] = SDL_SCANCODE_P;
    player->keyboard.mapping[SDLINPUT_ACTION_RIGHT_TRIGGER] = SDL_SCANCODE_L;
    player->keyboard.mapping[SDLINPUT_ACTION_L3] = SDL_SCANCODE_9;
    player->keyboard.mapping[SDLINPUT_ACTION_R3] = SDL_SCANCODE_0;
    player->keyboard.mapping[SDLINPUT_ACTION_START] = SDL_SCANCODE_RETURN;
    player->keyboard.mapping[SDLINPUT_ACTION_BACK] = SDL_SCANCODE_BACKSPACE;
}

void SDLInputConfig_Init(SDLInputConfig* config) {
    for (int i = 0; i < SDLINPUT_MAX_PLAYERS; i++) {
        init_player_defaults(&config->players[i], i);
    }
}

static void load_player_config(SDLInputConfig_Player* player, const SDLConfig* config, const char* gamepad_section, const char* keyboard_section) {
    // Load gamepad mappings
    for (int i = 0; i < SDLINPUT_ACTION_COUNT; i++) {
        const char* action_name = action_to_string(i);
        const char* value = SDLConfig_GetString(config, gamepad_section, action_name);
        if (value) {
            // Try button first
            SDL_GamepadButton button = string_to_gamepad_button(value);
            if (button != SDL_GAMEPAD_BUTTON_INVALID) {
                player->gamepad.mapping[i].type = SDLINPUT_GAMEPAD_BUTTON;
                player->gamepad.mapping[i].button = button;
            } else {
                // Try axis
                SDL_GamepadAxis axis = string_to_gamepad_axis(value);
                if (axis != SDL_GAMEPAD_AXIS_INVALID) {
                    player->gamepad.mapping[i].type = SDLINPUT_GAMEPAD_AXIS;
                    player->gamepad.mapping[i].axis = axis;
                }
            }
        }
    }

    // Load keyboard mappings
    for (int i = 0; i < SDLINPUT_ACTION_COUNT; i++) {
        const char* action_name = action_to_string(i);
        const char* value = SDLConfig_GetString(config, keyboard_section, action_name);
        if (value) {
            SDL_Scancode scancode = string_to_scancode(value);
            if (scancode != SDL_SCANCODE_UNKNOWN) {
                player->keyboard.mapping[i] = scancode;
            }
        }
    }
}

void SDLInputConfig_LoadFromConfig(SDLInputConfig* input_config, const SDLConfig* config) {
    // Load player 1 config (keyboard is shared)
    load_player_config(&input_config->players[0], config, SECTION_GAMEPAD_P1, SECTION_KEYBOARD);

    // Load player 2 config (keyboard is shared)
    load_player_config(&input_config->players[1], config, SECTION_GAMEPAD_P2, SECTION_KEYBOARD);
}

static void save_player_config(const SDLInputConfig_Player* player, SDLConfig* config, const char* gamepad_section, const char* keyboard_section) {
    // Update gamepad section
    for (int i = 0; i < SDLINPUT_ACTION_COUNT; i++) {
        const char* action_name = action_to_string(i);
        const SDLInputConfig_GamepadInput* input = &player->gamepad.mapping[i];

        if (input->type == SDLINPUT_GAMEPAD_BUTTON) {
            const char* button_name = gamepad_button_to_string(input->button);
            SDLConfig_SetString(config, gamepad_section, action_name, button_name);
        } else if (input->type == SDLINPUT_GAMEPAD_AXIS) {
            const char* axis_name = gamepad_axis_to_string(input->axis);
            SDLConfig_SetString(config, gamepad_section, action_name, axis_name);
        }
    }

    // Update keyboard section
    for (int i = 0; i < SDLINPUT_ACTION_COUNT; i++) {
        const char* action_name = action_to_string(i);
        const char* scancode_name = scancode_to_string(player->keyboard.mapping[i]);
        SDLConfig_SetString(config, keyboard_section, action_name, scancode_name);
    }
}

void SDLInputConfig_SaveToConfig(const SDLInputConfig* input_config, SDLConfig* config) {
    for (int i = 0; i < SDLINPUT_ACTION_COUNT; i++) {
        const char* action_name = action_to_string(i);

        // Player 1 gamepad
        const SDLInputConfig_GamepadInput* p1_input = &input_config->players[0].gamepad.mapping[i];
        if (p1_input->type == SDLINPUT_GAMEPAD_BUTTON) {
            const char* button_name = gamepad_button_to_string(p1_input->button);
            SDLConfig_SetString(config, SECTION_GAMEPAD_P1, action_name, button_name);
        } else if (p1_input->type == SDLINPUT_GAMEPAD_AXIS) {
            const char* axis_name = gamepad_axis_to_string(p1_input->axis);
            SDLConfig_SetString(config, SECTION_GAMEPAD_P1, action_name, axis_name);
        }

        // Player 2 gamepad
        const SDLInputConfig_GamepadInput* p2_input = &input_config->players[1].gamepad.mapping[i];
        if (p2_input->type == SDLINPUT_GAMEPAD_BUTTON) {
            const char* button_name = gamepad_button_to_string(p2_input->button);
            SDLConfig_SetString(config, SECTION_GAMEPAD_P2, action_name, button_name);
        } else if (p2_input->type == SDLINPUT_GAMEPAD_AXIS) {
            const char* axis_name = gamepad_axis_to_string(p2_input->axis);
            SDLConfig_SetString(config, SECTION_GAMEPAD_P2, action_name, axis_name);
        }
    }

    for (int i = 0; i < SDLINPUT_ACTION_COUNT; i++) {
        const char* action_name = action_to_string(i);
        const char* scancode_name = scancode_to_string(input_config->players[0].keyboard.mapping[i]);
        SDLConfig_SetString(config, SECTION_KEYBOARD, action_name, scancode_name);
    }
}
