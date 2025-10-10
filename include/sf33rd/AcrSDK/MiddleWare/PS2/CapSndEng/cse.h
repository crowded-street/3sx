#ifndef CSE_H
#define CSE_H

#include "structs.h"
#include "types.h"

#define SPUBANKID_MAX 16

typedef struct {
    // total size: 0x48
    u8 InitializeFlag;            // offset 0x0, size 0x1
    u8 ___dummy0___;              // offset 0x1, size 0x1
    u8 ___dummy1___;              // offset 0x2, size 0x1
    u8 ___dummy2___;              // offset 0x3, size 0x1
    u32 Counter;                  // offset 0x4, size 0x4
    u32 SpuBankId[SPUBANKID_MAX]; // offset 0x8, size 0x40
} CSE_SYSWORK;

typedef struct {
    // total size: 0xC
    s32 result;     // offset 0x0, size 0x4
    u32 guid;       // offset 0x4, size 0x4
    u8 vtrans_move; // offset 0x8, size 0x1
    u8 sifdma_move; // offset 0x9, size 0x1
    u8 dma_remain;  // offset 0xA, size 0x1
    u8 mbx_remain;  // offset 0xB, size 0x1
} CSE_VTRANS_RESULT;

typedef struct {
    // total size: 0x14
    u32 cmd;          // offset 0x0, size 0x4
    u32 guid;         // offset 0x4, size 0x4
    uintptr_t e_addr; // offset 0x8, size 0x4
    u32 s_addr;       // offset 0xC, size 0x4
    u32 size;         // offset 0x10, size 0x4
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
