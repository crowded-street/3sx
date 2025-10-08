#include "port/sdl/sdl_pad.h"
#include "port/sdl/sdl_input_config.h"

#include <SDL3/SDL.h>

#define INPUT_SOURCES_MAX 2
#define ANALOG_STICK_DEADZONE 8000  // Deadzone for analog stick to dpad conversion

typedef enum SDLPad_InputType { SDLPAD_INPUT_NONE = 0, SDLPAD_INPUT_GAMEPAD, SDLPAD_INPUT_KEYBOARD } SDLPad_InputType;

typedef struct SDLPad_GamepadInputSource {
    Uint32 type;
    SDL_Gamepad* gamepad;
} SDLPad_GamepadInputSource;

typedef struct SDLPad_KeyboardInputSource {
    Uint32 type;
} SDLPad_KeyboardInputSource;

typedef union SDLPad_InputSource {
    Uint32 type;
    SDLPad_GamepadInputSource gamepad;
    SDLPad_KeyboardInputSource keyboard;
} SDLPad_InputSource;

static SDLPad_InputSource input_sources[INPUT_SOURCES_MAX] = { 0 };
static int connected_input_sources = 0;
static SDLPad_ButtonState button_state[INPUT_SOURCES_MAX] = { 0 };
static SDLInputConfig input_config = { 0 };

static int input_source_index_from_joystick_id(SDL_JoystickID id) {
    for (int i = 0; i < INPUT_SOURCES_MAX; i++) {
        const SDLPad_InputSource* input_source = &input_sources[i];

        if (input_source->type != SDLPAD_INPUT_GAMEPAD) {
            continue;
        }

        const SDL_JoystickID this_id = SDL_GetGamepadID(input_source->gamepad.gamepad);

        if (this_id == id) {
            return i;
        }
    }

    return -1;
}

static void update_button_state_for_action(SDLPad_ButtonState* state, int action, bool pressed, Sint16 axis_value) {
    switch (action) {
    case SDLINPUT_ACTION_DPAD_UP:
        state->dpad_up = pressed;
        break;
    case SDLINPUT_ACTION_DPAD_DOWN:
        state->dpad_down = pressed;
        break;
    case SDLINPUT_ACTION_DPAD_LEFT:
        state->dpad_left = pressed;
        break;
    case SDLINPUT_ACTION_DPAD_RIGHT:
        state->dpad_right = pressed;
        break;
    case SDLINPUT_ACTION_NORTH:
        state->north = pressed;
        break;
    case SDLINPUT_ACTION_SOUTH:
        state->south = pressed;
        break;
    case SDLINPUT_ACTION_EAST:
        state->east = pressed;
        break;
    case SDLINPUT_ACTION_WEST:
        state->west = pressed;
        break;
    case SDLINPUT_ACTION_LEFT_SHOULDER:
        state->left_shoulder = pressed;
        break;
    case SDLINPUT_ACTION_RIGHT_SHOULDER:
        state->right_shoulder = pressed;
        break;
    case SDLINPUT_ACTION_LEFT_TRIGGER:
        state->left_trigger = axis_value;
        break;
    case SDLINPUT_ACTION_RIGHT_TRIGGER:
        state->right_trigger = axis_value;
        break;
    case SDLINPUT_ACTION_L3:
        state->l3 = pressed;
        break;
    case SDLINPUT_ACTION_R3:
        state->r3 = pressed;
        break;
    case SDLINPUT_ACTION_BACK:
        state->back = pressed;
        break;
    case SDLINPUT_ACTION_START:
        state->start = pressed;
        break;
    default:
        break;
    }
}

static void handle_gamepad_added_event(SDL_GamepadDeviceEvent* event) {
    if (connected_input_sources >= INPUT_SOURCES_MAX) {
        return;
    }

    const SDL_Gamepad* gamepad = SDL_OpenGamepad(event->which);

    for (int i = 0; i < INPUT_SOURCES_MAX; i++) {
        SDLPad_InputSource* input_source = &input_sources[i];

        if (input_source->type != SDLPAD_INPUT_NONE) {
            continue;
        }

        input_source->type = SDLPAD_INPUT_GAMEPAD;
        input_source->gamepad.gamepad = gamepad;
        break;
    }

    connected_input_sources += 1;
}

static void handle_gamepad_removed_event(SDL_GamepadDeviceEvent* event) {
    const int index = input_source_index_from_joystick_id(event->which);

    if (index < 0) {
        return;
    }

    SDLPad_InputSource* input_source = &input_sources[index];
    SDL_CloseGamepad(input_source->gamepad.gamepad);
    input_source->type = SDLPAD_INPUT_NONE;
    memset(&button_state[index], 0, sizeof(SDLPad_ButtonState));
    connected_input_sources -= 1;
}

void SDLPad_Init(const SDLInputConfig* input_config_param) {
    // Copy the provided config to our static storage
    memcpy(&input_config, input_config_param, sizeof(SDLInputConfig));

    // Check if a gamepad is already connected
    int num_gamepads = 0;
    SDL_JoystickID* gamepads = SDL_GetGamepads(&num_gamepads);

    if (num_gamepads == 0) {
        // No gamepads -> keyboard controls only player 1, player 2 disconnected
        input_sources[0].type = SDLPAD_INPUT_KEYBOARD;
        input_sources[1].type = SDLPAD_INPUT_NONE;
        connected_input_sources = 1;
    } else if (num_gamepads == 1 && gamepads != NULL) {
        // 1 gamepad -> gamepad for player 1, keyboard for player 2
        SDL_Gamepad* gamepad = SDL_OpenGamepad(gamepads[0]);
        if (gamepad != NULL) {
            input_sources[0].type = SDLPAD_INPUT_GAMEPAD;
            input_sources[0].gamepad.gamepad = gamepad;
        } else {
            input_sources[0].type = SDLPAD_INPUT_KEYBOARD;
        }
        input_sources[1].type = SDLPAD_INPUT_KEYBOARD;
        connected_input_sources = 2;
    } else if (num_gamepads >= 2 && gamepads != NULL) {
        // 2+ gamepads -> gamepad 1 for player 1, gamepad 2 for player 2
        for (int i = 0; i < 2; i++) {
            SDL_Gamepad* gamepad = SDL_OpenGamepad(gamepads[i]);
            if (gamepad != NULL) {
                input_sources[i].type = SDLPAD_INPUT_GAMEPAD;
                input_sources[i].gamepad.gamepad = gamepad;
                connected_input_sources += 1;
            }
        }
    }

    if (gamepads != NULL) {
        SDL_free(gamepads);
    }
}

void SDLPad_HandleGamepadDeviceEvent(SDL_GamepadDeviceEvent* event) {
    switch (event->type) {
    case SDL_EVENT_GAMEPAD_ADDED:
        handle_gamepad_added_event(event);
        break;

    case SDL_EVENT_GAMEPAD_REMOVED:
        handle_gamepad_removed_event(event);
        break;

    default:
        // Do nothing
        break;
    }
}

void SDLPad_HandleGamepadButtonEvent(SDL_GamepadButtonEvent* event) {
    const int index = input_source_index_from_joystick_id(event->which);

    if (index < 0) {
        return;
    }

    SDLPad_ButtonState* state = &button_state[index];

    // Map gamepad button to action using config for this player
    const SDLInputConfig_Player* player_config = &input_config.players[index];
    for (int action = 0; action < SDLINPUT_ACTION_COUNT; action++) {
        const SDLInputConfig_GamepadInput* input = &player_config->gamepad.mapping[action];

        if (input->type != SDLINPUT_GAMEPAD_BUTTON) {
            continue;
        }

        if (input->button != event->button) {
            continue;
        }

        update_button_state_for_action(state, action, event->down, event->down ? SDL_MAX_SINT16 : 0);
        break; // Only handle first matching action
    }
}

void SDLPad_HandleGamepadAxisMotionEvent(SDL_GamepadAxisEvent* event) {
    const int index = input_source_index_from_joystick_id(event->which);

    if (index < 0) {
        return;
    }

    SDLPad_ButtonState* state = &button_state[index];

    // Store raw analog stick values
    switch (event->axis) {
    case SDL_GAMEPAD_AXIS_LEFTX:
        state->left_stick_x = event->value;
        // Convert to dpad if above deadzone
        state->dpad_left = (event->value < -ANALOG_STICK_DEADZONE);
        state->dpad_right = (event->value > ANALOG_STICK_DEADZONE);
        break;
    case SDL_GAMEPAD_AXIS_LEFTY:
        state->left_stick_y = event->value;
        // Convert to dpad if above deadzone
        state->dpad_up = (event->value < -ANALOG_STICK_DEADZONE);
        state->dpad_down = (event->value > ANALOG_STICK_DEADZONE);
        break;
    case SDL_GAMEPAD_AXIS_RIGHTX:
        state->right_stick_x = event->value;
        break;
    case SDL_GAMEPAD_AXIS_RIGHTY:
        state->right_stick_y = event->value;
        break;
    default:
        break;
    }

    // Map gamepad axis to action using config for this player
    const SDLInputConfig_Player* player_config = &input_config.players[index];
    for (int action = 0; action < SDLINPUT_ACTION_COUNT; action++) {
        const SDLInputConfig_GamepadInput* input = &player_config->gamepad.mapping[action];

        if (input->type != SDLINPUT_GAMEPAD_AXIS) {
            continue;
        }

        if (input->axis != event->axis) {
            continue;
        }

        update_button_state_for_action(state, action, event->value > 0, event->value);
        break; // Only handle first matching action
    }
}

void SDLPad_HandleKeyboardEvent(SDL_KeyboardEvent* event) {
    // Check both players' keyboard mappings
    for (int player = 0; player < INPUT_SOURCES_MAX; player++) {
        // Skip if this player is not using keyboard
        if (input_sources[player].type != SDLPAD_INPUT_KEYBOARD) {
            continue;
        }

        SDLPad_ButtonState* state = &button_state[player];
        // Always use player 1's keyboard config
        const SDLInputConfig_Player* player_config = &input_config.players[0];

        // Map keyboard scancode to action using config for this player
        for (int action = 0; action < SDLINPUT_ACTION_COUNT; action++) {
            SDL_Scancode scancode = player_config->keyboard.mapping[action];

            if (scancode != event->scancode) {
                continue;
            }

            update_button_state_for_action(state, action, event->down, event->down ? SDL_MAX_SINT16 : 0);
            break; // Only handle first matching action
        }
    }
}

bool SDLPad_IsGamepadConnected(int id) {
    return input_sources[id].type != SDLPAD_INPUT_NONE;
}

void SDLPad_GetButtonState(int id, SDLPad_ButtonState* state) {
    memcpy(state, &button_state[id], sizeof(SDLPad_ButtonState));
}

void SDLPad_RumblePad(int id, bool low_freq_enabled, Uint8 high_freq_rumble) {
    const SDLPad_InputSource* input_source = &input_sources[id];

    if (input_source->type != SDLPAD_INPUT_GAMEPAD) {
        return;
    }

    const Uint16 low_freq_rumble = low_freq_enabled ? UINT16_MAX : 0;
    const Uint16 high_freq_rumble_adjusted = ((float)high_freq_rumble / UINT8_MAX) * UINT16_MAX;
    const Uint32 duration = high_freq_rumble_adjusted > 0 ? 500 : 200;

    SDL_RumbleGamepad(input_source->gamepad.gamepad, low_freq_rumble, high_freq_rumble_adjusted, duration);
}
