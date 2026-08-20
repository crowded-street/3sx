#ifndef GD3RD_H
#define GD3RD_H

#include "constants.h"
#include "types.h"

#include <stdbool.h>

typedef struct {
    s8 ok;
    s8 type;
    s16 key;
    uintptr_t texture_table;
    uintptr_t trans_table;
} TEX_GRP_LD;

typedef struct LoadRequest {
    u8 be; /// Flag that's set if the load request is active
    u8 type;
    s16 id;
    u8 rno;
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
} LoadRequest;

/// Per-player character data requests
extern Character plt_req[2];

extern const u8 lpr_wrdata[3];
extern const u8 lpt_seldat[4];

bool fsOpen(u16 fnum);
void fsClose();
u32 fsGetFileSize(u16 fnum);
bool fsCheckCommandExecuting();
s32 fsRequestFileRead(void* buff);
s32 fsCheckFileReaded();
bool fsFileReadSync(void* buff);

s16 load_it_use_any_key(u16 fnum, u8 kokey, u8 group);
s32 load_it_use_any_key2(u16 fnum, void** adrs, s16* key, u8 kokey, u8 group);
bool load_it_use_this_key(u16 fnum, s16 key);

void Init_Load_Request_Queue();
void Request_LDREQ_Break();
bool Check_LDREQ_Break();

/// Push load requests for character data
/// @param id Player ID
/// @param character Character to load data for
void Push_LDREQ_Queue_Player(u8 id, Character character);

/// Check if all load requests for player have been processed
/// @param id Player ID
bool Check_LDREQ_Queue_Player(u8 id);

/// Process load request queue
void Check_LDREQ_Queue();

/// Check if load request queue is empty
bool Check_LDREQ_Clear();

/// Push a single load request
/// @param ix Index of load request info
/// @param id Request id
void Push_LDREQ_Queue_Direct(s16 ix, s16 id);

bool Check_LDREQ_Queue_Direct(s16 ix);

void Push_LDREQ_Queue_BG(s16 ix);

/// Check if all load requests for background have been processed
/// @param ix Background index
bool Check_LDREQ_Queue_BG(s16 ix);

#endif
