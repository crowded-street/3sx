#include "common.h"

#include <cri/private/libadxe/structs.h>

#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/cri_cvfs.h>
#include <cri/private/libadxe/htci.h>
#include <cri/private/libadxe/htci_sub.h>

#include <cri/ee/adx_ps2.h>
#include <cri/ee/cri_xpt.h>

void adxps2_err_host(void* obj, int ecode) {
    ADXERR_CallErrFunc1(ecode);
}

void ADXPS2_SetupHostFs(ADXPS2_SPRM_HOST* sprmh) {
    cvFsEntryErrFunc(adxps2_err_host, 0);

    cvFsAddDev("HST", &htCiGetInterface, 0);
    cvFsSetDefDev("HST");

    if (sprmh != NULL) {
        htCiSetOpenMode(sprmh->rdmode);
        htCiSetRootDir(sprmh->rtdir);

        if (*(int*)&sprmh->pad[0] == 0) {
            htCiSetFileSystem64(0);
        } else {
            htCiSetFileSystem64(1);
        }

        if (*(int*)&sprmh->pad[4] == 0) {
            htCiSetLockHost(0);
        } else {
            htCiSetLockHost(1);
        }
    } else {
        htCiSetOpenMode(0);
        htCiSetRootDir(NULL);
        htCiSetFileSystem64(0);
        htCiSetLockHost(0);
    }
}

int ADXPS2_LoadFcacheHost(ADXPS2_FCPRM* prm) {
    int ret;

    if (prm) {
        ret = htCiLoadFcache(prm->flist, prm->fcbuf, prm->fcsize, prm->maxflen);
    } else {
        ret = htCiLoadFcache(0, 0, 0, 0);
    }

    return ret;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_suht", ADXPS2_IsExistFcacheHost);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_suht", ADXPS2_SetRdModeHost);
