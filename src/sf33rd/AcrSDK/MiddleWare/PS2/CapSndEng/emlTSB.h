#ifndef EML_TSB_H
#define EML_TSB_H

#include "structs.h"
#include "types.h"

#define TSB_MAX 16
#define ECHOWORK_MAX 16

extern SoundEvent* gpTsb[TSB_MAX];
extern CSE_ECHOWORK EchoWork[ECHOWORK_MAX];

s32 mlTsbInit();
s32 mlTsbExecServer();
s32 mlTsbStopAll();
s32 mlTsbSetBankAddr(u32 bank, SoundEvent* addr);
SoundEvent* mlTsbGetDataAdrs(u32 bank, u32 code);
s32 mlTsbRequest(u16 bank, u16 code, s32* aRtpc);

#endif
