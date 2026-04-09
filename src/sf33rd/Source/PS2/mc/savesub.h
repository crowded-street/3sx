#ifndef SAVESUB_H
#define SAVESUB_H

#include "sf33rd/Source/PS2/mc/mcsub.h"
#include "structs.h"
#include "types.h"

#include "port/io/afs.h"

typedef struct _sub_info {
    struct memcard_date date;
    s32 player[2];
} _sub_info;

typedef struct _save_work {
    s8 r_no_0;
    s8 r_no_1;
    s8 r_no_2;
    s8 r_no_3;
    s8 r_no_4;
    s8 r_no_5;
    s8 r_no_6;
    s8 r_no_n;
    s16 cnt_0;
    s16 cnt_1;
    s16 cnt_2;
    s16 cnt_3;
    u16 sw;
    u16 tr;
    u16 rp;
    u16 pad;
    u8 lv1st[2];
    u8 lvctr[2];
    s8 return_code;
    u8 success_flag;
    u8 file_type;
    u8 file_no;
    u8 save_mode;
    u8 yes_no_flag;
    u8 cursor_sign;
    u8 ask_save_flag;
    u8 auto_save_old;
    s32 ram_key;
    u8* fnt_adrs;
    u8* buf_adrs;
    u8* buf_adrs0;
    u8* ico_adrs;
    u8* dat_adrs;
    u8* exp_adrs;
    AFSHandle afs_handle;
    s32 avail_size;
    s8 sel_slot_no;
    s8 sel_slot_max;
    s8 sel_slot_st;
    s8 sel_slot_old;
    s8 sel_slot_cnt;
    s8 sel_slot_free[3];
    s8 sel_file_top;
    s8 sel_file_idx;
    s8 sel_file_max;
    s8 sel_file_max0;
    s8 sel_file_no;
    s8 sel_file_st;
    s8 sel_file_cnt;
    s8 sel_file_cnt2;
    s8 sel_file_sort;
    s8 sel_file_free[3];
    u8 sel_file_order[20];
    struct _sub_info info[20];
    struct _sub_info info_new;
    struct memcard_date auto_date[2];
    s32 mc_result;
    u8 mc_msg_st;
    u8 mc_msg_no;
    s8 mc_sum_err;
    s8 mc_ver_err;
    u8 al_no_err;
    u8 al_no_card;
    u8 al_no_file;
    u8 al_no_space;
    u8 al_error;
    s8 mc_ra_cnt;
    s8 mc_yn_cnt;
    s8 mc_yn_ok;
} _save_work;

typedef struct _head_save {
    u16 version;
    u16 magic;
    u16 sum;
    u16 reserved;
} _head_save;

typedef struct _save_data {
    struct _head_save head;
    struct memcard_date date;
    struct _SAVE_W save_w;
} _save_data;

typedef struct _sdir_data {
    struct _head_save head;
    struct memcard_date date;
    SystemDir sdir_w;
} _sdir_data;

typedef struct _replay_data {
    struct _head_save head;
    struct memcard_date date;
    _REPLAY_W replay_w;
} _replay_data;

void SaveInit(s32 file_type, s32 save_mode);
s32 SaveMove();

#endif
