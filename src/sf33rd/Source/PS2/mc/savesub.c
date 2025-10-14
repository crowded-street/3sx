#include "sf33rd/Source/PS2/mc/savesub.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/RAMCNT.h"

#if !defined(TARGET_PS2)
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#endif

#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/workuser.h"
#include "sf33rd/Source/PS2/mc/knjsub.h"
#include "sf33rd/Source/PS2/mc/mcsub.h"
#include "sf33rd/Source/PS2/mc/msgsub.h"

#include "port/io/afs.h"

#include <tim2.h>

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// sdata
s16 icon_fnum[3] = { 83, 84, 85 };
s16 data_fnum[3] = { 87, -1, 88 };
s16 font_fnum[3] = { 80, 81, 82 };

s8* pl_name[20] = {
    "GIL", "ALX", "RYU", "YUN", "DUD", "NCR", "HUG", "IBK", "ELE", "ORO",
    "YAN", "KEN", "SEN", "URN", "AKM", "CHN", "MAK", " Q ", "TWE", "REM",
};

// bss
_save_work SaveWork;

// sbss

static u32 AutoMcSlot;
static u32 LastMcSlot;

// forward decls
static void save_move_init(_save_work* save);
static void save_move_sels(_save_work* save);
static void save_move_self(_save_work* save);
static void save_move_aload(_save_work* save);
static void save_move_asave(_save_work* save);
static void save_move_exit(_save_work* save);
static s32 save_move_test(_save_work* save);
static void save_sw_get(_save_work* save);
static void decide_se();
static void cancel_se();
static void select_se();
static void nogood_se();
static void last_slot_set(_save_work* save);
static void auto_slot_set(_save_work* save);
static void yes_no_set(_save_work* save, s32 def);
static void self_order_get(_save_work* save);
static void save_data_store(_save_work* save);
static void save_data_store_system(_save_work* save);
static void save_data_store_sysdir(_save_work* save);
static void save_data_store_replay(_save_work* save);
static s32 save_data_decode_system(_save_work* save, s32 mode);
static s32 save_data_decode_sysdir(_save_work* save, s32 mode);
static s32 save_data_decode_replay(_save_work* save, s32 mode);
static void load_data_set_system(_save_work* save);
static void load_data_set_sysdir(_save_work* save);
static void load_data_set_replay(_save_work* save);
static void load_data_set(_save_work* save);
static void icon_tex_chg(u8* src, u16* dst, u32 size);
static void encode_data(u16* src, s32 size);
static void decode_data(_save_work* save, u16* src, s32 size);
static void save_slot_trans(_save_work* save);
static void save_file_trans(_save_work* save);
static void save_msg_trans(_save_work* save);
static void mc_msg_set(_save_work* save, s32 msg_no);
static s32 push_any_key(_save_work* save);
static s32 decide_ck();
static s32 cancel_ck();
static s32 save_data_decode(_save_work* save, s32 mode);
static s32 info_data_check(_save_work* save);
static s32 yes_no_check(_save_work* save);

static void load_data(s32 fnum, void* adrs) {
    s32 nsct;
    _save_work* save = &SaveWork;

    save->afs_handle = AFS_Open(fnum);

    if (save->afs_handle == AFS_NONE) {
        printf("file open error.(%d)\n", fnum);

        while (1) {
            // do nothing
        }
    }

    nsct = AFS_GetSectorCount(save->afs_handle);
    printf("load_data: fnum=%d adrs=0x%" PRIXPTR " size=0x%X\n", fnum, (uintptr_t)adrs, nsct << 11);
    AFS_Read(save->afs_handle, nsct, adrs);
}

static s32 load_busy_ck() {
    s32 stat;
    _save_work* save = &SaveWork;

    if (save->afs_handle == AFS_NONE) {
        return 0;
    }

    stat = AFS_GetState(save->afs_handle);

    if (stat == AFS_READ_STATE_READING) {
        return 1;
    }

    AFS_Close(save->afs_handle);
    save->afs_handle = AFS_NONE;

    return 0;
}

void SaveInit(s32 file_type, s32 save_mode) {
    u32 size;
    uintptr_t adrs;
    _save_work* save = &SaveWork;

    Forbid_Reset = 1;

    memset(save, 0, sizeof(_save_work));

    save->return_code = 1;
    save->file_type = file_type;
    save->save_mode = save_mode;
    save->auto_save_old = save_w[1].Auto_Save;

    McActInit(save->file_type, 0);

    if (LastMcSlot != 0) {
        save->sel_slot_no = LastMcSlot - 1;
    }

    save->sel_slot_max = 2;

    size = (save_mode & 1) ? 0x156100 : 0xE0000;
    save->ram_key = Pull_ramcnt_key(size, 2, 0, 0);
    adrs = Get_ramcnt_address(save->ram_key);
    adrs = ((adrs + 0x40 - 1) / 0x40) * 0x40;

    save->fnt_adrs = (u8*)adrs;
    save->buf_adrs = save->fnt_adrs + 0xC0000;
    save->ico_adrs = save->buf_adrs + 0x10000;
    save->dat_adrs = save->ico_adrs + 0x10000;
    save->exp_adrs = save->dat_adrs + 0x6E000;
    save->buf_adrs0 = save->buf_adrs;

    MsgLanguage = 1;

    load_data(font_fnum[MsgLanguage], save->fnt_adrs);
}

typedef void (*save_move_func)(_save_work*);

s32 SaveMove() {
    static save_move_func save_move_jmp[7] = {
        save_move_init,
        save_move_sels,
        save_move_self,
        save_move_aload,
        save_move_asave,
        save_move_exit,
        (save_move_func)&save_move_test // This one returns...
    };

    _save_work* save = &SaveWork;

    save_sw_get(save);
    save_move_jmp[save->r_no_0](save);
    McActMain();
    save_msg_trans(save);
    task[SAVER_TASK_NUM].timer = 0;

    return save->return_code;
}

static void save_sw_get(_save_work* save) {
    u16 i;
    u16 sw;
    u16 temp_a1;

    save->sw = save->tr = save->rp = 0;

    for (i = 0; i < 2; i++) {
        save->sw |= (sw = PLsw[i][0]);
        save->tr |= ~PLsw[i][1] & PLsw[i][0];

        if (save->lv1st[i] == 0) {
            if (sw & 0xF) {
                save->lv1st[i] = 1;
                save->lvctr[i] = 0x14;
                save->rp |= sw & 0xF;
            }
        } else {
            if (sw & 0xF) {
                if (--save->lvctr[i] == 0) {
                    save->lvctr[i] = 4;
                    save->rp |= sw & 0xF;
                }
            } else {
                save->lv1st[i] = 0;
            }
        }
    }
}

static void save_move_init(_save_work* save) {
    s32 knj_type;
    s32 fnum;

    switch (save->r_no_1) {
    case 0:
        if (load_busy_ck() != 0) {
            break;
        }

        switch (MsgLanguage) {
        default:
        case 0:
            knj_type = 4;
            break;

        case 1:
            knj_type = 8;
            break;

        case 2:
            knj_type = 12;
            break;
        }

        fatal_error("Not implemented");
        // KnjInit(knj_type, (uintptr_t)save->fnt_adrs, 0x200, flPs2State.ZBuffAdrs);
        load_data(icon_fnum[save->file_type], save->ico_adrs);
        save->r_no_1 += 1;
        /* fallthrough */

    case 1:
        if (load_busy_ck() != 0) {
            break;
        }

        save->r_no_1 += 1;
        save->avail_size = McActAvailSet((s32*)save->ico_adrs);

        if (save->save_mode & 1) {
            fnum = data_fnum[save->file_type];

            if (fnum >= 0) {
                load_data(fnum, save->dat_adrs);
            }
        }

        /* fallthrough */

    case 2:
    case 3:
        switch (save->save_mode) {
        default:
            save->r_no_0 = 1;
            break;

        case 2:
            save->r_no_0 = 3;
            break;

        case 3:
            save->r_no_0 = 4;
            break;
        }

        save->r_no_1 = 0;
        save->r_no_2 = 0;
        break;
    }
}

static void mc_msg_set(_save_work* save, s32 msg_no) {
    save->mc_msg_st = 0;
    save->mc_msg_no = msg_no;
}

static void sels_result_set(_save_work* save, s32 msg_no) {
    save->r_no_1 = 8;
    save->r_no_2 = 0;
    mc_msg_set(save, msg_no);
}

static void sel_slot_init(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        save->cnt_0 = 10;
        /* fallthrough */

    case 1:
        if (--save->cnt_0 <= 0) {
            save->r_no_1 = 1;
        }

        break;
    }
}

static void sel_slot_start(_save_work* save) {
    save->r_no_1 = 2;
    save->r_no_2 = 0;
    save->sel_slot_st = 0;
    save->yes_no_flag = 0;
    mc_msg_set(save, 2);
}

static void sel_slot_main(_save_work* save) {
    s32 val;
    s32 max;

    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        if (decide_ck() != 0) {
            save->r_no_2 += 1;
            save->sel_slot_st = 1;
            save->cursor_sign = 0;
            decide_se();
            break;
        }

        if (cancel_ck() != 0) {
            save->r_no_2 += 1;
            save->cursor_sign = 1;
            cancel_se();
            break;
        }

        val = save->sel_slot_no;
        max = save->sel_slot_max;

        if ((save->tr & 8) && (++val > (max - 1))) {
            val = max - 1;
        } else if ((save->tr & 4) && (--val < 0)) {
            val = 0;
        }

        if ((save->sel_slot_no) != val) {
            save->sel_slot_no = val;
            select_se();
        }

        break;

    case 1:
        if (save->cursor_sign == 0) {
            if (save->file_type == 0) {
                save->r_no_1 = 3;
            } else {
                save->r_no_1 = 4;
            }
        } else {
            save->r_no_1 = 9;
        }

        save->r_no_2 = 0;
        mc_msg_set(save, 0);
        break;
    }
}

static void sel_slot_check(_save_work* save) {
    s32 mode;

    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        mc_msg_set(save, 8);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        McActSave0Set(save->sel_slot_no, save->buf_adrs, 1);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            mode = save->save_mode ? 0 : 1;
            if (save_data_decode(save, mode) != 0) {
                save->mc_result = -0x100;
                sels_result_set(save, 4);
                break;
            }

            if (save->save_mode == 0) {
                load_data_set(save);
                last_slot_set(save);
                sels_result_set(save, 26);
            } else {
                save->r_no_1 = 5;
                save->r_no_2 = 0;
                save->r_no_n = 7;
                mc_msg_set(save, 19);
                yes_no_set(save, 2);
            }

            break;

        case -1:
            break;

        case -255:
            sels_result_set(save, 5);
            break;

        case -254:
            if (save->save_mode == 0) {
                sels_result_set(save, 7);
            } else {
                save->r_no_1 = 5;
                save->r_no_2 = 0;
                save->r_no_n = 6;
                mc_msg_set(save, 14);
                yes_no_set(save, 2);
            }

            break;

        case -253:
            if (save->save_mode == 0) {
                sels_result_set(save, 7);
            } else {
                save->r_no_1 = 7;
                save->r_no_2 = 0;
            }

            break;

        case -252:
            if (save->save_mode == 0) {
                sels_result_set(save, 7);
            } else {
                sels_result_set(save, 23);
            }

            break;

        case -256:
            sels_result_set(save, 9);
            break;
        }

        break;
    }
}

static void sel_slot_check2(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        save->cnt_0 = 0;
        save->cnt_1 = 0;
        save->sel_file_max0 = save->file_type == 1 ? 10 : 20;

        mc_msg_set(save, 8);

        memset(save->info, 0xFF, sizeof(save->info));
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        McActInit(save->file_type, save->cnt_0);
        McActExistSet(save->sel_slot_no, &save->info[save->cnt_0]);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case -251:
            save->info[save->cnt_0].date.dayofweek = 254;
            /* fallthrough */

        case 0:
            save->cnt_1 += 1;

            if (info_data_check(save) != 0) {
                save->info[save->cnt_0].date.dayofweek = 254;
            }

            /* fallthrough */

        case -253:
            if (++save->cnt_0 < save->sel_file_max0) {
                save->r_no_2 -= 1;
                break;
            }

            if (save->save_mode == 0 && save->cnt_1 == 0) {
                sels_result_set(save, 7);
                break;
            }

            save->r_no_0 = 2;
            save->r_no_1 = 0;
            save->r_no_2 = 0;
            save->sel_file_max = save->cnt_1;
            mc_msg_set(save, 0);
            break;

        case -1:
            break;

        case -255:
            sels_result_set(save, 5);
            break;

        case -254:
            if (save->save_mode == 0) {
                sels_result_set(save, 7);
            } else {
                save->r_no_1 = 5;
                save->r_no_2 = 0;
                save->r_no_n = 6;
                mc_msg_set(save, 14);
                yes_no_set(save, 2);
            }

            break;

        case -256:
            sels_result_set(save, 9);
            break;
        }

        break;
    }
}

static void sel_slot_yesno(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;

        McActCheckSet();
        /* fallthrough */

    case 1:
        if (McActConChk(save->sel_slot_no) == 0) {
            save->r_no_1 = 1;
            break;
        }

        switch (yes_no_check(save)) {
        case 1:
            save->r_no_1 = save->r_no_n;
            save->r_no_2 = 0;
            break;

        case 2:
            save->r_no_1 = 1;
            break;
        }

        break;
    }
}

static void sel_slot_format(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        mc_msg_set(save, 15);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        McActFormatSet(save->sel_slot_no);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            if (save->file_type == 0) {
                save->r_no_1 = 7;
                save->r_no_2 = 0;
            } else {
                save->r_no_0 = 2;
                save->r_no_1 = 0;
                save->r_no_2 = 0;
                save->sel_file_max = 0;
                mc_msg_set(save, 0);
            }

            break;

        case -1:
            break;

        default:
            sels_result_set(save, 16);
            break;
        }

        break;
    }
}

static void sel_slot_save(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        mc_msg_set(save, 20);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;

        save_data_store(save);
        McActSaveSet(save->sel_slot_no, save->buf_adrs);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            save_data_decode(save, 0);
            load_data_set(save);
            last_slot_set(save);
            sels_result_set(save, 21);
            save->ask_save_flag = 0;
            break;

        case -1:
            break;

        default:
            sels_result_set(save, 22);
            break;
        }

        break;
    }
}

static void sel_slot_result(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        if (save->mc_result == 0) {
            save->success_flag = 1;
            McActNewClr();
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;
        /* fallthrough */

    case 1:
        if (push_any_key(save) != 0) {
            break;
        }

        if (save->mc_result != 0) {
            save->r_no_1 = 1;
            break;
        }

        if (save_w[1].Auto_Save == 0) {
            goto block_19;
        }

        if (LastMcSlot == save->sel_slot_old) {
            if (save->auto_save_old != 0) {
                goto block_19;
            }
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;
        auto_slot_set(save);
        mc_msg_set(save, 31);
        /* fallthrough */

    case 2:
        if (push_any_key(save) == 0) {
        block_19:
            save->r_no_1 = 9;
            mc_msg_set(save, 0);
        }

        break;
    }
}

static void sel_slot_exit(_save_work* save) {
    if (save->mc_msg_st) {
        return;
    }

    save->r_no_0 = 5;
    save->r_no_1 = 0;
    save->sel_slot_st = -1;
}

typedef void (*sel_slot_func)(_save_work*);

static void save_move_sels(_save_work* save) {
    static sel_slot_func sel_slot_jmp[] = {
        sel_slot_init,  sel_slot_start,  sel_slot_main, sel_slot_check,  sel_slot_check2,
        sel_slot_yesno, sel_slot_format, sel_slot_save, sel_slot_result, sel_slot_exit,
    };

    sel_slot_jmp[save->r_no_1](save);
    save_slot_trans(save);
}

static void sels_return_set(_save_work* save) {
    save->r_no_0 = 1;
    save->r_no_1 = 1;
    mc_msg_set(save, 0);
}

static void self_result_set(_save_work* save, s32 msg_no) {
    save->r_no_1 = 7;
    save->r_no_2 = 0;
    mc_msg_set(save, msg_no);
}

static void sel_file_init(_save_work* save) {
    s32 i;
    s32 n;
    _sub_info* info = save->info;
    u8* order = save->sel_file_order;

    save->r_no_1 = 1;
    save->sel_file_top = 0;
    save->sel_file_idx = 0;
    save->sel_file_st = 0;
    save->sel_file_sort = 0;

    n = save->sel_file_max;

    if ((save->save_mode != 0) && (n < save->sel_file_max0)) {
        save->sel_file_max += 1;
        *order++ = 0xFF;
    }

    i = 0;

    while (n > 0) {
        if (info->date.dayofweek != 255) {
            *order++ = i;
            n -= 1;
        }

        i++;
        info++;
    }
}

static void sel_file_start(_save_work* save) {
    s32 msg_no;

    save->r_no_1 = 2;
    save->r_no_2 = 0;
    save->sel_file_st = 0;
    save->yes_no_flag = 0;

    msg_no = save->save_mode ? 18 : 17;
    mc_msg_set(save, msg_no);
}

static void sel_file_main(_save_work* save) {
    u32 i;
    u32 val;
    s32 top;
    s32 idx;
    s32 tmax;
    s32 dmax;

    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        McActCheckSet();
        /* fallthrough */

    case 1:
        if (McActConChk(save->sel_slot_no) == 0) {
            sels_return_set(save);
            break;
        }

        top = save->sel_file_top;
        idx = save->sel_file_idx;

        if (decide_ck() != 0) {
            val = save->sel_file_order[top + idx];

            if (save->save_mode == 0) {
                if ((save->info[val].date.dayofweek) == 254) {
                    nogood_se();
                    break;
                }

                save->r_no_1 = 3;
            } else {
                if (val == 255) {
                    val = 0;

                    for (i = 0; i < save->sel_file_max0; i++) {
                        if (save->info[i].date.dayofweek == 255) {
                            val = i;
                            break;
                        }
                    }
                }

                save->r_no_1 = 4;
            }

            save->r_no_2 = 0;
            mc_msg_set(save, 0);

            save->sel_file_no = val;
            save->sel_file_st = 1;
            decide_se();
            break;
        }

        if (cancel_ck() != 0) {
            sels_return_set(save);
            cancel_se();
            break;
        }

        if (save->tr & 0x20) {
            save->sel_file_sort ^= 1;
            self_order_get(save);
            cancel_se();
            break;
        }

        tmax = save->sel_file_max;
        dmax = 4;

        if (save->rp & 2) {
            if ((top + idx + 1) >= tmax) {
                top = idx = 0;
            } else if ((idx + 1) >= dmax) {
                top += 1;
            } else {
                idx += 1;
            }

        } else if (save->rp & 1) {
            if ((top + idx) == 0) {
                if (tmax <= dmax) {
                    idx = tmax - 1;
                } else {
                    top = tmax - dmax;
                    idx = dmax - 1;
                }
            } else if (idx == 0) {
                top -= 1;
            } else {
                idx -= 1;
            }

        } else if (save->rp & 8) {
            if ((top + idx + 1) >= tmax) {
                top = idx = 0;
            } else if ((top + idx + dmax) >= tmax) {
                if (tmax <= dmax) {
                    top = 0;
                    idx = tmax - 1;
                } else {
                    top = tmax - dmax;
                    idx = dmax - 1;
                }
            } else if ((top + dmax) > (tmax - dmax)) {
                idx = (idx + (top + dmax)) - (tmax - dmax);
                top = tmax - dmax;
            } else {
                top += dmax;
            }

        } else if (save->rp & 4) {
            if ((top + idx) == 0) {
                if (tmax <= dmax) {
                    idx = tmax - 1;
                } else {
                    top = tmax - dmax;
                    idx = dmax - 1;
                }
            } else if ((top - dmax) < 0) {
                if ((top + idx) < dmax) {
                    idx = 0;
                } else {
                    idx = (top + idx) - dmax;
                }

                top = 0;
            } else {
                top -= dmax;
            }
        }

        if ((top != save->sel_file_top) || (idx != save->sel_file_idx)) {
            save->sel_file_top = top;
            save->sel_file_idx = idx;
            select_se();
        }

        break;
    }
}

static void sel_file_load(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        mc_msg_set(save, 3);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;

        McActInit(save->file_type, save->sel_file_no);
        McActLoadSet(save->sel_slot_no, save->buf_adrs);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            if (save_data_decode(save, 1) != 0) {
                save->mc_result = -0x100;
                self_result_set(save, 4);
            } else {
                load_data_set(save);
                last_slot_set(save);
                self_result_set(save, 26);
            }

            break;

        case -1:
            break;

        default:
            self_result_set(save, 4);
            break;
        }

        break;
    }
}

static void sel_file_check(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        mc_msg_set(save, 8);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        McActInit(save->file_type, save->sel_file_no);
        McActSave0Set(save->sel_slot_no, save->buf_adrs, 0);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            save->r_no_1 = 5;
            save->r_no_2 = 0;
            save->r_no_n = 6;
            mc_msg_set(save, 0x13);
            yes_no_set(save, 2);
            break;

        case -1:
            break;

        case -253:
            save->r_no_1 = 6;
            save->r_no_2 = 0;
            break;

        case -252:
            self_result_set(save, 23);
            break;

        default:
            self_result_set(save, 9);
            break;
        }

        break;
    }
}

static void sel_file_yesno(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;

        McActCheckSet();
        /* fallthrough */

    case 1:
        if (McActConChk(save->sel_slot_no) == 0) {
            sels_return_set(save);
            break;
        }

        switch (yes_no_check(save)) {
        case 1:
            save->r_no_1 = save->r_no_n;
            save->r_no_2 = 0;
            break;

        case 2:
            save->r_no_1 = 1;
            break;
        }

        break;
    }
}

static void sel_file_save(_save_work* save) {
    s32 top;
    s32 idx;

    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        mc_msg_set(save, 20);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;

        save_data_store(save);
        McActInit(save->file_type, save->sel_file_no);
        McActSaveSet(save->sel_slot_no, save->buf_adrs);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            save->r_no_2 += 1;
            McActExistSet(save->sel_slot_no, &save->info_new);
            break;

        case -1:
            break;

        default:
            self_result_set(save, 22);
            break;
        }

        break;

    case 3:
        switch (save->mc_result = McActResult()) {
        case 0:
            save_data_decode(save, 0);
            load_data_set(save);
            last_slot_set(save);
            self_result_set(save, 21);

            top = save->sel_file_top;
            idx = save->sel_file_idx;
            save->sel_file_order[top + idx] = save->sel_file_no;
            save->info[save->sel_file_no] = save->info_new;
            break;

        case -1:
            break;

        default:
            self_result_set(save, 22);
            break;
        }

        break;
    }
}

static void sel_file_result(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        if (save->mc_result == 0) {
            save->success_flag = 1;
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;
        /* fallthrough */

    case 1:
        if (push_any_key(save) != 0) {
            break;
        }

        if (save->mc_result != 0) {
            sels_return_set(save);
            break;
        }

        save->r_no_1 = 8;
        mc_msg_set(save, 0);
        break;
    }
}

static void sel_file_exit(_save_work* save) {
    if (save->mc_msg_st) {
        return;
    }

    save->r_no_0 = 5;
    save->r_no_1 = 0;
    save->sel_slot_st = -1;
}

typedef void (*sel_file_func)(_save_work*);

static void save_move_self(_save_work* save) {
    static sel_file_func sel_file_jmp[] = {
        sel_file_init,  sel_file_start, sel_file_main,   sel_file_load, sel_file_check,
        sel_file_yesno, sel_file_save,  sel_file_result, sel_file_exit,
    };

    sel_file_jmp[save->r_no_1](save);
    save_file_trans(save);
}

static u8* aload_buf_adrs(_save_work* save) {
    return save->buf_adrs0 + (save->sel_slot_no << 15);
}

static void auto_load_init(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        save->cnt_0 = 1;

        LastMcSlot = 0;
        Present_Mode = 1;
        /* fallthrough */

    case 1:
        if (--save->cnt_0 <= 0) {
            save->r_no_1 = 1;
            save->r_no_2 = 0;
        }

        break;
    }
}

static void auto_load_load(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        mc_msg_set(save, 8);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            return;
        }

        save->r_no_2 += 1;
        save->sel_slot_no = 0;
        save->al_no_err = 0;
        save->al_no_card = 0;
        save->al_no_file = 0;
        save->al_no_space = 0;
        save->al_error = 0;
        /* fallthrough */

    case 2:
        save->r_no_2 += 1;
        save->buf_adrs = aload_buf_adrs(save);
        McActSave0Set(save->sel_slot_no, save->buf_adrs, 1);
        /* fallthrough */

    case 3:
        switch (save->mc_result = McActResult()) {
        case 0:
            if (save_data_decode(save, 1) != 0) {
                save->mc_result = -0x100;
                goto block_29;
            } else {
                save->al_no_err |= 1 << save->sel_slot_no;
                McActLastDate(&save->auto_date[save->sel_slot_no]);
            }

            break;

        case -1:
            return;

        case -255:
            save->al_no_card |= 1 << save->sel_slot_no;
            break;

        case -254:
        case -253:
            save->al_no_file |= 1 << save->sel_slot_no;
            break;

        case -252:
            save->al_no_space |= 1 << save->sel_slot_no;
            break;

        case -256:
        block_29:
            save->al_error |= 1 << save->sel_slot_no;
            break;
        }

        if (save->sel_slot_no == 0) {
            save->sel_slot_no += 1;
            save->r_no_2 = 2;
            break;
        }

        if (save->al_error != 0) {
        block_30:
            mc_msg_set(save, 27);
            save->sel_slot_no = (save->al_error & 1) ? 0 : 1;
            goto block_53;
        }

        if (save->al_no_err != 0) {
            save->r_no_1 = 2;
            save->r_no_2 = 0;
            break;
        }

        if (save->al_no_card == 3) {
            mc_msg_set(save, 28);
            goto block_53;
        }

        if (save->al_no_file != 0) {
            save->r_no_1 = 5;
            mc_msg_set(save, 0);
            break;
        }

        if (save->al_no_space == 0) {
            goto block_30;
        }

        if (save->al_no_space != 3) {
            mc_msg_set(save, 29);
            save->sel_slot_no = (save->al_no_space & 1) ? 0 : 1;
        } else {
            mc_msg_set(save, 30);
        }

    block_53:
        save->r_no_1 = 3;
        save->r_no_2 = 0;
        save->r_no_n = 1;
        yes_no_set(save, 2);
        break;
    }
}

static void auto_load_set(_save_work* save) {
    s32 i;
    s8 s[2][32];
    memcard_date* md;

    if (save->al_no_err == 1) {
        save->sel_slot_no = 0;
    } else if (save->al_no_err == 2) {
        save->sel_slot_no = 1;
    } else {
        for (i = 0; i < 2; i++) {
            md = &save->auto_date[i];
            sprintf(s[i], "%04d%02d%02d%02d%02d%02d", md->year, md->month, md->day, md->hour, md->min, md->sec);
        }

        save->sel_slot_no = (strcmp(s[0], s[1]) >= 0) ? 0 : 1;
    }

    save->buf_adrs = aload_buf_adrs(save);
    load_data_set(save);
    last_slot_set(save);
    save->r_no_1 = 4;
    save->r_no_2 = 0;
    save->success_flag = 1;
    mc_msg_set(save, 26);
}

static void auto_load_yesno(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;
        /* fallthrough */

    case 1:
        switch (yes_no_check(save)) {
        case 1:
            save->r_no_1 = 5;
            mc_msg_set(save, 0);
            break;

        case 2:
            save->r_no_1 = save->r_no_n;
            save->r_no_2 = 0;
            break;
        }

        break;
    }
}

static void auto_load_result(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;
        /* fallthrough */

    case 1:
        if (push_any_key(save) != 0) {
            break;
        }

        if (save_w[1].Auto_Save == 0) {
            goto label;
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;
        auto_slot_set(save);
        mc_msg_set(save, 31);

        McActNewClr();
        /* fallthrough */

    case 2:
        if (push_any_key(save) == 0) {
        label:
            save->r_no_1 = 5;
            mc_msg_set(save, 0);
        }

        break;
    }
}

static void auto_load_exit(_save_work* save) {
    if (save->mc_msg_st) {
        return;
    }

    save->r_no_0 = 5;
    save->r_no_1 = 0;
}

typedef void (*auto_load_func)(_save_work*);

static void save_move_aload(_save_work* save) {
    static auto_load_func auto_load_jmp[] = {
        auto_load_init, auto_load_load, auto_load_set, auto_load_yesno, auto_load_result, auto_load_exit,
    };

    auto_load_jmp[save->r_no_1](save);
}

static void asave_result_set(_save_work* save, s32 msg_no) {
    save->r_no_1 = 3;
    save->r_no_2 = 0;
    mc_msg_set(save, msg_no);
}

static void auto_save_init(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (load_busy_ck()) {
            break;
        }

        save->r_no_2 += 1;
        save->cnt_0 = 1;
        save->sel_slot_no = AutoMcSlot - 1;

        Present_Mode = 1;
        /* fallthrough */

    case 1:
        if (--save->cnt_0 > 0) {
            break;
        }

        save->r_no_1 = 1;
        save->r_no_2 = 0;
    }
}

static void auto_save_load(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        mc_msg_set(save, 8);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        McActSave0Set(save->sel_slot_no, save->buf_adrs, 0);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            if (McActNewChk(save->sel_slot_no) != 0) {
                save->mc_result = -0x100;
                goto label;
            } else {
                save->r_no_1 = 2;
                save->r_no_2 = 0;
            }

            break;

        case -1:
            break;

        case -255:
            asave_result_set(save, 0x20);
            break;

        case -254:
        case -253:
        case -252:
        label:
            asave_result_set(save, 0x21);
            break;

        case -256:
            asave_result_set(save, 0x16);
            break;
        }

        break;
    }
}

static void auto_save_save(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        save->r_no_2 += 1;
        mc_msg_set(save, 20);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;

        save_data_store(save);
        McActSaveSet(save->sel_slot_no, save->buf_adrs);
        /* fallthrough */

    case 2:
        switch (save->mc_result = McActResult()) {
        case 0:
            save_data_decode(save, 0);
            last_slot_set(save);
            save->success_flag = 1;
            asave_result_set(save, 21);
            break;

        case -1:
            break;

        default:
            asave_result_set(save, 22);
            break;
        }

        break;
    }
}

static void auto_save_result(_save_work* save) {
    switch (save->r_no_2) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_2 += 1;
        save->r_no_3 = 0;
        save->cnt_2 = 0x258;
        /* fallthrough */

    case 1:
        if ((save->mc_result != 0) || (--save->cnt_2 > 0)) {
            if (push_any_key(save) != 0) {
                break;
            }

            if (save->mc_result != 0) {
                Convert_Buff[3][0][2] = save_w[1].Auto_Save = 0;
                save->ask_save_flag = 1;
            }
        }

        save->r_no_1 = 4;
        mc_msg_set(save, 0);
        break;
    }
}

static void auto_save_exit(_save_work* save) {
    if (save->mc_msg_st) {
        return;
    }

    save->r_no_0 = 5;
    save->r_no_1 = 0;
}

typedef void (*auto_save_func)(_save_work*);

static void save_move_asave(_save_work* save) {
    static auto_save_func auto_save_jmp[] = {
        auto_save_init, auto_save_load, auto_save_save, auto_save_result, auto_save_exit,
    };

    auto_save_jmp[save->r_no_1](save);
}

static void save_move_exit(_save_work* save) {
    switch (save->r_no_1) {
    case 0:
        if (save->ask_save_flag == 0) {
            goto block_17;
        }

        save->r_no_1 += 1;
        save->save_mode = 1;

        mc_msg_set(save, 34);
        yes_no_set(save, 2);
        /* fallthrough */

    case 1:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_1 += 1;
        save->r_no_3 = 0;
        /* fallthrough */

    case 2:
        switch (yes_no_check(save)) {
        case 1:
            mc_msg_set(save, 0);
            goto block_17;

        case 2:
            sels_return_set(save);
            break;
        }

        break;

    case 10:
    block_17:
        save->r_no_1 = 11;
        save->cnt_0 = 10;
        /* fallthrough */

    case 11:
        if (load_busy_ck() != 0) {
            break;
        }

        if (--save->cnt_0 > 0) {
            break;
        }

        Convert_Buff[3][0][2] = save_w[1].Auto_Save;
        Forbid_Reset = 0;
        KnjFinish();
        Push_ramcnt_key(save->ram_key);
        save->return_code = save->success_flag ? 0 : -1;
        break;
    }
}

static s32 save_move_test(_save_work* save) {
    s32 d0 = save->mc_msg_no;
    s32 d1 = 35;

    if (save->tr & 8) {
        d0 += 1;

        if (d0 > (d1 - 1)) {
            d0 = 2;
        }
    }

    if (save->tr & 4) {
        d0 -= 1;

        if (d0 < 2) {
            d0 = d1 - 1;
        }
    }

    save->mc_msg_no = d0;

    if (cancel_ck() != 0) {
        save->r_no_0 = 0;
        save->r_no_1 = 3;
        save->yes_no_flag = 0;
        save->mc_msg_no = 0;
    }

    return 1;
}

static s32 decide_ck() {
    return SaveWork.tr & 0x100;
}

static s32 cancel_ck() {
    return SaveWork.tr & 0x200;
}

static s32 shotall_ck() {
    return SaveWork.tr & 0xFF0;
}

static void decide_se() {
    SE_selected();
}

static void cancel_se() {
    SE_selected();
}

static void select_se() {
    SE_cursor_move();
}

static void nogood_se() {
    SE_selected();
}

static void last_slot_set(_save_work* save) {
    save->sel_slot_old = LastMcSlot;
    LastMcSlot = save->sel_slot_no + 1;
}

static void auto_slot_set(_save_work* save) {
    AutoMcSlot = LastMcSlot;
}

static s32 push_any_key(_save_work* save) {
    switch (save->r_no_3) {
    case 0:
        if (save->mc_msg_st) {
            break;
        }

        save->r_no_3 += 1;
        save->cnt_3 = 600;
        break;

    case 1:
        if ((save->save_mode == 2) && (--save->cnt_3 <= 0)) {
            return 0;
        }

        if (shotall_ck() != 0) {
            decide_se();
            return 0;
        }

        break;
    }

    return 1;
}

static void yes_no_set(_save_work* save, s32 def) {
    save->yes_no_flag = def;
}

static s32 yes_no_check(_save_work* save) {
    switch (save->r_no_3) {
    case 0:
        if (decide_ck() != 0) {
            decide_se();
            save->r_no_3 += 1;
            break;
        }

        if (cancel_ck() != 0) {
            cancel_se();
            save->r_no_3 += 1;
            save->yes_no_flag = 2;
            break;
        }

        if (((save->tr & 8) && (save->yes_no_flag == 1)) || ((save->tr & 4) && (save->yes_no_flag == 2))) {
            save->yes_no_flag ^= 3;
            select_se();
        }

        break;

    case 1:
        return save->yes_no_flag;
    }

    return 0;
}

static void self_order_get(_save_work* save) {
    u8* o;
    u8 tmp;
    u32 i;
    u32 j;
    u32 k;
    u32 n;
    u32 f;
    memcard_date* md[2];
    s8 s[2][16];

    n = save->sel_file_max;
    o = save->sel_file_order;

    for (i = 0; i < (n - 1); i++) {
        if (o[i] == 0xFF) {
            continue;
        }

        for (j = i + 1; j < n; j++) {
            f = 0;

            if (save->sel_file_sort == 0) {
                if (o[i] > o[j]) {
                    f = 1;
                }
            } else {
                md[0] = (struct memcard_date*)&save->info[o[i]];
                md[1] = (struct memcard_date*)&save->info[o[j]];

                for (k = 0; k < 2; k++) {
                    sprintf(s[k],
                            "%04d%02d%02d%02d%02d%02d",
                            md[k]->year,
                            md[k]->month,
                            md[k]->day,
                            md[k]->hour,
                            md[k]->min,
                            md[k]->sec);
                }

                if (strcmp(s[0], s[1]) < 0) {
                    f = 1;
                }
            }

            if (f != 0) {
                tmp = o[i];
                o[i] = o[j];
                o[j] = tmp;
            }
        }
    }
}

typedef void (*save_data_store_func)(_save_work*);

static void save_data_store(_save_work* save) {
    static auto_save_func save_data_store_jmp[] = { save_data_store_system,
                                                    save_data_store_sysdir,
                                                    save_data_store_replay };

    save_data_store_jmp[save->file_type](save);
}

static void save_data_store_system(_save_work* save) {
    u8* src;
    u16* dst;
    u32* head;
    s32 pltype;
    _save_data* data = (_save_data*)save->buf_adrs;

    memset(data, 0, sizeof(_save_data));
    Save_Game_Data();
    memcpy(&data->save_w, &save_w[Present_Mode], sizeof(struct _SAVE_W));
    encode_data((u16*)data, sizeof(_save_data));

    pltype = mppGetFavoritePlayerNumber() - 1;

    if ((pltype < 0) || (pltype >= 20)) {
        return;
    }

    if ((dst = McActIconTexAdrs(0, 0)) == NULL) {
        return;
    }

    head = (u32*)save->dat_adrs;
    src = (u8*)&save->dat_adrs[(&head[2])[pltype * 2]];
    Meltw((u16*)src, (u16*)save->exp_adrs, (&head[3])[pltype * 2]);
    icon_tex_chg(save->exp_adrs, dst, 0x4000);
}

static void save_data_store_sysdir(_save_work* save) {
    _sdir_data* data = (_sdir_data*)save->buf_adrs;

    memcpy(&data->sdir_w, &system_dir[Present_Mode], sizeof(SystemDir));
    encode_data((u16*)data, sizeof(*data));
}

static void save_data_store_replay(_save_work* save) {
    s32 i;
    u16* dst;
    u8* src;
    u8 val;
    u32* head;
    u32 pltype[2];
    _replay_data* data = (_replay_data*)save->buf_adrs;
    _sub_info* sub = (_sub_info*)&data[1];
    _REPLAY_W* rw = &Replay_w;
    struct _MINI_SAVE_W* msw = &rw->mini_save_w;
    struct _SAVE_W* sw = &save_w[Present_Mode];
    struct _REP_GAME_INFOR* rp = &Rep_Game_Infor[10];

    for (i = 0; i < 2; i++) {
        pltype[i] = sub->player[i] = rp->player_infor[i].my_char;
    }

    memcpy(&rw->game_infor, rp, sizeof(*rp));

    memcpy(&msw->Pad_Infor, &sw->Pad_Infor, sizeof(msw->Pad_Infor));
    msw->Time_Limit = sw->Time_Limit;
    memcpy(msw->Battle_Number, sw->Battle_Number, sizeof(msw->Battle_Number));
    msw->Damage_Level = sw->Damage_Level;
    memcpy(&msw->extra_option, &sw->extra_option, sizeof(msw->extra_option));

    memcpy(&rw->system_dir, &system_dir[Present_Mode], sizeof(rw->system_dir));
    memcpy(&data->replay_w, rw, sizeof(data->replay_w));
    encode_data((u16*)data, sizeof(*data));

    if ((dst = McActIconTexAdrs(2, 0)) == NULL) {
        return;
    }

    head = (u32*)save->dat_adrs;

    for (i = 0; i < 2; i++) {
        if ((val = pltype[i]) >= 20) {
            continue;
        }

        if (i != rp->winner) {
            val += 20;
        }

        src = (u8*)&save->dat_adrs[(&head[2])[val * 2]];
        Meltw((u16*)src, (u16*)save->exp_adrs, (&head[3])[val * 2]);
        icon_tex_chg(save->exp_adrs, &dst[i << 13], 0x2000);
    }
}

typedef s32 (*save_data_decode_func)(_save_work*, s32);

static s32 save_data_decode(_save_work* save, s32 mode) {
    static save_data_decode_func save_data_decode_jmp[] = { save_data_decode_system,
                                                            save_data_decode_sysdir,
                                                            save_data_decode_replay };

    return save_data_decode_jmp[save->file_type](save, mode);
}

static s32 save_data_decode_system(_save_work* save, s32 mode) {
    _save_data* data = (_save_data*)save->buf_adrs;

    decode_data(save, &data->head.version, 0x218);

    if ((save->mc_ver_err != 0) || (save->mc_sum_err != 0)) {
        if (mode != 0) {
            return -1;
        }

        memset(data, 0, sizeof(*data));
        memcpy(&data->save_w, save_w, sizeof(data->save_w));
    }

    return 0;
}

static s32 save_data_decode_sysdir(_save_work* save, s32 mode) {
    _sdir_data* data = (_sdir_data*)save->buf_adrs;

    decode_data(save, (u16*)data, sizeof(*data));

    if ((save->mc_ver_err != 0) || (save->mc_sum_err != 0)) {
        return -1;
    }

    return 0;
}

static s32 save_data_decode_replay(_save_work* save, s32 mode) {
    _replay_data* data = (_replay_data*)save->buf_adrs;

    decode_data(save, (u16*)data, sizeof(*data));

    if ((save->mc_ver_err != 0) || (save->mc_sum_err != 0)) {
        return -1;
    }

    return 0;
}

typedef void (*load_data_set_func)(_save_work*);

static void load_data_set(_save_work* save) {
    static load_data_set_func load_data_set_jmp[] = { load_data_set_system,
                                                      load_data_set_sysdir,
                                                      load_data_set_replay };

    load_data_set_jmp[save->file_type](save);
}

static void load_data_set_system(_save_work* save) {
    s32 i;
    s32 page;
    _save_data* data = (_save_data*)save->buf_adrs;
    struct _SAVE_W* sw = &save_w[Present_Mode];

    *sw = data->save_w;

    memcpy(sw, &data->save_w, sizeof(struct _SAVE_W));
    memcpy(&save_w[4], &data->save_w, sizeof(struct _SAVE_W));
    memcpy(&save_w[5], &data->save_w, sizeof(struct _SAVE_W));

    sys_w.bgm_type = sw->BgmType;
    sys_w.sound_mode = sw->SoundMode;

    bgm_level = sw->BGM_Level;
    se_level = sw->SE_Level;

    if (mpp_w.cutAnalogStickData) {
        mpp_w.cutAnalogStickData = 0;
        save_w[Present_Mode].AnalogStick = 0;
        save_w[4].AnalogStick = 0;
        save_w[5].AnalogStick = 0;
    }

    setupSoundMode();
    SsBgmHalfVolume(0);
    setSeVolume();

    Copy_Save_w();
    Copy_Check_w();

    X_Adjust = sw->Adjust_X;
    Y_Adjust = sw->Adjust_Y;

    dspwhUnpack(sw->Screen_Size, &Disp_Size_H, &Disp_Size_V);
    Setup_Disp_Size(0);

    sys_w.screen_mode = sw->Screen_Mode;

    page = Check_SysDir_Page();

    if (page < 9) {
        page += 1;

        for (; page < 10; page++) {
            for (i = 0; i < 7; i++) {
                system_dir[Present_Mode].contents[page][i] = system_dir->contents[page][i];
            }
        }
    }
}

static void load_data_set_sysdir(_save_work* save) {
    s32 i;
    s32 page;
    _sdir_data* data = (_sdir_data*)save->buf_adrs;
    SystemDir* sw = &system_dir[Present_Mode];

    memcpy(sw, &data->sdir_w, sizeof(*sw));

    page = Check_SysDir_Page();

    if (page < 9) {
        page += 1;

        for (; page < 10; page++) {
            for (i = 0; i < 7; i++) {
                system_dir[Present_Mode].contents[page][i] = system_dir->contents[page][i];
            }
        }
    }
}

static void load_data_set_replay(_save_work* save) {
    _replay_data* data = (_replay_data*)save->buf_adrs;

    memcpy(&Replay_w, &data->replay_w, sizeof(Replay_w));
}

static s32 info_data_check(_save_work* save) {
    s32 i;
    _sub_info* info = &save->info[save->cnt_0];
    memcard_date* md = &info->date;

    if (md->year > 9999) {
        return 1;
    }

    if ((md->month <= 0) || (md->month > 12)) {
        return 1;
    }

    if ((md->day <= 0) || (md->day > 31)) {
        return 1;
    }

    if (md->hour > 23) {
        return 1;
    }

    if (md->min > 59) {
        return 1;
    }

    if (md->sec > 59) {
        return 1;
    }

    if (md->dayofweek > 6) {
        return 1;
    }

    if (save->file_type == 1) {
        return 0;
    }

    for (i = 0; i < 2; i++) {
        if ((info->player[i] < 0) || (info->player[i] >= 20)) {
            return 1;
        }
    }

    return 0;
}

static void icon_tex_chg(u8* src, u16* dst, u32 size) {
    u32 align;
    TIM2_FILEHEADER* fh;
    TIM2_PICTUREHEADER* ph;

    fh = (TIM2_FILEHEADER*)src;
    align = (fh->FormatId) ? 128 : 16;
    ph = (TIM2_PICTUREHEADER*)(src + align);
    src = (u8*)ph;
    src = src + ph->HeaderSize;

    memcpy(dst, src, size * 2);
}

static void encode_data(u16* src, s32 size) {
    s32 i;
    u16 magic;
    u16* sum;

    size -= 8;
    magic = random_32_com();

    *src++ = 0x406;
    *src++ = magic;
    sum = src;
    *src++ = 0;
    *src++ = 0x5963;

    for (i = 0; i < ((size - 8) / 2); i++) {
        *sum += *src;
        *src++ ^= magic;

        if (magic == 0) {
            magic = 1;
        }

        magic = (magic * 0xB0) % 65363;
    }
}

static void decode_data(_save_work* save, u16* src, s32 size) {
    s32 i;
    u16 ver;
    u16 magic;
    u16 sum;
    u16 sum0;

    size -= 8;

    ver = *src++;
    save->mc_ver_err = (ver != 0x406) ? 1 : 0;

    magic = *src++;
    sum = *src++;
    src++;

    sum0 = 0;
    for (i = 0; i < ((size - 8) / 2); i++) {
        *src ^= magic;
        sum0 += *src++;

        if (magic == 0) {
            magic = 1;
        }

        magic = (magic * 0xB0) % 65363;
    }

    save->mc_sum_err = (sum != sum0) ? 1 : 0;
}

static void save_slot_trans(_save_work* save) {
    s32 i;
    s32 no;
    s32 len;
    s32 x;
    s32 sx;
    s32 sy;
    u32 col[2];
    u32 alp[2];
    static s32 x_tbl[2] = { 110, 274 };
    s8** tbl = GetMemCardMsg(1);

    sy = MsgLanguage == 0 ? 16 : 20;
    sx = sy * flWidth / 640;
    KnjSetSize(sx, sy);

    save->sel_slot_cnt += save->sel_slot_st != 1 ? 1 : 15;

    if (save->sel_slot_cnt >= 60) {
        save->sel_slot_cnt = 0;
    }

    no = save->sel_slot_no;
    col[no] = 0x80008080;
    alp[no] = 128.0f + (48.0f * cosf((6.28f * save->sel_slot_cnt) / 60.0f));
    col[no ^ 1] = 0x80808080;
    alp[no ^ 1] = 0x80;

    len = ((sx * KnjStrLen(tbl[1])) / 2) / 2;

    for (i = 0; i < 2; i++) {
        x = x_tbl[i] - 30;
        dispButtonImage2(x, 76, 1, 60, 64, (i == no) ? 0 : 160, 8);

        KnjSetAlpha(alp[i]);
        KnjSetRgb(col[i]);
        x = (flWidth * x_tbl[i]) / 384;
        KnjLocate(x - len, 128);
        KnjPuts((tbl + i)[1]); // ???
    }
}

static void save_file_trans(_save_work* save) {
    static s8* week_str[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    s8** tbl0 = GetMemCardMsg(1);
    s32 xs;
    s32 ys;
    s32 tx;
    s32 ty;
    s32 i;
    s32 no;
    s32 top;
    s32 max;
    u32 val;
    u32 col;
    u32 alp;
    _sub_info* info;
    memcard_date* md;

    xs = (flWidth * 22) / 640;
    ys = 22;

    KnjSetSize(xs, 16);

    save->sel_file_cnt += save->sel_file_st != 1 ? 1 : 15;

    if (save->sel_file_cnt >= 60) {
        save->sel_file_cnt = 0;
    }

    if (++save->sel_file_cnt2 >= 30) {
        save->sel_file_cnt2 = 0;
    }

    tx = (xs / 2) * 8;
    ty = 160;

    no = save->sel_slot_no;
    top = save->sel_file_top;
    max = save->sel_file_max;

    for (i = 0; i < 4; i++) {
        if ((top + i) >= max) {
            break;
        }

        if (i == save->sel_file_idx) {
            col = 0x80008080;
            alp = 128.0f + (48.0f * cosf((6.28f * save->sel_file_cnt) / 60.0f));
        } else {
            col = 0x80808080;
            alp = 0x80;
        }

        KnjSetAlpha(alp);
        KnjSetRgb(col);
        KnjLocate(tx, (i * ys) + ty);

        if ((val = save->sel_file_order[top + i]) < 0xFF) {
            info = &save->info[val];
            md = &info->date;

            KnjPrintf("[FILE %02d]  ", val);

            if (md->dayofweek < 254) {
                KnjPrintf("%04d-%02d-%02d %02d:%02d:%02d (%s)",
                          md->year,
                          md->month,
                          md->day,
                          md->hour,
                          md->min,
                          md->sec,
                          week_str[md->dayofweek]);

                if (save->file_type == 2) {
                    KnjPrintf("  %s vs %s", pl_name[info->player[0]], pl_name[info->player[1]]);
                }
            } else {
                KnjPuts("BROKEN FILE");
            }
        } else {
            KnjPuts("- - - - - - - - - - NEW FILE - - - - - - - - - -");
        }
    }

    KnjSetColor(0x80008000);
    alp = 176.0f + (48.0f * cosf((6.28f * save->sel_file_cnt2) / 30.0f));
    KnjSetAlpha(alp);

    if (top > 0) {
        KnjLocate(tx + 8, (ty - ys) - 4);

        if (MsgLanguage == 0) {
            KnjPuts("▲");
        } else {
            KnjPuts("Up");
        }
    }

    if ((top + 4) < max) {
        KnjLocate(tx + 8, (ys * 4) + ty + 4);

        if (MsgLanguage == 0) {
            KnjPuts("▼");
        } else {
            KnjPuts("Down");
        }
    }
}

static void save_msg_trans(_save_work* save) {
    s32 xs;
    s32 ys;
    s32 len;
    s32 i;
    s32 no;
    s32 x;
    s32 y;
    s32 r_flag;
    s8** tbl0;
    s8** tbl;
    s8* msg;
    s8* p;
    s8* q;
    s8 buf[80];
    s8 tmp[80];
    s8 tmp2[32];
    u32 yn;
    u32 col[2];
    u32 alp[2];

    if ((no = save->mc_msg_no) == 0) {
        return;
    }

    tbl0 = GetMemCardMsg(1);
    tbl = GetMemCardMsg(no);

    xs = (flWidth * 20) / 640;
    ys = 20;

    KnjSetSize(xs, ys);
    KnjSetColor(0x80808080);

    switch (save->save_mode) {
    default:
        x = 48;
        y = 288;
        break;

    case 2:
        x = 48;
        y = 240;
        break;

    case 3:
        x = 64;
        y = 192;
        break;
    }

    if (save->r_no_0 == 6) {
        x = 64;
        y = 192;
        KnjLocate(x, y - 44);
        KnjPuts("MESSAGE No = ");
        sprintf(tmp, "%d", no);
        KnjSetColor(0x80008080);
        KnjPuts(tmp);
        KnjSetColor(0x80808080);
    }

    r_flag = 0;

    while ((msg = *tbl++)) {
        buf[0] = 0;
        q = tmp;
        strcpy(tmp, msg);

    loop:
        if ((p = strchr(q, '%')) == NULL) {
            goto loop_end;
        }

        if (p[1] == 's') {
            p[0] = '\0';
            strcat(buf, q);
            strcat(buf, "<C2>");
            q = p + 2;

            if (MsgLanguage == 0) {
                McActZenNum(save->sel_slot_no + 1, tmp2, 0, 0);
            } else {
                tmp2[0] = save->sel_slot_no + '1';
                tmp2[1] = 0;
            }

            strcat(buf, tmp2);
            strcat(buf, "<C7>");
            goto loop;
        }

        if (p[1] == 'a') {
            p[0] = '\0';
            strcat(buf, q);
            strcat(buf, "<C2>");
            q = p + 2;

            if (MsgLanguage == 0) {
                McActZenNum(save->avail_size, tmp2, 0, 0);
            } else {
                sprintf(tmp2, "%d", save->avail_size);
            }

            strcat(buf, tmp2);
            strcat(buf, "<C7>");
            goto loop;
        }

        if (p[1] == 'r') {
            r_flag = 1;
            p[0] = '\0';
            goto loop;
        }

        if (p[1] == 'e') {
            p[0] = '\0';
            strcat(buf, q);
            strcat(buf, (&tbl0[save->save_mode])[9]);
            q = p + 2;
            goto loop;
        }

        if (p[1] == 'E') {
            p[0] = '\0';
            strcat(buf, q);
            strcpy(tmp2, (&tbl0[save->save_mode])[9]);

            if ((tmp2[0] >= 'a') && (tmp2[0] <= 'z')) {
                tmp2[0] -= ' ';
            }

            strcat(buf, tmp2);
            q = p + 2;
            goto loop;
        }

        if (p[1] == 'f') {
            p[0] = '\0';
            strcat(buf, q);
            strcat(buf, (&tbl0[save->file_type])[13]);
            q = p + 2;
            goto loop;
        }

    loop_end:
        strcat(buf, q);
        FormStrDisp(x, y, buf, 0);
        y += ys + 2;
    }

    if (r_flag == 1) {
        if (++save->mc_ra_cnt >= 30) {
            save->mc_ra_cnt = 0;
        }

        alp[0] = 64.0f - (64.0f * cosf((6.28f * save->mc_ra_cnt) / 30.0f));

        if (alp[0] != 0) {
            KnjSetAlpha(alp[0]);
            KnjPuts(tbl0[0]);
        }
    }

    switch (no) {
    case 10:
    case 14:
    case 19:
    case 24:
    case 27:
    case 28:
    case 29:
    case 30:
    case 34:
        y += 8;
        save->mc_yn_cnt += (save->mc_yn_ok == 0) ? 1 : 15;

        if (save->mc_yn_cnt >= 60) {
            save->mc_yn_cnt = 0;
        }

        yn = save->yes_no_flag - 1;
        col[yn] = 0x80008080;
        alp[yn] = 128.0f + (48.0f * cosf((6.28f * save->mc_yn_cnt) / 60.0f));
        col[yn ^ 1] = 0x80808080;
        alp[yn ^ 1] = 0x80;

        len = KnjStrLen(tbl0[3]);
        KnjSetColor(0x80808080);
        KnjLocate(x + 32 + xs * (len + 1) / 2, y);
        KnjPuts("/");

        for (i = 0; i < 2; i++) {
            KnjSetAlpha(alp[i]);
            KnjSetRgb(col[i]);
            KnjLocate(x + 32 + (xs * (len + 3) / 2) * i, y);
            KnjPuts((&tbl0[i])[3]);
        }

        break;
    }
}
