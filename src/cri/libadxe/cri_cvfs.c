#include "common.h"
#include <cri/private/libadxe/structs.h>

#include <cri/cri_xpts.h>

#include <stdio.h>
#include <string.h>

#define CVFS_DEVICE_MAX 32
#define CVFS_HANDLE_MAX 40
#define CVFS_MAX_NAME_LENGTH 0x129

typedef struct {
    CVFSDevice* device;
    Char8 name[12];
} CVFSNamedDevice;

Char8* volatile cvfs_build = "\nCVFS/PS2EE Ver.2.34 Build:Sep 18 2003 10:00:09\n\0\0\0\0";
void (*cvfs_errfn)(void*, const Char8*) = NULL;
void* cvfs_errobj = NULL;
Sint32 cvfs_init_cnt = 0;

#if defined(TARGET_PS2)
extern Char8 D_006BDDB8[0x148];
extern Char8 D_006BDDA8[0x10];
extern CVFSNamedDevice D_006BDBA8[CVFS_DEVICE_MAX]; // cvfs_named_devices
extern CVFSHandle D_006BDA68[CVFS_HANDLE_MAX];      // cvfs_handles
#else
Char8 D_006BDDB8[0x148];
Char8 D_006BDDA8[0x10];
CVFSNamedDevice D_006BDBA8[CVFS_DEVICE_MAX]; // cvfs_named_devices
CVFSHandle D_006BDA68[CVFS_HANDLE_MAX];      // cvfs_handles
#endif

// forward decls
void getDevName(Char8* filename, Char8* device_name, const Char8* full_path);
CVFSHandle* allocCvFsHn();
void releaseCvFsHn(CVFSHandle* handle);
void getDefDev(Char8* arg0);
void addDevName(const Char8* device_name, Char8* out);
CVFSDevice* getDevice(const Char8* name);
CVFSDevice* addDevice(const Char8* device_name, CVFSDevice* (*device_provider)());
Sint32 isExistDev(const Char8*, strlen_t);
void toUpperStr(Char8* str);

void cvFsCallUsrErrFn(void* object, const Char8* msg, Sint32 arg2) {
    if (cvfs_errfn != NULL) {
        cvfs_errfn(cvfs_errobj, msg);
    }
}

void cvFsError(const Char8* msg) {
    cvFsCallUsrErrFn(&cvfs_errobj, msg, 0);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsInit);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsFinish);

void cvFsAddDev(const Char8* device_name, CVFSDevice* (*device_provider)(), Sint32 arg2) {
    CVFSDevice* device;

    cvfs_build;

    if (device_name == NULL) {
        cvFsError("cvFsAddDev #1:illegal device name");
        return;
    }

    if (device_provider == NULL) {
        cvFsError("cvFsAddDev #2:illegal I/F func name");
        return;
    }

    device = addDevice(device_name, device_provider);

    if (device == NULL) {
        cvFsError("cvFsAddDev #3:failed added a device\0\0\0\0");
        return;
    }

    if (device->EntryErrFunc != NULL) {
        device->EntryErrFunc(cvFsCallUsrErrFn, NULL);
    }
}

CVFSDevice* addDevice(const Char8* device_name, CVFSDevice* (*device_provider)()) {
    CVFSDevice* device;
    Sint32 i = 0;

    toUpperStr(device_name);
    device = device_provider();

    if (getDevice(device_name) != NULL) {
        return device;
    }

    for (i = 0; i < CVFS_DEVICE_MAX; i++) {
        if (D_006BDBA8[i].name[0] == '\0') {
            break;
        }
    }

    if (i == CVFS_DEVICE_MAX) {
        return NULL;
    }

    D_006BDBA8[i].device = device;
    memcpy(D_006BDBA8[i].name, device_name, strlen(device_name) + 1);
    return device;
}

CVFSDevice* getDevice(const Char8* name) {
    strlen_t len = strlen(name);
    Uint32 i;

    for (i = 0; i < CVFS_DEVICE_MAX; i++) {
        if (strncmp(name, &D_006BDBA8[i].name, len) == 0) {
            return D_006BDBA8[i].device;
        }
    }

    return NULL;
}

void toUpperStr(Char8* str) {
    size_t len = strlen(str);
    Sint32 i;

    for (i = 0; i < len + 1; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= ('a' - 'A');
        }
    }
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055BE38);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsDelDev);

void cvFsSetDefDev(const Char8* device_name) {
    strlen_t device_name_len;

    if (device_name == NULL) {
        cvFsError("cvFsSetDefDev #1:illegal device name");
        return;
    }

    device_name_len = strlen(device_name);

    if (device_name_len == 0) {
        D_006BDDA8[0] = '\0';
        return;
    }

    toUpperStr(device_name);

    if (isExistDev(device_name, device_name_len) == 1) {
        memcpy(D_006BDDA8, device_name, device_name_len + 1);
        return;
    }

    cvFsError("cvFsSetDefDev #2:unknown device name");
}

Sint32 isExistDev(const Char8* device_name, strlen_t device_name_length) {
    Sint32 i;

    for (i = 0; i < CVFS_DEVICE_MAX; i++) {
        if (strncmp(device_name, D_006BDBA8[i].name, device_name_length) == 0) {
            return 1;
        }
    }

    return 0;
}

Char8* cvFsGetDefDev() {
    return D_006BDDA8;
}

CVFSDevice* variousProc(Char8* filename, Char8* device_name, const Char8* full_path) {
    CVFSDevice* device;

    if (filename[0] == '\0') {
        getDefDev(filename);

        if (filename[0] == '\0') {
            return NULL;
        }
    }

    addDevName(filename, device_name);
    device = getDevice(filename);

    if (device == NULL) {
        getDefDev(filename);
        device = getDevice(filename);

        if (device == NULL) {
            return NULL;
        }

        strcpy(device_name, full_path);
    }

    return device;
}

CVFSHandle* cvFsOpen(const Char8* fname, void* arg1, Sint32 arg2) {
    Char8 filename[CVFS_MAX_NAME_LENGTH + 1];
    Char8 device_name[CVFS_MAX_NAME_LENGTH + 1];
    CVFSHandle* fs_hn;
    CVFSDevice* device;

    if (fname == NULL) {
        cvFsError("cvFsOpen #1:illegal file name");
        return NULL;
    }

    getDevName(filename, device_name, fname);

    if (device_name[0] == '\0') {
        // Looks like this one should be "cvFsOpen #2:illegal device name"
        cvFsError("cvFsOpen #1:illegal file name");
        return NULL;
    }

    fs_hn = allocCvFsHn();

    if (fs_hn == NULL) {
        cvFsError("cvFsOpen #3:failed handle alloced\0\0\0\0\0\0\0cvFsOpen #2:illegal device name");
        return NULL;
    }

    device = variousProc(filename, device_name, fname);
    fs_hn->device = device;

    if (device == NULL) {
        releaseCvFsHn(fs_hn);
        cvFsError("cvFsOpen #4:device not found");
        return NULL;
    }

    if (device->Open != NULL) {
        fs_hn->fd = device->Open(device_name, arg1, arg2);
    } else {
        releaseCvFsHn(fs_hn);
        cvFsError("cvFsOpen #5:vtbl error");
        return NULL;
    }

    if (fs_hn->fd == NULL) {
        releaseCvFsHn(fs_hn);
        cvFsError("cvFsOpen #6:open failed");
        return NULL;
    }

    return fs_hn;
}

CVFSHandle* allocCvFsHn() {
    Sint32 i;

    for (i = 0; i < CVFS_HANDLE_MAX; i++) {
        if (D_006BDA68[i].fd == NULL) {
            break;
        }
    }

    if (i == CVFS_HANDLE_MAX) {
        return NULL;
    }

    return &D_006BDA68[i];
}

void releaseCvFsHn(CVFSHandle* handle) {
    handle->fd = NULL;
    handle->device = NULL;
}

/// @brief Parses a device-qualified path.
/// @param[out] filename Destination for the filename (after the colon).
/// @param[out] device_name Destination for the device name (before the colon).
/// @param[in] full_path Input string in the form "device:file".
void getDevName(Char8* filename, Char8* device_name, const Char8* full_path) {
    Char8* dev_name = device_name;
    Sint32 i;
    Sint32 j;

    if (full_path == NULL) {
        return;
    }

    i = 0;

    while (i < CVFS_MAX_NAME_LENGTH && full_path[i] != ':' && full_path[i] != '\0') {
        filename[i] = full_path[i];
        i++;
    }

    if (full_path[i] == '\0') {
        filename[i] = '\0';
        memcpy(device_name, filename, strlen(filename) + 1);
        filename[0] = '\0';
        return;
    }

    filename[i] = '\0';
    i += 1;

    if (i == 2) {
        filename[0] = '\0';
        i = 0;
    }

    j = i;

    while (j < CVFS_MAX_NAME_LENGTH && full_path[j] != '\0') {
        device_name[j - i] = full_path[j];
        j++;
    }

    device_name[j - i] = '\0';
    toUpperStr(filename);
}

void getDefDev(Char8* arg0) {
    strlen_t len = strlen(D_006BDDA8);

    if (D_006BDDA8[0] == '\0') {
        arg0[0] = '\0';
        return;
    }

    memcpy(arg0, D_006BDDA8, len + 1);
}

void cvFsClose(CVFSHandle* fs_handle) {
    if (fs_handle == NULL) {
        cvFsError("cvFsClose #1:handle error");
        return;
    }

    if (fs_handle->device->Close != NULL) {
        fs_handle->device->Close(fs_handle->fd);
    } else {
        cvFsError("cvFsClose #2:vtbl error");
        return;
    }

    releaseCvFsHn(fs_handle);
}

Sint32 cvFsTell(CVFSHandle* fs_handle) {
    Sint32 offset;

    if (fs_handle == NULL) {
        cvFsError("cvFsTell #1:handle error");
        return 0;
    }

    if (fs_handle->device->Tell != NULL) {
        offset = fs_handle->device->Tell(fs_handle->fd);
    } else {
        offset = 0;
        cvFsError("cvFsTell #2:vtbl error");
    }

    return offset;
}

Sint32 cvFsSeek(CVFSHandle* fs_handle, Sint32 offset, Sint32 whence) {
    Sint32 ret;

    if (fs_handle == NULL) {
        cvFsError("cvFsSeek #1:handle error");
        return 0;
    }

    if (fs_handle->device->Seek != NULL) {
        ret = fs_handle->device->Seek(fs_handle->fd, offset, whence);
    } else {
        ret = 0;
        cvFsError("cvFsSeek #2:vtbl error");
    }

    return ret;
}

Sint32 cvFsReqRd(CVFSHandle* fs_handle, Sint32 len, void* buf) {
    Sint32 ret;

    if (fs_handle == NULL) {
        cvFsError("cvFsReqRd #1:handle error");
        return 0;
    }

    if (fs_handle->device->ReqRd != NULL) {
        ret = fs_handle->device->ReqRd(fs_handle->fd, len, buf);
    } else {
        ret = 0;
        cvFsError("cvFsReqRd #2:vtbl error");
    }

    return ret;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C048);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C068);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsReqWr);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C080);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C0A0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsStopTr);

void cvFsExecServer() {
    CVFSDevice* device;
    Sint32 i;

    for (i = 0; i < CVFS_DEVICE_MAX; i++) {
        device = D_006BDBA8[i].device;

        if (device != NULL && device->ExecServer != NULL) {
            device->ExecServer();
        }
    }
}

Sint32 cvFsGetStat(CVFSHandle* fs_handle) {
    Sint32 stat = 3;

    if (fs_handle == NULL) {
        cvFsError("cvFsGetStat #1:handle error");
        return 3;
    }

    if (fs_handle->device->GetStat != NULL) {
        stat = fs_handle->device->GetStat(fs_handle->fd);
    } else {
        cvFsError("cvFsGetStat #2:vtbl error\0\0\0\0");
    }

    return stat;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C100);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C150);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C178);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetFileSize);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C198);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C1F0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C218);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetFileSizeEx);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C238);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetFileSizeByHndl);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C268);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C290);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetFreeSize);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C2B0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C2D0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetSctLen);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C2F0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C310);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsSetSctLen);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C330);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C350);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetNumTr);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C370);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C3C0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C3E8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsChangeDir);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C408);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C430); // This one is suspicious
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C458);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C480);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsIsExistFile);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetNumFiles);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsLoadDirInfo);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C4A0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C4C8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetMaxByteRate);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C4F0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C540);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C560);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsMakeDir);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C580);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C5D0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C5F8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsRemoveDir);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C618);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C668);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C690);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsDeleteFile);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C6B0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetDevName);

void cvFsEntryErrFunc(void (*function)(void*, const Char8*), void* object) {
    if (function == NULL) {
        cvfs_errfn = NULL;
        cvfs_errobj = NULL;
        return;
    }

    cvfs_errfn = function;
    cvfs_errobj = object;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C6D0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C6F0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsOptFn1);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C710);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C730);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsOptFn2);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C750);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C778);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C7A8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsSetCurVolume);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsAddVolume);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C7D0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C7F8);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C820);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C848);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsAddVolumeEx);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C870);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C898);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C8C0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsDelVolume);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C8E8);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C918);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C948);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetVolumeInfo);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C970);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C998);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C9C0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsSetDefVol);

Sint32 isNeedDevName(Char8* device_name) {
    CVFSDevice* device = getDevice(device_name);
    Sint32 ret;

    if (device == NULL) {
        return 0;
    }

    if (device->OptFn1 != NULL) {
        ret = device->OptFn1(0, 0x64, 0, 0);
    } else {
        return 0;
    }

    return ret;
}

void addDevName(const Char8* device_name, Char8* out) {
    Char8* name_to_add = device_name;

    if (name_to_add == NULL) {
        name_to_add = cvFsGetDefDev();
    }

    if (isNeedDevName(name_to_add) == 1) {
        strcpy(D_006BDDB8, out);
        sprintf(out, "%s:%s", name_to_add, D_006BDDB8);
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsIsExistDevice);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055C9F0);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetNumTr64);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetFileSize64);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetFileSizeEx64);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", D_0055CA10);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/cri_cvfs", cvFsGetFsys64Info);
