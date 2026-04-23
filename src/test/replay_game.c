#if STATCHECK

#include "test/replay_game.h"
#include "arcade/arcade_constants.h"
#include "constants.h"
#include "test/test_runner_utils.h"

#include "stb/stb_ds.h"
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>

static Uint16 read_input_buff(SDL_IOStream* io, Sint64 offset) {
    const Uint16 raw_buff = read_u16(io, offset);
    Uint16 buff = 0;

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

static void adjust_character_numbers(ReplayGame* game) {
    for (int i = 0; i < 2; i++) {
        game->characters[i] = CHAR_ARCADE_TO_3SX(game->characters[i]);
    }
}

void ReplayGame_Parse(ReplayGame* game) {
    SDL_zerop(game);

    bool in_game = false;
    bool in_game_prev = false;
    bool did_set_char_data = false;

    for (int frame_num = 0;; frame_num++) {
        const char* path = ram_path(frame_num);
        SDL_IOStream* io = SDL_IOFromFile(path, "rb");
        SDL_free(path);

        if (io == NULL) {
            break;
        }

        const Uint16 g_no_1 = read_u16(io, G_NO_OFFSET + 2);
        const Uint16 g_no_2 = read_u16(io, G_NO_OFFSET + 4);
        const Uint16 g_no_3 = read_u16(io, G_NO_OFFSET + 6);
        const bool game_just_started = (g_no_1 == 2) && (g_no_2 == 0) && (g_no_3 == 0);

        if (game_just_started) {
            in_game = true;
        }

        // Read character and SA indices until we get to game.
        // This ensures we read the latest data

        if (in_game && !did_set_char_data) {
            SDL_SeekIO(io, MY_CHAR_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->characters, 2);

            SDL_SeekIO(io, SUPER_ARTS_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->supers, 2);

            SDL_SeekIO(io, NEW_CHALLENGER_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadU8(io, &game->new_challenger);

            SDL_SeekIO(io, PLAYER_COLOR_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->colors, 2);

            adjust_character_numbers(game);
            did_set_char_data = true;
        }

        // Parse inputs

        if (in_game && in_game_prev) {
            const ReplayInput input =
                (ReplayInput) { .p1 = read_input_buff(io, P1SW_0_OFFSET), .p2 = read_input_buff(io, P2SW_0_OFFSET) };
            arrput(game->inputs, input);

            if (game->start_index == 0) {
                game->start_index = frame_num;
            }
        }

        in_game_prev = in_game;
        SDL_CloseIO(io);
    }
}

void ReplayGame_Destroy(ReplayGame* game) {
    arrfree(game->inputs);
}

#endif
