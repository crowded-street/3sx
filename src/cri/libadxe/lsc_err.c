#include "common.h"
#include <cri/private/libadxe/lsc_err.h>

#include <cri/cri_xpts.h>

#include <stdio.h>

#if defined(TARGET_PS2)
#include "mw_stdarg.h"
#else
#include <stdarg.h>
#endif

// data
void (*lsc_err_func)(void*, Char8*) = NULL;
void* lsc_err_obj = NULL;
Char8 lsc_err_msg[0x100] = { 0 };

void LSC_EntryErrFunc(void (*func)(void*, Char8*), void* object) {
    if (func == NULL) {
        lsc_err_func = NULL;
        lsc_err_obj = NULL;
        return;
    }

    lsc_err_func = func;
    lsc_err_obj = object;
}

void LSC_CallErrFunc(const Char8* format, ...) {
    va_list args;

    va_start(args, format);
    vsprintf(lsc_err_msg, format, args);

    if (lsc_err_func != NULL) {
        lsc_err_func(lsc_err_obj, lsc_err_msg);
    }
}
