/**
 * @file ck_pass.c
 * Passive AI logic
 * The per-character area dispatchers, and the unit checks they run.
 */

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "common.h"
#include "sf33rd/Source/Game/com/ck_pass_internal.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static s32 Check_PL_Unit_AS(PLW* wk);
static s32 Check_PL_Unit_A(PLW* wk);
static s32 Check_PL_Unit_BS(PLW* wk);
static s32 Check_PL_Unit_B(PLW* wk);
static s32 Check_PL_Unit_CS(PLW* wk);
static s32 Check_PL_Unit_C(PLW* wk);
static s32 Check_PL_Unit_DS(PLW* wk);
static s32 Check_PL_Unit_D(PLW* wk);

s8 PASSIVE_X;

s32 Ck_Passive_Term(PLW* wk) {
    PASSIVE_X = 0;
    Passive_jmp_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
    return PASSIVE_X;
}

static s32 ken_attacks_in_area_d(PLW* wk, WORK* em) {
    if (Check_PL_Unit_D(wk)) {
        return 1;
    }

    return 0;
}

static s32 defends_in_area_d(PLW* wk, WORK* em) {
    if (Check_PL_Unit_DS(wk)) {
        return 1;
    }

    if (Check_Stand(wk, em, 4105)) {
        return 1;
    }

    return 0;
}

static s32 ken_options_in_area_d(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return ken_attacks_in_area_d(wk, em);
    }

    return defends_in_area_d(wk, em);
}

static s32 attacks_in_area_d(PLW* wk, WORK* em) {
    Check_PL_Unit_D(wk);
    return 1;

    return 0;
}

static s32 hugo_defends_in_area_d(PLW* wk, WORK* em) {
    if (Check_PL_Unit_DS(wk)) {
        return 1;
    }

    Check_Stand(wk, em, 4105);
    return 1;

    return 0;
}

static s32 hugo_options_in_area_d(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return attacks_in_area_d(wk, em);
    }

    return hugo_defends_in_area_d(wk, em);
}

static s32 gill_options_in_area_d(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return attacks_in_area_d(wk, em);
    }

    return defends_in_area_d(wk, em);
}

/* The two limited attacks both the area-6 list and the unit-checked list try,
 * in that order. */
static s32 limited_attacks_in_area(PLW* wk, WORK* em) {
    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args) { 12, 32, 3, 0 })) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args) { 7, 32, 5, 0 })) {
        return 1;
    }

    return 0;
}

static s32 attacks_in_area_6(PLW* wk, WORK* em) {
    if (Check_PL_Unit_C(wk)) {
        return 1;
    }

    if (limited_attacks_in_area(wk, em)) {
        return 1;
    }

    return 0;
}

static s32 defends_in_area_6(PLW* wk, WORK* em) {
    if (Check_PL_Unit_CS(wk)) {
        return 1;
    }

    if (Check_After_Attack(wk, em, 28)) {
        return 1;
    }

    if (Check_Stand(wk, em, 4105)) {
        return 1;
    }

    return 0;
}

static s32 attacks_in_area_with_unit(PLW* wk, WORK* em, s32 (*unit_check)(PLW*)) {
    if (unit_check(wk)) {
        return 1;
    }

    if (limited_attacks_in_area(wk, em)) {
        return 1;
    }

    if (Check_Special_Technique(wk, em, &(SP_Tech_Args) { 15, 0, 33, 1, -1 })) {
        return 1;
    }

    return 0;
}

static s32 attacks_in_area_5(PLW* wk, WORK* em) {
    return attacks_in_area_with_unit(wk, em, Check_PL_Unit_B);
}

static s32 defends_in_area_with_unit(PLW* wk, WORK* em, s32 (*unit_check)(PLW*)) {
    if (unit_check(wk)) {
        return 1;
    }

    if (Check_After_Attack(wk, em, 28)) {
        return 1;
    }

    if (Check_VS_Squat(wk, em, &(VS_Squat_Args) { 29, 33, 32 })) {
        return 1;
    }

    if (Check_Stand(wk, em, 4105)) {
        return 1;
    }

    return 0;
}

static s32 defends_in_area_5(PLW* wk, WORK* em) {
    return defends_in_area_with_unit(wk, em, Check_PL_Unit_BS);
}

static s32 attacks_in_area_4(PLW* wk, WORK* em) {
    return attacks_in_area_with_unit(wk, em, Check_PL_Unit_A);
}

static s32 defends_in_area_4(PLW* wk, WORK* em) {
    return defends_in_area_with_unit(wk, em, Check_PL_Unit_AS);
}

static s32 hugo_attacks_in_area_6(PLW* wk, WORK* em) {
    if (Check_PL_Unit_C(wk)) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args) { 7, 32, 3, 0 })) {
        return 1;
    }

    return 0;
}

static s32 hugo_attacks_in_area_with_unit(PLW* wk, WORK* em, s32 (*unit_check)(PLW*)) {
    if (unit_check(wk)) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args) { 7, 32, 3, 0 })) {
        return 1;
    }

    if (Check_Special_Technique(wk, em, &(SP_Tech_Args) { 15, 0, 33, 1, -1 })) {
        return 1;
    }

    return 0;
}

static s32 hugo_attacks_in_area_5(PLW* wk, WORK* em) {
    return hugo_attacks_in_area_with_unit(wk, em, Check_PL_Unit_B);
}

static s32 hugo_attacks_in_area_4(PLW* wk, WORK* em) {
    return hugo_attacks_in_area_with_unit(wk, em, Check_PL_Unit_A);
}

static s32 hugo_options_in_area_4(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return hugo_attacks_in_area_4(wk, em);
    }

    return defends_in_area_4(wk, em);
}

static s32 hugo_options_in_area_5(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return hugo_attacks_in_area_5(wk, em);
    }

    return defends_in_area_5(wk, em);
}

static s32 hugo_options_in_area_6(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return hugo_attacks_in_area_6(wk, em);
    }

    return defends_in_area_6(wk, em);
}

static s32 options_in_area_4(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return attacks_in_area_4(wk, em);
    }

    return defends_in_area_4(wk, em);
}

static s32 options_in_area_5(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return attacks_in_area_5(wk, em);
    }

    return defends_in_area_5(wk, em);
}

static s32 options_in_area_6(PLW* wk, WORK* em) {
    if (Attack_Flag[wk->wu.id]) {
        return attacks_in_area_6(wk, em);
    }

    return defends_in_area_6(wk, em);
}

/* What a chain step does after a jump that did not take, and what the last step
 * does beyond its own options. Ken runs a personal action in both places; Hugo
 * does neither, and the empty function is how that is written. */
static void vs_nothing_after(PLW* wk, WORK* em) {}

static void vs_personal_action(PLW* wk, WORK* em) {
    Check_Personal_Action(wk, em);
}

static void vs_squat_beyond_area(PLW* wk, WORK* em) {
    Check_VS_Squat(wk, em, &(VS_Squat_Args) { 7, 33, 32 });
}

/* One step of a character's area chain: the options tried while the fighter is
 * in this area, what follows a jump that did not take, and where the chain hands
 * on when the fighter is elsewhere. */
typedef struct {
    s32 (*options_in_area)(PLW* wk, WORK* em);
    void (*after_jump)(PLW* wk, WORK* em);
    void (*next_area)(PLW* wk, WORK* em);
} VsAreaStep;

/* The last step of a chain. It has nowhere to hand on to, so instead it tries
 * the character's out-of-area options and then whatever it does beyond them. */
typedef struct {
    s32 (*options_in_area)(PLW* wk, WORK* em);
    void (*after_jump)(PLW* wk, WORK* em);
    s32 (*options_beyond_area)(PLW* wk, WORK* em);
    void (*beyond_area)(PLW* wk, WORK* em);
} VsLastAreaStep;

/* Areas 4, 5 and 6 are the same step at three reaches. The reach goes with the
 * area number, because that number is a case label and cannot be a parameter,
 * so there is one helper per area rather than one for all three.
 *
 * Where a character discarded the jump's result and fell straight to the break,
 * the test is written out and the action after it is empty; both paths reach the
 * same place. */
static void vs_from_area_4(PLW* wk, WORK* em, const VsAreaStep* step) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 4:
        if (step->options_in_area(wk, em)) {
            break;
        }

        if (Check_VS_Jump(wk, (PLW*)em, 16)) {
            break;
        }

        step->after_jump(wk, em);
        break;

    default:
        step->next_area(wk, em);
        break;
    }
}

static void vs_from_area_5(PLW* wk, WORK* em, const VsAreaStep* step) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 5:
        if (step->options_in_area(wk, em)) {
            break;
        }

        if (Check_VS_Jump(wk, (PLW*)em, 32)) {
            break;
        }

        step->after_jump(wk, em);
        break;

    default:
        step->next_area(wk, em);
        break;
    }
}

static void vs_from_area_6(PLW* wk, WORK* em, const VsLastAreaStep* step) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 6:
        if (step->options_in_area(wk, em)) {
            break;
        }

        if (Check_VS_Jump(wk, (PLW*)em, 64)) {
            break;
        }

        step->after_jump(wk, em);
        break;

    default:
        if (step->options_beyond_area(wk, em)) {
            break;
        }

        step->beyond_area(wk, em);
        break;
    }
}

static void ken_vs_from_area_6(PLW* wk, WORK* em) {
    vs_from_area_6(
        wk, em, &(VsLastAreaStep) { options_in_area_6, vs_personal_action, ken_options_in_area_d, vs_personal_action }
    );
}

static void ken_vs_from_area_5(PLW* wk, WORK* em) {
    vs_from_area_5(wk, em, &(VsAreaStep) { options_in_area_5, vs_personal_action, ken_vs_from_area_6 });
}

static void ken_vs_from_area_4(PLW* wk, WORK* em) {
    vs_from_area_4(wk, em, &(VsAreaStep) { options_in_area_4, vs_personal_action, ken_vs_from_area_5 });
}

/* KEN_vs, HUGO_vs and GILL_vs are the same option list: dash in each of the
 * four near areas, and hand anything further out to the character's own area-4
 * handler. Only that handler differs, so it comes in as an action - all three
 * already have the same signature. */
static void dash_in_near_areas(PLW* wk, void (*from_area_4)(PLW* wk, WORK* em)) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 0:
        Check_Dash(wk, em, 1);
        break;

    case 1:
        Check_Dash(wk, em, 1);
        break;

    case 2:
        Check_Dash(wk, em, 1);
        break;

    case 3:
        Check_Dash(wk, em, 1);
        break;

    default:
        from_area_4(wk, em);
        break;
    }
}

void KEN_vs(PLW* wk) {
    dash_in_near_areas(wk, ken_vs_from_area_4);
}

static void hugo_vs_from_area_6(PLW* wk, WORK* em) {
    vs_from_area_6(
        wk, em, &(VsLastAreaStep) { hugo_options_in_area_6, vs_nothing_after, hugo_options_in_area_d, vs_nothing_after }
    );
}

static void hugo_vs_from_area_5(PLW* wk, WORK* em) {
    vs_from_area_5(wk, em, &(VsAreaStep) { hugo_options_in_area_5, vs_nothing_after, hugo_vs_from_area_6 });
}

static void hugo_vs_from_area_4(PLW* wk, WORK* em) {
    vs_from_area_4(wk, em, &(VsAreaStep) { hugo_options_in_area_4, vs_nothing_after, hugo_vs_from_area_5 });
}

void HUGO_vs(PLW* wk) {
    dash_in_near_areas(wk, hugo_vs_from_area_4);
}

static void gill_vs_from_area_6(PLW* wk, WORK* em) {
    vs_from_area_6(
        wk, em, &(VsLastAreaStep) { options_in_area_6, vs_nothing_after, gill_options_in_area_d, vs_squat_beyond_area }
    );
}

static void gill_vs_from_area_5(PLW* wk, WORK* em) {
    vs_from_area_5(wk, em, &(VsAreaStep) { options_in_area_5, vs_nothing_after, gill_vs_from_area_6 });
}

static void gill_vs_from_area_4(PLW* wk, WORK* em) {
    vs_from_area_4(wk, em, &(VsAreaStep) { options_in_area_4, vs_nothing_after, gill_vs_from_area_5 });
}

void GILL_vs(PLW* wk) {
    dash_in_near_areas(wk, gill_vs_from_area_4);
}

static s32 Check_PL_Unit_AS(PLW* wk) {
    return Passive_AS_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

static s32 Check_PL_Unit_A(PLW* wk) {
    return Passive_A_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

static s32 Check_PL_Unit_BS(PLW* wk) {
    return Passive_BS_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

static s32 Check_PL_Unit_B(PLW* wk) {
    return Passive_B_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

static s32 Check_PL_Unit_CS(PLW* wk) {
    return Passive_CS_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

static s32 Check_PL_Unit_C(PLW* wk) {
    return Passive_C_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

static s32 Check_PL_Unit_DS(PLW* wk) {
    return Passive_DS_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

static s32 Check_PL_Unit_D(PLW* wk) {
    return Passive_D_tbl[((PLW*)wk->wu.target_adrs)->player_number](wk);
}

void (*const Passive_jmp_tbl[21])() = { GILL_vs, KEN_vs, KEN_vs, KEN_vs, KEN_vs, KEN_vs, HUGO_vs,
                                        KEN_vs,  KEN_vs, KEN_vs, KEN_vs, KEN_vs, KEN_vs, KEN_vs,
                                        KEN_vs,  KEN_vs, KEN_vs, KEN_vs, KEN_vs, KEN_vs, KEN_vs };
