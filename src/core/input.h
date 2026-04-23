#ifndef CORE_INPUT_H
#define CORE_INPUT_H

#include <stdbool.h>
#include <stdint.h>

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
    int16_t left_trigger;
    int16_t right_trigger;
    bool dpad_up;
    bool dpad_down;
    bool dpad_left;
    bool dpad_right;
    int16_t left_stick_x;
    int16_t left_stick_y;
    int16_t right_stick_x;
    int16_t right_stick_y;
} Input_ButtonState;

bool Input_IsGamepadConnected(int id);
void Input_GetButtonState(int id, Input_ButtonState* state);
void Input_RumblePad(int id, bool low_freq_enabled, uint8_t high_freq_rumble);

#endif
