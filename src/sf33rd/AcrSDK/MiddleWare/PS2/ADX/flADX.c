#include "sf33rd/AcrSDK/MiddleWare/PS2/ADX/flADX.h"
#include "common.h"
#include <cri_mw.h>
#include <sifdev.h>
#include <stdio.h>
#include <string.h>

#define ADX_MAX_FILENAME_LEN 32

ADXPS2_SPRM_HOST ht_sprm;
s8 ht_fc_buff[533];
ADXPS2_SPRM_DVD dv_sprm;
ADXPS2_FCPRM ht_fcprm;
s8 dv_fc_buff[1640];
ADXPS2_FCPRM dv_fcprm;

void flAdxModuleInit() {
    ADXPS2_SetupVoice(2, 2);

    while (1) {
        if (sceSifLoadModule("cdrom0:\\THIRD\\IOP\\MOD_MW\\CRI_ADXI.IRX;1", 40, "thpri=40\0spucore=0\0sdinit=0") >= 0) {
            break;
        }
    }
}

void flAdxInitialize(s8* host_root, s8* cvd_root) {
    if (host_root != NULL) {
        printf("Setup HOST file system.\n");
        memset(&ht_sprm, 0, sizeof(ADXPS2_SPRM_HOST));
        ht_sprm.rtdir = host_root;
        ADXPS2_SetupHostFs(&ht_sprm);
        memset(&ht_fcprm, 0, sizeof(ADXPS2_FCPRM));
        ht_fcprm.flist = "0flist.dir";
        ht_fcprm.maxflen = ADX_MAX_FILENAME_LEN;
        ht_fcprm.fcsize = sizeof(ht_fc_buff);
        ht_fcprm.fcbuf = ht_fc_buff;
        ADXPS2_LoadFcacheHost(&ht_fcprm);
    }

    if (cvd_root != NULL) {
        printf("Setup CD/DVD file system.\n");
        memset(&dv_sprm, 0, sizeof(ADXPS2_SPRM_DVD));
        dv_sprm.rtdir = cvd_root;
        ADXPS2_SetupDvdFs(&dv_sprm);
        printf("Setup CD/DVD cache system.\n");
        memset(&dv_fcprm, 0, sizeof(ADXPS2_FCPRM));
        dv_fcprm.flist = "0flist.dir";
        dv_fcprm.maxflen = ADX_MAX_FILENAME_LEN;
        dv_fcprm.fcsize = sizeof(dv_fc_buff);
        dv_fcprm.fcbuf = dv_fc_buff;
        ADXPS2_LoadFcacheDvd(&dv_fcprm);
    }

    ADXPS2_SetupThrd(NULL);
}

void flAdxControll(s32 mode) {
    ADXPS2_ExecVint(mode);
}
