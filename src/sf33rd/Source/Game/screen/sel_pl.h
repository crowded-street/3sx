#ifndef SEL_PL_H
#define SEL_PL_H

#include "constants.h"
#include "types.h"

#include <stdbool.h>

extern s16 Play_Type_1st;

s16 Select_Player();

/// Whether characters are picked one after the other with a single cursor
bool Sel_PL_Is_Sequential();

/// Side that picks first when picking sequentially. Its pad then picks for the other side
s16 Sel_PL_Lead_ID();

/// Decide whether or not to switch to Shin Akuma's stage based on availability and current game mode
Stage Resolve_Akuma_Stage(Stage stage);

#endif
