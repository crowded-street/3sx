#include "common.h"
#include <cri/private/libadxe/sj_err.h>

#include <cri/cri_xpts.h>
#include <cri/sj.h>

#include <string.h>

typedef struct {
    SJ_OBJ sj;
    Sint32 used;
    UUID* uuid;
    Sint32 unkC;
    Sint32 unk10;
    Sint8* buf_ptr;
    Sint32 bsize;
    void (*err_func)(void* obj, Sint32 ecode);
    void* err_obj;
} SJMEM_OBJ;

typedef SJMEM_OBJ* SJMEM;

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define SJMEM_MAX_OBJ 32

// forward decls
void SJMEM_Reset(SJMEM sjmem);
void SJMEM_Destroy(SJMEM sjmem);
UUID* SJMEM_GetUuid(SJMEM sjmem);
void SJMEM_Reset(SJMEM sjmem);
void SJMEM_GetChunk(SJMEM sjmem, Sint32 id, Sint32 nbyte, SJCK* ck);
void SJMEM_UngetChunk(SJMEM sjmem, Sint32 id, SJCK* ck);
void SJMEM_PutChunk(SJMEM sjmem, Sint32 id, SJCK* ck);
Sint32 SJMEM_GetNumData(SJMEM sjmem, Sint32 id);
Sint32 SJMEM_IsGetChunk(SJMEM sjmem, Sint32 id, Sint32 nbyte, Sint32* rbyte);
void SJMEM_EntryErrFunc(SJMEM sjmem, void (*func)(void* obj, Sint32 ecode), void* obj);

// data
SJ_IF sjmem_vtbl = { .QueryInterface = NULL,
                     .AddRef = NULL,
                     .Release = NULL,
                     .Destroy = SJMEM_Destroy,
                     .GetUuid = SJMEM_GetUuid,
                     .Reset = SJMEM_Reset,
                     .GetChunk = SJMEM_GetChunk,
                     .UngetChunk = SJMEM_UngetChunk,
                     .PutChunk = SJMEM_PutChunk,
                     .GetNumData = SJMEM_GetNumData,
                     .IsGetChunk = SJMEM_IsGetChunk,
                     .EntryErrFunc = SJMEM_EntryErrFunc };

Sint32 sjmem_init_cnt = 0;
SJMEM_OBJ sjmem_obj[SJMEM_MAX_OBJ] = { 0 };

const UUID sjmem_uuid = {
    .Data1 = 0xDD9EEE41, .Data2 = 0x1679, .Data3 = 0x11D2, .Data4 = { 0x93, 0x6C, 0x00, 0x60, 0x08, 0x94, 0x48, 0xBC }
};

void SJMEM_Error(void* obj, Sint32 ecode) {
    SJERR_CallErr("SJMEM Error");
}

void SJMEM_Init() {
    if (sjmem_init_cnt == 0) {
        memset(sjmem_obj, 0, sizeof(sjmem_obj));
    }

    sjmem_init_cnt += 1;
}

void SJMEM_Finish() {
    if (--sjmem_init_cnt == 0) {
        memset(sjmem_obj, 0, sizeof(sjmem_obj));
    }
}

SJ SJMEM_Create(Sint8* data, Sint32 bsize) {
    SJMEM sjmem;
    Sint32 i;

    for (i = 0; i < SJMEM_MAX_OBJ; i++) {
        if (sjmem_obj[i].used == 0) {
            break;
        }
    }

    if (i == SJMEM_MAX_OBJ) {
        return NULL;
    }

    sjmem = &sjmem_obj[i];
    sjmem->used = 1;
    sjmem->sj.vtbl = &sjmem_vtbl;
    sjmem->buf_ptr = data;
    sjmem->bsize = bsize;
    sjmem->uuid = &sjmem_uuid;
    sjmem->err_func = SJMEM_Error;
    sjmem->err_obj = sjmem;
    SJMEM_Reset(sjmem);
    return sjmem;
}

void SJMEM_Destroy(SJMEM sjmem) {
    if (sjmem != NULL) {
        memset(sjmem, 0, sizeof(SJMEM_OBJ));
        sjmem->used = 0;
    }
}

UUID* SJMEM_GetUuid(SJMEM sjmem) {
    return sjmem->uuid;
}

void SJMEM_EntryErrFunc(SJMEM sjmem, void (*func)(void* obj, Sint32 ecode), void* obj) {
    sjmem->err_func = func;
    sjmem->err_obj = obj;
}

void SJMEM_Reset(SJMEM sjmem) {
    sjmem->unk10 = 0;
    sjmem->unkC = sjmem->bsize;
}

Sint32 SJMEM_GetNumData(SJMEM sjmem, Sint32 id) {
    if (id == 1) {
        return sjmem->unkC;
    }

    if (id != 0 && sjmem->err_func != NULL) {
        sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
    }

    return 0;
}

void SJMEM_GetChunk(SJMEM sjmem, Sint32 id, Sint32 nbyte, SJCK* ck) {
    Sint32 len;

    SJCRS_Lock();

    if (id == 0) {
        ck->len = 0;
        ck->data = NULL;
    } else if (id == 1) {
        len = sjmem->unkC;
        len = MIN(len, nbyte);
        ck->len = len;
        ck->data = sjmem->buf_ptr + sjmem->unk10;
        sjmem->unk10 += len;
        sjmem->unkC -= ck->len;
    } else {
        ck->len = 0;
        ck->data = NULL;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }

    SJCRS_Unlock();
}

void SJMEM_PutChunk(SJMEM sjmem, Sint32 id, SJCK* ck) {
    if ((ck->len <= 0) || (ck->data == NULL)) {
        return;
    }

    SJCRS_Lock();

    if (id == 0) {
        // Do nothing
    } else if (id == 1) {
        // Do nothing
    } else {
        ck->len = 0;
        ck->data = NULL;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }

    SJCRS_Unlock();
}

void SJMEM_UngetChunk(SJMEM sjmem, Sint32 id, SJCK* ck) {
    if ((ck->len <= 0) || (ck->data == NULL)) {
        return;
    }

    SJCRS_Lock();

    if (id == 0) {
        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    } else if (id == 1) {
        sjmem->unk10 = MAX(sjmem->unk10 - ck->len, 0);
        sjmem->unkC = MIN(sjmem->bsize, sjmem->unkC + ck->len);

        if (sjmem->unk10 != (ck->data - sjmem->buf_ptr)) {
            if (sjmem->err_func != NULL) {
                sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
            }
        }
    } else {
        ck->len = 0;
        ck->data = NULL;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }

    SJCRS_Unlock();
}

Sint32 SJMEM_IsGetChunk(SJMEM sjmem, Sint32 id, Sint32 nbyte, Sint32* rbyte) {
    Sint32 unk;

    SJCRS_Lock();

    if (id == 0) {
        unk = 0;
    } else if (id == 1) {
        unk = MIN(sjmem->unkC, nbyte);
    } else {
        unk = 0;

        if (sjmem->err_func != NULL) {
            sjmem->err_func(sjmem->err_obj, SJ_ERR_PRM);
        }
    }

    *rbyte = unk;
    SJCRS_Unlock();
    return unk == nbyte;
}

Sint8* SJMEM_GetBufPtr(SJMEM sjmem) {
    return sjmem->buf_ptr;
}

Sint32 SJMEM_GetBufSize(SJMEM sjmem) {
    return sjmem->bsize;
}
