#ifndef ENTRY_H
#define ENTRY_H

#include "structs.h"
#include "types.h"

extern const u8 Coin_Message_Data[7][2];

void Entry_Task(struct _TASK*);
s32 Ck_Break_Into(u16 Sw_0, u16 Sw_1, s16 PL_id);

#endif // ENTRY_H
