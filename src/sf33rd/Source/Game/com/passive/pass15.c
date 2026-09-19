/**
 * @file pass15.c
 * COM Passive: Chun-Li
 */

#include "sf33rd/Source/Game/com/passive/pass15.h"
#include "sf33rd/Source/Game/com/passive/pass15_internal.h"
#include "sf33rd/Source/Game/com/patterns/com_patterns.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/workuser.h"

void (*const Passive15_Tbl[164])();

void Passive15(PLW* wk) {
    Passive15_Tbl[(s16)Pattern_Index[wk->wu.id]](wk);
}

void Passive15_0000(PLW* wk) {
    active_pattern_normal_attack(wk, 0xD, M_Lv[wk->wu.id]);
}

void Passive15_0001(PLW* wk) {
    active_pattern_lever_off_look(wk);
}

void Passive15_0002(PLW* wk) {
    pattern_wait_get_up_pierce_on_command_attack(
        wk,
        &(Command_Attack_Args){8, 0, 0xB, -1},
        &(EM_Term_Params){0x7FFF, -1, 1, 1, -1}
    );
}

void Passive15_0003(PLW* wk) {
    pattern_vs_jump_guard(wk);
}

void Passive15_0004(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x53, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive15_0005(PLW* wk) {
    pattern_forced_guard(wk, 0);
}

void Passive15_0006(PLW* wk) {
    pattern_approach_walk_em_term(wk, 0x7F, &(EM_Term_Params){0x7FFF, -1, 1, 1, -1});
}

void Passive15_0007(PLW* wk) {
    active_pattern_approach_walk_command_attack(wk, 0x7F, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive15_0008(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive15_0009(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive15_0010(PLW* wk) {
    pattern_short_range_attack(wk);
}

void Passive15_0011(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-1, -0x7FB0, 0xB, 0x20, 2, -0x7FA0, -1, 0x20});
}

void Passive15_0012(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x400);
}

void Passive15_0013(PLW* wk) {
    pattern_search_back_term_command_attack(wk, 0x60, 0x6C, &(Command_Attack_Args){8, 1, 0xB, -1});
}

void Passive15_0014(PLW* wk) {
    active_pattern_branch_unit_area(wk, &(Branch_Menu_Args){6, 0x59, 0x5A, 0x5B, 1});
}

void Passive15_0015(PLW* wk) {
    pattern_wait_get_up_2(wk, 0);
}

void Passive15_0016(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away(wk, 0x6C, 0x89, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
}

void Passive15_0017(PLW* wk) {
    pattern_wait_get_up_normal_attack_2(wk, 0x200);
}

void Passive15_0018(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_2(wk, 0x89, &(Branch_Menu_Args){6, 0x69, 0x6A, 1, 1});
}

void Passive15_0019(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive15_0020(PLW* wk) {
    pattern_wait_get_up_2(wk, 3);
}

void Passive15_0021(PLW* wk) {
    pattern_wait_get_up_command_attack(wk, 0, -1, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive15_0022(PLW* wk) {
    pattern_wait_get_up_jump_attack_term_normal_attack_2(
        wk,
        &(Jump_Term_Args){-0x7F80, -0x7FB0, 0xB, 0x100, 0, -0x7FA0, -1, 0x200},
        8,
        0x200
    );
}

void Passive15_0023(PLW* wk) {
    pattern_jump_attack_term_wait_get_up_normal_attack(
        wk,
        &(Jump_Term_Args){-0x7F60, -0x7FB0, 0xB, 0x20, 0, -0x7FA0, -1, 0x200},
        0x200
    );
}

void Passive15_0024(PLW* wk) {
    pattern_search_back_term_pierce_on_keep_away_3(wk, 0x89);
}

void Passive15_0025(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x60, 6, 0x6C);
        break;

    case 1:
        Pierce_On(wk);
        break;

    case 2:
        Keep_Away(wk, 0x89, 0);
        break;

    case 3:
        Wait_Get_Up(wk, 3, -1);
        break;

    case 4:
        Normal_Attack(wk, 8, 0x10);
        break;

    case 5:
        Normal_Attack(wk, 8, 0x200);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0026(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-1, -0x7FB0, 6, 1, -1}, 8, 0x42);
}

void Passive15_0027(PLW* wk) {
    pattern_command_attack_normal_attack(wk, &(Command_Attack_Args){8, 0x1E, 8, -1});
}

void Passive15_0028(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 10, 0x400, 1});
}

void Passive15_0029(PLW* wk) {
    pattern_normal_attack_3(wk, 0x10, 8, 0x40);
}

void Passive15_0030(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1}, 8, 0x400);
}

void Passive15_0031(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        Search_Back_Term(wk, 0x40, 6, 0x6C);
        break;

    case 1:
        Walk(wk, 1, 0x20, 1);
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0032(PLW* wk) {
    pattern_sa_term_com_random_select(
        wk,
        &(SA_Term_Args){0xFFFF, 0x2F, 0x30, 0},
        &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E},
        1
    );
}

void Passive15_0033(PLW* wk) {
    active_pattern_com_random_select(wk, &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}, 1);
}

void Passive15_0034(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xC, 0x10, &(Command_Attack_Args){8, 0x1E, 10, -1});
}

void Passive15_0035(PLW* wk) {
    active_pattern_normal_attack_6(wk, 0x10, 0x102, 0x200);
}

void Passive15_0036(PLW* wk) {
    pattern_command_attack_com_random_select(
        wk,
        &(Command_Attack_Args){0xB, 0x1E, 10, -1},
        &(Branch_Menu_Args){6, 0x42, 0x46, 0x4A, 0x4A}
    );
}

void Passive15_0037(PLW* wk) {
    pattern_em_term_com_random_select(
        wk,
        &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1},
        &(Branch_Menu_Args){6, 0x9D, 0x9D, 0x9E, 0x9E}
    );
}

void Passive15_0038(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-1, -0x7F80, 6, 1, -1});
        break;

    case 1:
        Rapid_Command_Attack(wk, &(Rapid_Command_Args){8, 0x4D, 0x400, 0x78});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0039(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-1, -0x7FC8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 10, -1}
    );
}

void Passive15_0040(PLW* wk) {
    pattern_em_term_normal_attack_7(wk, &(EM_Term_Params){-1, -0x7FF8, 6, 1, -1}, 0xB, 0x200);
}

void Passive15_0041(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -0x7FC0, 6, 1, -1}, 0xB, 0x40);
}

void Passive15_0042(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x202, 2});
}

void Passive15_0043(PLW* wk) {
    active_pattern_em_term_command_attack(
        wk,
        &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1E, 9, -1}
    );
}

void Passive15_0044(PLW* wk) {
    pattern_approach_walk_em_term_command_attack(
        wk,
        0x4B,
        &(EM_Term_Params){-1, -0x7FC0, 6, 1, -1},
        &(Command_Attack_Args){8, 0x1F, 9, -1}
    );
}

void Passive15_0045(PLW* wk) {
    switch (CP_Index[wk->wu.id][0]) {
    case 0:
        EM_Term(wk, &(EM_Term_Params){-0x7F80, -0x7FB8, 6, 1, -1});
        break;

    case 1:
        Jump_Attack(wk, &(Jump_Attack_Args){8, 6, 0x202, 0});
        break;

    default:
        End_Pattern(wk);
        break;
    }
}

void Passive15_0046(PLW* wk) {
    pattern_keep_status(wk, -1);
}

void Passive15_0047(PLW* wk) {
    active_pattern_normal_attack_command_attack(wk, 0xB, 0x102, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void Passive15_0048(PLW* wk) {
    pattern_normal_attack_3(wk, 0x102, 8, 0x402);
}

void Passive15_0049(PLW* wk) {
    pattern_em_term_adjust_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1});
}

void Passive15_0050(PLW* wk) {
    active_pattern_em_term_normal_attack(wk, &(EM_Term_Params){-0x7F80, -1, 6, 1, -1}, 8, 0x200);
}

void Passive15_0051(PLW* wk) {
    pattern_normal_attack_3(wk, 0x220, 0xB, 0x102);
}

void Passive15_0052(PLW* wk) {
    active_pattern_normal_attack(wk, 8, 0x202);
}

void Passive15_0053(PLW* wk) {
    pattern_lever_attack(wk, 0, 0x402);
}

void Passive15_0054(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1F, 10, -1});
}

void Passive15_0055(PLW* wk) {
    active_pattern_jump_attack(wk, &(Jump_Attack_Args){8, 0xC, 0x202, 0});
}

void Passive15_0056(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x200, 0, -0x7FA0, -1, 0x20});
}

void Passive15_0057(PLW* wk) {
    active_pattern_jump_attack_term(wk, &(Jump_Term_Args){-0x7F60, -0x7FA0, 8, 0x20, 0, -0x7FA0, -1, 0x20});
}

void Passive15_0058(PLW* wk) {
    pattern_keep_status(wk, 1);
}

void Passive15_0059(PLW* wk) {
    active_pattern_command_attack(wk, &(Command_Attack_Args){8, 0x1E, 9, -1});
}

void (*const Passive15_Tbl[164])(PLW*) = {
    Passive15_0000, Passive15_0001, Passive15_0002, Passive15_0003, Passive15_0004, Passive15_0005, Passive15_0006,
    Passive15_0007, Passive15_0008, Passive15_0009, Passive15_0010, Passive15_0011, Passive15_0012, Passive15_0013,
    Passive15_0014, Passive15_0015, Passive15_0016, Passive15_0017, Passive15_0018, Passive15_0019, Passive15_0020,
    Passive15_0021, Passive15_0022, Passive15_0023, Passive15_0024, Passive15_0025, Passive15_0026, Passive15_0027,
    Passive15_0028, Passive15_0029, Passive15_0030, Passive15_0031, Passive15_0032, Passive15_0033, Passive15_0034,
    Passive15_0035, Passive15_0036, Passive15_0037, Passive15_0038, Passive15_0039, Passive15_0040, Passive15_0041,
    Passive15_0042, Passive15_0043, Passive15_0044, Passive15_0045, Passive15_0046, Passive15_0047, Passive15_0048,
    Passive15_0049, Passive15_0050, Passive15_0051, Passive15_0052, Passive15_0053, Passive15_0054, Passive15_0055,
    Passive15_0056, Passive15_0057, Passive15_0058, Passive15_0059, Passive15_0060, Passive15_0061, Passive15_0062,
    Passive15_0063, Passive15_0064, Passive15_0065, Passive15_0066, Passive15_0067, Passive15_0068, Passive15_0069,
    Passive15_0070, Passive15_0071, Passive15_0072, Passive15_0073, Passive15_0074, Passive15_0075, Passive15_0076,
    Passive15_0077, Passive15_0078, Passive15_0079, Passive15_0080, Passive15_0081, Passive15_0082, Passive15_0083,
    Passive15_0084, Passive15_0085, Passive15_0086, Passive15_0087, Passive15_0088, Passive15_0089, Passive15_0090,
    Passive15_0091, Passive15_0092, Passive15_0093, Passive15_0094, Passive15_0095, Passive15_0096, Passive15_0097,
    Passive15_0098, Passive15_0099, Passive15_0100, Passive15_0101, Passive15_0102, Passive15_0103, Passive15_0104,
    Passive15_0105, Passive15_0106, Passive15_0107, Passive15_0108, Passive15_0109, Passive15_0110, Passive15_0111,
    Passive15_0112, Passive15_0113, Passive15_0114, Passive15_0115, Passive15_0116, Passive15_0117, Passive15_0118,
    Passive15_0119, Passive15_0120, Passive15_0121, Passive15_0122, Passive15_0123, Passive15_0124, Passive15_0125,
    Passive15_0126, Passive15_0127, Passive15_0128, Passive15_0129, Passive15_0130, Passive15_0131, Passive15_0132,
    Passive15_0133, Passive15_0134, Passive15_0135, Passive15_0136, Passive15_0137, Passive15_0138, Passive15_0139,
    Passive15_0140, Passive15_0141, Passive15_0142, Passive15_0143, Passive15_0144, Passive15_0145, Passive15_0146,
    Passive15_0147, Passive15_0148, Passive15_0149, Passive15_0150, Passive15_0151, Passive15_0152, Passive15_0153,
    Passive15_0154, Passive15_0155, Passive15_0156, Passive15_0157, Passive15_0158, Passive15_0159, Passive15_0160,
    Passive15_0161, Passive15_0162, Passive15_0163
};
