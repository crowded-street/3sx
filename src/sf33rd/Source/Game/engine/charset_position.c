/**
 * @file charset_position.c
 * The character-script commands that move a work: position, speed and the
 * tests on them.
 *
 * Split out of charset_commands.c, which was 1764 lines with 132 functions.
 * These are the opcodes that write xyz or mvxy, or branch on one of them -
 * psxy/ps_x/ps_y, paxy/pa_x/pa_y, asxy, schx/schy, rvxy/rv_x/rv_y, mpcy/epcy,
 * sstx/ssty and mxyt/mvix. Every duplication family among them moved whole.
 * decode_chcmd in charset.c still indexes them by opcode.
 */

#include "sf33rd/Source/Game/engine/charset.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/engine/charset_internal.h"
static s32 on_bonus_car_below_floor(const WORK* wk, const UNK11* ctc) {
    return bg_w.stage == 20 && ((PLW*)wk)->bs2_on_car && ctc->pat < bs2_floor[2];
}

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

/* A player work's own Y set, which is the only one that has to keep the rider
 * of the bonus-stage car above the floor. */
static void set_player_y(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        // CPS3 compares to 21 here
        if (on_bonus_car_below_floor(wk, ctc)) {
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
}

/* The same set for any other work, which has no car to stand on. */
static void set_other_y(WORK* wk, UNK11* ctc) {
    WORK* emwk;

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
}

s32 comm_ps_y(WORK* wk, UNK11* ctc) {
    if (wk->work_id == 1) {
        set_player_y(wk, ctc);
        return 1;
    }

    set_other_y(wk, ctc);
    return 1;
}

/* The three relative-move opcodes share these two steps: X moves the way the
 * work faces, Y always the same way. */
static void add_script_x_offset(WORK* wk, UNK11* ctc) {
    if (wk->rl_flag) {
        wk->xyz[0].cal += ctc->ix << 8;
    } else {
        wk->xyz[0].cal -= ctc->ix << 8;
    }
}

static void add_script_y_offset(WORK* wk, UNK11* ctc) {
    wk->xyz[1].cal += ctc->pat << 8;
}

s32 comm_paxy(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        add_script_x_offset(wk, ctc);
        add_script_y_offset(wk, ctc);
        break;

    case 2:
        add_script_x_offset(wk, ctc);
        add_script_y_offset(wk, ctc);
        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;

        add_script_x_offset(emwk, ctc);
        add_script_y_offset(emwk, ctc);
        break;
    }

    return 1;
}

s32 comm_pa_x(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        add_script_x_offset(wk, ctc);
        break;

    case 2:
        add_script_x_offset(wk, ctc);
        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;

        add_script_x_offset(emwk, ctc);
        break;
    }

    return 1;
}

s32 comm_pa_y(WORK* wk, UNK11* ctc) {
    WORK* emwk;

    switch (ctc->koc) {
    case 0:
        add_script_y_offset(wk, ctc);
        break;

    case 2:
        add_script_y_offset(wk, ctc);
        /* fallthrough */

    default:
        emwk = (WORK*)wk->target_adrs;
        add_script_y_offset(emwk, ctc);
        break;
    }

    return 1;
}

s32 comm_mxyt(WORK* wk, UNK11* ctc) {
    if (ctc->koc) {
        setup_mvxy_data(wk, ctc->koc);
    } else {
        reset_mvxy_data(wk);
    }

    return 1;
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

/* comm_schx and comm_schy scale a work's speed by ix/pat; they differ only in
 * which axis of mvxy they scale. */
static s32 scale_speed_on_axis(WORK* wk, UNK11* ctc, s32 axis) {
    switch (ctc->koc) {
    case 0:
        wk->mvxy.a[axis].sp = (wk->mvxy.a[axis].sp * ctc->ix) / ctc->pat;
        break;

    case 2:
        wk->mvxy.a[axis].sp = (wk->mvxy.a[axis].sp * ctc->ix) / ctc->pat;
        /* fallthrough */

    case 1:
        wk->mvxy.d[axis].sp = (wk->mvxy.d[axis].sp * ctc->ix) / ctc->pat;
        break;
    }

    return 1;
}

s32 comm_schx(WORK* wk, UNK11* ctc) {
    return scale_speed_on_axis(wk, ctc, 0);
}

s32 comm_schy(WORK* wk, UNK11* ctc) {
    return scale_speed_on_axis(wk, ctc, 1);
}

s32 comm_mvix(WORK* wk, UNK11* ctc) {
    wk->mvxy.index = ctc->koc;
    return 1;
}

/* comm_mpcy and comm_epcy ask the same question - is this Y above, below or
 * equal to the script's value - of two different works. */
static s16 compare_y_to_koc(WORK* target, UNK11* ctc) {
    s16 ans = 0;

    switch (ctc->ix) {
    case 1:
        if (target->xyz[1].disp.pos > ctc->koc) {
            ans = 1;
        }

        break;

    case 2:
        if (target->xyz[1].disp.pos < ctc->koc) {
            ans = 1;
        }

        break;

    default:
        if (target->xyz[1].disp.pos == ctc->koc) {
            ans = 1;
        }

        break;
    }

    return ans;
}

s32 comm_mpcy(WORK* wk, UNK11* ctc) {
    s16 ans = compare_y_to_koc(wk, ctc);

    if (ans == 0) {
        return 1;
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

s32 comm_epcy(WORK* wk, UNK11* ctc) {
    s16 ans = compare_y_to_koc((WORK*)wk->target_adrs, ctc);

    if (ans == 0) {
        return 1;
    }

    return decord_if_jump(wk, ctc, ctc->pat);
}

/* The three relative-set opcodes place one work at an offset from the other,
 * with X mirrored by the script work's facing. dest and src swap between the
 * arms; facing never does. */
static void set_relative_x(WORK* dest, WORK* src, WORK* facing, UNK11* ctc) {
    if (facing->rl_flag) {
        dest->xyz[0].cal = src->xyz[0].cal + (ctc->ix << 8);
    } else {
        dest->xyz[0].cal = src->xyz[0].cal - (ctc->ix << 8);
    }
}

static void set_relative_y(WORK* dest, WORK* src, UNK11* ctc) {
    dest->xyz[1].cal = src->xyz[1].cal + (ctc->pat << 8);
}

s32 comm_rvxy(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;

    switch (ctc->koc) {
    case 0:
        set_relative_x(wk, emwk, wk, ctc);
        set_relative_y(wk, emwk, ctc);
        break;

    case 2:
        set_relative_x(wk, emwk, wk, ctc);
        set_relative_y(wk, emwk, ctc);
        /* fallthrough */

    default:
        set_relative_x(emwk, wk, wk, ctc);
        set_relative_y(emwk, wk, ctc);
        break;
    }

    return 1;
}

s32 comm_rv_x(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;

    switch (ctc->koc) {
    case 0:
        set_relative_x(wk, emwk, wk, ctc);
        break;

    case 2:
        set_relative_x(wk, emwk, wk, ctc);
        /* fallthrough */

    default:
        set_relative_x(emwk, wk, wk, ctc);
        break;
    }

    return 1;
}

s32 comm_rv_y(WORK* wk, UNK11* ctc) {
    WORK* emwk = (WORK*)wk->target_adrs;

    switch (ctc->koc) {
    case 0:
        set_relative_y(wk, emwk, ctc);
        break;

    case 2:
        set_relative_y(wk, emwk, ctc);
        /* fallthrough */

    default:
        set_relative_y(emwk, wk, ctc);
        break;
    }

    return 1;
}

static void apply_koc_sp_op(s32* target, s16 ix, s32 patl) {
    switch (ix) {
    default:
        *target = patl;
        break;

    case 1:
        *target &= patl;
        break;

    case 2:
        *target |= patl;
        break;

    case 3:
        *target += patl;
        break;

    case 4:
        *target -= patl;
        break;

    case 5:
        *target *= patl;
        break;

    case 6:
        *target /= patl;
        break;
    }
}

s32 comm_sstx(WORK* wk, UNK11* ctc) {
    SST sstx;

    sstx.patl = 0;
    sstx.pats.h = ctc->pat;
    sstx.patl >>= 8;

    switch (ctc->koc) {
    case 0:
        apply_koc_sp_op(&wk->mvxy.a[0].sp, ctc->ix, sstx.patl);
        break;

    case 2:
        apply_koc_sp_op(&wk->mvxy.a[0].sp, ctc->ix, sstx.patl);

        /* fallthrough */

    case 1:
        apply_koc_sp_op(&wk->mvxy.d[0].sp, ctc->ix, sstx.patl);
        break;

    default:
        wk->mvxy.kop[0] = ctc->pat;
        break;
    }

    return 1;
}

/* The seven ways a script command can combine a value into a speed field.
 * comm_ssty wrote this switch out three times, identical apart from which field
 * it targets, so the field is the parameter - passed by address, which is the
 * single difference between the three copies. Case labels are unchanged. */
static void apply_ssty_op(s32* target, s16 ix, s32 patl) {
    switch (ix) {
    default:
        *target = patl;
        break;

    case 1:
        *target &= patl;
        break;

    case 2:
        *target |= patl;
        break;

    case 3:
        *target += patl;
        break;

    case 4:
        *target -= patl;
        break;

    case 5:
        *target *= patl;
        break;

    case 6:
        *target /= patl;
        break;
    }
}

s32 comm_ssty(WORK* wk, UNK11* ctc) {
    SST ssty;

    ssty.patl = 0;
    ssty.pats.h = ctc->pat;
    ssty.patl >>= 8;

    switch (ctc->koc) {
    case 0:
        apply_ssty_op(&wk->mvxy.a[1].sp, ctc->ix, ssty.patl);
        break;

    case 2:
        apply_ssty_op(&wk->mvxy.a[1].sp, ctc->ix, ssty.patl);
        /* fallthrough */

    case 1:
        apply_ssty_op(&wk->mvxy.d[1].sp, ctc->ix, ssty.patl);
        break;

    default:
        wk->mvxy.kop[1] = ctc->pat;
        break;
    }

    return 1;
}
