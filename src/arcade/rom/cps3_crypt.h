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

/// Decrypts a program flash bank, whose chips are already merged.
/// @param dst Destination buffer, `size` bytes. Receives big-endian SH-2 words.
/// @param src Encrypted big-endian SH-2 words.
/// @param address SH-2 address of the first word. The keystream depends on it.
void Cps3_DecodeProgramFlash(Uint8* dst, const Uint8* src, size_t size, Uint32 address, Uint32 key1, Uint32 key2);

/// Merges the 16-bit lanes of a graphics SIMM. Graphics SIMMs are not encrypted.
/// Bytes are laid out in the order the sound chip reads them, same as MAME.
/// @param dst Destination buffer, `chip_size * chip_count` bytes.
/// @param chips Chips of the SIMM, `chip_size` bytes each. Even and odd chips form lane pairs.
/// @param chip_count Number of chips, up to 8. The SIMM may be partially populated.
void Cps3_DecodeGraphicsSimm(Uint8* dst, const Uint8* const chips[8], int chip_count, size_t chip_size);

/// Converts a graphics flash bank, which holds big-endian 16-bit words, to the layout of `Cps3_DecodeGraphicsSimm`.
void Cps3_DecodeGraphicsFlash(Uint8* dst, const Uint8* src, size_t size);

#endif
