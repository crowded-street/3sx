#ifndef MLPAD_H
#define MLPAD_H

#include "sf33rd/AcrSDK/common/pad.h"
#include "types.h"

typedef struct {
    u8 state;
    u8 anstate;
    u16 kind;
    PAD_CONN conn;

    /// Currently pressed buttons
    u32 sw;

    /// Buttons for the previous frame
    u32 sw_old;

    /// Buttons newly pressed this frame
    u32 sw_new;

    /// Buttons released this frame
    u32 sw_off;

    /// Buttons that changed
    u32 sw_chg;

    PAD_ANSHOT anshot;
    PAD_STICK stick[2];
    u32 sw_repeat;
    union {
        u16 work;
        struct {
            u8 press;
            u8 sw_up;
        } ctr;
    } rpsw[32];
} FLPAD;

typedef struct {
    u8 conf_sw[32];
    u8 flip_lever;
    u8 flip_ast1;
    u8 flip_ast2;
    u8 free;
    s16 abut_on;
    s16 ast1_on;
    s16 ast2_on;
    u16 free2;
} FLPAD_CONFIG;

extern FLPAD* flpad_adr[2];

s32 flPADInitialize();
void flPADDestroy();
void flPADWorkClear();
void flPADConfigSet(const FLPAD_CONFIG* adrs, s32 padnum);
void flPADGetALL();
void flPADACRConf();
void padconf_setup_depth(u8* deps, u8 num, u32 iodat);
void flupdate_pad_stick_dir(PAD_STICK* st);
void flupdate_pad_button_data(FLPAD* pad, u32 data);
void flupdate_pad_on_cnt(FLPAD* pad);
void flPADSetRepeatSw(FLPAD* pad, u32 IOdata, u8 ctr, u8 times);

#endif
