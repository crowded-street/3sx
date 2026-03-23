#if DEBUG

#ifndef REPLAY_MATCH_H
#define REPLAY_MATCH_H

#include <SDL3/SDL_stdinc.h>

typedef struct ReplayInput {
    Uint16 p1;
    Uint16 p2;
} ReplayInput;

typedef struct ReplayRound {
    ReplayInput* inputs;
    size_t start_index;
    Uint8 winner;
} ReplayRound;

typedef struct ReplayGame {
    ReplayRound* rounds;
    Uint8 characters[2];
    Uint8 supers[2];
    Uint8 winner;
} ReplayGame;

typedef struct ReplayMatch {
    ReplayGame* games;
} ReplayMatch;

void ReplayMatch_Parse(ReplayMatch* match);
void ReplayMatch_Destroy(ReplayMatch* match);

#endif

#endif
