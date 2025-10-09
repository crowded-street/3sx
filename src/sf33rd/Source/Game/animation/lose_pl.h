#ifndef LOSE_PL_H
#define LOSE_PL_H

#include "structs.h"
#include "types.h"

extern s16 lose_rno[3];
extern s16 lose_free[2];

void lose_player(PLW* wk);
void Lose_00000(PLW* wk);
void Lose_10000(PLW* wk);
void Lose_20000(PLW* wk);
void Lose_30000(PLW* wk);
void Normal_normal_Loser(PLW* wk);
void Judge_normal_loser(PLW* wk);
void meta_lose_pause(PLW* wk);

#endif
