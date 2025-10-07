#include "common.h"
#include <cri/private/libadxe/rna_crs.h>

#include <cri/ee/cri_xpt.h>

// data
Sint32 rna_crs_lvl = 0;
Sint32 rna_crs_msk = 0;

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/rna_crs", RNACRS_Init);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/rna_crs", RNACRS_Finish);

void RNACRS_Lock() {
    rna_crs_lvl += 1;
}

void RNACRS_Unlock() {
    rna_crs_lvl -= 1;
}
