#include "test/test_runner.h"
#include "main.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"

#include <SDL3/SDL.h>

#include <stdio.h>

#define MY_CHAR_OFFSET 0x11387
#define SUPER_ARTS_OFFSET 0x1138B

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
    const char* base_path = configuration.test.states_path;
    const size_t base_len = SDL_strlen(base_path);
    const size_t path_max_len = SDL_strlen(base_path) + 64;
    char* path = SDL_malloc(path_max_len);
    SDL_strlcpy(path, base_path, path_max_len);
    char filename[64];

    for (int frame_num = 1000;; frame_num++) {
        path[base_len] = '\0';
        SDL_snprintf(filename, sizeof(filename), "/frame_%08d.ram", frame_num);
        SDL_strlcat(path, filename, path_max_len);
        SDL_IOStream* io = SDL_IOFromFile(path, "r");

        if (io == NULL) {
            break;
        }

        SDL_SeekIO(io, MY_CHAR_OFFSET, SDL_IO_SEEK_SET);
        SDL_ReadIO(io, characters, 2);

        SDL_SeekIO(io, SUPER_ARTS_OFFSET, SDL_IO_SEEK_SET);
        SDL_ReadIO(io, super_arts, 2);

        SDL_CloseIO(io);

        if (characters[0] != -1 && characters[1] != -1 && super_arts[0] != -1 && super_arts[1] != -1) {
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
}

void TestRunner_Prologue() {
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
                char_select_phase = 4;
            }

            break;

        case 4:
            mash_button(SWK_ATTACKS, 0);
            break;
        }

        break;
    }
}

void TestRunner_Epilogue() {
    frame += 1;
    p1sw_buff = 0;
    p2sw_buff = 0;
}
