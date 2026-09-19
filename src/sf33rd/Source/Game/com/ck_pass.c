/**
 * @file ck_pass.c
 * Passive AI logic
 * The per-character area dispatchers, and the unit checks they run.
 */

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/ck_pass_internal.h"

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

static s32 attacks_in_area_6(PLW* wk, WORK* em) {
    if (Check_PL_Unit_C(wk)) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){12, 32, 3, 0})) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){7, 32, 5, 0})) {
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

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){12, 32, 3, 0})) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){7, 32, 5, 0})) {
        return 1;
    }

    if (Check_Special_Technique(wk, em, &(SP_Tech_Args){15, 0, 33, 1, -1})) {
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

    if (Check_VS_Squat(wk, em, &(VS_Squat_Args){29, 33, 32})) {
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

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){7, 32, 3, 0})) {
        return 1;
    }

    return 0;
}

static s32 hugo_attacks_in_area_with_unit(PLW* wk, WORK* em, s32 (*unit_check)(PLW*)) {
    if (unit_check(wk)) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){7, 32, 3, 0})) {
        return 1;
    }

    if (Check_Special_Technique(wk, em, &(SP_Tech_Args){15, 0, 33, 1, -1})) {
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

static void ken_vs_from_area_6(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 6:
        if (options_in_area_6(wk, em)) {
            break;
        }

        if (Check_VS_Jump(wk, (PLW*)em, 64)) {
            break;
        }

        Check_Personal_Action(wk, em);

        break;

    default:
        if (ken_options_in_area_d(wk, em)) {
            break;
        }

        Check_Personal_Action(wk, em);
        break;
    }
}

static void ken_vs_from_area_5(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 5:
        if (options_in_area_5(wk, em)) {
            break;
        }

        if (Check_VS_Jump(wk, (PLW*)em, 32)) {
            break;
        }

        Check_Personal_Action(wk, em);
        break;

    default:
        ken_vs_from_area_6(wk, em);
        break;
    }
}

static void ken_vs_from_area_4(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 4:
        if (options_in_area_4(wk, em)) {
            break;
        }

        if (Check_VS_Jump(wk, (PLW*)em, 16)) {
            break;
        }

        Check_Personal_Action(wk, em);
        break;

    default:
        ken_vs_from_area_5(wk, em);
        break;
    }
}

void KEN_vs(PLW* wk) {
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
        ken_vs_from_area_4(wk, em);
        break;
    }
}

static void hugo_vs_from_area_6(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 6:
        if (hugo_options_in_area_6(wk, em)) {
            break;
        }

        Check_VS_Jump(wk, (PLW*)em, 64);
        break;

    default:
        if (hugo_options_in_area_d(wk, em)) {
            break;
        }
    }
}

static void hugo_vs_from_area_5(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 5:
        if (hugo_options_in_area_5(wk, em)) {
            break;
        }

        Check_VS_Jump(wk, (PLW*)em, 32);
        break;

    default:
        hugo_vs_from_area_6(wk, em);
        break;
    }
}

static void hugo_vs_from_area_4(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 4:
        if (hugo_options_in_area_4(wk, em)) {
            break;
        }

        Check_VS_Jump(wk, (PLW*)em, 16);
        break;

    default:
        hugo_vs_from_area_5(wk, em);
        break;
    }
}

void HUGO_vs(PLW* wk) {
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
        hugo_vs_from_area_4(wk, em);
        break;
    }
}

static void gill_vs_from_area_6(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 6:
        if (options_in_area_6(wk, em)) {
            break;
        }

        Check_VS_Jump(wk, (PLW*)em, 64);
        break;

    default:
        if (gill_options_in_area_d(wk, em)) {
            break;
        }

        Check_VS_Squat(wk, em, &(VS_Squat_Args){7, 33, 32});
        break;
    }
}

static void gill_vs_from_area_5(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 5:
        if (options_in_area_5(wk, em)) {
            break;
        }

        Check_VS_Jump(wk, (PLW*)em, 32);
        break;

    default:
        gill_vs_from_area_6(wk, em);
        break;
    }
}

static void gill_vs_from_area_4(PLW* wk, WORK* em) {
    switch (Passive_Mode + Area_Number[wk->wu.id]) {
    case 4:
        if (options_in_area_4(wk, em)) {
            break;
        }

        Check_VS_Jump(wk, (PLW*)em, 16);
        break;

    default:
        gill_vs_from_area_5(wk, em);
        break;
    }
}

void GILL_vs(PLW* wk) {
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
        gill_vs_from_area_4(wk, em);
        break;
    }
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
