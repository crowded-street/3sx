#ifndef ROM_H
#define ROM_H

#include <SDL3/SDL.h>

#include <stddef.h>

/// SH-2 address at which SIMM1 is mapped. SIMM2 follows it directly.
#define ROM_PROGRAM_BASE 0x06000000

typedef enum RomGame {
    ROM_GAME_SFIII3,
    ROM_GAME_COUNT,
} RomGame;

/// A CPS3 ROM set with every SIMM unpacked and decoded.
typedef struct Rom Rom;

/// @return File name of the .zip holding the ROM set, relative to resources.
const char* Rom_GetZipName(RomGame game);

/// Unzips and decodes every SIMM of the ROM set.
/// @param path Path to the .zip file with the ROM set.
/// @return New ROM on success, `NULL` on failure. Call `SDL_GetError` for details.
Rom* Rom_Create(RomGame game, const char* path);

void Rom_Destroy(Rom* rom);

RomGame Rom_GetGame(const Rom* rom);

/// @param simm SIMM number, 1–6. SIMM1 and SIMM2 hold program data, SIMM3–6 hold graphics.
/// @return Decoded contents of the SIMM, or `NULL` if the ROM set has no such SIMM.
const Uint8* Rom_GetSimm(const Rom* rom, int simm, size_t* size);

/// @return Decrypted program SIMMs as one block in big-endian byte order, starting at `ROM_PROGRAM_BASE`.
const Uint8* Rom_GetProgram(const Rom* rom, size_t* size);

/// @return Graphics SIMMs as one block, starting with SIMM3.
const Uint8* Rom_GetGraphics(const Rom* rom, size_t* size);

#endif
