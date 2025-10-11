#include "common.h"
#include <cri/private/libadxe/svm.h>

#include <string.h>

#define ADXERR_MAX_LEN 0x100

void (*adxerr_func)(void*, Sint8*) = NULL;
void* adxerr_obj = NULL;
char adxerr_msg[ADXERR_MAX_LEN] = { 0 };

Sint8 D_006BDA48[32];

void ADXERR_Init() {
    memset(&adxerr_msg, 0, sizeof(adxerr_msg));
    adxerr_func = NULL;
    adxerr_obj = NULL;
}

void ADXERR_Finish() {
    memset(&adxerr_msg, 0, sizeof(adxerr_msg));
    adxerr_func = NULL;
    adxerr_obj = NULL;
}

void ADXERR_EntryErrFunc(void (*arg0)(void*, Sint8*), void* arg1) {
    adxerr_func = arg0;
    adxerr_obj = arg1;
    SVM_SetCbErr(arg0, arg1);
}

void ADXERR_CallErrFunc1(Sint8* msg) {
    strncpy(adxerr_msg, msg, ADXERR_MAX_LEN - 1);

    if (adxerr_func != NULL) {
        adxerr_func(adxerr_obj, adxerr_msg);
    }

    SVM_CallErr(adxerr_msg);
}

void ADXERR_CallErrFunc2(Sint8* arg0, Sint8* arg1) {
    strncpy(adxerr_msg, arg0, ADXERR_MAX_LEN - 1);
    strncat(adxerr_msg, arg1, ADXERR_MAX_LEN - 1);

    if (adxerr_func != NULL) {
        adxerr_func(adxerr_obj, adxerr_msg);
    }

    SVM_CallErr(adxerr_msg);
}

void ADXERR_ItoA(Sint32 value, Sint8* str, strlen_t base) {
    Sint32 i;
    strlen_t len;

    for (i = 0; i < 32; i++) {
        str[i] = value % 10;
        value /= 10;

        if (value == 0) {
            str[i] = '\0';
            break;
        }
    }

    len = strlen(D_006BDA48);
    len = (len >= base - 1) ? base - 1 : len;

    for (i = 0; i < len; i++) {
        str[i] = D_006BDA48[len - i - 1];
    }

    str[i] = '\0';
}

void ADXERR_ItoA2(Sint32 arg0, Sint32 arg1, Sint8* str, Sint32 base) {
    ADXERR_ItoA(arg0, str, base);
    strncat(str, " ", base - strlen(str) - 1);
    ADXERR_ItoA(arg1, &str[strlen(str)], 4 - strlen(str));
}
