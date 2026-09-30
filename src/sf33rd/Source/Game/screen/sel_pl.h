#ifndef SEL_PL_H
#define SEL_PL_H

#include "constants.h"
#include "types.h"

extern s16 Play_Type_1st;

s16 Select_Player();

/// Applied whenever the game picks a stage itself. Akuma's stage becomes Shin Akuma's half the time (never in netplay),
/// and Shin Akuma's stage falls back to Akuma's when it isn't available.
Stage Resolve_Akuma_Stage(Stage stage);

#endif
