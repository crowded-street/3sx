/**
 * @file ck_pass_units_c.c
 * Passive AI logic
 * Per-character option lists for area C, attacking and defending.
 */

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/ck_pass_internal.h"

s32 VS_GILL_CS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){24, 8, 65, -1, -1}, &(SP_Tech_Args){24, 8, 64, -1, -1});
}

s32 VS_ALEX_CS(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){23, 24, 23, -1, -1},
        &(SP_Tech_Args){14, 24, 22, -1, -1},
        &(SP_Tech_Args){14, 56, 37, -1, -1}
    );
}

s32 VS_RYU_CS(PLW* wk) {
    if (check_three_specials(
            wk,
            &(SP_Tech_Args){23, 8, 1, -1, 0},
            &(SP_Tech_Args){11, 8, 0, 1, -1},
            &(SP_Tech_Args){23, 48, 3, -1, -1}
        )) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){16, 32, 4, -1, -1}, &(SP_Tech_Args){11, 32, 5, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_YUN_CS(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){23, 8, 29, -1, -1},
        &(SP_Tech_Args){23, 32, 36, -1, -1},
        &(SP_Tech_Args){14, 24, 74, -1, -1}
    );
}

s32 VS_DUDLEY_CS(PLW* wk) {
    if (check_three_specials(
            wk,
            &(SP_Tech_Args){23, 8, 1, -1, 0},
            &(SP_Tech_Args){24, 8, 2, -1, 0},
            &(SP_Tech_Args){14, 32, 13, -1, -1}
        )) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){23, 32, 11, -1, -1}, &(SP_Tech_Args){14, 32, 12, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_NECRO_CS(PLW* wk) {
    return 0;
}

s32 VS_HUGO_CS(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 61, -1, -1}, &(SP_Tech_Args){24, 8, 61, -1, -1})) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){24, 32, 5, 32767})) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){24, 32, 4, 32767})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){19, 24, 58, -1, -1}, &(SP_Tech_Args){24, 24, 61, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_IBUKI_CS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){23, 32, 25, -1, -1});
}

s32 VS_ELENA_CS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){23, 8, 17, -1, 0}, &(SP_Tech_Args){23, 32, 14, -1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){14, 32, 15, -1, -1}, &(SP_Tech_Args){2, 48, 16, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_ORO_CS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){23, 8, 39, -1, 0});
}

s32 VS_KEN_CS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){23, 8, 1, -1, 0})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){23, 32, 8, -1, -1}, &(SP_Tech_Args){23, 32, 6, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_SEAN_CS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){23, 32, 8, -1, -1});
}

s32 VS_URIEN_CS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){24, 8, 65, -1, -1}, &(SP_Tech_Args){24, 8, 64, -1, -1});
}

s32 VS_GOUKI_CS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){23, 8, 1, -1, 0}, &(SP_Tech_Args){11, 8, 0, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){16, 32, 4, -1, -1}, &(SP_Tech_Args){23, 32, 69, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_CHUN_LI_CS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){11, 8, 78, 1, -1}, &(SP_Tech_Args){24, 8, 76, -1, -1});
}

s32 VS_MAKOTO_CS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){8, 8, 92, -1, -1}, &(SP_Tech_Args){14, 24, 95, -1, -1});
}

s32 VS_Q_CS(PLW* wk) {
    return 0;
}

s32 VS_NO12_CS(PLW* wk) {
    return 0;
}

s32 VS_REMY_CS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){23, 8, 101, -1, 0}, &(SP_Tech_Args){11, 8, 0, 1, -1});
}

s32 VS_GILL_C(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){24, 8, 64, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){15, 8, 63, 1, -1}, &(SP_Tech_Args){24, 8, 61, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_ALEX_C(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_two_specials(wk, &(SP_Tech_Args){24, 24, 115, 1, 0}, &(SP_Tech_Args){15, 8, 30, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 73, 1, -1}, &(SP_Tech_Args){15, 40, 72, 1, -1})) {
        return 1;
    }

    if (Check_F_Cross_Chop(wk, em, 15)) {
        return 1;
    }

    return 0;
}

s32 VS_RYU_C(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){24, 8, 2, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){16, 32, 4, 1, -1}, &(SP_Tech_Args){11, 32, 5, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_YUN_C(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){8, 8, 31, -1, -1}, &(SP_Tech_Args){20, 8, 52, 1, -1});
}

s32 VS_DUDLEY_C(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){24, 8, 2, 1, 0},
        &(SP_Tech_Args){18, 8, 75, 1, 0},
        &(SP_Tech_Args){14, 32, 12, -1, -1}
    );
}

s32 VS_NECRO_C(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 38, -1, -1}, &(SP_Tech_Args){15, 8, 40, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){17, 24, 24, -1, -1}, &(SP_Tech_Args){11, 32, 41, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_HUGO_C(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_three_specials(
            wk,
            &(SP_Tech_Args){24, 8, 2, 1, -1},
            &(SP_Tech_Args){24, 8, 61, -1, -1},
            &(SP_Tech_Args){19, 24, 58, 1, -1}
        )) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){24, 32, 5, 32767})) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){24, 32, 4, 32767})) {
        return 1;
    }

    if (check_three_specials(
            wk,
            &(SP_Tech_Args){19, 24, 62, 1, -1},
            &(SP_Tech_Args){19, 56, 55, 1, -1},
            &(SP_Tech_Args){24, 24, 61, 1, -1}
        )) {
        return 1;
    }

    return 0;
}

s32 VS_IBUKI_C(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){8, 24, 26, 1, -1});
}

s32 VS_ELENA_C(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){8, 8, 18, 1, -1},
        &(SP_Tech_Args){15, 8, 19, 1, -1},
        &(SP_Tech_Args){2, 48, 16, -1, -1}
    );
}

s32 VS_ORO_C(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){15, 8, 44, 1, -1});
}

s32 VS_KEN_C(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){24, 8, 2, 1, -1});
}

s32 VS_SEAN_C(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_three_specials(
            wk,
            &(SP_Tech_Args){15, 8, 32, 1, -1},
            &(SP_Tech_Args){24, 8, 21, 1, -1},
            &(SP_Tech_Args){24, 8, 20, 1, -1}
        )) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){24, 0, 5, 32767})) {
        return 1;
    }

    return 0;
}

s32 VS_URIEN_C(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){8, 8, 65, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 64, 1, -1}, &(SP_Tech_Args){15, 8, 63, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_GOUKI_C(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){11, 8, 0, 1, -1},
        &(SP_Tech_Args){24, 8, 2, 1, -1},
        &(SP_Tech_Args){16, 32, 4, 1, -1}
    );
}

s32 VS_CHUN_LI_C(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 76, 1, -1}, &(SP_Tech_Args){15, 8, 114, 1, 0})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){21, 8, 77, 1, -1}, &(SP_Tech_Args){11, 8, 78, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_MAKOTO_C(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){8, 8, 92, 1, -1}, &(SP_Tech_Args){25, 24, 95, 1, -1});
}

s32 VS_Q_C(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){24, 8, 84, 1, -1}, &(SP_Tech_Args){21, 8, 87, 1, -1});
}

s32 VS_NO12_C(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){22, 8, 107, 1, -1}, &(SP_Tech_Args){17, 8, 105, 1, -1});
}

s32 VS_REMY_C(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){11, 8, 0, 1, -1});
}

s32 (*const Passive_CS_tbl[20])(PLW* wk) = { VS_GILL_CS,    VS_ALEX_CS,   VS_RYU_CS,   VS_YUN_CS,   VS_DUDLEY_CS,
                                             VS_NECRO_CS,   VS_HUGO_CS,   VS_IBUKI_CS, VS_ELENA_CS, VS_ORO_CS,
                                             VS_YUN_CS,     VS_KEN_CS,    VS_SEAN_CS,  VS_URIEN_CS, VS_GOUKI_CS,
                                             VS_CHUN_LI_CS, VS_MAKOTO_CS, VS_Q_CS,     VS_NO12_CS,  VS_REMY_CS };

s32 (*const Passive_C_tbl[20])(PLW* wk) = { VS_GILL_C,    VS_ALEX_C,   VS_RYU_C,   VS_YUN_C,   VS_DUDLEY_C,
                                            VS_NECRO_C,   VS_HUGO_C,   VS_IBUKI_C, VS_ELENA_C, VS_ORO_C,
                                            VS_YUN_C,     VS_KEN_C,    VS_SEAN_C,  VS_URIEN_C, VS_GOUKI_C,
                                            VS_CHUN_LI_C, VS_MAKOTO_C, VS_Q_C,     VS_NO12_C,  VS_REMY_C };
