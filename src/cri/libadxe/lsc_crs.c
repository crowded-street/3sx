#include "common.h"
#include <cri/private/libadxe/sj_crs.h>

#include <cri/cri_xpts.h>

void LSC_LockCrs(Sint32* unused) {
    Sint32 prevent_tail_call;
    SJCRS_Lock();
    prevent_tail_call = 0;
}

void LSC_UnlockCrs(Sint32* unused) {
    SJCRS_Unlock();
}
