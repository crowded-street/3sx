/*
 * Shared between the game flow files.
 *
 * The round, result and demo states are reached from the dispatcher in game.c
 * and call back into it, so the ones that cross the cut need declaring. Every
 * one had external linkage in the decompilation already; what is file-local
 * stayed with its callers.
 */

#ifndef GAME_INTERNAL_H
#define GAME_INTERNAL_H

#include "structs.h"
#include "types.h"

void Wait_Auto_Load(struct _TASK* /* unused */);
void Loop_Demo(struct _TASK* /* unused */);
void Game();
void Game00();
void Game01();
void Game02();
void Game03();
void Game04();
void Game05();
void Game06();
void Game07();
void Game08();
void Game09();
void Game10();
void Game11();
void Game12();
void Check_Back_Demo();
void Game0_0();
void Game0_1();
void Game0_2();
void Next_Demo_Loop();
void Game12_0();
void Game12_1();
void Game12_2();
void Game2_0();
void Game2_1();
void Game2_2();
void Game2_3();
void Game2_4();
void Game2_5();
void Game2_6();
void Game2_7();
void Time_Control();
s32 Disp_Ranking();
void Request_Break_Sub(s16 PL_id);
s16 Disp_Rank_Sub(s16 PL_id);
s16 Ck_Coin();
void Loop_Demo_Sub();
void Before_Select_Sub();

#endif
