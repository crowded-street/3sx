#include "common.h"
#include <cri/cri_xpts.h>
#include <cri/private/libadxe/sj_utl.h>

void SJ_SplitChunk(SJCK* ck, Sint32 nbyte, SJCK* ck1, SJCK* ck2) {
    *ck1 = *ck;
    ck2->len = ck1->len;

    if (nbyte < ck1->len) {
        ck1->len = nbyte;
    }

    ck2->len -= ck1->len;

    if (ck2->len == 0) {
        ck2->data = NULL;
    } else {
        ck2->data = ck1->data + ck1->len;
    }
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_utl", sj_val_to_hexstr);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_utl", sj_hexstr_to_val);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_utl", SJ_MakeTag);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_utl", SJ_GetTagContent);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_utl", SJ_SearchTag);
