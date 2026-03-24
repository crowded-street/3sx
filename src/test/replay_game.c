#if DEBUG

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
    // There's no Shin Akuma in PS2 version, which is why we have to decrement character numbers after Akuma
    for (int i = 0; i < 2; i++) {
        if (game->characters[i] > CHAR_AKUMA) {
            game->characters[i] -= 1;
        }
    }
}

void ReplayGame_Parse(ReplayGame* game) {
    SDL_zerop(game);

    bool in_round = false;
    bool in_round_prev = false;
    bool allow_battle_prev = false;
    bool did_set_char_data = false;

    ReplayRound round;
    SDL_zero(round);

    for (int frame_num = 0;; frame_num++) {
        const char* path = ram_path(frame_num);
        bool stop = false;
        SDL_IOStream* io = SDL_IOFromFile(path, "rb");
        SDL_free(path);

        if (io == NULL) {
            break;
        }

        const bool allow_battle = read_u8(io, ALLOW_A_BATTLE_F_OFFSET);
        const Uint16 c_no_0 = read_u16(io, C_NO_OFFSET);
        const Uint16 c_no_1 = read_u16(io, C_NO_OFFSET + 2);
        const bool round_just_started = (c_no_0 == 1) && (c_no_1 == 4);

        if (round_just_started) {
            in_round = true;
        } else if (allow_battle_prev && !allow_battle) {
            in_round = false;
        }

        // Read character and SA indices until we get to game.
        // This ensures we read the latest data

        if (in_round && !did_set_char_data) {
            SDL_SeekIO(io, MY_CHAR_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->characters, 2);

            SDL_SeekIO(io, SUPER_ARTS_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->supers, 2);

            adjust_character_numbers(game);
            did_set_char_data = true;
        }

        // Parse inputs

        if (in_round && in_round_prev) {
            // We read previous inputs because CPS3 updates input buffers at the end of a frame
            const ReplayInput input =
                (ReplayInput) { .p1 = read_input_buff(io, P1SW_1_OFFSET), .p2 = read_input_buff(io, P2SW_1_OFFSET) };
            arrput(round.inputs, input);

            if (round.start_index == 0) {
                round.start_index = frame_num;
            }
        } else if (in_round_prev) {
            arrput(game->rounds, round);
            SDL_zero(round);
        }

        in_round_prev = in_round;
        allow_battle_prev = allow_battle;
        SDL_CloseIO(io);

        if (stop) {
            break;
        }
    }
}

void ReplayGame_Destroy(ReplayGame* game) {
    for (int i = 0; i < arrlen(game->rounds); i++) {
        arrfree(game->rounds[i].inputs);
    }

    arrfree(game->rounds);
}

#endif
