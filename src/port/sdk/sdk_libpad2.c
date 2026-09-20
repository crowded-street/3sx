#include "common.h"
#include "core/input.h"

#include <libpad2.h>

#include <string.h>

int scePad2GetState(int socket_number) {
    return Input_IsGamepadConnected(socket_number) ? scePad2StateStable : scePad2StateNoLink;
}

int scePad2GetButtonProfile(int socket_number, unsigned char* profile) {
    // Profile for Digital controller
    // profile[0] = 0xF9;
    // profile[1] = 0xFF;
    // profile[2] = 0;
    // profile[3] = 0;

    // Profile for Dualshock 2
    profile[0] = 0xFF;
    profile[1] = 0xFF;
    profile[2] = 0xFF;
    profile[3] = 0xFF;

    return 4;
}

/* One button's pressure byte: full where the button is down. The eight differ
 * in two values, the field written and the button read, and both are written
 * out at their own call site. The field travels as its address because C has
 * no way to pass a member name; the button is a plain read of a local struct
 * and cannot trap, so evaluating it either way is the same program. */
static void set_button_pressure(unsigned char* out, bool pressed) {
    *out = pressed ? 0xFF : 0;
}

int scePad2Read(int socket_number, scePad2ButtonState* data) {
    memset(data, 0, sizeof(scePad2ButtonState));

    Input_ButtonState button_state;
    Input_GetButtonState(socket_number, &button_state);

    // sw0 and sw1 store the pressed state of each button as bits.
    // 0 = pressed, 1 = released

    data->sw0.byte = 0xFF;
    data->sw1.byte = 0xFF;

    data->sw0.bits.l3 = !button_state.left_stick;
    data->sw0.bits.r3 = !button_state.right_stick;
    data->sw0.bits.select = !button_state.back;
    data->sw0.bits.start = !button_state.start;
    data->sw0.bits.left = !button_state.dpad_left;
    data->sw0.bits.right = !button_state.dpad_right;
    data->sw0.bits.up = !button_state.dpad_up;
    data->sw0.bits.down = !button_state.dpad_down;

    data->sw1.bits.l1 = !button_state.left_shoulder;
    data->sw1.bits.r1 = !button_state.right_shoulder;
    data->sw1.bits.l2 = button_state.left_trigger == 0;
    data->sw1.bits.r2 = button_state.right_trigger == 0;
    data->sw1.bits.cross = !button_state.south;
    data->sw1.bits.circle = !button_state.east;
    data->sw1.bits.square = !button_state.west;
    data->sw1.bits.triangle = !button_state.north;

    // Map SDL stick values (-32768 to 32767) to PS2 format (0x00 to 0xFF, center at 0x7F)
    data->lJoyH = (button_state.left_stick_x + 32768) * 255 / 65535;
    data->lJoyV = (button_state.left_stick_y + 32768) * 255 / 65535;
    data->rJoyH = (button_state.right_stick_x + 32768) * 255 / 65535;
    data->rJoyV = (button_state.right_stick_y + 32768) * 255 / 65535;

    // This sets button pressure

    set_button_pressure(&data->crossP, button_state.south);
    set_button_pressure(&data->circleP, button_state.east);
    set_button_pressure(&data->squareP, button_state.west);
    set_button_pressure(&data->triangleP, button_state.north);
    set_button_pressure(&data->upP, button_state.dpad_up);
    set_button_pressure(&data->downP, button_state.dpad_down);
    set_button_pressure(&data->leftP, button_state.dpad_left);
    set_button_pressure(&data->rightP, button_state.dpad_right);

    return sizeof(scePad2ButtonState);
}

int sceVibGetProfile(int socket_number, unsigned char* profile) {
    profile[0] = 3; // Small and big motor
    return 1;
}

int sceVibSetActParam(int socket_number, int profile_size, unsigned char* profile, int data_size, unsigned char* data) {
    const bool is_small_enabled = profile[0] & 1;
    const bool is_big_enabled = profile[0] & 2;
    unsigned char big_value = 0;
    bool small_value = false;

    if (is_small_enabled) {
        small_value = data[0] & 1;
    }

    if (is_big_enabled) {
        if (is_small_enabled) {
            big_value = ((data[0] & 0xFE) >> 1) | ((data[1] & 1) << 7);
        } else {
            big_value = data[0];
        }
    }

    Input_RumblePad(socket_number, small_value, big_value);
    return 1;
}
