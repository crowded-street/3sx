#include "core/input.h"

#if CRS_INPUT_DRIVER_SDL
#include "platform/input/sdl/sdl_pad.h"
#elif CRS_INPUT_DRIVER_STATCHECK
#include "platform/input/statcheck/statcheck_input.h"
#endif

bool Input_IsGamepadConnected(int id) {
#if CRS_INPUT_DRIVER_SDL
    return SDLPad_IsGamepadConnected(id);
#elif CRS_INPUT_DRIVER_STATCHECK
    return StatcheckInput_IsGamepadConnected(id);
#else
    return false;
#endif
}

void Input_GetButtonState(int id, Input_ButtonState* state) {
#if CRS_INPUT_DRIVER_SDL
    SDLPad_GetButtonState(id, state);
#elif CRS_INPUT_DRIVER_STATCHECK
    StatcheckInput_GetButtonState(id, state);
#endif
}

void Input_RumblePad(int id, bool low_freq_enabled, Uint8 high_freq_rumble) {
#if CRS_INPUT_DRIVER_SDL
    SDLPad_RumblePad(id, low_freq_enabled, high_freq_rumble);
#elif CRS_INPUT_DRIVER_STATCHECK
    StatcheckInput_RumblePad(id, low_freq_enabled, high_freq_rumble);
#endif
}
