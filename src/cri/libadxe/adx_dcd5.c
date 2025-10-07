#include "common.h"
#include <cri/private/libadxe/structs.h>

#include <cri/private/libadxe/adx_dcd.h>
#include <cri/private/libadxe/adx_dcd3.h>
#include <cri/private/libadxe/cri_cvfs.h>

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd5", ADX_DecodeMono4);
#else
Sint32 ADX_DecodeMono4(Sint32, Sint32, Sint32, Sint16*, Sint16, Sint16, Sint16*, Sint16, Sint16) {
    not_implemented(__func__);
}
#endif

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd5", ADX_DecodeSte4AsMono);
#else
Sint32 ADX_DecodeSte4AsMono(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int arg4, int arg5, short arg6,
                            short arg7, int arg8, short arg9, short arg10) {
    not_implemented(__func__);
}
#endif

#if defined(TARGET_PS2)
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd5", ADX_DecodeSte4AsSte);
#else
Sint32 ADX_DecodeSte4AsSte(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int arg4, int arg5, short arg6,
                           short arg7, int arg8, short arg9, short arg10) {
    not_implemented(__func__);
}
#endif

void ADX_DecodeSte4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int arg4, int arg5, short arg6, short arg7,
                    int arg8, short arg9, short arg10) {
    if (adx_decode_output_mono_flag == 0) {
        ADX_DecodeSte4AsSte(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    } else {
        ADX_DecodeSte4AsMono(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    }
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd5", AdxQtbl);
