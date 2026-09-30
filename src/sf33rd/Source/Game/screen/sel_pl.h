#ifndef SEL_PL_H
#define SEL_PL_H

#include "constants.h"
#include "types.h"

extern s16 Play_Type_1st;

s16 Select_Player();

/// Decide whether or not to switch to Shin Akuma's stage based on availability and current game mode
Stage Resolve_Akuma_Stage(Stage stage);

#endif
