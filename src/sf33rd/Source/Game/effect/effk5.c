/**
 * @file effk5.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effk5.h"
#include "common.h"
#include "sf33rd/Source/Game/CALDIR.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/workuser.h"

// Types

typedef struct {
    // total size: 0x8
    s16 data[4]; // offset 0x0, size 0x8
} K5Data;

typedef struct {
    // total size: 0x34
    u16 index;        // offset 0x0, size 0x2
    u16 rno;          // offset 0x2, size 0x2
    Reg32SpReal a[4]; // offset 0x4, size 0x10
    Reg32SpReal d[4]; // offset 0x14, size 0x10
    Reg32CalPos r[4]; // offset 0x24, size 0x10
} MVJ;

typedef union {
    u32 swi; // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        u16 l; // offset 0x0, size 0x2
        u16 h; // offset 0x2, size 0x2
    } sws;     // offset 0x0, size 0x4
    struct {
        // total size: 0x4
        u8 ll; // offset 0x0, size 0x1
        u8 l;  // offset 0x1, size 0x1
        u8 h;  // offset 0x2, size 0x1
        u8 hh; // offset 0x3, size 0x1
    } swc;     // offset 0x0, size 0x4
} MVSW;

// Data

const s16 lookup_index[10] = { 0, 0, 0, 0, 1, 1, 1, 1, 0xFFFF, 0xFFFF };

const s8 k5_exc_check[125] = { 1, 2, 0, 2, 2, 2, 2, 1, 1, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1,
                               2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 0,
                               0, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1,
                               1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2 };

// Forward decls

void K5_init_data(WORK* mwk, MVJ* mvj, u16* ixtbl);
void K5_init_data_copy(MVJ* mvj, K5Data* dad, s16 num);
void K5_init_data_copy2(K5Data* dad, MVJ* mvj, s16 num);
void get_table_adrs_K5(WORK* wk);
void k5_add_sub(MVJ* mvj);
void get_okuri_time(WORK* ewk, WORK* mwk, MVJ* mvj);
void K5_main_process(WORK* ewk, WORK* mwk, MVJ* mvj);
void init_K5_work(WORK* ewk, WORK* mwk, MVJ* mvj);
void get_master_table_address(WORK* ewk, WORK* mwk);
s32 get_cal_work(WORK* wk);
void K5_decode_new_hit_index(WORK* wk, MVJ* mvj, u16 mf);
u32 decode_mvsw(u16 flag);

// Funcs

void effect_K5_move(WORK_Other* ewk) {
    WORK* mwk = (WORK*)ewk->my_master;
    MVJ* mvj;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0] += 1;

        if (get_cal_work(&ewk->wu) == -1) {
            ewk->wu.routine_no[0] = 3;
            return;
        }

        // This line is bullshit. Effect K5 needs some space for MVJ manipulation. Instead of allocating
        // space for that somewhere else they decided to use some of the space dedicated to effect work.
        // Why did they choose routine_no as the starting offset specifically? They did that because it's
        // the first var of WORK that is not used for effect scheduling. If they chose an earlier address
        // that would lead to crashes and infinite loops. Fun times!
        // There's one more line just like this one down below.
        mvj = (MVJ*)(((WORK*)ewk->wu.target_adrs)->routine_no);

        init_K5_work(&ewk->wu, mwk, mvj);
        ewk->wu.old_rno[1] = mwk->cg_hit_ix;
        get_table_adrs_K5(mwk);
        K5_init_data(mwk, mvj, (u16*)(&mwk->cg_ja));
        break;

    case 1:
        if (ewk->wu.dead_f == 1) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            return;
        }

        if (((PLW*)mwk)->waku_ram_index != ewk->wu.myself) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
            return;
        }

        get_master_table_address(&ewk->wu, mwk);
        mvj = (MVJ*)(((WORK*)ewk->wu.target_adrs)->routine_no);

        if (mwk->K5_exec_ok) {
            mwk->K5_exec_ok = 0;

            if (mwk->K5_init_flag || (ewk->wu.old_rno[1] != mwk->cg_hit_ix)) {
                mwk->K5_init_flag = 0;
                ewk->wu.old_rno[1] = mwk->cg_hit_ix;
                ewk->wu.routine_no[1] = 0;
                K5_init_data(mwk, mvj, (u16*)(&mwk->cg_ja));
            }

            K5_main_process(&ewk->wu, mwk, mvj);
        }

        K5_init_data_copy2((K5Data*)&rambod[mwk->id], mvj, 4);
        K5_init_data_copy2((K5Data*)&ramhan[mwk->id], mvj + 4, 4);
        mwk->h_bod = (UNK_1*)&rambod[mwk->id];
        mwk->h_han = (UNK_2*)&ramhan[mwk->id];
        break;

    case 2:
        push_effect_work((WORK*)ewk->wu.target_adrs);
        /* fallthrough */

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void K5_main_process(WORK* ewk, WORK* mwk, MVJ* mvj) {
    s16 i;

    switch (ewk->routine_no[1]) {
    case 0:
        get_okuri_time(ewk, mwk, mvj);
        break;

    case 1:
        for (i = 0; i < 8; i++) {
            if (mvj[i].rno) {
                k5_add_sub(&mvj[i]);
            }
        }

        break;
    }
}

typedef union {
    u32* cpl; // offset 0x0, size 0x4
    u16* cps; // offset 0x0, size 0x4
    u8* cpc;  // offset 0x0, size 0x4
} GOTCP;

void get_okuri_time(WORK* ewk, WORK* mwk, MVJ* mvj) {
    GOTCP gotcp;
    ST st;
    s16 exc;
    u16 now_mf;

    if ((mwk->cgd_type != 2) && (mwk->cg_ja.mf.full & 0x1010)) {
        now_mf = mwk->cg_ja.mf.full;
        exc = 0;
        ewk->old_rno[0] = mwk->cg_ctr;
        ewk->cg_ix = mwk->cg_ix;

        while (1) {
            ewk->cg_ix += mwk->cgd_type;
            gotcp.cpl = &mwk->set_char_ad[ewk->cg_ix];

            if (gotcp.cps[0] >= 0x100) {
                st.l = gotcp.cpl[2];
                st.l *= 8;
                ewk->cg_hit_ix = st.w.h & 0x1FF;

                if (ewk->old_rno[1] == ewk->cg_hit_ix) {
                    if ((gotcp.cpc[1] != 0xFF) || (gotcp.cpc[1] < 0xC8)) {
                        ewk->old_rno[0] += gotcp.cpc[1];
                    }

                    continue;
                }

                if (ewk->old_rno[0] >= 2) {
                    K5_decode_new_hit_index(ewk, mvj, now_mf);
                    ewk->routine_no[1] = 1;
                    return;
                }

                break;
            }

            if (k5_exc_check[gotcp.cps[0]] == 2) {
                break;
            }

            if (k5_exc_check[gotcp.cps[0]]) {
                continue;
            }

            if (exc++ >= 4) {
                break;
            }

            switch (gotcp.cps[0]) {
            case 2:
                ewk->cg_ix = (gotcp.cps[3] - 2) * mwk->cgd_type;
                break;

            case 49:
                if ((test_flag == 0) || (ixbfw_cut == 0)) {
                    ewk->cg_ix += (gotcp.cps[3] - 1) * mwk->cgd_type;
                }

                break;

            case 50:
                if ((test_flag == 0) || (ixbfw_cut == 0)) {
                    ewk->cg_ix -= (gotcp.cps[3] + 1) * mwk->cgd_type;
                }

                break;
            }
        }
    }

    ewk->old_rno[0] = 1;
    ewk->routine_no[1] = 2;
}

void K5_decode_new_hit_index(WORK* wk, MVJ* mvj, u16 mf) {
    s16 i;
    s16 t0;
    s16 t1;
    MVSW mvsw;

    get_table_adrs_K5(wk);
    mvsw.swi = decode_mvsw(mf);

    if (wk->cg_ja.boix != mvj[0].index) {
        for (i = 0; i < 4; i++) {
            if (mvj[i].r[1].pos.h != 0) {
                wk->xyz[0].disp.pos = mvj[i].r[0].pos.h;
                wk->xyz[1].disp.pos = mvj[i].r[1].pos.h;

                if ((t1 = wk->h_bod->body_dm[i][1])) {
                    t0 = wk->h_bod->body_dm[i][0];
                } else {
                    t0 = wk->xyz[0].disp.pos + wk->xyz[1].disp.pos / 2;
                }

                cal_all_speed_data(wk, wk->old_rno[0], t0, t1, mvsw.swc.hh, mvsw.swc.l);
                mvj[i].r[0].cal = wk->xyz[0].cal;
                mvj[i].r[1].cal = wk->xyz[1].cal;
                mvj[i].a[0].sp = wk->mvxy.a[0].sp;
                mvj[i].d[0].sp = wk->mvxy.d[0].sp;
                mvj[i].a[1].sp = wk->mvxy.a[1].sp;
                mvj[i].d[1].sp = wk->mvxy.d[1].sp;
                wk->xyz[0].disp.pos = mvj[i].r[2].pos.h;
                wk->xyz[1].disp.pos = mvj[i].r[3].pos.h;

                if ((t1 = wk->h_bod->body_dm[i][3])) {
                    t0 = wk->h_bod->body_dm[i][2];
                } else {
                    t0 = wk->xyz[0].disp.pos + wk->xyz[1].disp.pos / 2;
                }

                cal_all_speed_data(wk, wk->old_rno[0], t0, t1, mvsw.swc.h, mvsw.swc.ll);
                mvj[i].r[2].cal = wk->xyz[0].cal;
                mvj[i].r[3].cal = wk->xyz[1].cal;
                mvj[i].a[2].sp = wk->mvxy.a[0].sp;
                mvj[i].d[2].sp = wk->mvxy.d[0].sp;
                mvj[i].a[3].sp = wk->mvxy.a[1].sp;
                mvj[i].d[3].sp = wk->mvxy.d[1].sp;
                mvj[i].rno = 1;
            } else {
                mvj[i].rno = 0;
            }

            mvj[i].index = wk->cg_ja.boix;
        }
    }

    if (mvj[4].index != (wk->cg_ja.bhix + wk->cg_ja.haix)) {
        for (i = 4; i < 8; i++) {
            if (mvj[i].r[1].pos.h != 0) {
                wk->xyz[0].disp.pos = mvj[i].r[0].pos.h;
                wk->xyz[1].disp.pos = mvj[i].r[1].pos.h;

                if ((t1 = wk->h_han->hand_dm[i - 4][1])) {
                    t0 = wk->h_han->hand_dm[i - 4][0];
                } else {
                    t0 = wk->xyz[0].disp.pos + wk->xyz[1].disp.pos / 2;
                }

                cal_all_speed_data(wk, wk->old_rno[0], t0, t1, mvsw.swc.hh, mvsw.swc.l);
                mvj[i].r[0].cal = wk->xyz[0].cal;
                mvj[i].r[1].cal = wk->xyz[1].cal;
                mvj[i].a[0].sp = wk->mvxy.a[0].sp;
                mvj[i].d[0].sp = wk->mvxy.d[0].sp;
                mvj[i].a[1].sp = wk->mvxy.a[1].sp;
                mvj[i].d[1].sp = wk->mvxy.d[1].sp;
                wk->xyz[0].disp.pos = mvj[i].r[2].pos.h;
                wk->xyz[1].disp.pos = mvj[i].r[3].pos.h;

                if ((t1 = wk->h_han->hand_dm[i - 4][3])) {
                    t0 = wk->h_han->hand_dm[i - 4][2];
                } else {
                    t0 = wk->xyz[0].disp.pos + wk->xyz[1].disp.pos / 2;
                }

                cal_all_speed_data(wk, wk->old_rno[0], t0, t1, mvsw.swc.h, mvsw.swc.ll);
                mvj[i].r[2].cal = wk->xyz[0].cal;
                mvj[i].r[3].cal = wk->xyz[1].cal;
                mvj[i].a[2].sp = wk->mvxy.a[0].sp;
                mvj[i].d[2].sp = wk->mvxy.d[0].sp;
                mvj[i].a[3].sp = wk->mvxy.a[1].sp;
                mvj[i].d[3].sp = wk->mvxy.d[1].sp;
                mvj[i].rno = 1;
            } else {
                mvj[i].rno = 0;
            }

            mvj[i].index = wk->cg_ja.bhix + wk->cg_ja.haix;
        }
    }
}

u32 decode_mvsw(u16 flag) {
    MVSW mvsw;

    mvsw.swi = flag;

    if (flag & 0x1000) {
        mvsw.swc.hh = mvsw.swc.h = mvsw.swc.l;
        mvsw.swc.hh >>= 2;
        mvsw.swc.hh &= 3;
        mvsw.swc.h &= 3;
    } else {
        mvsw.sws.h = 0xFFFF;
    }

    if (flag & 0x10) {
        mvsw.swc.l = mvsw.swc.ll;
        mvsw.swc.l >>= 2;
        mvsw.swc.l &= 3;
        mvsw.swc.ll &= 3;
    } else {
        mvsw.sws.l = 0xFFFF;
    }

    return mvsw.swi;
}

void get_table_adrs_K5(WORK* wk) {
    wk->cg_ja = wk->hit_ix_table[wk->cg_hit_ix];
    wk->h_bod = &wk->body_adrs[wk->cg_ja.boix];
    wk->h_han = &wk->hand_adrs[wk->cg_ja.bhix + wk->cg_ja.haix];
}

void init_K5_work(WORK* ewk, WORK* mwk, MVJ* mvj) {
    s16 i;

    for (i = 0; i < 10; i++) {
        mvj[i].index = mvj[i].rno = 0;
    }

    ewk->cg_hit_ix = mwk->cg_hit_ix;
    ewk->hit_ix_table = mwk->hit_ix_table;
    ewk->body_adrs = mwk->body_adrs;
    ewk->hand_adrs = mwk->hand_adrs;
    mwk->K5_init_flag = 1;
}

void get_master_table_address(WORK* ewk, WORK* mwk) {
    ewk->hit_ix_table = mwk->hit_ix_table;
    ewk->body_adrs = mwk->body_adrs;
    ewk->hand_adrs = mwk->hand_adrs;
}

void K5_init_data(WORK* mwk, MVJ* mvj, u16* ixtbl) {
    s16 i;

    for (i = 0; i < 8; i++) {
        mvj[i].rno = 0;
        mvj[i].index = ixtbl[lookup_index[i]];
    }

    K5_init_data_copy(mvj, (K5Data*)mwk->body_adrs[mwk->cg_ja.boix].body_dm, 4);
    K5_init_data_copy(mvj + 4, (K5Data*)mwk->hand_adrs[mwk->cg_ja.bhix + mwk->cg_ja.haix].hand_dm, 4);
}

void K5_init_data_copy(MVJ* mvj, K5Data* dad, s16 num) {
    s16 i;

    for (i = 0; i < num; i++) {
        mvj[i].r[0].pos.h = dad[i].data[0];
        mvj[i].r[1].pos.h = dad[i].data[1];
        mvj[i].r[2].pos.h = dad[i].data[2];
        mvj[i].r[3].pos.h = dad[i].data[3];
    }
}

void K5_init_data_copy2(K5Data* dad, MVJ* mvj, s16 num) {
    s16 i;

    for (i = 0; i < num; i++) {
        dad[i].data[0] = mvj[i].r[0].pos.h;
        dad[i].data[1] = mvj[i].r[1].pos.h;
        dad[i].data[2] = mvj[i].r[2].pos.h;
        dad[i].data[3] = mvj[i].r[3].pos.h;
    }
}

s32 get_cal_work(WORK* wk) {
    WORK* fwk;
    s16 ix;

    if ((ix = pull_effect_work(7)) == -1) {
        return -1;
    }

    fwk = (WORK*)frw[ix];
    wk->target_adrs = (u32*)fwk;
    fwk->be_flag = 1;
    fwk->id = 0xCD;
    return 0;
}

void k5_add_sub(MVJ* mvj) {
    s16 i;

    for (i = 0; i < 4; i++) {
        mvj->r[i].cal += mvj->a[i].sp;
    }

    for (i = 0; i < 4; i++) {
        mvj->a[i].sp += mvj->d[i].sp;
    }
}

s32 effect_K5_init(PLW* wk) {
    WORK_Other* ewk;
    s16 ix;

    if (Bonus_Game_Flag == 0x14) {
        return -1;
    }

    if ((ix = pull_effect_work(0)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 0xCD;
    ewk->wu.work_id = 0x10;
    ewk->my_master = (u32*)wk;
    wk->waku_ram_index = ewk->wu.myself;
    return 0;
}
