#include "sf33rd/Source/PS2/mc/mcsub.h"
#include "common.h"

#include <libmc.h>

#include <stdio.h>
#include <string.h>

typedef void (*act_jmp_ptr)(memcard_work*);

// forward decls
static void mc_act_stop(memcard_work* mw);
static void mc_act_return(memcard_work* mw);
static void mc_act_check(memcard_work* mw);
static void mc_act_exist(memcard_work* mw);
static void mc_act_load(memcard_work* mw);
static void mc_act_save0(memcard_work* mw);
static void mc_act_save(memcard_work* mw);
static void mc_act_format(memcard_work* mw);
static void mc_act_unformat(memcard_work* mw);
static void mc_act_delete(memcard_work* mw);
static void mc_act_remove(memcard_work* mw);
static void mc_act_list(memcard_work* mw);

static void day_of_week(memcard_date* md);
static void mc_icon_sys_set(memcard_work* mw);

static _memcard_file mc_file_game = {
    "ＳＦ３　３ｒｄ　ＳＴＲＩＫＥ",
    "ＳＹＳＴＥＭ",
    {
        { 1, "icon.sys", 0, 0x3C4 },
        { 2, "icon00.ico", 0, 0 },
        { 0, "icon00.ico", 0, 0 },
        { 0, "icon00.ico", 0, 0 },
        { 2, "BASLUS-20949SF3SYS", 0, 0x218 },
        { 0, NULL, 0, 0 },
    },
    0,
    0,
    0x7FFFFFFF,
};

static _memcard_file mc_file_sysdir = {
    "ＳＦ３　３ｒｄ　ＳＴＲＩＫＥ",
    "ＤＩＲＥＣＴＩＯＮ−",
    {
        { 1, "icon.sys", 0, 0x3C4 },
        { 1, "icon10.ico", 0, 0 },
        { 0, "icon10.ico", 0, 0 },
        { 0, "icon10.ico", 0, 0 },
        { 2, "BASLUS-20949SF3D", 0, 0x58 },
        { 0, "sub.bin", 0, 0 },
    },
    3,
    0,
    0x7FFFFFFF,
};

static _memcard_file mc_file_replay = {
    "ＳＦ３　３ｒｄ　ＳＴＲＩＫＥ",
    "ＲＥＰＬＡＹ−",
    {
        { 1, "icon.sys", 0, 0x3C4 },
        { 2, "icon20.ico", 0, 0 },
        { 0, "icon20.ico", 0, 0 },
        { 0, "icon20.ico", 0, 0 },
        { 2, "BASLUS-20949SF3R", 0, 0x7458 },
        { 2, "sub.bin", 0, 0x10 },
    },
    3,
    0,
    0x7FFFFFFF,
};

static _memcard_file* mc_file_tbl[3] = { &mc_file_game, &mc_file_sysdir, &mc_file_replay };

static act_jmp_ptr mc_act_jmp[11] = { mc_act_stop,   mc_act_check,  mc_act_exist,  mc_act_load,
                                      mc_act_save0,  mc_act_save,   mc_act_format, mc_act_unformat,
                                      mc_act_delete, mc_act_remove, mc_act_list };

// bss
memcard_work MemcardWork;

// sbss
sceMcTblGetDir mc_dir;
s8 mc_path[64];

void MemcardInit() {
    s32 ret;
    memcard_work* mw = &MemcardWork;

    memset(mw, 0, sizeof(MemcardWork));
    mw->max_port = 2;

    do {
        ret = sceMcInit();
        printf("McInit() = %d.\n", ret);
    } while (ret < 0);
}

static s32 mc_sync(memcard_work* mw) {
    s32 ret;

    mw->cmds = 0;
    mw->rslt = 0;

    ret = sceMcSync(1, &mw->cmds, &mw->rslt);
    return (ret == 0) ? -1 : 0;
}

static s32 mc_check_card(memcard_work* mw) {
    s32 s_stat;

    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        mw->r_no_1 += 1;
        mw->retry = 3;
        /* fallthrough */

    case 1:
        if (sceMcGetInfo(mw->port, 0, &mw->info_type, &mw->info_free, &mw->info_format) < 0) {
            if (--mw->retry > 0) {
                break;
            }

        block_9:
            mw->r_no_1 = 0;
            mw->stat[mw->port] = 0;
            mw->free[mw->port] = 0;
            return 0;
        }

        mw->r_no_1 += 1;
        break;

    case 2:
        if (mc_sync(mw) < 0) {
            break;
        }

        switch (mw->rslt) {
        case 0:
            s_stat = 1;

        block_19:
            if (mw->info_type != 2) {
                goto block_9;
            }

            if (mw->info_format == 0) {
                goto block_27;
            }

            mw->stat[mw->port] = s_stat;
            mw->free[mw->port] = mw->info_free;
            break;

        case -1:
            mw->new |= 1 << mw->port;
            s_stat = 2;
            goto block_19;

        case -2:
            if (mw->info_type != 2) {
                goto block_9;
            }

            mw->new |= 1 << mw->port;

        block_27:
            if (--mw->retry > 0) {
            block_28:
                mw->r_no_1 = 1;
                return -1;
            }

            mw->stat[mw->port] = 3;
            mw->free[mw->port] = 0x1F40;
            break;

        default:
            if (--mw->retry > 0) {
                goto block_28;
            }

            goto block_9;
        }

        mw->r_no_1 = 0;
        return mw->stat[mw->port];
    }

    return -1;
}

s32 mc_check_file(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcGetDir(mw->port, 0, mw->path, 0, 1, &mc_dir) < 0) {
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McGetDir(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        if (mw->rslt == 1) {
            printf(" size = %d / %d.\n", mw->size, mc_dir.FileSizeByte);
        }

        mw->r_no_1 = 0;

        if (mw->rslt == 1) {
            return 0;
        }

        if ((mw->rslt == -5) || (mw->rslt < -0xA)) {
            return 2;
        }

        if (mw->rslt == -6) {
            return 3;
        }

        return 1;
    }

    return -1;
}

static s32 mc_read_file(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcOpen(mw->port, 0, mw->path, 1) < 0) {
        block_9:
            mw->r_no_1 = 0;
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McOpen(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        if (mw->rslt < 0) {
            goto block_9;
        }

        mw->fd = mw->rslt;

        if (sceMcRead(mw->fd, mw->bufs, mw->size) < 0) {
            goto block_9;
        }

        mw->r_no_1 += 1;
        break;

    case 2:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McRead(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        if (mw->rslt < 0) {
            goto block_9;
        }

        if (sceMcClose(mw->fd) < 0) {
            goto block_9;
        }

        mw->r_no_1 += 1;
        break;

    case 3:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McClose(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        mw->r_no_1 = 0;
        return mw->rslt < 0 ? 1 : 0;
    }

    return -1;
}

static s32 mc_mkdir(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcMkdir(mw->port, 0, mw->path) < 0) {
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McMkdir(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        if (mw->rslt == -4) {
            mw->rslt = 0;
        }

        mw->r_no_1 = 0;
        return mw->rslt < 0 ? 1 : 0;
    }

    return -1;
}

static s32 mc_create_file(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcOpen(mw->port, 0, mw->path, 0x200) < 0) {
        block_7:
            mw->r_no_1 = 0;
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McOpen(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        if (mw->rslt < 0) {
            goto block_7;
        }

        if (sceMcClose(mw->fd) < 0) {
            goto block_7;
        }

        mw->r_no_1 += 1;
        break;

    case 2:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McClose(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        mw->r_no_1 = 0;
        return mw->rslt < 0 ? 1 : 0;
    }

    return -1;
}

static s32 mc_write_file(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcOpen(mw->port, 0, mw->path, 2) < 0) {
        block_9:
            mw->r_no_1 = 0;
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McOpen(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        if (mw->rslt < 0) {
            goto block_9;
        }

        mw->fd = mw->rslt;

        if (sceMcWrite(mw->fd, mw->bufs, mw->size) < 0) {
            goto block_9;
        }

        mw->r_no_1 += 1;
        break;

    case 2:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McWrite(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        if (mw->rslt < 0) {
            goto block_9;
        }

        if (sceMcClose(mw->fd) < 0) {
            goto block_9;
        }

        mw->r_no_1 += 1;
        break;

    case 3:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McClose(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        mw->r_no_1 = 0;
        return mw->rslt < 0 ? 1 : 0;
    }

    return -1;
}

static s32 mc_delete_file(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcDelete(mw->port, 0, mw->path) < 0) {
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McDelete(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        mw->r_no_1 = 0;
        return mw->rslt < 0 ? 1 : 0;
    }

    return -1;
}

static s32 mc_format(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcFormat(mw->port, 0) < 0) {
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McFormat(%d) = %d.\n", mw->port, mw->rslt);

        mw->r_no_1 = 0;
        return mw->rslt < 0 ? 1 : 0;
    }

    return -1;
}

static s32 mc_unformat(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcUnformat(mw->port, 0) < 0) {
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McUnformat(%d) = %d.\n", mw->port, mw->rslt);

        mw->r_no_1 = 0;
        return mw->rslt < 0 ? 1 : 0;
    }

    return -1;
}

static s32 mc_delete_dir(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (mw->attr == 0) {
            goto block_18;
        }

        mw->cnt_1 = 0;

    block_8:
        sprintf(mc_path, "%s/*", mw->path);
        mw->r_no_1 = 1;

    case 1:
        if (sceMcGetDir(mw->port, 0, mc_path, mw->cnt_1, 1, &mc_dir) < 0) {
        block_10:
            mw->r_no_1 = 0;
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 2:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McGetDir(%d,%s) = %d.\n", mw->port, mc_path, mw->rslt);

        if (mw->rslt < 0) {
            goto block_10;
        }

        if (mw->rslt != 1) {
            goto block_18;
        }

        mw->cnt_1 += 1;

        if (mc_dir.EntryName[0] == '.') {
            goto block_8;
        }

        sprintf(mc_path, "%s/%s", mw->path, mc_dir.EntryName);
        mw->mode = 0;
        goto block_19;

    block_18:
        sprintf(mc_path, "%s", mw->path);
        mw->mode = 1;

    block_19:
        if (sceMcDelete(mw->port, 0, mc_path) < 0) {
            goto block_10;
        }

        mw->r_no_1 = 3;
        break;

    case 3:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McDelete(%d,%s) = %d.\n", mw->port, mc_path, mw->rslt);

        if (mw->rslt < 0) {
            goto block_10;
        }

        mw->r_no_1 = 0;

        if (mw->mode == 0) {
            goto block_8;
        }

        return 0;
    }

    return -1;
}

static s32 mc_get_dir(memcard_work* mw) {
    switch (mw->r_no_1) {
    case 0:
        if (mc_sync(mw) < 0) {
            break;
        }

        if (sceMcGetDir(mw->port, 0, mw->path, 0, 0x100, mw->bufs) < 0) {
            return 1;
        }

        mw->r_no_1 += 1;
        break;

    case 1:
        if (mc_sync(mw) < 0) {
            break;
        }

        printf("McGetDir(%d,%s) = %d.\n", mw->port, mw->path, mw->rslt);

        mw->r_no_1 = 0;

        if (mw->rslt < 0) {
            mw->cnt_1 = 0;
            return 2;
        }

        mw->cnt_1 = mw->rslt;
        return 0;
    }

    return -1;
}

void McActInit(s32 file_type, s32 file_no) {
    s8 tmp[4];
    memcard_work* mw = &MemcardWork;
    _memcard_file* mf = mc_file_tbl[file_type];

    mw->r_no_0 = 0;
    mw->r_no_1 = 0;
    mw->act_no = 0;
    mw->file_type = file_type;
    mw->file_no = file_no;

    strcpy(mw->dir, mf->file[4].name);

    if (mf->fnum_flag & 1) {
        sprintf(tmp, "%02d", file_no);
        strcat(mw->dir, tmp);
    }
}

void McActMain() {
    memcard_work* mw = &MemcardWork;
    mc_act_jmp[mw->act_no](mw);
}

void McActStopSet() {
    memcard_work* mw = &MemcardWork;

    mw->act_no = 0;
    mw->exe_flag = 0;
}

static void mc_act_stop(memcard_work* mw) {
    // do nothing
}

static void mc_act_return(memcard_work* mw) {
    if (mw->result != -1) {
        McActStopSet();
    }
}

void McActCheckSet() {
    memcard_work* mw = &MemcardWork;

    mw->act_no = 1;
    mw->result = -1;
    mw->r_no_0 = 0;
    mw->r_no_1 = 0;
    mw->port = 0;
}

static void mc_act_check(memcard_work* mw) {
    if (mc_check_card(mw) < 0) {
        return;
    }

    if (++mw->port >= mw->max_port) {
        mw->port = 0;
    }
}

void McActExistSet(s32 port, void* bufs) {
    memcard_work* mw = &MemcardWork;
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    mw->act_no = 2;
    mw->result = -1;
    mw->r_no_0 = 0;
    mw->r_no_1 = 0;
    mw->port = port;
    mw->bufs = bufs;

    if (mf->file[5].flag == 0) {
        sprintf(mw->path, "%s/%s", mw->dir, mw->dir);
        mw->size = mf->file[4].size;
        mw->mode = 0;
    } else {
        sprintf(mw->path, "%s/%s", mw->dir, mf->file[5].name);
        mw->size = mf->file[5].size;
        mw->mode = 1;
    }
}

static void mc_act_exist(memcard_work* mw) {
    memcard_date* md;

    switch (mw->r_no_0) {
    case 0:
        switch (mc_check_card(mw)) {
        case 0:
            mw->result = -0xFF;
            break;

        case 3:
            mw->result = -0xFE;
            break;

        case 1:
        case 2:
            mw->r_no_0 = mw->r_no_0 + 1;
            break;
        }

        break;

    case 1:
        switch (mc_check_file(mw)) {
        case 0:
            md = (memcard_date*)&mc_dir._Modify;
            day_of_week(md);

            if (mw->mode == 0) {
                mw->r_no_0 = 0;
                mw->result = 0;
                memcpy(mw->bufs, md, 8);
            } else {
                mw->r_no_0 = mw->r_no_0 + 1;
            }

            break;

        case 1:
            mw->r_no_0 = mw->r_no_0 + 2;
            strcpy(mw->path, mw->dir);
            mw->size = 0;
            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFF;
            break;

        case 3:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 2:
        switch (mc_read_file(mw)) {
        case 0:
            mw->r_no_0 = 0;
            mw->result = 0;
            memcpy(mw->bufs, &mc_dir._Modify, 8);
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 3:
        switch (mc_check_file(mw)) {
        case 0:
            mw->r_no_0 = 0;
            mw->result = -0xFB;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0xFD;
            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFF;
            break;

        case 3:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

void McActLoadSet(s32 port, void* bufs) {
    memcard_work* mw = &MemcardWork;
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    mw->act_no = 3;
    mw->result = -1;
    mw->r_no_0 = 0;
    mw->r_no_1 = 0;
    mw->port = port;
    mw->bufs = bufs;
    sprintf(mw->path, "%s/%s", mw->dir, mw->dir);
    mw->size = mf->file[4].size;
}

static void mc_act_load(memcard_work* mw) {
    switch (mw->r_no_0) {
    case 0:
        switch (mc_check_card(mw)) {
        case 0:
            mw->result = -0xFF;
            break;

        case 3:
            mw->result = -0xFE;
            break;

        case 1:
        case 2:
            mw->r_no_0 = mw->r_no_0 + 1;
            break;
        }

        break;

    case 1:
        switch (mc_check_file(mw)) {
        case 0:
            mw->r_no_0 = mw->r_no_0 + 1;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0xFD;
            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFF;
            break;

        case 3:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 2:
        switch (mc_read_file(mw)) {
        case 0:
            mw->r_no_0 = 0;
            mw->result = 0;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

void McActSave0Set(s32 port, void* bufs, s32 mode) {
    memcard_work* mw = &MemcardWork;
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    mw->act_no = 4;
    mw->result = -1;
    mw->r_no_0 = 0;
    mw->r_no_1 = 0;
    mw->port = port;
    mw->bufs = bufs;
    mw->mode = mode;
    sprintf(mw->path, "%s/%s", mw->dir, mw->dir);
    mw->size = mf->file[4].size;
    memset(bufs, 0, mw->size);
}

static void mc_act_save0(memcard_work* mw) {
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    switch (mw->r_no_0) {
    case 0:
        switch (mc_check_card(mw)) {
        case 0:
            mw->result = -0xFF;
            break;

        case 3:
            mw->result = -0xFE;
            break;

        case 1:
        case 2:
            mw->r_no_0 = mw->r_no_0 + 1;
            break;
        }

        break;

    case 1:
        switch (mc_check_file(mw)) {
        case 0:
            if (mw->mode == 0) {
                mw->r_no_0 = 0;
                mw->result = 0;
            } else {
                mw->r_no_0 += 1;
            }

            break;

        case 1:
            mw->r_no_0 += 2;
            strcpy(mw->path, mw->dir);
            mw->size = 0;
            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFF;
            break;

        case 3:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 2:
        switch (mc_read_file(mw)) {
        case 0:
            mw->r_no_0 = 0;
            mw->result = 0;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 3:
        switch (mc_check_file(mw)) {
        case 0:
            mw->r_no_0 = 0;
            mw->result = 0;
            break;

        case 1:
            mw->r_no_0 = 0;
            if (mw->free[mw->port] >= mf->req_clust) {
                mw->result = -0xFD;
            } else {
                mw->result = -0xFC;
            }

            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFF;
            break;

        case 3:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

void McActSaveSet(s32 port, void* bufs) {
    memcard_work* mw = &MemcardWork;
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    mw->act_no = 5;
    mw->result = -1;
    mw->r_no_0 = 0;
    mw->r_no_1 = 0;
    mw->cnt_0 = 0;
    mw->port = port;
    sprintf(mw->path, "%s/%s", mw->dir, mw->dir);
    mw->size = mf->file[4].size;
    mf->file[4].bufs = (intptr_t)bufs;
    mf->file[5].bufs = (intptr_t)bufs + mw->size;
    mw->exe_flag = 1;
}

static void mc_act_save(memcard_work* mw) {
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    switch (mw->r_no_0) {
    case 0:
        switch (mc_check_card(mw)) {
        case 0:
            mw->result = -0xFF;
            break;

        case 3:
            mw->result = -0xFE;
            break;

        case 1:
        case 2:
            mw->r_no_0 += 1;
            mc_icon_sys_set(mw);
            sprintf(mw->path, "%s", mw->dir);
            mw->copy = mf->copy_flag;
            break;
        }

        break;

    case 1:
        switch (mc_mkdir(mw)) {
        case 0:
            mw->r_no_0 = mw->r_no_0 + 1;
            mw->cnt_0 = 0;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 2:
    block_20:
        if ((mw->flag = mf->file[mw->cnt_0].flag) == 0) {
            goto block_57;
        }

        sprintf(mw->path, "%s/", mw->dir);

        if (mw->cnt_0 == 4) {
            strcat(mw->path, mw->dir);
        } else {
            strcat(mw->path, mf->file[mw->cnt_0].name);
        }

        mw->bufs = (void*)mf->file[mw->cnt_0].bufs;
        mw->size = mf->file[mw->cnt_0].size;
        mw->r_no_0 += 1;

    case 3:
        switch (mc_check_file(mw)) {
        case 0:
            if (mc_dir.FileSizeByte != mw->size) {
                mw->r_no_0 = mw->r_no_0 + 1;
                break;
            } else if (mw->flag == 1) {
                goto block_57;
            }

            mw->r_no_0 += 3;
            break;

        case 1:
            mw->r_no_0 += 2;
            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFF;
            break;

        case 3:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 4:
        switch (mc_delete_file(mw)) {
        case 0:
            mw->r_no_0 += 1;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 5:
        switch (mc_create_file(mw)) {
        case 0:
            mw->r_no_0 += 1;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 6:
        switch (mc_write_file(mw)) {
        case 0:
        block_57:
            if (++mw->cnt_0 < 6) {
                mw->r_no_0 = 2;
                goto block_20;
            }

            mw->r_no_0 = 0;
            mw->result = 0;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

void McActFormatSet(s32 port) {
    memcard_work* mw = &MemcardWork;

    mw->act_no = 6;
    mw->result = -1;
    mw->r_no_0 = 0;
    mw->r_no_1 = 0;
    mw->port = port;
    mw->exe_flag = 1;
}

static void mc_act_format(memcard_work* mw) {
    switch (mw->r_no_0) {
    case 0:
        switch (mc_format(mw)) {
        case 0:
            mw->result = 0;
            break;

        case 1:
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

static void mc_act_unformat(memcard_work* mw) {
    switch (mw->r_no_0) {
    case 0:
        switch (mc_unformat(mw)) {
        case 0:
            mw->result = 0;
            break;

        case 1:
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

static void mc_act_delete(memcard_work* mw) {
    switch (mw->r_no_0) {
    case 0:
        switch (mc_check_card(mw)) {
        case 0:
            mw->result = -0xFF;
            break;

        case 3:
            mw->result = -0xFE;
            break;

        case 1:
        case 2:
            mw->r_no_0 += 1;
            break;
        }

        break;

    case 1:
        switch (mc_check_file(mw)) {
        case 0:
            mw->r_no_0 += 1;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0xFD;
            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFF;
            break;

        case 3:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;

    case 2:
        switch (mc_delete_dir(mw)) {
        case 0:
            mw->r_no_0 = 0;
            mw->result = 0;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

static void mc_act_remove(memcard_work* mw) {
    switch (mw->r_no_0) {
    case 0:
        switch (mc_delete_dir(mw)) {
        case 0:
            mw->result = 0;
            break;

        case 1:
            mw->result = -0x100;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

static void mc_act_list(memcard_work* mw) {
    switch (mw->r_no_0) {
    case 0:
        switch (mc_check_card(mw)) {
        case 0:
            mw->result = -0xFF;
            break;

        case 3:
            mw->result = -0xFE;
            break;

        case 1:
        case 2:
            mw->r_no_0 += 1;
            break;
        }

        break;

    case 1:
        switch (mc_get_dir(mw)) {
        case 0:
            mw->r_no_0 = 0;
            mw->result = mw->cnt_1;
            break;

        case 1:
            mw->r_no_0 = 0;
            mw->result = -0x100;
            break;

        case 2:
            mw->r_no_0 = 0;
            mw->result = -0xFD;
            break;
        }

        break;
    }

    mc_act_return(mw);
}

s32 McActResult() {
    return MemcardWork.result;
}

s32 McActConChk(s32 port) {
    return MemcardWork.stat[port];
}

void McActNewClr() {
    MemcardWork.new = 0;
}

s32 McActNewChk(s32 port) {
    return MemcardWork.new & (1 << port);
}

s32 McActAvailSet(s32* ico) {
    intptr_t top;
    s32 i;
    s32 n;
    s32 cluster;
    memcard_work* mw = &MemcardWork;
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    if (ico != NULL) {
        top = (intptr_t)ico;
        ico++;
        n = (s32)*ico++;

        for (i = 0; i < 4; i++) {
            if (n <= 0) {
                break;
            }

            if (mf->file[i].flag != 0) {
                mf->file[i].bufs = *ico++;
                mf->file[i].bufs += top;
                mf->file[i].size = *ico++;
                n -= 1;
            }
        }
    }

    n = 0;
    cluster = 0;

    for (i = 0; i < 6; i++) {
        if (mf->file[i].flag == 0) {
            continue;
        }

        cluster += (mf->file[i].size + 0x400 - 1) / 0x400;
        n += 1;
    }

    cluster = cluster + (n + 1) / 2;
    cluster += 2;
    mf->req_clust = cluster;
    printf("McActAvailSet(%d) = %d clusters (%d byte).\n", mw->file_type, cluster, cluster << 10);

    return cluster;
}

void McActLastDate(memcard_date* date) {
    memcard_date* md;

    md = (memcard_date*)&mc_dir._Modify;
    day_of_week(md);
    memcpy(date, md, sizeof(memcard_date));
}

void McActZenNum(s32 num, s8* buf, s32 mode, s32 max) {
    s32 i;
    s32 d0;
    s32 d1;
    static s8 zen_num_tbl[] = "０１２３４５６７８９";

    if ((max <= 0) || (max > 10)) {
        max = 10;
    }

    d0 = 1;

    for (i = 0; i < (max - 1); i++) {
        d0 *= 10;
    }

    for (i = 0; i < max; i++) {
        d1 = num / d0;

        if (d1 > 9) {
            d1 = 9;
        }

        num %= d0;

        if (d1 > 0) {
            mode = 1;
        }

        if (mode != 0) {
            strncpy(buf, &zen_num_tbl[d1 * 2], 2);
            buf += 2;
        }

        d0 /= 10;
    }

    *buf = 0;
}

void* McActIconTexAdrs(s32 file_type, s32 num) {
    u32* data;
    u32 nbsp;
    u32 attrib;
    u32 nbvtx;
    u32 nbksp;
    u32 nbkf;
    _memcard_file* mf = mc_file_tbl[file_type];

    data = (u32*)mf->file[num + 1].bufs;
    data++;
    nbsp = *data++;
    attrib = *data++;
    data++;
    nbvtx = *data++;

    if (!(attrib & 4)) {
        return 0;
    }

    if (attrib & 8) {
        return 0;
    }

    data += (nbsp * 2 + 4) * nbvtx;

    data++;
    data++;
    data++;
    data++;
    nbksp = *data++;

    while (nbksp--) {
        data++;
        nbkf = *data++;
        data += nbkf * 2;
    }

    return data;
}

static void day_of_week(memcard_date* md) {
    s32 y;
    s32 m;

    y = md->year;
    m = md->month;

    if (m < 3) {
        y -= 1;
        m += 12;
    }

    md->dayofweek = (md->day + ((y / 400) + ((y + y / 4) - (y / 100)) + ((m * 13 + 8) / 5))) % 7;
}

static void mc_icon_sys_set(memcard_work* mw) {
    s8 tmp[8];
    sceMcIconSys* isys;
    _memcard_file* mf = mc_file_tbl[mw->file_type];

    if (mf->file[0].flag == 0) {
        return;
    }

    isys = (sceMcIconSys*)mf->file[0].bufs;
    isys->OffsLF = strlen(mf->title1);
    sprintf((s8*)isys->TitleName, "%s%s", mf->title1, mf->title2);

    if (mf->fnum_flag & 2) {
        McActZenNum(mw->file_no, tmp, 1, 2);
        strcat((s8*)isys->TitleName, tmp);
    }

    strcpy((s8*)isys->FnameView, mf->file[1].name);
    strcpy((s8*)isys->FnameCopy, mf->file[2].name);
    strcpy((s8*)isys->FnameDel, mf->file[3].name);
}
