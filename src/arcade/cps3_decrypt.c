#include "arcade/cps3_decrypt.h"

#include <SDL3/SDL.h>

#define BASE_OFFSET 0x6000000
#define KEY_1 0xA55432B4
#define KEY_2 0x0C129981

static Uint32 rotate_left(Uint32 value, Uint32 n) {
    value &= 0xFFFF;
    const Uint32 aux = value >> (16 - n);
    return ((value << n) | aux) % 0x10000;
}

static Uint32 rotxor(Uint32 val, Uint32 xorval) {
    val &= 0xFFFF;
    xorval &= 0xFFFF;
    Uint32 res = val + rotate_left(val, 2);
    res = rotate_left(res, 4) ^ (res & (val ^ xorval));
    return res;
}

static Uint32 cps3_mask(Uint32 address, Uint32 key1, Uint32 key2) {
    address ^= key1;
    Uint32 val = (address & 0xFFFF) ^ 0xFFFF;
    val = rotxor(val, key2 & 0xFFFF);
    val ^= (address >> 16) ^ 0xFFFF;
    val = rotxor(val, key2 >> 16);
    val ^= (address & 0xFFFF) ^ (key2 & 0xFFFF);
    return val | (val << 16);
}

Uint32 cps3_decrypt(Uint8 b0, Uint8 b1, Uint8 b2, Uint8 b3, Uint32 i) {
    const Uint32 cur_data = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
    const Uint32 masked = cps3_mask(BASE_OFFSET + (i * 4), KEY_1, KEY_2);
    return SDL_Swap32BE(cur_data ^ masked);
}
