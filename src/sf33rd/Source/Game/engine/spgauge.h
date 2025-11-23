#ifndef SPGAUGE_H
#define SPGAUGE_H

#include "types.h"

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

// MARK: - Serialized

extern s8 Old_Stop_SG;
extern s8 Exec_Wipe_F;
extern s8 time_clear[2];
extern s16 spg_number;
extern s16 spg_work;
extern s16 spg_offset;
extern s8 time_num;
extern s8 time_timer;
extern s8 time_flag[2];
extern s16 col;
extern s8 time_operate[2];
extern s8 sast_now[2];
extern s8 max2[2];
extern s8 max_rno2[2];
extern SPG_DAT spg_dat[2];

void spgauge_cont_init();
void spgauge_cont_main();
void spgauge_cont_demo_init();
void sag_bug_fix(s32 side);
void tr_spgauge_cont_init(s8 pl);
void tr_spgauge_cont_init2(s8 pl);

#endif
