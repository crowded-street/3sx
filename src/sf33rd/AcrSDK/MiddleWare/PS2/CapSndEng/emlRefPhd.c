#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/emlRefPhd.h"
#include "common.h"

#include <string.h>

s32 IsSafeHeadChunk(_ps2_head_chunk* pHEAD) {
    if (strncmp((s8*)pHEAD, "Head", 4) == 0) {
        return 1;
    }

    return 0;
}

s32 IsSafeProgChunk(_ps2_prog_chunk* pPROG) {
    if (strncmp((char*)pPROG, "Prog", 4) == 0) {
        return 1;
    }

    return 0;
}

s32 IsSafeSmplChunk(_ps2_smpl_chunk* pSMPL) {
    if (strncmp((char*)pSMPL, "Smpl", 4) == 0) {
        return 1;
    }

    return 0;
}

s32 IsSafeVagiChunk(_ps2_vagi_chunk* pVAGI) {
    if (strncmp((char*)pVAGI, "Vagi", 4) == 0) {
        return 1;
    }

    return 0;
}

s32 GetNumSplit(_ps2_head_chunk* pHEAD, u8 prog) {
    _ps2_prog_chunk* pPROG;
    _ps2_prog_param* pPPRM;
    u32 offset;

    if (IsSafeHeadChunk(pHEAD) != 1) {
        return -1;
    }

    pPROG = (_ps2_prog_chunk*)((uintptr_t)pHEAD + (u32)pHEAD->progChunkOffset);
    if (IsSafeProgChunk(pPROG) != 1) {
        return -2;
    }

    if (pPROG->maxProgNum < prog) {
        return -11;
    }

    offset = pPROG->progParamOffset[prog];
    if (offset == -1) {
        return -11;
    }

    pPPRM = (_ps2_prog_param*)((uintptr_t)pPROG + offset);
    return pPPRM->nSplit;
}

s32 GetPhdParam(CSE_PHDPADDR* pHDPA, _ps2_head_chunk* pHEAD, u8 prog, u8 note, u8 index) {
    _ps2_prog_chunk* pPROG;
    _ps2_smpl_chunk* pSMPL;
    _ps2_vagi_chunk* pVAGI;
    _ps2_split_block* pSBLK;
    _ps2_prog_param* pPPRM;
    _ps2_smpl_param* pSPRM;
    _ps2_vagi_param* pVPRM;

    pPROG = (_ps2_prog_chunk*)((uintptr_t)&pHEAD->tag + (u32)pHEAD->progChunkOffset);
    pSMPL = (_ps2_smpl_chunk*)((uintptr_t)&pHEAD->tag + (u32)pHEAD->smplChunkOffset);
    pVAGI = (_ps2_vagi_chunk*)((uintptr_t)&pHEAD->tag + (u32)pHEAD->vagiChunkOffset);

    if (IsSafeHeadChunk(pHEAD) != 1) {
        return -1;
    }

    if (IsSafeProgChunk(pPROG) != 1) {
        return -2;
    }

    if (IsSafeSmplChunk(pSMPL) != 1) {
        return -3;
    }

    if (IsSafeVagiChunk(pVAGI) != 1) {
        return -4;
    }

    pPPRM = (_ps2_prog_param*)((uintptr_t)pPROG + pPROG->progParamOffset[prog]);
    pSBLK = &pPPRM->splitBlock[index];
    pSPRM = &pSMPL->smplParam[pSBLK->sampleIndex];
    pVPRM = &pVAGI->vagiParam[pSPRM->vagiIndex];

    if (!(pSBLK->lowKey > note) && !(note > pSBLK->highKey)) {
        pHDPA->pPprm = pPPRM;
        pHDPA->pSblk = pSBLK;
        pHDPA->pSprm = pSPRM;
        pHDPA->pVprm = pVPRM;
        return 1;
    }

    return -1;
}

s32 CalcPhdParam(CSE_PHDP* pPHDP, CSE_PHDPADDR* pHDPA, u8 note, u32 SpuTopAddr) {
    s16 pan;

    pPHDP->vol = pHDPA->pPprm->vol;
    pPHDP->vol = ((pPHDP->vol * pHDPA->pSblk->vol) / 127);
    pPHDP->vol = ((pPHDP->vol * pHDPA->pSprm->vol) / 127);
    pan = pHDPA->pPprm->pan - 64;

    if (pan < -64) {
        pan = -64;
    } else if (pan > 63) {
        pan = 63;
    }

    pan += (pHDPA->pSblk->pan - 64);
    if (pan < -64) {
        pan = -64;
    } else if (pan > 63) {
        pan = 63;
    }

    pan += (pHDPA->pSprm->pan - 64);
    if (pan < -64) {
        pan = -64;
    } else if (pan > 63) {
        pan = 63;
    }

    pPHDP->pan = (pan + 64);
    pPHDP->pitch = pHDPA->pPprm->fine + pHDPA->pPprm->trans * 100;
    pPHDP->pitch += pHDPA->pSblk->fine + pHDPA->pSblk->trans * 100;
    pPHDP->pitch += pHDPA->pSprm->fine + pHDPA->pSprm->trans * 100;
    pPHDP->pitch += (note - pHDPA->pSprm->base) * 100;
    pPHDP->bendLow = pHDPA->pSblk->bendLow;
    pPHDP->bendHigh = pHDPA->pSblk->bendHigh;
    pPHDP->adsr1 = pHDPA->pSprm->ADSR1;
    pPHDP->adsr2 = pHDPA->pSprm->ADSR2;
    pPHDP->s_addr = SpuTopAddr + pHDPA->pVprm->vagOffset;
    pPHDP->freq = pHDPA->pVprm->sampleRate;
    return 0;
}
