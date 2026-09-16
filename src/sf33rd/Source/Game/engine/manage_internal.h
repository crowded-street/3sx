#ifndef MANAGE_INTERNAL_H
#define MANAGE_INTERNAL_H

#include "structs.h"
#include "types.h"

/* Shared between manage.c and manage_bonus.c, which were one 2549-line file.
 * Every name here already had external linkage; the split widened nothing. */

/* Defined in manage.c and already non-static. Declared here so the split did
 * not move or touch a const table. */
extern u8 Disp_Bonus_Contents;
extern s8 MANAGE_X;
extern const u32 Comp_Bonus_Data[11];
extern const u8 BIC_SA_Data[2][4];
extern const u32 Ball_Perfect_PTS[2][5];

/* Defined in manage_bonus.c. Game_Management's phase table reaches the bonus
 * stage through Game_Manage_12th, and the phases chain through each other. */
void Game_Manage_12th();
void Game_Manage_12_0();
void Game_Manage_12_1();
void Game_Manage_12_2();
void Game_Manage_12_3();
void Game_Manage_12_4();
void Game_Manage_12_5();
void Game_Manage_12_7();
void Game_Manage_12_8();
void Disp_Bonus_Perfect();
void Flash_Bonus_Perfect();
u8 Check_Bonus_Perfect();
u32 Setup_Final_Score(s16 Type);
s32 Bonus_Cut_Sub();

/* Defined in manage.c; the bonus phases use these. */
void Check_Fade_Out_BGM(s16 Time);
s16 Check_Time_Over();

#endif // MANAGE_INTERNAL_H
