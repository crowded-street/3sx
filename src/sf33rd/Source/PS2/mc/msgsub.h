#ifndef MSGSUB_H
#define MSGSUB_H

#include "types.h"

extern s32 MsgLanguage;

s8** GetMemCardMsg(s32 msg_no);
s32 FormStrDisp(s32 x, s32 y, const s8* str, s32 len);

#endif
