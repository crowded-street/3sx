#include "common.h"
#include <cri/private/libadxe/adx_bau.h>

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", AU_GetInfo);

#if defined(TARGET_PS2)
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", D_00559930);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", D_00559938);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_CheckAu);
#else
Sint32 ADXB_CheckAu(void*) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADX_DecodeInfoAu);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_DecodeHeaderAu);
#else
Sint32 ADXB_DecodeHeaderAu(ADXB, void*, Sint32) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_ExecOneAu16);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_ExecOneAu8);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_ExecOneAuUlaw);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bau", ADXB_ExecOneAu);
#else
void ADXB_ExecOneAu(ADXB adxb) {
    not_implemented(__func__);
}
#endif
