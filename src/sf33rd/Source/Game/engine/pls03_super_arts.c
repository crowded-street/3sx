/**
 * @file pls03_super_arts.c
 * Super Art selection and execution.
 *
 * Split out of pls03.c, which was 1326 lines. These six functions are reached
 * only through pls03.h and call nothing else in pls03.c but the two helpers
 * declared in pls03_internal.h.
 */

#include "sf33rd/Source/Game/engine/pls03.h"
#include "arcade/arcade_balance.h"
#include "arcade/arcade_stubs.h"
#include "bin2obj/asstbl.h"
#include "common.h"
#include "constants.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03_internal.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/system/sysdir.h"


/// Check EX SA attack
/* One grounded EX super-art slot: every gate it must pass, then the
 * command match that fires it. Returns 1 when the art started.
 *
 * check_full_gauge_attack and check_full_gauge_attack2 were byte-identical
 * apart from which slot field they read, so the slot index is the parameter -
 * u8, the field's own type - and each caller passes its own field. */
static s32 try_grounded_ex_super(PLW* wk, u8 slot_ix, s8 always) {
    u16* conpane;
    s16 j;
    u16 cusw;
    u16 exsw;

    if (wk->spmv_ng_flag & DIP_UNKNOWN_30) {
        return 0;
    }

    if (slot_ix == 0) {
        return 0;
    }

    if (slot_ix > 0x1C) {
        return 0;
    }

    if (always && !(wk->cp->btix[slot_ix] & 0x100)) {
        return 0;
    }

    if ((wk->spmv_ng_flag2 & DIP2_UNKNOWN_23) && chainex_check[wk->wu.id][slot_ix - 20]) {
        return 0;
    }

    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 0x40;
    }

    if (is_blocked_by_arcade_switch(wk, slot_ix)) {
        return 0;
    }

    conpane = &wk->cp->sw_lvbt;

    if (wk->cp->waza_flag[slot_ix] == -1) {
        return 0;
    }

    if (((wk->cp->btix[slot_ix] & 0xFF) != 0x80) && wk->cp->waza_flag[slot_ix]) {
        cusw = conpane[wk->cp->btix[slot_ix] & 0xFF];

        for (j = 3; j >= 0; j--) {
            if ((j == 3) && !(wk->cp->btix[slot_ix] & 0x600)) {
                continue;
            }

            exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j]];

            if (exsw == cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j] & 0xF]) {
                setup_comm_back(&wk->wu);

                if (ArcadeBalance_IsEnabled()) {
                    wk->as = &asstbl_lv_9900_g_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                                     [j + (slot_ix - 20) * 4];
                } else {
                    wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(slot_ix)]
                                             [j + (slot_ix - 20) * 4];
                }

                wk->wu.cg_cancel = 0;
                wk->sa->mp = -1;
                hissatsu_setup_union(wk, wk->cp->waza_r[slot_ix][j]);
                waza_compel_all_init2(wk);

                if (!ArcadeBalance_IsEnabled()) {
                    chainex_check[wk->wu.id][slot_ix - 20] = 1;
                    chainex_spat_cancel_kidou(&wk->wu);
                }

                return 1;
            }
        }
    }

    return 0;
}

/* One airborne EX super-art slot. Not shared with the grounded version:
 * that one tests `> 0x1C` where this tests `< 0x1C`, and indexes its table
 * from 20 rather than 38. A comparison operator and an offset are not
 * values Recipe D may parameterise.
 *
 * check_full_gauge_attack and check_full_gauge_attack2 were byte-identical
 * apart from which slot field they read, so the slot index is the parameter -
 * u8, the field's own type - and each caller passes its own field. */
static s32 try_airborne_ex_super(PLW* wk, u8 slot_ix, s8 always) {
    u16* conpane;
    s16 j;
    u16 cusw;
    u16 exsw;

    if (wk->spmv_ng_flag & DIP_UNKNOWN_31) {
        return 0;
    }

    if (slot_ix == 0) {
        return 0;
    }

    if (slot_ix < 0x1C) {
        return 0;
    }

    if (always && !(wk->cp->btix[slot_ix] & 0x100)) {
        return 0;
    }

    if ((wk->spmv_ng_flag2 & DIP2_UNKNOWN_23) && chainex_check[wk->wu.id][slot_ix - 20]) {
        return 0;
    }

    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 0x40;
    }

    if (is_blocked_by_arcade_switch(wk, slot_ix)) {
        return 0;
    }

    conpane = &wk->cp->sw_lvbt;

    if (wk->cp->waza_flag[slot_ix] == -1) {
        return 0;
    }

    if (((wk->cp->btix[slot_ix] & 0xFF) != 0x80) && wk->cp->waza_flag[slot_ix]) {
        cusw = conpane[wk->cp->btix[slot_ix] & 0xFF];

        for (j = 3; j >= 0; j--) {
            if ((j == 3) && !(wk->cp->btix[slot_ix] & 0x600)) {
                continue;
            }

            exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j]];

            if (exsw == cmdshot_conv_tbl[wk->cp->exdt[slot_ix][j] & 0xF]) {
                setup_comm_back(&wk->wu);

                if (ArcadeBalance_IsEnabled()) {
                    wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                                     [j + (slot_ix - 38) * 4];
                } else {
                    wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(slot_ix)]
                                             [j + (slot_ix - 38) * 4];
                }

                wk->wu.cg_cancel = 0;
                wk->sa->mp = -1;
                hissatsu_setup_union(wk, wk->cp->waza_r[slot_ix][j]);
                waza_compel_all_init2(wk);

                if (!ArcadeBalance_IsEnabled()) {
                    chainex_check[wk->wu.id][slot_ix - 20] = 1;
                    chainex_spat_cancel_kidou(&wk->wu);
                }

                return 1;
            }
        }
    }

    return 0;
}

s32 check_full_gauge_attack(PLW* wk, s8 always) {
    if (wk->sa->mp != 1) {
        return 0;
    }

    if (pcon_dp_flag) {
        return 0;
    }

    if (player_is_grounded_or_on_car(wk)) {
        return try_grounded_ex_super(wk, wk->sa->exsa_g_ix, always);
    }

    return try_airborne_ex_super(wk, wk->sa->exsa_a_ix, always);
}

s32 check_full_gauge_attack2(PLW* wk, s8 always) {
    if (wk->sa->mp != 1) {
        return 0;
    }

    if (pcon_dp_flag) {
        return 0;
    }

    if (player_is_grounded_or_on_car(wk)) {
        return try_grounded_ex_super(wk, wk->sa->exs2_g_ix, always);
    }

    return try_airborne_ex_super(wk, wk->sa->exs2_a_ix, always);
}

s16 check_super_arts_attack(PLW* wk) { // 🟡
    s16 rnum = 0;
    s16 i;

    if (cmd_sel[wk->wu.id]) {
        if (wk->sa->ok != -1) {
            for (i = 0; i < 3; i++) {
                Super_Arts[wk->wu.id] = i;
                set_super_arts_status_dc(wk->wu.id);
                rnum = check_super_arts_attack_dc(wk);

                if (rnum) {
                    wk->sa->gt2 = wk->sa->gauge_type;
                    break;
                }
            }
        }
    } else {
        rnum = check_super_arts_attack_dc(wk);
    }

    return rnum;
}

static bool should_skip_dc_slot(PLW* wk, s16 ix, s16 j) {
    if (ArcadeBalance_IsEnabled()) {
        if ((j == 3) && !(wk->cp->btix[ix] & 0x600)) {
            return true;
        }
    } else {
        if ((j == 3) && (!(wk->cp->btix[ix] & 0x600) || (wk->sa->ex4th_full && (wk->sa->mp != 1)))) {
            return true;
        }
    }

    return false;
}

s32 check_super_arts_attack_dc(PLW* wk) { // 🟡
    s16 j;
    u16 cusw;
    u16 exsw;
    u16* conpane;

    if (wk->sa->ok != 1) {
        return 0;
    }

    if (pcon_dp_flag) {
        return 0;
    }

    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 1;
    }

    if (player_is_grounded_or_on_car(wk)) {
        if (wk->spmv_ng_flag & DIP_UNKNOWN_30) {
            return 0;
        }

        if (wk->sa->nmsa_g_ix == 0) {
            return 0;
        }

        if (wk->sa->nmsa_g_ix > 0x1C) {
            return 0;
        }

        if ((wk->spmv_ng_flag2 & DIP2_UNKNOWN_23) && chainex_check[wk->wu.id][wk->sa->nmsa_g_ix - 20]) {
            return 0;
        }

        if (is_blocked_by_arcade_switch(wk, wk->sa->nmsa_g_ix)) {
            return 0;
        }

        conpane = &wk->cp->sw_lvbt;

        if (wk->cp->waza_flag[wk->sa->nmsa_g_ix] == -1) {
            return 0;
        }

        if (((wk->cp->btix[wk->sa->nmsa_g_ix] & 0xFF) != 0x80) && wk->cp->waza_flag[wk->sa->nmsa_g_ix]) {
            cusw = conpane[wk->cp->btix[wk->sa->nmsa_g_ix] & 0xFF];

            for (j = 3; j >= 0; j--) {
                if (should_skip_dc_slot(wk, wk->sa->nmsa_g_ix, j)) {
                    continue;
                }

                exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_g_ix][j]];

                if (exsw == cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_g_ix][j] & 0xF]) {
                    setup_comm_back(&wk->wu);

                    if (ArcadeBalance_IsEnabled()) {
                        wk->as = &asstbl_lv_9900_g_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                                         [j + (wk->sa->nmsa_g_ix - 20) * 4];
                    } else {
                        wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_g_ix)]
                                                 [j + (wk->sa->nmsa_g_ix - 20) * 4];
                        wk->sa->ex4th_exec = (j == 3) * wk->sa->ex4th_full;
                    }

                    wk->wu.cg_cancel = 0;
                    wk->sa->ok = -1;
                    hissatsu_setup_union(wk, wk->cp->waza_r[wk->sa->nmsa_g_ix][j]);
                    waza_compel_all_init2(wk);

                    if (!ArcadeBalance_IsEnabled()) {
                        chainex_check[wk->wu.id][wk->sa->nmsa_g_ix - 20] = 1;
                        chainex_spat_cancel_kidou(&wk->wu);
                    }

                    return 1;
                }
            }
        }

        return 0;
    } else {
        if (wk->spmv_ng_flag & DIP_UNKNOWN_31) {
            return 0;
        }

        if (wk->sa->nmsa_a_ix == 0) {
            return 0;
        }

        if (wk->sa->nmsa_a_ix < 0x1C) {
            return 0;
        }

        if ((wk->spmv_ng_flag2 & DIP2_UNKNOWN_23) && chainex_check[wk->wu.id][wk->sa->nmsa_a_ix - 20]) {
            return 0;
        }

        if (is_blocked_by_arcade_switch(wk, wk->sa->nmsa_a_ix)) {
            return 0;
        }

        conpane = &wk->cp->sw_lvbt;

        if (wk->cp->waza_flag[wk->sa->nmsa_a_ix] == -1) {
            return 0;
        }

        if (((wk->cp->btix[wk->sa->nmsa_a_ix] & 0xFF) != 0x80) && (wk->cp->waza_flag[wk->sa->nmsa_a_ix])) {
            cusw = conpane[wk->cp->btix[wk->sa->nmsa_a_ix] & 0xFF];

            for (j = 3; j >= 0; j--) {
                if (should_skip_dc_slot(wk, wk->sa->nmsa_a_ix, j)) {
                    continue;
                }

                exsw = cusw & cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_a_ix][j]];

                if (exsw == cmdshot_conv_tbl[wk->cp->exdt[wk->sa->nmsa_a_ix][j] & 0xF]) {
                    setup_comm_back(&wk->wu);

                    if (ArcadeBalance_IsEnabled()) {
                        wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)]
                                                         [j + (wk->sa->nmsa_a_ix - 38) * 4];
                    } else {
                        wk->as = &_assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_a_ix)]
                                                 [j + (wk->sa->nmsa_a_ix - 38) * 4];
                        wk->sa->ex4th_exec = (j == 3) * wk->sa->ex4th_full;
                    }

                    wk->wu.cg_cancel = 0;
                    wk->sa->ok = -1;
                    hissatsu_setup_union(wk, wk->cp->waza_r[wk->sa->nmsa_a_ix][j]);
                    waza_compel_all_init2(wk);

                    if (!ArcadeBalance_IsEnabled()) {
                        chainex_check[wk->wu.id][wk->sa->nmsa_a_ix - 20] = 1;
                        chainex_spat_cancel_kidou(&wk->wu);
                    }

                    return 1;
                }
            }
        }

        return 0;
    }
}

s32 execute_super_arts(PLW* wk) { // 🟡
    if (wk->cancel_timer == 0) {
        wk->permited_koa |= 1;
    }

    if ((wk->sa->gauge_type != 3) && pcon_dp_flag) {
        return 0;
    }

    if (player_is_grounded_or_on_car(wk)) {
        if (wk->spmv_ng_flag & DIP_UNKNOWN_30) {
            return 0;
        }

        if (wk->sa->ok != 1) {
            return 0;
        }

        if (wk->sa->nmsa_g_ix > 0x1C) {
            return 0;
        }

        if (is_blocked_by_arcade_switch(wk, wk->sa->nmsa_g_ix)) {
            return 0;
        }

        setup_comm_back(&wk->wu);

        if (ArcadeBalance_IsEnabled()) {
            wk->as = &asstbl_lv_9900_g_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][(wk->sa->nmsa_g_ix - 20) * 4];
        } else {
            wk->as = _assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_g_ix)] + (wk->sa->nmsa_g_ix - 20) * 4;
            wk->sa->ex4th_exec = 0;
        }

        wk->wu.cg_cancel = 0;
        wk->sa->ok = -1;
        hissatsu_setup_union(wk, wk->cp->waza_r[wk->sa->nmsa_g_ix][0]);
        waza_compel_all_init2(wk);

        if (!ArcadeBalance_IsEnabled()) {
            wk->sa->gt2 = wk->sa->gauge_type;
        }

        return 1;
    } else {
        if (wk->spmv_ng_flag & DIP_UNKNOWN_31) {
            return 0;
        }

        if (wk->sa->ok != 1) {
            return 0;
        }

        if (wk->sa->nmsa_a_ix < 0x1C) {
            return 0;
        }

        if (is_blocked_by_arcade_switch(wk, wk->sa->nmsa_a_ix)) {
            return 0;
        }

        setup_comm_back(&wk->wu);

        if (ArcadeBalance_IsEnabled()) {
            wk->as = &asstbl_lv_9900_a_arcade[CHAR_3SX_TO_ARCADE(wk->player_number)][(wk->sa->nmsa_a_ix - 38) * 4];
        } else {
            wk->as = _assadr_lv_9900[wk->player_number][cmdixconv(wk->sa->nmsa_a_ix)] + (wk->sa->nmsa_a_ix - 38) * 4;
            wk->sa->ex4th_exec = 0;
        }

        wk->wu.cg_cancel = 0;
        wk->sa->ok = -1;
        hissatsu_setup_union(wk, wk->cp->waza_r[wk->sa->nmsa_a_ix][0]);
        waza_compel_all_init2(wk);

        if (!ArcadeBalance_IsEnabled()) {
            wk->sa->gt2 = wk->sa->gauge_type;
        }

        return 1;
    }
}
