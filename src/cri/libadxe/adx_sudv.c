#include "common.h"
#include <cri/private/libadxe/adx_errs.h>
#include <cri/private/libadxe/cri_cvfs.h>
#include <cri/private/libadxe/dvci.h>
#include <cri/private/libadxe/dvci_sub.h>

#include <cri/ee/adx_ps2.h>

// data
Char8 volatile adxps2_rt_ver0[12] = "\nADXRT  Ver.";
Char8 volatile adxps2_rt_ver1 = '2';
Char8 volatile adxps2_rt_ver2 = '8';
Char8 volatile adxps2_rt_ver3 = '1';
Char8 volatile adxps2_rt_ver4 = '0';
Char8 volatile adxps2_rt_ver5[29] = " Build:Sep 18 2003 10:00:02\n";

void adxps2_err_dvd(void* obj, const Char8* fmt) {
    ADXERR_CallErrFunc1(fmt);
}

void ADXPS2_SetupDvdFs(ADXPS2_SPRM_DVD* sprmd) {
    cvFsEntryErrFunc(adxps2_err_dvd, NULL);
    cvFsAddDev("CDV", dvCiGetInterface, 0);
    cvFsSetDefDev("CDV");

    if (sprmd != NULL) {
        dvCiSetRdMode(sprmd->nrtry, sprmd->speed, sprmd->dtype, sprmd->rdmode);
        dvCiSetRootDir(sprmd->rtdir);
    } else {
        dvCiSetRdMode(0, 0, 0, 0);
        dvCiSetRootDir(NULL);
    }
}

Sint32 ADXPS2_LoadFcacheDvd(ADXPS2_FCPRM* fcprm) {
    Sint32 ret = 0;

    if (fcprm != NULL) {
        ret = dvCiLoadFcache(fcprm->flist, fcprm->fcbuf, fcprm->fcsize, fcprm->maxflen);
    } else {
        dvCiLoadFcache(NULL, NULL, 0, 0);
    }

    return ret;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sudv", ADXPS2_SetFcacheDvd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sudv", ADXPS2_SetRdModeDvd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sudv", ADXPS2_IsExistFcacheDvd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sudv", ADXPS2_SetDvdLayer);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sudv", ADXPS2_GetDvdLayer);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_sudv", ADXPS2_SetDvdSeekPos);
