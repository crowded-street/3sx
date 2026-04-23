#if STATCHECK

#include "test/test_runner.h"
#include "arcade/arcade_constants.h"
#include "constants.h"
#include "main.h"
#include "platform/input/statcheck/statcheck_input.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "test/replay_game.h"
#include "test/test_runner_compare.h"
#include "test/test_runner_utils.h"

#include "stb/stb_ds.h"
#include <SDL3/SDL.h>

#include <signal.h>
#include <stdio.h>

typedef enum Phase {
    PHASE_TITLE,
    PHASE_MENU,
    PHASE_CHARACTER_SELECT_TRANSITION,
    PHASE_CHARACTER_SELECT,
    PHASE_GAME_TRANSITION,
    PHASE_GAME,
} Phase;

static const Uint8 character_to_cursor[20][2] = { { 7, 1 }, { 1, 0 }, { 5, 2 }, { 6, 1 }, { 3, 2 }, { 4, 0 }, { 1, 2 },
                                                  { 3, 0 }, { 2, 2 }, { 4, 2 }, { 0, 1 }, { 0, 2 }, { 2, 0 }, { 5, 0 },
                                                  { 6, 0 }, { 3, 1 }, { 2, 1 }, { 4, 1 }, { 1, 1 }, { 5, 1 } };

static const SWKey color_to_keys[13] = {
    SWK_WEST,
    SWK_NORTH,
    SWK_RIGHT_SHOULDER,
    SWK_SOUTH,
    SWK_EAST,
    SWK_RIGHT_TRIGGER,
    SWK_WEST | SWK_RIGHT_SHOULDER | SWK_EAST,
    SWK_START | SWK_WEST,
    SWK_START | SWK_NORTH,
    SWK_START | SWK_RIGHT_SHOULDER,
    SWK_START | SWK_SOUTH,
    SWK_START | SWK_EAST,
    SWK_START | SWK_RIGHT_TRIGGER,
};

static Uint64 frame = 0;
static Phase phase = PHASE_TITLE;
static int char_select_phase = 0;
static int wait_timer = 0;
static int inputs_index = 0;
static int comparison_index = 0;
static bool initialized = false;
static ReplayGame game;
static Uint16 input_buffers[2] = { 0 };

static SDL_IOStream* io_at_index(int index) {
    const char* path = ram_path(index);
    SDL_IOStream* io = SDL_IOFromFile(path, "rb");
    SDL_free(path);
    return io;
}

static void set_cursor(Character character, int player) {
    Cursor_X[player] = character_to_cursor[character][0];
    Cursor_Y[player] = character_to_cursor[character][1];
}

/// Repeatedly press and release a button
static void mash_button(SWKey button, int player) {
    input_buffers[player] |= (frame & 1) ? button : 0;
}

static void tap_button(SWKey button, int player) {
    input_buffers[player] |= button;
}

static void initialize_data() {
    ReplayGame_Parse(&game);
    comparison_index = game.start_index;
}

static bool need_to_finish() {
    if (inputs_index >= arrlen(game.inputs)) {
        return true;
    }

    const bool game_ended = (PL_Wins[0] == 2) || (PL_Wins[1] == 2);

    if (game_ended) {
        return true;
    }

    return false;
}

static void finish() {
    exit(0);
}

static void apply_input_buffer(int id, Uint16 input) {
    Input_ButtonState state = { 0 };

    state.south = (input & SWK_SOUTH) ? true : false;
    state.east = (input & SWK_EAST) ? true : false;
    state.west = (input & SWK_WEST) ? true : false;
    state.north = (input & SWK_NORTH) ? true : false;
    state.back = (input & SWK_BACK) ? true : false;
    state.start = (input & SWK_START) ? true : false;
    state.left_stick = (input & SWK_LEFT_STICK) ? true : false;
    state.right_stick = (input & SWK_RIGHT_STICK) ? true : false;
    state.left_shoulder = (input & SWK_LEFT_SHOULDER) ? true : false;
    state.right_shoulder = (input & SWK_RIGHT_SHOULDER) ? true : false;
    state.left_trigger = (input & SWK_LEFT_TRIGGER) ? SDL_MAX_SINT16 : 0;
    state.right_trigger = (input & SWK_RIGHT_TRIGGER) ? SDL_MAX_SINT16 : 0;
    state.dpad_up = (input & SWK_UP) ? true : false;
    state.dpad_down = (input & SWK_DOWN) ? true : false;
    state.dpad_left = (input & SWK_LEFT) ? true : false;
    state.dpad_right = (input & SWK_RIGHT) ? true : false;

    StatcheckInput_SetButtonState(id, &state);
}

void TestRunner_Prologue() {
    SDL_zeroa(input_buffers);

    if (!initialized) {
        initialize_data();
        initialized = true;
    }

    switch (phase) {
    case PHASE_TITLE:
        const struct _TASK* menu_task = &task[TASK_MENU];

        if (menu_task->r_no[0] == 0 && menu_task->r_no[1] == 1 && menu_task->r_no[2] == 3) {
            phase = PHASE_MENU;
            break;
        }

        mash_button(SWK_START, 0);
        break;

    case PHASE_MENU:
        if (G_No[1] == 1 && G_No[2] == 2) {
            // Even though we move cursor manually later, setting Last_My_char2 is required
            // for Last_Super_Arts to take effect
            Last_My_char2[0] = game.characters[0];
            Last_My_char2[1] = game.characters[1];
            Last_Super_Arts[0] = game.supers[0];
            Last_Super_Arts[1] = game.supers[1];
            phase = PHASE_CHARACTER_SELECT_TRANSITION;
            wait_timer = 60;
            break;
        }

        mash_button(SWK_SOUTH, 0);
        break;

    case PHASE_CHARACTER_SELECT_TRANSITION:
        wait_timer -= 1;

        if (wait_timer <= 0) {
            phase = PHASE_CHARACTER_SELECT;
        }

        break;

    case PHASE_CHARACTER_SELECT:
        switch (char_select_phase) {
        case 0:
            set_cursor(game.characters[0], 0);
            set_cursor(game.characters[1], 1);
            tap_button(SWK_START, 1);
            wait_timer = 20;
            char_select_phase = 1;
            break;

        case 1:
            wait_timer -= 1;

            if (wait_timer <= 0) {
                // We must set New_Challenger manually so that the game selects the correct stage.
                // If we set this var earlier it would be overwritten
                New_Challenger = game.new_challenger;
                char_select_phase = 2;
            }

            break;

        case 2:
            tap_button(color_to_keys[game.colors[0]], 0);
            tap_button(color_to_keys[game.colors[1]], 1);
            wait_timer = 45;
            char_select_phase = 3;
            break;

        case 3:
            wait_timer -= 1;

            if (wait_timer <= 0) {
                tap_button(SWK_SOUTH, 0);
                tap_button(SWK_SOUTH, 1);
                phase = PHASE_GAME_TRANSITION;
            }

            break;
        }

        break;

    case PHASE_GAME_TRANSITION:
        if (G_No[1] != 2) {
            // This skips the VS animation
            mash_button(SWK_ATTACKS, 0);
            break;
        }

        SDL_IOStream* io = io_at_index(comparison_index - 1);
        sync_values(io);
        SDL_CloseIO(io);
        phase = PHASE_GAME;
        /* fallthrough */

    case PHASE_GAME:
        const ReplayInput input = game.inputs[inputs_index];
        input_buffers[0] = input.p1;
        input_buffers[1] = input.p2;
        inputs_index += 1;

        if (need_to_finish()) {
            finish();
        }

        break;
    }

    apply_input_buffer(0, input_buffers[0]);
    apply_input_buffer(1, input_buffers[1]);
}

void TestRunner_Epilogue() {
    switch (phase) {
    case PHASE_GAME:
        SDL_IOStream* io = io_at_index(comparison_index);

        if (io == NULL) {
            break;
        }

        compare_values(io, frame);

        SDL_CloseIO(io);
        comparison_index += 1;
        break;

    default:
        // Do nothing
        break;
    }

    frame += 1;
}

#endif
