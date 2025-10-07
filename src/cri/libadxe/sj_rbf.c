#include "common.h"
#include <cri/private/libadxe/sj_err.h>
#include <cri/private/libadxe/sj_rbf.h>

#include <cri/cri_xpts.h>

#include <string.h>

typedef struct {
    SJ_OBJ sj;                                 // 0x00
    Sint32 used;                               // 0x04
    UUID* uuid;                                // 0x08
    Sint32 unkC;                               // 0x0C
    Sint32 unk10;                              // 0x10
    Sint32 unk14;                              // 0x14
    Sint32 unk18;                              // 0x18
    Sint8* buf;                                // 0x1C
    Sint32 bsize;                              // 0x20
    Sint32 xsize;                              // 0x24
    Sint32 unk28;                              // 0x28
    Sint32 unk2C;                              // 0x2C
    Sint32 unk30;                              // 0x30
    Sint32 unk34;                              // 0x34
    void (*err_func)(void* obj, Sint32 ecode); // 0x38
    void* err_obj;                             // 0x3C
} SJRBF_OBJ;

typedef SJRBF_OBJ* SJRBF;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

#define SJRBF_MAX_OBJ 256

// forward decls
void SJRBF_Reset(SJRBF sjrbf);
void SJRBF_Destroy(SJRBF sjrbf);
UUID* SJRBF_GetUuid(SJRBF sjrbf);
void SJRBF_Reset(SJRBF sjrbf);
void SJRBF_GetChunk(SJRBF sjrbf, Sint32 id, Sint32 nbyte, SJCK* ck);
void SJRBF_UngetChunk(SJRBF sjrbf, Sint32 id, SJCK* ck);
void SJRBF_PutChunk(SJRBF sjrbf, Sint32 id, SJCK* ck);
Sint32 SJRBF_GetNumData(SJRBF sjrbf, Sint32 id);
Sint32 SJRBF_IsGetChunk(SJRBF sjrbf, Sint32 id, Sint32 nbyte, Sint32* rbyte);
void SJRBF_EntryErrFunc(SJRBF sjrbf, void (*func)(void* obj, Sint32 ecode), void* obj);

// data
Char8* volatile sj_build = "\nSJ/PS2EE Ver.6.18 Build:Sep 18 2003 09:59:52\n";

SJ_IF sjrbf_vtbl = { .QueryInterface = NULL,
                     .AddRef = NULL,
                     .Release = NULL,
                     .Destroy = SJRBF_Destroy,
                     .GetUuid = SJRBF_GetUuid,
                     .Reset = SJRBF_Reset,
                     .GetChunk = SJRBF_GetChunk,
                     .UngetChunk = SJRBF_UngetChunk,
                     .PutChunk = SJRBF_PutChunk,
                     .GetNumData = SJRBF_GetNumData,
                     .IsGetChunk = SJRBF_IsGetChunk,
                     .EntryErrFunc = SJRBF_EntryErrFunc };

Sint32 sjrbf_init_cnt = 0;
SJRBF_OBJ sjrbf_obj[SJRBF_MAX_OBJ] = { 0 };

const UUID sjrbf_uuid = {
    .Data1 = 0x3B9A9E81, .Data2 = 0x0DBB, .Data3 = 0x11D2, .Data4 = { 0xA6, 0xBF, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 }
};

void SJRBF_Error(void* obj, Sint32 ecode) {
    SJERR_CallErr("SJRBF Error");
}

void SJRBF_Init() {
    sj_build;

    SJCRS_Lock();

    if (sjrbf_init_cnt == 0) {
        memset(sjrbf_obj, 0, sizeof(sjrbf_obj));
    }

    sjrbf_init_cnt += 1;
    SJCRS_Unlock();
}

void SJRBF_Finish() {
    SJCRS_Lock();

    if (--sjrbf_init_cnt == 0) {
        memset(sjrbf_obj, 0, sizeof(sjrbf_obj));
    }

    SJCRS_Unlock();
}

SJ SJRBF_Create(Sint8* buf, Sint32 bsize, Sint32 xsize) {
    SJRBF sjrbf;
    Sint32 i;

    SJCRS_Lock();

    for (i = 0; i < SJRBF_MAX_OBJ; i++) {
        if (sjrbf_obj[i].used == 0) {
            break;
        }
    }

    sjrbf = NULL;

    if (i != SJRBF_MAX_OBJ) {
        sjrbf = &sjrbf_obj[i];
        sjrbf->used = 1;
        sjrbf->sj.vtbl = &sjrbf_vtbl;
        sjrbf->buf = buf;
        sjrbf->bsize = bsize;
        sjrbf->xsize = xsize;
        sjrbf->uuid = &sjrbf_uuid;
        sjrbf->err_func = SJRBF_Error;
        sjrbf->err_obj = sjrbf;
        SJRBF_Reset(sjrbf);
    }

    SJCRS_Unlock();
    return sjrbf;
}

void SJRBF_Destroy(SJRBF sjrbf) {
    SJCRS_Lock();

    if (sjrbf != NULL) {
        memset(sjrbf, 0, sizeof(SJRBF_OBJ));
        sjrbf->used = 0;
    }

    SJCRS_Unlock();
}

UUID* SJRBF_GetUuid(SJRBF sjrbf) {
    return sjrbf->uuid;
}

void SJRBF_EntryErrFunc(SJRBF sjrbf, void (*func)(void* obj, Sint32 ecode), void* obj) {
    sjrbf->err_func = func;
    sjrbf->err_obj = obj;
}

void SJRBF_Reset(SJRBF sjrbf) {
    SJCRS_Lock();
    sjrbf->unk34 = 0;
    sjrbf->unkC = 0;
    sjrbf->unk14 = 0;
    sjrbf->unk18 = 0;
    sjrbf->unk28 = 0;
    sjrbf->unk2C = 0;
    sjrbf->unk30 = 0;
    sjrbf->unk10 = sjrbf->bsize;
    SJCRS_Unlock();
}

Sint32 SJRBF_GetNumData(SJRBF sjrbf, Sint32 id) {
    if (id == 1) {
        return sjrbf->unkC;
    } else if (id == 0) {
        return sjrbf->unk10;
    } else {
        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }

        return 0;
    }
}

void SJRBF_GetChunk(SJRBF sjrbf, Sint32 id, Sint32 nbyte, SJCK* ck) {
    SJCRS_Lock();

    if (id == 0) {
        ck->len = MIN(MIN(sjrbf->unk10, sjrbf->bsize - sjrbf->unk14 + sjrbf->xsize), nbyte);
        ck->data = sjrbf->buf + sjrbf->unk14;
        sjrbf->unk14 = (sjrbf->unk14 + ck->len) % sjrbf->bsize;
        sjrbf->unk10 -= ck->len;
        sjrbf->unk28 += ck->len;
    } else if (id == 1) {
        ck->len = MIN(MIN(sjrbf->unkC, sjrbf->bsize - sjrbf->unk18 + sjrbf->xsize), nbyte);
        ck->data = sjrbf->buf + sjrbf->unk18;
        sjrbf->unk18 = (sjrbf->unk18 + ck->len) % sjrbf->bsize;
        sjrbf->unkC -= ck->len;
        sjrbf->unk30 += ck->len;
    } else {
        ck->len = 0;
        ck->data = NULL;

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }

    SJCRS_Unlock();
}

void SJRBF_PutChunk(SJRBF sjrbf, Sint32 id, SJCK* ck) {
    ptrdiff_t temp_a2_4;
    ptrdiff_t temp_a3;

    if ((ck->len <= 0) || (ck->data == NULL)) {
        return;
    }

    SJCRS_Lock();

    if (id == 1) {
        temp_a3 = ck->data - sjrbf->buf;
        sjrbf->unkC += ck->len;

        if (temp_a3 < sjrbf->xsize) {
            memcpy(sjrbf->buf + (sjrbf->bsize + temp_a3), ck->data, MIN(ck->len, sjrbf->xsize - temp_a3));
        }

        temp_a3 = (ck->data - sjrbf->buf) + ck->len;

        if (sjrbf->bsize < temp_a3) {
            temp_a2_4 = MIN(ck->len, temp_a3 - (ptrdiff_t)sjrbf->bsize);
            memcpy(sjrbf->buf, sjrbf->buf + (temp_a3 - temp_a2_4), temp_a2_4);
        }

        sjrbf->unk34 += ck->len;
    } else if (id == 0) {
        sjrbf->unk10 += ck->len;
        sjrbf->unk2C += ck->len;
    } else {
        ck->len = 0;
        ck->data = NULL;

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }

    SJCRS_Unlock();
}

void SJRBF_UngetChunk(SJRBF sjrbf, Sint32 id, SJCK* ck) {
    Sint32 a;
    Sint32 b;

    if ((ck->len <= 0) || (ck->data == NULL)) {
        return;
    }

    SJCRS_Lock();

    if (id == 0) {
        a = (sjrbf->unk14 + sjrbf->bsize - ck->len) % sjrbf->bsize;
        b = (Sint32)(ck->data - sjrbf->buf) % sjrbf->bsize;

        if (a == b) {
            sjrbf->unk14 = a;
            sjrbf->unk10 += ck->len;
        } else {
            if (sjrbf->err_func != NULL) {
                sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
            }
        }

        sjrbf->unk28 -= ck->len;
    } else if (id == 1) {
        a = (sjrbf->unk18 + sjrbf->bsize - ck->len) % sjrbf->bsize;
        b = (Sint32)(ck->data - sjrbf->buf) % sjrbf->bsize;

        if (a == b) {
            sjrbf->unk18 = a;
            sjrbf->unkC += ck->len;
        } else {
            if (sjrbf->err_func != NULL) {
                sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
            }
        }

        sjrbf->unk30 -= ck->len;
    } else {
        ck->len = 0;
        ck->data = NULL;

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }

    SJCRS_Unlock();
}

Sint32 SJRBF_IsGetChunk(SJRBF sjrbf, Sint32 id, Sint32 nbyte, Sint32* rbyte) {
    Sint32 unk;

    SJCRS_Lock();

    if (id == 0) {
        unk = MIN(sjrbf->unk10, sjrbf->bsize - sjrbf->unk14 + sjrbf->xsize);
        unk = MIN(unk, nbyte);
    } else if (id == 1) {
        unk = MIN(sjrbf->unkC, sjrbf->bsize - sjrbf->unk18 + sjrbf->xsize);
        unk = MIN(unk, nbyte);
    } else {
        unk = 0;

        if (sjrbf->err_func != NULL) {
            sjrbf->err_func(sjrbf->err_obj, SJ_ERR_PRM);
        }
    }

    *rbyte = unk;
    SJCRS_Unlock();
    return unk == nbyte;
}

void* SJRBF_GetBufPtr(SJ sj) {
    SJRBF sjrbf = (SJRBF)sj;
    return sjrbf->buf;
};

Sint32 SJRBF_GetBufSize(SJ sj) {
    SJRBF sjrbf = (SJRBF)sj;
    return sjrbf->bsize;
};

Sint32 SJRBF_GetXtrSize(SJ sj) {
    SJRBF sjrbf = (SJRBF)sj;
    return sjrbf->xsize;
}

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_rbf", SJRBF_SetFlowCnt);

INCLUDE_ASM("asm/anniversary/nonmatchings/cri/libadxe/sj_rbf", SJRBF_GetFlowCnt);
