#ifndef SPU_H_
#define SPU_H_

#include "common.h"
#include <SDL3/SDL_mutex.h>

struct SPUVConf {
    u32 ssa;
    u32 pitch;
    u32 voll, volr;
    u16 adsr1, adsr2;
};

extern SDL_Mutex* soundLock;

void SPU_Init(void (*cb)());
void SPU_Upload(u32 dst, void* src, u32 size);
void SPU_Tick(s16* output);
void SPU_StartVoice(int vnum, struct SPUVConf* conf);
int SPU_VoiceGetVolume(int vnum);
void SPU_KeyOffVoice(int vnum);
void SPU_StopVoice(int vnum);

#endif // SPU_H_
