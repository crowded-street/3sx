#include "common.h"
#include <cri/private/libadxe/sj_err.h>
#include <cri/private/libadxe/svm.h>

void SJERR_CallErr(const Char8* msg) {
    SVM_CallErr1(msg);
}
