#ifndef EFL_SPU_MAP_H
#define EFL_SPU_MAP_H

#include "structs.h"
#include "types.h"

#define SPUBANK_MAX 16

typedef struct {
    u32 BankSize[SPUBANK_MAX];
} PSPUMAP_PAGE;

typedef struct {
    struct {
        union {
            s8 TagStrings[8];
            u64 Tag;         
        };
        u32 NumPages;    
        u32 ___dummy___; 
    } Head;              
    PSPUMAP_PAGE Page[]; 
} PSPUMAP;

typedef struct {
    u32 BankAddr[SPUBANK_MAX];
    u32 BankSize[SPUBANK_MAX];
    u32 EEA_Addr[2];          
} CURRMAP;

extern CURRMAP CurrMap; 
extern PSPUMAP* pSpuMap;
extern u32 SpuTopAddr;  
extern u32 CurrPage;    

s32 flSpuMapInit(PSPUMAP* pMap);
s32 flSpuMapChgPage(u32 page);
u32 flSpuMapGetBankAddr(u32 bank);

#endif
