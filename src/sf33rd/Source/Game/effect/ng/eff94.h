#ifndef EFF94_NG_H
#define EFF94_NG_H

#include "structs.h"
#include "types.h"

#define EFFECT_NG94_ID 239

/// First ColorRAM row of the tanuki palettes, one for each of Ibuki's NG colors
#define EFF94_PALETTE_ROW 0x1D0

void effect_ng94_move(WORK_Other* ewk);
s32 effect_ng94_init_for_player(s16 player_index, s16 variant);
void effect_ng94_init_for_players(s16 variant);

#endif
