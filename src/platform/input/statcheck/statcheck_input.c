#if CRS_INPUT_DRIVER_STATCHECK

#include "platform/input/statcheck/statcheck_input.h"

static Input_ButtonState input_state[2] = { 0 };

// Internal

void StatcheckInput_SetButtonState(int id, const Input_ButtonState* state) {
    SDL_copyp(&input_state[id], state);
}

// Public

bool StatcheckInput_IsGamepadConnected(int id) {
    return true;
}

void StatcheckInput_GetButtonState(int id, Input_ButtonState* state) {
    SDL_copyp(state, &input_state[id]);
}

void StatcheckInput_RumblePad(int id, bool low_freq_enabled, Uint8 high_freq_rumble) {
    // Do nothing
}

#endif // CRS_INPUT_DRIVER_STATCHECK
