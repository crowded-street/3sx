#include "port/sound/emlShim.h"

#include "common.h"
#include "port/sound/list.h"
#include "port/sound/spu.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlSndDrv.h"
#include <stdio.h>
#include <string.h>

// Loosely based on the voice work struct from CSELIB00.IRX
// We probably don't actually need much of this

struct VId {
    u32 guid;
    u8 attr;
    u8 prio;
    u8 bank;
    u8 note;
    u8 id1;
    u8 id2;
};

enum {
    MATCH_UNK = (1 << 0),
    MATCH_BANK_SBIT = (1 << 1),
    MATCH_UNK2 = (1 << 2),
    MATCH_NOTE = (1 << 3),
    MATCH_ID1 = (1 << 4),
    MATCH_ID2 = (1 << 5),
    MATCH_GUID = (1 << 6),
    MATCH_BANK = (1 << 7),
};

struct LFO {
    u32 state;
    u16 speed;
    u16 depth;
};

struct VWork {
    int voice_num;
    struct VId id;
    u32 tick;
    u32 kofftime;
    s8 ph_pan;
    s8 req_pan;
    s16 ph_vol;
    s16 req_vol;
    s16 ph_pitch;
    s16 req_pitch;
    u32 freq;
    u16 adsr1, adsr2;
    struct LFO lfo_pitch, lfo_vol;
    struct list_head list;
};

static short masterVolume;
static short assignedBankVolume[16];
static short bankVolume[16];

static struct VWork vpool[48];

static LIST_HEAD(active_voices);
static LIST_HEAD(free_voices);

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

static const u16 NotePitchTable[] = {
    0x8000, 0x879C, 0x8FAC, 0x9837, 0xA145, 0xAADC, 0xB504, 0xBFC8, 0xCB2F, 0xD744, 0xE411, 0xF1A1, 0x8000, 0x800E,
    0x801D, 0x802C, 0x803B, 0x804A, 0x8058, 0x8067, 0x8076, 0x8085, 0x8094, 0x80A3, 0x80B1, 0x80C0, 0x80CF, 0x80DE,
    0x80ED, 0x80FC, 0x810B, 0x811A, 0x8129, 0x8138, 0x8146, 0x8155, 0x8164, 0x8173, 0x8182, 0x8191, 0x81A0, 0x81AF,
    0x81BE, 0x81CD, 0x81DC, 0x81EB, 0x81FA, 0x8209, 0x8218, 0x8227, 0x8236, 0x8245, 0x8254, 0x8263, 0x8272, 0x8282,
    0x8291, 0x82A0, 0x82AF, 0x82BE, 0x82CD, 0x82DC, 0x82EB, 0x82FA, 0x830A, 0x8319, 0x8328, 0x8337, 0x8346, 0x8355,
    0x8364, 0x8374, 0x8383, 0x8392, 0x83A1, 0x83B0, 0x83C0, 0x83CF, 0x83DE, 0x83ED, 0x83FD, 0x840C, 0x841B, 0x842A,
    0x843A, 0x8449, 0x8458, 0x8468, 0x8477, 0x8486, 0x8495, 0x84A5, 0x84B4, 0x84C3, 0x84D3, 0x84E2, 0x84F1, 0x8501,
    0x8510, 0x8520, 0x852F, 0x853E, 0x854E, 0x855D, 0x856D, 0x857C, 0x858B, 0x859B, 0x85AA, 0x85BA, 0x85C9, 0x85D9,
    0x85E8, 0x85F8, 0x8607, 0x8617, 0x8626, 0x8636, 0x8645, 0x8655, 0x8664, 0x8674, 0x8683, 0x8693, 0x86A2, 0x86B2,
    0x86C1, 0x86D1, 0x86E0, 0x86F0, 0x8700, 0x870F, 0x871F, 0x872E, 0x873E, 0x874E, 0x875D, 0x876D, 0x877D, 0x878C
};

static void UpdateVolPanPitch(struct VWork* voice);
static int gcVoices();

// Note to pitch from ps2sdk
static u16 sceSdNote2Pitch(u16 center_note, u16 center_fine, u16 note, short fine) {
    s32 _fine;
    s32 _fine2;
    s32 _note;
    s32 offset1, offset2;
    s32 val;
    s32 val2;
    s32 val3;
    s32 ret;

    _fine = fine + (u16)center_fine;
    _fine2 = _fine;

    if (_fine < 0)
        _fine2 = _fine + 127;

    _fine2 = _fine2 / 128;
    _note = note + _fine2 - center_note;
    val3 = _note / 6;

    if (_note < 0)
        val3--;

    offset2 = _fine - _fine2 * 128;

    if (_note < 0)
        val2 = -1;
    else
        val2 = 0;
    if (val3 < 0)
        val3--;

    val2 = (val3 / 2) - val2;
    val = val2 - 2;
    offset1 = _note - (val2 * 12);

    if ((offset1 < 0) || ((offset1 == 0) && (offset2 < 0))) {
        offset1 = offset1 + 12;
        val = val2 - 3;
    }

    if (offset2 < 0) {
        offset1 = (offset1 - 1) + _fine2;
        offset2 += (_fine2 + 1) * 128;
    }

    ret = (NotePitchTable[offset1] * NotePitchTable[offset2 + 12]) / 0x10000;

    if (val < 0)
        ret = (ret + (1 << (-val - 1))) >> -val;

    return (u16)ret;
}

static void InitLfo(struct LFO* lfo, u16 speed, u16 depth) {
    lfo->state = 0;
    lfo->speed = speed;
    lfo->depth = depth;
}

static void MoveLFO(struct LFO* lfo) {
    lfo->state += lfo->speed;
}

static short GetSquare(int state) {
    int v1; // $v0

    v1 = state & 0x3FF;
    if (state < 0 && (state & 0x3FF) != 0)
        v1 -= 1024;

    if (v1 < 512)
        return 0x7FFF;

    if (v1 >= 1024)
        return 0;

    return -32767;
}

static int GetLfoVal(struct LFO* lfo) {
    int ret;

    ret = lfo->depth * GetSquare(lfo->state >> 8);
    if (ret < 0) {
        ret += 0x7fff;
    }

    return ret >> 15;
}

static void workTick() {
    struct VWork* i;

    list_for_each (i, &active_voices, list) {
        i->tick++;
        MoveLFO(&i->lfo_pitch);
        MoveLFO(&i->lfo_vol);

        if (i->kofftime && i->kofftime < i->tick) {
            SPU_VoiceKeyOff(i->voice_num);
        }

        UpdateVolPanPitch(i);
    }

    gcVoices();
}

void emlShimInit() {
    memset(vpool, 0, sizeof(vpool));

    list_init(&active_voices);
    list_init(&free_voices);

    SPU_Init(workTick);

    SDL_LockMutex(soundLock);

    masterVolume = 0x3fff;
    for (int i = 0; i < 16; i++) {
        bankVolume[i] = 0x3fff;
        assignedBankVolume[i] = 0x3fff;
    }

    for (int i = 0; i < 48; i++) {
        vpool[i].voice_num = i;
        list_init(&vpool[i].list);
        list_insert(&free_voices, &vpool[i].list);
    }

    SDL_UnlockMutex(soundLock);
}

static int gcVoices() {
    struct VWork *i, *n;
    int numFreed = 0;

    SDL_LockMutex(soundLock);

    list_for_each_safe (i, n, &active_voices, list) {
        if (SPU_VoiceGetEnvLvl(i->voice_num) == 0) {
            list_remove(&i->list);
            list_insert(&free_voices, &i->list);
            numFreed++;
        }
    }

    SDL_UnlockMutex(soundLock);

    return numFreed;
}

static struct VWork* allocVoice() {
    struct VWork* voice;

    if (list_empty(&free_voices)) {
        if (!gcVoices()) {
            return NULL;
        }
        // TODO use voice priority to find a voice to reuse?
    }

    voice = list_first_entry(&free_voices, struct VWork, list);
    list_remove(&voice->list);
    list_insert(&active_voices, &voice->list);

    return voice;
}

static u32 makeConditions(CSE_REQP* rq) {
    u32 flags = rq->flags;
    u32 cond = 0;

    if (flags & 1) {
        cond |= MATCH_UNK;
    }

    if (flags & 2) {
        cond |= MATCH_BANK_SBIT;
    } else if (flags & 8) {
        cond |= MATCH_UNK2;
    }

    if (flags & 0x10) {
        cond |= MATCH_NOTE;
    }

    if (flags & 0x20) {
        cond |= MATCH_ID1;
    }

    if (flags & 0x40) {
        cond |= MATCH_ID2;
    }

    if (flags & 0x80) {
        cond |= MATCH_GUID;
    }

    if (flags & 4) {
        cond |= MATCH_BANK;
    }

    return cond;
}

static int checkConditions(struct VId* id, CSE_REQP* match, u32 cond) {
    int bit = 1;

    while (cond) {
        switch (cond & bit) {
        case MATCH_UNK:
            // nothing?
            break;
        case MATCH_BANK_SBIT:
            if ((id->bank & 0x80) == 0) {
                return 0;
            }
            break;
        case MATCH_UNK2:
            if ((id->bank & 0x80) != 0) {
                return 0;
            }
            break;
        case MATCH_NOTE:
            if (id->note != match->note) {
                return 0;
            }
            break;
        case MATCH_ID1:
            if (id->id1 != match->id1) {
                return 0;
            }
            break;
        case MATCH_ID2:
            if (id->id2 != match->id2) {
                return 0;
            }
            break;
        case MATCH_GUID:
            if (id->guid != match->guid) {
                return 0;
            }
            break;
        case MATCH_BANK:
            if ((id->bank & 0xf) != (match->bank & 0xf)) {
                return 0;
            }
            break;
        }

        cond &= ~bit;
        bit <<= 1;
    }

    return 1;
}

static void UpdateVolPanPitch(struct VWork* voice) {
    int volume, bankvol, pan, voll, volr, note, pitch;
    struct SPUVConf conf;

    bankvol = bankVolume[voice->id.bank & 0xf];
    volume = (bankvol * voice->req_vol) / 0x3fff;
    volume = (volume * voice->ph_vol) / 0x3fff;
    volume = clamp(volume + GetLfoVal(&voice->lfo_vol), 0, 0x3fff);
    pan = clamp((voice->ph_pan + voice->req_pan) - 64, 0, 127);
    note = clamp(voice->ph_pitch + voice->req_pitch + GetLfoVal(&voice->lfo_pitch), -6000, 6000);
    pitch = (voice->freq * sceSdNote2Pitch(0x3c, 0, note / 100 + 60, note % 100)) / 48000;

    voll = 0x3fff * ((volume * (127 - pan)) / 127) / 0x3fff;
    volr = 0x3fff * ((volume * pan) / 127) / 0x3fff;

    conf.pitch = pitch;
    conf.voll = voll;
    conf.volr = volr;
    conf.adsr1 = voice->adsr1;
    conf.adsr2 = voice->adsr2;

    SPU_VoiceSetConf(voice->voice_num, &conf);
}

static int doSeDrop(CSE_REQP* reqp) {
    u32 cond = makeConditions(reqp);
    struct VWork* i;
    int ret = 1;

    if (reqp->limit || (reqp->flags & 1) == 0) {
        printf("Unimplemented voice count limit mode\n");
        return ret;
    }

    list_for_each (i, &active_voices, list) {
        if (checkConditions(&i->id, reqp, cond)) {
            if (reqp->prio < i->id.prio) {
                ret = 0;
                continue;
            }

            SPU_VoiceKeyOff(i->voice_num);
        }
    }

    return ret;
}

void emlShimStartSound(CSE_SYS_PARAM_SNDSTART* param) {
    int volume, bankvol, pan, voll, volr, note, pitch;
    struct SPUVConf conf;
    struct VWork* voice;

    SDL_LockMutex(soundLock);
    if (!doSeDrop(&param->reqp)) {
        SDL_UnlockMutex(soundLock);
        return;
    }

    voice = allocVoice();
    if (!voice) {
        printf("no free voices!\n");
        SDL_UnlockMutex(soundLock);
        return;
    }

    // This stuff is reverse engineered from CSELIB00.IRX

    // From function SeKeyOn
    voice->tick = 0;
    voice->kofftime = param->reqp.kofftime;
    voice->id.guid = param->reqp.guid;
    voice->id.attr = param->reqp.attr;
    voice->id.prio = param->reqp.prio;
    voice->id.bank = param->reqp.bank | (param->reqp.flags & 2 ? 0x80 : 0);
    voice->id.note = param->reqp.note;
    voice->id.id1 = param->reqp.flags & 0x20 ? param->reqp.id1 : 0xff;
    voice->id.id2 = param->reqp.flags & 0x40 ? param->reqp.id2 : 0xff;
    voice->freq = param->phdp.freq;
    voice->ph_pan = param->phdp.pan;
    voice->req_pan = param->reqp.pan;
    voice->req_vol = param->reqp.vol ? (param->reqp.vol * 0x3fff) / 0x7f : 0;
    voice->ph_vol = param->phdp.vol ? (param->phdp.vol * 0x3fff) / 0x7f : 0;
    voice->ph_pitch = param->phdp.pitch;
    voice->req_pitch = param->reqp.pitch;

    voice->adsr1 = param->phdp.adsr1;
    voice->adsr2 = param->phdp.adsr2;

    InitLfo(&voice->lfo_pitch, 0x6000, 0);
    InitLfo(&voice->lfo_vol, 0x418, 0);

    UpdateVolPanPitch(voice);

    SPU_VoiceStart(voice->voice_num, param->phdp.s_addr >> 1);

    SDL_UnlockMutex(soundLock);
}

void emlShimSeKeyOff(CSE_REQP* pReqp) {
    u32 cond = makeConditions(pReqp);
    struct VWork* i;

    SDL_LockMutex(soundLock);

    list_for_each (i, &active_voices, list) {
        if (checkConditions(&i->id, pReqp, cond)) {
            SPU_VoiceKeyOff(i->voice_num);
        }
    }

    SDL_UnlockMutex(soundLock);
}

void emlShimSeStop(CSE_REQP* pReqp) {
    u32 cond = makeConditions(pReqp);
    struct VWork* i;

    SDL_LockMutex(soundLock);

    list_for_each (i, &active_voices, list) {
        if (checkConditions(&i->id, pReqp, cond)) {
            SPU_VoiceStop(i->voice_num);
        }
    }

    SDL_UnlockMutex(soundLock);
}

void emlShimSeStopAll() {
    struct VWork* i;

    SDL_LockMutex(soundLock);

    list_for_each (i, &active_voices, list) {
        SPU_VoiceStop(i->voice_num);
    }

    SDL_UnlockMutex(soundLock);
}

void emlShimSysSetVolume(CSE_SYS_PARAM_BANKVOL* param) {
    SDL_LockMutex(soundLock);

    if (param->bank == 0xff) {
        masterVolume = param->vol ? (param->vol * 0x3fff) / 0x7f : 0;
    } else {
        assignedBankVolume[param->bank] = param->vol ? (param->vol * 0x3fff) / 0x7f : 0;
    }

    for (int i = 0; i < 16; i++) {
        bankVolume[i] = (masterVolume * assignedBankVolume[i]) / 0x3fff;
    }

    SDL_UnlockMutex(soundLock);
}

void emlShimSeSetLfo(CSE_SYS_PARAM_LFO* param) {
    u32 cond = makeConditions(&param->reqp);
    struct VWork* i;

    SDL_LockMutex(soundLock);

    list_for_each (i, &active_voices, list) {
        if (checkConditions(&i->id, &param->reqp, cond)) {
            i->lfo_pitch.state = 0;
            i->lfo_pitch.speed = param->pmd_speed;
            i->lfo_pitch.depth = param->pmd_depth;

            i->lfo_vol.state = 0;
            i->lfo_vol.speed = param->amd_speed;
            i->lfo_vol.depth = param->amd_depth;
        }
    }

    SDL_UnlockMutex(soundLock);
}
