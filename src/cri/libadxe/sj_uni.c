#include "common.h"
#include <cri/private/libadxe/sj_err.h>

#include <cri/cri_xpts.h>
#include <cri/sj.h>

#include <string.h>

typedef struct SJUNI_CK {
    struct SJUNI_CK* next;
    Sint32 unk4;
    SJCK ck;
} SJUNI_CK;

typedef struct {
    SJ_OBJ sj;
    Sint8 used;
    Sint8 mode;
    UUID* uuid;
    SJUNI_CK* work;
    Sint32 chunk_count;
    SJUNI_CK* chain_pool;
    SJUNI_CK* unk18[4];
    void (*err_func)(void* obj, Sint32 ecode);
    void* err_obj;
} SJUNI_OBJ;

typedef SJUNI_OBJ* SJUNI;

#define SJUNI_MAX_OBJ 64

// forward decls
void SJUNI_Reset(SJUNI sjuni);
void SJUNI_Destroy(SJUNI sjuni);
UUID* SJUNI_GetUuid(SJUNI sjuni);
void SJUNI_Reset(SJUNI sjuni);
void SJUNI_GetChunk(SJUNI sjuni, Sint32 id, Sint32 nbyte, SJCK* ck);
void SJUNI_UngetChunk(SJUNI sjuni, Sint32 id, SJCK* ck);
void SJUNI_PutChunk(SJUNI sjuni, Sint32 id, SJCK* ck);
Sint32 SJUNI_GetNumData(SJUNI sjuni, Sint32 id);
Sint32 SJUNI_IsGetChunk(SJUNI sjuni, Sint32 id, Sint32 nbyte, Sint32* rbyte);
void SJUNI_EntryErrFunc(SJUNI sjuni, void (*func)(void* obj, Sint32 ecode), void* obj);

// data
SJ_IF sjuni_vtbl = { .QueryInterface = NULL,
                     .AddRef = NULL,
                     .Release = NULL,
                     .Destroy = SJUNI_Destroy,
                     .GetUuid = SJUNI_GetUuid,
                     .Reset = SJUNI_Reset,
                     .GetChunk = SJUNI_GetChunk,
                     .UngetChunk = SJUNI_UngetChunk,
                     .PutChunk = SJUNI_PutChunk,
                     .GetNumData = SJUNI_GetNumData,
                     .IsGetChunk = SJUNI_IsGetChunk,
                     .EntryErrFunc = SJUNI_EntryErrFunc };

Sint32 sjuni_init_cnt = 0;
SJUNI_OBJ sjuni_obj[SJUNI_MAX_OBJ] = { 0 };

const UUID sjuni_uuid = {
    .Data1 = 0x2E534FA3, .Data2 = 0xAF97, .Data3 = 0x11D2, .Data4 = { 0xA5, 0x27, 0x00, 0x60, 0x08, 0x94, 0x48, 0xBC }
};

void SJUNI_Error(void* obj, Sint32 ecode) {
    SJERR_CallErr("SJUNI Error");
}

void SJUNI_Init() {
    if (sjuni_init_cnt == 0) {
        memset(sjuni_obj, 0, sizeof(sjuni_obj));
    }

    sjuni_init_cnt += 1;
}

void SJUNI_Finish() {
    if (--sjuni_init_cnt == 0) {
        memset(sjuni_obj, 0, sizeof(sjuni_obj));
    }
}

SJ SJUNI_Create(Sint32 mode, Sint8* work, Sint32 wksize) {
    SJUNI sjuni;
    Sint32 i;

    for (i = 0; i < SJUNI_MAX_OBJ; i++) {
        if (sjuni_obj[i].used == 0) {
            break;
        }
    }

    if (i == SJUNI_MAX_OBJ) {
        return NULL;
    }

    sjuni = &sjuni_obj[i];
    sjuni->used = 1;
    sjuni->sj.vtbl = &sjuni_vtbl;
    sjuni->mode = mode;
    sjuni->uuid = &sjuni_uuid;
    sjuni->work = (SJUNI_CK*)work;
    sjuni->chunk_count = wksize / 16; // 16 is the size of SJUNI_CK
    sjuni->err_func = SJUNI_Error;
    sjuni->err_obj = sjuni;
    SJUNI_Reset(sjuni);
    return sjuni;
}

void SJUNI_Destroy(SJUNI sjuni) {
    if (sjuni != NULL) {
        memset(sjuni, 0, sizeof(SJUNI_OBJ));
        sjuni->used = 0;
    }
}

UUID* SJUNI_GetUuid(SJUNI sjuni) {
    return sjuni->uuid;
}

void SJUNI_EntryErrFunc(SJUNI sjuni, void (*func)(void* obj, Sint32 ecode), void* obj) {
    sjuni->err_func = func;
    sjuni->err_obj = obj;
}

void SJUNI_Reset(SJUNI sjuni) {
    Sint32 i;
    SJUNI_CK* work = sjuni->work;

    sjuni->chain_pool = work;

    for (i = 0; i < (sjuni->chunk_count - 1); i++) {
        work[i].next = &work[i + 1];
        work[i].ck.data = NULL;
        work[i].ck.len = 0;
    }

    work[i].next = NULL;
    work[i].ck.data = NULL;
    work[i].ck.len = 0;

    for (i = 0; i < 4; i++) {
        sjuni->unk18[i] = 0;
    }
}

Sint32 SJUNI_GetNumData(SJUNI sjuni, Sint32 id) {
    SJUNI_CK* chunk;
    Sint32 num_data;

    if ((Uint32)id >= 4) {
        if (sjuni->err_func != NULL) {
            sjuni->err_func(sjuni->err_obj, SJ_ERR_PRM);
        }

        return 0;
    }

    chunk = sjuni->unk18[id];
    num_data = 0;

    while (chunk != NULL) {
        num_data += chunk->ck.len;
        chunk = chunk->next;
    }

    return num_data;
}

void SJUNI_GetChunk(SJUNI sjuni, Sint32 id, Sint32 nbyte, SJCK* ck) {
    SJCK sjck0;
    SJCK sjck1;
    SJUNI_CK* cur;

    if ((Uint32)id >= 4) {
        if (sjuni->err_func != NULL) {
            sjuni->err_func(sjuni->err_obj, SJ_ERR_PRM);
        }

        ck->data = NULL;
        ck->len = 0;
        return;
    }

    SJCRS_Lock();
    cur = sjuni->unk18[id];

    if (cur != NULL) {
        sjck0 = cur->ck;

        if (nbyte >= sjck0.len) {
            *ck = sjck0;
            sjuni->unk18[id] = cur->next;
            cur->next = sjuni->chain_pool;
            sjuni->chain_pool = cur;
        } else if (sjuni->mode == SJUNI_MODE_JOIN) {
            SJ_SplitChunk(&sjck0, nbyte, &sjck0, &sjck1);
            *ck = sjck0;
            cur->ck = sjck1;
        } else {
            ck->data = NULL;
            ck->len = 0;
        }
    } else {
        ck->data = NULL;
        ck->len = 0;
    }

    SJCRS_Unlock();
}

void SJUNI_PutChunk(SJUNI sjuni, Sint32 id, SJCK* ck) {
    SJUNI_CK* last;
    SJUNI_CK* cur;
    SJUNI_CK** next_of_last;

    if ((Uint32)id >= 4) {
        if (sjuni->err_func != NULL) {
            sjuni->err_func(sjuni->err_obj, SJ_ERR_PRM);
        }

        return;
    }

    if (ck->len <= 0 || ck->data == NULL) {
        return;
    }

    SJCRS_Lock();
    next_of_last = &sjuni->unk18[id];
    cur = *next_of_last;
    last = NULL;

    while (cur != NULL) {
        last = cur;
        cur = cur->next;
        next_of_last = &last->next;
    }

    if (sjuni->mode == SJUNI_MODE_JOIN && last != NULL && (last->ck.data + last->ck.len == ck->data)) {
        last->ck.len += ck->len;
    } else {
        last = sjuni->chain_pool;

        if (last == NULL) {
            if (sjuni->err_func != NULL) {
                sjuni->err_func(sjuni->err_obj, SJ_ERR_PRM);
            }
        } else {
            sjuni->chain_pool = sjuni->chain_pool->next;
            last->ck = *ck;
            last->next = NULL;
            *next_of_last = last;
        }
    }

    SJCRS_Unlock();
}

void SJUNI_UngetChunk(SJUNI sjuni, Sint32 id, SJCK* ck) {
    SJUNI_CK* temp;

    if ((Uint32)id >= 4) {
        if (sjuni->err_func != NULL) {
            sjuni->err_func(sjuni->err_obj, SJ_ERR_PRM);
        }

        return;
    }

    if (ck->len <= 0 || ck->data == NULL) {
        return;
    }

    SJCRS_Lock();
    temp = sjuni->unk18[id];

    if ((sjuni->mode == SJUNI_MODE_JOIN) && (temp != NULL) && (ck->data + ck->len == temp->ck.data)) {
        temp->ck.data = ck->data;
        temp->ck.len += ck->len;
    } else {
        temp = sjuni->chain_pool;

        if (temp == NULL) {
            if (sjuni->err_func != NULL) {
                sjuni->err_func(sjuni->err_obj, SJ_ERR_PRM);
            }
        } else {
            sjuni->chain_pool = sjuni->chain_pool->next;
            temp->ck = *ck;
            temp->next = NULL;
            temp->next = sjuni->unk18[id];
            sjuni->unk18[id] = temp;
        }
    }

    SJCRS_Unlock();
}

Sint32 SJUNI_IsGetChunk(SJUNI sjuni, Sint32 id, Sint32 nbyte, Sint32* rbyte) {
    SJCK ck;

    *rbyte = 0;

    if ((Uint32)id >= 4) {
        if (sjuni->err_func != NULL) {
            sjuni->err_func(sjuni->err_obj, SJ_ERR_PRM);
        }

        return 0;
    }

    if (sjuni->unk18[id] == NULL) {
        return 0;
    }

    ck = sjuni->unk18[id]->ck;
    *rbyte = ck.len;

    if (sjuni->mode == SJUNI_MODE_JOIN) {
        if (ck.len >= nbyte) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return (ck.len == nbyte);
    }
}

Sint32 SJUNI_GetNumChunk(SJ sj, Sint32 id) {
    SJUNI sjuni = (SJUNI)sj;
    SJUNI_CK* chunk = sjuni->unk18[id];
    Sint32 num_chunk = 0;

    while (chunk != NULL) {
        chunk = chunk->next;
        num_chunk += 1;
    }

    return num_chunk;
}

Sint32 SJUNI_GetNumChainPool(SJ sj) {
    SJUNI sjuni = (SJUNI)sj;
    SJUNI_CK* chunk = sjuni->chain_pool;
    Sint32 num_chain_pool = 0;

    while (chunk != NULL) {
        chunk = chunk->next;
        num_chain_pool += 1;
    }

    return num_chain_pool;
}
