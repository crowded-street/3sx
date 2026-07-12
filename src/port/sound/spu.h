#ifndef SPU_H_
#define SPU_H_

#include "common.h"
#include <SDL3/SDL_mutex.h>

#define SPU_VOICE_COUNT 24

extern SDL_Mutex* soundLock;

void SPU_Init(void (*cb)());
void SPU_Upload(u32 dst, void* src, u32 size);
void SPU_VoiceStart(int vnum, u32 start_addr, u32 size);
void SPU_VoiceSetPitch(int vnum, int pitch);
void SPU_VoiceSetVolume(int vnum, int voll, int volr);
void SPU_VoiceSetADSR(int vnum, u16 adsr1, u16 adsr2);
bool SPU_VoiceIsFinished(int vnum);
void SPU_VoiceKeyOff(int vnum);
void SPU_VoiceStop(int vnum);

#endif // SPU_H_
