#include "common.h"
#include <cri/private/libadxe/structs.h>

#include <cri/private/libadxe/lsc.h>
#include <cri/private/libadxe/lsc_crs.h>
#include <cri/private/libadxe/lsc_err.h>
#include <cri/private/libadxe/lsc_ini.h>

#include <cri/cri_xpts.h>

#include <string.h>

Char8* volatile lsc_build = "\nLSC/PS2EE Ver.2.14 Build:Sep 18 2003 10:00:13\n";
Char8 lsc_obj_mark[] = "MARK:lsc_obj";
Sint32 lsc_init_cnt = 0;
LSC_OBJ lsc_obj[LSC_MAX_OBJ] = { 0 };

void lsc_EntrySvrInt() {
    // Do nothing
}

void lsc_DeleteSvrInt() {
    // Do nothing
}

void LSC_Init() {
    Sint32 lock;

    lsc_build;
    LSC_LockCrs(&lock);

    if (lsc_init_cnt == 0) {
        memset(lsc_obj, 0, sizeof(lsc_obj));
        lsc_EntrySvrInt();
        LSC_EntryErrFunc(NULL, NULL);
    }

    lsc_init_cnt += 1;
    LSC_UnlockCrs(&lock);
}

void LSC_Finish() {
    Sint32 temp_v0;
    Sint32 var_s1;
    LSC lsc;
    Sint32 i;

    if (--lsc_init_cnt == 0) {
        for (i = 0; i < LSC_MAX_OBJ; i++) {
            lsc = &lsc_obj[i];

            if (lsc->used == 1) {
                LSC_Destroy(lsc);
            }
        }

        memset(lsc_obj, 0, sizeof(lsc_obj));
        lsc_DeleteSvrInt();
        LSC_EntryErrFunc(NULL, NULL);
    }
}
