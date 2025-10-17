#ifndef MCSUB_H
#define MCSUB_H

#include "types.h"

typedef struct _file {
    s32 flag;
    s8* name;
    intptr_t bufs;
    s32 size;
} _file;

typedef struct _memcard_file {
    s8* title1;
    s8* title2;
    struct _file file[6];
    s32 fnum_flag;
    s32 copy_flag;
    s32 req_clust;
} _memcard_file;

typedef struct memcard_work {
    s32 r_no_0;
    s32 r_no_1;
    s32 cnt_0;
    s32 cnt_1;
    s32 max_port;
    s32 port;
    s32 cmds;
    s32 rslt;
    s32 retry;
    s32 result;
    s32 info_type;
    s32 info_free;
    s32 info_format;
    s32 stat[3];
    s32 free[3];
    u32 new;
    s8 dir[34];
    s8 path[66];
    s32 fd;
    s32 flag;
    s32 size;
    s32 attr;
    s32 mode;
    s32 copy;
    s32 act_no;
    void* bufs;
    s32 file_type;
    s32 file_no;
    void* icon;
    s32 exe_flag;
} memcard_work;

typedef struct memcard_date {
    u8 dayofweek;
    u8 sec;
    u8 min;
    u8 hour;
    u8 day;
    u8 month;
    u16 year;
} memcard_date;

void MemcardInit();
void* McActIconTexAdrs(s32 file_type, s32 num);
s32 McActAvailSet(s32* ico);
s32 McActResult();
void McActCheckSet();
void McActMain();
void McActInit(s32 file_type, s32 file_no);
void McActSave0Set(s32 port, void* bufs, s32 mode);
void McActExistSet(s32 port, void* bufs);
s32 McActConChk(s32 port);
void McActFormatSet(s32 port);
void McActSaveSet(s32 port, void* bufs);
void McActNewClr();
void McActLoadSet(s32 port, void* bufs);
void McActLastDate(memcard_date* date);
s32 McActNewChk(s32 port);
void McActZenNum(s32 num, s8* buf, s32 mode, s32 max);

#endif
