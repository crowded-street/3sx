#ifdef CRS_AUDIO_DRIVER_PSP
#include "port/sound/spu.h"

#include <SDL3/SDL.h>
#include <pspaudio.h>
#include <pspsascore.h>
#include <psputility.h>

//#define GRAIN_SIZE (PSP_SAS_GRAIN_SIZE)
#define GRAIN_SIZE 192 

SDL_Mutex* soundLock;

__attribute__((aligned(64))) SceSasCore core;
__attribute__((aligned(64))) int16_t mixer[GRAIN_SIZE * 4];

static int channel = -1;
static u16 ram[(2 * 1024 * 1024) >> 1];
static struct SPUVConf voice[SPU_VOICE_COUNT] = {};
static void (*user_cb)();

static void sas_thread() {
    SDL_SetCurrentThreadPriority(SDL_THREAD_PRIORITY_TIME_CRITICAL);
    while (1) {
		user_cb();

        /* Run a SAS cycle, which will update and process the voices. */
        __sceSasCore(&core, mixer);

        /* Finally, output the audio. */
        sceAudioOutputPannedBlocking(channel, PSP_AUDIO_VOLUME_MAX, PSP_AUDIO_VOLUME_MAX, mixer);
    }
}

void SPU_Init(void (*cb)()) {
    soundLock = SDL_CreateMutex();
	user_cb = cb;

    /* The following modules must be loaded in order for sceSasCore to work */
    int result = sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);
    if (result < 0) {
        SDL_Log("err: 0x%08X\n", result);
    }

    result = sceUtilityLoadModule(PSP_MODULE_AV_SASCORE);
    if (result < 0) {
        SDL_Log("err: 0x%08X\n", result);
    }

    /* Initialize sceSasCore instance */
    result = __sceSasInit(&core, GRAIN_SIZE, SPU_VOICE_COUNT, PSP_SAS_OUTPUTMODE_STEREO, PSP_SAS_SAMPLE_RATE);
    if (result < 0) {
        SDL_Log("err: 0x%08X\n", result);
    }

    channel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, GRAIN_SIZE, PSP_AUDIO_FORMAT_STEREO);
    if (channel < 0) {
        SDL_Log("Failed to initialize sceAudio channel!. err: 0x%08X\n", channel);
    }

    SDL_CreateThread(sas_thread, "sas_thread", NULL);
}

void SPU_Upload(u32 dst, void* src, u32 size) {
    memcpy(&ram[dst >> 1], src, size);
}

void SPU_VoiceStart(int vnum, u32 start_addr) {
    struct SPUVConf* v = &voice[vnum];

    // HACK: we don't have the vag size on hand, doesn't seem to be used far ADPCM (on ppssspp at least)
    __sceSasSetVoice(&core, vnum, &ram[start_addr], sizeof(ram), 1);
    __sceSasSetSimpleADSR(&core, vnum, v->adsr1, v->adsr2);
    __sceSasSetPitch(&core, vnum, 48000 * v->pitch / 44100);
    __sceSasSetVolume(&core, vnum, (0x1000 * v->voll) / 0x3fff, (0x1000 * v->volr) / 0x3fff, 0, 0);

    __sceSasSetKeyOn(&core, vnum);
}

void SPU_VoiceSetConf(int vnum, struct SPUVConf* conf) {
    voice[vnum] = *conf;
}

bool SPU_VoiceIsFinished(int vnum) {
    return (__sceSasGetEndFlag(&core) & (1 << vnum)) != 0;
}
void SPU_VoiceKeyOff(int vnum) {
    __sceSasSetKeyOff(&core, vnum);
}

void SPU_VoiceStop(int vnum) {
    __sceSasSetKeyOff(&core, vnum);
    __sceSasSetVolume(&core, vnum, 0, 0, 0, 0);
}

#endif
