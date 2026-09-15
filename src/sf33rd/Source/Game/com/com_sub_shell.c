/**
 * @file com_sub_shell.c
 * CPU Controlled Character shell (projectile) reactions.
 *
 * Split out of com_sub.c unchanged - no function body was edited.
 */

#include "sf33rd/Source/Game/com/com_sub.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/active/ac0000.h"
#include "sf33rd/Source/Game/com/active/ac0001.h"
#include "sf33rd/Source/Game/com/active/ac0002.h"
#include "sf33rd/Source/Game/com/active/ac0003.h"
#include "sf33rd/Source/Game/com/active/ac0004.h"
#include "sf33rd/Source/Game/com/ck_pass.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_datu.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/com/follow/fl_com00.h"
#include "sf33rd/Source/Game/com/follow/fl_com02.h"
#include "sf33rd/Source/Game/com/passive/pass0000.h"
#include "sf33rd/Source/Game/com/passive/pass0001.h"
#include "sf33rd/Source/Game/com/passive/pass0002.h"
#include "sf33rd/Source/Game/com/passive/pass0003.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/screen/vs_shell.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"
#include "sf33rd/Source/Game/com/com_sub_internal.h"

/* The dodge roll: non-zero when the COM declines to react to the shell at all.
 * Applies CC_Value before the forced-CPU cap, unlike the guard-level rolls. */
static s32 Check_Diagonal_Shell_Declined(PLW* wk) {
    Lv = Setup_Lv08(0);
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_16_com();
    Lv += *CC_Value;

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }

    return Rnd > VS_Diagonal_Shell_Data[emLevelRemake(Lv, 8, 0)];
}

/* One shell slot: non-zero when this shell is worth dodging. */
static s32 Check_Diagonal_Shell_Slot(PLW* wk, WORK_Other* tmw) {
    if (tmw->wu.routine_no[1] == 2) {
        return 0;
    }
    if (wk->wu.rl_waza == tmw->wu.rl_flag) {
        return 0;
    }

    if (Check_Behind(wk, tmw) != 0) {
        return 0;
    }

    if (tmw->wu.charset_id == 2) {
        return 0;
    }
    return Check_Ignore_Shell2(tmw) != 0;
}

s32 Check_Diagonal_Shell(PLW* wk) {
    WORK_Other* tmw;
    WORK* em;
    s16 i;

    if (Check_Diagonal_Shell_Declined(wk)) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;

    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs(&wk->wu, em->id, i, &tmw) == 0)) {
            return 0;
        }

        if (Check_Diagonal_Shell_Slot(wk, tmw) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 Check_Ignore_Shell2(WORK_Other* tmw) {
    return Check_Ignore_Shell(tmw);
}

/* Commit to a reaction against one incoming shell. Returns 1 when the COM
 * switched pattern, 0 when Decide_Shell_Guard declined - both were returns out
 * of Check_Shell in the original, at nesting depth 4. */
static s32 Check_Shell_Engage(PLW* wk, WORK_Other* tmw) {
    s16 xx;

    xx = Compute_Hit_Time(wk, tmw);

    if (Decide_Shell_Guard(wk, tmw) != 0) {
        return 0;
    }

    CP_No[wk->wu.id][0] = 8;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;

    CP_Index[wk->wu.id][0] = 0;
    CP_Index[wk->wu.id][1] = 0;
    CP_Index[wk->wu.id][2] = 0;
    CP_Index[wk->wu.id][3] = 0;

    Shell_Address[wk->wu.id] = tmw;

    Guard_or_Jump_VS_Shell(wk, tmw, xx);

    return 1;
}

/* The slot filter the two full shell scans share: non-zero when this slot holds
 * nothing worth reacting to. Check_Diagonal_Shell uses a shorter form - it does
 * not test routine_no[0] - so it keeps its own.
 *
 * Written as one short-circuit chain rather than a ladder of returns: the order
 * matters because Check_Behind is only reached once the cheap tests pass, and
 * this form introduces no new constants for refactor_guard to read as a
 * substitution. */
static s32 Check_Shell_Slot_Skipped(PLW* wk, WORK_Other* tmw) {
    return (tmw->wu.routine_no[1] == 2) || (wk->wu.rl_waza == tmw->wu.rl_flag) ||
           (tmw->wu.routine_no[0] != 1) || (Check_Behind(wk, tmw) != 0) || (tmw->wu.charset_id == 2);
}

s32 Check_Shell(PLW* wk) {
    WORK_Other* tmw;
    WORK* em;
    s16 i;

    if (Shell_Ignore_Timer[wk->wu.id]) {
        Shell_Ignore_Timer[wk->wu.id]--;
        return 0;
    }
    if (CP_No[wk->wu.id][0] == 8) {
        return 0;
    }

    em = (WORK*)wk->wu.target_adrs;

    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs(&wk->wu, em->id, i, &tmw) == 0)) {
            return 0;
        }

        if (Check_Shell_Slot_Skipped(wk, tmw)) {
            continue;
        }

        if (Check_Ignore_Shell(tmw) == 0) {
            return Check_Shell_Engage(wk, tmw);
        }
    }

    return 0;
}

s32 Check_Shell_Another_in_Flip(PLW* wk) {
    WORK_Other* tmw;
    WORK* em;
    s32 i;
    s32 xx = 0;

    em = (WORK*)wk->wu.target_adrs;

    for (i = 0; i < 8; i++) {
        if ((get_vs_shell_adrs(em, em->id, i, &tmw) == 0) && (get_vs_shell_adrs(&wk->wu, em->id, i, &tmw) == 0)) {
            return 0;
        }

        if (Check_Shell_Slot_Skipped(wk, tmw)) {
            continue;
        }

        if (Check_Ignore_Shell(tmw) != 0) {
            continue;
        }

        xx = Compute_Hit_Time(wk, tmw);

        Shell_Address[wk->wu.id] = tmw;

        break;
    }
    return xx;
}

s32 Check_Ignore_Shell(WORK_Other* tmw) {
    if (tmw->wu.type == 0xDE) {
        return 1;
    }
    if ((tmw->wu.type >= 0x24) && (tmw->wu.type < 0x28)) {
        return 1;
    }
    if ((tmw->wu.type >= 0xD) && (tmw->wu.type < 0x10)) {
        return 1;
    }
    if ((tmw->wu.type == 0x54) || (tmw->wu.type == 0x55)) {
        return 1;
    }
    if ((tmw->wu.type >= 0x4D) && (tmw->wu.type < 0x51)) {
        return 1;
    }
    if ((tmw->wu.type >= 0x7A) && (tmw->wu.type < 0x7F)) {
        return 1;
    }

    return 0;
}

s32 Compute_Hit_Time(PLW* wk, WORK_Other* tmw) {
    s32 lx1;
    s32 divsp;
    s16 x2;

    lx1 = get_att_head_position(&tmw->wu);
    lx1 <<= 16;

    lx1 -= wk->wu.xyz[0].cal;

    if (tmw->wu.mvxy.a[0].sp != 0) {
        divsp = tmw->wu.mvxy.a[0].sp;
    } else {
        divsp = 0x48000;
    }

    x2 = lx1 / divsp;

    if (x2 < 0) {
        x2 = x2 * -1;
    }

    return x2;
}

s32 Decide_Shell_Guard(PLW* wk, WORK_Other* tmw) {
    s16 xx;

    Lv = Setup_Lv10(0);

    if (Break_Into_CPU == 2) {
        Lv = 9;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 2;
    }

    Rnd = random_32_com();

    xx = Setup_EM_Rank_Index(wk);
    if (Shell_Guard_Data[xx][emLevelRemake(Lv, 0xA, 0)] > Rnd) {
        return 0;
    }
    Shell_Ignore_Timer[wk->wu.id] = 0x3C;
    return 1;
}

/* Difficulty-dependent choice of shell reaction. The two random dodges fall
 * through to the default arm when they do not fire, exactly as written. */
static void Guard_Shell_By_Difficulty(PLW* wk, WORK_Other* tmw) {
    switch (save_w[Present_Mode].Difficulty) {
    case 7:
        if (wk->wu.vital_new < 4) {
            if (!(random_32_com() & 0xF)) {
                Pattern_Index[wk->wu.id] = 9;
                break;
            }
        }
        /* fallthrough */
    case 6:
        if (wk->wu.vital_new < 2) {
            if (!(random_32_com() & 7)) {
                Pattern_Index[wk->wu.id] = 9;
                break;
            }
        }
        /* fallthrough */
    default:
        Pattern_Index[wk->wu.id] = Decide_Shell_Reaction(wk, tmw, Shell_Change_Data_For_Reaction[tmw->wu.type]);
        break;
    }
}

void Guard_or_Jump_VS_Shell(PLW* wk, WORK_Other* tmw, s16 xx) {
    if (xx <= Shell_Dodge_Data[0][wk->player_number]) {
        if (Check_Flip_Term(wk, &tmw->wu) != 0) {
            Pattern_Index[wk->wu.id] = 9;
        } else {
            Pattern_Index[wk->wu.id] = 0;
        }
    } else {
        Guard_Shell_By_Difficulty(wk, tmw);
    }

    Setup_Shell_Disposal(wk, tmw);
}

void Setup_Shell_Disposal(PLW* wk, WORK_Other* tmw) {
    switch (Pattern_Index[wk->wu.id]) {
    case 0:
        Next_Be_Shell_Guard(wk, &tmw->wu);
        break;
    case 9:
        Passive_Flag[wk->wu.id] = 1;
        break;
    case 10:
        Pattern_Index[wk->wu.id] = Decide_Shell_Reaction(wk, tmw, 0);

        if (Pattern_Index[wk->wu.id] == 0) {
            Next_Be_Shell_Guard(wk, &tmw->wu);
        }

        break;
    }
}

void Next_Be_Shell_Guard(PLW* wk, WORK* tmw) {
    CP_No[wk->wu.id][0] = 9;
    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = 0xA;

    dash_flag_clear(wk->wu.id);
    Guard_Type[wk->wu.id] = 0;
    Check_Guard_Type(wk, tmw);
}

s32 Decide_Shell_Reaction(PLW* wk, WORK_Other* tmw, u16 dir_step) {
    if (dir_step == 0xFF) {
        return 0;
    }

    Lv = Setup_Lv08(0);

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }
    if ((Demo_Flag == 0) && (Weak_PL == wk->wu.id)) {
        Lv = 4;
    }

    Rnd = random_16_com();

    return VS_Shell_Active_Data[wk->player_number][dir_step][emLevelRemake(Lv, 8, 0)][Rnd];
}

