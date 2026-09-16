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

/* Defined in manage_result.c - the scoring, grading and continue helpers the
 * phase machine in manage.c calls into. All already had external linkage. */
void Check_Naming(s16 PL_id);
s32 Check_Ending();
s32 Check_Ending_Sub();
void Additional_Bonus(s16 PL_id);
u32 Setup_Comp_Bonus();
void Setup_Win_Mark();
void Check_Perfect(s16 PL_id);
void Update_VS_Data();
void BGM_Fade_Sub();
void BGM_Control();
void Setup_BGM_Fade_In(s16 Time);
void Check_Stage_BGM();
void Check_Conclusion_Type();
void chkComWins();
void Update_BI_Term();
void Ck_Win_Record();
void Update_Level_Control();
s32 Judge_Next_Disposal();
void Quick_Entry();
s32 Check_Entry_Again();
void Loser_Sub();
void Be_Continue();
void Disp_Winner();
void Pool_Score(s16 PL_id);
s32 Check_Break_Into_CPU(s16 PL_id);
void Judge_Winner();
s32 Check_Disp_Winner();
void Check_Fade_Out_BGM(s16 Time);
s32 Check_BI_Grade(s16 PL_id);

#endif // MANAGE_INTERNAL_H
