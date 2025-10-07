#include "common.h"
#include <cri/private/libadxe/adx_crs.h>
#include <cri/private/libadxe/svm.h>

s32 adxcrs_lvl = 0;
s32 adxcrs_msk = 0;

void ADXCRS_Init() {
    adxcrs_lvl = 0;
}

void ADXCRS_Lock() {
    SVM_Lock();
}

void ADXCRS_Unlock() {
    SVM_Unlock();
}
