#if STATCHECK

#include "test/test_runner_utils.h"
#include "args.h"

#include <SDL3/SDL.h>

const char* ram_path(int index) {
    const char* base_path = get_args()->statcheck.states_path;
    const char* result = NULL;
    SDL_asprintf(&result, "%s/frame_%08d.ram", base_path, index);
    return result;
}

Uint8 read_u8(SDL_IOStream* io, Sint64 offset) {
    Uint8 result;
    SDL_SeekIO(io, offset, SDL_IO_SEEK_SET);
    SDL_ReadU8(io, &result);
    return result;
}

Uint16 read_u16(SDL_IOStream* io, Sint64 offset) {
    Uint16 result;
    SDL_SeekIO(io, offset, SDL_IO_SEEK_SET);
    SDL_ReadU16BE(io, &result);
    return result;
}

Sint16 read_s16(SDL_IOStream* io, Sint64 offset) {
    Sint16 result;
    SDL_SeekIO(io, offset, SDL_IO_SEEK_SET);
    SDL_ReadS16BE(io, &result);
    return result;
}

#endif
