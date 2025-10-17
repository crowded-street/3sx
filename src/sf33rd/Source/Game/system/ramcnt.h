#ifndef RAMCNT_H
#define RAMCNT_H

#include "structs.h"
#include "types.h"

#define RCKEY_WORK_MAX 64

extern s16 rckeyctr;
extern s16 rckeymin;
extern _MEMMAN_OBJ rckey_mmobj;
extern RCKeyWork rckey_work[RCKEY_WORK_MAX];
extern s16 rckeyque[RCKEY_WORK_MAX];

void disp_ramcnt_free_area();
void Init_ram_control_work(u8* adrs, s32 size);
void Push_ramcnt_key(s16 key);
void Push_ramcnt_key_original_2(s16 key);
void Purge_memory_of_kind_of_key(u8 kokey);
void Set_size_data_ramcnt_key(s16 key, u32 size);
uintptr_t Get_ramcnt_address(s16 key);
s16 Search_ramcnt_type(u8 kokey);
s16 Pull_ramcnt_key(size_t memreq, u8 kokey, u8 group, u8 frre);
size_t Get_size_data_ramcnt_key(s16 key);
s32 Test_ramcnt_key(s16 key);
void Push_ramcnt_key_original(s16 key);

#endif
