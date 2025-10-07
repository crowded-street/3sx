#include "common.h"
#include <cri/private/libadxe/adx_baif.h>

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", AIFF_GetInfo);

Sint32 ADXB_CheckAiff(void*) {
    not_implemented(__func__);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADX_DecodeInfoAiff);

Sint32 ADXB_DecodeHeaderAiff(ADXB, void*, Sint32) {
    not_implemented(__func__);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADXB_ExecOneAiff16);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADXB_ExecOneAiff8);

void ADXB_ExecOneAiff(ADXB adxb) {
    not_implemented(__func__);
}
