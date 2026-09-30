#include "arcade/rom/cps3_crypt.h"

#include <SDL3/SDL.h>

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

void Cps3_DecodeProgramSimm(
    Uint8* dst, const Uint8* const chips[4], size_t chip_size, Uint32 address, Uint32 key1, Uint32 key2
) {
    for (size_t i = 0; i < chip_size; i++) {
        const Uint32 encrypted =
            ((Uint32)chips[0][i] << 24) | ((Uint32)chips[1][i] << 16) | ((Uint32)chips[2][i] << 8) | chips[3][i];
        const Uint32 word = encrypted ^ cps3_mask(address + (Uint32)i * 4, key1, key2);
        Uint8* out = dst + i * 4;
        out[0] = word >> 24;
        out[1] = word >> 16;
        out[2] = word >> 8;
        out[3] = word;
    }
}

void Cps3_DecodeGraphicsSimm(Uint8* dst, const Uint8* const chips[8], size_t chip_size) {
    for (int pair = 0; pair < 4; pair++) {
        Uint8* out = dst + pair * chip_size * 2;
        const Uint8* even = chips[pair * 2];
        const Uint8* odd = chips[pair * 2 + 1];

        for (size_t i = 0; i < chip_size; i++) {
            out[i * 2] = odd[i];
            out[i * 2 + 1] = even[i];
        }
    }
}
