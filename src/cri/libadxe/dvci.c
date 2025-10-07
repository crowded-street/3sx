#include "common.h"
#include <cri/private/libadxe/cri_srd.h>
#include <cri/private/libadxe/dvci_sub.h>
#include <cri/private/libadxe/structs.h>
#include <cri/private/libadxe/svm.h>

#include <cri/ee/cri_xpt.h>

#include <eekernel.h>
#include <libcdvd.h>

#include <string.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define CLAMP(val, min, max) (MAX(MIN((val), (max)), (min)))

// forward decls
void dvCiExecServer();
void dvCiEntryErrFunc(void (*func)(void*, const Char8*, void*), void* obj);
Sint32 dvCiGetFileSize(const Char8* fname);
DVG_CI dvCiOpen(Char8* fname, void* arg1, Sint32 rw);
void dvCiClose(DVG_CI handl);
Sint32 dvCiSeek(DVG_CI handl, Sint32 offset, Sint32 whence);
Sint32 dvCiTell(DVG_CI handl);
Sint32 dvCiReqRd(DVG_CI handl, Sint32 nsct, void* buf);
void dvCiStopTr(DVG_CI handl);
Sint32 dvCiGetStat(DVG_CI handl);
Sint32 dvCiGetSctLen();
Sint32 dvCiGetNumTr(DVG_CI handl);
Sint32 dvCiIsExistFile(const Char8* fname);
Sint32 dvCiOptFn1(DVG_CI handl, Sint32 arg1);
void dvCiExecHndl(DVG_CI dvg_ci);

// data
Char8* volatile dvg_ci_build = "\nDVCI/PS2EE Ver.2.64 Build:Sep 18 2003 10:00:10\n";
void (*dvg_ci_err_func)(void*, const Char8*, void*) = NULL;
void* dvg_ci_err_obj = NULL;
Sint32 dvg_ci_dvd_layer = 0;
Sint32 dvci_execserver_enter_flg = 0;

CVFSDevice dvg_ci_vtbl = { dvCiExecServer,
                           dvCiEntryErrFunc,
                           dvCiGetFileSize,
                           NULL,
                           dvCiOpen,
                           dvCiClose,
                           dvCiSeek,
                           dvCiTell,
                           dvCiReqRd,
                           NULL,
                           dvCiStopTr,
                           dvCiGetStat,
                           dvCiGetSctLen,
                           NULL,
                           dvCiGetNumTr,
                           NULL,
                           dvCiIsExistFile,
                           NULL,
                           NULL,
                           NULL,
                           NULL,
                           NULL,
                           NULL,
                           NULL,
                           dvCiOptFn1,
                           NULL };

DVG_CI_OBJ dvg_ci_obj[DVG_CI_MAX_OBJ] = { 0 };
Char8 dvg_ci_fname[297] = { 0 };

CVFSDevice* dvCiGetInterface() {
    dvg_ci_build;
    dvCiSetRootDir(NULL);
    return &dvg_ci_vtbl;
}

void dvci_call_errfn(void* obj, const Char8* msg) {
    if (dvg_ci_err_func != NULL) {
        dvg_ci_err_func(dvg_ci_err_obj, msg, obj);
    }
}

Sint32 dvCiCdSearchFile(sceCdlFILE* fp, const Char8* fname) {
    Sint32 err;

    SRD_SetHistory(0x8000);
    err = sceCdLayerSearchFile(fp, fname, dvg_ci_dvd_layer);
    SRD_SetHistory(0x8001);
    return err;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci", dvci_is_exec_hn);

void dvCiExecHndl(DVG_CI dvg_ci) {
    if (dvg_ci->srd == NULL) {
        return;
    }

    SVM_LockVar();

    switch (SRD_GetStat(dvg_ci->srd)) {
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        // Do nothing
        break;

    case 9:
        dvg_ci->stat = 3;
        SRD_Destroy(dvg_ci->srd);
        dvg_ci->srd = NULL;
        break;

    case 3:
        dvg_ci->unk14 = dvg_ci->unk10;
        dvg_ci->stat = 1;
        dvg_ci->unkC += dvg_ci->unk10;
        SRD_Destroy(dvg_ci->srd);
        dvg_ci->srd = NULL;
        break;

    case 0:
        dvg_ci->stat = 0;
        SRD_Destroy(dvg_ci->srd);
        dvg_ci->srd = NULL;
        break;
    }

    SVM_UnlockVar();
}

void dvCiExecServer() {
    DVG_CI dvg_ci;
    Sint32 i;

    if (SVM_TestAndSet(&dvci_execserver_enter_flg) == 0) {
        return;
    }

    for (i = 0; i < DVG_CI_MAX_OBJ; i++) {
        dvg_ci = &dvg_ci_obj[i];

        if (dvg_ci->used == 1) {
            dvCiExecHndl(dvg_ci);
        }
    }

    dvci_execserver_enter_flg = 0;
}

void dvCiEntryErrFunc(void (*func)(void*, const Char8*, void*), void* obj) {
    dvg_ci_err_func = func;
    dvg_ci_err_obj = obj;
}

void dvci_to_large_to_yen(Char8* path) {
    size_t len = strlen(path);
    Sint32 i;

    for (i = 0; i < len; i++) {
        if (path[i] == '/') {
            path[i] = '\\';
        }

        if (path[i] >= 'a' && path[i] <= 'z') {
            path[i] -= 'a' - 'A';
        }
    }
}

Sint32 dvCiIsExistFile(const Char8* fname) {
    sceCdlFILE fp;
    Sint32 err;

    if (fname == NULL) {
        return 0;
    }

    dvci_get_fstate(fname, &fp);

    if (fp.size == 0) {
        dvci_conv_fname(fname, dvg_ci_fname);

        SRD_SetHistory(0x8100);
        sceCdSync(0);
        SRD_SetHistory(0x8101);

        err = dvCiCdSearchFile(&fp, dvg_ci_fname);

        if (err == 0) {
            return 0;
        }
    }

    return 1;
}

Sint32 dvCiGetFileSize(const Char8* fname) {
    sceCdlFILE fp;

    if (fname == NULL) {
        dvci_call_errfn(NULL, "E0092901:fname is null.(dvCiGetFileSize)");
        return 0;
    }

    dvci_get_fstate(fname, &fp);

    if (fp.size == 0) {
        dvci_conv_fname(fname, dvg_ci_fname);

        SRD_SetHistory(0x8200);
        sceCdSync(0);
        SRD_SetHistory(0x8201);

        if (dvCiCdSearchFile(&fp, dvg_ci_fname) == 0) {
            if (dvg_ci_dbg_out_lv != 2) {
                scePrintf("DVCI: sceCdSearchFile failed. \"%s\"\n", dvg_ci_fname);
            }

            dvci_call_errfn(NULL, "E0092902:can't find file.(dvCiGetFileSize)");
            return 0;
        }
    }

    return fp.size;
}

DVG_CI dvci_alloc() {
    DVG_CI dvg_ci = NULL;
    Sint32 i;

    for (i = 0; i < DVG_CI_MAX_OBJ; i++) {
        if (dvg_ci_obj[i].used == 0) {
            dvg_ci = &dvg_ci_obj[i];
        }
    }

    return dvg_ci;
}

void dvci_free(DVG_CI dvg_ci) {
    memset(dvg_ci, 0, sizeof(DVG_CI_OBJ));
}

DVG_CI dvCiOpen(Char8* fname, void* arg1, Sint32 rw) {
    DVG_CI dvg_ci;
    Sint32 fstate;

    if (fname == NULL) {
        dvci_call_errfn(fname, "E0092908:fname is null.(dvCiOpen)");
        return NULL;
    }

    if (rw != 0) {
        dvci_call_errfn(NULL, "E0092909:rw is illigal.(dvCiOpen)");
        return NULL;
    }

    dvg_ci = dvci_alloc();

    if (dvg_ci == NULL) {
        dvci_call_errfn(NULL, "E0092910:not enough handle resource.(dvCiOpen)");
        return NULL;
    }

    fstate = dvci_get_fstate(fname, &dvg_ci->unk20);

    if (dvg_ci->unk20.size == 0) {
        dvci_conv_fname(fname, dvg_ci_fname);

        if (dvg_ci_dbg_out_lv != 2) {
            scePrintf("DVCI: File cache was not hit. \"%s\"\n", dvg_ci_fname);
        }

        SRD_SetHistory(0x8300);
        sceCdSync(0);
        SRD_SetHistory(0x8301);

        if (dvCiCdSearchFile(&dvg_ci->unk20, dvg_ci_fname) == 0) {
            dvci_call_errfn(NULL, "E0092911:sceCdSearchFile fail.(dvCiOpen)\0\0\0\0");
            dvci_free(dvg_ci);
            return NULL;
        }

        dvg_ci->unk1 = 0;
    } else {
        dvg_ci->unk1 = 1;
    }

    dvg_ci->unk8 = (dvg_ci->unk20.size + 2048 - 1) / 2048;
    dvg_ci->used = 1;
    dvg_ci->unk4 = dvg_ci->unk20.size;
    dvg_ci->unkC = 0;
    dvg_ci->unk18 = 0;
    dvg_ci->unk10 = 0;
    dvg_ci->unk14 = 0;
    dvg_ci->stat = 0;
    dvg_ci->srd = NULL;

    if (fstate == 1) {
        dvg_ci->unk8 = 0x7FFFFFFF;
    }

    return dvg_ci;
}

void dvCiClose(DVG_CI handl) {
    if (handl != NULL) {
        if ((Uint8)handl->stat >= 2) {
            dvCiStopTr(handl);
        }

        handl->used = 0;
        dvci_free(handl);
    }
}

Sint32 dvCiSeek(DVG_CI handl, Sint32 offset, Sint32 whence) {
    if (handl == NULL) {
        dvci_call_errfn(handl, "E0092912:handl is null.");
        return 0;
    }

    if (whence == 0) {
        handl->unkC = offset;
    } else if (whence == 2) {
        handl->unkC = handl->unk8 + offset;
    } else if (whence == 1) {
        handl->unkC += offset;
    }

    handl->unk14 = 0;
    handl->unkC = CLAMP(handl->unkC, 0, handl->unk8);
    return handl->unkC;
}

Sint32 dvCiTell(DVG_CI handl) {
    if (handl == NULL) {
        dvci_call_errfn(handl, "E0092912:handl is null.");
        return 0;
    }

    return handl->unkC;
}

Sint32 dvCiReqRd(DVG_CI handl, Sint32 nsct, void* buf) {
    if (handl == NULL) {
        dvci_call_errfn(handl, "E0092912:handl is null.");
        return NULL;
    }

    if (nsct < 0) {
        dvci_call_errfn(handl, "E0092913:nsct < 0.(dvCiReqRd)");
        return NULL;
    }

    if (buf == NULL) {
        dvci_call_errfn(handl, "E0092914:buf is null.(dvCiReqRd)");
        return NULL;
    }

    if (handl->stat == 2) {
        return NULL;
    }

    if (nsct == 0) {
        handl->stat = 1;
        return NULL;
    }

    handl->srd = SRD_Create();

    if (handl->srd == NULL) {
        return NULL;
    }

    handl->cdrmode.trycount = dvg_ci_cdrmode.trycount;
    handl->cdrmode.spindlctrl = dvg_ci_cdrmode.spindlctrl;
    handl->cdrmode.datapattern = dvg_ci_cdrmode.datapattern;
    handl->unk18 = buf;
    handl->unk10 = MIN(nsct, handl->unk8 - handl->unkC);

    InvalidDCache(buf, buf + (handl->unk10 << 11) - 1);

    if (SRD_ReqRdDvd(handl->srd, handl->unk20.lsn + handl->unkC, handl->unk10, handl->unk18, &handl->cdrmode) == 0) {
        SRD_Destroy(handl->srd);
        return NULL;
    }

    if (dvg_ci_rdmode == 1) {
        sceCdSync(0);
        SRD_WaitForExecServer();
        SRD_ExecServer();
        SRD_WaitComplete(handl->srd);
        SRD_Destroy(handl->srd);
        handl->srd = NULL;
        handl->unk14 = nsct;
        handl->stat = 1;
        handl->unkC += handl->unk10;
    } else {
        handl->stat = 2;
    }

    return handl->unk10;
}

void dvCiStopTr(DVG_CI handl) {
    Sint32 var_s2 = 0;

    if (handl == NULL) {
        dvci_call_errfn(handl, "E0092912:handl is null.");
        return;
    }

    if (handl->stat < 2U) {
        return;
    }

    if (handl->srd != NULL) {
        SRD_Break(handl->srd);
    }

    while (1) {
        SRD_ExecServer();
        dvCiExecServer();
        dvCiGetStat(handl);

        if (handl->stat != 2) {
            break;
        }

        var_s2 += 1;

        if (var_s2 > 0x9FFFF) {
            if (dvg_ci_dbg_out_lv != 2) {
                scePrintf("DVCI: dvCiStopTr timeout. (10sec)\r\n");
            }

            break;
        }
    }

    handl->stat = 0;
    handl->unk14 = 0;
}

Sint32 dvCiGetStat(DVG_CI handl) {
    if (handl == NULL) {
        dvci_call_errfn(handl, "E0092912:handl is null.");
        return 0;
    }

    return handl->stat;
}

Sint32 dvCiGetSctLen() {
    return 0x800;
}

Sint32 dvCiGetNumTr(DVG_CI handl) {
    if (handl == NULL) {
        dvci_call_errfn(handl, "E0092912:handl is null.");
        return 0;
    }

    if (handl->stat == 1) {
        return handl->unk14 << 11;
    }

    return 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci", dvCiSetOutputDebugLevel);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci", dvCiGetOutputDebugLevel);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci", dvCiSetDvdLayer);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci", dvCiGetDvdLayer);

Sint32 dvci_getfilesize32(const Char8* fname) {
    sceCdlFILE fp;

    dvci_get_fstate(fname, &fp);

    if (fp.size == 0) {
        dvci_conv_fname(fname, dvg_ci_fname);

        SRD_SetHistory(0x8400);
        sceCdSync(0);
        SRD_SetHistory(0x8401);

        if (dvCiCdSearchFile(&fp, dvg_ci_fname) == 0) {
            if (dvg_ci_dbg_out_lv != 2) {
                scePrintf("DVCI: sceCdSearchFile failed. \"%s\"\n", dvg_ci_fname);
            }

            return 0;
        }
    }

    return fp.size;
}

Sint32 dvci_getnumtr_upper(DVG_CI handl) {
    Sint64 val = handl->unk14;
    val <<= 11;
    return val >> 32;
}

Sint32 dvci_getnumtr_lower(DVG_CI handl) {
    Sint64 val = handl->unk14;
    val <<= 11;
    return val & 0xFFFFFFFF;
}

Sint32 dvci_filesize_upper(const Char8* fname) {
    return 0;
}

Sint32 dvci_filesize_lower(const Char8* fname) {
    return dvci_getfilesize32(fname);
}

Sint32 dvCiOptFn1(DVG_CI handl, Sint32 arg1) {
    Sint32 result = 0;

    if (!handl) {
        return 0;
    }

    switch (arg1) {
    case 0xC8:
        result = dvci_getnumtr_upper(handl);
        break;

    case 0xC9:
        result = dvci_getnumtr_lower(handl);
        break;

    case 0xCA:
        result = dvci_filesize_upper(handl);
        break;

    case 0xCB:
        result = dvci_filesize_lower(handl);
        break;

    case 0xCC:
        result = dvci_filesize_upper(handl);
        break;

    case 0xCD:
        result = dvci_filesize_lower(handl);
        break;

    case 0x12B:
        result = 1;
        break;
    }

    return result;
}
