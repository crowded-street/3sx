#include "common.h"
#include <cri/private/libadxe/adx_baif.h>

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", AIFF_GetInfo);

#if defined(TARGET_PS2)
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", D_00559920);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", D_00559928);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADXB_CheckAiff);
#else
Sint32 ADXB_CheckAiff(void*) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADX_DecodeInfoAiff);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADXB_DecodeHeaderAiff);
#else
Sint32 ADXB_DecodeHeaderAiff(ADXB, void*, Sint32) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADXB_ExecOneAiff16);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADXB_ExecOneAiff8);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_baif", ADXB_ExecOneAiff);
#else
void ADXB_ExecOneAiff(ADXB adxb) {
    not_implemented(__func__);
}
#endif
