#include "common.h"
#include <cri/private/libadxe/dtr.h>
#include <cri/private/libadxe/dtx.h>
#include <cri/private/libadxe/structs.h>

#include <cri/cri_xpts.h>
#include <cri/sj.h>
#include <eekernel.h>
#include <sif.h>

#include <string.h>

#define DTR_MAX_OBJ 32

#define MIN(a, b) ((a) < (b) ? (a) : (b))

// data
Char8* dtr_build = "\nDTR Ver.1.03 Build:Sep 18 2003 10:00:10\n\0\0\0\0";
Sint32 dtr_init_cnt = 0;
DTR_OBJ dtr_obj[DTR_MAX_OBJ] = { 0 };

void DTR_ExecHndl(DTR dtr) {
    SJCK cks;
    SJCK sp10;
    SJCK ckd;
    SJCK sp30;
    Sint32 nbyte;
    Uint8 var_v1;

    if (dtr->unk1 != 1) {
        return;
    }

    var_v1 = dtr->unk2;

    if (dtr->unk2 == dtr->unk1) {
        if (sceSifDmaStat(dtr->unk2C) < 0) {
            SJ_PutChunk(dtr->unk4, 0, &dtr->unkC);
            dtr->unkC.data = NULL;
            dtr->unkC.len = 0;
            SJ_PutChunk(dtr->unk8, 1, &dtr->unk14);
            dtr->unk14.data = NULL;
            dtr->unk14.len = 0;
            dtr->unk2 = 0;
            dtr->unk3C += dtr->unkC.len;

            if (dtr->unkC.len != 0) {
                scePrintf("DTR_ExecHndl: Internal Error\n");

                while (1) {
                    // Do nothing
                }
            }

            var_v1 = 0;
        } else {
            var_v1 = dtr->unk2;
        }
    }

    if (var_v1 != 0) {
        return;
    }

    SJ_GetChunk(dtr->unk4, 1, SJCK_LEN_MAX, &cks);
    SJ_GetChunk(dtr->unk8, 0, SJCK_LEN_MAX, &ckd);
    nbyte = MIN(cks.len, ckd.len);
    nbyte = nbyte / dtr->unk38 * dtr->unk38;
    SJ_SplitChunk(&cks, nbyte, &cks, &sp10);
    SJ_UngetChunk(dtr->unk4, 1, &sp10);
    SJ_SplitChunk(&ckd, nbyte, &ckd, &sp30);
    SJ_UngetChunk(dtr->unk8, 0, &sp30);

    if (nbyte <= 0) {
        return;
    }

    if (cks.len & 0x3F) {
        scePrintf("DTR_ExecHndl: Internal Error len\n");
        scePrintf("cks.data=%08x, ckd.data=%08x, cks.len=%d\n", cks.data, ckd.data, cks.len);

        while (1) {
            // Do nothing
        }
    }

    if ((uintptr_t)cks.data & 0x3F) {
        scePrintf("DTR_ExecHndl: Internal Error cks.data\n");
        scePrintf("cks.data=%08x, ckd.data=%08x, cks.len=%d\n", cks.data, ckd.data, cks.len);

        while (1) {
            // Do nothing
        }
    }

    if ((uintptr_t)ckd.data & 0x3F) {
        scePrintf("DTR_ExecHndl: Internal Error ckd.data\n");
        scePrintf("cks.data=%08x, ckd.data=%08x, cks.len=%d\n", cks.data, ckd.data, cks.len);

        while (1) {
            // Do nothing
        }
    }

    SyncDCache(cks.data, cks.data + cks.len - 1);
    dtr->dma_data.data = (u32)((uintptr_t)cks.data & 0x0FFFFFFF);
    dtr->dma_data.addr = (u32)(uintptr_t)ckd.data;
    dtr->dma_data.size = cks.len;
    dtr->dma_data.mode = 0;
    dtr->unk2C = sceSifSetDma(&dtr->dma_data, 1);

    if (dtr->unk2C == 0) {
        scePrintf("E0101701 DTR_ExecHndl: can't use DMA\n");
        SJ_UngetChunk(dtr->unk4, 1, &cks);
        SJ_UngetChunk(dtr->unk8, 0, &ckd);
    } else {
        dtr->unkC = cks;
        dtr->unk14 = ckd;
        dtr->unk2 = 1;
    }
}

Sint32 DTR_GetStat(DTR dtr) {
    return dtr->unk1;
}

void DTR_ExecServer() {
    Sint32 i;

    for (i = 0; i < DTR_MAX_OBJ; i++) {
        if (dtr_obj[i].unk0 == 1) {
            DTR_ExecHndl(&dtr_obj[i]);
        }
    }

    DTX_ExecServer();
}

INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/dtr", D_0055CEF0);
INCLUDE_RODATA("asm/anniversary/nonmatchings/cri/libadxe/dtr", D_0055CF08);
INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dtr", DTR_Init);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dtr", DTR_Finish);

DTR DTR_Create(SJ sj, SJ arg1) {
    DTR dtr;
    Sint32 i;

    SJCRS_Lock();

    for (i = 0; i < DTR_MAX_OBJ; i++) {
        dtr = &dtr_obj[i];

        if (dtr->unk0 == 0) {
            break;
        }
    }

    if (i == DTR_MAX_OBJ) {
        scePrintf("i:%d DTR_MAX_OBJ:%d\n", i, DTR_MAX_OBJ);
        dtr = NULL;
    } else {
        memset(dtr, 0, sizeof(DTR_OBJ));
        dtr->unk1 = 0;
        dtr->unk38 = 0x40;
        dtr->unk4 = sj;
        dtr->unk8 = arg1;
        dtr->unk2 = 0;
        dtr->unk30 = 1;
        dtr->unk0 = 1;
        dtr->unk34 = 0;
    }

    SJCRS_Unlock();
    return dtr;
}

void DTR_Destroy(DTR dtr) {
    SJCRS_Lock();
    dtr->unk0 = 0;
    SJCRS_Unlock();
}

void DTR_Start(DTR dtr) {
    dtr->unk3C = 0;
    dtr->unk1 = 1;
}

void DTR_Stop(DTR dtr) {
    dtr->unk1 = 0;

    if (dtr->unk2 != 1) {
        return;
    }

    while (sceSifDmaStat(dtr->unk2C) >= 0) {
        // Do nothing
    }

    SJ_PutChunk(dtr->unk4, 0, &dtr->unkC);
    dtr->unkC.data = NULL;
    dtr->unkC.len = 0;
    SJ_PutChunk(dtr->unk8, 1, &dtr->unk14);
    dtr->unk14.data = NULL;
    dtr->unk2 = 0;
    dtr->unk14.len = 0;
    dtr->unk3C += dtr->unkC.len;
}
