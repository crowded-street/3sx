#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlMemMap.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/eflSpuMap.h"

// sbss

void* PhdAddr[PHDADDR_MAX];

s32 mlMemMapInit(void* pSpuMemMap) {
    s32 result;
    s32 i;

    result = flSpuMapInit((PSPUMAP*)pSpuMemMap);
    if (result < 0) {
        return result;
    }

    for (i = 0; i < PHDADDR_MAX; i++) {
        PhdAddr[i] = NULL;
    }

    return 0;
}

u32 mlMemMapGetBankAddr(u32 bank) {
    return flSpuMapGetBankAddr(bank);
}

s32 mlMemMapSetPhdAddr(u32 bank, void* addr) {
    if (bank >= PHDADDR_MAX) {
        return -1;
    }

    PhdAddr[bank] = addr;
    return 0;
}

void* mlMemMapGetPhdAddr(u32 bank) {
    if (bank >= PHDADDR_MAX) {
        return NULL;
    }

    return PhdAddr[bank];
}
