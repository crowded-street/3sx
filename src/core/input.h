#ifndef CORE_INPUT_H
#define CORE_INPUT_H

#include <stdbool.h>

#include <SDL3/SDL.h>

typedef struct Input_ButtonState {
    bool south;
    bool east;
    bool west;
    bool north;
    bool back;
    bool start;
    bool left_stick;
    bool right_stick;
    bool left_shoulder;
    bool right_shoulder;
    Sint16 left_trigger;
    Sint16 right_trigger;
    bool dpad_up;
    bool dpad_down;
    bool dpad_left;
    bool dpad_right;
    Sint16 left_stick_x;
    Sint16 left_stick_y;
    Sint16 right_stick_x;
    Sint16 right_stick_y;
} Input_ButtonState;

bool Input_IsGamepadConnected(int id);
void Input_GetButtonState(int id, Input_ButtonState* state);
void Input_RumblePad(int id, bool low_freq_enabled, Uint8 high_freq_rumble);

#endif
