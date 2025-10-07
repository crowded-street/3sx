#include "common.h"
#include <cri/private/libadxe/adx_crs.h>
#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/adx_tlk.h>

#include <cri/cri_adxf.h>
#include <cri/cri_adxt.h>
#include <cri/sj.h>

#include <string.h>

void ADXT_StartAfs(ADXT adxt, Sint32 patid, Sint32 fid) {
    Char8 error[16];
    void* dir;
    Sint32 ofst;
    Sint32 fnsct;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080811 ADXT_StartAfs: parameter error");
        return;
    }

    ADXT_Stop(adxt);

    if (ADXF_GetFnameRangeEx(patid, fid, adxt->unkB0, &dir, &ofst, &fnsct) != 0) {
        return;
    }

    if (adxt->stm == NULL) {
        ADXERR_ItoA2(patid, fid, &error, 16);
        ADXERR_CallErrFunc2("E8101202 ADXT_StartAfs: can't open ", error);
        adxt->ercode = ADXT_ERR_SHRTBUF;
        adxt->stat = ADXT_STAT_ERROR;
        return;
    }

    adxt->stat = ADXT_STAT_DECINFO;
    adxt->padB4 = adxt->unkB0;
    adxt->padB8 = dir;
    adxt->padBC = ofst;
    adxt->padC0 = fnsct;
    adxt->unkAC = 1;
    adxt->pmode = ADXT_PMODE_AFS;
    ADXT_SetLnkSw(adxt, 0);
}

void ADXT_StartFname(ADXT adxt, Char8* fname) {
    if ((adxt == NULL) || (fname == NULL)) {
        ADXERR_CallErrFunc1("E02080807 ADXT_StartFname: parameter error");
        return;
    }

    ADXT_Stop(adxt);
    strcpy(adxt->unkB0, fname);
    adxt->padB4 = adxt->unkB0;
    adxt->padC0 = 0xFFFFF;
    adxt->padB8 = 0;
    adxt->padBC = 0;
    adxt->stat = ADXT_STAT_DECINFO;
    adxt->unkAC = 1;
    adxt->pmode = ADXT_PMODE_FNAME;
    ADXT_SetLnkSw(adxt, 0);
}

void ADXT_StartMem(ADXT adxt, void* adxdat) {
    ADXT_StartMem2(adxt, adxdat, 0x40000000);
}

void ADXT_StartMem2(ADXT adxt, void* adxdat, Sint32 datlen) {
    SJ sj;

    if ((adxt == NULL) || (adxdat == NULL) || (datlen < 0)) {
        ADXERR_CallErrFunc1("E02080809 ADXT_StartMem2: parameter error");
        return;
    }

    ADXT_Stop(adxt);
    ADXCRS_Lock();

    sj = SJMEM_Create((Sint8*)adxdat, datlen);

    if (sj == NULL) {
        ADXCRS_Unlock();
        ADXERR_CallErrFunc1("E8101207: can't create sj (ADXT_StartMem)\n\0\0");
        return;
    }

    adxt_start_sj(adxt, sj);
    adxt->pmode = ADXT_PMODE_MEM;
    ADXT_SetLnkSw(adxt, 0);
    ADXCRS_Unlock();
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk2", D_0055BC70);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk2", D_0055BCA0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_tlk2", ADXT_StartMemIdx);
