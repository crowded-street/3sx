#ifndef PS2PAD_H
#define PS2PAD_H

#include "sf33rd/AcrSDK/common/pad.h"
#include "types.h"

typedef struct {
    u8 state;
    u8 anstate;
    u16 kind;
    PAD_CONN conn;
    u32 sw;
    PAD_ANSHOT anshot;
    PAD_STICK stick[2];
} TARPAD;

typedef struct {
    u8 state;
    u8 phase;
    u8 port;
    u8 slot;
    u8 kind;
    u8 vib;
    u8 socket_id;
    u8 pad_id;
    u32 bprofile;
    u32 vprofile;
    u32 vib_timer;
} PS2Slot;

extern TARPAD tarpad_root[2];
extern PS2Slot ps2slot[2];

s32 tarPADInit();
void tarPADDestroy();
void flPADConfigSetACRtoXX(s32 padnum, s16 a, s16 b, s16 c);
void tarPADRead();

#endif
