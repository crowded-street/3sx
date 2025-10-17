#ifndef SYS_SUB_H
#define SYS_SUB_H

#include "structs.h"
#include "types.h"

#include <stdbool.h>

extern const struct _SAVE_W Game_Default_Data;

void Switch_Screen_Init(s32 /* unused */);
s32 Switch_Screen(u8 Wipe_Type);
s32 Switch_Screen_Revival(u8 Wipe_Type);
u16 Convert_User_Setting(s16 PL_id);
void Clear_Personal_Data(s16 PL_id);
s16 Check_Count_Cut(s16 PL_id, s16 Limit);
void Disp_Personal_Count(s16 PL_id, s8 counter);
void Setup_Play_Type();
void Clear_Flash_No();

/// @brief Check if a player wants to skip an animation
/// @return `true` if animation should be skipped, `false` otherwise
bool Cut_Cut_Cut();

void Score_Sub();
void Disp_Win_Record();
void Disp_Win_Record_Sub(u16 win_record, s16 zz);
s32 Button_Cut_EX(s16* Timer, s16 Limit_Time);
s32 Setup_Target_PL();
void Setup_Final_Grade();
void Clear_Win_Type();
void Clear_Disp_Ranking(s16 PL_id);
void Meltw(u16* s, u16* d, s32 file_ptr);
void Setup_ID();
void Game_Data_Init();
void Setup_IO_ConvDataDefault(s32 id);
void Save_Game_Data();
void Copy_Save_w();
void Copy_Check_w();
void Setup_Default_Game_Option();
s32 Check_Change_Contents();
void cpRevivalTask();
s32 Check_Menu_Task();
void Setup_Limit_Time();
void Setup_Training_Difficulty();
void Setup_BG(s16 BG_INDEX, s16 X, s16 Y);
void Setup_Virtual_BG(s16 BG_INDEX, s16 X, s16 Y);
void BG_move();
void BG_move_Ex(u8 ix);
void Basic_Sub();
void Basic_Sub_Ex();
s32 Check_PL_Load();
void BG_Draw_System();
u16 Check_Demo_Data(s16 PL_id);
void System_all_clear_Level_B();
s16 Cut_Cut_C_Timer();
void Switch_Priority_76();
s32 Cut_Cut_Sub(s16 xx);
bool Cut_Cut_Loser();
void Soft_Reset_Sub();
void Check_Replay();
void Check_Replay_Status(s16 PL_id, u8 Status);
s16 Check_SysDir_Page();
void Clear_Flash_Init(s16 level);
s16 Clear_Flash_Sub();
void Copy_Key_Disp_Work();
s32 Check_Extra_Setting();
void All_Clear_Random_ix();
void All_Clear_Timer();
void All_Clear_ETC();
void Setup_Net_Random_ix();
s32 Request_Fade(u16 fade_code);
s32 Check_Fade_Complete_SP();
s32 Check_Fade_Complete();
s32 Check_Ranking(s16 PL_id);
void Disp_Digit16x24(u32 Score_Buff, s16 Disp_X, s16 Disp_Y, s16 Color);
void Disp_Copyright();
void Initialize_EM_Candidate(s16 PL_id);
void Check_Same_CPU(s16 PL_id);
void All_Clear_Suicide();
void Clear_Break_Com(s16 PL_id);
void njWaitVSync_with_N();
void Meltw(u16* s, u16* d, s32 file_ptr);
void cpRevivalTask();
s32 Flash_Violent(WORK_Other* /* unused */, s32 /* unused */);

#endif
