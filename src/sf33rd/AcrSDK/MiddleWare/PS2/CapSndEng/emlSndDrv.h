#ifndef EML_SND_DRV_H
#define EML_SND_DRV_H

#include "structs.h"
#include "types.h"

typedef struct {
    u32 cmd; 
    u32 guid;
    u8 mono; 
} CSE_SYS_PARAM_MONO;

typedef struct {
    u32 cmd; 
    u32 guid;
    u8 bank; 
    u8 vol;  
} CSE_SYS_PARAM_BANKVOL;

typedef struct {
    u32 cmd;      
    u32 guid;     
    CSE_REQP reqp;
    u16 pmd_speed;
    u16 pmd_depth;
    u16 amd_speed;
    u16 amd_depth;
} CSE_SYS_PARAM_LFO;

typedef struct {
    u32 cmd;      
    u32 guid;     
    CSE_REQP reqp;
    CSE_PHDP phdp;
} CSE_SYS_PARAM_SNDSTART;

typedef struct {
    u32 cmd;      
    u32 guid;     
    CSE_REQP reqp;
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
