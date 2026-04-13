#if ARCADE_ROM

#ifndef ARCADE_CHAR_DATA_H
#define ARCADE_CHAR_DATA_H

#include "constants.h"
#include "structs.h"

void ArcadeCharData_Init();
const CharInitData* ArcadeCharData_Get(Character character);

#endif

#endif // ARCADE_ROM
