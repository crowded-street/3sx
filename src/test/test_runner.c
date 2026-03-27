#if DEBUG

#include "test/test_runner.h"
#include "arcade/arcade_constants.h"
#include "constants.h"
#include "main.h"
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
    PHASE_ROUND_TRANSITION,
    PHASE_ROUND,
} Phase;

static const Uint8 character_to_cursor[20][2] = { { 7, 1 }, { 1, 0 }, { 5, 2 }, { 6, 1 }, { 3, 2 }, { 4, 0 }, { 1, 2 },
                                                  { 3, 0 }, { 2, 2 }, { 4, 2 }, { 0, 1 }, { 0, 2 }, { 2, 0 }, { 5, 0 },
                                                  { 6, 0 }, { 3, 1 }, { 2, 1 }, { 4, 1 }, { 1, 1 }, { 5, 1 } };

static Uint64 frame = 0;
static Phase phase = PHASE_TITLE;
static int char_select_phase = 0;
static int wait_timer = 0;
static int inputs_index = 0;
static int comparison_index = 0;

static bool initialized = false;
static ReplayGame game;
static int round_index = 0;

static SDL_IOStream* io_at_index(int index) {
    const char* path = ram_path(index);
    SDL_IOStream* io = SDL_IOFromFile(path, "rb");
    SDL_free(path);
    return io;
}

static ReplayRound* _round() {
    return &game.rounds[round_index];
}

static void set_cursor(Character character, int player) {
    Cursor_X[player] = character_to_cursor[character][0];
    Cursor_Y[player] = character_to_cursor[character][1];
}

/// Repeatedly press and release a button
static void mash_button(SWKey button, int player) {
    u16* dst = player ? &p2sw_buff : &p1sw_buff;
    *dst |= (frame & 1) ? button : 0;
}

static void tap_button(SWKey button, int player) {
    u16* dst = player ? &p2sw_buff : &p1sw_buff;
    *dst |= button;
}

static void initialize_data() {
    ReplayGame_Parse(&game);
    round_index = 0;
}

static void reset_comparison_index() {
    comparison_index = _round()->start_index;
}

static void finish_round() {
    inputs_index = 0;

    if (round_index < arrlen(game.rounds) - 1) {
        round_index += 1;
        phase = PHASE_ROUND_TRANSITION;
    } else {
        exit(0);
    }

    reset_comparison_index();
}

void TestRunner_Prologue() {
    if (frame == SDL_MAX_UINT64) {
        raise(SIGSTOP);
    }

    p1sw_buff = 0;
    p2sw_buff = 0;

    if (!initialized) {
        initialize_data();
        reset_comparison_index();
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
            tap_button(SWK_SOUTH, 0);
            tap_button(SWK_SOUTH, 1);
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
        if (G_No[1] == 2) {
            phase = PHASE_ROUND_TRANSITION;
        } else {
            // This skips the VS animation
            mash_button(SWK_ATTACKS, 0);
        }

        break;

    case PHASE_ROUND_TRANSITION:
        if (C_No[0] != 1 || C_No[1] != 4) {
            break;
        }

        SDL_IOStream* io = io_at_index(comparison_index - 1);
        sync_values(io);
        SDL_CloseIO(io);

        phase = PHASE_ROUND;
        // fallthrough

    case PHASE_ROUND:
        ReplayInput* inputs = _round()->inputs;
        const ReplayInput input = inputs[inputs_index];
        p1sw_buff = input.p1;
        p2sw_buff = input.p2;
        inputs_index += 1;

        if (inputs_index >= arrlen(inputs)) {
            finish_round();
        }

        break;
    }
}

void TestRunner_Epilogue() {
    switch (phase) {
    case PHASE_ROUND:
        SDL_IOStream* io = io_at_index(comparison_index);

        if (io == NULL) {
            break;
        }

        compare_values(io);

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
