#include "common.h"
#include <cri/private/libadxe/cri_srd.h>
#include <cri/private/libadxe/htci_sub.h>

#include <cri/ee/cri_xpt.h>

#include <sifdev.h>

#include <string.h>

// data
Sint32 htg_flist_tbl[4] = { 0 };
Sint32 htg_ci_open_mode = 0x8001;
Sint32 htg_ci_dbg_out_lv = 0;
Char8 htg_ci_root_dir[257] = { 0 };
Char8 htg_ci_head_name[33] = { 0 };

Sint32 htci_is_inc_colon(const Char8* str) {
    size_t len;
    Sint32 i;

    len = strlen(str);

    if (len > 16) {
        len = 16;
    }

    for (i = 0; i < len; i++) {
        if (str[i] == ':') {
            return 1;
        }
    }

    return 0;
}

Sint32 htci_is_bgn_host(const Char8* str) {
    Char8 copy[6];

    memset(copy, 0, 6);
    memcpy(copy, str, 4);
    copy[5] = '\0';
    return strcmp(copy, "host\0\0\0") == 0;
}

#if defined(TARGET_PS2)
// Also used in htCiSetRootDir
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D760);
extern Char8 D_0055D760[];
asm(".section .text");
#endif

void htci_conv_fname(const Char8* flist, Char8* fname) {
    strlen_t len;
    Sint32 i;
    Char8 root_dir_last_char;

    fname[0] = '\0';

    if (htci_is_inc_colon(flist) == 0) {
        strcpy(fname, htg_ci_head_name);
    }

    if (htci_is_bgn_host(fname) == 1) {
        strcat(fname, htg_ci_root_dir);
        root_dir_last_char = htg_ci_root_dir[strlen(htg_ci_root_dir) - 1];

        if ((root_dir_last_char != '\\') && (root_dir_last_char != '/') && (root_dir_last_char != '\0') &&
            (flist[0] != '\\') && (flist[0] != '/')) {
#if defined(TARGET_PS2)
            strcat(fname, D_0055D760);
#else
            strcat(fname, "/");
#endif
        }
    }

    strcat(fname, flist);
    len = strlen(fname);

    for (i = 0; i < len; i++) {
        if (fname[i] == '\\') {
            fname[i] = '/';
        }
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", conv_cmp_003DE220);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htci_charicmp);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htci_stricmp);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", analysis_flist_003DE328);

Sint32 htci_load_flist(const Char8* flist, Sint8* buf) {
    Char8 fname[256];
    Sint32 fd;
    Sint32 read_err;
    Sint32 offset_start;
    Sint64 offset_end;

    memset(fname, 0, sizeof(fname));
    htci_conv_fname(flist, fname);

    SRD_SetHistory(0x6000);
    fd = SRD_SceOpen(fname, SCE_RDONLY);
    SRD_SetHistory(0x6001);

    if (fd < 0) {
        return 0;
    }

    SRD_SetHistory(0x6100);
    offset_end = SRD_SceLseek(fd, 0, SCE_SEEK_END);
    SRD_SetHistory(0x6101);

    if (offset_end < 0) {
        return 0;
    }

    SRD_SetHistory(0x6200);
    offset_start = (Sint32)SRD_SceLseek(fd, 0, SCE_SEEK_SET);
    SRD_SetHistory(0x6201);

    if (offset_start < 0) {
        return 0;
    }

    if (offset_end > 0x1000) {
        offset_end = 0x1000;
    }

    SRD_SetHistory(0x6300);
    read_err = SRD_SceRead(fd, buf, (Sint32)offset_end);
    SRD_SetHistory(0x6301);

    if (read_err < 0) {
        SRD_SetHistory(0x6500);
        SRD_SceClose(fd);
        SRD_SetHistory(0x6501);

        return 0;
    }

    SRD_SetHistory(0x6400);
    SRD_SceClose(fd);
    SRD_SetHistory(0x6401);

    return 1;
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D768);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D790);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D7B0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D7C8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", open_file_all);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D7F8);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D820);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D840);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", close_file_all);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", get_fstate);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htci_init_flist);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htci_get_finf);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D858);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D888);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D8B8);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D8E0);
#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiLoadFcache);
#else
int htCiLoadFcache() {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiIsExistFcache);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiSetOpenMode);
#else
int htCiSetOpenMode(int mode) {
    not_implemented(__func__);
}
#endif

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiSetRootDir);
#else
int htCiSetRootDir(char* root) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiGetRootDir);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", D_0055D910);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiSetAccessName);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiGetAccessName);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htci_get_opmode);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiSetOutputDebugLevel);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci_sub", htCiGetOutputDebugLevel);
