#if STATCHECK

#ifndef TEST_RUNNER_COMPARE_H
#define TEST_RUNNER_COMPARE_H

#include <SDL3/SDL_iostream.h>

void compare_values(SDL_IOStream* io, Uint64 frame);
void sync_values(SDL_IOStream* io);

#endif

#endif
