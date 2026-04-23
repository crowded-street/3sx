#if STATCHECK

#ifndef TEST_RUNNER_UTILS_H
#define TEST_RUNNER_UTILS_H

#include <SDL3/SDL_iostream.h>

const char* ram_path(int index);
Uint8 read_u8(SDL_IOStream* io, Sint64 offset);
Uint16 read_u16(SDL_IOStream* io, Sint64 offset);
Sint16 read_s16(SDL_IOStream* io, Sint64 offset);

#endif

#endif
