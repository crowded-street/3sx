#ifndef ARCADE_STAGE_H
#define ARCADE_STAGE_H

#include "arcade/rom/rom.h"
#include "constants.h"

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stddef.h>

/// Builds stage data that the PS2 files lack from the CPS3 ROM. The data is copied, so `rom` can be freed afterwards.
void ArcadeStage_Init(const Rom* rom);

void ArcadeStage_Finish();

/// @param count Receives the number of colors.
/// @return BG palette in the same format as the PS2 `bgNN0.bin` files, or `NULL` if none was built for the stage.
const Uint16* ArcadeStage_GetPalette(Stage stage, size_t* count);

bool ArcadeStage_IsShinAkumaStageAvailable();

#endif
