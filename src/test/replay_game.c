#if STATCHECK

#include "test/replay_game.h"
#include "arcade/arcade_constants.h"
#include "constants.h"
#include "test/ram_archive.h"
#include "test/test_runner_utils.h"

#include <SDL3/SDL.h>

static void adjust_character_numbers(ReplayGame* game) {
    for (int i = 0; i < 2; i++) {
        game->characters[i] = CHAR_ARCADE_TO_3SX(game->characters[i]);
    }
}

bool ReplayGame_Init(ReplayGame* game, const char* ram_archive_path) {
    SDL_zerop(game);

    if (!RamArchive_Init(&game->archive, ram_archive_path)) {
        SDL_Log("ReplayGame_Init: Failed to initialize RAM archive");
        return false;
    }

    for (int frame_num = 0;; frame_num++) {
        SDL_IOStream* io = RamArchive_GetFrame(&game->archive, frame_num);

        if (io == NULL) {
            break;
        }

        const Uint16 g_no_1 = read_u16(io, G_NO_OFFSET + 2);
        const Uint16 g_no_2 = read_u16(io, G_NO_OFFSET + 4);
        const Uint16 g_no_3 = read_u16(io, G_NO_OFFSET + 6);
        const bool game_just_started = (g_no_1 == 2) && (g_no_2 == 0) && (g_no_3 == 0);

        if (game_just_started) {
            SDL_SeekIO(io, MY_CHAR_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->characters, 2);

            SDL_SeekIO(io, SUPER_ARTS_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->supers, 2);

            SDL_SeekIO(io, NEW_CHALLENGER_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadU8(io, &game->new_challenger);

            SDL_SeekIO(io, PLAYER_COLOR_OFFSET, SDL_IO_SEEK_SET);
            SDL_ReadIO(io, game->colors, 2);

            adjust_character_numbers(game);
            game->start_index = frame_num + 1;
        }

        SDL_CloseIO(io);

        if (game_just_started) {
            break;
        }
    }

    return true;
}

void ReplayGame_Destroy(ReplayGame* game) {
    RamArchive_Destroy(&game->archive);
    SDL_zerop(game);
}

#endif
