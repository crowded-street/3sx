#ifndef COM_SUB_H
#define COM_SUB_H

#include "structs.h"
#include "types.h"

void End_Pattern(PLW* wk);
void Next_Be_Passive(PLW* wk, s32);
void Turn_Over_On(PLW* wk);
void Only_Shot(PLW* wk, s16 Lever_Data);
void Lever_On(PLW* wk, u16 LR_Lever, u16 UD_Lever);
void Lever_Off(PLW* wk);
void Pierce_On(PLW* wk);
void Setup_DENJIN_LEVEL(PLW* wk);
void Push_Shot(PLW* wk, s16 Power_Level);
void Keep_Away(PLW* wk, s16 Target_Pos, s16 Option);
void Search_Back_Term(PLW* wk, s16 Move_Value, s16 Next_Action, s16 Next_Menu);
void Approach_Walk(PLW* wk, s16 Target_Pos, s16 Option);
void Walk(PLW* wk, u16 Lever, s16 Time, s16 unused);
void Forced_Guard(PLW* wk, s16 Guard_Type);
void Provoke(PLW* wk, s16 Lever);
void Normal_Attack(PLW* wk, s16 Reaction, u16 Lever_Data);
void Normal_Attack_SP(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Time);
void Adjust_Attack(PLW* wk, s16 Reaction, u16 Lever_Data);
void Lever_Attack(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data);
void Lever_Attack_SP(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data, s16 Time);
s32 Setup_Guard_Lever(PLW* wk, u16 Lever);
s32 Check_Start_Lever_Attack(PLW* wk, u16 Lever, u16 Lever_Data);
void SA_Term(PLW* wk, u16 SA0, u16 SA1, u16 SA2, u16 Term_No);
void Check_SA(PLW* wk, s16 Next_Action, s16 Next_Menu);
void Check_EX(PLW* wk, s16 Next_Action, s16 Next_Menu);
void Check_SA_Full(PLW* wk, s16 Next_Action, s16 Next_Menu);
void Branch_Unit_Area(PLW* wk, s16 Next_Action, s16 Menu, s16 Menu_01, s16 Menu_02, s16 Menu_03);
void Com_Random_Select(PLW* wk, s16 Next_Action, s16 Menu_00, s16 Menu_01, s16 Menu_02, s16 Menu_03, s16 Rnd_Type);
void Branch_Wait_Area(PLW* wk, s16 Time_00, s16 Time_01, s16 Time_02, s16 Time_03);
void Wait(PLW* wk, s16 Time);
void Look(PLW* wk, s16 Time);
void Keep_Status(PLW* wk, u16 Lever_Data, s16 Option_Data);
void VS_Jump_Guard(PLW* wk);
void Wait_Lie(PLW* wk, u16 Lever_Data);
void Wait_Get_Up(PLW* wk, u16 Lever_Data, s16 Option);
void Wait_Attack_Complete(PLW* wk, u16 Lever_Data, s16 Option);
void Short_Range_Attack(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Next_Action, s16 Next_Menu);
void EM_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Exit_Number, s16 Next_Action, s16 Next_Menu);
void Jump(PLW* wk, s16 Time);
void Hi_Jump(PLW* wk, s16 Pl_Number, s16 Jump_Dir);
void Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir);
void Jump_Attack_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX,
                      s16 Range_JY, s16 J_Lever_Data);
void Hi_Jump_Attack(PLW* wk, s16 Reaction, s16 Time_Data, u16 Lever_Data, s16 Jump_Dir);
void Hi_Jump_Attack_Term(PLW* wk, s16 Range_X, s16 Range_Y, s16 Reaction, u16 Lever_Data, s16 Jump_Dir, s16 Range_JX,
                         s16 Range_JY, u16 J_Lever_Data);
void ORO_JA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Lever_Data, s16 RJX,
                 s16 RJY, u16 JLD);
void ORO_HJA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Lever_Data, s16 RJX,
                  s16 RJY, u16 JLD);
void Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot);
void J_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot);
void Rapid_Command_Attack(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Shot, u16 Time);
void ORO_JCA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Tech_Number,
                  s16 Power_Level, s16 Ex_Shot, s16 RJX, s16 RJY, u16 JLD);
void ORO_HJCA_Term(PLW* wk, s16 Reaction, s16 Jump_Dir, s16 JY, s16 Jump_Dir2, s16 RX, s16 RY, u16 Tech_Number,
                   s16 Power_Level, s16 Ex_Shot, s16 RJX, s16 RJY, u16 JLD);
void Jump_Command_Attack_Term(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot, s16 RX, s16 RY,
                              s16 Jump_Dir, s16 JRX, s16 JRY, u16 JLD);
void Hi_Jump_Command_Attack_Term(PLW* wk, s16 Reaction, u16 Tech_Number, s16 Power_Level, s16 Ex_Shot, s16 RX, s16 RY,
                                 s16 Jump_Dir, s16 JRX, s16 JRY, u16 JLD);
void Check_Store_Lever(PLW* wk, u16 Tech_Number, s16 Next_Action, s16 Next_Menu);
s32 Setup_Lv04(s16 xx);
s32 Setup_Lv08(s16 xx);
s32 Setup_Lv10(s16 xx);
s32 Setup_Lv18(s16 xx);
s32 Setup_VS_Catch_Data(PLW* wk);
void Next_Another_Menu(PLW* wk, s16 Next_Action, u16 Next_Menu);
s32 Select_Passive(PLW* wk);
void Check_BOSS(PLW* wk, u32 Next_Action, u16 Next_Menu);
void Check_BOSS_EX(PLW* wk, u32 Next_Action, u16 Next_Menu);
void ETC_Term(PLW* wk, s16 Exit_No, u32 Next_Action, u16 Next_Menu);
void SHELL_Term(PLW* wk, s16 Next_Command, s16 Exit_Number, s16 Next_Action, s16 Next_Menu, s16 unused); // unused arg
void Next_Be_Flip(PLW* wk, s16 xx);
s32 Check_Guard(PLW* wk);
s32 Check_Passive(PLW* wk);
void Check_Rapid(PLW* wk, u16 Tech_Number);
s32 Check_Shell_Another_in_Flip(PLW* wk);
s32 Ck_Area(PLW* wk);
void Ck_Distance(PLW* wk);
s32 Command_Type_00(PLW* wk, s16 Power_Level, u16 Tech_Number, s16 Ex_Shot);
s32 Command_Type_01(PLW* wk, s16 Power_Level, s16 Ex_Shot); // unused args
void Decide_Follow_Menu(PLW* wk);
s32 Flip_Term_Correct(PLW* wk);
void Rapid_Sub(PLW* wk);
void Select_Active(PLW* wk);
s32 Setup_EM_Rank_Index(PLW* wk);
s32 emLevelRemake(s32 now, s32 max, s32 exd);
s32 emGetMaxBlocking();
void Next_Be_Guard(PLW* wk, WORK* em, s16 Type_Of_Guard);

#endif
