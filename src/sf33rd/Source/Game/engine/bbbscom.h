#ifndef BBBSCOM_H
#define BBBSCOM_H

#include "structs.h"
#include "types.h"

extern const BBBSTable* bbbs_table[][5];

void bbbs_com_execute(PLW* wk);
void bbbs_com_initialize();
void makeup_bonus_game_level(s16 ix);
s32 set_bonus_game_difficulty(s16 emid);
s32 set_bonus_game_nando(u16 swdat);
s32 katteni_bonus_nando(u16 swdat);

#endif
