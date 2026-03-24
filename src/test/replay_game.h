#if DEBUG

#ifndef REPLAY_GAME_H
#define REPLAY_GAME_H

#include <SDL3/SDL_stdinc.h>

typedef struct ReplayInput {
    Uint16 p1;
    Uint16 p2;
} ReplayInput;

typedef struct ReplayRound {
    ReplayInput* inputs;
    size_t start_index;
} ReplayRound;

typedef struct ReplayGame {
    ReplayRound* rounds;
    Uint8 characters[2];
    Uint8 supers[2];
    Uint8 new_challenger;
} ReplayGame;

void ReplayGame_Parse(ReplayGame* game);
void ReplayGame_Destroy(ReplayGame* game);

#endif

#endif
