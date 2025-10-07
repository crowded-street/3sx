#include "common.h"
#include <cri/private/libadxe/dtx.h>

#include <cri/cri_xpts.h>

Char8* volatile sjrmt_build = "\nSJRMT Ver 1.01 Build:Sep 18 2003 09:59:53\n";
Sint32 sjrmt_init_cnt = 0;

// bss
#if defined(TARGET_PS2)
extern intptr_t D_006C0240[64];
extern intptr_t D_006C0140[52];
#else
intptr_t D_006C0240[64]; // sjr_rpc_rcv_buf
intptr_t D_006C0140[52]; // sjr_rpc_snd_buf
#endif

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRBF_CreateRmt);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJMEM_CreateRmt);

void* SJUNI_CreateRmt(Sint32 arg0, Sint8* arg1, Sint32 arg2) {
    D_006C0140[0] = arg0;
    D_006C0140[1] = (intptr_t)arg1;
    D_006C0140[2] = arg2;
    DTX_CallUrpc(34, D_006C0140, 3, D_006C0240, 1);
    return (void*)D_006C0240[0];
}

void SJRMT_Destroy(void* sjrmt) {
    D_006C0140[0] = sjrmt;
    DTX_CallUrpc(35, D_006C0140, 1, NULL, 0);
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRMT_GetUuid);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRMT_Reset);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRMT_GetChunk);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRMT_UngetChunk);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRMT_PutChunk);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRMT_GetNumData);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjr_clt", SJRMT_IsGetChunk);

void SJRMT_Init() {
    sjrmt_build;

    if (sjrmt_init_cnt == 0) {
        DTX_Init();
    }

    sjrmt_init_cnt += 1;
}

void SJRMT_Finish() {
    if (--sjrmt_init_cnt == 0) {
        DTX_Finish();
    }
}
