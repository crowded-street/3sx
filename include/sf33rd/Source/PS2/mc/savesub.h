#ifndef SAVESUB_H
#define SAVESUB_H

#include "sf33rd/Source/PS2/mc/mcsub.h"
#include "structs.h"
#include "types.h"

#include "port/io/afs.h"

typedef struct _sub_info {
    // total size: 0x10
    struct memcard_date date; // offset 0x0, size 0x8
    s32 player[2];            // offset 0x8, size 0x8
} _sub_info;

typedef struct _save_work {
    // total size: 0x1E4
    s8 r_no_0;                        // offset 0x0, size 0x1
    s8 r_no_1;                        // offset 0x1, size 0x1
    s8 r_no_2;                        // offset 0x2, size 0x1
    s8 r_no_3;                        // offset 0x3, size 0x1
    s8 r_no_4;                        // offset 0x4, size 0x1
    s8 r_no_5;                        // offset 0x5, size 0x1
    s8 r_no_6;                        // offset 0x6, size 0x1
    s8 r_no_n;                        // offset 0x7, size 0x1
    s16 cnt_0;                        // offset 0x8, size 0x2
    s16 cnt_1;                        // offset 0xA, size 0x2
    s16 cnt_2;                        // offset 0xC, size 0x2
    s16 cnt_3;                        // offset 0xE, size 0x2
    u16 sw;                           // offset 0x10, size 0x2
    u16 tr;                           // offset 0x12, size 0x2
    u16 rp;                           // offset 0x14, size 0x2
    u16 pad;                          // offset 0x16, size 0x2
    u8 lv1st[2];                      // offset 0x18, size 0x2
    u8 lvctr[2];                      // offset 0x1A, size 0x2
    s8 return_code;                   // offset 0x1C, size 0x1
    u8 success_flag;                  // offset 0x1D, size 0x1
    u8 file_type;                     // offset 0x1E, size 0x1
    u8 file_no;                       // offset 0x1F, size 0x1
    u8 save_mode;                     // offset 0x20, size 0x1
    u8 yes_no_flag;                   // offset 0x21, size 0x1
    u8 cursor_sign;                   // offset 0x22, size 0x1
    u8 ask_save_flag;                 // offset 0x23, size 0x1
    u8 auto_save_old;                 // offset 0x24, size 0x1
    s32 ram_key;                      // offset 0x28, size 0x4
    u8* fnt_adrs;                     // offset 0x2C, size 0x4
    u8* buf_adrs;                     // offset 0x30, size 0x4
    u8* buf_adrs0;                    // offset 0x34, size 0x4
    u8* ico_adrs;                     // offset 0x38, size 0x4
    u8* dat_adrs;                     // offset 0x3C, size 0x4
    u8* exp_adrs;                     // offset 0x40, size 0x4
    AFSHandle afs_handle;             // offset 0x44, size 0x4
    s32 avail_size;                   // offset 0x48, size 0x4
    s8 sel_slot_no;                   // offset 0x4C, size 0x1
    s8 sel_slot_max;                  // offset 0x4D, size 0x1
    s8 sel_slot_st;                   // offset 0x4E, size 0x1
    s8 sel_slot_old;                  // offset 0x4F, size 0x1
    s8 sel_slot_cnt;                  // offset 0x50, size 0x1
    s8 sel_slot_free[3];              // offset 0x51, size 0x3
    s8 sel_file_top;                  // offset 0x54, size 0x1
    s8 sel_file_idx;                  // offset 0x55, size 0x1
    s8 sel_file_max;                  // offset 0x56, size 0x1
    s8 sel_file_max0;                 // offset 0x57, size 0x1
    s8 sel_file_no;                   // offset 0x58, size 0x1
    s8 sel_file_st;                   // offset 0x59, size 0x1
    s8 sel_file_cnt;                  // offset 0x5A, size 0x1
    s8 sel_file_cnt2;                 // offset 0x5B, size 0x1
    s8 sel_file_sort;                 // offset 0x5C, size 0x1
    s8 sel_file_free[3];              // offset 0x5D, size 0x3
    u8 sel_file_order[20];            // offset 0x60, size 0x14
    struct _sub_info info[20];        // offset 0x74, size 0x140
    struct _sub_info info_new;        // offset 0x1B4, size 0x10
    struct memcard_date auto_date[2]; // offset 0x1C4, size 0x10
    s32 mc_result;                    // offset 0x1D4, size 0x4
    u8 mc_msg_st;                     // offset 0x1D8, size 0x1
    u8 mc_msg_no;                     // offset 0x1D9, size 0x1
    s8 mc_sum_err;                    // offset 0x1DA, size 0x1
    s8 mc_ver_err;                    // offset 0x1DB, size 0x1
    u8 al_no_err;                     // offset 0x1DC, size 0x1
    u8 al_no_card;                    // offset 0x1DD, size 0x1
    u8 al_no_file;                    // offset 0x1DE, size 0x1
    u8 al_no_space;                   // offset 0x1DF, size 0x1
    u8 al_error;                      // offset 0x1E0, size 0x1
    s8 mc_ra_cnt;                     // offset 0x1E1, size 0x1
    s8 mc_yn_cnt;                     // offset 0x1E2, size 0x1
    s8 mc_yn_ok;                      // offset 0x1E3, size 0x1
} _save_work;

typedef struct _head_save {
    // total size: 0x8
    u16 version;  // offset 0x0, size 0x2
    u16 magic;    // offset 0x2, size 0x2
    u16 sum;      // offset 0x4, size 0x2
    u16 reserved; // offset 0x6, size 0x2
} _head_save;

typedef struct _save_data {
    // total size: 0x218
    struct _head_save head;   // offset 0x0, size 0x8
    struct memcard_date date; // offset 0x8, size 0x8
    struct _SAVE_W save_w;    // offset 0x10, size 0x208
} _save_data;

typedef struct _sdir_data {
    // total size: 0x58
    struct _head_save head;   // offset 0x0, size 0x8
    struct memcard_date date; // offset 0x8, size 0x8
    SystemDir sdir_w;         // offset 0x10, size 0x48
} _sdir_data;

typedef struct _replay_data {
    // total size: 0x7458
    struct _head_save head;   // offset 0x0, size 0x8
    struct memcard_date date; // offset 0x8, size 0x8
    _REPLAY_W replay_w;       // offset 0x10, size 0x7448
} _replay_data;

void SaveInit(s32 file_type, s32 save_mode);
s32 SaveMove();

#endif
