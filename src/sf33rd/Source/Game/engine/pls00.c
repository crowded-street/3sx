/**
 * @file pls00.c
 * Player State Dispatcher and Input Processor
 */

#include "sf33rd/Source/Game/engine/pls00.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/engine/pls00_internal.h"

const s8 lvdir_conv[4];

void (*const process_ndcca[5])(PLW* wk);
void (*const plpnm_xxxxx[59])(PLW* wk);
void (*const plpnm_xxxxx_arcade[59])(PLW* wk);
void (*const plpdm_xxxxx[32])(PLW* wk);
void (*const plpdm_xxxxx_arcade[39])(PLW* wk);

/* Runs check_nm_attack as the last term, exactly where the original did. */
static s32 normal_attack_cancels_into_new(PLW* wk) {
    return (wk->wu.cg_cancel & 4) && ((wk->cp->sw_now & 0x770) != wk->current_attack) && check_nm_attack(wk);
}

void check_lever_data(PLW* wk) { // 🟢
    if (wk->wu.routine_no[0] == 4) {
        process_ndcca[wk->wu.routine_no[1]](wk);
    }
}

void process_normal(PLW* wk) { // 🟢
    if (ArcadeBalance_IsEnabled()) {
        plpnm_xxxxx_arcade[wk->wu.routine_no[2]](wk);
    } else {
        plpnm_xxxxx[wk->wu.routine_no[2]](wk);
    }
}

void TO_nm_01000(WORK* wk) { // 🟢
    wk->routine_no[1] = 0;
    wk->routine_no[2] = 1;
    wk->routine_no[3] = 0;
    wk->cg_type = 0;
    nm_01000((PLW*)wk);
}

void TO_nm_36000(WORK* wk) { // 🟢
    wk->routine_no[1] = 0;
    wk->routine_no[2] = 36;
    wk->routine_no[3] = 0;
    wk->cg_type = 0;
    nm_01000((PLW*)wk);
}

void TO_nm_09000(WORK* wk) { // 🟢
    wk->routine_no[1] = 0;
    wk->routine_no[2] = 9;
    wk->routine_no[3] = 0;
    wk->cg_type = 0;
    nm_09000((PLW*)wk);
}

void TO_nm_37000(WORK* wk) { // 🟢
    wk->routine_no[1] = 0;
    wk->routine_no[2] = 37;
    wk->routine_no[3] = 0;
    wk->cg_type = 0;
    nm_09000((PLW*)wk);
}

void TO_nm_38000(WORK* wk) { // 🟢
    wk->routine_no[1] = 0;
    wk->routine_no[2] = 38;
    wk->routine_no[3] = 1;
    wk->cg_type = 0;
}

void TO_nm_18000_01(WORK* wk) { // 🟢
    wk->routine_no[1] = 0;
    wk->routine_no[2] = 18;
    wk->routine_no[3] = 1;
    wk->cg_type = 0;
    nm_18000((PLW*)wk);
}


static bool run_active_cancel_checks(PLW* wk) {
    if (check_full_gauge_attack(wk, 0)) {
        return true;
    }

    if (check_full_gauge_attack2(wk, 0)) {
        return true;
    }

    if (check_super_arts_attack(wk)) {
        return true;
    }

    if (check_special_attack(wk)) {
        return true;
    }

    if (check_chouhatsu(wk)) {
        return true;
    }

    if (check_catch_attack(wk)) {
        return true;
    }

    if (check_leap_attack(wk)) {
        return true;
    }

    return false;
}

/* Everything that can take the attack away from the player mid-move: the
 * ground check, a cancel inside its window, a full-gauge attack, and the
 * triangle jump. Returns 1 when one of them took over. */
static s32 attack_was_interrupted(PLW* wk) {
    if (check_ashimoto_ex(wk)) {
        return 1;
    }

    if (wk->cancel_timer && wk->wu.hit_stop == 0) {
        wk->cancel_timer--;
    }

    if (wk->cancel_timer && run_active_cancel_checks(wk)) {
        return 1;
    }

    if (wk->wu.routine_no[2] < 16 && check_full_gauge_attack(wk, 1)) {
        wk->wu.cg_cancel &= 0;
        return 1;
    }

    if (wk->wu.routine_no[2] == 3 && check_sankaku_tobi(wk)) {
        return 1;
    }

    return 0;
}

void process_attack(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] != 0) {
        if (attack_was_interrupted(wk)) {
            return;
        }
    }

    if (!check_cg_cancel_data(wk) && wk->wu.routine_no[3] != 0) {
        if (wk->wu.xyz[1].disp.pos == 0 || wk->bs2_on_car != 0) {
            jumping_cg_type_check(wk);
        }
    }
}

/* A normal loses its super-art cancel bits. A move that is already a special
 * keeps them, which is what the kind_of_waza test asks. */
static void clear_sa_cancel_bits(PLW* wk) {
    if (!(wk->wu.kind_of_waza & 0xF8)) {
        wk->wu.cg_cancel &= 0x9F;
    }
}

static void apply_special_move_cancel_mask(PLW* wk) {
    if (wk->wu.routine_no[1] != 4) {
        clear_sa_cancel_bits(wk);
        return;
    }

    switch (wk->player_number) {
    case CHAR_IBUKI:
        if (wk->wu.routine_no[2] != 25) {
            clear_sa_cancel_bits(wk);
        }

        break;

    case CHAR_TWELVE:
        if (wk->wu.routine_no[2] != 17) {
            clear_sa_cancel_bits(wk);
        }

        break;

    default:
        clear_sa_cancel_bits(wk);
        break;
    }
}

/* The three super-art cancels the 0x40 bit allows, in the order the original
 * tried them. Each clears cg_cancel before reporting success, exactly where the
 * original did. Returns 0 when none fired, which is where the original fell
 * through to the 0x20 test. */
static s32 try_super_art_cancel(PLW* wk) {
    if (check_full_gauge_attack(wk, 0)) {
        wk->wu.cg_cancel &= 0;
        return 1;
    }

    if (!ArcadeBalance_IsEnabled()) {
        if ((wk->player_number != CHAR_AKUMA) && check_full_gauge_attack2(wk, 0)) {
            wk->wu.cg_cancel &= 0;
            return 1;
        }
    }

    if (check_super_arts_attack(wk)) {
        wk->wu.cg_cancel &= 0;
        return 1;
    }

    return 0;
}

/* The two cancels the 0x20 bit allows. `||` short-circuits, so check_chouhatsu
 * is still only reached when check_special_attack declined - and neither is
 * called before the other, as in the original. */
static s32 try_special_or_taunt_cancel(PLW* wk) {
    return check_special_attack(wk) || check_chouhatsu(wk);
}

/* The cancels only a hit allows: the two DIP switches that take them away, and
 * the super-art and special-move cancels themselves. */
static s32 try_hit_cancels(PLW* wk) {
    if (wk->spmv_ng_flag2 & DIP2_SPECIAL_MOVE_SUPER_ART_CANCEL_DISABLED) {
        apply_special_move_cancel_mask(wk);
    }

    if ((wk->spmv_ng_flag2 & DIP2_SUPER_ART_CANCEL_DISABLED) && (wk->wu.kind_of_waza & 0xF8)) {
        wk->wu.cg_cancel &= 0xBF;
    }

    if ((wk->wu.cg_cancel & 0x40) && try_super_art_cancel(wk)) {
        return 1;
    }

    if ((wk->wu.cg_cancel & 0x20) && try_special_or_taunt_cancel(wk)) {
        return 1;
    }

    return 0;
}

/* The cancels that do not need a hit: rapid fire, push-out, and chaining into
 * another normal. */
static s32 try_chain_cancels(PLW* wk) {
    if ((wk->wu.cg_cancel & 16) && check_renda_cancel(wk)) {
        return 1;
    }

    if ((wk->wu.cg_cancel & 8) && check_meoshi_cancel(wk)) {
        return 1;
    }

    return normal_attack_cancels_into_new(wk);
}

/* And the two movement cancels, which need a hit again. */
static s32 try_movement_cancels(PLW* wk) {
    if ((wk->wu.cg_cancel & 2) && check_F_R_dash(wk)) {
        return 1;
    }

    if ((wk->wu.cg_cancel & 1) && check_hijump_only(wk)) {
        wk->high_jump_flag = 1;
        return 1;
    }

    return 0;
}

s32 check_cg_cancel_data(PLW* wk) { // 🟡
    if (wk->wu.cg_cancel == 0) {
        return 0;
    }

    if (wk->wu.meoshi_hit_flag != 0) {
        if (try_hit_cancels(wk)) {
            return 1;
        }
    }

    if (try_chain_cancels(wk)) {
        return 1;
    }

    if (wk->wu.meoshi_hit_flag == 0) {
        return 0;
    }

    return try_movement_cancels(wk);
}

const s8 lvdir_conv[4] = { 0, 2, 1, 0 };

void (*const process_ndcca[5])(PLW* wk) = {
    process_normal, process_damage, process_catch, process_caught, process_attack
};

void (*const plpnm_xxxxx[59])(PLW* wk) = {
    nm_00000, nm_01000, nm_02000, nm_03000, nm_03000, nm_05000, nm_05000, nm_07000, nm_08000, nm_09000,
    nm_10000, nm_03000, nm_03000, nm_03000, nm_03000, nm_03000, nm_16000, nm_17000, nm_18000, nm_18000,
    nm_18000, nm_18000, nm_18000, nm_18000, nm_18000, nm_18000, nm_18000, nm_27000, nm_27000, nm_29000,
    nm_27000, nm_31000, nm_31000, nm_31000, nm_34000, nm_34000, nm_36000, nm_37000, nm_38000, nm_39000,
    nm_40000, nm_40000, nm_42000, nm_42000, nm_42000, nm_45000, nm_45000, nm_47000, nm_48000, nm_49000,
    nm_49000, nm_51000, nm_52000, nm_52000, nm_51000, nm_55000, nm_55000, nm_57000, nm_55000
};

void (*const plpnm_xxxxx_arcade[59])(PLW* wk) = {
    nm_00000, nm_01000, nm_02000, nm_03000, nm_03000, nm_05000, nm_05000, nm_07000, nm_08000, nm_09000,
    nm_10000, nm_11000, nm_11000, nm_13000, nm_13000, nm_13000, nm_16000, nm_17000, nm_18000, nm_18000,
    nm_18000, nm_18000, nm_18000, nm_18000, nm_18000, nm_18000, nm_18000, nm_27000, nm_27000, nm_29000,
    nm_27000, nm_31000, nm_31000, nm_31000, nm_34000, nm_34000, nm_36000, nm_37000, nm_38000, nm_39000,
    nm_40000, nm_40000, nm_42000, nm_42000, nm_42000, nm_45000, nm_45000, nm_47000, nm_48000, nm_49000,
    nm_49000, nm_51000, nm_52000, nm_52000, nm_51000, nm_55000, nm_55000, nm_57000, nm_55000
};

void (*const plpdm_xxxxx[32])(PLW* wk) = { dm_00000, dm_04000, dm_04000, dm_04000, dm_04000, dm_04000, dm_04000,
                                           dm_04000, dm_08000, dm_08000, dm_08000, dm_08000, dm_04000, dm_04000,
                                           dm_18000, dm_18000, dm_04000, dm_17000, dm_18000, dm_18000, dm_18000,
                                           dm_18000, dm_18000, dm_18000, dm_00000, dm_25000, dm_18000, dm_18000,
                                           dm_18000, dm_18000, dm_18000, dm_18000 };

void (*const plpdm_xxxxx_arcade[39])(PLW* wk) = { dm_00000, dm_04000, dm_04000, dm_04000, dm_04000, dm_04000, dm_04000,
                                                  dm_04000, dm_08000, dm_08000, dm_08000, dm_08000, dm_04000, dm_04000,
                                                  dm_18000, dm_18000, dm_04000, dm_17000, dm_18000, dm_18000, dm_18000,
                                                  dm_18000, dm_18000, dm_18000, dm_00000, dm_25000, dm_18000, dm_18000,
                                                  dm_18000, dm_18000, dm_18000, dm_18000, dm_32000, dm_32000, dm_32000,
                                                  dm_32000, dm_33000, dm_33000, dm_33000 };
