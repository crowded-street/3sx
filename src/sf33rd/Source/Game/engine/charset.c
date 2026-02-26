/**
 * @file charset.c
 * The interpreter for character animation and logic scripts
 */

#include "sf33rd/Source/Game/engine/charset.h"
#include "common.h"
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

#define LO_2_BYTES(_val) (((s16*)&_val)[0])
#define HI_2_BYTES(_val) (((s16*)&_val)[1])
#define WK_AS_PLW ((PLW*)wk)

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
static u16 check_xcopy_filter_se_req(WORK* wk);
void check_cgd_patdat2(WORK* wk);
void setup_metamor_kezuri(WORK* wk);

void set_char_move_init(WORK* wk, s16 koc, s16 index) {
    wk->now_koc = koc;
    wk->char_index = index;

#if defined(CPS3)
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

#if !defined(CPS3)
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

#if !defined(CPS3)
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

#if defined(CPS3)
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
#if !defined(CPS3)
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

#if defined(CPS3)
    wk->set_char_ad = (u32*)wk->char_table[koc][index];
#else
    wk->set_char_ad = &wk->char_table[koc][wk->char_table[koc][index] / 4];
#endif

    now_ctr = wk->cg_ctr;

#if defined(CPS3)
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

#if defined(CPS3)
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

#if defined(CPS3)
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

#if defined(CPS3)
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

#if !defined(CPS3)
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

#if defined(CPS3)
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

#if !defined(CPS3)
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

s32 comm_dummy(WORK* /* unused */, UNK11* /* unused */) {
    return 1;
}

s32 comm_roa(WORK* wk, UNK11* /* unused */) {
    if (wk->cmoa.pat == 0) {
        wk->cmoa.koc = wk->now_koc;
        wk->cmoa.ix = wk->char_index;
        wk->cmoa.pat = 1;
    }

    set_char_move_init2(wk, wk->cmoa.koc, wk->cmoa.ix, wk->cmoa.pat, 0);
    return 0;
}

s32 comm_end(WORK* wk, UNK11* ctc) {
    wk->cg_ix = (ctc->pat - 2) * wk->cgd_type;
    return 1;
}

s32 comm_jmp(WORK* wk, UNK11* ctc) {
    setup_comm_back(wk);
    set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 0);
    return 0;
}

s32 comm_jpss(WORK* wk, UNK11* ctc) {
    setup_comm_back(wk);
    set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
    return 0;
}

s32 comm_jsr(WORK* wk, UNK11* ctc) {
    wk->cmsw.koc = wk->now_koc;
    wk->cmsw.ix = wk->char_index;
    wk->cmsw.pat = (wk->cg_ix / wk->cgd_type) + 2;
    set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 0);
    return 0;
}

s32 comm_ret(WORK* wk, UNK11* /* unused */) {
    set_char_move_init2(wk, wk->cmsw.koc, wk->cmsw.ix, wk->cmsw.pat, 0);
    return 0;
}

s32 comm_sps(WORK* wk, UNK11* ctc) {
    wk->pat_status = ctc->pat;
    return 1;
}

s32 comm_setr(WORK* wk, UNK11* ctc) {
    wk->routine_no[ctc->koc] = ctc->ix;
    return 1;
}

s32 comm_addr(WORK* wk, UNK11* ctc) {
    wk->routine_no[ctc->koc] += ctc->ix;
    return 1;
}

s32 comm_if_l(WORK* wk, UNK11* ctc) {
    u16 lvdat;
    u16 my_lvdat;

    if (ctc->koc & 0x4000) {
        my_lvdat = wk->cmwk[ctc->koc & 0xF];
    } else {
        my_lvdat = ctc->koc;
    }

    lvdat = get_comm_if_lever(wk);

    if (!(my_lvdat & 0x7FFF)) {
        if (lvdat == 0) {
            return decord_if_jump(wk, ctc, ctc->ix);
        } else {
            return decord_if_jump(wk, ctc, ctc->pat);
        }
    } else if (my_lvdat & 0x8000) {
        if (lvdat == (my_lvdat & 0xF)) {
            return decord_if_jump(wk, ctc, ctc->ix);
        } else {
            return decord_if_jump(wk, ctc, ctc->pat);
        }
    } else {
        if (lvdat & my_lvdat) {
            return decord_if_jump(wk, ctc, ctc->ix);
        } else {
            return decord_if_jump(wk, ctc, ctc->pat);
        }
    }
}

s32 comm_djmp(WORK* wk, UNK11* ctc) {
    u8 ldir;

    if ((ldir = get_comm_djmp_lever_dir((PLW*)wk))) {
        if (ldir == 1) {
            return decord_if_jump(wk, ctc, ctc->ix);
        } else {
            return decord_if_jump(wk, ctc, ctc->pat);
        }
    } else {
        return decord_if_jump(wk, ctc, ctc->koc);
    }
}

s32 comm_for(WORK* wk, UNK11* ctc) {
    if (ctc->pat & 0x4000) {
        wk->cmlp.code = wk->cmwk[ctc->pat & 0xF];
    } else {
        wk->cmlp.code = ctc->pat;
    }

    wk->cmlp.koc = wk->now_koc;
    wk->cmlp.ix = wk->char_index;
    wk->cmlp.pat = wk->cg_ix / wk->cgd_type + 2;
    return 1;
}

s32 comm_nex(WORK* wk, UNK11* ctc) {
    if (wk->cmlp.code && --wk->cmlp.code > 0) {
        set_char_move_init2(wk, wk->cmlp.koc, wk->cmlp.ix, wk->cmlp.pat, 1);
        return 0;
    } else {
        return 1;
    }
}

s32 comm_for2(WORK* wk, UNK11* ctc) {
    if (ctc->pat & 0x4000) {
        wk->cml2.code = wk->cmwk[ctc->pat & 0xF];
    } else {
        wk->cml2.code = ctc->pat;
    }

    wk->cml2.koc = wk->now_koc;
    wk->cml2.ix = wk->char_index;
    wk->cml2.pat = (wk->cg_ix / wk->cgd_type) + 2;
    return 1;
}

s32 comm_nex2(WORK* wk, UNK11* ctc) {
    if (wk->cml2.code && --wk->cml2.code > 0) {
        set_char_move_init2(wk, wk->cml2.koc, wk->cml2.ix, wk->cml2.pat, 1);
        return 0;
    } else {
        return 1;
    }
}

s32 comm_rja(WORK* wk, UNK11* ctc) {
    wk->cmja.koc = ctc->koc;
    wk->cmja.ix = ctc->ix;
    wk->cmja.pat = ctc->pat;
    return 1;
}

s32 comm_uja(WORK* wk, UNK11* ctc) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmja.koc, wk->cmja.ix, wk->cmja.pat, 0);
    return 0;
}

s32 comm_rja2(WORK* wk, UNK11* ctc) {
    wk->cmj2.koc = ctc->koc;
    wk->cmj2.ix = ctc->ix;
    wk->cmj2.pat = ctc->pat;
    return 1;
}

s32 comm_uja2(WORK* wk, UNK11* ctc) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj2.koc, wk->cmj2.ix, wk->cmj2.pat, 0);
    return 0;
}

s32 comm_rja3(WORK* wk, UNK11* ctc) {
    wk->cmj3.koc = ctc->koc;
    wk->cmj3.ix = ctc->ix;
    wk->cmj3.pat = ctc->pat;
    return 1;
}

s32 comm_uja3(WORK* wk, UNK11* ctc) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj3.koc, wk->cmj3.ix, wk->cmj3.pat, 0);
    return 0;
}

s32 comm_rja4(WORK* wk, UNK11* ctc) {
    wk->cmj4.koc = ctc->koc;
    wk->cmj4.ix = ctc->ix;
    wk->cmj4.pat = ctc->pat;
    return 1;
}

s32 comm_uja4(WORK* wk, UNK11* /* unused */) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj4.koc, wk->cmj4.ix, wk->cmj4.pat, 0);
    return 0;
}

s32 comm_rja5(WORK* wk, UNK11* ctc) {
    wk->cmj5.koc = ctc->koc;
    wk->cmj5.ix = ctc->ix;
    wk->cmj5.pat = ctc->pat;
    return 1;
}

s32 comm_uja5(WORK* wk, UNK11* /* unused */) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj5.koc, wk->cmj5.ix, wk->cmj5.pat, 0);
    return 0;
}

s32 comm_rja6(WORK* wk, UNK11* ctc) {
    wk->cmj6.koc = ctc->koc;
    wk->cmj6.ix = ctc->ix;
    wk->cmj6.pat = ctc->pat;
    return 1;
}

s32 comm_uja6(WORK* wk, UNK11* ctc) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj6.koc, wk->cmj6.ix, wk->cmj6.pat, 0);
    return 0;
}

s32 comm_rja7(WORK* wk, UNK11* ctc) {
    wk->cmj7.koc = ctc->koc;
    wk->cmj7.ix = ctc->ix;
    wk->cmj7.pat = ctc->pat;
    return 1;
}

s32 comm_uja7(WORK* wk, UNK11* ctc) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmj7.koc, wk->cmj7.ix, wk->cmj7.pat, 0);
    return 0;
}

s32 comm_rmja(WORK* wk, UNK11* ctc) {
    wk->cmms.koc = ctc->koc;
    wk->cmms.ix = ctc->ix;
    wk->cmms.pat = ctc->pat;
    return 1;
}

s32 comm_umja(WORK* wk, UNK11* /* unused */) {
    setup_comm_back(wk);
    set_char_move_init2(wk, wk->cmms.koc, wk->cmms.ix, wk->cmms.pat, 0);
    return 0;
}

s32 comm_mdat(WORK* wk, UNK11* ctc) {
    wk->cmmd.koc = ctc->koc;
    wk->cmmd.ix = ctc->ix;
    wk->cmmd.pat = ctc->pat;
    return 1;
}

s32 comm_ydat(WORK* wk, UNK11* ctc) {
    wk->cmyd.koc = ctc->koc;
    wk->cmyd.ix = ctc->ix;
    wk->cmyd.pat = ctc->pat;
    return 1;
}

s32 comm_mpos(WORK* wk, UNK11* ctc) { // TODO: Confirm CPS3 match
    wk->att.hit_mark = ctc->koc;
    wk->hit_mark_x = ctc->ix;
    wk->hit_mark_y = ctc->pat;
    return 1;
}

s32 comm_cafr(WORK* wk, UNK11* ctc) {
    wk->cmcf.koc = ctc->koc;
    wk->cmcf.ix = ctc->ix;
    wk->cmcf.pat = ctc->pat;
    return 1;
}

s32 comm_care(WORK* wk, UNK11* ctc) {
    wk->cmcr.koc = ctc->koc;
    wk->cmcr.ix = ctc->ix;
    wk->cmcr.pat = ctc->pat;
    return 1;
}

// Player set XY
s32 comm_psxy(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        wk->xyz[0].disp.pos = ctc->ix;
        wk->xyz[1].disp.pos = ctc->pat;
        break;

    case 2:
        wk->xyz[0].disp.pos = ctc->ix;
        wk->xyz[1].disp.pos = ctc->pat;
        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;
        emwk->xyz[0].disp.pos = ctc->ix;
        emwk->xyz[1].disp.pos = ctc->pat;
        break;
    }

    return 1;
}

// Player set X
s32 comm_ps_x(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        wk->xyz[0].disp.pos = ctc->ix;
        break;

    case 2:
        wk->xyz[0].disp.pos = ctc->ix;
        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;
        emwk->xyz[0].disp.pos = ctc->ix;
        break;
    }

    return 1;
}

// Player set Y
s32 comm_ps_y(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    if (wk->work_id == 1) {
        switch (ctc->koc) {
        case 0:
            // CPS3 compares to 21 here
            if (bg_w.stage == 20 && ((PLW*)wk)->bs2_on_car && ctc->pat < bs2_floor[2]) {
                wk->xyz[1].disp.pos = bs2_floor[2];
            } else {
                wk->xyz[1].disp.pos = ctc->pat;
            }

            break;

        case 2:
            wk->xyz[1].disp.pos = ctc->pat;
            /* fallthrough */

        default:
            emwk = (WORK*)wk->target_adrs;
            emwk->xyz[1].disp.pos = ctc->pat;
            break;
        }

        return 1;
    } else {
        switch (ctc->koc) {
        case 0:
            wk->xyz[1].disp.pos = ctc->pat;
            break;

        case 2:
            wk->xyz[1].disp.pos = ctc->pat;
            /* fallthrough */

        default:
            emwk = (WORK*)wk->target_adrs;
            emwk->xyz[1].disp.pos = ctc->pat;
            break;
        }

        return 1;
    }
}

// Player add XY
s32 comm_paxy(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        if (wk->rl_flag) {
            wk->xyz[0].cal += ctc->ix << 8;
        } else {
            wk->xyz[0].cal -= ctc->ix << 8;
        }

        wk->xyz[1].cal += ctc->pat << 8;
        break;

    case 2:
        if (wk->rl_flag) {
            wk->xyz[0].cal += ctc->ix << 8;
        } else {
            wk->xyz[0].cal -= ctc->ix << 8;
        }

        wk->xyz[1].cal += ctc->pat << 8;
        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;

        if (emwk->rl_flag) {
            emwk->xyz[0].cal += ctc->ix << 8;
        } else {
            emwk->xyz[0].cal -= ctc->ix << 8;
        }

        emwk->xyz[1].cal += ctc->pat << 8;
        break;
    }

    return 1;
}

// Player add X
s32 comm_pa_x(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        if (wk->rl_flag) {
            wk->xyz[0].cal += ctc->ix << 8;
        } else {
            wk->xyz[0].cal -= ctc->ix << 8;
        }

        break;

    case 2:
        if (wk->rl_flag) {
            wk->xyz[0].cal += ctc->ix << 8;
        } else {
            wk->xyz[0].cal -= ctc->ix << 8;
        }

        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;

        if (emwk->rl_flag) {
            emwk->xyz[0].cal += ctc->ix << 8;
        } else {
            emwk->xyz[0].cal -= ctc->ix << 8;
        }

        break;
    }

    return 1;
}

// Player add Y
s32 comm_pa_y(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        wk->xyz[1].cal += ctc->pat << 8;
        break;

    case 2:
        wk->xyz[1].cal += ctc->pat << 8;
        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;
        emwk->xyz[1].cal += ctc->pat << 8;
        break;
    }

    return 1;
}

s32 comm_exec(WORK* wk, UNK11* ctc) {
    effinitjptbl[ctc->koc](wk, (u8)ctc->ix);
    return 1;
}

s32 comm_rngc(WORK* wk, UNK11* ctc) {
    s16 rngdat;

    if (wk->work_id == 1) {
        rngdat = get_em_body_range(wk);
    } else {
        rngdat = get_em_body_range((WORK*)((WORK_Other*)wk)->my_master);
    }

    if (rngdat > ctc->koc) {
        return decord_if_jump(wk, ctc, ctc->pat);
    } else {
        return decord_if_jump(wk, ctc, ctc->ix);
    }
}

s32 comm_mxyt(WORK* wk, UNK11* ctc) {
    if (ctc->koc) {
        setup_mvxy_data(wk, ctc->koc);
    } else {
        reset_mvxy_data(wk);
    }

    return 1;
}

s32 comm_pjmp(WORK* wk, UNK11* ctc) {
    if (random_32() < ctc->koc) {
        return decord_if_jump(wk, ctc, ctc->ix);
    } else {
        return decord_if_jump(wk, ctc, ctc->pat);
    }
}

s32 comm_hjmp(WORK* wk, UNK11* ctc) {
    if (wk->meoshi_hit_flag != 0 && wk->hf.hit_flag != 0) {
        if (wk->hf.hit_flag & 0x303) {
            return decord_if_jump(wk, ctc, ctc->koc);
        }

        if (wk->hf.hit_flag & 0x3030) {
            return decord_if_jump(wk, ctc, ctc->ix);
        }

        if (wk->hf.hit_flag & 0xC0C0) {
            return decord_if_jump(wk, ctc, ctc->pat);
        }
    }

    return 1;
}

// Clear hit flag
s32 comm_hclr(WORK* wk, UNK11* /* unused */) {
    wk->hf.hit_flag = 0;
    return 1;
}

// Move command forward
s32 comm_ixfw(WORK* wk, UNK11* ctc) {
    if (test_flag == 0 || ixbfw_cut == 0) {
        wk->cg_ix += (ctc->pat - 1) * wk->cgd_type;
    }

    return 1;
}

// Move command backward
s32 comm_ixbw(WORK* wk, UNK11* ctc) {
    if ((test_flag == 0) || (ixbfw_cut == 0)) {
        wk->cg_ix -= (ctc->pat + 1) * wk->cgd_type;
    }

    return 1;
}

s32 comm_quax(WORK* /* unused */, UNK11* ctc) {
    bg_w.quake_x_index = ctc->koc;
    return 1;
}

s32 comm_quay(WORK* /* unused */, UNK11* ctc) {
    bg_w.quake_y_index = ctc->koc;
    pp_screen_quake(bg_w.quake_y_index);
    return 1;
}

s32 comm_if_s(WORK* wk, UNK11* ctc) {
    u16 shdat;
    u16 my_shdat;

    if (ctc->koc & 0x4000) {
        my_shdat = wk->cmwk[ctc->koc & 0xF];
    } else {
        my_shdat = ctc->koc;
    }

    shdat = get_comm_if_shot(wk);

    if (wk->work_id == 1 && ((PLW*)wk)->player_number == 16 && ((PLW*)wk)->spmv_ng_flag & DIP_TAUNT_AFTER_KO_DISABLED &&
        my_shdat == 0x440 && pcon_dp_flag) {
        shdat = 0;
    }

    if (my_shdat == shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_rapp(WORK* wk, UNK11* ctc) {
    if (wk->work_id == 1) {
        if (wcp[wk->id].waza_flag[9]) {
            setup_comm_back(wk);
            set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
            return 0;
        }

        return 1;
    }

    if (wcp[((WORK_Other*)wk)->master_id & 1].waza_flag[9]) {
        setup_comm_back(wk);
        set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
        return 0;
    }

    return 1;
}

s32 comm_rapk(WORK* wk, UNK11* ctc) {
    if (wk->work_id == 1) {
        if (wcp[wk->id].waza_flag[11]) {
            setup_comm_back(wk);
            set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
            return 0;
        }

        return 1;
    }

    if (wcp[((WORK_Other*)wk)->master_id & 1].waza_flag[11]) {
        setup_comm_back(wk);
        set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
        return 0;
    }

    return 1;
}

s32 comm_gets(WORK* wk, UNK11* /* unused */) {
    setupCharTableData(wk, 0, 1);
    return 1;
}

s32 comm_s123(WORK* wk, UNK11* ctc) {
    wk->routine_no[1] = ctc->koc;
    wk->routine_no[2] = ctc->ix;
    wk->routine_no[3] = ctc->pat;
    return 1;
}

s32 comm_s456(WORK* wk, UNK11* ctc) {
    wk->routine_no[4] = ctc->koc;
    wk->routine_no[5] = ctc->ix;
    wk->routine_no[6] = ctc->pat;
    return 1;
}

s32 comm_a123(WORK* wk, UNK11* ctc) {
    wk->routine_no[4] += ctc->koc;
    wk->routine_no[5] += ctc->ix;
    wk->routine_no[6] += ctc->pat;
    return 1;
}

s32 comm_a456(WORK* wk, UNK11* ctc) {
    wk->routine_no[4] += ctc->koc;
    wk->routine_no[5] += ctc->ix;
    wk->routine_no[6] += ctc->pat;
    return 1;
}

s32 comm_stop(PLW* wk, UNK11* ctc) {
    PLW* wk2;

    if (test_flag == 0) {
        wk->wu.dm_stop = 0;
        wk->wu.hit_stop = ctc->koc;
        wk2 = (PLW*)wk->wu.target_adrs;
        wk2->wu.hit_stop = ctc->ix;
        wk2->sa_stop_sai = ctc->ix - 4;

        if (wk2->sa_stop_sai < 0) {
            wk2->sa_stop_sai = 1;
        }

        setup_shell_hit_stop(&wk->wu, ctc->ix, ctc->pat);
        setup_shell_hit_stop(&wk2->wu, ctc->ix, 0);
        wk->sa_stop_flag = 0;
        wk2->sa_stop_flag = 2;
        wk2->just_sa_stop_timer = Game_timer;
    }

    return 1;
}

s32 comm_smhf(WORK* wk, UNK11* ctc) {
    wk->meoshi_hit_flag = ctc->koc;
    return 1;
}

s32 comm_ngme(WORK* wk, UNK11* /* unused */) {
    WORK* emwk;

    emwk = (WORK*)wk->hit_adrs;
    emwk->routine_no[1] = 3;
    emwk->routine_no[2] = 1;
    emwk->routine_no[3] = 1;

    if (test_flag) {
        wk->cmyd.pat = 1;
    }

    return 1;
}

s32 comm_ngem(WORK* wk, UNK11* /* unused */) {
    WORK* emwk;

    emwk = (WORK*)wk->hit_adrs;
    emwk->routine_no[1] = 3;
    emwk->routine_no[2] = 2;
    emwk->routine_no[3] = 1;

    if (test_flag) {
        wk->cmyd.pat = 2;
    }

    return 1;
}

s32 comm_iflb(WORK* wk, UNK11* ctc) {
    u16 shdat;
    u16 my_shdat;

    if (ctc->koc & 0x4000) {
        my_shdat = wk->cmwk[ctc->koc & 0xF];
    } else {
        my_shdat = ctc->koc;
    }

    shdat = get_comm_if_lvsh(wk);

    if (my_shdat == shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_asxy(WORK* wk, UNK11* ctc) {
    s16* from_rom2 = &wk->step_xy_table[ctc->koc];
    s32 st = *from_rom2++;

    st <<= 8;

    if (wk->rl_flag) {
        wk->xyz[0].cal += st;
    } else {
        wk->xyz[0].cal -= st;
    }

    st = *from_rom2;
    st <<= 8;
    wk->xyz[1].cal += st;
    return 1;
}

s32 comm_schx(WORK* wk, UNK11* ctc) {
    switch (ctc->koc) {
    case 0:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * ctc->ix) / ctc->pat;
        break;

    case 2:
        wk->mvxy.a[0].sp = (wk->mvxy.a[0].sp * ctc->ix) / ctc->pat;
        /* fallthrough */

    case 1:
        wk->mvxy.d[0].sp = (wk->mvxy.d[0].sp * ctc->ix) / ctc->pat;
        break;
    }

    return 1;
}

s32 comm_schy(WORK* wk, UNK11* ctc) {
    switch (ctc->koc) {
    case 0:
        wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * ctc->ix) / ctc->pat;
        break;

    case 2:
        wk->mvxy.a[1].sp = (wk->mvxy.a[1].sp * ctc->ix) / ctc->pat;
        /* fallthrough */

    case 1:
        wk->mvxy.d[1].sp = (wk->mvxy.d[1].sp * ctc->ix) / ctc->pat;
        break;
    }

    return 1;
}

s32 comm_back(WORK* wk, UNK11* /* unused */) {
    set_char_move_init2(wk, wk->cmbk.koc, wk->cmbk.ix, wk->cmbk.pat, 0);
    return 0;
}

s32 comm_mvix(WORK* wk, UNK11* ctc) {
    wk->mvxy.index = ctc->koc;
    return 1;
}

s32 comm_sajp(WORK* wk, UNK11* ctc) {
    PLW* pwk;

    if (wk->work_id == 1) {
        if (My_char[wk->id] != 18 && ((PLW*)wk)->sa->kind_of_arts == ctc->koc && ((PLW*)wk)->sa->ok == -1) {
            return decord_if_jump(wk, ctc, ctc->ix);
        }
    } else {
        pwk = (PLW*)((WORK_Other*)wk)->my_master;

        if (pwk->wu.work_id == 1 && pwk->sa->kind_of_arts == ctc->koc && pwk->sa->ok == -1) {
            return decord_if_jump(&pwk->wu, ctc, ctc->ix);
        }
    }

    return 1;
}

s32 comm_ccch(WORK* wk, UNK11* ctc) {
    if (ctc->koc) {
        wk->extra_col += ctc->ix;
        wk->extra_col &= 0x2FFF;
    } else {
        wk->extra_col = ctc->ix;
    }

    return 1;
}

s32 comm_wset(WORK* wk, UNK11* ctc) {
    switch (ctc->ix) {
    default:
        wk->cmwk[ctc->koc & 0xF] = ctc->pat;
        break;

    case 1:
        wk->cmwk[ctc->koc & 0xF] &= ctc->pat;
        break;

    case 2:
        wk->cmwk[ctc->koc & 0xF] |= ctc->pat;
        break;

    case 3:
        wk->cmwk[ctc->koc & 0xF] += ctc->pat;
        break;

    case 4:
        wk->cmwk[ctc->koc & 0xF] -= ctc->pat;
        break;

    case 5:
        wk->cmwk[ctc->koc & 0xF] *= ctc->pat;
        break;

    case 6:
        wk->cmwk[ctc->koc & 0xF] /= ctc->pat;
        break;
    }

    return 1;
}

s32 comm_wswk(WORK* wk, UNK11* ctc) {
    switch (ctc->ix) {
    default:
        wk->cmwk[ctc->koc & 0xF] = wk->cmwk[ctc->pat & 0xF];
        break;

    case 1:
        wk->cmwk[ctc->koc & 0xF] &= wk->cmwk[ctc->pat & 0xF];
        break;

    case 2:
        wk->cmwk[ctc->koc & 0xF] |= wk->cmwk[ctc->pat & 0xF];
        break;

    case 3:
        wk->cmwk[ctc->koc & 0xF] += wk->cmwk[ctc->pat & 0xF];
        break;

    case 4:
        wk->cmwk[ctc->koc & 0xF] -= wk->cmwk[ctc->pat & 0xF];
        break;

    case 5:
        wk->cmwk[ctc->koc & 0xF] *= wk->cmwk[ctc->pat & 0xF];
        break;

    case 6:
        wk->cmwk[ctc->koc & 0xF] /= wk->cmwk[ctc->pat & 0xF];
        break;
    }

    return 1;
}

s32 comm_wadd(WORK* wk, UNK11* ctc) {
    wk->cmwk[ctc->koc & 0xF] += ctc->ix;
    wk->cmwk[ctc->koc & 0xF] &= ctc->pat;
    return 1;
}

s32 comm_wceq(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] == ctc->ix) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_wcne(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] != ctc->ix) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }
    return 1;
}

s32 comm_wcgt(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] > ctc->ix) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_wclt(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] < ctc->ix) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_wadd2(WORK* wk, UNK11* ctc) {
    wk->cmwk[ctc->koc & 0xF] += wk->cmwk[ctc->ix & 0xF];
    wk->cmwk[ctc->koc & 0xF] &= ctc->pat;
    return 1;
}

s32 comm_wceq2(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] == wk->cmwk[ctc->ix & 0xF]) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_wcne2(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] != wk->cmwk[ctc->ix & 0xF]) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_wcgt2(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] > wk->cmwk[ctc->ix & 0xF]) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_wclt2(WORK* wk, UNK11* ctc) {
    if (wk->cmwk[ctc->koc & 0xF] < wk->cmwk[ctc->ix & 0xF]) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_rapp2(WORK* wk, UNK11* ctc) {
    if (wk->work_id == 1) {
        if (wcp[wk->id].waza_flag[8]) {
            setup_comm_back(wk);
            set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
            return 0;
        }

        return 1;
    }

    if (wcp[((WORK_Other*)wk)->master_id & 1].waza_flag[8]) {
        setup_comm_back(wk);
        set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
        return 0;
    }

    return 1;
}

s32 comm_rapk2(WORK* wk, UNK11* ctc) {
    if (wk->work_id == 1) {
        if (wcp[wk->id].waza_flag[10]) {
            setup_comm_back(wk);
            set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
            return 0;
        }

        return 1;
    }

    if (wcp[((WORK_Other*)wk)->master_id & 1].waza_flag[10]) {
        setup_comm_back(wk);
        set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
        return 0;
    }

    return 1;
}

s32 comm_iflg(WORK* wk, UNK11* ctc) {
    if (ctc->koc == 0) {
        if (wk->cmwk[11] < ctc->ix) {
            return 1;
        }

        return decord_if_jump(wk, ctc, ctc->pat);
    }

    if (((WORK*)wk->target_adrs)->cmwk[11] < ctc->ix) {
        return 1;
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_mpcy(WORK* wk, UNK11* ctc) {
    s16 ans = 0;

    switch (ctc->ix) {
    case 1:
        if (wk->xyz[1].disp.pos > ctc->koc) {
            ans = 1;
        }

        break;

    case 2:
        if (wk->xyz[1].disp.pos < ctc->koc) {
            ans = 1;
        }

        break;

    default:
        if (wk->xyz[1].disp.pos == ctc->koc) {
            ans = 1;
        }

        break;
    }

    if (ans == 0) {
        return 1;
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_epcy(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;
    s16 ans = 0;

    switch (ctc->ix) {
    case 1:
        if (emwk->xyz[1].disp.pos > ctc->koc) {
            ans = 1;
        }

        break;

    case 2:
        if (emwk->xyz[1].disp.pos < ctc->koc) {
            ans = 1;
        }

        break;

    default:
        if (emwk->xyz[1].disp.pos == ctc->koc) {
            ans = 1;
        }

        break;
    }

    if (ans == 0) {
        return 1;
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_imgs(PLW* wk, UNK11* ctc) {
    PLW* tk;

    if (test_flag == 0) {
        tk = (PLW*)wk->wu.target_adrs;

        switch (ctc->koc) {
        case 0:
            wk->image_setup_flag = 2;
            wk->image_data_index = ctc->ix;
            break;

        default:
            wk->image_setup_flag = 2;
            wk->image_data_index = ctc->ix;
            /* fallthrough */

        case 1:
            tk->image_setup_flag = 2;
            tk->image_data_index = ctc->ix;
            break;
        }
    }

    return 1;
}

s32 comm_imgc(PLW* wk, UNK11* ctc) {
    PLW* tk = (PLW*)wk->wu.target_adrs;

    switch (ctc->koc) {
    case 0:
        wk->image_setup_flag = 0;
        break;

    default:
        wk->image_setup_flag = 0;
        /* fallthrough */

    case 1:
        tk->image_setup_flag = 0;
        break;
    }

    return 1;
}

s32 comm_rvxy(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;

    switch (ctc->koc) {
    case 0:
        if (wk->rl_flag) {
            wk->xyz[0].cal = emwk->xyz[0].cal + (ctc->ix << 8);
        } else {
            wk->xyz[0].cal = emwk->xyz[0].cal - (ctc->ix << 8);
        }

        wk->xyz[1].cal = emwk->xyz[1].cal + (ctc->pat << 8);
        break;

    case 2:
        if (wk->rl_flag) {
            wk->xyz[0].cal = emwk->xyz[0].cal + (ctc->ix << 8);
        } else {
            wk->xyz[0].cal = emwk->xyz[0].cal - (ctc->ix << 8);
        }

        wk->xyz[1].cal = emwk->xyz[1].cal + (ctc->pat << 8);
        /* fallthrough */

    default:
        if (wk->rl_flag) {
            emwk->xyz[0].cal = wk->xyz[0].cal + (ctc->ix << 8);
        } else {
            emwk->xyz[0].cal = wk->xyz[0].cal - (ctc->ix << 8);
        }

        emwk->xyz[1].cal = wk->xyz[1].cal + (ctc->pat << 8);
        break;
    }

    return 1;
}

s32 comm_rv_x(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;

    switch (ctc->koc) {
    case 0:
        if (wk->rl_flag) {
            wk->xyz[0].cal = emwk->xyz[0].cal + (ctc->ix << 8);
        } else {
            wk->xyz[0].cal = emwk->xyz[0].cal - (ctc->ix << 8);
        }

        break;

    case 2:
        if (wk->rl_flag) {
            wk->xyz[0].cal = emwk->xyz[0].cal + (ctc->ix << 8);
        } else {
            wk->xyz[0].cal = emwk->xyz[0].cal - (ctc->ix << 8);
        }

        /* fallthrough */

    default:
        if (wk->rl_flag) {
            emwk->xyz[0].cal = wk->xyz[0].cal + (ctc->ix << 8);
        } else {
            emwk->xyz[0].cal = wk->xyz[0].cal - (ctc->ix << 8);
        }

        break;
    }

    return 1;
}

s32 comm_rv_y(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;

    switch (ctc->koc) {
    case 0:
        wk->xyz[1].cal = emwk->xyz[1].cal + (ctc->pat << 8);
        break;

    case 2:
        wk->xyz[1].cal = emwk->xyz[1].cal + (ctc->pat << 8);
        /* fallthrough */

    default:
        emwk->xyz[1].cal = wk->xyz[1].cal + (ctc->pat << 8);
        break;
    }

    return 1;
}

s32 comm_ccfl(PLW* wk, UNK11* /* unused */) {
    wk->caution_flag = 0;
    return 1;
}

s32 comm_myhp(WORK* wk, UNK11* ctc) {
    s16 num = 0;
    s32 cmpvital = (Max_vitality * ctc->ix) / 100;

    switch (ctc->koc) {
    case 1:
        if (wk->vital_new > cmpvital) {
            num = 1;
        }

        break;

    case 2:
        if (wk->vital_new < cmpvital) {
            num = 1;
        }

        break;

    default:
        if (wk->vital_new == cmpvital) {
            num = 1;
        }

        break;
    }

    if (num) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_emhp(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;
    s16 num = 0;
    s32 cmpvital = (Max_vitality * ctc->ix) / 100;

    switch (ctc->koc) {
    case 1:
        if (cmpvital < emwk->vital_new) {
            num = 1;
        }

        break;

    case 2:
        if (emwk->vital_new < cmpvital) {
            num = 1;
        }

        break;

    default:
        if (emwk->vital_new == cmpvital) {
            num = 1;
        }

        break;
    }

    if (num) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return 1;
}

s32 comm_exbgs(WORK* /* unused */, UNK11* /* unused */) {
    return 1;
}

s32 comm_exbgc(WORK* /* unused */, UNK11* /* unused */) {
    return 1;
}

s32 comm_atmf(PLW* wk, UNK11* ctc) {
    wk->atemi_flag = ctc->koc;
    wk->atemi_point = ctc->ix;
    return 1;
}

s32 comm_chkwf(PLW* wk, UNK11* ctc) {
    if (wk->cp->waza_flag[ctc->koc] == 0 || wk->cp->waza_flag[ctc->koc] == -1) {
        return decord_if_jump(&wk->wu, ctc, ctc->pat);
    }

    waza_flag_clear_only_1(wk->wu.id, ctc->koc);
    return decord_if_jump(&wk->wu, ctc, ctc->ix);
}

s32 comm_retmj(PLW* wk, UNK11* /* unused */) {
    wk->wu.now_koc = wk->wu.cmb2.koc;
    wk->wu.char_index = wk->wu.cmb2.ix;
    wk->wu.cg_ix = wk->wu.cmb2.pat;
    wk->wu.set_char_ad = &wk->wu.char_table[wk->wu.now_koc][wk->wu.char_table[wk->wu.now_koc][wk->wu.char_index] / 4];
    setupCharTableData(&wk->wu, 0, 1);
    wk->meoshi_jump_flag = 0;
    return 0;
}

s32 comm_sstx(WORK* wk, UNK11* ctc) {
    SST sstx;

    sstx.patl = 0;
    sstx.pats.h = ctc->pat;
    sstx.patl >>= 8;

    switch (ctc->koc) {
    case 0:
        switch (ctc->ix) {
        default:
            wk->mvxy.a[0].sp = sstx.patl;
            break;

        case 1:
            wk->mvxy.a[0].sp &= sstx.patl;
            break;

        case 2:
            wk->mvxy.a[0].sp |= sstx.patl;
            break;

        case 3:
            wk->mvxy.a[0].sp += sstx.patl;
            break;

        case 4:
            wk->mvxy.a[0].sp -= sstx.patl;
            break;

        case 5:
            wk->mvxy.a[0].sp *= sstx.patl;
            break;

        case 6:
            wk->mvxy.a[0].sp /= sstx.patl;
            break;
        }

        break;

    case 2:
        switch (ctc->ix) {
        default:
            wk->mvxy.a[0].sp = sstx.patl;
            break;

        case 1:
            wk->mvxy.a[0].sp &= sstx.patl;
            break;

        case 2:
            wk->mvxy.a[0].sp |= sstx.patl;
            break;

        case 3:
            wk->mvxy.a[0].sp += sstx.patl;
            break;

        case 4:
            wk->mvxy.a[0].sp -= sstx.patl;
            break;

        case 5:
            wk->mvxy.a[0].sp *= sstx.patl;
            break;

        case 6:
            wk->mvxy.a[0].sp /= sstx.patl;
            break;
        }

        /* fallthrough */

    case 1:
        switch (ctc->ix) {
        default:
            wk->mvxy.d[0].sp = sstx.patl;
            break;

        case 1:
            wk->mvxy.d[0].sp &= sstx.patl;
            break;

        case 2:
            wk->mvxy.d[0].sp |= sstx.patl;
            break;

        case 3:
            wk->mvxy.d[0].sp += sstx.patl;
            break;

        case 4:
            wk->mvxy.d[0].sp -= sstx.patl;
            break;

        case 5:
            wk->mvxy.d[0].sp *= sstx.patl;
            break;

        case 6:
            wk->mvxy.d[0].sp /= sstx.patl;
            break;
        }

        break;

    default:
        wk->mvxy.kop[0] = ctc->pat;
        break;
    }

    return 1;
}

s32 comm_ssty(WORK* wk, UNK11* ctc) {
    SST ssty;

    ssty.patl = 0;
    ssty.pats.h = ctc->pat;
    ssty.patl >>= 8;

    switch (ctc->koc) {
    case 0:
        switch (ctc->ix) {
        default:
            wk->mvxy.a[1].sp = ssty.patl;
            break;

        case 1:
            wk->mvxy.a[1].sp &= ssty.patl;
            break;

        case 2:
            wk->mvxy.a[1].sp |= ssty.patl;
            break;

        case 3:
            wk->mvxy.a[1].sp += ssty.patl;
            break;

        case 4:
            wk->mvxy.a[1].sp -= ssty.patl;
            break;

        case 5:
            wk->mvxy.a[1].sp *= ssty.patl;
            break;

        case 6:
            wk->mvxy.a[1].sp /= ssty.patl;
            break;
        }

        break;

    case 2:
        switch (ctc->ix) {
        default:
            wk->mvxy.a[1].sp = ssty.patl;
            break;

        case 1:
            wk->mvxy.a[1].sp &= ssty.patl;
            break;

        case 2:
            wk->mvxy.a[1].sp |= ssty.patl;
            break;

        case 3:
            wk->mvxy.a[1].sp += ssty.patl;
            break;

        case 4:
            wk->mvxy.a[1].sp -= ssty.patl;
            break;

        case 5:
            wk->mvxy.a[1].sp *= ssty.patl;
            break;

        case 6:
            wk->mvxy.a[1].sp /= ssty.patl;
            break;
        }

        /* fallthrough */

    case 1:
        switch (ctc->ix) {
        default:
            wk->mvxy.d[1].sp = ssty.patl;
            break;

        case 1:
            wk->mvxy.d[1].sp &= ssty.patl;
            break;

        case 2:
            wk->mvxy.d[1].sp |= ssty.patl;
            break;

        case 3:
            wk->mvxy.d[1].sp += ssty.patl;
            break;

        case 4:
            wk->mvxy.d[1].sp -= ssty.patl;
            break;

        case 5:
            wk->mvxy.d[1].sp *= ssty.patl;
            break;

        case 6:
            wk->mvxy.d[1].sp /= ssty.patl;
            break;
        }

        break;

    default:
        wk->mvxy.kop[1] = ctc->pat;
        break;
    }

    return 1;
}

s32 comm_ngda(WORK* wk, UNK11* ctc) {
    wk->cmyd.koc = ctc->koc;
    wk->cmyd.ix = ctc->ix;
    wk->cmyd.pat = ctc->pat;
    return 1;
}

s32 comm_flip(WORK* wk, UNK11* /* unused */) {
    wk->rl_flag = (wk->rl_flag + 1) & 1;
    return 1;
}

s32 comm_kage(WORK* wk, UNK11* ctc) {
    wk->kage_hx = ctc->koc;
    wk->kage_hy = ctc->ix;
    wk->kage_char = ctc->pat;
    return 1;
}

s32 comm_dspf(WORK* wk, UNK11* ctc) {
    wk->disp_flag = ctc->koc;
    return 1;
}

s32 comm_ifrlf(WORK* wk, UNK11* ctc) {
    if (ctc->koc) {
        if (wk->rl_flag == wk->rl_waza) {
            return decord_if_jump(wk, ctc, ctc->pat);
        }

        return decord_if_jump(wk, ctc, ctc->ix);
    }

    if (wk->rl_flag == wk->rl_waza) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_srlf(WORK* wk, UNK11* ctc) {
    if (ctc->koc) {
        if (wk->rl_flag != wk->rl_waza) {
            wk->rl_flag = wk->rl_waza;
        }
    } else if (wk->rl_flag == wk->rl_waza) {
        wk->rl_flag = (wk->rl_flag + 1) & 1;
    }

    return 1;
}

s32 comm_bgrlf(WORK* wk, UNK11* ctc) {
    if (wk->rl_flag) {
        if (wk->position_x > bg_w.bgw[1].pos_x_work) {
            return decord_if_jump(wk, ctc, ctc->pat);
        }

        return decord_if_jump(wk, ctc, ctc->ix);
    }

    if (wk->position_x < bg_w.bgw[1].pos_x_work) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return decord_if_jump(wk, ctc, ctc->ix);
}

s32 comm_scmd(PLW* wk, UNK11* ctc) {
    wk->cmd_request = ctc->koc;
    return 1;
}

s32 comm_rljmp(WORK* wk, UNK11* ctc) {
    if (wk->rl_flag) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return decord_if_jump(wk, ctc, ctc->ix);
}

s32 comm_ifs2(WORK* wk, UNK11* ctc) {
    u16 shdat;
    u16 my_shdat;

    if (ctc->koc & 0x4000) {
        my_shdat = wk->cmwk[ctc->koc & 0xF];
    } else {
        my_shdat = ctc->koc;
    }

    shdat = get_comm_if_shot(wk);

    if (my_shdat & shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_abbak(WORK* wk, UNK11* /* unused */) {
    set_char_move_init2(wk, wk->cmb3.koc, wk->cmb3.ix, wk->cmb3.pat, 0);
    return 0;
}

s32 comm_sse(WORK* wk, UNK11* ctc) {
    u16* seadrs;

    wk->cg_se = ctc->koc;

    if (wk->cg_se & 0x800) {
        seadrs = (u16*)&wk->se_random_table[wk->se_random_table[wk->cg_se & 0x7FF] / 4];
        wk->cg_se = seadrs[random_16()];
    }

    if (wk->cg_se) {
        sound_effect_request[wk->cg_se](wk, check_xcopy_filter_se_req(wk));
    }

    return 1;
}

s32 comm_s_chg(WORK* wk, UNK11* ctc) {
    u16 shdat;
    u16 my_shdat;

    if (ctc->koc & 0x4000) {
        my_shdat = wk->cmwk[ctc->koc & 0xF];
    } else {
        my_shdat = ctc->koc;
    }

    shdat = get_comm_if_shot_now_off(wk);

    if (my_shdat == shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_schg2(WORK* wk, UNK11* ctc) {
    u16 shdat;
    u16 my_shdat;

    if (ctc->koc & 0x4000) {
        my_shdat = wk->cmwk[ctc->koc & 0xF];
    } else {
        my_shdat = ctc->koc;
    }

    shdat = get_comm_if_shot_now_off(wk);

    if (my_shdat & shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_rhsja(PLW* wk, UNK11* ctc) {
    wk->wu.cmhs.koc = ctc->koc;
    wk->wu.cmhs.ix = ctc->ix;
    wk->wu.cmhs.pat = ctc->pat;
    wk->hsjp_ok = 1;
    return 1;
}

s32 comm_uhsja(PLW* wk, UNK11* /* unused */) {
    setup_comm_back(&wk->wu);
    wk->hsjp_ok = 0;
    set_char_move_init2(&wk->wu, wk->wu.cmhs.koc, wk->wu.cmhs.ix, wk->wu.cmhs.pat, 0);
    return 0;
}

s32 comm_ifcom(WORK* wk, UNK11* ctc) {
    if (wk->operator) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return decord_if_jump(wk, ctc, ctc->ix);
}

s32 comm_axjmp(WORK* wk, UNK11* ctc) {
    if (wk->mvxy.a[0].real.h > 2) {
        return decord_if_jump(wk, ctc, ctc->koc);
    }

    if (wk->mvxy.a[0].real.h < -2) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return decord_if_jump(wk, ctc, ctc->ix);
}

s32 comm_ayjmp(WORK* wk, UNK11* ctc) {
    if (wk->mvxy.a[1].real.h > 0) {
        return decord_if_jump(wk, ctc, ctc->koc);
    }

    if (wk->mvxy.a[1].real.h < 0) {
        return decord_if_jump(wk, ctc, ctc->pat);
    }

    return decord_if_jump(wk, ctc, ctc->ix);
}

s32 comm_ifs3(WORK* wk, UNK11* ctc) {
    u16 shdat;
    u16 my_shdat;

    if (ctc->koc & 0x4000) {
        my_shdat = wk->cmwk[ctc->koc & 0xF];
    } else {
        my_shdat = ctc->koc;
    }

    shdat = get_comm_if_shot_now(wk);

    if (my_shdat & shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
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
            wk->curr_rca = wk->rival_catch_tbl + (wk->cg_rival - 20 + ((PLW*)wk)->tsukami_num);
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
        if ((WK_AS_PLW->spmv_ng_flag2 & DIP2_TARGET_COMBO_DISABLED) && (wk->cg_cancel & 8) && !(wk->kow & 0xF8)) {
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

        if (!(WK_AS_PLW->spmv_ng_flag2 & DIP2_SPECIAL_TO_SPECIAL_CANCEL_DISABLED) && !(wk->kow & 0x60) &&
            (wk->kow & 0xF8) && (wk->cg_cancel & 0x40)) {
            wk->cg_cancel |= 0x60;
        }

        if (!(wk->kow & 0xF8) && (wk->routine_no[1] == 4) && (wk->routine_no[2] < 16)) {
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
            wk->curr_rca = wk->rival_catch_tbl + (wk->cg_rival - 20 + ((PLW*)wk)->tsukami_num);
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
