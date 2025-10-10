#ifndef COM_PL_H
#define COM_PL_H

#include "structs.h"

extern const u16 Correct_Lv_Data[16];

u16 cpu_algorithm(PLW* wk);
void Next_Be_Free(PLW* wk);
s32 Check_Passive(PLW* wk);
void Check_Guard_Type(PLW* wk, WORK* em);
s32 Check_Flip_GO(PLW* wk, s16 xx);
s32 SetShellFlipLever(PLW* wk);
void Be_Catch(PLW* wk);

#endif
