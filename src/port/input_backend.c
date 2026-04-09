#include "port/input_backend.h"

#include "port/sdl/sdl_pad.h"

#include <SDL3/SDL.h>

#include <string.h>

void InputBackend_Init() {
    SDLPad_Init();
}

void InputBackend_HandleGamepadDeviceEvent(const void* event) {
    SDLPad_HandleGamepadDeviceEvent((SDL_GamepadDeviceEvent*)event);
}

bool InputBackend_IsGamepadConnected(int id) {
    return SDLPad_IsGamepadConnected(id);
}

void InputBackend_GetButtonState(int id, InputBackend_ButtonState* state) {
    SDLPad_ButtonState sdl_state;

    if (state == NULL) {
        return;
    }

    memset(state, 0, sizeof(*state));

    if (!InputBackend_IsGamepadConnected(id)) {
        return;
    }

    memset(&sdl_state, 0, sizeof(sdl_state));
    SDLPad_GetButtonState(id, &sdl_state);

    state->south = sdl_state.south;
    state->east = sdl_state.east;
    state->west = sdl_state.west;
    state->north = sdl_state.north;
    state->back = sdl_state.back;
    state->start = sdl_state.start;
    state->left_stick = sdl_state.left_stick;
    state->right_stick = sdl_state.right_stick;
    state->left_shoulder = sdl_state.left_shoulder;
    state->right_shoulder = sdl_state.right_shoulder;
    state->left_trigger = sdl_state.left_trigger;
    state->right_trigger = sdl_state.right_trigger;
    state->dpad_up = sdl_state.dpad_up;
    state->dpad_down = sdl_state.dpad_down;
    state->dpad_left = sdl_state.dpad_left;
    state->dpad_right = sdl_state.dpad_right;
    state->left_stick_x = sdl_state.left_stick_x;
    state->left_stick_y = sdl_state.left_stick_y;
    state->right_stick_x = sdl_state.right_stick_x;
    state->right_stick_y = sdl_state.right_stick_y;
}

void InputBackend_RumblePad(int id, bool low_freq_enabled, uint8_t high_freq_rumble) {
    SDLPad_RumblePad(id, low_freq_enabled, high_freq_rumble);
}
