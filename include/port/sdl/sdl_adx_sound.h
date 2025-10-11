#ifndef SDL_ADX_SOUND_H
#define SDL_ADX_SOUND_H

#include <stdbool.h>
#include <stddef.h>

typedef enum ADXState {
    ADX_STATE_STOP,
    ADX_STATE_PLAYING,
    ADX_STATE_PLAYEND,
} ADXState;

void SDLADXSound_ProcessTracks();

void SDLADXSound_Init();
void SDLADXSound_Exit();
void SDLADXSound_Stop();
int SDLADXSound_IsPaused();
void SDLADXSound_Pause(int pause);
void SDLADXSound_StartSeamless();
void SDLADXSound_StartMem(void* buf, size_t size);
int SDLADXSound_GetNumFiles();
void SDLADXSound_EntryAfs(int file_id);
void SDLADXSound_StartAfs(int file_id);
void SDLADXSound_ResetEntry();
void SDLADXSound_SetOutVol(int volume);
void ADX_SetMono(bool mono);
ADXState SDLADXSound_GetState();

#endif
