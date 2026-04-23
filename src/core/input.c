#include "core/input.h"

#include "port/sdl/sdl_pad.h"

#include <SDL3/SDL.h>

bool Input_IsGamepadConnected(int id) {
    return SDLPad_IsGamepadConnected(id);
}

void Input_GetButtonState(int id, Input_ButtonState* state) {
    SDLPad_GetButtonState(id, state);
}

void Input_RumblePad(int id, bool low_freq_enabled, Uint8 high_freq_rumble) {
    SDLPad_RumblePad(id, low_freq_enabled, high_freq_rumble);
}
