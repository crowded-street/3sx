#include "port/sdl/sdl_pad.h"
#include "port/config/keymap.h"

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

static bool any_pressed(const bool* keys, KeymapButton button) {
    bool result = false;
    const SDL_Scancode* codes = Keymap_GetScancodes(button);

    for (int i = 0; i < KEYMAP_CODES_PER_BUTTON; i++) {
        const SDL_Scancode code = codes[i];

        if (code == SDL_SCANCODE_UNKNOWN) {
            break;
        }

        result = result || keys[code];    
    }

    return result;
}

static void get_keyboard_state(SDLPad_ButtonState* state) {
    SDL_zerop(state);
    const bool* keys = SDL_GetKeyboardState(NULL);

    state->dpad_up = any_pressed(keys, KEYMAP_BUTTON_UP);
    state->dpad_left = any_pressed(keys, KEYMAP_BUTTON_LEFT);
    state->dpad_down = any_pressed(keys, KEYMAP_BUTTON_DOWN);
    state->dpad_right = any_pressed(keys, KEYMAP_BUTTON_RIGHT);
    state->north = any_pressed(keys, KEYMAP_BUTTON_NORTH);
    state->west = any_pressed(keys, KEYMAP_BUTTON_WEST);
    state->south = any_pressed(keys, KEYMAP_BUTTON_SOUTH);
    state->east = any_pressed(keys, KEYMAP_BUTTON_EAST);
    state->left_shoulder = any_pressed(keys, KEYMAP_BUTTON_LEFT_SHOULDER);
    state->right_shoulder = any_pressed(keys, KEYMAP_BUTTON_RIGHT_SHOULDER);
    state->left_trigger = any_pressed(keys, KEYMAP_BUTTON_LEFT_TRIGGER) ? SDL_MAX_SINT16 : 0;
    state->right_trigger = any_pressed(keys, KEYMAP_BUTTON_RIGHT_TRIGGER) ? SDL_MAX_SINT16 : 0;
    state->left_stick = any_pressed(keys, KEYMAP_BUTTON_LEFT_STICK);
    state->right_stick = any_pressed(keys, KEYMAP_BUTTON_RIGHT_STICK);
    state->back = any_pressed(keys, KEYMAP_BUTTON_BACK);
    state->start = any_pressed(keys, KEYMAP_BUTTON_START);

#if defined(DEBUG)
    state->right_stick |= keys[SDL_SCANCODE_TAB];
#endif
}

static void get_gamepad_state(int id, SDLPad_ButtonState* state) {
    const SDL_Gamepad* pad = input_sources[id].gamepad.gamepad;

    state->dpad_up = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_DPAD_UP);
    state->dpad_left = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
    state->dpad_down = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
    state->dpad_right = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
    state->north = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_NORTH);
    state->west = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_WEST);
    state->south = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_SOUTH);
    state->east = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_EAST);
    state->left_shoulder = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
    state->right_shoulder = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
    state->left_stick = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_LEFT_STICK);
    state->right_stick = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
    state->back = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_BACK);
    state->start = SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_START);

    state->left_trigger = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
    state->right_trigger = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
    state->left_stick_x = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTX);
    state->left_stick_y = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTY);
    state->right_stick_x = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHTX);
    state->right_stick_y = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHTY);
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

bool SDLPad_IsGamepadConnected(int id) {
    return input_sources[id].type != SDLPAD_INPUT_NONE;
}

void SDLPad_GetButtonState(int id, SDLPad_ButtonState* state) {
    if (id == keyboard_index) {
        get_keyboard_state(state);
    } else {
        get_gamepad_state(id, state);
    }
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
