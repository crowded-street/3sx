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
#include <stdbool.h>

void advance_opening_step(s16 index);

/* The scene step both scene files are built out of, and its two variants for
 * the steps that also set the object display or start an effect. Defined
 * alongside advance_opening_step, which they call. */
bool opening_cue_step(bool cue, s16 next, s16 bg);
bool opening_cue_step_disp(bool cue, s16 next, s16 bg, s16 disp);
void opening_cue_step_effect(bool cue, s16 next, s16 bg, s16 effect);
extern const s16 op_103_sound[12];

#endif
