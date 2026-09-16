/**
 * @file charset.c
 * The interpreter for character animation and logic scripts
 */

#include "sf33rd/Source/Game/engine/charset.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effxx.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/sound/se_data.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/engine/charset_internal.h"

u16 att_req = 0;

extern s32 (*const decode_chcmd[125])();
extern s32 (*const decode_if_lever[16])();
extern const s16 jphos_table[16];
extern const s16 kezuri_pow_table[5];

s16 decord_if_jump(WORK* wk, UNK11* cpc, s16 ix);
u16 get_comm_if_lever(WORK* wk);
u16 get_comm_if_shot(WORK* wk);
u16 get_comm_if_shot_now_off(WORK* wk);
u16 get_comm_if_shot_now(WORK* wk);
u16 get_comm_if_lvsh(WORK* wk);
u8 get_comm_djmp_lever_dir(PLW* wk);
void setup_comm_retmj(WORK* wk);
void check_cgd_patdat2(WORK* wk);
void setup_metamor_kezuri(WORK* wk);

static s32 special_cancel_window_is_open(const WORK* wk) {
    return !(WK_AS_PLW->spmv_ng_flag2 & DIP2_SPECIAL_TO_SPECIAL_CANCEL_DISABLED) && !(wk->kow & 0x60) &&
           (wk->kow & 0xF8) && (wk->cg_cancel & 0x40);
}

static s32 is_cancellable_normal(const WORK* wk) {
    return !(wk->kow & 0xF8) && (wk->routine_no[1] == 4) && (wk->routine_no[2] < 16);
}

static s32 target_combo_is_blocked(const WORK* wk) {
    return (WK_AS_PLW->spmv_ng_flag2 & DIP2_TARGET_COMBO_DISABLED) && (wk->cg_cancel & 8) && !(wk->kow & 0xF8);
}

void set_char_move_init(WORK* wk, s16 koc, s16 index) {
    wk->now_koc = koc;
    wk->char_index = index;

#if CPS3
    wk->set_char_ad = (u32*)wk->char_table[koc][index];

    const u32* src = wk->set_char_ad;
    u32* dst = (u32*)&wk->cg_ctr;

    for (int i = 0; i < 6; i++) {
        dst[i] = 0;
    }

    dst[-1] = src[-1];
    dst[-2] = src[-2];
#else
    wk->set_char_ad = &wk->char_table[koc][wk->char_table[koc][index] / sizeof(u32)];
    setupCharTableData(wk, 1, 1);
#endif

    wk->cg_ix = -wk->cgd_type;
    wk->cg_ctr = 1;
    wk->cg_next_ix = 0;
    wk->old_cgnum = 0;
    wk->cg_wca_ix = 0;
    wk->cmoa.koc = wk->now_koc;
    wk->cmoa.ix = wk->char_index;
    wk->cmoa.pat = 1;
    wk->cmwk[8] = 0;
    wk->cmwk[15] = 0;

#if !CPS3
    wk->kow = wk->kind_of_waza;
#endif

    if (wk->work_id & 0xF) {
        wk->at_koa = acatkoa_table[wk->kind_of_waza];
    }

    if (wk->work_id == 1) {
        ((PLW*)wk)->tc_1st_flag = 0; // TODO: Confirm CPS3 match

        if (wk->now_koc == 4 || wk->now_koc == 5) {
            grade_add_onaji_waza(wk->id);
        }

        ((PLW*)wk)->ja_nmj_rno = 0; // TODO: Confirm CPS3 match
        pp_pulpara_remake_at_init(wk);
    }

    wk->K5_init_flag = 1; // TODO: Confirm CPS3 match
    char_move(wk);
}

void setupCharTableData(WORK* wk, s32 clr, s32 info) {
    u32* dst = (u32*)&wk->cg_type;
    u32* src;
    s32 i;

    if (info != 0) {
        src = wk->set_char_ad;
        dst[-1] = src[-1];
        dst[-2] = src[-2];

        if (clr != 0) {
            for (i = 0; i < 6; i++) {
                dst[i] = 0;
            }
        }
    } else {
        src = wk->set_char_ad + wk->cg_ix;

        for (i = 0; i < wk->cgd_type; i++) {
            dst[i] = src[i];
        }
    }
}

void set_char_move_init2(WORK* wk, s16 koc, s16 index, s16 ip, s16 scf) {
    u8 pst;
    u8 kow;

#if !CPS3
    if (index < 0) {
        index = 0;
    }

    if (ip <= 0) {
        ip = 1;
    }
#endif

    pst = wk->pat_status;
    kow = wk->kind_of_waza;
    wk->now_koc = koc;
    wk->char_index = index;

#if CPS3
    wk->set_char_ad = (u32*)wk->char_table[koc][index];

    const u32* src = wk->set_char_ad;
    u32* dst = (u32*)&wk->cg_ctr;

    for (int i = 0; i < 6; i++) {
        dst[i] = 0;
    }

    dst[-1] = src[-1];
    dst[-2] = src[-2];
#else
    wk->set_char_ad = wk->char_table[koc] + (wk->char_table[koc][index] / 4);
    setupCharTableData(wk, 1, 1);
#endif

    wk->cg_ix = (ip - 1) * wk->cgd_type - wk->cgd_type;
    wk->cg_ctr = 1;
    wk->cg_next_ix = 0;
    wk->old_cgnum = 0;
    wk->cg_wca_ix = 0;

    if (wk->cmoa.pat == 0) {
        wk->cmoa.koc = wk->now_koc;
        wk->cmoa.ix = wk->char_index;
        wk->cmoa.pat = 1;
    }

    if (scf) {
        wk->pat_status = pst;
        wk->kind_of_waza = kow;
    } else {
#if !CPS3
        wk->kow = wk->kind_of_waza;
#endif
    }

    if (wk->work_id & 0xF) {
        wk->at_koa = acatkoa_table[wk->kind_of_waza];
    }

    wk->K5_init_flag = 1; // TODO: Confirm CPS3 match
    char_move(wk);
}

void exset_char_move_init(WORK* wk, s16 koc, s16 index) {
    u8 now_ctr;

    wk->now_koc = koc;
    wk->char_index = index;

#if CPS3
    wk->set_char_ad = (u32*)wk->char_table[koc][index];
#else
    wk->set_char_ad = &wk->char_table[koc][wk->char_table[koc][index] / 4];
#endif

    now_ctr = wk->cg_ctr;

#if CPS3
    u32* dst = (u32*)&wk->cg_ctr;
    const u32* src = wk->set_char_ad + wk->cg_ix;

    for (int i = 0; i < wk->cgd_type; i++) {
        dst[i] = src[i];
    }

#else
    setupCharTableData(wk, 0, 0);
#endif

    wk->cg_ctr = now_ctr;
    wk->cmoa.koc = wk->now_koc;
    wk->cmoa.ix = wk->char_index;
    wk->cmoa.pat = 1;
    wk->K5_init_flag = 1;  // TODO: Confirm CPS3 match
    check_cgd_patdat2(wk); // TODO: Confirm CPS3 match
}

void char_move_z(WORK* wk) {
    if (test_flag) {
        wk->cg_next_ix = 0;
    }

    wk->cg_ctr = 1;
    wk->K5_init_flag = 1; // TODO: Confirm CPS3 match
    char_move(wk);
}

void char_move_wca(WORK* wk) {
    wk->cg_next_ix = 0;
    wk->cg_ix = (wk->cg_wca_ix - 1) * wk->cgd_type - wk->cgd_type;
    wk->cg_ctr = 1;
    wk->K5_init_flag = 1;
    char_move(wk);
}

void char_move_wca_init(WORK* wk) {
    wk->cg_next_ix = 0;
    wk->cg_ix = (wk->cg_wca_ix - 1) * wk->cgd_type - wk->cgd_type;
    wk->cg_ctr = 1;
    wk->K5_init_flag = 1;
}

s32 comm_wca(WORK* wk, UNK11* /* unused */) {
    char_move_wca_init(wk);
    return 1;
}

void char_move_index(WORK* wk, s16 ix) {
    wk->cg_next_ix = 0;
    wk->cg_ix = (ix - 1) * wk->cgd_type - wk->cgd_type;
    wk->cg_ctr = 1;
    wk->K5_init_flag = 1;
    char_move(wk);
}

void char_move_cmja(WORK* wk) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmja.koc, wk->cmja.ix, wk->cmja.pat, 0);
}

#if CPS3
void char_move_cmj2(WORK* wk) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj2.koc, wk->cmj2.ix, wk->cmj2.pat, 0);
}

void char_move_cmj3(WORK* wk) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj3.koc, wk->cmj3.ix, wk->cmj3.pat, 0);
}
#endif

void char_move_cmj4(WORK* wk) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj4.koc, wk->cmj4.ix, wk->cmj4.pat, 0);
}

#if CPS3
void char_move_cmoa(WORK* wk) {
    set_char_move_init2(wk, wk->cmoa.koc, wk->cmoa.ix, wk->cmoa.pat, 0);
}
#endif

void char_move_cmms(WORK* wk) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmms.koc, wk->cmms.ix, wk->cmms.pat, 0);
}

void char_move_cmms2(WORK* wk) {
    u32* to_ram;
    s16 i;
    s16 now_cgd;

    setup_comm_back(wk);
    now_cgd = wk->cgd_type;
    wk->now_koc = wk->cmms.koc;
    wk->char_index = wk->cmms.ix;

#if CPS3
    wk->set_char_ad = (u32*)wk->char_table[wk->now_koc][wk->char_index];

    const u32* src = wk->set_char_ad;
    u32* dst = wk->cg_ctr;

    dst[-1] = src[-1];
    dst[-2] = src[-2];
#else
    wk->set_char_ad = &wk->char_table[wk->now_koc][wk->char_table[wk->now_koc][wk->char_index] / 4];
    setupCharTableData(wk, 0, 1);
#endif

    if (now_cgd > wk->cgd_type) {
        // FIXME: this will break if the layout of WORK changes
        to_ram = (u32*)&wk->cg_wca_ix;

        for (i = 0; i < (now_cgd - wk->cgd_type); i++) {
            *--to_ram = 0;
        }
    }

    wk->cg_ix = (wk->cmms.pat - 1) * wk->cgd_type - wk->cgd_type;
    wk->cg_ctr = 1;
    wk->cg_next_ix = 0;
    wk->old_cgnum = 0;
    wk->cg_wca_ix = 0;

#if !CPS3
    wk->kow = wk->kind_of_waza;
#endif
}

s32 char_move_cmms3(PLW* wk) {
    UNK11* cpc;
    u32* to_ram;
    s16 i;
    s16 now_cgd;

    wk->meoshi_jump_flag = 1;
    setup_comm_retmj(&wk->wu);
    setup_comm_back(&wk->wu);
    now_cgd = wk->wu.cgd_type;
    wk->wu.now_koc = wk->wu.cmms.koc;
    wk->wu.char_index = wk->wu.cmms.ix;

#if CPS3
    wk->wu.set_char_ad = (u32*)wk->wu.char_table[wk->wu.now_koc][wk->wu.char_index];

    const u32* src = wk->wu.set_char_ad;
    u32* dst = wk - wu.cg_ctr;

    dst[-1] = src[-1];
    dst[-2] = src[-2];
#else
    wk->wu.set_char_ad = &wk->wu.char_table[wk->wu.now_koc][wk->wu.char_table[wk->wu.now_koc][wk->wu.char_index] / 4];
    setupCharTableData(&wk->wu, 0, 1);
#endif

    wk->wu.cg_ix = wk->wu.cmms.pat * wk->wu.cgd_type - wk->wu.cgd_type;

#if !CPS3
    wk->wu.kow = wk->wu.kind_of_waza;
#endif

    while (1) {
        cpc = (UNK11*)(wk->wu.set_char_ad + wk->wu.cg_ix);

        if (cpc->code >= 0x100) {
            break;
        }

        if (decode_chcmd[cpc->code](wk, cpc) != 0) {
            wk->wu.cg_ix += wk->wu.cgd_type;
        } else if (wk->meoshi_jump_flag != 0) {
            break;
        } else {
            return 0;
        }
    }

    if (now_cgd > wk->wu.cgd_type) {
        to_ram = (u32*)&wk->wu.cg_wca_ix;

        for (i = 0; i < now_cgd - wk->wu.cgd_type; i++) {
            *--to_ram = 0;
        }
    }

    wk->wu.cg_ix -= wk->wu.cgd_type;
    wk->wu.cg_ctr = 1;
    wk->wu.cg_next_ix = 0;
    wk->wu.old_cgnum = 0;
    wk->wu.cg_wca_ix = 0;
    wk->meoshi_jump_flag = 0;
    return 1;
}

void char_move_cmhs(PLW* wk) {
    if (wk->hsjp_ok != 0) {
        setup_comm_back(&wk->wu);
        wk->hsjp_ok = 0;
        set_char_move_init2(&wk->wu, wk->wu.cmhs.koc, wk->wu.cmhs.ix, wk->wu.cmhs.pat, 0);
    }
}

void char_move(WORK* wk) {
    wk->K5_exec_ok = 1;

    if (--wk->cg_ctr == 0) {
        check_cm_extended_code(wk);
    }
}

void check_cm_extended_code(WORK* wk) {
    UNK11* cpc;

    if (wk->cg_next_ix) {
        wk->cg_ix = (wk->cg_next_ix - 1) * wk->cgd_type;
    } else {
        wk->cg_ix += wk->cgd_type;
    }

    while (1) {
        cpc = (UNK11*)(wk->set_char_ad + wk->cg_ix);

        if (cpc->code >= 0x100) {
            check_cgd_patdat(wk);
            break;
        }

        if (decode_chcmd[cpc->code](wk, cpc) == 0) {
            break;
        }

        wk->cg_ix += wk->cgd_type;
    }
}


s16 decord_if_jump(WORK* wk, UNK11* cpc, s16 ix) {
    s16 rnum;

    switch (ix & 0xE000) {
    case 0x4000:
        wk->cg_ix += ((ix & 0xFF) - 1) * wk->cgd_type;
        rnum = 1;
        break;

    case 0x8000:
        wk->cg_ix -= ((ix & 0xFF) + 1) * wk->cgd_type;
        rnum = 1;
        break;

    case 0x2000:
        rnum = decode_if_lever[ix & 0xFF](wk, cpc);
        break;

    default:
        wk->cg_ix = (ix - 2) * wk->cgd_type;
        rnum = 1;
        break;
    }

    return rnum;
}

u16 get_comm_if_lever(WORK* wk) {
    u16 num;

    if (wk->work_id == 1) {
        num = wcp[wk->id].sw_new & 0xF;
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].sw_new & 0xF;
    }

    return num;
}

u16 get_comm_if_shot(WORK* wk) {
    u16 num;

    if (wk->work_id == 1) {
        num = wcp[wk->id].sw_new & 0x770;
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].sw_new & 0x770;
    }

    return num;
}

u16 get_comm_if_shot_now_off(WORK* wk) {
    u16 num;

    if (wk->work_id == 1) {
        num = wcp[wk->id].sw_now & 0x770;
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].sw_now & 0x770;
    }

    if (wk->cg_cancel & 0x80) {
        if (wk->work_id == 1) {
            num |= wcp[wk->id].sw_off & 0x770;
        } else {
            num |= wcp[((WORK_Other*)wk)->master_id & 1].sw_off & 0x770;
        }
    }

    return num;
}

u16 get_comm_if_shot_now(WORK* wk) {
    u16 num;

    if (wk->work_id == 1) {
        num = wcp[wk->id].sw_now & 0x770;
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].sw_now & 0x770;
    }

    return num;
}

u16 get_comm_if_lvsh(WORK* wk) {
    u16 num;

    if (wk->work_id == 1) {
        num = wcp[wk->id].sw_new & 0x77F;
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].sw_new & 0x77F;
    }

    return num;
}

u8 get_comm_djmp_lever_dir(PLW* wk) {
    u8 num;

    if (wk->wu.work_id == 1) {
        if (wk->py->flag == 0) {
            num = wcp[wk->wu.id].lever_dir;
        } else {
            num = 0;
        }
    } else {
        num = wcp[((WORK_Other*)wk)->master_id & 1].lever_dir;
    }

    return num;
}

void setup_comm_back(WORK* wk) {
    wk->K5_init_flag = 1;
    wk->cmbk.koc = wk->now_koc;
    wk->cmbk.ix = wk->char_index;
    wk->cmbk.pat = (wk->cg_ix / wk->cgd_type) + 2;
}

void setup_comm_retmj(WORK* wk) {
    wk->cmb2.koc = wk->now_koc;
    wk->cmb2.ix = wk->char_index;
    wk->cmb2.pat = wk->cg_ix;
}

void setup_comm_abbak(WORK* wk) {
    wk->cmb3.koc = wk->now_koc;
    wk->cmb3.ix = wk->char_index;
    wk->cmb3.pat = (wk->cg_ix / wk->cgd_type) + 2;
}

static int catch_table_offset(Character thrown_character) {
    if (ArcadeBalance_IsEnabled()) {
        return CHAR_3SX_TO_ARCADE(thrown_character) - 24;
    } else {
        return thrown_character - 20;
    }
}

void check_cgd_patdat(WORK* wk) {
    ST st;

    u16* seAdrs;
    s16* from_rom2;

    setupCharTableData(wk, 0, 0);

    switch (wk->cgd_type) {
    case 6:
        if (wk->cg_add_xy) {
            from_rom2 = wk->step_xy_table + wk->cg_add_xy;
            st.l = *from_rom2++;
            st.l <<= 8;

            if (wk->rl_flag) {
                wk->xyz[0].cal += st.l;
            } else {
                wk->xyz[0].cal -= st.l;
            }

            st.l = *from_rom2;
            st.l <<= 8;
            wk->xyz[1].cal += st.l;
        }

        if (wk->cg_status & 0x80) {
            wk->pat_status = wk->cg_status & 0x7F;
        }

        /* fallthrough */

    case 4:
        wk->cg_meoshi = wk->cg_hit_ix & 0x1FFF;
        st.w.h = wk->cg_att_ix;
        st.w.l = wk->cg_hit_ix;
        wk->cg_att_ix >>= 6;
        st.l *= 8;
        wk->cg_hit_ix = st.w.h & 0x1FF;

        if (wk->cg_att_ix) {
            set_new_attnum(wk);
        }

        if (wk->cg_effect) {
            effinitjptbl[wk->cg_effect](wk, wk->cg_eftype);
        }

        break;
    }

    wk->cg_jphos = jphos_table[wk->cg_olc_ix & 0xF];
    wk->cg_olc_ix >>= 4;
    wk->cg_flip = wk->cg_se & 3;
    wk->cg_prio = (wk->cg_se & 0xF) >> 2;
    wk->cg_se >>= 4;

    if (wk->cg_se & 0x800) {
        seAdrs = (u16*)(wk->se_random_table + (wk->se_random_table[wk->cg_se & 0x7FF] / 4));
        wk->cg_se = seAdrs[random_16()];
    }

    if (wk->cg_se) {
        sound_effect_request[wk->cg_se](wk, check_xcopy_filter_se_req(wk));
    }

    if (wk->work_id == 1) {
        if (wk->cg_rival == 0) {
            wk->curr_rca = NULL;
        } else {
            wk->curr_rca = wk->rival_catch_tbl + (wk->cg_rival + catch_table_offset(((PLW*)wk)->tsukami_num));
        }

        wk->cg_olc = wk->olc_ix_table[wk->cg_olc_ix];
    }

    if (wk->work_id < 16) {
        wk->cg_ja = wk->hit_ix_table[wk->cg_hit_ix];
        set_jugde_area(wk);
    }

    if ((wk->cg_type != 0xFF) && (wk->cg_type & 0x80)) {
        wk->cg_wca_ix = wk->cg_type & 0x7F;
        wk->cg_type = 0;
    }

    if (wk->work_id == 1) {
        if (target_combo_is_blocked(wk)) {
            if (wk->kow & 6) {
                wk->cg_cancel &= 0xF7;
                wk->cg_meoshi = 0;
            } else if (wk->cg_meoshi & 0x110) {
                wk->cg_meoshi &= 0xF99F;
            } else {
                wk->cg_cancel &= 0xF7;
                wk->cg_meoshi = 0;
            }
        }

        if (WK_AS_PLW->spmv_ng_flag2 & DIP2_SA_TO_SA_CANCEL_DISABLED) {
            if (wk->kow & 0x60) {
                wk->cg_cancel &= 0xBF;
            }
        } else if ((wk->kow & 0x60) && (wk->cg_cancel & 0x40)) {
            wk->meoshi_hit_flag = 1;
        }

        if (special_cancel_window_is_open(wk)) {
            wk->cg_cancel |= 0x60;
        }

        if (is_cancellable_normal(wk)) {
            switch (plpat_rno_filter[wk->routine_no[2]]) {
            case 9:
                if (wk->routine_no[3] != 1) {
                    break;
                }

                /* fallthrough */

            case 1:
                if (!(WK_AS_PLW->spmv_ng_flag2 & DIP2_ALL_MOVES_CANCELLABLE_BY_HIGH_JUMP_DISABLED)) {
                    wk->cg_cancel |= 1;
                }

                if (!(WK_AS_PLW->spmv_ng_flag2 & DIP2_ALL_MOVES_CANCELLABLE_BY_DASH_DISABLED)) {
                    wk->cg_cancel |= 2;
                }

                if (!(WK_AS_PLW->spmv_ng_flag2 & DIP2_GROUND_CHAIN_COMBO_DISABLED)) {
                    if (WK_AS_PLW->player_number == 4) {
                        wk->cg_meoshi = chain_hidou_nm_ground_table[wk->kow & 7];
                        wk->cg_cancel |= 8;
                        return;
                    }

                    wk->cg_meoshi = chain_normal_ground_table[wk->kow & 7];
                    wk->cg_cancel |= 8;
                    return;
                }

                break;

            case 2:
                if (!(WK_AS_PLW->spmv_ng_flag2 & DIP2_AIR_CHAIN_COMBO_DISABLED) && !hikusugi_check(wk)) {
                    if (WK_AS_PLW->player_number == 7) {
                        wk->cg_meoshi = chain_hidou_nm_air_table[wk->kow & 7];
                        wk->cg_cancel |= 8;
                        return;
                    }

                    wk->cg_meoshi = chain_normal_air_table[wk->kow & 7];
                    wk->cg_cancel |= 8;
                }

                break;
            }
        }
    }
}

u16 check_xcopy_filter_se_req(WORK* wk) {
    u16 voif;

    if ((voif = wk->cg_se) < 0x160) {
        return voif;
    }

    if (wk->work_id != 1) {
        if (LO_2_BYTES(WK_AS_PLW->spmv_ng_flag) != 1) {
            return voif;
        }

        if ((u16)HI_2_BYTES(WK_AS_PLW->spmv_ng_flag) > 1) {
            return voif;
        }

        if (plw[HI_2_BYTES(WK_AS_PLW->spmv_ng_flag)].metamorphose == 0) {
            return voif;
        }

        return voif + 0x600;
    }

    if (WK_AS_PLW->metamorphose == 0) {
        return voif;
    }

    return voif + 0x600;
}

void check_cgd_patdat2(WORK* wk) {
    ST st;
    u16* seadrs;

    switch (wk->cgd_type) {
    case 6:
        if (wk->cg_status & 0x80) {
            wk->pat_status = wk->cg_status & 0x7F;
        }

        /* fallthrough */

    case 4:
        wk->cg_meoshi = wk->cg_hit_ix & 0x1FFF;
        st.w.h = wk->cg_att_ix;
        st.w.l = wk->cg_hit_ix;
        wk->cg_att_ix >>= 6;
        st.l *= 8;
        wk->cg_hit_ix = st.w.h & 0x1FF;

        if (wk->cg_att_ix) {
            set_new_attnum(wk);
        }

        break;
    }

    wk->cg_jphos = jphos_table[wk->cg_olc_ix & 0xF];
    wk->cg_olc_ix >>= 4;
    wk->cg_flip = wk->cg_se & 3;
    wk->cg_prio = (wk->cg_se & 0xF) >> 2;
    wk->cg_se >>= 4;

    if (wk->cg_se & 0x800) {
        seadrs = (u16*)&wk->se_random_table[wk->se_random_table[wk->cg_se & 0x7FF] / 4];
        wk->cg_se = seadrs[random_16()];
    }

    if (wk->work_id == 1) {
        if (wk->cg_rival == 0) {
            wk->curr_rca = NULL;
        } else {
            wk->curr_rca = wk->rival_catch_tbl + (wk->cg_rival + catch_table_offset(((PLW*)wk)->tsukami_num));
        }
    }

    wk->cg_olc = wk->olc_ix_table[wk->cg_olc_ix];
    wk->cg_ja = wk->hit_ix_table[wk->cg_hit_ix];

    set_jugde_area(wk);

    if (wk->cg_type != 0xFF && wk->cg_type & 0x80) {
        wk->cg_wca_ix = wk->cg_type & 0x7F;
        wk->cg_type = 0;
    }
}

void set_new_attnum(WORK* wk) {
    s16 aag_sw;

    wk->renew_attack = wk->cg_att_ix;

    att_req += 1;
    att_req &= 0x7FFF;

    if (att_req == 0) {
        att_req += 1;
    }

    aag_sw = 0;

    if (wk->cg_att_ix < 0) {
        wk->cg_att_ix = -wk->cg_att_ix;
        wk->attack_num = att_req;
        wk->att_hit_ok = 1;
        aag_sw = 1;
        wk->meoshi_hit_flag = 0;

        if (wk->work_id == 1) {
            WK_AS_PLW->caution_flag = 1;
            WK_AS_PLW->total_att_hit_ok += 1;
        }

        grade_add_att_renew((WORK_Other*)wk);
    }

    wk->att = *(wk->att_ix_table + wk->cg_att_ix);
    wk->zu_flag = wk->att.level & 0x80;
    wk->jump_att_flag = wk->att.level & 0x40;
    wk->at_attribute = (wk->att.level >> 4) & 3;
    wk->no_death_attack = wk->att.level & 8;
    wk->att.level &= 7;
    wk->kezuri_pow = kezuri_pow_table[(wk->att.guard >> 6) & 3];
    wk->att.guard &= 0x3F;
    wk->att_zuru = (wk->att.dir >> 4) & 7;
    wk->att.dir &= 0xF;
    wk->add_arts_point = (wk->att.piyo >> 4) & 0xF;
    wk->att.piyo &= 0xF;
    wk->vs_id = (wk->att.ng_type >> 4) & 0xF;
    wk->att.ng_type &= 0xF;
    wk->dir_atthit = cal_attdir(wk);

    if (aag_sw) {
        add_sp_arts_gauge_init((PLW*)wk);
    }

    if ((wk->work_id == 1) && !(WK_AS_PLW->spmv_ng_flag & DIP_EXTREME_CHIP_DAMAGE_DISABLED)) {
        setup_metamor_kezuri(wk);
    }
}

void setup_metamor_kezuri(WORK* wk) {
    if (wk->kezuri_pow == 0) {
        wk->kezuri_pow = kezuri_pow_table[4];
    }
}

void set_jugde_area(WORK* wk) {
    wk->h_bod = wk->body_adrs + wk->cg_ja.boix;
    wk->h_cat = wk->catch_adrs + wk->cg_ja.caix;
    wk->h_cau = wk->caught_adrs + wk->cg_ja.cuix;
    wk->h_att = wk->attack_adrs + wk->cg_ja.atix;
    wk->h_hos = wk->hosei_adrs + wk->cg_ja.hoix;
    wk->h_han = wk->hand_adrs + (wk->cg_ja.bhix + wk->cg_ja.haix);
}

void get_char_data_zanzou(WORK* wk) {
    if (wk->cg_att_ix) {
        set_new_attnum(wk);
    }

    wk->cg_ja = wk->hit_ix_table[wk->cg_hit_ix];
    set_jugde_area(wk);
}

const s16 jphos_table[16] = { 0x0000, 0xFFF0, 0xFFF4, 0xFFF8, 0xFFFC, 0x0004, 0x0008, 0x000C,
                              0x0010, 0x0014, 0x0018, 0x001C, 0x0020, 0x0024, 0x0028, 0x002C };

const s16 kezuri_pow_table[5] = { 0, 4, 8, 16, 24 };

s32 comm_dummy(WORK*, UNK11*);
s32 comm_roa(WORK*, UNK11*);
s32 comm_end(WORK*, UNK11*);
s32 comm_jmp(WORK*, UNK11*);
s32 comm_jpss(WORK*, UNK11*);
s32 comm_jsr(WORK*, UNK11*);
s32 comm_ret(WORK*, UNK11*);
s32 comm_sps(WORK*, UNK11*);
s32 comm_setr(WORK*, UNK11*);
s32 comm_addr(WORK*, UNK11*);
s32 comm_if_l(WORK*, UNK11*);
s32 comm_djmp(WORK*, UNK11*);
s32 comm_for(WORK*, UNK11*);
s32 comm_nex(WORK*, UNK11*);
s32 comm_for2(WORK*, UNK11*);
s32 comm_nex2(WORK*, UNK11*);
s32 comm_rja(WORK*, UNK11*);
s32 comm_uja(WORK*, UNK11*);
s32 comm_rja2(WORK*, UNK11*);
s32 comm_uja2(WORK*, UNK11*);
s32 comm_rja3(WORK*, UNK11*);
s32 comm_uja3(WORK*, UNK11*);
s32 comm_rja4(WORK*, UNK11*);
s32 comm_uja4(WORK*, UNK11*);
s32 comm_rja5(WORK*, UNK11*);
s32 comm_uja5(WORK*, UNK11*);
s32 comm_rja6(WORK*, UNK11*);
s32 comm_uja6(WORK*, UNK11*);
s32 comm_rja7(WORK*, UNK11*);
s32 comm_uja7(WORK*, UNK11*);
s32 comm_rmja(WORK*, UNK11*);
s32 comm_umja(WORK*, UNK11*);
s32 comm_mdat(WORK*, UNK11*);
s32 comm_ydat(WORK*, UNK11*);
s32 comm_mpos(WORK*, UNK11*);
s32 comm_cafr(WORK*, UNK11*);
s32 comm_care(WORK*, UNK11*);
s32 comm_psxy(WORK*, UNK11*);
s32 comm_ps_x(WORK*, UNK11*);
s32 comm_ps_y(WORK*, UNK11*);
s32 comm_paxy(WORK*, UNK11*);
s32 comm_pa_x(WORK*, UNK11*);
s32 comm_pa_y(WORK*, UNK11*);
s32 comm_exec(WORK*, UNK11*);
s32 comm_rngc(WORK*, UNK11*);
s32 comm_mxyt(WORK*, UNK11*);
s32 comm_pjmp(WORK*, UNK11*);
s32 comm_hjmp(WORK*, UNK11*);
s32 comm_hclr(WORK*, UNK11*);
s32 comm_ixfw(WORK*, UNK11*);
s32 comm_ixbw(WORK*, UNK11*);
s32 comm_quax(WORK*, UNK11*);
s32 comm_quay(WORK*, UNK11*);
s32 comm_if_s(WORK*, UNK11*);
s32 comm_rapp(WORK*, UNK11*);
s32 comm_rapk(WORK*, UNK11*);
s32 comm_gets(WORK*, UNK11*);
s32 comm_s123(WORK*, UNK11*);
s32 comm_s456(WORK*, UNK11*);
s32 comm_a123(WORK*, UNK11*);
s32 comm_a456(WORK*, UNK11*);
s32 comm_stop(PLW*, UNK11*);
s32 comm_smhf(WORK*, UNK11*);
s32 comm_ngme(WORK*, UNK11*);
s32 comm_ngem(WORK*, UNK11*);
s32 comm_iflb(WORK*, UNK11*);
s32 comm_asxy(WORK*, UNK11*);
s32 comm_schx(WORK*, UNK11*);
s32 comm_schy(WORK*, UNK11*);
s32 comm_back(WORK*, UNK11*);
s32 comm_mvix(WORK*, UNK11*);
s32 comm_sajp(WORK*, UNK11*);
s32 comm_ccch(WORK*, UNK11*);
s32 comm_wset(WORK*, UNK11*);
s32 comm_wswk(WORK*, UNK11*);
s32 comm_wadd(WORK*, UNK11*);
s32 comm_wceq(WORK*, UNK11*);
s32 comm_wcne(WORK*, UNK11*);
s32 comm_wcgt(WORK*, UNK11*);
s32 comm_wclt(WORK*, UNK11*);
s32 comm_wadd2(WORK*, UNK11*);
s32 comm_wceq2(WORK*, UNK11*);
s32 comm_wcne2(WORK*, UNK11*);
s32 comm_wcgt2(WORK*, UNK11*);
s32 comm_wclt2(WORK*, UNK11*);
s32 comm_rapp2(WORK*, UNK11*);
s32 comm_rapk2(WORK*, UNK11*);
s32 comm_iflg(WORK*, UNK11*);
s32 comm_mpcy(WORK*, UNK11*);
s32 comm_epcy(WORK*, UNK11*);
s32 comm_imgs(PLW*, UNK11*);
s32 comm_imgc(PLW*, UNK11*);
s32 comm_rvxy(WORK*, UNK11*);
s32 comm_rv_x(WORK*, UNK11*);
s32 comm_rv_y(WORK*, UNK11*);
s32 comm_ccfl(PLW*, UNK11*);
s32 comm_myhp(WORK*, UNK11*);
s32 comm_emhp(WORK*, UNK11*);
s32 comm_exbgs(WORK*, UNK11*);
s32 comm_exbgc(WORK*, UNK11*);
s32 comm_atmf(PLW*, UNK11*);
s32 comm_chkwf(PLW*, UNK11*);
s32 comm_retmj(PLW*, UNK11*);
s32 comm_sstx(WORK*, UNK11*);
s32 comm_ssty(WORK*, UNK11*);
s32 comm_ngda(WORK*, UNK11*);
s32 comm_flip(WORK*, UNK11*);
s32 comm_kage(WORK*, UNK11*);
s32 comm_dspf(WORK*, UNK11*);
s32 comm_ifrlf(WORK*, UNK11*);
s32 comm_srlf(WORK*, UNK11*);
s32 comm_bgrlf(WORK*, UNK11*);
s32 comm_scmd(PLW*, UNK11*);
s32 comm_rljmp(WORK*, UNK11*);
s32 comm_ifs2(WORK*, UNK11*);
s32 comm_abbak(WORK*, UNK11*);
s32 comm_sse(WORK*, UNK11*);
s32 comm_s_chg(WORK*, UNK11*);
s32 comm_schg2(WORK*, UNK11*);
s32 comm_rhsja(PLW*, UNK11*);
s32 comm_uhsja(PLW*, UNK11*);
s32 comm_ifcom(WORK*, UNK11*);
s32 comm_axjmp(WORK*, UNK11*);
s32 comm_ayjmp(WORK*, UNK11*);
s32 comm_ifs3(WORK*, UNK11*);

s32 (*const decode_chcmd[125])() = {
    comm_dummy, comm_roa,   comm_end,   comm_jmp,   comm_jpss,  comm_jsr,   comm_ret,   comm_sps,   comm_setr,
    comm_addr,  comm_if_l,  comm_djmp,  comm_for,   comm_nex,   comm_for2,  comm_nex2,  comm_rja,   comm_uja,
    comm_rja2,  comm_uja2,  comm_rja3,  comm_uja3,  comm_rja4,  comm_uja4,  comm_rja5,  comm_uja5,  comm_rja6,
    comm_uja6,  comm_rja7,  comm_uja7,  comm_rmja,  comm_umja,  comm_mdat,  comm_ydat,  comm_mpos,  comm_cafr,
    comm_care,  comm_psxy,  comm_ps_x,  comm_ps_y,  comm_paxy,  comm_pa_x,  comm_pa_y,  comm_exec,  comm_rngc,
    comm_mxyt,  comm_pjmp,  comm_hjmp,  comm_hclr,  comm_ixfw,  comm_ixbw,  comm_quax,  comm_quay,  comm_if_s,
    comm_rapp,  comm_rapk,  comm_gets,  comm_s123,  comm_s456,  comm_a123,  comm_a456,  comm_stop,  comm_smhf,
    comm_ngme,  comm_ngem,  comm_iflb,  comm_asxy,  comm_schx,  comm_schy,  comm_back,  comm_mvix,  comm_sajp,
    comm_ccch,  comm_wset,  comm_wswk,  comm_wadd,  comm_wceq,  comm_wcne,  comm_wcgt,  comm_wclt,  comm_wadd2,
    comm_wceq2, comm_wcne2, comm_wcgt2, comm_wclt2, comm_rapp2, comm_rapk2, comm_iflg,  comm_mpcy,  comm_epcy,
    comm_imgs,  comm_imgc,  comm_rvxy,  comm_rv_x,  comm_rv_y,  comm_ccfl,  comm_myhp,  comm_emhp,  comm_exbgs,
    comm_exbgc, comm_atmf,  comm_chkwf, comm_retmj, comm_sstx,  comm_ssty,  comm_ngda,  comm_flip,  comm_kage,
    comm_dspf,  comm_ifrlf, comm_srlf,  comm_bgrlf, comm_scmd,  comm_rljmp, comm_ifs2,  comm_abbak, comm_sse,
    comm_s_chg, comm_schg2, comm_rhsja, comm_uhsja, comm_ifcom, comm_axjmp, comm_ayjmp, comm_ifs3
};

s32 (*const decode_if_lever[16])() = { comm_dummy, comm_ret,  comm_uja,   comm_uja2, comm_uja3, comm_uja4,
                                       comm_uja5,  comm_uja6, comm_uja7,  comm_umja, comm_back, comm_nex,
                                       comm_nex2,  comm_wca,  comm_retmj, comm_abbak };

const u16 acatkoa_table[65] = { 4,   4,   8,   8,   8,   8,   8,   8,   16,  16,  16,  16,  16,  16,  16,  16,  32,
                                32,  32,  32,  32,  32,  32,  32,  64,  64,  64,  64,  64,  64,  64,  64,  128, 128,
                                128, 128, 128, 128, 128, 128, 256, 256, 256, 256, 256, 256, 256, 256, 128, 128, 128,
                                128, 128, 128, 128, 128, 256, 256, 256, 256, 256, 256, 256, 256, 2048 };
