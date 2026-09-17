/**
 * @file pls00_damage_states.c
 * The dm_* damage, catch and caught state handlers.
 *
 * Split out of pls00.c, which was 1535 lines. Reached through plpdm_xxxxx in
 * pls00.c, which still owns the dispatch tables.
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

static void reset_dm_stop_after_parry(PLW* wk) {
    if (wk->wu.operator == 0) {
        Next_Be_Free(wk);
    }

    if (wk->wu.dm_stop < 0) {
        if (wk->wu.dm_stop > -4) {
            wk->wu.dm_stop = -4;
        }
    } else if (wk->wu.dm_stop < 4) {
        wk->wu.dm_stop = 4;
    }
}

/* Semi-auto parry: each of the four guard states has a parry state to turn
 * into. The four are written out because grouping their case labels would
 * duplicate them. */
static void convert_guard_to_parry(PLW* wk) {
    s32 csw = 0;

    switch (wk->wu.routine_no[2]) {
    case 4:
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 31;
        csw = 1;
        break;

    case 5:
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 32;
        csw = 1;
        break;

    case 6:
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 33;
        csw = 1;
        break;

    case 7:
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 34;
        csw = 1;
        break;
    }

    if (csw) {
        reset_dm_stop_after_parry(wk);
    }
}

void process_damage(PLW* wk) { // 🟡
    if (wk->wu.routine_no[3] == 0) {
        if (!(wk->spmv_ng_flag & DIP_SEMI_AUTO_PARRY_DISABLED)) {
            convert_guard_to_parry(wk);
        }

        return;
    }

    if (ArcadeBalance_IsEnabled()) {
        plpdm_xxxxx_arcade[wk->wu.routine_no[2]](wk);
    } else {
        plpdm_xxxxx[wk->wu.routine_no[2]](wk);
    }
}

void dm_00000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[2] != 0) {
        return;
    }

    if (wk->wu.routine_no[3] != 2) {
        return;
    }

    if (check_sa_type_rebirth(wk) != 0) {
        wk->py->flag = 0;
        execute_super_arts(wk);
        return;
    }

    wk->wu.routine_no[3]++;
}

static void resolve_dm_04000_pat_status(PLW* wk, void (*on_low_pat_status)(WORK*), void (*on_high_pat_status)(WORK*)) {
    if (!ArcadeBalance_IsEnabled()) {
        if (setup_kuzureochi(wk) != 0) {
            return;
        }
    }

    if (wk->py->flag == 0) {
        wk->tsukamarenai_flag = 7;

        if (wk->wu.pat_status < 32) {
            on_low_pat_status(&wk->wu);
        } else {
            on_high_pat_status(&wk->wu);
        }

        return;
    }

    wk->wu.routine_no[2] = 19;
    wk->wu.routine_no[3] = 0;
}

void dm_04000(PLW* wk) { // 🟡
    switch (wk->wu.cg_type) {
    case 9:
        if (wk->py->flag == 0) {
            // do nothing
        }

        break;

    case 64:
        resolve_dm_04000_pat_status(wk, TO_nm_36000, TO_nm_37000);
        break;

    case 0xFF:
        resolve_dm_04000_pat_status(wk, TO_nm_01000, TO_nm_09000);
        break;
    }
}

void dm_08000(PLW* wk) { // 🟡
    switch (wk->wu.cg_type) {
    case 0xFF:
        wk->tsukamarenai_flag = 7;
        TO_nm_01000(&wk->wu);
        break;

    case 64:
        wk->tsukamarenai_flag = 7;
        TO_nm_36000(&wk->wu);
        break;

    default:
        if (!ArcadeBalance_IsEnabled()) {
            break;
        }

        if (wk->wu.routine_no[3] > 2) {
            break;
        }

        if (FUN_06120790(wk)) {
            wk->tsukamarenai_flag = 7;
        }

        break;
    }
}

void dm_17000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] == 3) {
        wk->wu.routine_no[1] = 0;
        wk->wu.routine_no[2] = 23;
        wk->wu.routine_no[3] = 2;
        jumping_cg_type_check(wk);
    }
}

void dm_18000(PLW* wk) { // 🟢
    switch (wk->wu.cg_type) {
    case 0xFF:
        if (wk->wu.vital_new < 0 && (check_sa_type_rebirth(wk) != 0)) {
            wk->py->flag = 0;
            execute_super_arts(wk);
            break;
        }

        if (wk->dead_flag) {
            wk->wu.routine_no[2] = 16;
        } else {
            wk->wu.routine_no[2] = 1;
        }

        wk->wu.routine_no[3] = 0;
        wk->wu.cg_type = 0;
        break;

    case 64:
        if (wk->py->flag == 0) {
            wk->tsukamarenai_flag = 7;
            TO_nm_36000(&wk->wu);
            break;
        }

        wk->wu.routine_no[2] = 19;
        wk->wu.routine_no[3] = 0;
        break;
    }
}

void dm_25000(PLW* wk) { // 🟢
    if (wk->sa_stop_flag == 1) {
        return;
    }

    if (--wk->py->time <= 0) {
        TO_nm_36000(&wk->wu);
        set_char_move_init(&wk->wu, 0, 47);
    }
}

static void dispatch_catch_cg_type(PLW* wk) {
    switch (wk->wu.cg_type) {
    case 64:
        if (wk->wu.pat_status < 32) {
            TO_nm_36000(&wk->wu);
        } else {
            TO_nm_37000(&wk->wu);
        }

        break;

    case 0xFF:
        if (wk->wu.pat_status < 32) {
            TO_nm_01000(&wk->wu);
        } else {
            TO_nm_09000(&wk->wu);
        }

        break;
    }
}

void process_catch(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] == 0) {
        return;
    }

    dispatch_catch_cg_type(wk);
}

void dm_32000(PLW* wk) { // 🔵
    dispatch_catch_cg_type(wk);
}

void process_caught(PLW* /* unused */) { // 🟢
    // Do nothing
}

void dm_33000(PLW* wk) { // 🔵
    // Do nothing
}

