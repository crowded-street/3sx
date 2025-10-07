#include "common.h"
#include <cri/private/libadxe/cri_srd.h>
#include <cri/private/libadxe/dvci.h>
#include <cri/private/libadxe/dvci_sub.h>
#include <cri/private/libadxe/htci_sub.h>
#include <cri/private/libadxe/structs.h>

#include <eekernel.h>
#include <libcdvd.h>

#include <string.h>

#if !defined(TARGET_PS2)
#include <strings.h>
#endif

#define LAST_CHAR(str) ((str)[strlen(str) - 1])

// data
DVG_FLIST_TBL dvg_flist_tbl = { 0 };
sceCdRMode dvg_ci_cdrmode = { 0 };
Sint32 dvg_ci_rdmode = 0;
Sint32 dvg_ci_dbg_out_lv = 0;
Char8 dvg_ci_root_dir[257] = { 0 };
Char8 dvg_fpath[128] = { 0 };

// bss
Sint8 D_006BE100[0x1000];

#if defined(TARGET_PS2)
// Used in dvci_conv_fname and dvCiSetRootDir
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", D_0055D280);
extern Char8 D_0055D280[];
#endif

void dvci_conv_fname(const Char8* fname, Char8* path) {
    Char8 first_char;

    strcpy(path, dvg_ci_root_dir);
    first_char = fname[0];

    if ((first_char != '/') && (first_char != '\\')) {
#if defined(TARGET_PS2)
        strcat(path, D_0055D280);
#else
        strcat(path, "\\");
#endif
    }

    strcat(path, fname);

    if (strcmp(path + strlen(path) - 2, ";1\0\0\0\0") != 0) {
        strcat(path, ";1\0\0\0\0");
    }

    dvci_to_large_to_yen(path);
}

// TODO: rename to conv_cmp
Char8 conv_cmp_003DC578(Char8 chr) {
    Char8 ret = chr;

    if (ret >= 'a' && ret <= 'z') {
        ret -= 'a' - 'A';
    }

    if (chr == '\\') {
        return '/';
    } else {
        return ret;
    }
}

Sint32 dvci_charicmp(Char8 a, Char8 b) {
    if (conv_cmp_003DC578(a) == conv_cmp_003DC578(b)) {
        return 0;
    } else {
        return -1;
    }
}

Sint32 dvci_stricmp(const Char8* a, const Char8* b) {
    strlen_t len_a = strlen(a);
    Sint32 i;

    if (len_a != strlen(b)) {
        return -1;
    }

    for (i = 0; i < len_a; i++) {
        if (dvci_charicmp(a[i], b[i]) != 0) {
            return -1;
        }
    }

    return 0;
}

#if defined(TARGET_PS2)
Sint32 analysis_flist_003DC6A0(Sint8*, Sint8*, Sint32, Sint32, Sint32);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", analysis_flist_003DC6A0);
#else
// This is a kludge, not a decompilation
Sint32 analysis_flist_003DC6A0(Sint8* fcbuf, Sint8* filelist_buf, Sint32 arg2, Sint32 arg3, Sint32 arg4) {
    strcpy(fcbuf + 0x140, "SF33RD.AFS");
    dvg_flist_tbl.unk0 = (uintptr_t)fcbuf;
    dvg_flist_tbl.unk4 = 1;
    dvg_flist_tbl.unk8 = arg3;
    dvg_flist_tbl.unkC = arg4;
    return 1;
}
#endif

Sint32 load_flist(Char8* flist, Sint8* buf) {
    sceCdlFILE fp;
    sceCdRMode cd_rmode;
    Char8 flist_prefix[5];

    memcpy(flist_prefix, flist, 5);
    flist_prefix[4] = '\0';

    if (strcasecmp(&flist_prefix, "HST:") == 0) {
        return htci_load_flist(flist + 4, buf);
    }

    memset(dvg_fpath, 0, sizeof(dvg_fpath));
    dvci_conv_fname(flist, dvg_fpath);

    SRD_SetHistory(0x9000);

    while (sceCdSync(1) == 1) {
        // Do nothing
    }

    SRD_SetHistory(0x9001);

    SRD_SetHistory(0x9100);

    if (dvCiCdSearchFile(&fp, &dvg_fpath) == 0) {
        SRD_SetHistory(0x9101);
        return 0;
    }

    SRD_SetHistory(0x9102);

    cd_rmode.spindlctrl = 1;
    cd_rmode.trycount = 0;
    cd_rmode.datapattern = 0;
    cd_rmode.pad = 0;

    SRD_SetHistory(0x9200);

    if (sceCdRead(fp.lsn, 2, buf, &cd_rmode) == 0) {
        SRD_SetHistory(0x9201);
        return 0;
    }

    SRD_SetHistory(0x9202);

    SRD_SetHistory(0x9300);

    while (sceCdSync(1) == 1) {
        // Do nothing
    }

    SRD_SetHistory(0x9301);

    return 1;
}

Sint32 search_fstate(Sint8* arg0, Sint32 arg1) {
    sceCdlFILE fp;
    Sint32 numf = 0;
    Sint32 numf_not_found = 0;
    Sint32 i;
    Char8* fname;
    DVG_FLIST_SUB* flist_sub = arg0;

    for (i = 0; i < arg1; i++) {
        fname = arg0 + ((dvg_flist_tbl.unk8 * 8) + ((dvg_flist_tbl.unkC + 1) * i));

        if (fname[0] == '\0') {
            continue;
        }

        dvci_conv_fname(fname, dvg_fpath);

        SRD_SetHistory(0x9400);

        while (sceCdSync(1) == 1) {
            // Do nothing
        }

        SRD_SetHistory(0x9401);

        SRD_SetHistory(0x9500);

        if (dvCiCdSearchFile(&fp, dvg_fpath) == 1) {
            numf += 1;
            SRD_SetHistory(0x9501);
            flist_sub[i].lsn = fp.lsn;
            flist_sub[i].size = fp.size;

            if (dvg_ci_dbg_out_lv == 0) {
                scePrintf("DVCI: \"%s\" found.\n", fname);
            }
        } else {
            SRD_SetHistory(0x9502);

            if (dvg_ci_dbg_out_lv == 0) {
                scePrintf("DVCI: \"%s\" Not found.\n", fname);
            }

            numf_not_found += 1;
        }
    }

    if (dvg_ci_dbg_out_lv == 0) {
        scePrintf("DVCI: Total %d files.\n", numf);
    }

    if (numf_not_found > 0 && dvg_ci_dbg_out_lv != 2) {
        scePrintf("DVCI: Warning, Can't load all cache file.%c", '\n');
    }

    return numf;
}

void get_fp_from_fname(sceCdlFILE* fp, const Char8* fname, uintptr_t arg2, Sint32 arg3) {
    DVG_FLIST_SUB* flist_sub = (DVG_FLIST_SUB*)arg2;
    Sint32 i;

    for (i = 0; i < arg3; i++) {
        if (dvci_stricmp(fname, (Char8*)arg2 + ((dvg_flist_tbl.unk8 * 8) + ((dvg_flist_tbl.unkC + 1) * i))) == 0) {
            fp->lsn = flist_sub[i].lsn;
            fp->size = flist_sub[i].size;
            return;
        }
    }

    fp->lsn = 0;
    fp->size = 0;
}

void dvci_init_flist() {
    memset(&dvg_flist_tbl, 0, sizeof(DVG_FLIST_TBL));
}

Sint32 dvci_get_fstate(const Char8* fname, sceCdlFILE* fp) {
    if (strcmp("DVD-ROM", fname) == 0) {
        fp->lsn = 0;
        fp->size = -1;

        if (dvg_ci_dbg_out_lv != 2) {
            scePrintf("DVCI: CD/DVD-ROM Image opened%c\n\0\0\0\0", '.');
        }

        return 1;
    }

    fp->lsn = 0;
    fp->size = 0;

    if (dvg_flist_tbl.unk0 != 0) {
        get_fp_from_fname(fp, fname, dvg_flist_tbl.unk0, dvg_flist_tbl.unk4);
    }

    return 0;
}

#if defined(TARGET_PS2)
// Also used in dvCiSetFcache
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", D_0055D340);
extern Char8 D_0055D340[];
// Also used in dvCiSetFcache
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", D_0055D370);
extern Char8 D_0055D370[];
#endif

Sint32 dvCiLoadFcache(Char8* flist, Sint8* fcbuf, Sint32 fcsize, Sint32 maxflen) {
    Uint32 temp_lo;
    Sint32 temp;

    memset(D_006BE100, 0, sizeof(D_006BE100));

    if (dvg_flist_tbl.unk0 != 0) {
        dvci_init_flist();
    }

    if ((flist == NULL) || (fcbuf == NULL) || (fcsize == 0)) {
        dvci_init_flist();
    }

    if (flist == NULL) {
        return 0;
    }

    if (fcbuf == NULL) {
        if (dvg_ci_dbg_out_lv == 0) {
#if defined(TARGET_PS2)
            scePrintf(D_0055D340, fcbuf);
#else
            scePrintf("DVCI: Invalidate filelist buffer pointer.\r\n", fcbuf);
#endif
        }

        return 0;
    }

    if (fcsize == 0) {
        if (dvg_ci_dbg_out_lv == 0) {
#if defined(TARGET_PS2)
            scePrintf(D_0055D370, fcsize);
#else
            scePrintf("DVCI: Invalidate filelist buffer size.\r\n", fcsize);
#endif
        }

        return 0;
    }

    memset(fcbuf, 0, fcsize);

    if (load_flist(flist, D_006BE100) == 0) {
        if (dvg_ci_dbg_out_lv == 0) {
            scePrintf("DVCI: Can't read filelist file.(%s)\n", flist);
        }

        dvci_call_errfn(NULL, "E0111501:can't read filelist.(dvCiLoadDirInfo)\0\0\0\0\0\0\0\0");
        return 0;
    }

    temp_lo = (Uint32)fcsize / (maxflen + 9);
    temp = analysis_flist_003DC6A0(fcbuf, D_006BE100, 0x1000, temp_lo, maxflen);
    return (((Uint32)fcsize / (maxflen + 9)) * 8) + search_fstate(fcbuf, temp) * (maxflen + 1);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", dvCiSetFcache);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", dvCiIsExistFcache);

void dvCiSetRdMode(Sint32 trycount, Sint32 spindlctrl, Sint32 datapattern, Sint32 rdmode) {
    dvg_ci_cdrmode.trycount = trycount;
    dvg_ci_cdrmode.spindlctrl = spindlctrl;
    dvg_ci_cdrmode.datapattern = datapattern;
    dvg_ci_rdmode = rdmode;
}

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", dvCiSetRootDir);
#else
// Matches on decomp.me but breaks here
void dvCiSetRootDir(const Char8* dir) {
    Char8 last_char;

    if (dir == NULL) {
        dvg_ci_root_dir[0] = '\0';
    } else {
        if ((dir[0] != '/') && (dir[0] != '\\')) {
            memcpy(dvg_ci_root_dir, "\\", 2);
        } else {
            dvg_ci_root_dir[0] = '\0';
        }

        strcat(dvg_ci_root_dir, dir);
        last_char = LAST_CHAR(dir);

        if ((last_char == '/') || (last_char == '\\')) {
            LAST_CHAR(dvg_ci_root_dir) = '\0';
        }
    }
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", dvCiGetRootDir);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dvci_sub", dvCiSetSeekPos);
