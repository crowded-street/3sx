#ifndef MLPAD_H
#define MLPAD_H

#include "structs.h"
#include "types.h"

extern FLPAD* flpad_adr[2];

s32 flPADInitialize();                                           // Range: 0x1173D0 -> 0x117458
void flPADDestroy();                                             // Range: 0x117460 -> 0x117480
void flPADWorkClear();                                           // Range: 0x117480 -> 0x1174C0
void flPADConfigSet(const FLPAD_CONFIG* adrs, s32 padnum);       // Range: 0x1174C0 -> 0x1175A4
void flPADGetALL();                                              // Range: 0x1175B0 -> 0x117988
void flPADACRConf();                                             // Range: 0x117990 -> 0x11880C
void padconf_setup_depth(u8* deps, u8 num, u32 iodat);           // Range: 0x118810 -> 0x1188D8
void flupdate_pad_stick_dir(PAD_STICK* st);                      // Range: 0x1188E0 -> 0x1189D8
void flupdate_pad_button_data(FLPAD* pad, u32 data);             // Range: 0x1189E0 -> 0x118A80
void flupdate_pad_on_cnt(FLPAD* pad);                            // Range: 0x118A80 -> 0x118B64
void flPADSetRepeatSw(FLPAD* pad, u32 IOdata, u8 ctr, u8 times); // Range: 0x118B70 -> 0x118CF0

#endif
