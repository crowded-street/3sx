#include "common.h"
#include <cri/private/libadxe/adx_dcd.h>
#include <cri/private/libadxe/adx_dcd3.h>

void ADX_SetDecodeSteAsMonoSw(Sint32 flag) {
    adx_decode_output_mono_flag = flag;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd3", ADX_DecodeMonoFloat);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd3", ADX_DecodeSteFloatAsSte);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd3", ADX_DecodeSteFloatAsMono);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/adx_dcd3", ADX_DecodeSteFloat);
