/*
 * Shared between the opening scene files.
 *
 * advance_opening_step is this campaign's own - it is the three-statement step
 * handover that PR #17 named - and the scenes on both sides of the cut call it.
 * op_103_sound is the decompilation's own table, defined next to the step
 * handover and read by scene 103; it had external linkage already and simply
 * had no declaration, because until now nothing outside the file could see it.
 * Nothing the decompilation wrote is widened.
 */

#ifndef OPENING_INTERNAL_H
#define OPENING_INTERNAL_H

#include "types.h"

void advance_opening_step(s16 index);
extern const s16 op_103_sound[12];

#endif
