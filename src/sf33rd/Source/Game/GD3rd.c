#include "sf33rd/Source/Game/GD3rd.h"
#include "common.h"
#include "sf33rd/AcrSDK/MiddleWare/PS2/CapSndEng/cse.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/FileSizeAFS.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/texgroup.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

#include "port/sdk_threads.h"

#include <cri_mw.h>
#include <libcdvd.h>
#include <libgraph.h>

typedef struct {
    // total size: 0x4
    u8 trycount;    // offset 0x0, size 0x1
    u8 spindlctrl;  // offset 0x1, size 0x1
    u8 datapattern; // offset 0x2, size 0x1
    u8 pad;         // offset 0x3, size 0x1
} PS2CDReadMode;

typedef struct {
    // total size: 0x4
    u8 type;  // offset 0x0, size 0x1
    u8 ix;    // offset 0x1, size 0x1
    u8 frre;  // offset 0x2, size 0x1
    u8 kokey; // offset 0x3, size 0x1
} LDREQ_TBL;

typedef void (*LDREQ_Process_Func)(REQ*);

u16 DskDrvErrBe;
u16 DskDrvErrType;
u16 DskDrvErrRetry;
PS2CDReadMode ps2CdReadMode;
s16 plt_req[2]; // size: 0x4, address: 0x579084
u8 ldreq_break;
struct _adx_fs* adxf = NULL;

#if defined(TARGET_PS2)
u8 sf3ptinfo[3352];
#else
u8 sf3ptinfo[3352 + 12];
#endif

REQ q_ldreq[16];      // size: 0x280, address: 0x5E1DD0
u8 ldreq_result[294]; // size: 0x126, address: 0x5E1CA0

const u8 lpr_wrdata[3] = { 0x03, 0xC0, 0x3C }; // size: 0x3, address: 0x51FCF0
const u8 lpc_seldat[2] = { 10, 11 };           // size: 0x2, address: 0x51FCF8
const u8 lpt_seldat[4] = { 3, 4, 5, 0 };       // size: 0x4, address: 0x51FD00

s32 Push_LDREQ_Queue(REQ* ldreq);
void Push_LDREQ_Queue_Metamor();
void q_ldreq_error(REQ* curr);
void disp_ldreq_status();
void Push_LDREQ_Queue_Union(s16 ix);
s32 Check_LDREQ_Queue_Union(s16 ix);

// forward decls
const LDREQ_Process_Func ldreq_process[6];
s8* ldreq_process_name[];
const LDREQ_TBL ldreq_tbl[294];
const s16 ldreq_ix[43][2];

s32 Setup_Directory_Record_Data() {
    DskDrvErrBe = 0;
    DskDrvErrType = 0xFFFF;
    DskDrvErrRetry = 0;

    ADXF_LoadPartitionNw(0, "SF33RD.AFS", NULL, sf3ptinfo);

    while (1) {
        if (ADXF_GetPtStat(0) == ADXF_STAT_READEND) {
            break;
        }

#if defined(TARGET_PS2)
        sceGsSyncV(0);
#else
        // CRI relies on VSync interrupts to execute its file system server.
        // On modern platforms we don't call the VSync interrupt handler until
        // we get to the main loop. That's why we have to emulate the interrupt
        // manually like this.
        begin_interrupt();
        ADXPS2_ExecVint(0);
        end_interrupt();
#endif

        ADXM_ExecMain();
    }

    ps2CdReadMode.trycount = 64;
    ps2CdReadMode.spindlctrl = 1;
    ps2CdReadMode.datapattern = 0;
    ps2CdReadMode.pad = 0;
    return 1;
}

void fsUpdateDiskDriveError() {
    s32 chkNext = 0;

    switch (sceCdGetDiskType()) {
    case SCECdNODISC:
        DskDrvErrType = 1;
        break;

    case SCECdDETCT:
        DskDrvErrType = 5;
        break;

    default:
        DskDrvErrType = 2;
        break;

    case SCECdPS2CD:
    case SCECdPS2CDDA:
    case SCECdPS2DVD:
        chkNext = 1;
        break;
    }

    if (chkNext == 0) {
        return;
    }

    switch (sceCdGetError()) {
    case SCECdErNO:
        DskDrvErrType = 0xFFFF;
        break;

    default:
        DskDrvErrType = 5;
        break;

    case SCECdErCUD:
    case SCECdErIPI:
    case SCECdErILI:
    case SCECdErREAD:
        DskDrvErrType = 4;
        break;

    case SCECdErTRMOPN:
        DskDrvErrType = 0;
        break;
    }
}

s32 fsOpen(REQ* req) {
    if (req->fnum >= AFS_FILE_COUNT) {
        return 0;
    }

    if (appFileSizes[req->fnum] == 0) {
        return 0;
    }

    if (adxf != NULL) {
        ADXF_Close(adxf);
    }

    adxf = ADXF_OpenAfs(0, req->fnum);

    if (adxf == NULL) {
        return 0;
    }

    req->info.number = 1;
    req->info.size = appFileSizes[req->fnum];
    return 1;
}

void fsClose(REQ* /* unused */) {
    ADXF_Close(adxf);
    adxf = NULL;
}

u32 fsGetFileSize(u16 fnum) {
    if (fnum >= AFS_FILE_COUNT) {
        return 0;
    }

    return appFileSizes[fnum];
}

u32 fsCalSectorSize(u32 size) {
    return (size + 2048 - 1) / 2048;
}

s32 fsCansel(REQ* /* unused */) {
    if (adxf != NULL && ADXF_GetStat(adxf) == ADXF_STAT_READING) {
        ADXF_StopNw(adxf);
    }

    return 1;
}

s32 fsCheckCommandExecuting() {
    if (adxf == NULL) {
        return 0;
    }

    if (ADXF_GetStat(adxf) == ADXF_STAT_READING || ADXF_GetStat(adxf) == ADXF_STAT_ERROR) {
        return 1;
    }

    return 0;
}

s32 fsRequestFileRead(REQ* /* unused */, u32 sec, void* buff) {
    ADXF_ReadNw(adxf, sec, buff);
    return 1;
}

s32 fsCheckFileReaded(REQ* /* unused */) {
    s32 rnum = ADXF_GetStat(adxf);
    fsUpdateDiskDriveError();

    if (rnum == ADXF_STAT_ERROR) {
        DskDrvErrBe = 1;
        return 2;
    }

    if (rnum == ADXF_STAT_READING) {
        return 0;
    }

    return 1;
}

s32 fsFileReadSync(REQ* req, u32 sec, void* buff) {
    s32 rnum = fsRequestFileRead(req, sec, buff);

    if (rnum == 0) {
        return 0;
    }

    while (1) {
        rnum = fsCheckFileReaded(req);

        if (rnum == 1) {
            return 1;
        }

        if (rnum == 2) {
            return 0;
        }

        waitVsyncDummy();
    }
}

void waitVsyncDummy() {
    ADXM_ExecMain();
    cseExecServer();

#if defined(TARGET_PS2)
    sceGsSyncV(0);
#else
    begin_interrupt();
    ADXPS2_ExecVint(0);
    end_interrupt();
#endif
}

s32 load_it_use_any_key2(u16 fnum, void** adrs, s16* key, u8 kokey, u8 group) {
    u32 size;
    u32 err;

    if (fnum >= AFS_FILE_COUNT) {
        flLogOut("ファイルナンバーに異常があります。ファイル番号：%d\n", fnum);
        while (1) {}
    }

    size = fsGetFileSize(fnum);
    *key = Pull_ramcnt_key(fsCalSectorSize(size) << 11, kokey, group, 0);
    *adrs = (void*)Get_ramcnt_address(*key);

    err = load_it_use_this_key(fnum, *key);

    if (err != 0) {
        return size;
    }

    Push_ramcnt_key(*key);
    return 0;
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

s32 load_it_use_this_key(u16 fnum, s16 key) {
    REQ req;
    u32 err;

    req.fnum = fnum;

    while (1) {
        err = fsOpen(&req);

        if (err == 0) {
            continue;
        }

        req.size = fsGetFileSize(req.fnum);
        req.sect = fsCalSectorSize(req.size);
        err = fsFileReadSync(&req, req.sect, (void*)Get_ramcnt_address(key));
        fsClose(&req);
        Set_size_data_ramcnt_key(key, req.size);

        if (err != 0) {
            return 1;
        }

        flLogOut("ファイルの読み込みに失敗しました。ファイル番号：%d\n", fnum);
    }
}

void Init_Load_Request_Queue_1st() {
    s16 i;

    for (i = 0; i < (s16)(sizeof(q_ldreq) / sizeof(REQ)); i++) {
        q_ldreq[i].be = 0;
        q_ldreq[i].type = 0;
    }

    ldreq_break = 0;
}

void Request_LDREQ_Break() {
    ldreq_break = 1;
}

u8 Check_LDREQ_Break() {
    if (ldreq_break) {
        return 1;
    }

    return fsCheckCommandExecuting();
}

void Push_LDREQ_Queue_Player(s16 id, s16 ix) {
    REQ ldreq;
    s16 i;
    s16 kara;
    s16 made;

    kara = ldreq_ix[ix][0];
    made = kara + ldreq_ix[ix][1];
    plt_req[id] = ix;

    for (i = kara; i < made; i++) {
        ldreq.type = ldreq_tbl[i].type;
        ldreq.id = id;
        ldreq.ix = ldreq_tbl[i].ix;
        ldreq.frre = ldreq_tbl[i].frre;
        ldreq.key = 0;
        ldreq.group = 0;
        ldreq.result = &ldreq_result[i];

        if (ldreq.type == 2) {
            ldreq.kokey = lpc_seldat[id];
        } else {
            ldreq.kokey = lpt_seldat[id];
        }

        Push_LDREQ_Queue(&ldreq);
    }
}

void Push_LDREQ_Queue_BG(s16 ix) {
#if defined(TARGET_PS2)
    void Push_LDREQ_Queue_Union(s32 ix);
#endif

    Push_LDREQ_Queue_Union(ix + 20);
    Push_LDREQ_Queue_Metamor();
}

void Push_LDREQ_Queue_Union(s16 ix) {
    REQ ldreq;
    s16 i;
    s16 kara;
    s16 made;

    kara = ldreq_ix[ix][0];
    made = kara + ldreq_ix[ix][1];

    for (i = kara; i < made; i++) {
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

void Push_LDREQ_Queue_Metamor() {
#if defined(TARGET_PS2)
    void Push_LDREQ_Queue_Direct(s32 ix, s16 id);
#endif

    switch ((My_char[0] == 0x12) + (My_char[1] == 0x12) * 2) {
    case 1:
        Push_LDREQ_Queue_Direct(My_char[1] + 0xD4, 0);
        break;

    case 2:
        Push_LDREQ_Queue_Direct(My_char[0] + 0xD4, 1);
        break;

    case 3:
        Push_LDREQ_Queue_Direct(0xE6, 2);
        break;
    }
}

void Push_LDREQ_Queue_Direct(s16 ix, s16 id) {
    REQ ldreq;
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

s32 Push_LDREQ_Queue(REQ* ldreq) {
    s16 i;
    u8 masknum;

    for (i = 0; i < 16; i++) {
        if (q_ldreq[i].be == 0) {
            break;
        }
    }

    if (i != 0x10) {
        q_ldreq[i] = ldreq[0];
        q_ldreq[i].be = 2;
        q_ldreq[i].rno = 0;
        q_ldreq[i].retry = 0x40;

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

        *(u8*)(&q_ldreq[i].result)[0] &= ~masknum;
        return 1;
    }

    flLogOut("ファイル読み込み要求バッファがオーバーしました。\n");
    return 0;
}

void Check_LDREQ_Queue() {
    s16 i;

    disp_ldreq_status();

    if (!ldreq_break) {
        if (q_ldreq->be != 0) {
            ldreq_process[q_ldreq->type](q_ldreq);

            if (q_ldreq->be == 0) {
                for (i = 0; i < 15; i++) {
                    q_ldreq[i] = q_ldreq[i + 1];
                }

                q_ldreq[i].be = 0;
                q_ldreq[i].type = 0;
            }

            return;
        }
    } else {
        if (q_ldreq->be == 1) {
            fsCansel(q_ldreq);
        }

        Init_Load_Request_Queue_1st();
    }
}

void disp_ldreq_status() {
    s16 i;

    flPrintColor(0xFFFFFF8F);

    if (Debug_w[0xE]) {
        for (i = 0; i < 16; i++) {
            flPrintL(2, i + 18, "%1d", q_ldreq[i].be);
            flPrintL(3, i + 18, ldreq_process_name[q_ldreq[i].type]);
        }

        flPrintL(2, i + 18, "%4d", system_timer);
    }
}

s32 Check_LDREQ_Clear() {
    return q_ldreq->be == 0 && q_ldreq[1].be == 0;
}

s32 Check_LDREQ_Queue_Player(s16 id) {
    s16 i;
    s16 kara;
    s16 made;

    kara = ldreq_ix[plt_req[id]][0];
    made = kara + ldreq_ix[plt_req[id]][1];

    for (i = kara; i < made; i++) {
        if (!(ldreq_result[i] & lpr_wrdata[id])) {
            break;
        }
    }

    if (i != made) {
        return 0;
    }

    return 1;
}

s32 Check_LDREQ_Queue_BG(s16 ix) {
#if defined(TARGET_PS2)
    s32 Check_LDREQ_Queue_Union(s32 ix);
#endif

    return Check_LDREQ_Queue_Union(ix + 20);
}

s32 Check_LDREQ_Queue_Union(s16 ix) {
    s16 i;
    s16 kara;
    s16 made;

    kara = ldreq_ix[ix][0];
    made = kara + ldreq_ix[ix][1];

    for (i = kara; i < made; i++) {
        if (!(ldreq_result[i] & lpr_wrdata[2])) {
            break;
        }
    }

    if (i != made) {
        return 0;
    }

    return 1;
}

s32 Check_LDREQ_Queue_Direct(s16 ix) {
    if (!(ldreq_result[ix] & lpr_wrdata[2])) {
        return 0;
    }

    return 1;
}

void q_ldreq_error(REQ* curr) {
    curr->be = 0;
    flLogOut("Q_LDREQ_ERROR : ロード処理の指定に誤りがあります。\n");
}

const LDREQ_Process_Func ldreq_process[6] = {
    // size: 0x18, address: 0x51FE30
    q_ldreq_error, q_ldreq_texture_group, q_ldreq_color_data, q_ldreq_color_data, q_ldreq_color_data, q_ldreq_color_data
};

s8* ldreq_process_name[] = {
    // size: 0x18, address: 0x573AB0
    "EMP", "TEX", "COL", "SCR", "SND", "KNJ",
};

const LDREQ_TBL ldreq_tbl[294] = {
    // size: 0x0, address: 0x51FE80
    {
        0x1,
        0x1,
        0x2,
        0x3,
    },
    {
        0x1,
        0x1B,
        0x2,
        0x3,
    },
    {
        0x1,
        0x23,
        0x2,
        0x3,
    },
    {
        0x2,
        0x0,
        0x1,
        0xA,
    },
    {
        0x4,
        0x82,
        0x1,
        0x16,
    },
    {
        0x1,
        0x2,
        0x2,
        0x3,
    },
    {
        0x2,
        0x1,
        0x1,
        0xA,
    },
    {
        0x4,
        0x83,
        0x1,
        0x16,
    },
    {
        0x4,
        0x99,
        0x1,
        0x15,
    },
    {
        0x1,
        0x26,
        0x1,
        0xC,
    },
    {
        0x1,
        0x3,
        0x2,
        0x3,
    },
    {
        0x1,
        0x59,
        0x2,
        0x3,
    },
    {
        0x2,
        0x2,
        0x1,
        0xA,
    },
    {
        0x4,
        0x84,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0x4,
        0x2,
        0x3,
    },
    {
        0x2,
        0x3,
        0x1,
        0xA,
    },
    {
        0x2,
        0x55,
        0x1,
        0xA,
    },
    {
        0x4,
        0x85,
        0x1,
        0x16,
    },
    {
        0x1,
        0x5,
        0x2,
        0x3,
    },
    {
        0x2,
        0x4,
        0x1,
        0xA,
    },
    {
        0x4,
        0x86,
        0x1,
        0x16,
    },
    {
        0x2,
        0x54,
        0x1,
        0x2,
    },
    {
        0x1,
        0x3D,
        0x1,
        0x2,
    },
    {
        0x3,
        0x9A,
        0x1,
        0x1D,
    },
    {
        0x1,
        0x6,
        0x2,
        0x3,
    },
    {
        0x1,
        0x1B,
        0x2,
        0x3,
    },
    {
        0x2,
        0x5,
        0x1,
        0xA,
    },
    {
        0x2,
        0x56,
        0x1,
        0xA,
    },
    {
        0x4,
        0x87,
        0x1,
        0x16,
    },
    {
        0x1,
        0x7,
        0x2,
        0x3,
    },
    {
        0x2,
        0x6,
        0x1,
        0xA,
    },
    {
        0x2,
        0x9B,
        0x1,
        0xA,
    },
    {
        0x4,
        0x88,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0x8,
        0x2,
        0x3,
    },
    {
        0x1,
        0x5A,
        0x2,
        0x3,
    },
    {
        0x2,
        0x7,
        0x1,
        0xA,
    },
    {
        0x2,
        0x57,
        0x1,
        0xA,
    },
    {
        0x4,
        0x89,
        0x1,
        0x16,
    },
    {
        0x1,
        0x9,
        0x2,
        0x3,
    },
    {
        0x2,
        0x8,
        0x1,
        0xA,
    },
    {
        0x4,
        0x8A,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0xA,
        0x2,
        0x3,
    },
    {
        0x1,
        0x5B,
        0x2,
        0x3,
    },
    {
        0x2,
        0x9,
        0x1,
        0xA,
    },
    {
        0x4,
        0x8B,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0xB,
        0x2,
        0x3,
    },
    {
        0x2,
        0xA,
        0x1,
        0xA,
    },
    {
        0x4,
        0x8C,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0xC,
        0x2,
        0x3,
    },
    {
        0x1,
        0x5C,
        0x2,
        0x3,
    },
    {
        0x2,
        0xB,
        0x1,
        0xA,
    },
    {
        0x4,
        0x8D,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0xD,
        0x2,
        0x3,
    },
    {
        0x1,
        0x5D,
        0x2,
        0x3,
    },
    {
        0x2,
        0xC,
        0x1,
        0xA,
    },
    {
        0x4,
        0x8E,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0xE,
        0x2,
        0x3,
    },
    {
        0x1,
        0x5E,
        0x2,
        0x3,
    },
    {
        0x2,
        0xD,
        0x1,
        0xA,
    },
    {
        0x4,
        0x8F,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0xF,
        0x2,
        0x3,
    },
    {
        0x1,
        0x5F,
        0x2,
        0x3,
    },
    {
        0x2,
        0xE,
        0x1,
        0xA,
    },
    {
        0x4,
        0x90,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0x10,
        0x2,
        0x3,
    },
    {
        0x1,
        0x60,
        0x2,
        0x3,
    },
    {
        0x2,
        0xF,
        0x1,
        0xA,
    },
    {
        0x4,
        0x91,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0x11,
        0x2,
        0x3,
    },
    {
        0x2,
        0x10,
        0x1,
        0xA,
    },
    {
        0x4,
        0x92,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0x12,
        0x2,
        0x3,
    },
    {
        0x2,
        0x11,
        0x1,
        0xA,
    },
    {
        0x4,
        0x93,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0x13,
        0x2,
        0x3,
    },
    {
        0x2,
        0x12,
        0x1,
        0xA,
    },
    {
        0x4,
        0x94,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x1,
        0x14,
        0x2,
        0x3,
    },
    {
        0x1,
        0x61,
        0x2,
        0x3,
    },
    {
        0x2,
        0x13,
        0x1,
        0xA,
    },
    {
        0x4,
        0x95,
        0x1,
        0x16,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x15,
        0x1,
        0x2,
    },
    {
        0x1,
        0x53,
        0x1,
        0x13,
    },
    {
        0x1,
        0x54,
        0x1,
        0x13,
    },
    {
        0x1,
        0x34,
        0x1,
        0x13,
    },
    {
        0x3,
        0x29,
        0x1,
        0x12,
    },
    {
        0x2,
        0x16,
        0x1,
        0x2,
    },
    {
        0x1,
        0x2C,
        0x1,
        0x13,
    },
    {
        0x3,
        0x2A,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x17,
        0x1,
        0x2,
    },
    {
        0x1,
        0x3A,
        0x1,
        0x13,
    },
    {
        0x3,
        0x2B,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x18,
        0x1,
        0x2,
    },
    {
        0x1,
        0x2D,
        0x1,
        0x13,
    },
    {
        0x3,
        0x2C,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x19,
        0x1,
        0x2,
    },
    {
        0x1,
        0x32,
        0x1,
        0x13,
    },
    {
        0x3,
        0x2D,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x1A,
        0x1,
        0x2,
    },
    {
        0x1,
        0x2A,
        0x1,
        0x13,
    },
    {
        0x3,
        0x2E,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x1B,
        0x1,
        0x2,
    },
    {
        0x1,
        0x2F,
        0x1,
        0x13,
    },
    {
        0x3,
        0x2F,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x1C,
        0x1,
        0x2,
    },
    {
        0x1,
        0x35,
        0x1,
        0x13,
    },
    {
        0x3,
        0x30,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x1D,
        0x1,
        0x2,
    },
    {
        0x1,
        0x2B,
        0x1,
        0x13,
    },
    {
        0x3,
        0x31,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x1E,
        0x1,
        0x2,
    },
    {
        0x1,
        0x30,
        0x1,
        0x13,
    },
    {
        0x3,
        0x32,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x1F,
        0x1,
        0x2,
    },
    {
        0x1,
        0x55,
        0x1,
        0x13,
    },
    {
        0x3,
        0x33,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x20,
        0x1,
        0x2,
    },
    {
        0x1,
        0x56,
        0x1,
        0x13,
    },
    {
        0x3,
        0x34,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x58,
        0x1,
        0x2,
    },
    {
        0x1,
        0x57,
        0x1,
        0x13,
    },
    {
        0x3,
        0x35,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x21,
        0x1,
        0x2,
    },
    {
        0x1,
        0x31,
        0x1,
        0x13,
    },
    {
        0x3,
        0x36,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x22,
        0x1,
        0x2,
    },
    {
        0x1,
        0x2E,
        0x1,
        0x13,
    },
    {
        0x3,
        0x37,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x23,
        0x1,
        0x2,
    },
    {
        0x1,
        0x38,
        0x1,
        0x13,
    },
    {
        0x3,
        0x38,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x24,
        0x1,
        0x2,
    },
    {
        0x1,
        0x33,
        0x1,
        0x13,
    },
    {
        0x3,
        0x39,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x20,
        0x1,
        0x2,
    },
    {
        0x1,
        0x56,
        0x1,
        0x13,
    },
    {
        0x3,
        0x34,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x25,
        0x1,
        0x2,
    },
    {
        0x1,
        0x58,
        0x1,
        0x13,
    },
    {
        0x3,
        0x3B,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x26,
        0x1,
        0x2,
    },
    {
        0x1,
        0x37,
        0x1,
        0x13,
    },
    {
        0x3,
        0x3C,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x27,
        0x1,
        0x2,
    },
    {
        0x1,
        0x36,
        0x1,
        0x13,
    },
    {
        0x1,
        0x22,
        0x2,
        0x13,
    },
    {
        0x3,
        0x3D,
        0x1,
        0x12,
    },
    {
        0x1,
        0x21,
        0x2,
        0x13,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x28,
        0x1,
        0x2,
    },
    {
        0x1,
        0x3B,
        0x1,
        0x13,
    },
    {
        0x1,
        0x22,
        0x2,
        0x13,
    },
    {
        0x3,
        0x3E,
        0x1,
        0x12,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x0,
        0x0,
        0x0,
        0x0,
    },
    {
        0x2,
        0x40,
        0x1,
        0x2,
    },
    {
        0x2,
        0x41,
        0x1,
        0x2,
    },
    {
        0x2,
        0x42,
        0x1,
        0x2,
    },
    {
        0x2,
        0x43,
        0x1,
        0x2,
    },
    {
        0x2,
        0x44,
        0x1,
        0x2,
    },
    {
        0x2,
        0x45,
        0x1,
        0x2,
    },
    {
        0x2,
        0x46,
        0x1,
        0x2,
    },
    {
        0x2,
        0x47,
        0x1,
        0x2,
    },
    {
        0x2,
        0x48,
        0x1,
        0x2,
    },
    {
        0x2,
        0x49,
        0x1,
        0x2,
    },
    {
        0x2,
        0x4A,
        0x1,
        0x2,
    },
    {
        0x2,
        0x4B,
        0x1,
        0x2,
    },
    {
        0x2,
        0x4C,
        0x1,
        0x2,
    },
    {
        0x2,
        0x4D,
        0x1,
        0x2,
    },
    {
        0x2,
        0x4E,
        0x1,
        0x2,
    },
    {
        0x2,
        0x4F,
        0x1,
        0x2,
    },
    {
        0x2,
        0x50,
        0x1,
        0x2,
    },
    {
        0x2,
        0x51,
        0x1,
        0x2,
    },
    {
        0x2,
        0x52,
        0x1,
        0x2,
    },
    {
        0x2,
        0x53,
        0x1,
        0x2,
    },
    {
        0x4,
        0x6E,
        0x1,
        0x16,
    },
    {
        0x4,
        0x6F,
        0x1,
        0x16,
    },
    {
        0x4,
        0x70,
        0x1,
        0x16,
    },
    {
        0x4,
        0x71,
        0x1,
        0x16,
    },
    {
        0x4,
        0x72,
        0x1,
        0x16,
    },
    {
        0x4,
        0x73,
        0x1,
        0x16,
    },
    {
        0x4,
        0x74,
        0x1,
        0x16,
    },
    {
        0x4,
        0x75,
        0x1,
        0x16,
    },
    {
        0x4,
        0x76,
        0x1,
        0x16,
    },
    {
        0x4,
        0x77,
        0x1,
        0x16,
    },
    {
        0x4,
        0x78,
        0x1,
        0x16,
    },
    {
        0x4,
        0x79,
        0x1,
        0x16,
    },
    {
        0x4,
        0x7A,
        0x1,
        0x16,
    },
    {
        0x4,
        0x7B,
        0x1,
        0x16,
    },
    {
        0x4,
        0x7C,
        0x1,
        0x16,
    },
    {
        0x4,
        0x7D,
        0x1,
        0x16,
    },
    {
        0x4,
        0x7E,
        0x1,
        0x16,
    },
    {
        0x4,
        0x7F,
        0x1,
        0x16,
    },
    {
        0x4,
        0x80,
        0x1,
        0x16,
    },
    {
        0x4,
        0x81,
        0x1,
        0x16,
    },
    {
        0x4,
        0x82,
        0x1,
        0x16,
    },
    {
        0x4,
        0x83,
        0x1,
        0x16,
    },
    {
        0x4,
        0x84,
        0x1,
        0x16,
    },
    {
        0x4,
        0x85,
        0x1,
        0x16,
    },
    {
        0x4,
        0x86,
        0x1,
        0x16,
    },
    {
        0x4,
        0x87,
        0x1,
        0x16,
    },
    {
        0x4,
        0x88,
        0x1,
        0x16,
    },
    {
        0x4,
        0x89,
        0x1,
        0x16,
    },
    {
        0x4,
        0x8A,
        0x1,
        0x16,
    },
    {
        0x4,
        0x8B,
        0x1,
        0x16,
    },
    {
        0x4,
        0x82,
        0x1,
        0x16,
    },
    {
        0x4,
        0x83,
        0x1,
        0x16,
    },
    {
        0x4,
        0x84,
        0x1,
        0x16,
    },
    {
        0x4,
        0x85,
        0x1,
        0x16,
    },
    {
        0x4,
        0x86,
        0x1,
        0x16,
    },
    {
        0x4,
        0x87,
        0x1,
        0x16,
    },
    {
        0x4,
        0x88,
        0x1,
        0x16,
    },
    {
        0x4,
        0x89,
        0x1,
        0x16,
    },
    {
        0x4,
        0x8A,
        0x1,
        0x16,
    },
    {
        0x4,
        0x8B,
        0x1,
        0x16,
    },
    {
        0x2,
        0x0,
        0x1,
        0xA,
    },
    {
        0x2,
        0x1,
        0x1,
        0xA,
    },
    {
        0x2,
        0x2,
        0x1,
        0xA,
    },
    {
        0x2,
        0x3,
        0x1,
        0xA,
    },
    {
        0x2,
        0x4,
        0x1,
        0xA,
    },
    {
        0x2,
        0x5,
        0x1,
        0xA,
    },
    {
        0x2,
        0x6,
        0x1,
        0xA,
    },
    {
        0x2,
        0x7,
        0x1,
        0xA,
    },
    {
        0x2,
        0x8,
        0x1,
        0xA,
    },
    {
        0x2,
        0x9,
        0x1,
        0xA,
    },
    {
        0x2,
        0xA,
        0x1,
        0xA,
    },
    {
        0x2,
        0xB,
        0x1,
        0xA,
    },
    {
        0x2,
        0xC,
        0x1,
        0xA,
    },
    {
        0x2,
        0xD,
        0x1,
        0xA,
    },
    {
        0x2,
        0xE,
        0x1,
        0xA,
    },
    {
        0x2,
        0xF,
        0x1,
        0xA,
    },
    {
        0x2,
        0x10,
        0x1,
        0xA,
    },
    {
        0x2,
        0x11,
        0x1,
        0xA,
    },
    {
        0x2,
        0x12,
        0x1,
        0xA,
    },
    {
        0x2,
        0x13,
        0x1,
        0xA,
    },
    {
        0x5,
        0x97,
        0x2,
        0x19,
    },
    {
        0x5,
        0x98,
        0x2,
        0x1A,
    },
};

const s16 ldreq_ix[43][2] = {
    // size: 0xAC, address: 0x520320
    { 0x0000, 0x0005 }, { 0x0005, 0x0003 }, { 0x000A, 0x0004 }, { 0x000F, 0x0004 }, { 0x0013, 0x0003 },
    { 0x0019, 0x0005 }, { 0x001E, 0x0004 }, { 0x0023, 0x0005 }, { 0x0028, 0x0003 }, { 0x002D, 0x0004 },
    { 0x0032, 0x0003 }, { 0x0037, 0x0004 }, { 0x003C, 0x0004 }, { 0x0041, 0x0004 }, { 0x0046, 0x0004 },
    { 0x004B, 0x0004 }, { 0x0050, 0x0003 }, { 0x0055, 0x0003 }, { 0x005A, 0x0003 }, { 0x005F, 0x0004 },
    { 0x0064, 0x0005 }, { 0x0069, 0x0003 }, { 0x006E, 0x0003 }, { 0x0073, 0x0003 }, { 0x0078, 0x0003 },
    { 0x007D, 0x0003 }, { 0x0082, 0x0003 }, { 0x0087, 0x0003 }, { 0x008C, 0x0003 }, { 0x0091, 0x0003 },
    { 0x0096, 0x0003 }, { 0x009B, 0x0003 }, { 0x00A0, 0x0003 }, { 0x00A5, 0x0003 }, { 0x00AA, 0x0003 },
    { 0x00AF, 0x0003 }, { 0x00B4, 0x0003 }, { 0x00B9, 0x0003 }, { 0x00BE, 0x0003 }, { 0x00C3, 0x0003 },
    { 0x00C8, 0x0005 }, { 0x00CE, 0x0004 }, { 0x0016, 0x0003 },
};
