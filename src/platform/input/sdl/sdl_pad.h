#if CRS_INPUT_DRIVER_SDL

#ifndef SDL_PAD_H
#define SDL_PAD_H

#include "core/input.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

// Internal

void SDLPad_Init();
void SDLPad_HandleGamepadDeviceEvent(SDL_GamepadDeviceEvent* event);

// Public

bool SDLPad_IsGamepadConnected(int id);
void SDLPad_GetButtonState(int id, Input_ButtonState* state);
void SDLPad_RumblePad(int id, bool low_freq_enabled, Uint8 high_freq_rumble);

#endif

#endif // CRS_INPUT_DRIVER_SDL
