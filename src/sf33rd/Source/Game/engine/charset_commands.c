/**
 * @file charset_commands.c
 * The character-script commands the interpreter dispatches to.
 *
 * Split out of charset.c, which was 2842 lines with 168 functions. Each comm_*
 * function implements one script opcode; decode_chcmd in charset.c indexes
 * them by opcode. The interpreter loop, the script setup and the pattern-data
 * checks stayed there.
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

/* Take the script's `ix` branch when the test passed and its `pat` branch when
 * it did not. comm_if_l wrote this out three times, once per lever mode; the
 * three tests differ and stay at their call sites, and all three are free of
 * side effects, so evaluating one before the call is the same as evaluating it
 * in the `if`. */
static s32 jump_ix_or_pat(WORK* wk, UNK11* ctc, s32 matched) {
    if (matched) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
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
        return jump_ix_or_pat(wk, ctc, lvdat == 0);
    } else if (my_lvdat & 0x8000) {
        return jump_ix_or_pat(wk, ctc, lvdat == (my_lvdat & 0xF));
    } else {
        return jump_ix_or_pat(wk, ctc, lvdat & my_lvdat);
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

// Player set X

// Player set Y

// Player add XY

// Player add X

// Player add Y

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

/* The six shot-test opcodes all read their operand the same way: bit 0x4000
 * means "one of my command work registers", anything else is the literal. */
static u16 comm_shot_operand(WORK* wk, UNK11* ctc) {
    if (ctc->koc & 0x4000) {
        return wk->cmwk[ctc->koc & 0xF];
    }

    return ctc->koc;
}

/* ...and they all end the same way, jumping to ix when the test holds and to
 * pat when it does not. The two tests stay apart: one compares, one masks. */
static s32 jump_on_shot_equal(WORK* wk, UNK11* ctc, u16 my_shdat, u16 shdat) {
    if (my_shdat == shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

static s32 jump_on_shot_overlap(WORK* wk, UNK11* ctc, u16 my_shdat, u16 shdat) {
    if (my_shdat & shdat) {
        return decord_if_jump(wk, ctc, ctc->ix);
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

/* One character's taunt shot pattern is ignored after a KO while the DIP switch
 * that disables post-KO taunts is set. Character 16 and the 0x440 pattern are
 * the original's, not a guess at which character that is. */
static s32 taunt_shot_is_suppressed(WORK* wk, u16 my_shdat) {
    return wk->work_id == 1 && ((PLW*)wk)->player_number == 16 && ((PLW*)wk)->spmv_ng_flag & DIP_TAUNT_AFTER_KO_DISABLED &&
           my_shdat == 0x440 && pcon_dp_flag;
}

s32 comm_if_s(WORK* wk, UNK11* ctc) {
    u16 my_shdat = comm_shot_operand(wk, ctc);
    u16 shdat = get_comm_if_shot(wk);

    if (taunt_shot_is_suppressed(wk, my_shdat)) {
        shdat = 0;
    }

    return jump_on_shot_equal(wk, ctc, my_shdat, shdat);
}

/* The four rapid-fire opcodes differ only in which waza_flag they watch: a
 * work of its own asks about its own player, a helper work about its master. */
static s32 jump_on_waza_flag(WORK* wk, UNK11* ctc, s32 waza_index) {
    if (wk->work_id == 1) {
        if (wcp[wk->id].waza_flag[waza_index]) {
            setup_comm_back(wk);
            set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
            return 0;
        }

        return 1;
    }

    if (wcp[((WORK_Other*)wk)->master_id & 1].waza_flag[waza_index]) {
        setup_comm_back(wk);
        set_char_move_init2(wk, ctc->koc, ctc->ix, ctc->pat, 1);
        return 0;
    }

    return 1;
}

s32 comm_rapp(WORK* wk, UNK11* ctc) {
    return jump_on_waza_flag(wk, ctc, 9);
}

s32 comm_rapk(WORK* wk, UNK11* ctc) {
    return jump_on_waza_flag(wk, ctc, 11);
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
    u16 my_shdat = comm_shot_operand(wk, ctc);
    u16 shdat = get_comm_if_lvsh(wk);

    return jump_on_shot_equal(wk, ctc, my_shdat, shdat);
}

s32 comm_back(WORK* wk, UNK11* /* unused */) {
    set_char_move_init2(wk, wk->cmbk.koc, wk->cmbk.ix, wk->cmbk.pat, 0);
    return 0;
}

/* This work's own super art is the one the script asks about, and it is
 * running. Character 18 is excluded, as in the original. */
static s32 own_art_is_running(WORK* wk, UNK11* ctc) {
    return My_char[wk->id] != 18 && ((PLW*)wk)->sa->kind_of_arts == ctc->koc && ((PLW*)wk)->sa->ok == -1;
}

/* The same question asked of an effect's master player. It checks work_id where
 * the other checks the character, so the two are not merged. */
static s32 master_art_is_running(PLW* pwk, UNK11* ctc) {
    return pwk->wu.work_id == 1 && pwk->sa->kind_of_arts == ctc->koc && pwk->sa->ok == -1;
}

s32 comm_sajp(WORK* wk, UNK11* ctc) {
    PLW* pwk;

    if (wk->work_id == 1) {
        if (own_art_is_running(wk, ctc)) {
            return decord_if_jump(wk, ctc, ctc->ix);
        }
    } else {
        pwk = (PLW*)((WORK_Other*)wk)->my_master;

        if (master_art_is_running(pwk, ctc)) {
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
    return jump_on_waza_flag(wk, ctc, 8);
}

s32 comm_rapk2(WORK* wk, UNK11* ctc) {
    return jump_on_waza_flag(wk, ctc, 10);
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
    u16 my_shdat = comm_shot_operand(wk, ctc);
    u16 shdat = get_comm_if_shot(wk);

    return jump_on_shot_overlap(wk, ctc, my_shdat, shdat);
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
    u16 my_shdat = comm_shot_operand(wk, ctc);
    u16 shdat = get_comm_if_shot_now_off(wk);

    return jump_on_shot_equal(wk, ctc, my_shdat, shdat);
}

s32 comm_schg2(WORK* wk, UNK11* ctc) {
    u16 my_shdat = comm_shot_operand(wk, ctc);
    u16 shdat = get_comm_if_shot_now_off(wk);

    return jump_on_shot_overlap(wk, ctc, my_shdat, shdat);
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
    u16 my_shdat = comm_shot_operand(wk, ctc);
    u16 shdat = get_comm_if_shot_now(wk);

    return jump_on_shot_overlap(wk, ctc, my_shdat, shdat);
}

