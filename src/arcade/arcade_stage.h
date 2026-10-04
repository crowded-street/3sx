#ifndef ARCADE_STAGE_H
#define ARCADE_STAGE_H

#include "arcade/rom/rom.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

/// Writes stage files that the PS2 data lacks to the cache, building them from the CPS3 ROM.
/// @param roms Loaded ROM sets, indexed by `RomGame`. Absent ones are `NULL`.
void ArcadeStage_Init(const Rom* const roms[ROM_GAME_COUNT]);

/// @return Whether the stages that come from the ROM set have been converted, so they are all available.
bool ArcadeStage_IsRomProcessed(RomGame game);

#endif
