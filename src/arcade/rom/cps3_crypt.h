#ifndef CPS3_CRYPT_H
#define CPS3_CRYPT_H

#include <SDL3/SDL.h>

#include <stddef.h>

/// Merges the four byte lanes of a program SIMM and decrypts it.
/// @param dst Destination buffer, `chip_size * 4` bytes. Receives big-endian SH-2 words.
/// @param chips Chips 0–3 of the SIMM, `chip_size` bytes each. Chip 0 holds the most significant byte of each word.
/// @param address SH-2 address of the first word. The keystream depends on it.
void Cps3_DecodeProgramSimm(
    Uint8* dst, const Uint8* const chips[4], size_t chip_size, Uint32 address, Uint32 key1, Uint32 key2
);

/// Merges the 16-bit lanes of a graphics SIMM. Graphics SIMMs are not encrypted.
/// Bytes are laid out in the order the sound chip reads them, same as MAME.
/// @param dst Destination buffer, `chip_size * 8` bytes.
/// @param chips Chips 0–7 of the SIMM, `chip_size` bytes each. Even and odd chips form lane pairs.
void Cps3_DecodeGraphicsSimm(Uint8* dst, const Uint8* const chips[8], size_t chip_size);

#endif
