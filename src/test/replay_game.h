#if STATCHECK

#ifndef REPLAY_GAME_H
#define REPLAY_GAME_H

#include "test/ram_archive.h"

#include <SDL3/SDL.h>

typedef struct ReplayGame {
    size_t start_index;
    Uint8 characters[2];
    Uint8 supers[2];
    Uint8 colors[2];
    Uint8 new_challenger;
    RamArchive archive;
} ReplayGame;

bool ReplayGame_Init(ReplayGame* game, const char* ram_archive_path);
void ReplayGame_Destroy(ReplayGame* game);

#endif

#endif
