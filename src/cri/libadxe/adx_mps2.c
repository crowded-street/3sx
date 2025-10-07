#include "common.h"
#include <cri/private/libadxe/svm.h>

#include <cri/cri_adxt.h>
#include <cri/ee/adx_ps2.h>
#include <cri/ee/cri_xpt.h>

#include <eekernel.h>
#include <eeregs.h>

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", D_0055AA98);

// data
#if defined(TARGET_PS2)
extern Char8* volatile adxps2_build;
extern Sint32 adxm_init_level;
extern Sint32 adxm_init_ex;
extern Sint32 adxm_save_tprm[12];
extern Sint32 adxm_lock_level;
extern Sint32 volatile adxm_goto_border_flag;
extern Sint32 volatile adxm_vsyncproc_exec_flag;
extern Sint32 volatile adxm_usrvsyncproc_exec_flag;
extern Sint32 volatile adxm_fsproc_exec_flag;
extern Sint32 volatile adxm_mwidleproc_exec_flag;
extern Sint32 adxm_usridleproc_exec_flag;
extern Sint32 volatile adxm_waiting_main_th_flag;
extern Sint64 adxm_safe_cnt;
extern Sint64 adxm_usrvsync_cnt;
extern Sint64 volatile adxm_vsync_cnt;
extern Sint64 volatile adxm_fs_cnt;
extern Sint64 volatile adxm_mwidle_cnt;
extern Sint64 adxm_usridle_cnt;
extern Sint64 adxm_vint_cnt;
extern Sint32 volatile adxm_id_safe;
extern Sint32 volatile adxm_id_usrvsync;
extern Sint32 volatile adxm_id_vsync;
extern Sint32 volatile adxm_id_fs;
extern Sint32 volatile adxm_id_main;
extern Sint32 volatile adxm_id_mwidle;
extern Sint32 volatile adxm_id_usridle;
extern Sint64 volatile adxm_safe_loop;
extern Sint64 volatile adxm_safe_exit;
extern Sint64 volatile adxm_usrvsync_loop;
extern Sint64 volatile adxm_usrvsync_exit;
extern Sint64 volatile adxm_vsync_loop;
extern Sint64 volatile adxm_vsync_exit;
extern Sint64 volatile adxm_fs_loop;
extern Sint64 volatile adxm_fs_exit;
extern Sint64 volatile adxm_mwidle_loop;
extern Sint64 volatile adxm_mwidle_exit;
extern Sint64 volatile adxm_usridle_loop;
extern Sint64 volatile adxm_usridle_exit;
extern Sint32 adxps2_exec_svr;
extern Sint32 volatile adxps2_exec_vint;
extern Sint32 (*adxm_old_cbf)(Sint32 arg);
extern Uint16 adxm_debug_bgcl;
extern Sint32 adxm_stack_safe[0x200];
extern Sint32 adxm_stack_usrvsync[0x400];
extern Sint32 adxm_stack_vsync[0x400];
extern Sint32 adxm_stack_fs[0x400];
extern Sint32 adxm_stack_mwidle[0x800];
extern Sint32 adxm_stack_usridle[0x800];
extern Sint32 adxm_cur_prio;
extern Sint32 adxm_cur_tid;
extern Sint32 adxm_main_prio_def;
#else
Char8* volatile adxps2_build = "\nADXPS2 Ver.2.46 Build:Sep 18 2003 10:00:02\n";
Sint32 adxm_init_level = 0;
Sint32 adxm_init_ex = 0;
Sint32 adxm_save_tprm[12] = { 0 };
Sint32 adxm_lock_level = 0;
Sint32 volatile adxm_goto_border_flag = 0;
Sint32 volatile adxm_vsyncproc_exec_flag = 0;
Sint32 volatile adxm_usrvsyncproc_exec_flag = 0;
Sint32 volatile adxm_fsproc_exec_flag = 0;
Sint32 volatile adxm_mwidleproc_exec_flag = 0;
Sint32 adxm_usridleproc_exec_flag = 0;
Sint32 volatile adxm_waiting_main_th_flag = 0;
Sint64 adxm_safe_cnt = 0;
Sint64 adxm_usrvsync_cnt = 0;
Sint64 volatile adxm_vsync_cnt = 0;
Sint64 volatile adxm_fs_cnt = 0;
Sint64 volatile adxm_mwidle_cnt = 0;
Sint64 adxm_usridle_cnt = 0;
Sint64 adxm_vint_cnt = 0;
Sint32 volatile adxm_id_safe = 0;
Sint32 volatile adxm_id_usrvsync = 0;
Sint32 volatile adxm_id_vsync = 0;
Sint32 volatile adxm_id_fs = 0;
Sint32 volatile adxm_id_main = 0;
Sint32 volatile adxm_id_mwidle = 0;
Sint32 volatile adxm_id_usridle = 0;
Sint64 volatile adxm_safe_loop = 0;
Sint64 volatile adxm_safe_exit = 0;
Sint64 volatile adxm_usrvsync_loop = 0;
Sint64 volatile adxm_usrvsync_exit = 0;
Sint64 volatile adxm_vsync_loop = 0;
Sint64 volatile adxm_vsync_exit = 0;
Sint64 volatile adxm_fs_loop = 0;
Sint64 volatile adxm_fs_exit = 0;
Sint64 volatile adxm_mwidle_loop = 0;
Sint64 volatile adxm_mwidle_exit = 0;
Sint64 volatile adxm_usridle_loop = 0;
Sint64 volatile adxm_usridle_exit = 0;
Sint32 adxps2_exec_svr = 0;
Sint32 volatile adxps2_exec_vint = 0;
Sint32 (*adxm_old_cbf)(Sint32 arg) = NULL;
Uint16 adxm_debug_bgcl = 0;
Sint32 adxm_stack_safe[0x200] = { 0 };
Sint32 adxm_stack_usrvsync[0x400] = { 0 };
Sint32 adxm_stack_vsync[0x400] = { 0 };
Sint32 adxm_stack_fs[0x400] = { 0 };
Sint32 adxm_stack_mwidle[0x800] = { 0 };
Sint32 adxm_stack_usridle[0x800] = { 0 };
Sint32 adxm_cur_prio = 0;
Sint32 adxm_cur_tid = 0;
Sint32 adxm_main_prio_def = 0;
#endif

#if defined(TARGET_PS2)
extern void* _gp;
#else
void* _gp;
#endif

// We have to declare adxt_vsync_cnt here instead of including adx_inis.h
// because other parts of the code expect it to be non-volatile. Go figure
extern Sint32 volatile adxt_vsync_cnt;

// forward decls
void adxm_ShutdownFsThrd();
void adxm_ShutdownMwIdleThrd();
void adxm_ShutdownPreProcedure();
void adxm_ShutdownSafeThrd();
void adxm_ShutdownVsyncThrd();
Sint32 adxm_ResumeThread(Sint32 tid);
Sint32 adxm_SuspendThread(Sint32 tid);
Sint32 adxm_IsSetupThrdEx();
void adxm_SleepThread();
void adxm_WakeupThread(Sint32 tid);

void _adxm_lock() {
    if (adxm_lock_level == 0) {
        Sint32 tid = GetThreadId();
        adxm_cur_prio = ChangeThreadPriority(tid, adxm_save_tprm[0]);
        adxm_cur_tid = tid;
        adxm_ResumeThread(adxm_id_safe);

        // Dead code for matching
        adxm_id_safe;
        adxm_lock_level += 1;
        adxm_lock_level -= 1;
    }

    adxm_lock_level += 1;
}

void _adxm_unlock() {
    if (--adxm_lock_level == 0) {
        adxm_SuspendThread(adxm_id_safe);
        adxm_id_safe;
        ChangeThreadPriority(GetThreadId(), adxm_cur_prio);
    }

    if (adxm_lock_level < 0) {
        adxm_lock_level = 0;
    }
}

void adxm_goto_svr_border(Sint32 tid, Sint32 priority) {
    Sint32 var_s0;

    if (tid != TH_SELF) {
        adxm_goto_border_flag = 1;

        ChangeThreadPriority(tid, adxm_save_tprm[0]);

        for (var_s0 = 0; var_s0 < 200000000; var_s0++) {
            adxm_WakeupThread(tid);
            adxm_ResumeThread(tid);

            if (adxm_goto_border_flag == 0) {
                break;
            }
        }

        if (var_s0 == 200000000) {
            SVM_CallErr1("1060102: Internal Error: adxm_goto_svr_border");
        }

        ChangeThreadPriority(tid, priority);
        adxm_SuspendThread(tid);

        // block the tail call
        while (0) {
            // Do nothing
        }
    }
}

void adxm_goto_mwidle_border() {
    adxm_goto_svr_border(adxm_id_mwidle, adxm_save_tprm[6]);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_goto_usridle_border);

void adxm_safe_trap() {
    adxm_safe_cnt += 1;
}

void adxm_safe_proc() {
    while (adxm_safe_loop == 0) {
        adxm_safe_trap();
    }

    adxm_safe_exit = 1;
    ExitDeleteThread();
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_usrvsync_proc);

void adxm_vsync_proc() {
    while (adxm_vsync_loop == 0) {
        adxm_vsync_cnt += 1;
        adxm_SuspendThread(adxm_id_mwidle);

        if (adxm_IsSetupThrdEx() == 1) {
            adxm_SuspendThread(adxm_id_usridle);
        }

        ADXPS2_WakeupMainThrd();
        adxm_vsyncproc_exec_flag = 1;
        SVM_ExecSvrVsync();
        adxm_vsyncproc_exec_flag = 0;
        adxm_SleepThread();
    }

    adxm_vsync_exit = 1;
    ExitDeleteThread();
}

void adxm_fs_proc() {
    while (adxm_fs_loop == 0) {
        adxm_fs_cnt += 1;
        adxm_fsproc_exec_flag = 1;

        if (adxm_debug_bgcl != 0) {
            *GS_BGCOLOR = adxm_debug_bgcl;
        }

        SVM_ExecSvrFs();

        if (adxm_debug_bgcl != 0) {
            *GS_BGCOLOR = 0;
        }

        adxm_fsproc_exec_flag = 0;
        adxm_SleepThread();
    }

    adxm_fs_exit = 1;
    ExitDeleteThread();
}

#if defined(TARGET_PS2)
void adxm_mwidle_proc();
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_mwidle_proc);
#else
// Never actually called
void adxm_mwidle_proc() {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_usridle_proc);

void adxm_iWakeupThread(Sint32 tid) {
    struct ThreadParam param;

    if (tid != TH_SELF) {
        iReferThreadStatus(tid, &param);

        if ((param.status == THS_WAIT) || (param.status == THS_WAITSUSPEND)) {
            iWakeupThread(tid);
        }
    }
}

void adxm_WakeupThread(Sint32 tid) {
    struct ThreadParam param;

    if (tid != TH_SELF) {
        ReferThreadStatus(tid, &param);

        if ((param.status == THS_WAIT) || (param.status == THS_WAITSUSPEND)) {
            WakeupThread(tid);
        }
    }
}

void adxm_SleepThread() {
    SleepThread();
}

Sint32 adxm_ResumeThread(Sint32 tid) {
    struct ThreadParam param;
    Sint32 ret = 0;

    if (tid != TH_SELF) {
        ReferThreadStatus(tid, &param);

        if ((param.status == THS_SUSPEND) || (param.status == THS_WAITSUSPEND)) {
            ret = ResumeThread(tid);
        } else {
            ret = 0;
        }
    }

    return ret;
}

Sint32 adxm_SuspendThread(Sint32 tid) {
    struct ThreadParam param;
    Sint32 ret = 0;

    if (tid != TH_SELF) {
        ReferThreadStatus(tid, &param);

        if ((param.status != THS_SUSPEND) && (param.status != THS_WAITSUSPEND)) {
            ret = SuspendThread(tid);
        }
    }

    return ret;
}

void adxm_SetupSafeThrd() {
    struct ThreadParam param;

    param.entry = adxm_safe_proc;
    param.stack = adxm_stack_safe;
    param.stackSize = sizeof(adxm_stack_safe);
    param.initPriority = adxm_main_prio_def;
    param.gpReg = &_gp;

    adxm_id_safe = CreateThread(&param);
    adxm_id_safe;
    StartThread(adxm_id_safe, NULL);

    if (adxm_id_safe != 0) {
        adxm_SuspendThread(adxm_id_safe);
        adxm_id_safe;
    }

    ChangeThreadPriority(adxm_id_safe, adxm_save_tprm[1]);
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", D_0055AAF8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_SetupUsrVsyncThrd);

void adxm_SetupVsyncThrd() {
    struct ThreadParam param;

    param.entry = adxm_vsync_proc;
    param.stack = adxm_stack_vsync;
    param.stackSize = sizeof(adxm_stack_vsync);
    param.initPriority = adxm_main_prio_def;
    param.gpReg = &_gp;

    adxm_id_vsync = CreateThread(&param);
    adxm_id_vsync;
    StartThread(adxm_id_vsync, NULL);
    ChangeThreadPriority(adxm_id_vsync, adxm_save_tprm[3]);
}

void adxm_SetupFsThrd() {
    struct ThreadParam param;

    param.entry = adxm_fs_proc;
    param.stack = adxm_stack_fs;
    param.stackSize = sizeof(adxm_stack_fs);
    param.initPriority = adxm_main_prio_def;
    param.gpReg = &_gp;

    adxm_id_fs = CreateThread(&param);
    adxm_id_fs;
    StartThread(adxm_id_fs, NULL);
    ChangeThreadPriority(adxm_id_fs, adxm_save_tprm[4]);
}

void adxm_SetupMwIdleThrd() {
    struct ThreadParam param;

    param.entry = adxm_mwidle_proc;
    param.stack = adxm_stack_mwidle;
    param.stackSize = sizeof(adxm_stack_mwidle);
    param.initPriority = adxm_main_prio_def;
    param.gpReg = &_gp;

    adxm_id_mwidle = CreateThread(&param);
    adxm_id_mwidle;
    StartThread(adxm_id_mwidle, NULL);
    ChangeThreadPriority(adxm_id_mwidle, adxm_save_tprm[6]);
    adxm_SuspendThread(adxm_id_mwidle);
    adxm_id_mwidle;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", D_0055AB38);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_SetupUsrIdleThrd);

Sint32 adxm_IsSetupThrdEx() {
    return adxm_init_ex;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecSvrUsrVsync);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecSvrVsync);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecSvrFs);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecSvrMain);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecSvrMwIdle);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecSvrUsrIdle);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecSvrAll);

Sint32 ADXM_IsSetupThrd() {
    return adxm_init_level != 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_SetCbErr);

void ADXM_SetupThrd(ADXM_TPRM* tprm) {
    struct ThreadParam param;
    ADXM_TPRM_EX* tprm_ex = (ADXM_TPRM_EX*)tprm;

    adxps2_build;

    adxm_waiting_main_th_flag = 0;

    if (adxm_init_level == 0) {
        SVM_Init();
        SVM_SetCbLock(_adxm_lock, NULL);
        SVM_SetCbUnlock(_adxm_unlock, NULL);

        if (tprm == NULL) {
            adxm_save_tprm[5] = 24;
            adxm_save_tprm[0] = 1;
            adxm_save_tprm[1] = 8;
            adxm_save_tprm[3] = 16;
            adxm_save_tprm[4] = 18;
            adxm_save_tprm[6] = 25;
        } else {
            adxm_save_tprm[5] = tprm_ex->prio_main;
            adxm_save_tprm[0] = tprm_ex->prio_lock;
            adxm_save_tprm[1] = tprm_ex->prio_safe;
            adxm_save_tprm[3] = tprm_ex->prio_usrvsync;
            adxm_save_tprm[4] = tprm_ex->prio_vsync;
            adxm_save_tprm[6] = tprm_ex->prio_mwidle;
        }

        adxm_id_main = GetThreadId();
        ReferThreadStatus(adxm_id_main, &param);
        adxm_main_prio_def = param.currentPriority;
        adxm_SetupSafeThrd();
        adxm_SetupVsyncThrd();
        adxm_SetupFsThrd();
        adxm_SetupMwIdleThrd();
        ChangeThreadPriority(adxm_id_main, adxm_save_tprm[5]);
        SVM_SetCbBdr(6, adxm_goto_mwidle_border, 0);
    }

    adxm_init_level += 1;
}

void adxm_ShutdownPreProcedure() {
    adxm_safe_loop = 1;
    adxm_usrvsync_loop = 1;
    adxm_vsync_loop = 1;
    adxm_fs_loop = 1;
    adxm_mwidle_loop = 1;
    adxm_usridle_loop = 1;
}

void ADXM_ShutdownThrd() {
    if (--adxm_init_level == 0) {
        adxm_ShutdownPreProcedure();
        adxm_ShutdownMwIdleThrd();
        adxm_ShutdownFsThrd();
        adxm_ShutdownVsyncThrd();
        adxm_ShutdownSafeThrd();
        ChangeThreadPriority(adxm_id_main, adxm_main_prio_def);
        SVM_SetCbLock(NULL, NULL);
        SVM_SetCbUnlock(NULL, NULL);
        SVM_Finish();
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_ShutdownUsrIdleThrd);

void adxm_ShutdownMwIdleThrd() {
    while (adxm_mwidle_exit == 0) {
        adxm_mwidle_loop = 1;
        ChangeThreadPriority(adxm_id_mwidle, 1);
        WakeupThread(adxm_id_mwidle);
        ResumeThread(adxm_id_mwidle);
    }

    adxm_mwidle_exit = 0;
    adxm_mwidle_loop = 0;
    adxm_id_mwidle = 0;
}

void adxm_ShutdownVsyncThrd() {
    while (adxm_vsync_exit == 0) {
        adxm_vsync_loop = 1;
        ChangeThreadPriority(adxm_id_vsync, 1);
        WakeupThread(adxm_id_vsync);
        ResumeThread(adxm_id_vsync);
    }

    adxm_vsync_exit = 0;
    adxm_vsync_loop = 0;
    adxm_id_vsync = 0;
}

void adxm_ShutdownFsThrd() {
    while (adxm_fs_exit == 0) {
        adxm_fs_loop = 1;
        ChangeThreadPriority(adxm_id_fs, 1);
        WakeupThread(adxm_id_fs);
        ResumeThread(adxm_id_fs);
    }

    adxm_fs_exit = 0;
    adxm_fs_loop = 0;
    adxm_id_fs = 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", adxm_ShutdownUsrVsyncThrd);

void adxm_ShutdownSafeThrd() {
    while (adxm_safe_exit == 0) {
        adxm_safe_loop = 1;
        ChangeThreadPriority(adxm_id_safe, 1);
        WakeupThread(adxm_id_safe);
        ResumeThread(adxm_id_safe);
    }

    adxm_safe_exit = 0;
    adxm_safe_loop = 0;
    adxm_id_safe = 0;
}

void ADXM_Lock() {
    SVM_Lock();
}

void ADXM_Unlock() {
    SVM_Unlock();
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_GetLockLevel);

Sint32 ADXPS2_ExecVint(Sint32 arg) {
    adxt_vsync_cnt += 1;

    if (adxps2_exec_vint == 0) {
        adxps2_exec_vint = 1;

        if (adxm_init_level > 0) {
            if (adxm_vsyncproc_exec_flag != 1) {
                adxm_iWakeupThread(adxm_id_vsync);
            }

            if (adxm_fsproc_exec_flag != 1) {
                adxm_iWakeupThread(adxm_id_fs);
            }

            if ((adxm_IsSetupThrdEx() == 1) && (adxm_usrvsyncproc_exec_flag != 1)) {
                adxm_iWakeupThread(adxm_id_usrvsync);
            }

            adxm_vint_cnt += 1;
        }

        SVM_ExecSvrVint();
        adxps2_exec_vint = 0;
    }

    if (adxm_old_cbf != NULL) {
        return adxm_old_cbf(arg);
    }

    return 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ExecVint);

Sint32 ADXM_ExecMain() {
    return SVM_ExecSvrMain();
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_ResumeIdleThrd);

void ADXPS2_WakeupMainThrd() {
    struct ThreadParam param;

    if (adxm_waiting_main_th_flag != 1) {
        return;
    }

    ReferThreadStatus(adxm_id_main, &param);

    if ((param.status != THS_WAIT) && (param.status != THS_WAITSUSPEND)) {
        return;
    }

    if (WakeupThread(adxm_id_main) != adxm_id_main) {
        return;
    }

    adxm_waiting_main_th_flag = 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_SleepMainThrd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_WaitVsync);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_WaitVsync);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_ExecServerEx);

void ADXPS2_SetupThrd(ADXPS2_TPRM* tprm) {
    ADXM_TPRM_EX _tprm;
    ADXM_TPRM_EX* tprm_p;

    if (tprm != NULL) {
        _tprm.prio_main = tprm->prio_main;
        _tprm.prio_lock = tprm->prio_lock;
        _tprm.prio_safe = tprm->prio_safe;
        _tprm.prio_vsync = tprm->prio_adx + 1;
        _tprm.prio_mwidle = 25;
        _tprm.prio_usrvsync = tprm->prio_adx;
        tprm_p = &_tprm;
    } else {
        tprm_p = NULL;
    }

    ADXM_SetupThrd(tprm_p);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_ShutdownThrd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_ExecServer);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_Lock);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXPS2_Unlock);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_SetupThrdEx);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_ShutdownThrdEx);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_SetCbUsrVsync);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_SetCbUsrIdle);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_mps2", ADXM_GotoUsrIdleBdr);
