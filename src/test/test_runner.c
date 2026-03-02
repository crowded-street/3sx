#include "test/test_runner.h"
#include "main.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"

#include <SDL3/SDL.h>

#include <stdio.h>

#define MY_CHAR_OFFSET 0x11387
#define SUPER_ARTS_OFFSET 0x1138B
#define GAME_ROUTINE_OFFSET 0x15438
#define P1SW_OFFSET 0x6AA8C
#define P2SW_OFFSET 0x6AA90

#define REPLAY_FRAMES_MAX 3 * 100 * 60

#define SWAP16(val) ((val << 8) | (val >> 8))

typedef enum Character {
    CHAR_GILL = 0,
    CHAR_ALEX = 1,
    CHAR_RYU = 2,
    CHAR_YUN = 3,
    CHAR_DUDLEY = 4,
    CHAR_NECRO = 5,
    CHAR_HUGO = 6,
    CHAR_IBUKI = 7,
    CHAR_ELENA = 8,
    CHAR_ORO = 9,
    CHAR_YANG = 10,
    CHAR_KEN = 11,
    CHAR_SEAN = 12,
    CHAR_URIEN = 13,
    CHAR_AKUMA = 14,
    CHAR_CHUNLI = 15,
    CHAR_MAKOTO = 16,
    CHAR_Q = 17,
    CHAR_TWELVE = 18,
    CHAR_REMY = 19,
} Character;

typedef enum Phase {
    PHASE_INIT,
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

static Uint64 frame = 0;
static Phase phase = PHASE_INIT;
static int char_select_phase = 0;
static int wait_timer = 0;
static Sint8 characters[2] = { -1, -1 };
static Sint8 super_arts[2] = { -1, -1 };
static u16 inputs[REPLAY_FRAMES_MAX][2] = { 0 };
static int inputs_index = 0;

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

static u16 read_u16(SDL_IOStream* io, Sint64 offset) {
    u16 result;
    SDL_SeekIO(io, offset, SDL_IO_SEEK_SET);
    SDL_ReadIO(io, &result, 2);
    return SWAP16(result);
}

static u16 read_input_buff(SDL_IOStream* io, Sint64 offset) {
    const u16 raw_buff = read_u16(io, offset);
    u16 buff = 0;

    buff |= raw_buff & 0xF;              // directions
    buff |= raw_buff & (1 << 4);         // LP
    buff |= raw_buff & (1 << 5);         // MP
    buff |= raw_buff & (1 << 6);         // HP
    buff |= (raw_buff & (1 << 7)) << 1;  // LK
    buff |= (raw_buff & (1 << 8)) << 1;  // MK
    buff |= (raw_buff & (1 << 9)) << 1;  // HK
    buff |= (raw_buff & (1 << 12)) << 2; // start

    return buff;
}

static void initialize_data() {
    const char* base_path = configuration.test.states_path;
    const size_t base_len = SDL_strlen(base_path);
    const size_t path_max_len = SDL_strlen(base_path) + 64;
    char* path = SDL_malloc(path_max_len);
    SDL_strlcpy(path, base_path, path_max_len);
    char filename[64];
    bool in_game_prev = false;
    bool did_set_char_data = false;

    for (int frame_num = 0;; frame_num++) {
        bool stop = false;
        path[base_len] = '\0';
        SDL_snprintf(filename, sizeof(filename), "/frame_%08d.ram", frame_num);
        SDL_strlcat(path, filename, path_max_len);
        SDL_IOStream* io = SDL_IOFromFile(path, "rb");

        if (io == NULL) {
            break;
        }

        const u16 routine = read_u16(io, GAME_ROUTINE_OFFSET);
        const bool in_game = (routine == 2);

        // Read character and SA indices until we get to game.
        // This ensures we read the latest data

        if (in_game && !did_set_char_data) {
            SDL_SeekIO(io, MY_CHAR_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, characters, 2);

            SDL_SeekIO(io, SUPER_ARTS_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, super_arts, 2);

            did_set_char_data = true;
        }

        // Parse inputs

        if (in_game) {
            inputs[inputs_index][0] = read_input_buff(io, P1SW_OFFSET);
            inputs[inputs_index][1] = read_input_buff(io, P2SW_OFFSET);
            inputs_index += 1;
        } else if (in_game_prev) {
            stop = true;
        }

        in_game_prev = in_game;
        SDL_CloseIO(io);

        if (stop) {
            break;
        }
    }

    SDL_free(path);

    // There's no Shin Akuma in PS2 version, which is why we have to decrement character numbers after Akuma
    for (int i = 0; i < 2; i++) {
        if (characters[i] > CHAR_AKUMA) {
            characters[i] -= 1;
        }
    }

    inputs_index = 0;
}

void TestRunner_Prologue() {
    p1sw_buff = 0;
    p2sw_buff = 0;

    switch (phase) {
    case PHASE_INIT:
        initialize_data();
        phase = PHASE_TITLE;
        // fallthrough

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
            Last_My_char2[0] = characters[0];
            Last_My_char2[1] = characters[1];
            Last_Super_Arts[0] = super_arts[0];
            Last_Super_Arts[1] = super_arts[1];
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
            set_cursor(characters[0], 0);
            set_cursor(characters[1], 1);
            tap_button(SWK_START, 1);
            wait_timer = 20;
            char_select_phase = 1;
            break;

        case 1:
            wait_timer -= 1;

            if (wait_timer <= 0) {
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
            phase = PHASE_GAME;
        } else {
            // This skips the VS animation
            mash_button(SWK_ATTACKS, 0);
            break;
        }

        // fallthrough

    case PHASE_GAME:
        p1sw_buff = inputs[inputs_index][0];
        p2sw_buff = inputs[inputs_index][1];
        inputs_index += 1;
        break;
    }
}

void TestRunner_Epilogue() {
    frame += 1;
}
