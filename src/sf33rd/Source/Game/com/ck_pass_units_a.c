/**
 * @file ck_pass_units_a.c
 * Passive AI logic
 * Per-character option lists for area A, attacking and defending.
 */

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/ck_pass_internal.h"

s32 check_one_special(PLW* wk, const SP_Tech_Args* p0) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (Check_Special_Technique(wk, em, p0)) {
        return 1;
    }

    return 0;
}

s32 check_two_specials(PLW* wk, const SP_Tech_Args* p0, const SP_Tech_Args* p1) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (Check_Special_Technique(wk, em, p0)) {
        return 1;
    }

    if (Check_Special_Technique(wk, em, p1)) {
        return 1;
    }

    return 0;
}

s32 check_three_specials(PLW* wk, const SP_Tech_Args* p0, const SP_Tech_Args* p1, const SP_Tech_Args* p2) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (Check_Special_Technique(wk, em, p0)) {
        return 1;
    }

    if (Check_Special_Technique(wk, em, p1)) {
        return 1;
    }

    if (Check_Special_Technique(wk, em, p2)) {
        return 1;
    }

    return 0;
}

s32 VS_GILL_AS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){24, 8, 61, -1, -1}, &(SP_Tech_Args){24, 8, 64, -1, -1});
}

s32 VS_ALEX_AS(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_three_specials(
            wk,
            &(SP_Tech_Args){23, 24, 23, -1, -1},
            &(SP_Tech_Args){14, 24, 22, -1, -1},
            &(SP_Tech_Args){14, 56, 37, -1, -1}
        )) {
        return 1;
    }

    if (Check_Rolling(wk, em)) {
        return 1;
    }

    return 0;
}

s32 VS_RYU_AS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){23, 8, 1, -1, 0}, &(SP_Tech_Args){11, 8, 0, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){11, 32, 4, -1, -1}, &(SP_Tech_Args){11, 32, 5, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_YUN_AS(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){23, 8, 29, -1, -1},
        &(SP_Tech_Args){23, 32, 36, -1, -1},
        &(SP_Tech_Args){14, 24, 74, -1, -1}
    );
}

s32 VS_DUDLEY_AS(PLW* wk) {
    if (check_three_specials(
            wk,
            &(SP_Tech_Args){23, 8, 1, -1, -1},
            &(SP_Tech_Args){24, 8, 2, -1, -1},
            &(SP_Tech_Args){14, 32, 13, -1, -1}
        )) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){23, 32, 11, -1, -1}, &(SP_Tech_Args){14, 32, 12, -1, 0})) {
        return 1;
    }

    return 0;
}

s32 VS_NECRO_AS(PLW* wk) {
    return 0;
}

s32 VS_HUGO_AS(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_one_special(wk, &(SP_Tech_Args){14, 8, 61, -1, -1})) {
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

s32 VS_IBUKI_AS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){23, 8, 28, -1, 0}, &(SP_Tech_Args){23, 32, 25, -1, -1});
}

s32 VS_ELENA_AS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){23, 8, 17, -1, -1}, &(SP_Tech_Args){23, 32, 14, -1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){14, 32, 15, -1, -1}, &(SP_Tech_Args){2, 48, 16, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_ORO_AS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){23, 8, 39, -1, 0});
}

s32 VS_KEN_AS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){23, 8, 1, -1, 0})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){23, 32, 8, -1, -1}, &(SP_Tech_Args){23, 32, 6, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_SEAN_AS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){23, 32, 8, -1, -1});
}

s32 VS_URIEN_AS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){24, 8, 65, -1, -1}, &(SP_Tech_Args){24, 8, 64, -1, -1});
}

s32 VS_GOUKI_AS(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){23, 8, 1, -1, 0},
        &(SP_Tech_Args){11, 8, 0, 1, -1},
        &(SP_Tech_Args){23, 32, 69, -1, -1}
    );
}

s32 VS_CHUN_LI_AS(PLW* wk) {
    return 0;
}

s32 VS_MAKOTO_AS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){14, 24, 95, -1, -1});
}

s32 VS_Q_AS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){25, 24, 88, 1, -1});
}

s32 VS_NO12_AS(PLW* wk) {
    return 0;
}

s32 VS_REMY_AS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){23, 8, 101, -1, 0}, &(SP_Tech_Args){11, 8, 0, 1, -1});
}

s32 VS_GILL_A(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){24, 8, 64, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){15, 8, 63, 1, -1}, &(SP_Tech_Args){24, 8, 61, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_ALEX_A(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_two_specials(wk, &(SP_Tech_Args){25, 24, 22, 1, -1}, &(SP_Tech_Args){24, 24, 115, 1, 0})) {
        return 1;
    }

    if (Check_F_Cross_Chop(wk, em, 15)) {
        return 1;
    }

    if (check_three_specials(
            wk,
            &(SP_Tech_Args){24, 8, 73, 1, -1},
            &(SP_Tech_Args){15, 8, 30, 1, -1},
            &(SP_Tech_Args){15, 40, 72, 1, -1}
        )) {
        return 1;
    }

    return 0;
}

s32 VS_RYU_A(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){23, 48, 3, -1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){16, 32, 4, -1, -1}, &(SP_Tech_Args){11, 32, 5, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_YUN_A(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){20, 8, 52, 1, -1}, &(SP_Tech_Args){25, 24, 74, 1, -1});
}

s32 VS_DUDLEY_A(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){24, 8, 2, -1, -1},
        &(SP_Tech_Args){18, 8, 75, 1, 0},
        &(SP_Tech_Args){14, 32, 12, -1, -1}
    );
}

s32 VS_NECRO_A(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 38, -1, -1}, &(SP_Tech_Args){15, 8, 40, -1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){17, 24, 24, 1, -1}, &(SP_Tech_Args){11, 32, 41, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_HUGO_A(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_one_special(wk, &(SP_Tech_Args){19, 24, 58, 1, -1})) {
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

s32 VS_IBUKI_A(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){24, 24, 26, 1, -1});
}

s32 VS_ELENA_A(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){8, 8, 18, -1, -1},
        &(SP_Tech_Args){2, 48, 16, -1, -1},
        &(SP_Tech_Args){15, 8, 19, -1, -1}
    );
}

s32 VS_ORO_A(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){15, 8, 44, 1, -1});
}

s32 VS_KEN_A(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){11, 8, 0, 1, -1});
}

s32 VS_SEAN_A(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 21, 1, -1}, &(SP_Tech_Args){15, 8, 32, 1, -1})) {
        return 1;
    }

    if (Check_Limited_Attack(wk, em, &(Limited_Attack_Args){24, 0, 5, 32767})) {
        return 1;
    }

    if (Check_Rolling(wk, em)) {
        return 1;
    }

    return 0;
}

s32 VS_URIEN_A(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){8, 8, 65, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 64, 1, -1}, &(SP_Tech_Args){15, 8, 63, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_GOUKI_A(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){11, 8, 0, 1, -1},
        &(SP_Tech_Args){16, 32, 4, 1, -1},
        &(SP_Tech_Args){13, 64, 47, 1, -1}
    );
}

s32 VS_CHUN_LI_A(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){11, 8, 78, 1, -1},
        &(SP_Tech_Args){15, 8, 114, 1, 0},
        &(SP_Tech_Args){21, 8, 77, 1, -1}
    );
}

s32 VS_MAKOTO_A(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){25, 24, 95, 1, -1});
}

s32 VS_Q_A(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){25, 24, 88, 1, -1});
}

s32 VS_NO12_A(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){17, 8, 105, 1, -1}, &(SP_Tech_Args){22, 8, 107, 1, -1});
}

s32 VS_REMY_A(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){11, 8, 0, 1, -1});
}

s32 (*const Passive_AS_tbl[20])(PLW* wk) = { VS_GILL_AS,    VS_ALEX_AS,   VS_RYU_AS,   VS_YUN_AS,   VS_DUDLEY_AS,
                                             VS_NECRO_AS,   VS_HUGO_AS,   VS_IBUKI_AS, VS_ELENA_AS, VS_ORO_AS,
                                             VS_YUN_AS,     VS_KEN_AS,    VS_SEAN_AS,  VS_URIEN_AS, VS_GOUKI_AS,
                                             VS_CHUN_LI_AS, VS_MAKOTO_AS, VS_Q_AS,     VS_NO12_AS,  VS_REMY_AS };

s32 (*const Passive_A_tbl[20])(PLW* wk) = { VS_GILL_A,    VS_ALEX_A,   VS_RYU_A,   VS_YUN_A,   VS_DUDLEY_A,
                                            VS_NECRO_A,   VS_HUGO_A,   VS_IBUKI_A, VS_ELENA_A, VS_ORO_A,
                                            VS_YUN_A,     VS_KEN_A,    VS_SEAN_A,  VS_URIEN_A, VS_GOUKI_A,
                                            VS_CHUN_LI_A, VS_MAKOTO_A, VS_Q_A,     VS_NO12_A,  VS_REMY_A };
