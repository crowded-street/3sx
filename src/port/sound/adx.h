#ifndef SOUND_ADX_H
#define SOUND_ADX_H

#include <stdbool.h>
#include <stddef.h>

typedef enum ADXState {
    ADX_STATE_STOP,
    ADX_STATE_PLAYING,
    ADX_STATE_PLAYEND,
} ADXState;

void ADX_ProcessTracks();

void ADX_Init();
void ADX_Exit();
void ADX_Stop();
int ADX_IsPaused();
void ADX_Pause(int pause);
void ADX_StartSeamless();
void ADX_StartMem(void* buf, size_t size);
int ADX_GetNumFiles();
void ADX_EntryAfs(int file_id);
void ADX_StartAfs(int file_id);
void ADX_ResetEntry();
void ADX_SetOutVol(int volume);
void ADX_SetMono(bool mono);
ADXState ADX_GetState();

#endif
