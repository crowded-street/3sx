#include "common.h"
#include <cri/private/libadxe/svm.h>

#include <cri/ee/cri_xpt.h>
#include <eekernel.h>
#include <libcdvd.h>
#include <sifdev.h>

#include <string.h>

typedef struct {
    /* 0x00 */ Sint8 unk0;
    /* 0x01 */ Sint8 unk1;
    /* 0x02 */ Sint8 stat;
    /* 0x03 */ Sint8 unk3;
    /* 0x04 */ Sint32 unk4;
    /* 0x08 */ Sint32 unk8;
    /* 0x0C */ Sint32 unkC;
    /* 0x10 */ void* unk10;
    /* 0x14 */ sceCdRMode unk14;
    /* 0x18 */ void* unk18;
    /* 0x1C */ Sint32 unk1C;
    /* 0x20 */ Sint32 unk20;
    /* 0x24 */ Sint32 unk24;
    /* 0x28 */ Sint64 unk28;
    /* 0x30 */ Sint32 fd;
    /* 0x34 */ Sint32 unk34;
} SRD_OBJ;

typedef SRD_OBJ* SRD;

SRD_OBJ srd_obj = { 0 };
Char8* volatile srd_build = "\nSRD/PS2EE Ver.2.18a Build:Sep 18 2003 10:00:14\n\0\0\0\0";
Sint32 srd_enter_fg = 0;
Sint32 srd_dvd_exec_locked = 0;
Sint32 srd_hst_exec_locked = 0;
Sint32 volatile srd_geterror_locked = 0;
Sint32 volatile srd_geterror_call = 1;
Sint32 srd_wait_svr_cnt = 0;
Sint32 volatile srd_debug_geterror = 0;
Sint32 volatile srd_debug_rdbg_cnt = 0;
Sint32 volatile srd_debug_rded_cnt = 0;
Sint32 volatile srd_create_cnt = 0;
Sint32 srd_destroy_cnt = 0;
Sint32 volatile srd_history_pre = 0;
Sint32 volatile srd_history = 0;
Sint32 srd_filesystem64 = 0;
Sint32 srd_host_lock = 0;

// forward decls
void SRD_SetHistory(Sint32 arg0);
Sint32 SRD_SceIoctl(Sint32 fd, Sint32 req, void* arg2);
Sint64 SRD_SceLseek(Sint32 fd, Sint64 offset, Sint32 whence);
Sint32 SRD_SceRead(Sint32 fd, void* buf, Sint32 nbyte);

void srd_reset_obj() {
    srd_build;
    memset(&srd_obj, 0, sizeof(srd_obj));
}

SRD SRD_Create() {
    SRD srd = NULL;

    SVM_LockVar();

    if (srd_obj.unk0 == 0) {
        srd_reset_obj();
        srd_obj.unk0 = 1;
        srd_obj.stat = 0;
        srd_obj.unk3 = -1;
        srd_obj.unk4 = 0;
        srd_create_cnt += 1;
        srd = &srd_obj;
    }

    SVM_UnlockVar();

    return srd;
}

void SRD_Destroy(SRD srd) {
    SVM_LockVar();

    if (srd != &srd_obj) {
        scePrintf("SRD: SRD_Destroy, invalidate srd handle. hn=%0x, srd_obj=%08x\r\n", srd, &srd_obj);
    } else {
        if ((srd->stat != 0) && (srd->stat != 3) && (srd->stat != 9)) {
            scePrintf("SRD: invalid SRD_Destroy.\r\n");
        }

        srd_reset_obj();
        srd_obj.unk0 = 0;
        srd_destroy_cnt += 1;
    }

    SVM_UnlockVar();
}

s32 SRD_ReqRdDvd(SRD srd, s32 arg1, s32 arg2, void* arg3, sceCdRMode* arg4) {
    s32 err;
    s32 var_s4 = 0;

    SRD_SetHistory(0x1200);
    err = sceCdSync(1);
    SRD_SetHistory(0x1201);

    if (err == 1) {
        return 0;
    }

    SVM_LockVar();

    if ((srd->stat == 0) || (srd->stat == 3) || (srd->stat == 9)) {
        srd->unk1 = 1;
        srd->stat = 1;
        srd->unk8 = arg1;
        srd->unkC = arg2;
        srd->unk10 = arg3;
        srd->unk14 = *arg4;
        srd->unk4 = 0;
        var_s4 = 1;
    }

    SVM_UnlockVar();
    return var_s4;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_ReqRdHst);

Sint32 SRD_GetStat(SRD srd) {
    return srd->stat;
}

void srd_wait() {
    Sint32 i;

    for (i = 0; i < 0x4000; i++) {
        // Do nothing
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", srd_wait_dvd);

void srd_wait_hst(SRD srd) {
    Sint32 sp = 1;
    Sint32 stat;
    Sint32 i;

    i = 0;

    while (1) {
        if (sp != 1) {
            break;
        }

        SRD_SetHistory(0x1100);
        stat = SRD_SceIoctl(srd->fd, 1, &sp);
        SRD_SetHistory(0x1101);

        if (stat < 0) {
            srd->stat = 9;
            srd->unk34 = stat;
            return;
        }

        srd_wait();
        i++;

        if (i > 0x9000) {
            scePrintf("SRD: srd_wait_hst timeout.\r\n");
            break;
        }
    }
}

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_WaitComplete);
#else
void SRD_WaitComplete(SRD srd) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_WaitCompleteVoid);

#if defined(TARGET_PS2)
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CAF8);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CB18);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_Break);
#else
void SRD_Break(SRD srd) {
    not_implemented(__func__);
}
#endif

Sint32 srd_check_dvd_error(SRD srd) {
    Sint32 cd_err;

    if (srd_geterror_call == 0) {
        return 0;
    }

    if (srd->stat != 2) {
        return 0;
    }

    srd_debug_geterror = 1;

    if (srd_geterror_locked == 1) {
        SVM_LockRsc();
        SRD_SetHistory(0x1400);
        cd_err = sceCdGetError();
        SRD_SetHistory(0x1401);
        SVM_UnlockRsc();
    } else {
        SRD_SetHistory(0x1500);
        cd_err = sceCdGetError();
        SRD_SetHistory(0x1501);
    }

    srd_debug_geterror = 0;

    if ((cd_err + 1) >= 2U) {
        srd->unk34 = cd_err;
        scePrintf("SRD: Drive Error (sceCdGetError = 0x%x)\r\n\0\0\0\0", cd_err);
        return 1;
    }

    return 0;
}

void srd_exec_dvd(SRD srd) {
    Sint32 err;
    Sint32 dvd_err;

    if (srd->unk4 == 1) {
        switch (srd->stat) {
        case 2:
            SRD_SetHistory(0x1600);
            err = sceCdBreak();
            SRD_SetHistory(0x1601);

            if (err == 0) {
                scePrintf("SRD: sceCdBreak is fail. %d\n", srd_debug_geterror);
            }

            SRD_SetHistory(0x1700);
            sceCdSync(0);
            SRD_SetHistory(0x1701);
            srd->stat = 0;
            srd_debug_rded_cnt += 1;
            break;

        case 1:
        case 3:
        case 9:
            srd->stat = 0;
            break;

        case 0:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            break;
        }

        srd->unk4 = 0;
    }

    switch (srd->stat) {
    case 1:
        SRD_SetHistory(0x1800);
        err = sceCdRead(srd->unk8, srd->unkC, srd->unk10, &srd->unk14);
        SRD_SetHistory(0x1801);

        if (err == 1) {
            srd_debug_rdbg_cnt += 1;
            srd->stat = 2;
        } else {
            srd->stat = 9;
            srd->unk34 = -1;
        }

        break;
    }

    switch (srd->stat) {
    case 2:
        SRD_SetHistory(0x1900);
        err = sceCdSync(1);
        SRD_SetHistory(0x1901);

        dvd_err = srd_check_dvd_error(srd);

        if (err == 0) {
            srd->stat = (dvd_err == 1) ? 9 : 3;
            srd_debug_rded_cnt += 1;
        }

        break;
    }
}

void srd_exec_hst(SRD srd) {
    Sint32 sp;
    Sint64 offset;

    if (srd->unk4 == 1) {
        switch (srd->stat) {
        case 0:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            break;

        case 2:
            srd_wait_hst(srd);
            srd->stat = 0;
            srd_debug_rded_cnt += 1;
            /* fallthrough */

        case 1:
        case 3:
        case 9:
            srd->stat = 0;
            break;
        }

        srd->unk4 = 0;
    }

    switch (srd->stat) {
    case 1:
        SRD_SetHistory(0x2000);
        offset = SRD_SceLseek(srd->fd, (srd->unk28 << 11), 0);
        SRD_SetHistory(0x2001);

        if (offset < 0) {
            srd->stat = 9;
            srd->unk34 = (Sint32)offset;
        }

        SRD_SetHistory(0x2100);
        offset = SRD_SceRead(srd->fd, srd->unk18, srd->unk20);
        SRD_SetHistory(0x2101);

        if (offset >= 0) {
            srd->stat = 2;
            srd_debug_rdbg_cnt += 1;
        } else {
            srd->stat = 9;
            srd->unk34 = (Sint32)offset;
        }

        break;
    }

    switch (srd->stat) {
    case 2:
        SRD_SetHistory(0x2200);

        if (SRD_SceIoctl(srd->fd, 1, &sp) < 0) {
            SRD_SetHistory(0x2201);
            srd->stat = 9;
            srd->unk34 = -1;
        } else {
            SRD_SetHistory(0x2202);

            if (sp == 0) {
                srd->stat = 3;
                srd_debug_rded_cnt += 1;
            }
        }

        break;
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_IsExecute);

void SRD_ExecServer() {
    SRD srd = &srd_obj;

    if (SVM_TestAndSet(&srd_enter_fg) == 0) {
        return;
    }

    switch (srd->unk1) {
    case 1:
        if (srd_dvd_exec_locked == 1) {
            SVM_LockRsc();
        }

        srd_exec_dvd(srd);

        if (srd_dvd_exec_locked == 1) {
            SVM_UnlockRsc();
        }

        break;
    }

    switch (srd->unk1) {
    case 2:
        if (srd_hst_exec_locked == 1) {
            SVM_LockRsc();
        }

        srd_exec_hst(srd);

        if (srd_hst_exec_locked == 1) {
            SVM_UnlockRsc();
        }

        break;
    }

    srd_enter_fg = 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetDevTypeNow);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetErrCode);

#if defined(TARGET_PS2)
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CBD8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_WaitForExecServer);
#else
void SRD_WaitForExecServer() {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_LockedForDvdExec);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_LockedForHstExec);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_LockedForGetError);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_CallForGetError);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_SetError);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetReadStatusDvd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetReadStatusHost);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetReadStatus);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CC08);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CC18);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CC38);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CC58);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CC78);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CC98);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CCB8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_DebugPrint);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetReadEndCount);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetReadBeginCount);

void SRD_SetHistory(Sint32 arg0) {
    srd_history_pre = srd_history;
    srd_history = arg0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetHistory);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_SetFilesystem64);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CCD8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetFilesystem64);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", D_0055CCF8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_SetLockHost);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_GetLockHost);

void srd_sw_lock() {
    if (srd_host_lock == 1) {
        SVM_LockRsc(1);
    }
}

void srd_sw_unlock() {
    if (srd_host_lock == 1) {
        SVM_UnlockRsc(1);
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_srd", SRD_SetDvdSeekPos);

Sint64 SRD_SceLseek(Sint32 fd, Sint64 offset, Sint32 whence) {
    Sint64 ofst;

    srd_sw_lock();
    if (srd_filesystem64 == 1) {
        ofst = sceLseek64(fd, offset, whence);

        if (ofst < 0) {
            scePrintf("SRD: sceLseek64 error = %d\r\n", ofst);
        }
    } else {
        ofst = sceLseek(fd, (Sint32)offset, whence);

        if (ofst < 0) {
            scePrintf("SRD: sceLseek error = %d\r\n", ofst);
        }
    }

    srd_sw_unlock();
    return ofst;
}

Sint32 SRD_SceIoctl(Sint32 fd, Sint32 req, void* arg2) {
    Sint32 err;

    srd_sw_lock();
    err = sceIoctl(fd, req, arg2);

    if (err < 0) {
        scePrintf("SRD: sceIoctl error = %d\r\n", err);
    }

    srd_sw_unlock();
    return err;
}

Sint32 SRD_SceOpen(const Char8* filename, Sint32 flag) {
    Sint32 err;

    srd_sw_lock();
    err = sceOpen(filename, flag);

    if (err < 0) {
        scePrintf("SRD: sceOpen error = %d\r\n", err);
    }

    srd_sw_unlock();
    return err;
}

Sint32 SRD_SceClose(Sint32 fd) {
    Sint32 err;

    srd_sw_lock();
    err = sceClose(fd);

    if (err < 0) {
        scePrintf("SRD: sceClose error = %d\r\n", err);
    }

    srd_sw_unlock();
    return err;
}

Sint32 SRD_SceRead(Sint32 fd, void* buf, Sint32 nbyte) {
    Sint32 err;

    srd_sw_lock();
    err = sceRead(fd, buf, nbyte);

    if (err < 0) {
        scePrintf("SRD: sceRead error = %d\r\n", err);
    }

    srd_sw_unlock();
    return err;
}
