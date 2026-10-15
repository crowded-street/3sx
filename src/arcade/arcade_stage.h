#ifndef ARCADE_STAGE_H
#define ARCADE_STAGE_H

#include "arcade/rom/rom.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

/// @param roms Loaded ROM sets, indexed by `RomGame`. Absent ones are `NULL`.
void ArcadeStage_Init(const Rom* const roms[ROM_GAME_COUNT]);

bool ArcadeStage_NeedsRom(RomGame game);
bool ArcadeStage_IsRomProcessed(RomGame game);

#endif
