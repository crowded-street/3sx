#include "port/sdl/sdl_pad.h"

#include <SDL3/SDL.h>

#define INPUT_SOURCES_MAX 2

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
static int keyboard_index = -1;
static SDLPad_ButtonState button_state[INPUT_SOURCES_MAX] = { 0 };

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

static void setup_keyboard() {
    if (keyboard_index >= 0) {
        return;
    }

    for (int i = 0; i < SDL_arraysize(input_sources); i++) {
        SDLPad_InputSource* input_source = &input_sources[i];

        if (input_source->type == SDLPAD_INPUT_NONE) {
            input_source->type = SDLPAD_INPUT_KEYBOARD;
            keyboard_index = i;
            connected_input_sources += 1;
            break;
        }
    }
}

static void remove_keyboard() {
    if (keyboard_index < 0) {
        return;
    }

    for (int i = 0; i < SDL_arraysize(input_sources); i++) {
        SDLPad_InputSource* input_source = &input_sources[i];

        if (input_source->type == SDLPAD_INPUT_KEYBOARD) {
            input_source->type = SDLPAD_INPUT_NONE;
            keyboard_index = -1;
            connected_input_sources -= 1;
            break;
        }
    }
}

static void handle_gamepad_added_event(SDL_GamepadDeviceEvent* event) {
    // Remove keyboard to potentially make space for the new gamepad
    remove_keyboard();

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

    // Setup keyboard again, if there's a free slot
    setup_keyboard();
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

    // Setup keyboard in the newly freed slot
    setup_keyboard();
}

void SDLPad_Init() {
    setup_keyboard();
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

    switch (event->button) {
    case SDL_GAMEPAD_BUTTON_SOUTH:
        state->south = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_EAST:
        state->east = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_WEST:
        state->west = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_NORTH:
        state->north = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_BACK:
        state->back = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_START:
        state->start = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_LEFT_STICK:
        state->left_stick = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
        state->right_stick = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
        state->left_shoulder = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
        state->right_shoulder = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_DPAD_UP:
        state->dpad_up = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
        state->dpad_down = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
        state->dpad_left = event->down;
        break;

    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
        state->dpad_right = event->down;
        break;
    }
}

void SDLPad_HandleGamepadAxisMotionEvent(SDL_GamepadAxisEvent* event) {
    const int index = input_source_index_from_joystick_id(event->which);

    if (index < 0) {
        return;
    }

    SDLPad_ButtonState* state = &button_state[index];

    switch (event->axis) {
    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
        state->left_trigger = event->value;
        break;

    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
        state->right_trigger = event->value;
        break;

    case SDL_GAMEPAD_AXIS_LEFTX:
        state->left_stick_x = event->value;
        break;

    case SDL_GAMEPAD_AXIS_LEFTY:
        state->left_stick_y = event->value;
        break;

    case SDL_GAMEPAD_AXIS_RIGHTX:
        state->right_stick_x = event->value;
        break;

    case SDL_GAMEPAD_AXIS_RIGHTY:
        state->right_stick_y = event->value;
        break;
    }
}

void SDLPad_HandleKeyboardEvent(SDL_KeyboardEvent* event) {
    if (keyboard_index < 0) {
        return;
    }

    SDLPad_ButtonState* state = &button_state[keyboard_index];

    switch (event->key) {
    case SDLK_W:
        state->dpad_up = event->down;
        break;

    case SDLK_A:
        state->dpad_left = event->down;
        break;

    case SDLK_S:
        state->dpad_down = event->down;
        break;

    case SDLK_D:
        state->dpad_right = event->down;
        break;

    case SDLK_I:
        state->north = event->down;
        break;

    case SDLK_J:
        state->south = event->down;
        break;

    case SDLK_K:
        state->east = event->down;
        break;

    case SDLK_U:
        state->west = event->down;
        break;

    case SDLK_P:
        state->left_shoulder = event->down;
        break;

    case SDLK_O:
        state->right_shoulder = event->down;
        break;

    case SDLK_SEMICOLON:
        state->left_trigger = event->down ? SDL_MAX_SINT16 : 0;
        break;

    case SDLK_L:
        state->right_trigger = event->down ? SDL_MAX_SINT16 : 0;
        break;

    case SDLK_9:
        state->left_stick = event->down;
        break;

    case SDLK_0:
        state->right_stick = event->down;
        break;

    case SDLK_BACKSPACE:
        state->back = event->down;
        break;

    case SDLK_RETURN:
        state->start = event->down;
        break;
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
