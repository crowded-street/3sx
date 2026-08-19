#ifndef GD3RD_H
#define GD3RD_H

#include "types.h"

typedef struct {
    s8 ok;
    s8 type;
    s16 key;
    uintptr_t texture_table;
    uintptr_t trans_table;
} TEX_GRP_LD;

typedef struct LoadRequest {
    u8 be;
    u8 type;
    s16 id;
    u8 rno;
    u8 retry;
    u8 ix;
    u8 frre;
    s16 key;
    u8 kokey;
    u8 group;
    u8* result;
    s32 size;
    s32 sect;
    u16 fnum;
    u8 free[2];
    TEX_GRP_LD* lds;
    struct {
        u32 number;
        u32 size;
    } info;
} LoadRequest;

extern s16 plt_req[2];
extern const u8 lpr_wrdata[3];
extern const u8 lpt_seldat[4];

s32 fsOpen(LoadRequest* req);
void fsClose(LoadRequest* /* unused */);
u32 fsGetFileSize(u16 fnum);
u32 fsCalSectorSize(u32 size);
s32 fsCheckCommandExecuting();
s32 fsRequestFileRead(LoadRequest* /* unused */, void* buff);
s32 fsCheckFileReaded(LoadRequest* /* unused */);
s32 fsFileReadSync(LoadRequest* req, void* buff);
void waitVsyncDummy();
s16 load_it_use_any_key(u16 fnum, u8 kokey, u8 group);
s32 load_it_use_any_key2(u16 fnum, void** adrs, s16* key, u8 kokey, u8 group);
s32 load_it_use_this_key(u16 fnum, s16 key);
void Init_Load_Request_Queue_1st();
void Request_LDREQ_Break();
u8 Check_LDREQ_Break();
void Push_LDREQ_Queue_Player(s16 id, s16 ix);
void Check_LDREQ_Queue();
s32 Check_LDREQ_Clear();
s32 Check_LDREQ_Queue_Player(s16 id);
void Push_LDREQ_Queue_Direct(s16 ix, s16 id);
void Push_LDREQ_Queue_Player(s16 id, s16 ix);
void Push_LDREQ_Queue_BG(s16 ix);
s32 Check_LDREQ_Queue_BG(s16 ix);
s32 Check_LDREQ_Queue_Direct(s16 ix);

#endif
