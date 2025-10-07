#include "common.h"
#include <cri/private/libadxe/adx_bwav.h>

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", D_0055A1D0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", D_0055A1D8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", ADX_DecodeInfoWav);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", ADXB_DecodeHeaderWav);
#else
Sint32 ADXB_DecodeHeaderWav(ADXB, void*, Sint32) {
    not_implemented(__func__);
}
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", ADXB_ExecOneWav16);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", ADXB_ExecOneWav8);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", ADXB_ExecOneWav4);

#if defined(TARGET_PS2)
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", D_0055A1E0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", D_0055A1E8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", ADXB_CheckWav);
#else
Sint32 ADXB_CheckWav(void*) {
    not_implemented(__func__);
}
#endif

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bwav", ADXB_ExecOneWav);
#else
void ADXB_ExecOneWav(ADXB adxb) {
    not_implemented(__func__);
}
#endif
