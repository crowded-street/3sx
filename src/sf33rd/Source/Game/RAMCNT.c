#include "sf33rd/Source/Game/RAMCNT.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/texgroup.h"

#define ERR_STOP                                                                                                       \
    while (1) {}

RCKeyWork rckey_work[RCKEY_WORK_MAX];
_MEMMAN_OBJ rckey_mmobj;
s16 rckeyque[RCKEY_WORK_MAX];
s16 rckeyctr;
s16 rckeymin;

void disp_ramcnt_free_area() {
    if (Debug_w[0xA]) {
        flPrintColor(0xFFFFFF8F);
        flPrintL(4, 8, "Ramcnt Status");
        flPrintL(4, 9, "Now %07X", mmGetRemainder(&rckey_mmobj));
        flPrintL(4, 0xA, "Min %07X", mmGetRemainderMin(&rckey_mmobj));
        flPrintL(4, 0xB, "Key %2d / %2d", rckeymin, rckeyctr);
    }
}

void Init_ram_control_work(u8* adrs, s32 size) {
    s16 i;

    mmHeapInitialize(&rckey_mmobj, adrs, size, ALIGN_UP(sizeof(_MEMMAN_CELL), 64), "- for Ramcnt -");

    for (i = 0; i < (RCKEY_WORK_MAX - 1); i++) {
        rckeyque[i] = ((RCKEY_WORK_MAX - 1) - i);
    }

    rckeyctr = (RCKEY_WORK_MAX - 1);
    rckeymin = (RCKEY_WORK_MAX - 1);
    rckeyque[rckeyctr] = 0;

    for (i = 0; i < sizeof(RCKeyWork) / 4; i++) {
        ((s32*)rckey_work)[i] = 0;
    }

    for (i = 1; i < RCKEY_WORK_MAX; i++) {
        rckey_work[i] = rckey_work[0];
    }
}

void Push_ramcnt_key(s16 key) {
    RCKeyWork* rwk = &rckey_work[key];

    if (rwk->use != 0) {
        if ((rwk->type == 8) || (rwk->type == 9)) {
            flLogOut("TEXCASH KEY PUSH ERROR\n");
            ERR_STOP;
        }

        Push_ramcnt_key_original_2(key);
    }

    return;
}

void Push_ramcnt_key_original(s16 key) {
    RCKeyWork* rwk = &rckey_work[key];

    if (rwk->use != 0) {
        if ((rwk->type != 8) && (rwk->type != 9)) {
            flLogOut("TEXCASH KEY PUSH ERROR2\n");
            ERR_STOP;
        }

        Push_ramcnt_key_original_2(key);
    }

    return;
}

void Push_ramcnt_key_original_2(s16 key) {
#if defined(TARGET_PS2)
    void purge_texture_group(u16 grp);
#endif

    RCKeyWork* rwk = &rckey_work[key];

    if (rwk->use != 0) {
        mmFree(&rckey_mmobj, (u8*)rwk->adr);
        rwk->type = 0;
        rwk->use = 0;

        if (rwk->group_num) {
            purge_texture_group(rwk->group_num);
        }

        rwk->group_num = 0;
        rckeyque[rckeyctr++] = key;
    }
}

void Purge_memory_of_kind_of_key(u8 kokey) {
    RCKeyWork* rwk;
    s16 i;

    for (i = 0; i < RCKEY_WORK_MAX; i++) {
        rwk = &rckey_work[i];

        if (rwk->use && (rwk->type == kokey)) {
            Push_ramcnt_key(i);
        }
    }
}

void Set_size_data_ramcnt_key(s16 key, u32 size) {
    if (key <= 0) {
        // An attempt was made to store a file size in an unused memory key.\n
        flLogOut("未使用のメモリキーへファイルサイズを格納しようとしました。\n");
        ERR_STOP;
    }

    rckey_work[key].size = size;
}

size_t Get_size_data_ramcnt_key(s16 key) {
    if (key <= 0) {
        // An attempt was made to get a file size from an unused memory key.\n
        flLogOut("未使用のメモリキーからファイルサイズを取得しようとしました。\n");
        ERR_STOP;
    }

    return rckey_work[key].size;
}

uintptr_t Get_ramcnt_address(s16 key) {
    if (key <= 0) {
        // An attempt was made to obtain an address from an unused memory key.\n
        flLogOut("未使用のメモリキーからアドレスを取得しようとしました。\n");
        ERR_STOP;
    }

    return rckey_work[key].adr;
}

s16 Search_ramcnt_type(u8 kokey) {
    s16 i;

    for (i = 1; i < RCKEY_WORK_MAX; i++) {
        if ((rckey_work[i].use) && (kokey == (rckey_work[i].type))) {
            return i;
        }
    }

    return 0;
}

s32 Test_ramcnt_key(s16 key) {
    if (key == 0) {
        return 1;
    }

    if (rckey_work[key].use == 0) {
        return 0;
    }

    if (rckey_work[key].group_num != 0) {
        return 0;
    }

    return 1;
}

s16 Pull_ramcnt_key(size_t memreq, u8 kokey, u8 group, u8 frre) {
    RCKeyWork* rwk;
    s16 key;

    if (rckeyctr <= 0) {
        // There are not enough memory keys.\n
        flLogOut("メモリキーの個数が足りなくなりました。\n");
        ERR_STOP;
    }

    key = rckeyque[(rckeyctr -= 1)];
    rwk = &rckey_work[key];

    if (rckeyctr < rckeymin) {
        rckeymin = rckeyctr;
    }

    if (memreq != 0) {
        rwk->size = memreq;

        if (frre != 0) {
            frre--;
        }

        rwk->adr = (uintptr_t)mmAlloc(&rckey_mmobj, memreq, frre);
    } else {
        goto err;
    }

    if (rwk->adr == 0) {
    err:
        rckeyque[rckeyctr++] = key;
        // Failed to allocate memory.\n
        flLogOut("メモリの確保に失敗しました。\n");
        ERR_STOP;
    }

    rwk->use = 1;
    rwk->type = kokey;
    rwk->group_num = group;
    return key;
}
