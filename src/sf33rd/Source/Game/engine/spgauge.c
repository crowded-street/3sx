/**
 * @file spgauge.c
 * Super Art Gauge Controller
 */

#include "sf33rd/Source/Game/engine/spgauge.h"
#include "common.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/sound/se.h"

typedef struct {
    const u16* spgtbl_ptr;
    const u16* spgptbl_ptr;
    s16 current_spg;
    s16 old_spg;
    s16 spgcol_number;
    s16 spg_level;
    s16 spg_maxlevel;
    s16 spg_len;
    s16 spg_dotlen;
    s16 flag;
    s16 flag2;
    s16 level_flag;
    s16 timer;
    s16 timer2;
    s8 kind;
    s8 max;
    s8 max_old;
    s8 max_rno;
    s8 time;
    s8 time_rno;
    s16 gauge_flash_time;
    s16 gauge_flash_col;
    u16 mchar;
    u16 mass_len;
    s8 sa_flag;
    s8 ex_flag;
    s8 no_chgcol;
    s8 time_no_clear;
    s8 sa_mukou;
} SPG_DAT;

// sbss
s8 Old_Stop_SG;
s8 Exec_Wipe_F;
s8 time_clear[2];
s16 spg_number;
s16 spg_work;
s16 spg_offset;
s8 time_num;
s8 time_timer;
s8 time_flag[2];
s16 col;
s8 time_operate[2];
s8 sast_now[2];
s8 max2[2];
s8 max_rno2[2];
SPG_DAT spg_dat[2];

const u16 spgauge_tbl[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

const u16 spg1p_npos_tbl[16] = { 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6 };

const u16 spg2p_npos_tbl[16] = { 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41 };

const u16 sa_time_data_tbl[6][2] = { { 0, 16 }, { 4, 20 }, { 8, 24 }, { 12, 28 }, { 8, 24 }, { 4, 20 } };

const u16 sa_color_data2_tbl[3][2] = { { 13, 141 }, { 12, 140 }, { 13, 141 } };

const u16 sagauge_colchg_tbl[4][2] = { { 17, 145 }, { 18, 146 }, { 19, 147 }, { 18, 146 } };

const u16* spgauge_puttbl[2] = { spgauge_tbl, spgauge_tbl };
const u16* spgauge_postbl[2] = { spg1p_npos_tbl, spg2p_npos_tbl };

void spgauge_control(s8 Spg_Num);
void wipe_check();
void satime_ko_after_clear(s8 Stpl_Num);
void sa_time_moji_send();
void samoji_control(s8 Stpl_Num);
void sast_control(s8 Stpl_Num);
void sast_color_chenge(s8 Stpl_Num);
void sagauge_color_chenge(s8 Stpl_Num);
void sa_moji_trans(s8 Stpl_Num, s8 Kind, s8 OnOff);
void sa_gauge_trans(s8 pl_kind);
void spgauge_sound_request(s8 Stpl_Num);
void spgauge_work_clear(s8 Stpl_Num);
void spgauge_wipe_write(s8 Stpl_Num);
void sa_waku_trans(s8 Stpl_Num, s8 Spg_Col);

void spgauge_cont_init() {
    s8 lpy;

    Sa_frame_Clear();

    for (lpy = 0; lpy < 2; lpy++) {
        spg_dat[lpy].current_spg = 0;
        spg_dat[lpy].old_spg = 0;
        spg_dat[lpy].spgtbl_ptr = spgauge_puttbl[lpy];
        spg_dat[lpy].spg_level = 0;
        spg_dat[lpy].spg_maxlevel = super_arts[lpy].store_max;
        spg_dat[lpy].spg_len = super_arts[lpy].gauge_len / 8;
        spg_dat[lpy].spg_dotlen = super_arts[lpy].gauge_len;
        spg_dat[lpy].flag = 0;
        spg_dat[lpy].flag2 = 0;
        spg_dat[lpy].level_flag = 0;
        spg_dat[lpy].timer = 51;
        spg_dat[lpy].timer2 = 2;
        spg_dat[lpy].kind = 0;
        spg_dat[lpy].max = 0;
        spg_dat[lpy].max_old = 0;
        spg_dat[lpy].max_rno = 0;
        spg_dat[lpy].time_rno = 0;
        spg_dat[lpy].gauge_flash_time = 2;
        spg_dat[lpy].gauge_flash_col = 0;
        spg_dat[lpy].sa_flag = 0;
        spg_dat[lpy].ex_flag = 0;
        spg_dat[lpy].no_chgcol = 0;
        spg_dat[lpy].time_no_clear = 0;
        spg_dat[lpy].sa_mukou = 0;
        sa_gauge_flash[lpy] = 0;
        spg_dat[lpy].spgptbl_ptr = spgauge_postbl[lpy];

        if (super_arts[lpy].gauge_type == 1) {
            spg_dat[lpy].time = 1;
            time_flag[lpy] = 1;
        } else {
            spg_dat[lpy].time = 0;
            time_flag[lpy] = 0;
        }

        spg_dat[lpy].mass_len = spg_dat[lpy].spg_len - 5;

        if (spg_dat[lpy].spg_len & 1) {
            spg_dat[lpy].mchar = 5;
        } else {
            spg_dat[lpy].mass_len = spg_dat[lpy].mass_len - 1;
            spg_dat[lpy].mchar = 6;
        }

        spg_dat[lpy].mass_len /= 2;
        time_operate[lpy] = 0;
        sast_now[lpy] = 0;
        max2[lpy] = 0;
        max_rno2[lpy] = 0;
    }

    spg_dat[0].spgcol_number = 17;
    spg_dat[1].spgcol_number = 145;
    sa_stock_trans(0, 0, 0);
    sa_stock_trans(0, 0, 1);
    sa_waku_trans(0, 0);
    sa_waku_trans(1, 0);
    sa_gauge_trans(0);
    sa_gauge_trans(1);
    Old_Stop_SG = 0;
    Exec_Wipe_F = 0;
    time_clear[0] = 0;
    time_clear[1] = 0;
    spg_offset = 0;
    time_num = 0;
    time_timer = 3;
    col = 0;
}

void spgauge_cont_demo_init() {
    s8 lpy;

    Sa_frame_Clear();

    for (lpy = 0; lpy < 2; lpy++) {
        demo_set_sa_full(&super_arts[lpy]);
        spg_dat[lpy].current_spg = super_arts[lpy].gauge_len;
        spg_dat[lpy].old_spg = super_arts[lpy].gauge_len;
        spg_dat[lpy].spgtbl_ptr = spgauge_puttbl[lpy];
        spg_dat[lpy].spg_level = super_arts[lpy].store;
        spg_dat[lpy].spg_maxlevel = super_arts[lpy].store_max;
        spg_dat[lpy].spg_len = super_arts[lpy].gauge_len / 8;
        spg_dat[lpy].spg_dotlen = super_arts[lpy].gauge_len;
        spg_dat[lpy].flag = 1;
        spg_dat[lpy].flag2 = 0;
        spg_dat[lpy].level_flag = 0;
        spg_dat[lpy].timer = 51;
        spg_dat[lpy].timer2 = 2;
        spg_dat[lpy].kind = 1;
        spg_dat[lpy].max = 0;
        spg_dat[lpy].max_old = 1;
        spg_dat[lpy].max_rno = 2;
        spg_dat[lpy].time_rno = 5;
        spg_dat[lpy].gauge_flash_time = 2;
        spg_dat[lpy].gauge_flash_col = 0;
        spg_dat[lpy].sa_flag = 0;
        spg_dat[lpy].ex_flag = 0;
        spg_dat[lpy].no_chgcol = 0;
        spg_dat[lpy].time_no_clear = 0;
        spg_dat[lpy].sa_mukou = 0;
        sa_gauge_flash[lpy] = 0;
        spg_dat[lpy].spgptbl_ptr = spgauge_postbl[lpy];

        if (super_arts[lpy].gauge_type == 1) {
            spg_dat[lpy].time = 1;
            time_flag[lpy] = 1;
        } else {
            spg_dat[lpy].time = 0;
            time_flag[lpy] = 0;
        }

        spg_dat[lpy].mass_len = spg_dat[lpy].spg_len - 5;

        if (spg_dat[lpy].spg_len & 1) {
            spg_dat[lpy].mchar = 5;
        } else {
            spg_dat[lpy].mass_len = spg_dat[lpy].mass_len - 1;
            spg_dat[lpy].mchar = 6;
        }

        spg_dat[lpy].mass_len /= 2;
        time_operate[lpy] = 0;
        sast_now[lpy] = 0;
        max2[lpy] = 0;
        max_rno2[lpy] = 0;
    }

    spg_dat[0].spgcol_number = 17;
    spg_dat[1].spgcol_number = 145;
    sa_stock_trans(spg_dat[0].spg_maxlevel, 1, 0);
    sa_stock_trans(spg_dat[1].spg_maxlevel, 1, 1);
    sa_waku_trans(0, 1);
    sa_waku_trans(1, 1);
    sa_gauge_trans(0);
    sa_gauge_trans(1);
    sa_moji_trans(0, 0, 1);
    sa_moji_trans(1, 0, 1);
    Old_Stop_SG = 0;
    Exec_Wipe_F = 0;
    time_clear[0] = 0;
    time_clear[1] = 0;
    spg_offset = 0;
    time_num = 0;
    time_timer = 3;
    col = 1;
}

void spgauge_cont_main() {
    u8 i;

    for (i = 0; i < 2; i++) {
        spgauge_base_put(i, spg_dat[i].spg_len);
    }

    if ((Game_pause & 0x80) || EXE_flag) {
        return;
    }

    if (Stop_SG) {
        wipe_check();
        Old_Stop_SG = Stop_SG;
        return;
    }

    if (Old_Stop_SG) {
        Old_Stop_SG = 0;
        Exec_Wipe_F = 0;
        time_clear[0] = 0;
        time_clear[1] = 0;
    }

    sa_time_moji_send();

    if (gauge_stop_flag[0] == 0) {
        spgauge_control(0);
    }

    if (gauge_stop_flag[1] == 0) {
        spgauge_control(1);
    }
}

void sag_bug_fix(s32 side) {
    spg_dat[side].max = spg_dat[side].max_old = spg_dat[side].max_rno = 0;
}

void spgauge_control(s8 Spg_Num) {
    if (sa_gauge_flash[Spg_Num] != 0 && plw[Spg_Num].sa->ex == -1) {
        spgauge_sound_request(Spg_Num);

        if (Conclusion_Flag != 0) {
            spg_dat[Spg_Num].sa_mukou = 1;
        }

        spg_dat[Spg_Num].max = 0;
        spg_dat[Spg_Num].max_rno = 0;
        spg_dat[Spg_Num].flag = 1;
        spg_dat[Spg_Num].flag2 = 1;
        spg_dat[Spg_Num].time_rno = 0;
        spg_dat[Spg_Num].timer2 = 2;
        spg_dat[Spg_Num].kind = 0;
        spg_dat[Spg_Num].no_chgcol = 0;
        spg_dat[Spg_Num].ex_flag = 1;
        spg_dat[Spg_Num].timer = 16;
        sa_gauge_flash[Spg_Num] &= ~2;
    } else if (sast_now[Spg_Num] == 0 && spg_dat[Spg_Num].flag2 == 0 && sa_gauge_flash[Spg_Num] != 0) {
        spgauge_sound_request(Spg_Num);

        if (super_arts[Spg_Num].gt2 == 1) {
            spg_dat[Spg_Num].time = 1;
            time_flag[Spg_Num] = 1;
        } else {
            spg_dat[Spg_Num].time = 0;
            time_flag[Spg_Num] = 0;
        }

        if (plw[Spg_Num].sa->store == plw[Spg_Num].sa->store_max && spg_dat[Spg_Num].max_old == 0 &&
            spg_dat[Spg_Num].max == 0) {
            spg_dat[Spg_Num].max = 1;
        } else {
            spg_dat[Spg_Num].max = 0;
        }

        spg_dat[Spg_Num].flag = 1;
        spg_dat[Spg_Num].flag2 = 1;
        spg_dat[Spg_Num].max_rno = 0;
        spg_dat[Spg_Num].time_rno = 0;
        spg_dat[Spg_Num].timer2 = 2;
        spg_dat[Spg_Num].kind = 0;
        spg_dat[Spg_Num].no_chgcol = 0;

        if (plw[Spg_Num].sa->ok == -1 || plw[Spg_Num].sa->mp == -1) {
            spg_dat[Spg_Num].sa_flag = 1;
            spg_dat[Spg_Num].timer = 51;

            if (Conclusion_Flag != 0) {
                if ((spg_dat[Spg_Num].time) == 1) {
                    spg_dat[Spg_Num].time_no_clear = 1;
                }

                if (My_char[Spg_Num] == 0 && ((plw[Spg_Num].sa->ok) == -1)) {
                    spg_dat[Spg_Num].sa_mukou = 0;
                } else {
                    spg_dat[Spg_Num].sa_mukou = 1;
                }
            }

            sa_gauge_flash[Spg_Num] &= ~4;
        } else {
            spg_dat[Spg_Num].timer = 25;
            sa_gauge_flash[Spg_Num] &= ~1;
        }
    }

    if (spg_dat[Spg_Num].max) {
        samoji_control(Spg_Num);
    } else if (spg_dat[Spg_Num].flag) {
        sast_control(Spg_Num);
    }

    if ((plw[Spg_Num].sa->ex != 0 || spg_dat[Spg_Num].ex_flag == 1 || spg_dat[Spg_Num].sa_flag == 1) && !Game_pause) {
        sagauge_color_chenge(Spg_Num);
    }

    if (spg_dat[Spg_Num].current_spg != plw[Spg_Num].sa->gauge.s.h || spg_dat[Spg_Num].max != 0) {
        if (spg_dat[Spg_Num].max) {
            spg_dat[Spg_Num].current_spg = spg_dat[Spg_Num].spg_dotlen;
        } else {
            spg_dat[Spg_Num].current_spg = plw[Spg_Num].sa->gauge.s.h;
        }

        if (max2[Spg_Num] != 1 && spg_dat[Spg_Num].max == 0 && spg_dat[Spg_Num].flag == 0 &&
            spg_dat[Spg_Num].max_old == 0 && spg_dat[Spg_Num].time_no_clear == 0) {
            sa_gauge_trans(Spg_Num);
        }
    }
}

void wipe_check() {
    if (Old_Stop_SG) {
        if (Exec_Wipe != 0) {
            return;
        }

        if (Exec_Wipe_F != 0) {
            return;
        }

        Exec_Wipe_F = 1;

        if (spg_dat[0].time == 1 && time_clear[0] == 1) {
            if (spg_dat[0].time_no_clear == 0) {
                spgauge_work_clear(0);
                sc_clear(1, 25, 4, 26);
                sast_color_chenge(0);
                spgauge_wipe_write(0);
            } else {
                satime_ko_after_clear(0);
            }
        }

        if (spg_dat[1].time == 1 && time_clear[1] == 1) {
            if (spg_dat[1].time_no_clear == 0) {
                spgauge_work_clear(1);
                sc_clear(43, 25, 46, 26);
                sast_color_chenge(1);
                spgauge_wipe_write(1);
            } else {
                satime_ko_after_clear(1);
            }
        }

    } else {
        if (spg_dat[0].time_no_clear == 1 || plw[0].sa->ok == -1) {
            plw[0].sa->ok = 0;
            time_clear[0] = 1;
            spg_dat[0].spg_level = plw[0].sa->store;
        }

        if (spg_dat[1].time_no_clear == 1 || plw[1].sa->ok == -1) {
            plw[1].sa->ok = 0;
            time_clear[1] = 1;
            spg_dat[1].spg_level = plw[1].sa->store;
        }
    }
}

void satime_ko_after_clear(s8 Stpl_Num) {
    spg_dat[Stpl_Num].max = 0;

    if (plw[Stpl_Num].sa->store == spg_dat[Stpl_Num].spg_maxlevel) {
        spg_dat[Stpl_Num].max_old = 1;
        spg_dat[Stpl_Num].max_rno = 2;
    } else {
        spg_dat[Stpl_Num].max_old = 0;
        spg_dat[Stpl_Num].max_rno = 0;
    }

    spg_dat[Stpl_Num].kind = 1;
    spg_dat[Stpl_Num].timer2 = 2;
    spg_dat[Stpl_Num].time_rno = 5;
    spg_dat[Stpl_Num].time_no_clear = 0;
    plw[Stpl_Num].sa->gauge.s.h = spg_dat[Stpl_Num].current_spg = plw[Stpl_Num].sa->bacckup_g_h;
    plw[Stpl_Num].sa->bacckup_g_h = 0;
}

void sa_time_moji_send() {
    if (time_flag[0] == 0 && time_flag[1] == 0) {
        return;
    }

    time_timer--;

    if (time_timer != 0) {
        return;
    }

    if (time_flag[0] == 1 && time_operate[0] == 1) {
        scfont_sqput2(1, 25, 11, 0, 2, sa_time_data_tbl[time_num][0], 0, 4, 2);
    }

    if (time_flag[1] == 1 && time_operate[1] == 1) {
        scfont_sqput2(43, 25, 11, 0, 2, sa_time_data_tbl[time_num][1], 0, 4, 2);
    }

    time_timer = 3;

    if (time_num == 5) {
        time_num = 0;
        return;
    }

    time_num++;
}

void samoji_control(s8 Stpl_Num) {
    switch (spg_dat[Stpl_Num].max_rno) {
    case 0:
        max2[Stpl_Num] = 0;
        max_rno2[Stpl_Num] = 1;
        sa_moji_trans(Stpl_Num, 0, 1);
        spg_dat[Stpl_Num].spg_level = plw[Stpl_Num].sa->store;
        sa_stock_trans(spg_dat[Stpl_Num].spg_level, 1, Stpl_Num);
        sa_waku_trans(Stpl_Num, 1);
        sagauge_color_chenge(Stpl_Num);
        spg_dat[Stpl_Num].max_rno = 1;
        break;

    case 1:
        spg_dat[Stpl_Num].timer--;

        if (spg_dat[Stpl_Num].timer) {
            spg_dat[Stpl_Num].timer2--;

            if (spg_dat[Stpl_Num].timer2 != 0) {
                break;
            }

            spg_dat[Stpl_Num].kind++;

            if (spg_dat[Stpl_Num].kind == 3) {
                spg_dat[Stpl_Num].kind = 0;
            }

            sa_waku_trans(Stpl_Num, spg_dat[Stpl_Num].kind);
            sa_stock_trans(spg_dat[Stpl_Num].spg_level, spg_dat[Stpl_Num].kind, Stpl_Num);
            spg_dat[Stpl_Num].timer2 = 1;
        } else {
            sa_waku_trans(Stpl_Num, 1);
            sa_stock_trans(spg_dat[Stpl_Num].spg_level, 1, Stpl_Num);
            spg_dat[Stpl_Num].max = 0;
            spg_dat[Stpl_Num].max_old = 1;
            spg_dat[Stpl_Num].kind = 1;
            spg_dat[Stpl_Num].timer2 = 2;
            spg_dat[Stpl_Num].max_rno = 2;
            spg_dat[Stpl_Num].time_rno = 5;
            spg_dat[Stpl_Num].flag2 = 0;
            max_rno2[Stpl_Num] = 0;
        }

        break;

    case 2:
        break;
    }
}

void sast_control(s8 Stpl_Num) {
    sast_now[Stpl_Num] = 1;

    if (spg_dat[Stpl_Num].time) {
        switch (spg_dat[Stpl_Num].time_rno) {
        case 0:
            if (plw[Stpl_Num].sa->ok == -1) {
                spg_dat[Stpl_Num].time_rno = 1;
            } else {
                if (plw[Stpl_Num].sa->store > spg_dat[Stpl_Num].spg_level) {
                    spg_dat[Stpl_Num].current_spg = spg_dat[Stpl_Num].spg_dotlen;
                    sa_gauge_trans(Stpl_Num);
                    spg_dat[Stpl_Num].spg_level = plw[Stpl_Num].sa->store;
                    sa_stock_trans(spg_dat[Stpl_Num].spg_level, col, Stpl_Num);
                }

                spg_dat[Stpl_Num].time_rno = 3;
                goto case_3;
            }

            /* fallthrough */

        case 1:
            spg_dat[Stpl_Num].timer--;

            if ((!spg_dat[Stpl_Num].sa_mukou || spg_dat[Stpl_Num].timer != 0) &&
                (spg_dat[Stpl_Num].spg_level == plw[Stpl_Num].sa->store)) {
                spg_dat[Stpl_Num].timer2--;

                if (spg_dat[Stpl_Num].kind == 0) {
                    if (spg_dat[Stpl_Num].timer2 == 0) {
                        sa_stock_trans(spg_dat[Stpl_Num].spg_level, 0, Stpl_Num);
                        sa_waku_trans(Stpl_Num, 0);
                        spg_dat[Stpl_Num].kind = 1;
                        spg_dat[Stpl_Num].timer2 = 2;
                    }
                } else if (spg_dat[Stpl_Num].timer2 == 0) {
                    sa_stock_trans(spg_dat[Stpl_Num].spg_level, 1, Stpl_Num);
                    sa_waku_trans(Stpl_Num, 1);
                    spg_dat[Stpl_Num].kind = 0;
                    spg_dat[Stpl_Num].timer2 = 2;
                }

                return;
            }

            if (spg_dat[Stpl_Num].sa_flag != 0 && spg_dat[Stpl_Num].sa_mukou == 0) {
                sa_waku_trans(Stpl_Num, 1);
                sa_moji_trans(Stpl_Num, 1, 1);
                time_operate[Stpl_Num] = 1;
                sast_color_chenge(Stpl_Num);
                spg_dat[Stpl_Num].time_rno = 2;
                spg_dat[Stpl_Num].no_chgcol = 1;

                spg_dat[Stpl_Num].current_spg = spg_dat[Stpl_Num].spg_dotlen;
                sa_gauge_trans(Stpl_Num);
                return;
            }

            goto jump;

        case 2:
            if (spg_dat[Stpl_Num].current_spg > 0 && plw[Stpl_Num].sa->ok == -1) {
                if (spg_dat[Stpl_Num].current_spg != spg_dat[Stpl_Num].old_spg) {
                    sa_gauge_trans(Stpl_Num);
                }

                spg_dat[Stpl_Num].old_spg = spg_dat[Stpl_Num].current_spg;
            } else {
                spg_dat[Stpl_Num].time_rno = 4;
            }

            return;

        case 3:
        case_3:
            spg_dat[Stpl_Num].timer--;

            if (spg_dat[Stpl_Num].timer) {
                spg_dat[Stpl_Num].timer2--;

                if (spg_dat[Stpl_Num].kind == 0) {
                    if (spg_dat[Stpl_Num].timer2 == 0) {
                        sa_stock_trans(spg_dat[Stpl_Num].spg_level, 0, Stpl_Num);
                        sa_waku_trans(Stpl_Num, 0);
                        spg_dat[Stpl_Num].kind = 1;
                        spg_dat[Stpl_Num].timer2 = 2;
                    }
                } else {
                    if (spg_dat[Stpl_Num].timer2 == 0) {
                        sa_stock_trans(spg_dat[Stpl_Num].spg_level, 1, Stpl_Num);
                        sa_waku_trans(Stpl_Num, 1);
                        spg_dat[Stpl_Num].kind = 0;
                        spg_dat[Stpl_Num].timer2 = 2;
                    }
                }

                return;
            }

            if ((spg_dat[Stpl_Num].ex_flag == 1 && omop_use_ex_gauge_ix[Stpl_Num] == 0) &&
                (spg_dat[Stpl_Num].max_old == 1 || max_rno2[Stpl_Num] == 1)) {
                break;
            }

            if (plw[Stpl_Num].sa->store == plw[Stpl_Num].sa->store_max) {
                max2[Stpl_Num] = 1;
            }

            if (spg_dat[Stpl_Num].sa_mukou == 1) {
                max2[Stpl_Num] = 0;
            }

            spg_dat[Stpl_Num].time_rno = 4;
            /* fallthrough */

        case 4:
            if (spg_dat[Stpl_Num].sa_mukou == 0) {
                sa_moji_trans(Stpl_Num, 1, 0);
                spg_dat[Stpl_Num].max_old = 0;
            }

        jump:
            time_operate[Stpl_Num] = 0;
            sast_color_chenge(Stpl_Num);
            spg_dat[Stpl_Num].spg_level = plw[Stpl_Num].sa->store;
            sa_stock_trans(spg_dat[Stpl_Num].spg_level, col, Stpl_Num);
            sa_waku_trans(Stpl_Num, col);

            if (max2[Stpl_Num] == 0 && spg_dat[Stpl_Num].sa_mukou == 0) {
                sa_gauge_trans(Stpl_Num);
            }

            spg_dat[Stpl_Num].flag = 0;
            spg_dat[Stpl_Num].time_rno = 5;
            spg_dat[Stpl_Num].max_rno = 0;
            spg_dat[Stpl_Num].sa_flag = 0;
            spg_dat[Stpl_Num].ex_flag = 0;
            spg_dat[Stpl_Num].no_chgcol = 0;
            spg_dat[Stpl_Num].sa_mukou = 0;

            spg_dat[Stpl_Num].flag2 = 0;
            sast_now[Stpl_Num] = 0;
            return;

        default:
        case 5:
            sast_now[Stpl_Num] = 0;
            return;
        }

        sast_color_chenge(Stpl_Num);
        sa_stock_trans(spg_dat[Stpl_Num].spg_level, col, Stpl_Num);
        sa_waku_trans(Stpl_Num, col);

        spg_dat[Stpl_Num].flag = 1;
        spg_dat[Stpl_Num].time_rno = 5;
        spg_dat[Stpl_Num].max_old = 1;
        spg_dat[Stpl_Num].max_rno = 2;
        spg_dat[Stpl_Num].sa_flag = 0;
        spg_dat[Stpl_Num].ex_flag = 0;
        spg_dat[Stpl_Num].no_chgcol = 0;
        spg_dat[Stpl_Num].sa_mukou = 0;
        spg_dat[Stpl_Num].flag2 = 0;
        max2[Stpl_Num] = 0;
        max_rno2[Stpl_Num] = 0;
        sast_now[Stpl_Num] = 0;
        return;
    }

    switch (spg_dat[Stpl_Num].max_rno) {
    case 0:
        if (plw[Stpl_Num].sa->store > spg_dat[Stpl_Num].spg_level) {
            spg_dat[Stpl_Num].current_spg = spg_dat[Stpl_Num].spg_dotlen;
            sa_gauge_trans(Stpl_Num);
            spg_dat[Stpl_Num].spg_level = plw[Stpl_Num].sa->store;
            sa_stock_trans(spg_dat[Stpl_Num].spg_level, col, Stpl_Num);
        }

        spg_dat[Stpl_Num].max_rno = 1;
        /* fallthrough */

    case 1:
        spg_dat[Stpl_Num].timer--;

        if (spg_dat[Stpl_Num].timer) {
            spg_dat[Stpl_Num].timer2--;

            if (spg_dat[Stpl_Num].kind == 0) {
                if (spg_dat[Stpl_Num].timer2 == 0) {
                    sa_stock_trans(spg_dat[Stpl_Num].spg_level, 0, Stpl_Num);
                    sa_waku_trans(Stpl_Num, 0);
                    spg_dat[Stpl_Num].kind = 1;
                    spg_dat[Stpl_Num].timer2 = 2;
                }
            } else if (spg_dat[Stpl_Num].timer2 == 0) {
                sa_stock_trans(spg_dat[Stpl_Num].spg_level, 1, Stpl_Num);
                sa_waku_trans(Stpl_Num, 1);
                spg_dat[Stpl_Num].kind = 0;
                spg_dat[Stpl_Num].timer2 = 2;
            }

            return;
        }

        if (plw[Stpl_Num].sa->store == plw[Stpl_Num].sa->store_max) {
            max2[Stpl_Num] = 1;
        }

        if (spg_dat[Stpl_Num].sa_mukou == 1) {
            max2[Stpl_Num] = 0;
        }

        if ((spg_dat[Stpl_Num].ex_flag == 1 && omop_use_ex_gauge_ix[Stpl_Num] == 0) &&
            (spg_dat[Stpl_Num].max_old == 1 || max_rno2[Stpl_Num] == 1)) {
            break;
        }

        if (spg_dat[Stpl_Num].max_old != 0 && spg_dat[Stpl_Num].sa_mukou == 0) {
            sa_moji_trans(Stpl_Num, 0, 0);
            spg_dat[Stpl_Num].max_old = 0;
        }

        sast_color_chenge(Stpl_Num);
        spg_dat[Stpl_Num].spg_level = plw[Stpl_Num].sa->store;
        sa_stock_trans(spg_dat[Stpl_Num].spg_level, col, Stpl_Num);
        sa_waku_trans(Stpl_Num, col);

        if (max2[Stpl_Num] == 0 && spg_dat[Stpl_Num].max_old == 0 && spg_dat[Stpl_Num].sa_mukou == 0) {
            sa_gauge_trans(Stpl_Num);
        }

        spg_dat[Stpl_Num].flag = 0;
        spg_dat[Stpl_Num].max_rno = 2;
        spg_dat[Stpl_Num].sa_flag = 0;
        spg_dat[Stpl_Num].ex_flag = 0;
        spg_dat[Stpl_Num].sa_mukou = 0;

        spg_dat[Stpl_Num].flag2 = 0;
        sast_now[Stpl_Num] = 0;
        /* fallthrough */

    default:
    case 2:
        sast_now[Stpl_Num] = 0;
        return;
    }

    sast_color_chenge(Stpl_Num);
    sa_stock_trans(spg_dat[Stpl_Num].spg_level, col, Stpl_Num);
    sa_waku_trans(Stpl_Num, col);

    spg_dat[Stpl_Num].flag = 1;
    spg_dat[Stpl_Num].max_old = 1;
    spg_dat[Stpl_Num].max_rno = 2;
    spg_dat[Stpl_Num].sa_flag = 0;
    spg_dat[Stpl_Num].ex_flag = 0;
    spg_dat[Stpl_Num].sa_mukou = 0;
    spg_dat[Stpl_Num].flag2 = 0;

    max2[Stpl_Num] = 0;
    max_rno2[Stpl_Num] = 0;
    sast_now[Stpl_Num] = 0;
}

void sast_color_chenge(s8 Stpl_Num) {
    if (plw[Stpl_Num].sa->gauge_type == 1 && plw[Stpl_Num].sa->ok == -1) {
        col = 1;

        if (Stpl_Num == 0) {
            spg_dat[0].spgcol_number = 14;
        } else {
            spg_dat[1].spgcol_number = 142;
        }

        return;
    } else if (plw[Stpl_Num].sa->store) {
        col = 1;

        if (Stpl_Num == 0) {
            spg_dat[0].spgcol_number = 18;
        } else {
            spg_dat[1].spgcol_number = 146;
        }
    } else {
        col = 0;

        if (Stpl_Num == 0) {
            spg_dat[0].spgcol_number = 17;
        } else {
            spg_dat[1].spgcol_number = 145;
        }
    }
}

void sa_color_chenge(s8 Stpl_Num) {
    if (spg_dat[Stpl_Num].kind) {
        if (Stpl_Num == 0) {
            spg_dat[0].spgcol_number = 18;
        } else {
            spg_dat[1].spgcol_number = 146;
        }
    } else {
        if (Stpl_Num == 0) {
            spg_dat[0].spgcol_number = 17;
        } else {
            spg_dat[1].spgcol_number = 145;
        }
    }
}

void sagauge_color_chenge(s8 Stpl_Num) {
    if (spg_dat[Stpl_Num].no_chgcol) {
        return;
    }

    spg_dat[Stpl_Num].gauge_flash_time--;

    if (spg_dat[Stpl_Num].gauge_flash_time != 0) {
        return;
    }

    spg_dat[Stpl_Num].gauge_flash_time = 2;

    if (Stpl_Num == 0) {
        sq_paint_chenge(6, 26, spg_dat[0].spg_len, 1, sagauge_colchg_tbl[spg_dat[0].gauge_flash_col][0]);
    } else if (spg_dat[1].max == 1 || spg_dat[1].max_old == 1 || spg_dat[1].spg_level == spg_dat[1].spg_maxlevel) {
        sq_paint_chenge(
            42 - spg_dat[1].spg_len, 26, spg_dat[1].mass_len, 1, sagauge_colchg_tbl[spg_dat[1].gauge_flash_col][1]);
        sq_paint_chenge(42 - spg_dat[1].spg_len + spg_dat[1].mass_len,
                        26,
                        spg_dat[1].mchar,
                        1,
                        sagauge_colchg_tbl[spg_dat[1].gauge_flash_col][0]);
        sq_paint_chenge(42 - spg_dat[1].spg_len + spg_dat[1].mass_len + spg_dat[1].mchar,
                        26,
                        spg_dat[1].mass_len,
                        1,
                        sagauge_colchg_tbl[spg_dat[1].gauge_flash_col][1]);
    } else {
        sq_paint_chenge(
            42 - spg_dat[1].spg_len, 26, spg_dat[1].spg_len, 1, sagauge_colchg_tbl[spg_dat[1].gauge_flash_col][1]);
    }

    if (spg_dat[Stpl_Num].gauge_flash_col == 3) {
        spg_dat[Stpl_Num].gauge_flash_col = 0;
    } else {
        spg_dat[Stpl_Num].gauge_flash_col++;
    }

    spg_dat[Stpl_Num].spgcol_number = sagauge_colchg_tbl[spg_dat[Stpl_Num].gauge_flash_col][Stpl_Num];
}

void sa_moji_trans(s8 Stpl_Num, s8 Kind, s8 OnOff) {
    switch (Kind) {
    case 0:
        if (OnOff) {
            spg_dat[Stpl_Num].current_spg = spg_dat[Stpl_Num].spg_dotlen;
            sa_gauge_trans(Stpl_Num);
            max_mark_write(Stpl_Num, spg_dat[Stpl_Num].spg_len, spg_dat[Stpl_Num].mchar, spg_dat[Stpl_Num].mass_len);
            break;
        }

        if (max2[Stpl_Num] == 0) {
            sa_gauge_trans(Stpl_Num);
        }

        break;

    default:
    case 1:
        if (Stpl_Num == 0) {
            if (OnOff) {
                scfont_sqput2(1, 25, 11, 0, 2, sa_time_data_tbl[time_num][0], 0, 4, 2);
                scfont_sqput2(1, 27, 11, 0, 0, 13, 12, 2, 1);
                break;
            }

            sc_clear(1, 25, 4, 26);
            sc_clear(1, 27, 2, 27);
            break;
        }

        if (OnOff) {
            scfont_sqput2(43, 25, 11, 0, 2, sa_time_data_tbl[time_num][1], 0, 4, 2);
            scfont_sqput2(45, 27, 11, 0, 0, 14, 8, 2, 1);
            break;
        }

        sc_clear(43, 25, 46, 26);
        sc_clear(45, 27, 46, 27);
        break;
    }
}

void sa_gauge_trans(s8 pl_kind) {
    s8 i;
    s16 len;
    const u16* sa_char_ptr;

    spg_work = 0;
    spg_number = 0;
    sa_char_ptr = *spgauge_puttbl;
    len = super_arts[pl_kind].gauge_len / 8;

    for (i = 0; i < len; i++) {
        spg_work += 8;

        if (spg_work >= spg_dat[pl_kind].current_spg) {
            if (spg_dat[pl_kind].current_spg >= (spg_work - 8)) {
                spg_offset = spg_dat[pl_kind].current_spg - (i * 8);
                scfont_put2((&spg_dat[pl_kind].spgptbl_ptr[spg_number])[16 - len],
                            26,
                            spg_dat[pl_kind].spgcol_number,
                            0,
                            sa_char_ptr[spg_offset],
                            11);
            } else {
                scfont_put2((&spg_dat[pl_kind].spgptbl_ptr[spg_number])[16 - len],
                            26,
                            spg_dat[pl_kind].spgcol_number,
                            0,
                            0,
                            11);
            }
        } else {
            scfont_put2(
                (&spg_dat[pl_kind].spgptbl_ptr[spg_number])[16 - len], 26, spg_dat[pl_kind].spgcol_number, 0, 8, 11);
        }

        spg_number++;
    }
}

void spgauge_sound_request(s8 Stpl_Num) {
    if (plw[Stpl_Num].sa->store > spg_dat[Stpl_Num].spg_level) {
        Sound_SE(Stpl_Num + 107);
    }
}

void spgauge_work_clear(s8 Stpl_Num) {
    plw[Stpl_Num].sa->gauge.s.h = spg_dat[Stpl_Num].current_spg = plw[Stpl_Num].sa->bacckup_g_h;
    plw[Stpl_Num].sa->bacckup_g_h = 0;
    spg_dat[Stpl_Num].old_spg = spg_dat[Stpl_Num].current_spg;
    spg_dat[Stpl_Num].flag = 0;
    spg_dat[Stpl_Num].flag2 = 0;
    spg_dat[Stpl_Num].level_flag = 0;

    if (plw[Stpl_Num].sa->store) {
        spg_dat[Stpl_Num].timer = 0;
        spg_dat[Stpl_Num].time_rno = 5;
    } else {
        spg_dat[Stpl_Num].timer = 51;
        spg_dat[Stpl_Num].time_rno = 0;
    }

    spg_dat[Stpl_Num].timer2 = 2;
    spg_dat[Stpl_Num].kind = 0;
    spg_dat[Stpl_Num].max = 0;
    spg_dat[Stpl_Num].max_old = 0;
    spg_dat[Stpl_Num].max_rno = 0;
    spg_dat[Stpl_Num].gauge_flash_time = 2;
    spg_dat[Stpl_Num].gauge_flash_col = 0;
    spg_dat[Stpl_Num].sa_flag = 0;
    spg_dat[Stpl_Num].ex_flag = 0;
    spg_dat[Stpl_Num].no_chgcol = 0;
    spg_dat[Stpl_Num].time_no_clear = 0;
    spg_dat[Stpl_Num].sa_mukou = 0;
    sa_gauge_flash[Stpl_Num] = 0;
    sa_color_chenge(Stpl_Num);
    time_operate[Stpl_Num] = 0;
    sast_now[Stpl_Num] = 0;
    max2[Stpl_Num] = 0;
}

void spgauge_wipe_write(s8 Stpl_Num) {
    sa_stock_trans(spg_dat[Stpl_Num].spg_level, col, Stpl_Num);
    sa_gauge_trans(Stpl_Num);
    sa_waku_trans(Stpl_Num, col);
}

void sa_waku_trans(s8 Stpl_Num, s8 Spg_Col) {
    s8 lpy;

    sc_ram_to_vram(Stpl_Num + (Spg_Col * 2));

    if (Stpl_Num == 0) {
        for (lpy = 0; lpy < spg_dat[0].spg_len; lpy++) {
            scfont_put2(lpy + 6, 27, sa_color_data2_tbl[Spg_Col][0], 0, 15, 12);
        }

        if (cmd_sel[0] == 0 && no_sa[0] == 0) {
            sa_number_write(0, lpy + 6);
        } else {
            scfont_sqput2(lpy + 6, 26, 14, 0, 2, 27, 2, 1, 2);
        }

        sa_fullstock_trans(spg_dat[0].spg_maxlevel, Spg_Col, 0);
        return;
    }

    for (lpy = 0; lpy < spg_dat[1].spg_len; lpy++) {
        scfont_put2(41 - lpy, 27, sa_color_data2_tbl[Spg_Col][1], 0, 15, 12);
    }

    if (cmd_sel[1] == 0 && no_sa[1] == 0) {
        sa_number_write(1, 40 - lpy);
    } else {
        scfont_sqput2(41 - lpy, 26, 142, 1, 2, 27, 2, 1, 2);
    }

    sa_fullstock_trans(spg_dat[1].spg_maxlevel, Spg_Col, 1);
}

void tr_spgauge_cont_init(s8 pl) {
    Sa_frame_Clear2(pl);
    spg_dat[pl].current_spg = 0;
    spg_dat[pl].old_spg = 0;
    spg_dat[pl].spgtbl_ptr = spgauge_puttbl[pl];
    spg_dat[pl].spg_level = 0;
    spg_dat[pl].spg_maxlevel = super_arts[pl].store_max;
    spg_dat[pl].spg_len = super_arts[pl].gauge_len / 8;
    spg_dat[pl].spg_dotlen = super_arts[pl].gauge_len;
    spg_dat[pl].flag = 0;
    spg_dat[pl].flag2 = 0;
    spg_dat[pl].level_flag = 0;
    spg_dat[pl].timer = 51;
    spg_dat[pl].timer2 = 2;
    spg_dat[pl].kind = 0;
    spg_dat[pl].max = 0;
    spg_dat[pl].max_old = 0;
    spg_dat[pl].max_rno = 0;
    spg_dat[pl].time_rno = 0;
    spg_dat[pl].gauge_flash_time = 2;
    spg_dat[pl].gauge_flash_col = 0;
    spg_dat[pl].sa_flag = 0;
    spg_dat[pl].ex_flag = 0;
    spg_dat[pl].no_chgcol = 0;
    spg_dat[pl].time_no_clear = 0;
    spg_dat[pl].sa_mukou = 0;
    sa_gauge_flash[pl] = 0;
    spg_dat[pl].spgptbl_ptr = spgauge_postbl[pl];

    if (super_arts[pl].gauge_type == 1) {
        spg_dat[pl].time = 1;
        time_flag[pl] = 1;
    } else {
        spg_dat[pl].time = 0;
        time_flag[pl] = 0;
    }

    spg_dat[pl].mass_len = spg_dat[pl].spg_len - 5;

    if (spg_dat[pl].spg_len & 1) {
        spg_dat[pl].mchar = 5;
    } else {
        spg_dat[pl].mass_len = spg_dat[pl].mass_len - 1;
        spg_dat[pl].mchar = 6;
    }

    spg_dat[pl].mass_len /= 2;
    time_operate[pl] = 0;
    sast_now[pl] = 0;
    max2[pl] = 0;
    max_rno2[pl] = 0;

    if (pl == 0) {
        spg_dat[0].spgcol_number = 17;
    } else {
        spg_dat[1].spgcol_number = 145;
    }

    sa_stock_trans(0, 0, pl);
    sa_waku_trans(pl, 0);
    sa_gauge_trans(pl);
    Old_Stop_SG = 0;
    Exec_Wipe_F = 0;
    time_clear[pl] = 0;
    spg_offset = 0;
    time_num = 0;
    time_timer = 3;
    col = 0;
}

void tr_spgauge_cont_init2(s8 pl) {
    Sa_frame_Clear2(pl);
    demo_set_sa_full(&super_arts[pl]);
    spg_dat[pl].current_spg = super_arts[pl].gauge_len;
    spg_dat[pl].old_spg = super_arts[pl].gauge_len;
    spg_dat[pl].spgtbl_ptr = spgauge_puttbl[pl];
    spg_dat[pl].spg_level = super_arts[pl].store;
    spg_dat[pl].spg_maxlevel = super_arts[pl].store_max;
    spg_dat[pl].spg_len = super_arts[pl].gauge_len / 8;
    spg_dat[pl].spg_dotlen = super_arts[pl].gauge_len;
    spg_dat[pl].flag = 1;
    spg_dat[pl].flag2 = 0;
    spg_dat[pl].level_flag = 0;
    spg_dat[pl].timer = 51;
    spg_dat[pl].timer2 = 2;
    spg_dat[pl].kind = 1;
    spg_dat[pl].max = 0;
    spg_dat[pl].max_old = 1;
    spg_dat[pl].max_rno = 2;
    spg_dat[pl].time_rno = 5;
    spg_dat[pl].gauge_flash_time = 2;
    spg_dat[pl].gauge_flash_col = 0;
    spg_dat[pl].sa_flag = 0;
    spg_dat[pl].ex_flag = 0;
    spg_dat[pl].no_chgcol = 0;
    spg_dat[pl].time_no_clear = 0;
    spg_dat[pl].sa_mukou = 0;
    sa_gauge_flash[pl] = 0;
    spg_dat[pl].spgptbl_ptr = spgauge_postbl[pl];

    if (super_arts[pl].gauge_type == 1) {
        spg_dat[pl].time = 1;
        time_flag[pl] = 1;
    } else {
        spg_dat[pl].time = 0;
        time_flag[pl] = 0;
    }

    spg_dat[pl].mass_len = spg_dat[pl].spg_len - 5;

    if (spg_dat[pl].spg_len & 1) {
        spg_dat[pl].mchar = 5;
    } else {
        spg_dat[pl].mass_len = spg_dat[pl].mass_len - 1;
        spg_dat[pl].mchar = 6;
    }

    spg_dat[pl].mass_len /= 2;
    time_operate[pl] = 0;
    sast_now[pl] = 0;
    max2[pl] = 0;
    max_rno2[pl] = 0;

    if (pl == 0) {
        spg_dat[0].spgcol_number = 17;
    } else {
        spg_dat[1].spgcol_number = 145;
    }

    sa_stock_trans(spg_dat[pl].spg_maxlevel, 1, pl);
    sa_waku_trans(pl, 1);
    sa_gauge_trans(pl);
    sa_moji_trans(pl, 0, 1);
    Old_Stop_SG = 0;
    Exec_Wipe_F = 0;
    time_clear[pl] = 0;
    spg_offset = 0;
    time_num = 0;
    time_timer = 3;
    col = 1;
}
