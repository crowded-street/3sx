#ifndef ARCADE_STAGE_H
#define ARCADE_STAGE_H

#include "arcade/rom/rom.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

/// Writes stage files that the PS2 data lacks to the cache, building them from the CPS3 ROM.
void ArcadeStage_Init(const Rom* rom);

bool ArcadeStage_IsShinAkumaStageAvailable();

#endif
