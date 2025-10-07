#include "common.h"
#include <cri/private/libadxe/sj_crs.h>
#include <cri/private/libadxe/svm.h>

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_crs", SJCRS_Init);

void SJCRS_Lock() {
    SVM_Lock();
}

void SJCRS_Unlock() {
    SVM_Unlock();
}
