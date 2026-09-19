/**
 * @file ck_pass_units_d.c
 * Passive AI logic
 * Per-character option lists for area D, attacking and defending.
 */

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/ck_pass_internal.h"

s32 VS_GILL_DS(PLW* wk) {
    return 0;
}

s32 VS_ALEX_DS(PLW* wk) {
    return 0;
}

s32 VS_RYU_DS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){11, 32, 5, 1, -1});
}

s32 VS_YUN_DS(PLW* wk) {
    return 0;
}

s32 VS_DUDLEY_DS(PLW* wk) {
    return 0;
}

s32 VS_NECRO_DS(PLW* wk) {
    return 0;
}

s32 VS_IBUKI_DS(PLW* wk) {
    return 0;
}

s32 VS_HUGO_DS(PLW* wk) {
    return 0;
}

s32 VS_ELENA_DS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){2, 48, 16, -1, -1});
}

s32 VS_ORO_DS(PLW* wk) {
    return 0;
}

s32 VS_KEN_DS(PLW* wk) {
    return 0;
}

s32 VS_SEAN_DS(PLW* wk) {
    return 0;
}

s32 VS_URIEN_DS(PLW* wk) {
    return 0;
}

s32 VS_GOUKI_DS(PLW* wk) {
    return 0;
}

s32 VS_CHUN_LI_DS(PLW* wk) {
    return 0;
}

s32 VS_MAKOTO_DS(PLW* wk) {
    return 0;
}

s32 VS_Q_DS(PLW* wk) {
    return 0;
}

s32 VS_NO12_DS(PLW* wk) {
    PLW* em = (PLW*)wk->wu.target_adrs;

    if (Check_VS_Jump(wk, em, 32)) {
        VS_Tech[wk->wu.id] = 15;
        return 1;
    }

    return 0;
}

s32 VS_REMY_DS(PLW* wk) {
    return 0;
}

s32 VS_GILL_D(PLW* wk) {
    return 0;
}

s32 VS_ALEX_D(PLW* wk) {
    return 0;
}

s32 VS_RYU_D(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){11, 32, 5, 1, -1});
}

s32 VS_YUN_D(PLW* wk) {
    return 0;
}

s32 VS_DUDLEY_D(PLW* wk) {
    return 0;
}

s32 VS_NECRO_D(PLW* wk) {
    return 0;
}

s32 VS_HUGO_D(PLW* wk) {
    return 0;
}

s32 VS_IBUKI_D(PLW* wk) {
    return 0;
}

s32 VS_ELENA_D(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){2, 48, 16, -1, -1});
}

s32 VS_ORO_D(PLW* wk) {
    return 0;
}

s32 VS_KEN_D(PLW* wk) {
    return 0;
}

s32 VS_SEAN_D(PLW* wk) {
    return 0;
}

s32 VS_URIEN_D(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){12, 8, 63, 1, 12});
}

s32 VS_GOUKI_D(PLW* wk) {
    return 0;
}

s32 VS_CHUN_LI_D(PLW* wk) {
    return 0;
}

s32 VS_MAKOTO_D(PLW* wk) {
    return 0;
}

s32 VS_Q_D(PLW* wk) {
    return 0;
}

s32 VS_NO12_D(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){8, 8, 105, 1, -1}, &(SP_Tech_Args){15, 8, 107, 1, -1});
}

s32 VS_REMY_D(PLW* wk) {
    return 0;
}

s32 (*const Passive_DS_tbl[20])(PLW* wk) = { VS_GILL_DS,    VS_ALEX_DS,   VS_RYU_DS,   VS_YUN_DS,   VS_DUDLEY_DS,
                                             VS_NECRO_DS,   VS_HUGO_DS,   VS_IBUKI_DS, VS_ELENA_DS, VS_ORO_DS,
                                             VS_YUN_DS,     VS_KEN_DS,    VS_SEAN_DS,  VS_URIEN_DS, VS_GOUKI_DS,
                                             VS_CHUN_LI_DS, VS_MAKOTO_DS, VS_Q_DS,     VS_NO12_DS,  VS_REMY_DS };

s32 (*const Passive_D_tbl[20])(PLW* wk) = { VS_GILL_D,    VS_ALEX_D,   VS_RYU_D,   VS_YUN_D,   VS_DUDLEY_D,
                                            VS_NECRO_D,   VS_HUGO_D,   VS_IBUKI_D, VS_ELENA_D, VS_ORO_D,
                                            VS_YUN_D,     VS_KEN_D,    VS_SEAN_D,  VS_URIEN_D, VS_GOUKI_D,
                                            VS_CHUN_LI_D, VS_MAKOTO_D, VS_Q_D,     VS_NO12_D,  VS_REMY_D };
