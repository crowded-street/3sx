#include "common.h"
#include <cri/private/libadxe/adx_bau.h>

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", AU_GetInfo);

Sint32 ADXB_CheckAu(void*) {
    not_implemented(__func__);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADX_DecodeInfoAu);

Sint32 ADXB_DecodeHeaderAu(ADXB, void*, Sint32) {
    not_implemented(__func__);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_ExecOneAu16);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_ExecOneAu8);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_ExecOneAuUlaw);

void ADXB_ExecOneAu(ADXB adxb) {
    not_implemented(__func__);
}
