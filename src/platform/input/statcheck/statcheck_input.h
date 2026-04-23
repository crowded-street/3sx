#if CRS_INPUT_DRIVER_STATCHECK

#ifndef STATCHECK_INPUT_H
#define STATCHECK_INPUT_H

#include "core/input.h"

#include <SDL3/SDL.h>

// Internal

void StatcheckInput_SetButtonState(int id, const Input_ButtonState* state);

// Public

bool StatcheckInput_IsGamepadConnected(int id);
void StatcheckInput_GetButtonState(int id, Input_ButtonState* state);
void StatcheckInput_RumblePad(int id, bool low_freq_enabled, Uint8 high_freq_rumble);

#endif

#endif // CRS_INPUT_DRIVER_STATCHECK
