#include "common.h"
#include <cri/private/libadxe/dtx.h>
#include <cri/private/libadxe/structs.h>
#include <cri/private/libadxe/svm.h>

#include <eekernel.h>
#include <sif.h>
#include <sifrpc.h>

#include <stdio.h>
#include <string.h>

#define DTX_MAX_OBJ 8

// data
Char8* dtx_build = "\nDTX Ver.1.04 Build:Sep 18 2003 10:00:10\n";
Sint32 dtx_rpc_id = 0x90000200;
Sint32 dtx_proc_init_flag = 0;
Sint32 dtx_send_sw = 1;

// bss
intptr_t dtx_rpc_rcv_buf[64];
intptr_t dtx_rpc_snd_buf[64];

// forward declarations
void DTX_ExecServer();
extern DTX_OBJ dtx_clnt[DTX_MAX_OBJ];
extern sceSifClientData dtx_cd;
extern Sint32 dtx_init_cnt;

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dtx", dtx_rpc_func);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dtx", dtx_svr_proc);

void* dtx_create_rmt(Sint32 id, void* eewk, void* iopwk, Sint32 wklen) {
    dtx_rpc_snd_buf[0] = id;
    dtx_rpc_snd_buf[1] = eewk;
    dtx_rpc_snd_buf[2] = iopwk;
    dtx_rpc_snd_buf[3] = wklen;
    sceSifCallRpc(&dtx_cd, 2, 0, dtx_rpc_snd_buf, 4 * sizeof(intptr_t), dtx_rpc_rcv_buf, 1 * sizeof(intptr_t), 0, 0);
    return (void*)dtx_rpc_rcv_buf[0];
}

void dtx_destroy_rmt(void* arg0) {
    dtx_rpc_snd_buf[0] = arg0;
    sceSifCallRpc(&dtx_cd, 3, 0, dtx_rpc_snd_buf, 1 * sizeof(intptr_t), dtx_rpc_rcv_buf, 0, 0, 0);
}

void dtx_def_rcvcbf(void* arg0, void* arg1, Sint32 arg2) {
    static Sint32 cnt = 0;
    cnt += 1;
}

void dtx_def_sndcbf(void* arg0, void* arg1, Sint32 arg2) {
    static Sint32 cnt = 0;
    Sint32 i;

    for (i = 0; i < arg2; i++) {
        ((Sint8*)arg1)[i] = -0x56;
    }

    sprintf(arg1, "Hello from EE (%d)", cnt);
    cnt += 1;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dtx", DTX_Open);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/dtx", DTX_Close);

DTX DTX_Create(Sint32 id, void* eewk, void* iopwk, Sint32 wklen) {
    DTX dtx;

    if ((uintptr_t)eewk & 0x3F) {
        scePrintf("eewk is not alignment 64 byte\n");
        return NULL;
    }

    if ((uintptr_t)iopwk & 0x1F) {
        scePrintf("iopwk is not alignment 32 byte\n");
        return NULL;
    }

    if (wklen & 0x3F) {
        scePrintf("wklen=%d is not 64*N\n", wklen);
        return NULL;
    }

    if ((uintptr_t)id >= DTX_MAX_OBJ) {
        scePrintf("illeagal ID (%d) \n", id);
        return NULL;
    }

    dtx = &dtx_clnt[id];

    if (dtx->unk0 != 0) {
        return NULL;
    }

    dtx->unk4 = dtx_create_rmt(id, eewk, iopwk, wklen);

    if (dtx->unk4 == NULL) {
        scePrintf("DTX_Create: can't create DTX of server\n");
        return NULL;
    }

    dtx->unk10 = wklen - 0x40;
    dtx->unk1C = wklen;
    dtx->unk18 = iopwk;
    dtx->unk14 = ((uintptr_t)eewk + dtx->unk10) | 0x20000000;
    dtx->unk8 = 0;
    dtx->unkC = eewk;
    dtx->unk1 = 0;
    memset(eewk, 0, dtx->unk10);
    SyncDCache(dtx->unkC, dtx->unkC + dtx->unk10 + 0x3F);
    InvalidDCache(dtx->unkC, dtx->unkC + dtx->unk10 + 0x3F);
    dtx->rcvcbf = dtx_def_rcvcbf;
    dtx->sndcbf = dtx_def_sndcbf;
    dtx->rcvcbf_obj = NULL;
    dtx->sndcbf_obj = NULL;
    dtx->unk2 = 1;
    dtx->unk0 = 1;

    return dtx;
}

void DTX_Destroy(DTX dtx) {
    dtx->unk2 = 0;

    while (dtx->unk1 == 1) {
        DTX_ExecServer();
    }

    dtx_destroy_rmt(dtx->unk4);
    memset(dtx, 0, sizeof(DTX_OBJ));
}

void DTX_SetRcvCbf(DTX dtx, RcvCbf func, void* object) {
    dtx->rcvcbf = func;
    dtx->rcvcbf_obj = object;
}

void DTX_SetSndCbf(DTX dtx, SndCbf func, void* object) {
    dtx->sndcbf = func;
    dtx->sndcbf_obj = object;
}

void DTX_ExecHndl(DTX dtx) {
    Sint32 static cnt = 0;
    cnt += 1;

    if ((dtx->unk1 == 1) && (dtx->unk8 < dtx->unk14[0xF])) {
        InvalidDCache(dtx->unkC, dtx->unkC + dtx->unk10 - 1);
        dtx->rcvcbf(dtx->rcvcbf_obj, dtx->unkC, dtx->unk10);
        dtx->unk8 = dtx->unk14[0xF];
        dtx->unk1 = 0;
    }

    if (dtx->unk1 == 0 && dtx->unk2 == 1 && dtx_send_sw == 1) {
        dtx->sndcbf(dtx->sndcbf_obj, dtx->unkC, dtx->unk10);
        dtx->unk8 += 1;
        dtx->unk14[0xF] = dtx->unk8;
        SyncDCache(dtx->unkC, dtx->unkC + dtx->unk10 - 1);
        InvalidDCache(dtx->unkC, dtx->unkC + dtx->unk10 + 0x3F);
        dtx->dma_data.data = (uintptr_t)dtx->unkC & 0x0FFFFFFF;
        dtx->dma_data.addr = dtx->unk18;
        dtx->dma_data.size = dtx->unk1C;
        dtx->dma_data.mode = 0;
        dtx->unk40 = sceSifSetDma(&dtx->dma_data, 1);
        dtx->unk1 = 1;
    }
}

void DTX_Init() {
    Sint32 x;
    Sint32 y;

    if (dtx_init_cnt == 0) {
        while (1) {
            if (sceSifBindRpc(&dtx_cd, dtx_rpc_id, 0) < 0) {
                scePrintf("E0092101: DTX_Init bind errr\n");
                while (1) {}
            }

            if (dtx_cd.serve != NULL) {
                break;
            }

            y = -1;
            x = 0xFFFF;

            while (x != y) {
                x -= 4;
            }
        }
    }

    dtx_init_cnt += 1;
}

void DTX_Finish() {
    DTX dtx;
    s32 i;

    dtx_init_cnt -= 1;

    if (dtx_init_cnt != 0) {
        return;
    }

    for (i = 0; i < DTX_MAX_OBJ; i++) {
        dtx = &dtx_clnt[i];

        if (dtx->unk0 == 1) {
            DTX_Destroy(dtx);
        }
    }
}

intptr_t DTX_CallUrpc(Uint32 call_id, intptr_t* snd_buf, Sint32 snd_buf_len, intptr_t* rcv_buf, Sint32 rcv_buf_len) {
    Sint32 i;

    SVM_Lock();

    for (i = 0; i < snd_buf_len; i++) {
        dtx_rpc_snd_buf[i] = snd_buf[i];
    }

    sceSifCallRpc(&dtx_cd,
                  call_id + 0x400,
                  0,
                  dtx_rpc_snd_buf,
                  snd_buf_len * sizeof(intptr_t),
                  dtx_rpc_rcv_buf,
                  rcv_buf_len * sizeof(intptr_t),
                  0,
                  0);

    for (i = 0; i < rcv_buf_len; i++) {
        rcv_buf[i] = dtx_rpc_rcv_buf[i];
    }

    SVM_Unlock();
    return dtx_rpc_rcv_buf[0];
}

void DTX_ExecServer() {
    DTX dtx;
    Sint32 i;

    SJCRS_Lock();

    for (i = 0; i < DTX_MAX_OBJ; i++) {
        dtx = &dtx_clnt[i];

        if (dtx->unk0 == 1) {
            DTX_ExecHndl(dtx);
        }
    }

    SJCRS_Unlock();
}

// more data
Sint32 dtx_init_cnt = 0;
DTX_OBJ dtx_clnt[DTX_MAX_OBJ] = { 0 };
DTX_OBJ dtx_svr[DTX_MAX_OBJ] = { 0 };
sceSifClientData dtx_cd = { 0 };
Char8 dtx_sd[0x44] = { 0 };
Char8 dtx_svrbuf[0x100] = { 0 };
Char8 dtx_urpc_fn[0x100] = { 0 };
Char8 dtx_urpc_obj[0x100] = { 0 };
