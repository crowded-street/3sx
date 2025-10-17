#ifndef CSE_H
#define CSE_H

#include "structs.h"
#include "types.h"

#define SPUBANKID_MAX 16

typedef struct {
    u8 InitializeFlag;           
    u8 ___dummy0___;             
    u8 ___dummy1___;             
    u8 ___dummy2___;             
    u32 Counter;                 
    u32 SpuBankId[SPUBANKID_MAX];
} CSE_SYSWORK;

typedef struct {
    s32 result;    
    u32 guid;      
    u8 vtrans_move;
    u8 sifdma_move;
    u8 dma_remain; 
    u8 mbx_remain; 
} CSE_VTRANS_RESULT;

typedef struct {
    u32 cmd;         
    u32 guid;        
    uintptr_t e_addr;
    u32 s_addr;      
    u32 size;        
} CSE_SPUID_PARAM;

s32 cseInitSndDrv();
s32 cseExecServer();
s32 cseTsbRequest(u16 bank, u16 code, s32 NumArgSets, ...);

s32 cseTsbSetBankAddr(u32 bank, SoundEvent* addr);
s32 cseMemMapSetPhdAddr(u32 bank, void* addr);

s32 cseSendBd2SpuWithId(void* ee_addr, u32 size, u32 bank, u32 id);
u32 cseGetIdStoredBd(u32 bank);

s32 cseSeStopAll();
s32 cseSysSetMasterVolume(s32 vol);
s32 cseSysSetMono(u32 mono_sw);

#endif
