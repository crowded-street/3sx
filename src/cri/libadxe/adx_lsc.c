#include "common.h"
#include <cri/private/libadxe/structs.h>

#include <cri/private/libadxe/adx_crs.h>
#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/adx_fs.h>
#include <cri/private/libadxe/lsc.h>

#include <cri_adxt.h>

// These should go in their own headers

Sint32 LSC_GetNumStm(void*);
void LSC_SetFlowLimit(void*, Sint32);
void LSC_Start(void*);
void LSC_SetLpFlg(void*, Sint32);
void LSC_ResetEntry(void*);

void ADXT_StopWithoutLsc();
void adxt_start_sj(ADXT, SJ_OBJ*);

void ADXT_EntryFname(ADXT adxt, Char8* fname) {
    void* lsc = adxt->lsc;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080848 ADXT_EntryFname: parameter error");
        return;
    }

    if (LSC_EntryFname(lsc) < 0) {
        ADXERR_CallErrFunc2("E0010601:Can't entry file ", fname);
    }
}

void ADXT_EntryAfs(ADXT adxt, Sint32 patid, Sint32 fid) {
    Char8 error[16];
    void* dir;
    Sint32 ofst;
    Sint32 fnsct;

    void* lsc = adxt->lsc;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080855 ADXT_EntryAfs: parameter error");
        return;
    }

    if (ADXF_GetFnameRangeEx(patid, fid, adxt->unkB0, &dir, &ofst, &fnsct) == 0) {
        LSC_EntryFileRange(lsc, ADXF_GetFnameFromPt(patid), dir, ofst, fnsct);
        return;
    }

    ADXERR_ItoA2(patid, fid, error, 16);
    ADXERR_CallErrFunc2("E0071301 ADXT_EntryAfs: can't entry ", error);
}

void ADXT_StartSeamless(ADXT adxt) {
    SJ sj;
    void* lsc = adxt->lsc;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080850 ADXT_StartSeamless: parameter error");
        return;
    }

    ADXT_StopWithoutLsc();
    ADXCRS_Lock();
    adxt_start_sj(adxt, adxt->sjf);
    sj = adxt->sji;
    SJ_Reset(sj);
    adxt->pmode = 4;
    LSC_SetFlowLimit(lsc, adxt->minsct << 11);
    ADXCRS_Unlock();
    LSC_Start(lsc);
    ADXT_SetLnkSw(adxt, 1);
}

void ADXT_SetSeamlessLp(ADXT adxt, Sint32 flg) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080851 ADXT_SetSeamlessLp: parameter error");
        return;
    }

    LSC_SetLpFlg(adxt->lsc, flg);
}

void ADXT_StartFnameLp(ADX_TALK* adxt, Char8* fname) {
    void* lsc;

    if (adxt == NULL || fname == NULL) {
        ADXERR_CallErrFunc1("E02080852 ADXT_StartFnameLp: parameter error");
        return;
    }

    lsc = adxt->lsc;
    LSC_Stop(lsc);
    ADXT_EntryFname(adxt, fname);
    LSC_SetLpFlg(lsc, 1);
    ADXT_StartSeamless(adxt);
}

void ADXT_ReleaseSeamless(ADXT adxt) {
    // Do nothing
}

Sint32 ADXT_GetNumFiles(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080854 ADXT_GetNumFiles: parameter error");
        return -1;
    }

    return LSC_GetNumStm(adxt->lsc);
}

void ADXT_ResetEntry(ADXT adxt) {
    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080849 ADXT_ResetEntry: parameter error");
        return;
    }

    if (adxt->stat != ADXT_STAT_STOP) {
        return;
    }

    LSC_ResetEntry(adxt->lsc);
}
