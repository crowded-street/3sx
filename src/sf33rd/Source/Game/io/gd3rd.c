/**
 * @file gd3rd.c
 * AFS file reading
 */

#include "sf33rd/Source/Game/io/gd3rd.h"
#include "common.h"
#include "constants.h"
#include "port/io/afs.h"
#include "port/utils.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd_data.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

#include <SDL3/SDL.h>

typedef void (*LDREQ_Process_Func)(LoadRequest*);

const u8 lpr_wrdata[3] = { 0x03, 0xC0, 0x3C };
const u8 lpc_seldat[2] = { 10, 11 };
const u8 lpt_seldat[4] = { 3, 4, 5, 0 };

Character plt_req[2];

void q_ldreq_error(LoadRequest* curr);

static const LDREQ_Process_Func ldreq_process[] = {
    [LDREQ_INVALID] = q_ldreq_error,     [LDREQ_TEXTURE] = q_ldreq_texture_group, [LDREQ_COLOR] = q_ldreq_color_data,
    [LDREQ_SCREEN] = q_ldreq_color_data, [LDREQ_SOUND] = q_ldreq_color_data,      [LDREQ_KANJI] = q_ldreq_color_data,
};

/// Load request queue
static LoadRequest q_ldreq[16] = { 0 };

static bool ldreq_break = false;
static u8 ldreq_result[294] = { 0 };
static AFSHandle afs_handle = AFS_NONE;

bool fsOpen(u16 fnum) {
    if (fnum >= AFS_GetFileCount()) {
        return false;
    }

    if (afs_handle != AFS_NONE) {
        AFS_Close(afs_handle);
    }

    afs_handle = AFS_Open(fnum);
    return true;
}

void fsClose() {
    AFS_Close(afs_handle);
    afs_handle = AFS_NONE;
}

u32 fsGetFileSize(u16 fnum) {
    if (fnum >= AFS_GetFileCount()) {
        return 0;
    }

    return AFS_GetSize(fnum);
}

static void fsCansel() {
    if ((afs_handle != AFS_NONE) && (AFS_GetState(afs_handle) == AFS_READ_STATE_READING)) {
        AFS_Stop(afs_handle);
    }
}

bool fsCheckCommandExecuting() {
    if (afs_handle == AFS_NONE) {
        return false;
    }

    const AFSReadState state = AFS_GetState(afs_handle);

    switch (state) {
    case AFS_READ_STATE_READING:
    case AFS_READ_STATE_ERROR:
        return true;

    case AFS_READ_STATE_IDLE:
    case AFS_READ_STATE_FINISHED:
        return false;

    default:
        fatal_error("Unhandled AFS state: %d", state);
    }
}

s32 fsRequestFileRead(void* buff) {
    AFS_Read(afs_handle, buff);
    return 1;
}

FileReadStatus fsCheckFileReaded() {
    const AFSReadState state = AFS_GetState(afs_handle);

    switch (state) {
    case AFS_READ_STATE_IDLE:
    case AFS_READ_STATE_FINISHED:
        return FS_READ_IDLE;

    case AFS_READ_STATE_READING:
        return FS_READ_READING;

    case AFS_READ_STATE_ERROR:
        return FS_READ_ERROR;

    default:
        fatal_error("Unhandled AFS state: %d", state);
    }
}

bool fsFileReadSync(void* buff) {
    AFS_ReadSync(afs_handle, buff);
    return fsCheckFileReaded() == FS_READ_IDLE;
}

s32 load_it_use_any_key2(u16 fnum, void** adrs, s16* key, u8 kokey, u8 group) {
    u32 size;

    if (fnum >= AFS_GetFileCount()) {
        flLogOut("ファイルナンバーに異常があります。ファイル番号：%d\n", fnum);
        while (1) {}
    }

    size = fsGetFileSize(fnum);
    *key = Pull_ramcnt_key(size, kokey, group, 0);
    *adrs = (void*)Get_ramcnt_address(*key);

    if (load_it_use_this_key(fnum, *key)) {
        return size;
    } else {
        Push_ramcnt_key(*key);
        return 0;
    }
}

s16 load_it_use_any_key(u16 fnum, u8 kokey, u8 group) {
    u32 err;
    void* adrs;
    s16 key;

    err = load_it_use_any_key2(fnum, &adrs, &key, kokey, group);

    if (err != 0) {
        return key;
    }

    return 0;
}

bool load_it_use_this_key(u16 fnum, s16 key) {
    if (!fsOpen(fnum)) {
        fatal_error("load_it_use_this_key: Failed to open file %d", fnum);
    }

    const u32 size = fsGetFileSize(fnum);
    bool success = fsFileReadSync((void*)Get_ramcnt_address(key));
    fsClose();
    Set_size_data_ramcnt_key(key, size);

    if (!success) {
        fatal_error("load_it_use_this_key: Failed to read file %d", fnum);
    }

    return true;
}

void Init_Load_Request_Queue() {
    SDL_zeroa(q_ldreq);
    ldreq_break = false;
}

void Request_LDREQ_Break() {
    ldreq_break = true;
}

bool Check_LDREQ_Break() {
    if (ldreq_break) {
        return true;
    }

    return fsCheckCommandExecuting();
}

static void Push_LDREQ_Queue(const LoadRequest* ldreq) {
    int i;

    for (i = 0; i < SDL_arraysize(q_ldreq); i++) {
        if (q_ldreq[i].be == 0) {
            break;
        }
    }

    if (i == SDL_arraysize(q_ldreq)) {
        fatal_error("Load request buffer is full");
    }

    q_ldreq[i] = *ldreq;
    q_ldreq[i].be = 2;
    q_ldreq[i].rno = 0;

    u8 masknum;

    switch (ldreq->id) {
    case 0:
        masknum = 3;
        break;

    case 1:
        masknum = 0xC0;
        break;

    default:
        masknum = 0x3C;
        break;
    }

    *q_ldreq[i].result &= ~masknum;
}

static void Push_LDREQ_Queue_Union(s16 ix) {
    const Span span = spans[ix];
    const int end = span.start + span.length;

    for (int i = span.start; i < end; i++) {
        LoadRequest ldreq = { 0 };
        ldreq.type = ldreq_tbl[i].type;
        ldreq.id = 2;
        ldreq.ix = ldreq_tbl[i].ix;
        ldreq.frre = ldreq_tbl[i].frre;
        ldreq.kokey = ldreq_tbl[i].kokey;
        ldreq.key = 0;
        ldreq.group = 0;
        ldreq.result = &ldreq_result[i];
        Push_LDREQ_Queue(&ldreq);
    }
}

void Push_LDREQ_Queue_Player(u8 id, Character character) {
    const Span span = spans[character];
    const int end = span.start + span.length;

    plt_req[id] = character;

    for (int i = span.start; i < end; i++) {
        LoadRequest ldreq = { 0 };
        ldreq.type = ldreq_tbl[i].type;
        ldreq.id = id;
        ldreq.ix = ldreq_tbl[i].ix;
        ldreq.frre = ldreq_tbl[i].frre;
        ldreq.key = 0;
        ldreq.group = 0;
        ldreq.result = &ldreq_result[i];

        if (ldreq.type == LDREQ_COLOR) {
            ldreq.kokey = lpc_seldat[id];
        } else {
            ldreq.kokey = lpt_seldat[id];
        }

        Push_LDREQ_Queue(&ldreq);
    }
}

static void Push_LDREQ_Queue_Metamor() {
    switch ((My_char[0] == CHAR_TWELVE) + (My_char[1] == CHAR_TWELVE) * 2) {
    case 1:
        Push_LDREQ_Queue_Direct(My_char[1] + 212, 0);
        break;

    case 2:
        Push_LDREQ_Queue_Direct(My_char[0] + 212, 1);
        break;

    case 3:
        Push_LDREQ_Queue_Direct(230, 2);
        break;
    }
}

void Push_LDREQ_Queue_BG(s16 ix) {
    Push_LDREQ_Queue_Union(ix + 20);
    Push_LDREQ_Queue_Metamor();
}

void Push_LDREQ_Queue_Direct(s16 ix, s16 id) {
    LoadRequest ldreq = { 0 };
    ldreq.type = ldreq_tbl[ix].type;
    ldreq.id = id;
    ldreq.ix = ldreq_tbl[ix].ix;
    ldreq.frre = ldreq_tbl[ix].frre;
    ldreq.kokey = ldreq_tbl[ix].kokey;
    ldreq.key = 0;
    ldreq.group = 0;
    ldreq.result = &ldreq_result[ix];
    Push_LDREQ_Queue(&ldreq);
}

void Check_LDREQ_Queue() {
    if (!ldreq_break) {
        if (q_ldreq[0].be != 0) {
            ldreq_process[q_ldreq[0].type](&q_ldreq[0]);

            if (q_ldreq[0].be == 0) {
                int i;

                for (i = 0; i < SDL_arraysize(q_ldreq) - 1; i++) {
                    q_ldreq[i] = q_ldreq[i + 1];
                }

                q_ldreq[i].be = 0;
                q_ldreq[i].type = LDREQ_INVALID;
            }
        }
    } else {
        if (q_ldreq[0].be == 1) {
            fsCansel();
        }

        Init_Load_Request_Queue();
    }
}

bool Check_LDREQ_Clear() {
    return q_ldreq[0].be == 0 && q_ldreq[1].be == 0;
}

static bool Check_LDREQ_Queue_Union(s16 ix, u8 id) {
    const Span span = spans[ix];
    const int end = span.start + span.length;

    for (int i = span.start; i < end; i++) {
        if (!(ldreq_result[i] & lpr_wrdata[id])) {
            return false;
        }
    }

    return true;
}

bool Check_LDREQ_Queue_Player(u8 id) {
    return Check_LDREQ_Queue_Union(plt_req[id], id);
}

bool Check_LDREQ_Queue_BG(s16 ix) {
    return Check_LDREQ_Queue_Union(ix + 20, 2);
}

bool Check_LDREQ_Queue_Direct(s16 ix) {
    if (!(ldreq_result[ix] & lpr_wrdata[2])) {
        return false;
    }

    return true;
}

void q_ldreq_error(LoadRequest* curr) {
    curr->be = 0;
    fatal_error("Q_LDREQ_ERROR: bad load request");
}
