/**
 * @file pass03.c
 * COM Passive: Yun
 */

#include "sf33rd/Source/Game/com/passive/pass03.h"
#include "sf33rd/Source/Game/com/passive/pass03_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive03_Tbl[136])();

void Passive03(PLW* wk) {
    Passive03_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive03_0000(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive03_0001(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Passive03_0002(PLW* wk) {
    pattern_wait_get_up_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 10, -1},
        &(EM_Term_Params){0x7FFF, -1, 1, 1, -1}
    );
}

void Passive03_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive03_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x70, 0x53, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive03_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive03_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x77, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive03_0007(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0xB7, &(Command_Attack_Args){8, 0x20, 9, -1});
}

void Passive03_0008(PLW* wk) {
    active_pattern_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){0xC, 0, 0xB, -1},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive03_0009(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive03_0010(PLW* wk) {
    pattern_em_term_short_range_attack(wk, &(Short_Range_Args){8, 0x40, 6, 0x1D});
}

void Passive03_0011(PLW* wk) {
    active_pattern_hi_jump_attack_term(wk, &(Hi_Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, 0x8400});
}

void Passive03_0012(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F90, 0, 0, 2, 0}, 0x20);
}

void Passive03_0013(PLW* wk) {
    pattern_sa_term_wait_get_up_sa_term(
        wk,
        &(SA_Term_Args){0xFFFF, 0xFFFF, 0x30, 0},
        &(SA_Term_Args){0xFFFF, 0x2F, 0xFFFF, 0},
        0xB
    );
}

void Passive03_0014(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7FB0, -0x7FB0, 0, 1, -1}, 0x200);
}

void Passive03_0015(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive03_0016(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Approach_Walk(wk, 0x2F, 2);
        break;

    case 1:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 2:
        Normal_Attack(wk, 0xB, 0x10);
        break;

    case 3:
        Normal_Attack(wk, 8, 0x20);
        break;

    case 4:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 5:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

static void passive03_0017_from_step_6(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 6:
        Normal_Attack(wk, 0xC, 0x40);
        break;

    case 7:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x20, 9, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0017(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x70, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x81, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 0, -1);
        break;

    case 4:
        Normal_Attack(wk, 0xB, 0x100);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x20);
        break;

    default:
        passive03_0017_from_step_6(wk);
        break;
    }
}

void Passive03_0018(PLW* wk) {
    pattern_approach_walk_wait_get_up_normal_attack_4(wk, 0x39, 0xB);
}

void Passive03_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive03_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive03_0021(PLW* wk) {
    pattern_wait_get_up_command_attack_2(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive03_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack_3(
        wk,
        &(Jump_Term_Args){-0x7FA0, -0x7FD0, 0xB, 0x100, 0, -0x7FA0, -1, 0x40},
        0xB,
        &(Command_Attack_Args){8, 0x20, 9, -1}
    );
}

void Passive03_0023(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack_4(
        wk,
        &(Jump_Term_Args){-0x7FA0, -1, 0xB, 0x8100, 0, -0x7FA0, -1, 0x40},
        &(Command_Attack_Args){8, 0x20, 8, -1}
    );
}

void Passive03_0024(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_4(wk, 0x70, 0x6C, 0x81);
}

void Passive03_0025(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_6(wk, 0x81, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive03_0026(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0x20);
}

void Passive03_0027(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive03_0028(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, 0x400});
}

void Passive03_0029(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x10, 0x10, 0x10);
}

void Passive03_0030(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1}, 0x400);
}

void Passive03_0031(PLW* wk) {
    pattern_walk(wk, 1);
}

void Passive03_0032(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        SA_Term(wk, &(SA_Term_Args){0x2E, 0x2F, 0xFFFF, 0});
        break;

    case 1:
        Normal_Attack(wk, 0xB, 0x22);
        break;

    case 2:
        Normal_Attack(wk, 8, 0x12);
        break;

    case 3:
        Command_Attack(wk, &(Command_Attack_Args){8, 0x1F, 8, -1});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive03_0033(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x110);
}

void Passive03_0034(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x20, &(Command_Attack_Args){8, 0x20, 8, -1});
}

void Passive03_0035(PLW* wk) {
    pattern_normal_attack_4(wk, 8, 0xB, 0x202);
}

void Passive03_0036(PLW* wk) {
    pattern_normal_attack_command_attack_3(wk, 8, 0xB, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive03_0037(PLW* wk) {
    active_pattern_em_term_lever_attack(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0, 0x110);
}

void Passive03_0038(PLW* wk) {
    pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FF0, 6, 1, -1}, 0x110);
}

void Passive03_0039(PLW* wk) {
    pattern_etc_term_branch_unit_area(wk, 2, 0x42, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5A, 0x5B});
}

void Passive03_0040(PLW* wk) {
    pattern_em_term_normal_attack_7(wk, &(EM_Term_Params){-0x7FA0, -0x7FF8, 6, 1, -1}, 9, 0x400);
}

void Passive03_0041(PLW* wk) {
    pattern_em_term_lever_attack_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 8, 0x42);
}

void Passive03_0042(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7FA0, -1, 8, 0x8400, 2, -0x7FA0, -1, -0x7C00});
}

void Passive03_0043(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FD8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x20, 9, -1}
    );
}

void Passive03_0044(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x39,
        &(EM_Term_Params){-1, -0x7FD0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive03_0045(PLW* wk) {
    pattern_em_term_branch_unit_area(
        wk,
        &(EM_Term_Params){-1, -0x7FE0, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0x5B}
    );
}

void Passive03_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive03_0047(PLW* wk) {
    pattern_normal_attack_branch_unit_area_2(wk, 0xB, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 0x5B});
}

void Passive03_0048(PLW* wk) {
    pattern_normal_attack_2(wk, 8, 0x102, 0x402);
}

void (*const Passive03_Tbl[136])(PLW*) = {
    Passive03_0000, Passive03_0001, Passive03_0002, Passive03_0003, Passive03_0004, Passive03_0005, Passive03_0006,
    Passive03_0007, Passive03_0008, Passive03_0009, Passive03_0010, Passive03_0011, Passive03_0012, Passive03_0013,
    Passive03_0014, Passive03_0015, Passive03_0016, Passive03_0017, Passive03_0018, Passive03_0019, Passive03_0020,
    Passive03_0021, Passive03_0022, Passive03_0023, Passive03_0024, Passive03_0025, Passive03_0026, Passive03_0027,
    Passive03_0028, Passive03_0029, Passive03_0030, Passive03_0031, Passive03_0032, Passive03_0033, Passive03_0034,
    Passive03_0035, Passive03_0036, Passive03_0037, Passive03_0038, Passive03_0039, Passive03_0040, Passive03_0041,
    Passive03_0042, Passive03_0043, Passive03_0044, Passive03_0045, Passive03_0046, Passive03_0047, Passive03_0048,
    Passive03_0049, Passive03_0050, Passive03_0051, Passive03_0052, Passive03_0053, Passive03_0054, Passive03_0055,
    Passive03_0056, Passive03_0057, Passive03_0058, Passive03_0059, Passive03_0060, Passive03_0061, Passive03_0062,
    Passive03_0063, Passive03_0064, Passive03_0065, Passive03_0066, Passive03_0067, Passive03_0068, Passive03_0069,
    Passive03_0070, Passive03_0071, Passive03_0072, Passive03_0073, Passive03_0074, Passive03_0075, Passive03_0076,
    Passive03_0077, Passive03_0078, Passive03_0079, Passive03_0080, Passive03_0081, Passive03_0082, Passive03_0083,
    Passive03_0084, Passive03_0085, Passive03_0086, Passive03_0087, Passive03_0088, Passive03_0089, Passive03_0090,
    Passive03_0091, Passive03_0092, Passive03_0093, Passive03_0094, Passive03_0095, Passive03_0096, Passive03_0097,
    Passive03_0098, Passive03_0099, Passive03_0100, Passive03_0101, Passive03_0102, Passive03_0103, Passive03_0104,
    Passive03_0105, Passive03_0106, Passive03_0107, Passive03_0108, Passive03_0109, Passive03_0110, Passive03_0111,
    Passive03_0112, Passive03_0113, Passive03_0114, Passive03_0115, Passive03_0116, Passive03_0117, Passive03_0118,
    Passive03_0119, Passive03_0120, Passive03_0121, Passive03_0122, Passive03_0123, Passive03_0124, Passive03_0125,
    Passive03_0126, Passive03_0127, Passive03_0128, Passive03_0129, Passive03_0130, Passive03_0131, Passive03_0132,
    Passive03_0133, Passive03_0134, Passive03_0135
};
