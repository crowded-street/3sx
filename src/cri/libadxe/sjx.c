#include "common.h"
#include <cri/private/libadxe/dtx.h>
#include <cri/private/libadxe/structs.h>

#include <cri/cri_xpts.h>
#include <cri/sj.h>

#include <eekernel.h>
#include <sifdev.h>

#include <string.h>

Char8* sjx_build = "\nSJX Ver 1.05 Build:Sep 18 2003 09:59:53\n";
Sint32 sjx_init_cnt = 0;
void* sjx_eewk = NULL;
void* sjx_iopwk0 = NULL;
void* sjx_iopwk = NULL;
SJX_OBJ sjx_obj[SJX_MAX_OBJ] = { 0 };
DTX sjx_dtx = NULL;
Sint32 sjx_wklen = 0;
Sint8 sjx_ee_work[0x8D0] = { 0 };

void sjx_rcvcbf(void* obj, void* arg1, Sint32 arg2) {
    SJX_UNK_2* head = arg1;
    SJX_UNK_1* cur;
    Sint32 count;
    Sint32 i;

    if (head == NULL) {
        while (1) {}
    }

    count = head->count;
    SJCRS_Lock();

    for (i = 0; i < count; i++) {
        cur = &head->items[i];

        if (cur->unk0 == 0) {
            SJ_PutChunk(cur->unk4->sj, cur->id, &cur->chunk);
        }
    }

    SJCRS_Unlock();
}

void sjx_sndcbf(void* object, void* arg1, Sint32 arg2) {
    SJCK chunk;
    SJX sjx;
    Sint32 j;
    Sint32 i;
    SJX_UNK_2* head = arg1;
    SJX_UNK_1* items = head->items;
    SJX_UNK_1* item;

    j = 0;

    SJCRS_Lock();

    for (i = 0; i < SJX_MAX_OBJ; i++) {
        sjx = &sjx_obj[i];

        if (sjx->used != 1) {
            continue;
        }

        while (1) {
            if (j == 0x80) {
                goto end;
            }

            SJ_GetChunk(sjx->sj, sjx->unkC, SJCK_LEN_MAX, &chunk);

            if (chunk.len == 0) {
                break;
            }

            items[j].unk0 = 0;
            items[j].unk4 = sjx->unk10;
            items[j].id = sjx->unkC;
            items[j].chunk = chunk;
            items[j].unk2 = sjx->unk2;
            j++;
        }
    }

end:
    SJCRS_Unlock();
    head->count = j;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjx", sjx_rcvcbf_iop);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjx", sjx_sndcbf_iop);

void SJX_ExecServer() {
    DTX_ExecServer();
}

void SJX_Init() {
    if (sjx_init_cnt == 0) {
        memset(sjx_ee_work, 0, sizeof(sjx_ee_work));
        DTX_Init();
        sjx_wklen = 0x880;
        sjx_eewk = (void*)(((uintptr_t)sjx_ee_work + 0x40) & ~0x3F);

        if (sjx_iopwk0 == NULL) {
            sjx_iopwk0 = sceSifAllocIopHeap(0x8D0);

            if (sjx_iopwk0 == NULL) {
                scePrintf("E0100301: SJX_Init can't allocate IOP Heap\n");
                while (1) {}
            }
        }

        sjx_iopwk = (void*)(((uintptr_t)sjx_iopwk0 + 0x40) & ~0x3F);
        sjx_dtx = DTX_Create(0, sjx_eewk, sjx_iopwk, sjx_wklen);

        if (sjx_dtx == NULL) {
            scePrintf("E0100302: SJX_Init can't create DTX\n");
            while (1) {}
        }

        DTX_SetRcvCbf(sjx_dtx, sjx_rcvcbf, NULL);
        DTX_SetSndCbf(sjx_dtx, sjx_sndcbf, NULL);
        DTX_CallUrpc(3, NULL, 0, NULL, 0);
    }

    sjx_init_cnt += 1;
}

void SJX_Finish() {
    if (--sjx_init_cnt == 0) {
        DTX_Destroy(sjx_dtx);
        sjx_dtx = NULL;

        if (sjx_iopwk0 != NULL) {
            sceSifFreeIopHeap(sjx_iopwk0);
            sjx_iopwk0 = NULL;
        }

        DTX_Finish();
    }
}

SJX SJX_Create(SJ sj, void* arg1, Sint32 arg2) {
    uintptr_t rpc_buf[4];
    SJX sjx;
    Sint32 i;

    for (i = 0; i < SJX_MAX_OBJ; i++) {
        sjx = &sjx_obj[i];

        if (sjx->used == 0) {
            break;
        }
    }

    if (i == SJX_MAX_OBJ) {
        return NULL;
    }

    memset(sjx, 0, sizeof(SJX_OBJ));
    sjx->sj = sj;
    sjx->unkC = arg2;
    sjx->unk8 = arg1;
    rpc_buf[0] = (uintptr_t)sj;
    rpc_buf[1] = arg1;
    rpc_buf[2] = arg2;
    rpc_buf[3] = (uintptr_t)sjx;
    sjx->unk10 = DTX_CallUrpc(0, rpc_buf, 4, rpc_buf, 1);

    if (sjx->unk10 == NULL) {
        scePrintf("E0100401: can't create SJX of IOP\n");
        return NULL;
    }

    sjx->used = 1;
    return sjx;
}

void SJX_Destroy(SJX sjx) {
    Sint32 rpc_buf[1];

    rpc_buf[0] = sjx->unk10;
    DTX_CallUrpc(1, rpc_buf, 1, NULL, 0);
    sjx->used = 0;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sjx", SJX_Reset);
