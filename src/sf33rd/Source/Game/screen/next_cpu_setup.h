#ifndef NEXT_CPU_SETUP_H
#define NEXT_CPU_SETUP_H

#include "types.h"

void Setup_EM_List();
void Setup_Next_Fighter();
s8 Setup_Com_Arts();
void Setup_Com_Color();
void Setup_Regular_OBJ(s16 PL_id);
void Regular_OBJ_Sub(s16 PL_id, s16 Dir);
void Setup_History_OBJ();
void Setup_VS_OBJ(s16 Option);
s8 Check_Bonus_Type();
void Setup_Next_Stage(s16 dir_step);
void Check_Auto_Cut();

#endif // NEXT_CPU_SETUP_H
