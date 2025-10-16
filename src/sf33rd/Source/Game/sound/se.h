#ifndef SE_H
#define SE_H

#include "structs.h"
#include "types.h"

extern u8 gSeqStatus[1];

void Stage_BGM(u16 Stage_Number, u16 Round_Number);
void Sound_SE(s16 Code);
void BGM_Request(s16 Code);
void BGM_Request_Code_Check(u16 Code);
void BGM_Stop();
void SE_All_Off();
void Se_Dummy(WORK_Other* ewk, u16 Code);
void Se_Shock(WORK_Other* ewk, u16 Code);
void Se_Myself(WORK_Other* ewk, u16 Code);
void Se_Myself_Die(WORK_Other* ewk, u16 Code);
void Se_Let(WORK_Other* ewk, u16 Code);
void Se_Let_SP(WORK_Other* ewk, u16 Code);
void Call_Se(WORK_Other* ewk, u16 Code);
void Se_Term(WORK_Other* ewk, u16 Code);
void Finish_SE();
s32 Check_Finish_SE();
u16 Get_Position(PLW* wk);
u16 Check_Bonus_SE(u16 Code);
void Store_Sound_Code(u16 code, SoundPatchConfig* rmc);
void Disp_Sound_Code();

#endif
