#ifndef EML_SND_DRV_H
#define EML_SND_DRV_H

#include "structs.h"
#include "types.h"

typedef struct {
    // total size: 0xC
    u32 cmd;  // offset 0x0, size 0x4
    u32 guid; // offset 0x4, size 0x4
    u8 mono;  // offset 0x8, size 0x1
} CSE_SYS_PARAM_MONO;

typedef struct {
    // total size: 0xC
    u32 cmd;  // offset 0x0, size 0x4
    u32 guid; // offset 0x4, size 0x4
    u8 bank;  // offset 0x8, size 0x1
    u8 vol;   // offset 0x9, size 0x1
} CSE_SYS_PARAM_BANKVOL;

typedef struct {
    // total size: 0x2C
    u32 cmd;       // offset 0x0, size 0x4
    u32 guid;      // offset 0x4, size 0x4
    CSE_REQP reqp; // offset 0x8, size 0x1C
    u16 pmd_speed; // offset 0x24, size 0x2
    u16 pmd_depth; // offset 0x26, size 0x2
    u16 amd_speed; // offset 0x28, size 0x2
    u16 amd_depth; // offset 0x2A, size 0x2
} CSE_SYS_PARAM_LFO;

typedef struct {
    // total size: 0x38
    u32 cmd;       // offset 0x0, size 0x4
    u32 guid;      // offset 0x4, size 0x4
    CSE_REQP reqp; // offset 0x8, size 0x1C
    CSE_PHDP phdp; // offset 0x24, size 0x14
} CSE_SYS_PARAM_SNDSTART;

typedef struct {
    // total size: 0x24
    u32 cmd;       // offset 0x0, size 0x4
    u32 guid;      // offset 0x4, size 0x4
    CSE_REQP reqp; // offset 0x8, size 0x1C
} CSE_SYS_PARAM_SECHANGE;

s32 mlSysSetMono(u32 mono_sw);
s32 mlSysSetMasterVolume(s32 vol);
s32 mlSysSetBankVolume(s32 bank, s32 vol);
s32 mlSeSetLfo(CSE_REQP* pReqp, u16 pmd_speed, u16 pmd_depth, u16 amd_speed, u16 amd_depth);
s32 mlSeStop(CSE_REQP* pReqp);
s32 mlSeKeyoff(CSE_REQP* pReqp);
s32 mlSeStopAll();
s32 mlSeInitSndDrv();
s32 PlaySe(CSE_REQP* pReqp, u16 bank, u16 prog);

#endif
