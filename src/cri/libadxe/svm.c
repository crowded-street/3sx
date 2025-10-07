#include "common.h"

#include <cri/ee/cri_mw.h>
#include <cri/private/libadxe/svm.h>

#include <stdio.h>
#include <string.h>

#if defined(TARGET_PS2)
#include "mw_stdarg.h"
#else
#include <stdarg.h>
#endif

typedef struct {
    void (*func)(void*);
    void* object;
} SVMLockCallback;

typedef struct {
    void (*func)(void*, Char8*);
    void* object;
} SVMErrorCallback;

typedef struct {
    Sint32 (*func)(void*);
    void* object;
} SVMSVRCallback;

typedef struct {
    void (*unk0)();
    Sint32 unk4;
} SVM_UNK;

// data
Char8* svm_build = "\nSVM/PS2EE Ver.1.51 Build:Sep 18 2003 09:59:52\n";
volatile Sint32 svm_init_level = 0;
volatile Sint32 svm_lock_level = 0;
volatile Sint32 svm_locking_type = 0;
Sint32 svm_exec_cnt[8] = { 0 };
Sint32 svm_svr_exec_flag[8] = { 0 };
Char8 svmerr_msg[128] = { 0 };

// bss
#if defined(TARGET_PS2)
extern Sint32 (*svm_test_and_set_callback)();
extern SVMErrorCallback svm_error_callback;
extern SVMLockCallback svm_unlock_callback;
extern SVMLockCallback svm_lock_callback;
extern SVM_UNK D_006C04E0[8];
extern SVMSVRCallback D_006C0360[8][6]; // svm_svr_callbacks
// extern Uint8 D_006C0340[0x20];
#else
Sint32 (*svm_test_and_set_callback)();
SVMErrorCallback svm_error_callback;
SVMLockCallback svm_unlock_callback;
SVMLockCallback svm_lock_callback;
SVM_UNK D_006C04E0[8];
SVMSVRCallback D_006C0360[8][6]; // svm_svr_callbacks
// Uint8 D_006C0340[0x20];
#endif

void svm_lock(Sint32 type) {
    if (svm_lock_callback.func == NULL) {
        return;
    }

    svm_lock_callback.func(svm_lock_callback.object);

    if (svm_lock_level == 0) {
        svm_locking_type = type;
    }

    svm_lock_level += 1;
}

void svm_unlock(Sint32 type) {
    if (svm_unlock_callback.func == NULL) {
        return;
    }

    svm_lock_level -= 1;

    if (svm_lock_level == 0) {
        if (svm_locking_type != type) {
            SVM_CallErr(
                "2103102:SVM:svm_unlock:lock type miss match.(type org=%d, type now=%d)", svm_locking_type, type);
        }

        svm_locking_type = 0;
    }

    svm_unlock_callback.func(svm_unlock_callback.object);
}

void SVM_Lock() {
    svm_lock(1);
}

void SVM_Unlock() {
    svm_unlock(1);
}

void SVM_LockVar() {
    svm_lock(2);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_LockSync);

void SVM_LockRsc() {
    svm_lock(4);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_LockThrd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_LockEtc);

void SVM_UnlockVar() {
    svm_unlock(2);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_UnlockSync);

void SVM_UnlockRsc() {
    svm_unlock(4);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_UnlockThrd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_UnlockEtc);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_GetLockType);

void SVM_CallErr(const Char8* format, ...) {
    va_list args;

    memset(svmerr_msg, 0, sizeof(svmerr_msg));
    va_start(args, format);
    vsprintf(&svmerr_msg, format, args);

    if (svm_error_callback.func != NULL) {
        svm_error_callback.func(svm_error_callback.object, svmerr_msg);
    }
}

void SVM_CallErr1(const Char8* msg) {
    strncpy(svmerr_msg, msg, sizeof(svmerr_msg) - 1);

    if (svm_error_callback.func != NULL) {
        svm_error_callback.func(svm_error_callback.object, svmerr_msg);
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_CallErr2);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_ItoA);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/svm", D_0055E808);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_ItoA2);

Sint32 SVM_SetCbSvr(Sint32 index, Sint32 (*func)(void*), void* object) {
    Sint32 i;
    SVMSVRCallback* callback;

    SVM_Lock();

    for (i = 0; i < 6; i++) {
        callback = &D_006C0360[index][i];

        if (callback->func == NULL) {
            callback->func = func;
            callback->object = object;
            break;
        }
    }

    if (i == 6) {
        SVM_CallErr1("1051001:SVM_SetCbSvr:too many server function");
    }

    SVM_Unlock();
    return (i != 6) ? i : -1;
}

void SVM_DelCbSvr(Sint32 group, Sint32 id) {
    if ((Uint32)id >= 6) {
        SVM_CallErr1("1051002:SVM_DelCbSvr:illegal id");
    }

    SVM_Lock();
    D_006C0360[group][id].func = NULL;
    D_006C0360[group][id].object = NULL;
    SVM_Unlock();
}

void SVM_SetCbSvrId(Sint32 svtype, Sint32 id, Sint32 (*func)(void*), void* object) {
    SVMSVRCallback* callback;

    if ((Uint32)id >= 6) {
        SVM_CallErr1("1071201:SVM_SetCbSvrId:illegal id");
    }

    if ((svtype < 0) || (id >= 8)) {
        SVM_CallErr1("1071202:SVM_SetCbSvrId:illegal svtype");
    }

    SVM_Lock();
    callback = &D_006C0360[svtype][id];

    if (callback->func != NULL) {
        SVM_CallErr1("2100801:SVM_SetCbSvrId:over write callback function.");
    }

    callback->func = func;
    callback->object = object;
    SVM_Unlock();
}

void SVM_SetCbBdr(Sint32 arg0, void (*arg1)(), Sint32 arg2) {
    SVM_Lock();
    D_006C04E0[arg0].unk0 = arg1;
    D_006C04E0[arg0].unk4 = arg2;
    SVM_Unlock();
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_GotoSvrBorder);

void SVM_SetCbErr(void (*func)(void*, Char8*), void* object) {
    SVM_Lock();
    svm_error_callback.func = func;
    svm_error_callback.object = object;
    SVM_Unlock();
}

void SVM_SetCbLock(void (*func)(void*), void* object) {
    svm_lock_callback.func = func;
    svm_lock_callback.object = object;
}

void SVM_SetCbUnlock(void (*func)(void*), void* object) {
    svm_unlock_callback.func = func;
    svm_unlock_callback.object = object;
}

Sint32 SVM_ExecSvrFunc(Sint32 svtype) {
    SVMSVRCallback* callback;
    Sint32 i;
    Sint32 func_result;
    Sint32 result = 0;

    for (i = 0; i < 6; i++) {
        callback = &D_006C0360[svtype][i];

        if (callback->func != NULL) {
            svm_svr_exec_flag[svtype] = 1;
            func_result = callback->func(callback->object);
            svm_svr_exec_flag[svtype] = 0;
            result |= func_result;
        }
    }

    svm_exec_cnt[svtype] += 1;
    return result;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/svm", D_0055E8E8);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/svm", D_0055E910);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_ExecSvrFuncId);

Sint32 SVM_ExecSvrVint() {
    return SVM_ExecSvrFunc(0);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_ExecSvrUsrVsync);

Sint32 SVM_ExecSvrVsync() {
    return SVM_ExecSvrFunc(2);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_ExecSvrUhigh);

Sint32 SVM_ExecSvrFs() {
    return SVM_ExecSvrFunc(4);
}

Sint32 SVM_ExecSvrMain() {
    return SVM_ExecSvrFunc(5);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_ExecSvrMwIdle);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_ExecSvrUsrIdle);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/svm", SVM_GetExecCount);

void svm_reset_variable() {
    Sint32 i;
    Sint32* exec_cnt_ptr;

    memset(svm_svr_exec_flag, 0, sizeof(svm_svr_exec_flag));
    memset(&svm_lock_callback, 0, sizeof(SVMLockCallback));
    memset(&svm_unlock_callback, 0, sizeof(SVMLockCallback));

    exec_cnt_ptr = svm_exec_cnt;

    for (i = 0; i < 6; i++) {
        *exec_cnt_ptr++ = 0;
    }

    svm_test_and_set_callback = NULL;
}

void SVM_Init() {
    if (svm_init_level == 0) {
        svm_reset_variable();
    }

    svm_init_level += 1;
}

void SVM_Finish() {
    svm_init_level -= 1;

    if (svm_init_level == 0) {
        svm_reset_variable();
        memset(&svm_error_callback, 0, sizeof(SVMErrorCallback));
    }
}

Sint32 SVM_TestAndSet(Sint32* mem) {
    Sint32 temp;
    Sint32 result;

    if (svm_test_and_set_callback != NULL) {
        result = svm_test_and_set_callback();
    } else {
        SVM_Lock();
        temp = *mem;
        *mem = 1;
        result = temp != 1;
        SVM_Unlock();
    }

    return result;
}

Sint32 SVM_SetCbTestAndSet(Sint32 (*callback)()) {
    if (svm_test_and_set_callback != NULL) {
        return 0;
    }

    svm_test_and_set_callback = callback;
    return 1;
}

Sint32 SVM_DelCbTestAndSet() {
    return SVM_SetCbTestAndSet(NULL);
}
