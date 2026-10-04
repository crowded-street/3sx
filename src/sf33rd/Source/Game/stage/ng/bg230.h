#ifndef BG230_H
#define BG230_H

#include "types.h"

/// Frames before the intro starts to scroll and zoom out
extern s8 bg230_intro_wait;

/// Attack presses during the intro. Three skip it.
extern s8 bg230_intro_skips;

void BG230();

#endif
