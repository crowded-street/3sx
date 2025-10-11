#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/eflSpuMap.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

// bss

CURRMAP CurrMap; // size: 0x88, address: 0x6EA200

// sbss

PSPUMAP* pSpuMap; // size: 0x4, address: 0x57B250
u32 CurrPage;     // size: 0x4, address: 0x57B24C
u32 SpuTopAddr;   // size: 0x4, address: 0x57B248

s32 flSpuMapInit(PSPUMAP* pMap) {
    u32 i;

    if (strncmp((char*)pMap, "SPUMAPDT", 8) != 0) {
        return -1;
    }

    pSpuMap = pMap;
    CurrPage = 0;
    SpuTopAddr = 0x5020;

    for (i = 0; i < SPUBANK_MAX; i++) {
        CurrMap.BankAddr[i] = 0x5020;
        CurrMap.BankSize[i] = 0;
    }

    flSpuMapChgPage(CurrPage);

    return 0;
}

s32 flSpuMapChgPage(u32 page) {
    s32 i;
    u32 addr;
    PSPUMAP_PAGE* pPage;

    if ((pSpuMap->Head.NumPages - 1) > page) {
        return -1;
    }

    CurrPage = page;
    pPage = &pSpuMap->Page[CurrPage];
    addr = SpuTopAddr;

    for (i = 0; i < SPUBANK_MAX; i++) {
        CurrMap.BankAddr[i] = addr;
        CurrMap.BankSize[i] = pPage->BankSize[i];

        if (CurrMap.BankSize[i] + CurrMap.BankAddr[i] > 0x1FFFFF) {
            printf("[EE]");
            printf("(ERR)");
            // "Address has changed\n"
            printf("アドレスが変\n");
            return -1;
        }

        addr += pPage->BankSize[i];
    }

    return 0;
}

u32 flSpuMapGetBankAddr(u32 bank) {
    if (bank >= SPUBANK_MAX) {
        return 0;
    }

    return CurrMap.BankAddr[bank];
}
