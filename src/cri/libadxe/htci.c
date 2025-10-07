#include "common.h"

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiSetFileSystem64);
#else
int htCiSetFileSystem64(int enable) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetFileSystem64);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiSetLockHost);
#else
int htCiSetLockHost(int lock) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetLockHost);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetInterface);
#else
void htCiGetInterface() {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_wait);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_call_errfn);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_is_exec_hn);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_is_exec_hn_one);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_zero_padding);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D400);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiExecHndl);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiExecServer);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiEntryErrFunc);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiIsExistFile);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetFileSize);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetFileSize64);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_get_fsize_opened);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_alloc);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_free);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiOpen);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiClose);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiSeek);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiTell);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiReqRd);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiStopTr);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetStat);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetSctLen);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiGetNumTr);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_get_filesize64);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_getnumtr_upper);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_getnumtr_lower);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_filesize_upper);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htci_filesize_lower);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/htci", htCiOptFn1);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D468);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D498);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D4B8);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D4E0);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D518);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D540);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D578);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D5A8);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D5D8);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D600);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D628);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D658);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D680);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D6A8);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D6C0);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D6D8);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D6F8);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D720);

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/htci", D_0055D738);
