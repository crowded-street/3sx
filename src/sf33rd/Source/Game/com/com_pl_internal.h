/*
 * Shared between the com_pl files.
 *
 * The damage states and the flip states are reached through jump tables built
 * in com_pl.c, so the ones that moved out need declaring. Every one of them had
 * external linkage in the decompilation already - what is file-local stayed
 * with its callers, which is why the mode entry points did not move.
 */

#ifndef COM_PL_INTERNAL_H
#define COM_PL_INTERNAL_H

#include "structs.h"
#include "types.h"

s32 Check_Flip(PLW* wk);
s32 Com_Rapid_Sub(PLW* wk, s16 Shot, u8* dir_step);
s32 Command_Attack_SP(PLW* wk, s8 Pl_Number, s16 Tech_Number, s16 Power_Level);
void Clear_Com_Flag(PLW* wk);
void Exit_Damage_Sub(PLW* wk);
void Next_Be_Float(PLW* wk);

#endif
