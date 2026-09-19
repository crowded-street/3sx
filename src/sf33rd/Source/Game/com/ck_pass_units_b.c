/**
 * @file ck_pass_units_b.c
 * Passive AI logic
 * Per-character option lists for area B, attacking and defending.
 */

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/ck_pass_internal.h"

s32 VS_GILL_BS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){24, 8, 61, -1, -1}, &(SP_Tech_Args){24, 8, 64, -1, -1});
}

s32 VS_ALEX_BS(PLW* wk) {
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

s32 VS_RYU_BS(PLW* wk) {
    if (check_three_specials(
            wk,
            &(SP_Tech_Args){23, 8, 1, -1, 0},
            &(SP_Tech_Args){11, 8, 0, 1, -1},
            &(SP_Tech_Args){24, 8, 2, -1, -1}
        )) {
        return 1;
    }

    if (check_three_specials(
            wk,
            &(SP_Tech_Args){23, 48, 3, -1, -1},
            &(SP_Tech_Args){16, 32, 4, -1, -1},
            &(SP_Tech_Args){11, 32, 5, 1, -1}
        )) {
        return 1;
    }

    return 0;
}

s32 VS_YUN_BS(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){23, 8, 29, -1, -1},
        &(SP_Tech_Args){23, 32, 36, -1, -1},
        &(SP_Tech_Args){14, 24, 74, -1, -1}
    );
}

s32 VS_DUDLEY_BS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){23, 8, 1, -1, 0}, &(SP_Tech_Args){14, 32, 13, -1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){23, 32, 11, -1, -1}, &(SP_Tech_Args){14, 32, 12, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_NECRO_BS(PLW* wk) {
    return 0;
}

s32 VS_HUGO_BS(PLW* wk) {
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

s32 VS_IBUKI_BS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){23, 8, 28, -1, 0}, &(SP_Tech_Args){23, 32, 25, -1, -1});
}

s32 VS_ELENA_BS(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){23, 8, 17, -1, -1}, &(SP_Tech_Args){23, 32, 14, -1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){14, 32, 15, -1, -1}, &(SP_Tech_Args){2, 48, 16, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_ORO_BS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){23, 8, 39, -1, 0});
}

s32 VS_KEN_BS(PLW* wk) {
    if (check_three_specials(
            wk,
            &(SP_Tech_Args){11, 8, 0, 1, -1},
            &(SP_Tech_Args){23, 8, 1, -1, 0},
            &(SP_Tech_Args){24, 8, 2, -1, -1}
        )) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){23, 32, 8, -1, -1}, &(SP_Tech_Args){23, 32, 6, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_SEAN_BS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){23, 32, 8, -1, -1});
}

s32 VS_URIEN_BS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){24, 8, 65, -1, -1}, &(SP_Tech_Args){24, 8, 64, -1, -1});
}

s32 VS_GOUKI_BS(PLW* wk) {
    if (check_three_specials(
            wk,
            &(SP_Tech_Args){23, 8, 1, -1, 0},
            &(SP_Tech_Args){11, 8, 0, 1, -1},
            &(SP_Tech_Args){24, 8, 2, -1, -1}
        )) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){16, 32, 4, -1, -1}, &(SP_Tech_Args){23, 32, 69, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_CHUN_LI_BS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){11, 8, 78, 1, -1}, &(SP_Tech_Args){24, 8, 76, -1, 0});
}

s32 VS_MAKOTO_BS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){8, 8, 92, -1, -1}, &(SP_Tech_Args){14, 24, 95, -1, -1});
}

s32 VS_Q_BS(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){25, 24, 88, 1, -1});
}

s32 VS_NO12_BS(PLW* wk) {
    return 0;
}

s32 VS_REMY_BS(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){23, 8, 101, -1, 0}, &(SP_Tech_Args){11, 8, 0, 1, -1});
}

s32 VS_GILL_B(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){24, 8, 64, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){15, 8, 63, 1, -1}, &(SP_Tech_Args){24, 8, 61, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_ALEX_B(PLW* wk) {
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

s32 VS_RYU_B(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){24, 8, 2, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){16, 32, 4, 1, -1}, &(SP_Tech_Args){11, 32, 5, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_YUN_B(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){8, 8, 31, -1, -1},
        &(SP_Tech_Args){20, 8, 52, 1, -1},
        &(SP_Tech_Args){25, 24, 74, 1, -1}
    );
}

s32 VS_DUDLEY_B(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){24, 8, 2, 1, -1},
        &(SP_Tech_Args){18, 8, 75, 1, 0},
        &(SP_Tech_Args){14, 32, 12, -1, -1}
    );
}

s32 VS_NECRO_B(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 38, -1, -1}, &(SP_Tech_Args){15, 8, 40, -1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){17, 24, 24, 1, -1}, &(SP_Tech_Args){11, 32, 41, -1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_HUGO_B(PLW* wk) {
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

s32 VS_IBUKI_B(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){24, 24, 26, 1, -1});
}

s32 VS_ELENA_B(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){8, 8, 18, -1, -1},
        &(SP_Tech_Args){15, 8, 19, 1, -1},
        &(SP_Tech_Args){2, 48, 16, -1, -1}
    );
}

s32 VS_ORO_B(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){15, 8, 44, 1, -1});
}

s32 VS_KEN_B(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){24, 8, 2, 1, -1});
}

s32 VS_SEAN_B(PLW* wk) {
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

    if (Check_Rolling(wk, em)) {
        return 1;
    }

    return 0;
}

s32 VS_URIEN_B(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 0, 1, -1}, &(SP_Tech_Args){8, 8, 65, 1, -1})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){24, 8, 64, 1, -1}, &(SP_Tech_Args){15, 8, 63, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_GOUKI_B(PLW* wk) {
    if (check_three_specials(
            wk,
            &(SP_Tech_Args){11, 8, 0, 1, -1},
            &(SP_Tech_Args){24, 8, 2, 1, -1},
            &(SP_Tech_Args){16, 32, 4, 1, -1}
        )) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){16, 32, 5, 1, -1}, &(SP_Tech_Args){13, 64, 47, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_CHUN_LI_B(PLW* wk) {
    if (check_two_specials(wk, &(SP_Tech_Args){11, 8, 78, 1, -1}, &(SP_Tech_Args){24, 8, 76, 1, 0})) {
        return 1;
    }

    if (check_two_specials(wk, &(SP_Tech_Args){15, 8, 114, 1, 0}, &(SP_Tech_Args){21, 8, 77, 1, -1})) {
        return 1;
    }

    return 0;
}

s32 VS_MAKOTO_B(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){8, 8, 92, 1, -1}, &(SP_Tech_Args){25, 24, 95, 1, -1});
}

s32 VS_Q_B(PLW* wk) {
    return check_three_specials(
        wk,
        &(SP_Tech_Args){24, 8, 84, 1, -1},
        &(SP_Tech_Args){21, 8, 87, 1, -1},
        &(SP_Tech_Args){25, 24, 88, 1, -1}
    );
}

s32 VS_NO12_B(PLW* wk) {
    return check_two_specials(wk, &(SP_Tech_Args){22, 8, 107, 1, -1}, &(SP_Tech_Args){17, 8, 105, 1, -1});
}

s32 VS_REMY_B(PLW* wk) {
    return check_one_special(wk, &(SP_Tech_Args){11, 8, 0, 1, -1});
}

s32 (*const Passive_BS_tbl[20])(PLW* wk) = { VS_GILL_BS,    VS_ALEX_BS,   VS_RYU_BS,   VS_YUN_BS,   VS_DUDLEY_BS,
                                             VS_NECRO_BS,   VS_HUGO_BS,   VS_IBUKI_BS, VS_ELENA_BS, VS_ORO_BS,
                                             VS_YUN_BS,     VS_KEN_BS,    VS_SEAN_BS,  VS_URIEN_BS, VS_GOUKI_BS,
                                             VS_CHUN_LI_BS, VS_MAKOTO_BS, VS_Q_BS,     VS_NO12_BS,  VS_REMY_BS };

s32 (*const Passive_B_tbl[20])(PLW* wk) = { VS_GILL_B,    VS_ALEX_B,   VS_RYU_B,   VS_YUN_B,   VS_DUDLEY_B,
                                            VS_NECRO_B,   VS_HUGO_B,   VS_IBUKI_B, VS_ELENA_B, VS_ORO_B,
                                            VS_YUN_B,     VS_KEN_B,    VS_SEAN_B,  VS_URIEN_B, VS_GOUKI_B,
                                            VS_CHUN_LI_B, VS_MAKOTO_B, VS_Q_B,     VS_NO12_B,  VS_REMY_B };
