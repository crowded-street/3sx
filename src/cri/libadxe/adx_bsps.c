#include "common.h"
#include <cri/private/libadxe/adx_bsps.h>

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bsps", ADX_DecodeInfoSpsd);

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bsps", ADXB_DecodeHeaderSpsd);
#else
Sint32 ADXB_DecodeHeaderSpsd(ADXB, void*, Sint32) {
    not_implemented(__func__);
}
#endif

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bsps", ADXB_ExecOneSpsd);
#else
void ADXB_ExecOneSpsd(ADXB adxb) {
    not_implemented(__func__);
}
#endif

#if defined(TARGET_PS2)
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_bsps", D_0055A1C8);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_bsps", ADXB_CheckSpsd);
#else
Sint32 ADXB_CheckSpsd(void*) {
    not_implemented(__func__);
}
#endif
